#define WIN32_LEAN_AND_MEAN

#include <process.h>
#include <locale.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <winsock2.h>
#include <windows.h>

#include "file_io.h"
#include "protocol.h"
#include "request_handler.h"
#include "save_config.h"
#include "scheduler.h"
#include "server_state.h"

#ifdef _MSC_VER
#pragma comment(lib, "Ws2_32.lib")
#endif

/* 서버 전체에서 공유하는 사용자 목록, 학생 목록, 실행 상태입니다. */
static ServerState g_state;

/* 서버 실행 중 치명적인 오류가 발생하면 오류 번호를 출력하고 종료합니다. */
static void error_exit(const char *message) {
    fprintf(stderr, "%s 오류 번호: %d\n", message, WSAGetLastError());
    exit(1);
}

/* 로그에 찍을 시간을 사람이 읽기 쉬운 형식으로 출력합니다. */
static void print_time_prefix(const char *label, time_t now) {
    struct tm *local_time = localtime(&now);

    if (local_time == NULL) {
        printf("[%s] 시간 확인 실패: ", label);
        return;
    }

    printf("[%s %04d-%02d-%02d %02d:%02d] ", label,
           local_time->tm_year + 1900, local_time->tm_mon + 1,
           local_time->tm_mday, local_time->tm_hour, local_time->tm_min);
}

/* 학생 연결 리스트의 현재 내용을 SMU_Students.txt에 저장합니다. */
static int save_student_data(ServerState *state) {
    int result;

    WaitForSingleObject(state->data_mutex, INFINITE);
    result = save_students(STUDENTS_FILE_PATH, state->students);
    ReleaseMutex(state->data_mutex);

    return result;
}

/* 저장 시각이 되면 학생 정보를 파일에 저장하는 백그라운드 작업입니다. */
static unsigned WINAPI scheduler_thread(void *arg) {
    ServerState *state = (ServerState *)arg;
    int last_logged_hour;
    int last_logged_minute;

    /* 처음 읽은 저장 시간을 기억해 두고, 나중에 바뀌면 로그로 알려줍니다. */
    read_save_time(&last_logged_hour, &last_logged_minute);

    while (state->running) {
        time_t now = time(NULL);
        int save_hour;
        int save_minute;

        read_save_time(&save_hour, &save_minute);

        /* save_config.h가 저장 중에 바뀌면 서버 로그에도 새 시간을 출력합니다. */
        if (save_hour != last_logged_hour || save_minute != last_logged_minute) {
            printf("자동 저장 시간 변경: %02d:%02d\n", save_hour, save_minute);
            fflush(stdout);
            last_logged_hour = save_hour;
            last_logged_minute = save_minute;
        }

        if (is_daily_save_due(now, save_hour, save_minute, &state->last_saved_yday)) {
            mark_daily_save_done(now, &state->last_saved_yday);
            if (save_student_data(state) == 0) {
                print_time_prefix("자동 저장 성공", now);
                printf("SMU_Students.txt에 학생 정보를 저장했습니다.\n");
            } else {
                print_time_prefix("자동 저장 실패", now);
                printf("SMU_Students.txt 저장에 실패했습니다.\n");
            }
            fflush(stdout);
        }

        Sleep(1000);
    }

    return 0;
}

/* 클라이언트 한 명의 요청을 반복해서 받고, 처리 결과를 다시 보냅니다. */
static unsigned WINAPI client_thread(void *arg) {
    ClientContext *client = (ClientContext *)arg;
    char buffer[PROTOCOL_BUFFER_SIZE];

    while (1) {
        int received = recv(client->socket, buffer, sizeof(buffer) - 1, 0);
        Message request;
        Message response;

        if (received <= 0) {
            break;
        }

        buffer[received] = '\0';
        if (message_from_text(buffer, &request) != 0) {
            message_init(&response, MSG_TYPE_RES, "UNKNOWN", MSG_STATUS_FAIL,
                         "받은 요청을 읽을 수 없습니다.");
        } else {
            printf("[요청 받음] 명령: %s, 내용: %s\n", request.command, request.payload);
            if (!handle_request(client, &request, &response)) {
                continue;
            }
        }

        socket_send_message((unsigned long long)client->socket, &response);
        if (strcmp(response.command, CMD_QUIT) == 0) {
            break;
        }
    }

    closesocket(client->socket);
    free(client);
    return 0;
}

/* 서버 시작 시 사용자와 학생 데이터를 파일에서 읽어 초기 상태를 만듭니다. */
static void initialize_state(ServerState *state) {
    memset(state, 0, sizeof(*state));
    state->data_mutex = CreateMutex(NULL, FALSE, NULL);
    state->last_saved_yday = -1;
    state->running = 1;

    if (load_users(USERS_FILE_PATH, &state->users) != 0) {
        printf("사용자 파일을 불러오지 못했습니다: %s\n", USERS_FILE_PATH);
    }
    if (load_students(STUDENTS_FILE_PATH, &state->students) < 0) {
        printf("학생 파일을 불러오지 못했습니다: %s\n", STUDENTS_FILE_PATH);
    }

    printf("불러온 사용자 수: %u명\n", (unsigned int)state->users.count);
    printf("불러온 학생 수: %d명\n", student_list_count(state->students));
    {
        int save_hour;
        int save_minute;

        /* 서버 시작 로그에 현재 설정된 자동 저장 시간을 보여줍니다. */
        read_save_time(&save_hour, &save_minute);
        printf("자동 저장 시간: %02d:%02d\n", save_hour, save_minute);
    }
    fflush(stdout);
}

/* 서버 소켓을 열고 클라이언트 접속을 계속 기다립니다. */
int main(void) {
    WSADATA wsa_data;
    SOCKET server_socket;
    SOCKADDR_IN server_addr;
    HANDLE scheduler;

    SetConsoleOutputCP(CP_UTF8);
    SetConsoleCP(CP_UTF8);
    setlocale(LC_ALL, ".UTF-8");

    initialize_state(&g_state);

    if (WSAStartup(MAKEWORD(2, 2), &wsa_data) != 0) {
        error_exit("네트워크 준비 실패");
    }

    server_socket = socket(PF_INET, SOCK_STREAM, 0);
    if (server_socket == INVALID_SOCKET) {
        error_exit("서버 소켓 생성 실패");
    }

    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    server_addr.sin_port = htons(SERVER_PORT);

    if (bind(server_socket, (SOCKADDR *)&server_addr, sizeof(server_addr)) == SOCKET_ERROR) {
        error_exit("포트 사용 실패");
    }
    if (listen(server_socket, 10) == SOCKET_ERROR) {
        error_exit("접속 대기 실패");
    }

    scheduler = (HANDLE)_beginthreadex(NULL, 0, scheduler_thread, &g_state, 0, NULL);
    if (scheduler != NULL) {
        CloseHandle(scheduler);
    }

    printf("학생 관리 서버가 실행 중입니다. 포트: %d\n", SERVER_PORT);
    fflush(stdout);

    while (1) {
        SOCKADDR_IN client_addr;
        int client_addr_size = sizeof(client_addr);
        SOCKET client_socket = accept(server_socket, (SOCKADDR *)&client_addr, &client_addr_size);
        ClientContext *client;
        HANDLE thread;

        if (client_socket == INVALID_SOCKET) {
            continue;
        }

        printf("클라이언트가 접속했습니다: %s\n", inet_ntoa(client_addr.sin_addr));
        client = (ClientContext *)calloc(1, sizeof(ClientContext));
        if (client == NULL) {
            closesocket(client_socket);
            continue;
        }

        client->socket = client_socket;
        client->state = &g_state;

        thread = (HANDLE)_beginthreadex(NULL, 0, client_thread, client, 0, NULL);
        if (thread == NULL) {
            closesocket(client_socket);
            free(client);
        } else {
            CloseHandle(thread);
        }
    }

    g_state.running = 0;
    student_list_free(g_state.students);
    CloseHandle(g_state.data_mutex);
    closesocket(server_socket);
    WSACleanup();
    return 0;
}
