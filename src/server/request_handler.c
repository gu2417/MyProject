#include "request_handler.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "file_io.h"
#include "save_config.h"

/* 탭으로 나뉜 요청 데이터에서 다음 필드를 하나 꺼냅니다. */
static char *next_field(char **cursor) {
    char *start;
    char *tab;

    if (cursor == NULL || *cursor == NULL) {
        return NULL;
    }

    start = *cursor;
    tab = strchr(start, '\t');
    if (tab == NULL) {
        *cursor = NULL;
    } else {
        *tab = '\0';
        *cursor = tab + 1;
    }

    return start;
}

/* 사용자 추가 요청의 내용을 User 구조체로 변환합니다. */
static int read_user_data(const char *payload, User *user) {
    char temp[MESSAGE_PAYLOAD_SIZE];
    char *cursor;
    char *user_id;
    char *password;
    char *department;
    char *name;

    snprintf(temp, sizeof(temp), "%s", payload ? payload : "");
    cursor = temp;
    user_id = next_field(&cursor);
    password = next_field(&cursor);
    department = next_field(&cursor);
    name = next_field(&cursor);

    if (user_id == NULL || password == NULL || department == NULL || name == NULL ||
        user_id[0] == '\0' || password[0] == '\0' || department[0] == '\0' ||
        name[0] == '\0') {
        return 0;
    }

    snprintf(user->user_id, sizeof(user->user_id), "%s", user_id);
    snprintf(user->password, sizeof(user->password), "%s", password);
    snprintf(user->department, sizeof(user->department), "%s", department);
    snprintf(user->name, sizeof(user->name), "%s", name);
    return 1;
}

/* 학생 추가나 수정 요청의 내용을 Student 구조체로 변환합니다. */
static int read_student_data(const char *payload, Student *student) {
    char temp[MESSAGE_PAYLOAD_SIZE];
    char *cursor;
    char *name;
    char *student_id;
    char *department;
    char *grade_text;
    char *gpa_text;
    char *email;
    char *mobile_phone;

    snprintf(temp, sizeof(temp), "%s", payload ? payload : "");
    cursor = temp;
    name = next_field(&cursor);
    student_id = next_field(&cursor);
    department = next_field(&cursor);
    grade_text = next_field(&cursor);
    gpa_text = next_field(&cursor);
    email = next_field(&cursor);
    mobile_phone = next_field(&cursor);

    if (name == NULL || student_id == NULL || department == NULL ||
        grade_text == NULL || gpa_text == NULL || email == NULL ||
        mobile_phone == NULL || name[0] == '\0' || student_id[0] == '\0' ||
        department[0] == '\0' || email[0] == '\0' || mobile_phone[0] == '\0') {
        return 0;
    }

    snprintf(student->name, sizeof(student->name), "%s", name);
    snprintf(student->student_id, sizeof(student->student_id), "%s", student_id);
    snprintf(student->department, sizeof(student->department), "%s", department);
    student->grade = atoi(grade_text);
    student->gpa = atof(gpa_text);
    snprintf(student->email, sizeof(student->email), "%s", email);
    snprintf(student->mobile_phone, sizeof(student->mobile_phone), "%s", mobile_phone);
    student->next = NULL;
    return 1;
}

/* 학생 변경은 메모리에 먼저 반영되고, 파일 저장은 정해진 시각에 수행됨을 안내합니다. */
static void make_scheduled_save_message(char *buffer, unsigned int size,
                                        const char *action) {
    int save_hour;
    int save_minute;

    /* 응답을 만들 때마다 현재 save_config.h의 저장 시간을 읽어 옵니다. */
    read_save_time(&save_hour, &save_minute);
    snprintf(buffer, size,
             "%s 연결 리스트에는 바로 반영되며, 파일은 %02d:%02d에 저장됩니다.",
             action, save_hour, save_minute);
}

/* 로그인하지 않은 사용자가 기능을 쓰지 못하도록 공통으로 검사합니다. */
static void require_login(ClientContext *client, Message *response, const char *command) {
    if (!client->logged_in) {
        message_init(response, MSG_TYPE_RES, command, MSG_STATUS_FAIL, "먼저 로그인해주세요.");
    }
}

/* 로그인 요청을 처리하고 클라이언트의 로그인 상태를 저장합니다. */
static void handle_login(ClientContext *client, const Message *request, Message *response) {
    char temp[MESSAGE_PAYLOAD_SIZE];
    char *cursor;
    char *user_id;
    char *password;
    int result;

    snprintf(temp, sizeof(temp), "%s", request->payload);
    cursor = temp;
    user_id = next_field(&cursor);
    password = next_field(&cursor);

    if (user_id == NULL || password == NULL) {
        message_init(response, MSG_TYPE_RES, CMD_LOGIN, MSG_STATUS_FAIL,
                     "아이디와 비밀번호를 모두 입력해주세요.");
        return;
    }

    WaitForSingleObject(client->state->data_mutex, INFINITE);
    result = user_check_login(&client->state->users, user_id, password);
    ReleaseMutex(client->state->data_mutex);

    if (result == 0) {
        client->logged_in = 1;
        snprintf(client->user_id, sizeof(client->user_id), "%s", user_id);
        message_init(response, MSG_TYPE_RES, CMD_LOGIN, MSG_STATUS_OK, "로그인되었습니다.");
    } else if (result == 1) {
        message_init(response, MSG_TYPE_RES, CMD_LOGIN, MSG_STATUS_FAIL,
                     "아이디를 찾을 수 없습니다.");
    } else {
        message_init(response, MSG_TYPE_RES, CMD_LOGIN, MSG_STATUS_FAIL,
                     "비밀번호가 맞지 않습니다.");
    }
}

/* 새 사용자 정보를 메모리에 추가하고 users.txt에 바로 저장합니다. */
static void handle_user_add(ClientContext *client, const Message *request, Message *response) {
    User user;
    int added;

    require_login(client, response, CMD_USER_ADD);
    if (!client->logged_in) {
        return;
    }

    if (!read_user_data(request->payload, &user)) {
        message_init(response, MSG_TYPE_RES, CMD_USER_ADD, MSG_STATUS_FAIL,
                     "사용자 정보를 빠짐없이 입력해주세요.");
        return;
    }

    WaitForSingleObject(client->state->data_mutex, INFINITE);
    added = user_add(&client->state->users, &user);
    if (added) {
        save_users(USERS_FILE_PATH, &client->state->users);
    }
    ReleaseMutex(client->state->data_mutex);

    message_init(response, MSG_TYPE_RES, CMD_USER_ADD,
                 added ? MSG_STATUS_OK : MSG_STATUS_FAIL,
                 added ? "사용자를 추가했습니다." :
                 "이미 있는 아이디이거나 더 이상 추가할 수 없습니다.");
}

/* 사용자 아이디를 기준으로 사용자를 삭제하고 users.txt에 바로 저장합니다. */
static void handle_user_delete(ClientContext *client, const Message *request, Message *response) {
    int deleted;

    require_login(client, response, CMD_USER_DELETE);
    if (!client->logged_in) {
        return;
    }

    if (request->payload[0] == '\0') {
        message_init(response, MSG_TYPE_RES, CMD_USER_DELETE, MSG_STATUS_FAIL,
                     "삭제할 사용자 아이디를 입력해주세요.");
        return;
    }

    WaitForSingleObject(client->state->data_mutex, INFINITE);
    deleted = user_delete(&client->state->users, request->payload);
    if (deleted) {
        save_users(USERS_FILE_PATH, &client->state->users);
    }
    ReleaseMutex(client->state->data_mutex);

    message_init(response, MSG_TYPE_RES, CMD_USER_DELETE,
                 deleted ? MSG_STATUS_OK : MSG_STATUS_FAIL,
                 deleted ? "사용자를 삭제했습니다." : "해당 사용자를 찾을 수 없습니다.");
}

/* 새 학생을 연결 리스트에 추가하고 파일 저장은 예약된 시각까지 미룹니다. */
static void handle_student_add(ClientContext *client, const Message *request, Message *response) {
    Student parsed;
    Student *student;
    char message[MESSAGE_PAYLOAD_SIZE];
    int added = 0;

    require_login(client, response, CMD_STUDENT_ADD);
    if (!client->logged_in) {
        return;
    }

    if (!read_student_data(request->payload, &parsed)) {
        message_init(response, MSG_TYPE_RES, CMD_STUDENT_ADD, MSG_STATUS_FAIL,
                     "학생 정보를 빠짐없이 입력해주세요.");
        return;
    }

    WaitForSingleObject(client->state->data_mutex, INFINITE);
    if (student_list_find_by_id(client->state->students, parsed.student_id) == NULL) {
        student = student_create(parsed.name, parsed.student_id, parsed.department,
                                 parsed.grade, parsed.gpa, parsed.email,
                                 parsed.mobile_phone);
        if (student != NULL) {
            student_list_append(&client->state->students, student);
            added = 1;
        }
    }
    ReleaseMutex(client->state->data_mutex);

    make_scheduled_save_message(message, sizeof(message), "학생을 추가했습니다.");
    message_init(response, MSG_TYPE_RES, CMD_STUDENT_ADD,
                 added ? MSG_STATUS_OK : MSG_STATUS_FAIL,
                 added ? message : "이미 있는 학번이거나 저장 공간이 부족합니다.");
}

/* 학번이 같은 학생을 찾아 연결 리스트 안의 정보를 수정합니다. */
static void handle_student_update(ClientContext *client, const Message *request, Message *response) {
    Student parsed;
    char message[MESSAGE_PAYLOAD_SIZE];
    int updated;

    require_login(client, response, CMD_STUDENT_UPDATE);
    if (!client->logged_in) {
        return;
    }

    if (!read_student_data(request->payload, &parsed)) {
        message_init(response, MSG_TYPE_RES, CMD_STUDENT_UPDATE, MSG_STATUS_FAIL,
                     "수정할 학생 정보를 빠짐없이 입력해주세요.");
        return;
    }

    WaitForSingleObject(client->state->data_mutex, INFINITE);
    updated = student_list_update(client->state->students, &parsed);
    ReleaseMutex(client->state->data_mutex);

    make_scheduled_save_message(message, sizeof(message), "학생 정보를 수정했습니다.");
    message_init(response, MSG_TYPE_RES, CMD_STUDENT_UPDATE,
                 updated ? MSG_STATUS_OK : MSG_STATUS_FAIL,
                 updated ? message : "해당 학번의 학생을 찾을 수 없습니다.");
}

/* 학번을 기준으로 학생을 연결 리스트에서 삭제합니다. */
static void handle_student_delete(ClientContext *client, const Message *request, Message *response) {
    char message[MESSAGE_PAYLOAD_SIZE];
    int deleted;

    require_login(client, response, CMD_STUDENT_DELETE);
    if (!client->logged_in) {
        return;
    }

    if (request->payload[0] == '\0') {
        message_init(response, MSG_TYPE_RES, CMD_STUDENT_DELETE, MSG_STATUS_FAIL,
                     "삭제할 학생 학번을 입력해주세요.");
        return;
    }

    WaitForSingleObject(client->state->data_mutex, INFINITE);
    deleted = student_list_remove_by_id(&client->state->students, request->payload);
    ReleaseMutex(client->state->data_mutex);

    make_scheduled_save_message(message, sizeof(message), "학생을 삭제했습니다.");
    message_init(response, MSG_TYPE_RES, CMD_STUDENT_DELETE,
                 deleted ? MSG_STATUS_OK : MSG_STATUS_FAIL,
                 deleted ? message : "해당 학번의 학생을 찾을 수 없습니다.");
}

/* 학생 검색과 학과별 조회를 같은 반복 방식으로 처리합니다. */
static int handle_student_query(ClientContext *client, const Message *request, Message *response,
                                int by_department) {
    Student *current;
    int count = 0;

    require_login(client, response, by_department ? CMD_DEPT_LIST : CMD_STUDENT_SEARCH);
    if (!client->logged_in) {
        return 1;
    }

    WaitForSingleObject(client->state->data_mutex, INFINITE);
    current = client->state->students;
    while (current != NULL) {
        int match = by_department
            ? student_matches_department(current, request->payload)
            : student_matches_keyword(current, request->payload);
        if (match) {
            char record[512];
            Message row_response;

            if (student_format_record(current, record, sizeof(record)) >= 0) {
                message_init(&row_response, MSG_TYPE_RES, request->command,
                             MSG_STATUS_OK, record);
                socket_send_message((unsigned long long)client->socket, &row_response);
            }
            ++count;
        }
        current = current->next;
    }
    ReleaseMutex(client->state->data_mutex);

    if (count == 0) {
        message_init(response, MSG_TYPE_RES, request->command, MSG_STATUS_FAIL,
                     "조건에 맞는 학생이 없습니다.");
        return 1;
    } else {
        char done_payload[64];
        Message done_response;

        snprintf(done_payload, sizeof(done_payload), "END\t%d", count);
        message_init(&done_response, MSG_TYPE_RES, request->command, MSG_STATUS_OK, done_payload);
        socket_send_message((unsigned long long)client->socket, &done_response);
        return 0;
    }
}

/* 요청 명령어를 보고 알맞은 처리 함수로 나누어 보냅니다. */
int handle_request(ClientContext *client, const Message *request, Message *response) {
    if (client == NULL || request == NULL || response == NULL) {
        return 0;
    }

    if (strcmp(request->type, MSG_TYPE_REQ) != 0) {
        message_init(response, MSG_TYPE_RES, request->command, MSG_STATUS_FAIL,
                     "요청 형식이 올바르지 않습니다.");
    } else if (strcmp(request->command, CMD_LOGIN) == 0) {
        handle_login(client, request, response);
    } else if (strcmp(request->command, CMD_USER_ADD) == 0) {
        handle_user_add(client, request, response);
    } else if (strcmp(request->command, CMD_USER_DELETE) == 0) {
        handle_user_delete(client, request, response);
    } else if (strcmp(request->command, CMD_STUDENT_SEARCH) == 0) {
        return handle_student_query(client, request, response, 0);
    } else if (strcmp(request->command, CMD_STUDENT_ADD) == 0) {
        handle_student_add(client, request, response);
    } else if (strcmp(request->command, CMD_STUDENT_DELETE) == 0) {
        handle_student_delete(client, request, response);
    } else if (strcmp(request->command, CMD_STUDENT_UPDATE) == 0) {
        handle_student_update(client, request, response);
    } else if (strcmp(request->command, CMD_DEPT_LIST) == 0) {
        return handle_student_query(client, request, response, 1);
    } else if (strcmp(request->command, CMD_QUIT) == 0) {
        message_init(response, MSG_TYPE_RES, CMD_QUIT, MSG_STATUS_OK, "프로그램을 종료합니다.");
    } else {
        message_init(response, MSG_TYPE_RES, request->command, MSG_STATUS_FAIL,
                     "없는 메뉴입니다.");
    }

    return 1;
}
