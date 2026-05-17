#include "protocol.h"

#include <stdio.h>
#include <string.h>
#include <winsock2.h>

/* 문자열 필드가 비어 있어도 구조체에 안전하게 복사합니다. */
static void copy_field(char *dest, size_t size, const char *src) {
    if (size == 0) {
        return;
    }
    snprintf(dest, size, "%s", src ? src : "");
}

/* 요청이나 응답에 들어갈 공통 메시지 구조체를 초기화합니다. */
void message_init(Message *message, const char *type, const char *command,
                  const char *status, const char *payload) {
    if (message == NULL) {
        return;
    }

    copy_field(message->type, sizeof(message->type), type);
    copy_field(message->command, sizeof(message->command), command);
    copy_field(message->status, sizeof(message->status), status);
    copy_field(message->payload, sizeof(message->payload), payload);
}

/* Message 구조체를 소켓으로 보낼 수 있는 탭 구분 문자열로 바꿉니다. */
int message_to_text(const Message *message, char *buffer, size_t buffer_size) {
    int written;

    if (message == NULL || buffer == NULL || buffer_size == 0) {
        return -1;
    }

    written = snprintf(buffer, buffer_size, "%s\t%s\t%s\t%s\n",
                       message->type, message->command, message->status,
                       message->payload);
    if (written < 0 || (size_t)written >= buffer_size) {
        return -1;
    }

    return written;
}

/* 서버와 클라이언트가 주고받은 문자열을 Message 구조체로 다시 분리합니다. */
int message_from_text(const char *buffer, Message *message) {
    char temp[PROTOCOL_BUFFER_SIZE];
    char *type;
    char *command;
    char *status;
    char *payload;
    char *first_tab;
    char *second_tab;
    char *third_tab;

    if (buffer == NULL || message == NULL) {
        return -1;
    }

    snprintf(temp, sizeof(temp), "%s", buffer);
    temp[strcspn(temp, "\r\n")] = '\0';

    first_tab = strchr(temp, '\t');
    if (first_tab == NULL) {
        return -1;
    }
    *first_tab = '\0';
    type = temp;

    second_tab = strchr(first_tab + 1, '\t');
    if (second_tab == NULL) {
        return -1;
    }
    *second_tab = '\0';
    command = first_tab + 1;

    third_tab = strchr(second_tab + 1, '\t');
    if (third_tab == NULL) {
        return -1;
    }
    *third_tab = '\0';
    status = second_tab + 1;
    payload = third_tab + 1;

    message_init(message, type, command, status, payload);
    return 0;
}

/* 완성된 Message를 문자열로 바꾼 뒤 소켓에 전송합니다. */
int socket_send_message(unsigned long long socket_value, const Message *message) {
    char buffer[PROTOCOL_BUFFER_SIZE];
    int length;

    length = message_to_text(message, buffer, sizeof(buffer));
    if (length < 0) {
        return -1;
    }

    return send((SOCKET)socket_value, buffer, length, 0);
}
