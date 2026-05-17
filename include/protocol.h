#ifndef PROTOCOL_H
#define PROTOCOL_H

#include <stddef.h>

/* 서버와 클라이언트가 약속해서 사용하는 기본 통신 설정입니다. */
#define SERVER_PORT 55555
#define PROTOCOL_BUFFER_SIZE 8192
#define MESSAGE_PAYLOAD_SIZE 4096

/* 메시지의 종류, 처리 결과, 명령어 이름을 문자열 상수로 관리합니다. */
#define MSG_TYPE_REQ "REQ"
#define MSG_TYPE_RES "RES"
#define MSG_STATUS_EMPTY "-"
#define MSG_STATUS_OK "OK"
#define MSG_STATUS_FAIL "FAIL"

#define CMD_LOGIN "LOGIN"
#define CMD_USER_ADD "USER_ADD"
#define CMD_USER_DELETE "USER_DELETE"
#define CMD_STUDENT_SEARCH "STUDENT_SEARCH"
#define CMD_STUDENT_ADD "STUDENT_ADD"
#define CMD_STUDENT_DELETE "STUDENT_DELETE"
#define CMD_STUDENT_UPDATE "STUDENT_UPDATE"
#define CMD_DEPT_LIST "DEPT_LIST"
#define CMD_QUIT "QUIT"

/* 한 번의 요청이나 응답을 담는 공통 메시지 구조체입니다. */
typedef struct Message {
    char type[8];
    char command[32];
    char status[16];
    char payload[MESSAGE_PAYLOAD_SIZE];
} Message;

void message_init(Message *message, const char *type, const char *command,
                  const char *status, const char *payload);
int message_to_text(const Message *message, char *buffer, size_t buffer_size);
int message_from_text(const char *buffer, Message *message);
int socket_send_message(unsigned long long socket_value, const Message *message);

#endif
