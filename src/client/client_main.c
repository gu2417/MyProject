#define WIN32_LEAN_AND_MEAN

#include <conio.h>
#include <locale.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <winsock2.h>
#include <windows.h>

#include "protocol.h"
#include "save_config.h"

#ifdef _MSC_VER
#pragma comment(lib, "Ws2_32.lib")
#endif

/* 클라이언트 화면에서 입력받는 학생 정보를 문자열로 보관합니다. */
typedef struct StudentForm {
    char name[50];
    char student_id[20];
    char department[80];
    char grade[16];
    char gpa[32];
    char email[100];
    char mobile[30];
} StudentForm;

/* 사용자 입력 한 줄을 읽고 마지막 개행 문자를 제거합니다. */
static void read_line(const char *prompt, char *buffer, unsigned int size) {
    printf("%s", prompt);
    if (fgets(buffer, size, stdin) == NULL) {
        buffer[0] = '\0';
        return;
    }
    buffer[strcspn(buffer, "\r\n")] = '\0';
}

/* 비밀번호 입력 시 화면에는 별표만 보이도록 처리합니다. */
static void read_password(const char *prompt, char *buffer, unsigned int size) {
    unsigned int index = 0;
    int ch;

    printf("%s", prompt);
    while ((ch = _getch()) != '\r') {
        if (ch == '\b') {
            if (index > 0) {
                --index;
                printf("\b \b");
            }
        } else if (index + 1 < size) {
            buffer[index++] = (char)ch;
            printf("*");
        }
    }
    buffer[index] = '\0';
    printf("\n");
}

/* 서버 응답이 여러 줄로 와도 한 번에 한 줄씩 Message로 분리합니다. */
static int receive_response(SOCKET socket_fd, Message *response) {
    static char pending[PROTOCOL_BUFFER_SIZE];
    static unsigned int pending_length = 0;
    char line[PROTOCOL_BUFFER_SIZE];
    unsigned int index;

    while (1) {
        for (index = 0; index < pending_length; ++index) {
            if (pending[index] == '\n') {
                unsigned int line_length = index;
                if (line_length >= sizeof(line)) {
                    line_length = sizeof(line) - 1;
                }
                memcpy(line, pending, line_length);
                line[line_length] = '\0';
                memmove(pending, pending + index + 1, pending_length - index - 1);
                pending_length -= index + 1;
                return message_from_text(line, response) == 0;
            }
        }

        if (pending_length >= sizeof(pending) - 1) {
            pending_length = 0;
            return 0;
        }

        {
            int received = recv(socket_fd, pending + pending_length,
                                sizeof(pending) - pending_length - 1, 0);
            if (received <= 0) {
                return 0;
            }
            pending_length += (unsigned int)received;
            pending[pending_length] = '\0';
        }
    }
}

/* 요청을 하나 보내고 응답 하나를 기다립니다. */
static int send_request(SOCKET socket_fd, const char *command, const char *payload, Message *response) {
    Message request;

    message_init(&request, MSG_TYPE_REQ, command, MSG_STATUS_EMPTY, payload);
    if (socket_send_message((unsigned long long)socket_fd, &request) < 0) {
        return 0;
    }

    return receive_response(socket_fd, response);
}

/* 구분 문자를 기준으로 문자열에서 필드를 하나 잘라냅니다. */
static char *cut_at(char **cursor, char delimiter) {
    char *start;
    char *mark;

    if (cursor == NULL || *cursor == NULL) {
        return NULL;
    }

    start = *cursor;
    mark = strchr(start, delimiter);
    if (mark == NULL) {
        *cursor = NULL;
    } else {
        *mark = '\0';
        *cursor = mark + 1;
    }

    return start;
}

/* 서버에서 받은 학생 한 명의 문자열을 화면 처리용 구조체로 바꿉니다. */
static int parse_student_record(const char *payload, StudentForm *student) {
    char temp[MESSAGE_PAYLOAD_SIZE];
    char *cursor;
    char *name;
    char *student_id;
    char *department;
    char *grade;
    char *gpa;
    char *email;
    char *mobile;

    if (payload == NULL || student == NULL) {
        return 0;
    }

    snprintf(temp, sizeof(temp), "%s", payload);
    cursor = temp;
    name = cut_at(&cursor, '\t');
    student_id = cut_at(&cursor, '\t');
    department = cut_at(&cursor, '\t');
    grade = cut_at(&cursor, '\t');
    gpa = cut_at(&cursor, '\t');
    email = cut_at(&cursor, '\t');
    mobile = cut_at(&cursor, '\t');

    if (!(name && student_id && department && grade && gpa && email && mobile)) {
        return 0;
    }

    snprintf(student->name, sizeof(student->name), "%s", name);
    snprintf(student->student_id, sizeof(student->student_id), "%s", student_id);
    snprintf(student->department, sizeof(student->department), "%s", department);
    snprintf(student->grade, sizeof(student->grade), "%s", grade);
    snprintf(student->gpa, sizeof(student->gpa), "%s", gpa);
    snprintf(student->email, sizeof(student->email), "%s", email);
    snprintf(student->mobile, sizeof(student->mobile), "%s", mobile);
    return 1;
}

/* StudentForm 구조체를 서버가 이해하는 탭 구분 문자열로 만듭니다. */
static void build_student_payload(const StudentForm *student, char *payload, unsigned int size) {
    snprintf(payload, size, "%s\t%s\t%s\t%s\t%s\t%s\t%s",
             student->name, student->student_id, student->department,
             student->grade, student->gpa, student->email, student->mobile);
}

/* 검색 결과가 끝났다는 서버 메시지인지 확인합니다. */
static int is_query_end(const Message *response) {
    return strcmp(response->status, MSG_STATUS_OK) == 0 &&
           strncmp(response->payload, "END\t", 4) == 0;
}

/* 서버 응답의 성공 여부를 보기 좋게 출력합니다. */
static void print_response(const Message *response) {
    const char *label = strcmp(response->status, MSG_STATUS_OK) == 0 ? "성공" : "안내";
    printf("[%s] %s\n", label, response->payload);
}

/* 학생 한 명의 정보를 한 줄로 정리해서 출력합니다. */
static void print_student_record(const char *payload) {
    StudentForm student;

    if (parse_student_record(payload, &student)) {
        printf("학번 %s | 이름 %s | 학과 %s | %s학년 | 평점 %s | 이메일 %s | 연락처 %s\n",
               student.student_id, student.name, student.department,
               student.grade, student.gpa, student.email, student.mobile);
    }
}

/* 검색이나 학과 조회처럼 결과가 여러 줄인 요청을 끝까지 출력합니다. */
static void send_query_and_print(SOCKET socket_fd, const char *command, const char *payload) {
    Message request;

    message_init(&request, MSG_TYPE_REQ, command, MSG_STATUS_EMPTY, payload);
    if (socket_send_message((unsigned long long)socket_fd, &request) < 0) {
        printf("서버와 연결이 끊겼습니다. 서버가 실행 중인지 확인해주세요.\n");
        return;
    }

    while (1) {
        Message response;

        if (!receive_response(socket_fd, &response)) {
            printf("서버와 연결이 끊겼습니다. 서버가 실행 중인지 확인해주세요.\n");
            return;
        }

        if (strcmp(response.status, MSG_STATUS_FAIL) == 0) {
            print_response(&response);
            return;
        }

        if (is_query_end(&response)) {
            printf("검색된 학생 수: %s명\n", response.payload + 4);
            return;
        }

        print_student_record(response.payload);
    }
}

/* 학번으로 학생 한 명을 찾아 수정 메뉴에서 사용할 현재 값을 가져옵니다. */
static int fetch_student_by_id(SOCKET socket_fd, const char *student_id, StudentForm *student) {
    Message request;

    message_init(&request, MSG_TYPE_REQ, CMD_STUDENT_SEARCH, MSG_STATUS_EMPTY, student_id);
    if (socket_send_message((unsigned long long)socket_fd, &request) < 0) {
        return 0;
    }

    while (1) {
        Message response;

        if (!receive_response(socket_fd, &response)) {
            return 0;
        }

        if (strcmp(response.status, MSG_STATUS_FAIL) == 0) {
            print_response(&response);
            return 0;
        }

        if (is_query_end(&response)) {
            return 0;
        }

        if (parse_student_record(response.payload, student) &&
            strcmp(student->student_id, student_id) == 0) {
            while (receive_response(socket_fd, &response)) {
                if (is_query_end(&response)) {
                    break;
                }
            }
            return 1;
        }
    }
}

/* 수정 메뉴에서 Enter를 누르면 기존 값을 유지하도록 입력을 처리합니다. */
static void read_optional_field(const char *label, char *value, unsigned int size) {
    char prompt[160];
    char input[160];

    snprintf(prompt, sizeof(prompt), "%s [%s]: ", label, value);
    read_line(prompt, input, sizeof(input));
    if (input[0] != '\0') {
        snprintf(value, size, "%s", input);
    }
}

/* 로그인 정보를 서버에 보내고 성공 여부를 반환합니다. */
static int login(SOCKET socket_fd) {
    char id[40];
    char password[80];
    char payload[MESSAGE_PAYLOAD_SIZE];
    Message response;

    read_line("아이디: ", id, sizeof(id));
    read_password("비밀번호: ", password, sizeof(password));
    snprintf(payload, sizeof(payload), "%s\t%s", id, password);

    if (!send_request(socket_fd, CMD_LOGIN, payload, &response)) {
        printf("서버와 연결이 끊겼습니다. 서버가 실행 중인지 확인해주세요.\n");
        return 0;
    }

    print_response(&response);
    return strcmp(response.status, MSG_STATUS_OK) == 0;
}

/* 새 사용자 정보를 입력받아 서버에 추가 요청을 보냅니다. */
static void add_user(SOCKET socket_fd) {
    char id[40];
    char password[80];
    char department[80];
    char name[50];
    char payload[MESSAGE_PAYLOAD_SIZE];
    Message response;

    read_line("새 사용자 아이디: ", id, sizeof(id));
    read_password("새 비밀번호: ", password, sizeof(password));
    read_line("담당 학과: ", department, sizeof(department));
    read_line("이름: ", name, sizeof(name));

    snprintf(payload, sizeof(payload), "%s\t%s\t%s\t%s", id, password, department, name);
    if (send_request(socket_fd, CMD_USER_ADD, payload, &response)) {
        print_response(&response);
    }
}

/* 삭제할 사용자 아이디를 입력받아 서버에 삭제 요청을 보냅니다. */
static void delete_user(SOCKET socket_fd) {
    char id[40];
    Message response;

    read_line("삭제할 사용자 아이디: ", id, sizeof(id));
    if (send_request(socket_fd, CMD_USER_DELETE, id, &response)) {
        print_response(&response);
    }
}

/* 학생 추가에 필요한 모든 항목을 입력받아 payload를 만듭니다. */
static void read_student_payload(char *payload, unsigned int size) {
    StudentForm student;

    read_line("이름: ", student.name, sizeof(student.name));
    read_line("학번: ", student.student_id, sizeof(student.student_id));
    read_line("학과: ", student.department, sizeof(student.department));
    read_line("학년: ", student.grade, sizeof(student.grade));
    read_line("평점: ", student.gpa, sizeof(student.gpa));
    read_line("이메일: ", student.email, sizeof(student.email));
    read_line("휴대폰 번호: ", student.mobile, sizeof(student.mobile));

    build_student_payload(&student, payload, size);
}

/* 새 학생 정보를 입력받아 서버에 추가 요청을 보냅니다. */
static void add_student(SOCKET socket_fd) {
    char payload[MESSAGE_PAYLOAD_SIZE];
    Message response;

    read_student_payload(payload, sizeof(payload));
    if (send_request(socket_fd, CMD_STUDENT_ADD, payload, &response)) {
        print_response(&response);
    }
}

/* 학번으로 기존 학생을 찾고, 바꿀 항목만 입력받아 수정합니다. */
static void update_student(SOCKET socket_fd) {
    char student_id[20];
    char payload[MESSAGE_PAYLOAD_SIZE];
    StudentForm student;
    Message response;

    read_line("수정할 학생 학번: ", student_id, sizeof(student_id));
    if (student_id[0] == '\0') {
        printf("학번을 입력해주세요.\n");
        return;
    }

    if (!fetch_student_by_id(socket_fd, student_id, &student)) {
        printf("해당 학번의 학생을 찾을 수 없습니다.\n");
        return;
    }

    printf("현재 정보입니다. 바꾸지 않을 항목은 Enter를 누르세요.\n");
    build_student_payload(&student, payload, sizeof(payload));
    print_student_record(payload);
    read_optional_field("이름", student.name, sizeof(student.name));
    read_optional_field("학과", student.department, sizeof(student.department));
    read_optional_field("학년", student.grade, sizeof(student.grade));
    read_optional_field("평점", student.gpa, sizeof(student.gpa));
    read_optional_field("이메일", student.email, sizeof(student.email));
    read_optional_field("휴대폰 번호", student.mobile, sizeof(student.mobile));

    build_student_payload(&student, payload, sizeof(payload));
    if (send_request(socket_fd, CMD_STUDENT_UPDATE, payload, &response)) {
        print_response(&response);
    }
}

/* 학번을 입력받아 서버에 학생 삭제 요청을 보냅니다. */
static void delete_student(SOCKET socket_fd) {
    char student_id[20];
    Message response;

    read_line("삭제할 학생 학번: ", student_id, sizeof(student_id));
    if (send_request(socket_fd, CMD_STUDENT_DELETE, student_id, &response)) {
        print_response(&response);
    }
}

/* 이름이나 학번 검색어를 입력받아 학생 검색 요청을 보냅니다. */
static void search_student(SOCKET socket_fd) {
    char keyword[80];

    read_line("검색어: ", keyword, sizeof(keyword));
    send_query_and_print(socket_fd, CMD_STUDENT_SEARCH, keyword);
}

/* 학과명을 반복해서 입력받고, 0을 입력하면 메뉴로 돌아갑니다. */
static void list_department(SOCKET socket_fd) {
    char department[80];

    while (1) {
        read_line("학과명(0 입력 시 메뉴로 돌아가기): ", department, sizeof(department));
        if (strcmp(department, "0") == 0) {
            return;
        }
        if (department[0] == '\0') {
            printf("학과명을 입력해주세요.\n");
            continue;
        }

        send_query_and_print(socket_fd, CMD_DEPT_LIST, department);
    }
}

/* 로그인한 사용자가 선택하는 전체 메뉴를 담당합니다. */
static void main_menu(SOCKET socket_fd) {
    while (1) {
        char choice[16];

        printf("\n[학생 관리]\n");
        printf("1. 사용자 추가\n");
        printf("2. 사용자 삭제\n");
        printf("3. 학생 검색\n");
        printf("4. 학생 추가\n");
        printf("5. 학생 정보 수정\n");
        printf("6. 학생 삭제\n");
        printf("7. 학과별 학생 보기\n");
        printf("8. 종료\n");
        read_line("> ", choice, sizeof(choice));

        switch (atoi(choice)) {
        case 1:
            add_user(socket_fd);
            break;
        case 2:
            delete_user(socket_fd);
            break;
        case 3:
            search_student(socket_fd);
            break;
        case 4:
            add_student(socket_fd);
            break;
        case 5:
            update_student(socket_fd);
            break;
        case 6:
            delete_student(socket_fd);
            break;
        case 7:
            list_department(socket_fd);
            break;
        case 8: {
            Message response;
            send_request(socket_fd, CMD_QUIT, "", &response);
            return;
        }
        default:
            printf("메뉴 번호를 다시 입력해주세요.\n");
            break;
        }
    }
}

/* 클라이언트 소켓을 준비하고 서버에 접속한 뒤 메뉴를 실행합니다. */
int main(void) {
    WSADATA wsa_data;
    SOCKET socket_fd;
    SOCKADDR_IN server_addr;

    SetConsoleOutputCP(CP_UTF8);
    SetConsoleCP(CP_UTF8);
    setlocale(LC_ALL, ".UTF-8");

    if (WSAStartup(MAKEWORD(2, 2), &wsa_data) != 0) {
        printf("네트워크 준비에 실패했습니다.\n");
        return 1;
    }

    socket_fd = socket(PF_INET, SOCK_STREAM, 0);
    if (socket_fd == INVALID_SOCKET) {
        printf("서버 연결을 준비하지 못했습니다.\n");
        WSACleanup();
        return 1;
    }

    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = inet_addr("127.0.0.1");
    server_addr.sin_port = htons(SERVER_PORT);

    if (connect(socket_fd, (SOCKADDR *)&server_addr, sizeof(server_addr)) == SOCKET_ERROR) {
        printf("서버에 연결하지 못했습니다. 먼저 server.exe를 실행해주세요.\n");
        closesocket(socket_fd);
        WSACleanup();
        return 1;
    }

    {
        int save_hour;
        int save_minute;

        /* 클라이언트 화면에도 서버와 같은 자동 저장 시간을 안내합니다. */
        read_save_time(&save_hour, &save_minute);
        printf("학생 관리 서버에 연결되었습니다.\n");
        printf("자동 저장 시간: %02d:%02d\n", save_hour, save_minute);
    }
    fflush(stdout);
    while (!login(socket_fd)) {
        printf("다시 입력해주세요.\n");
    }

    main_menu(socket_fd);
    closesocket(socket_fd);
    WSACleanup();
    return 0;
}
