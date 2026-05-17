#ifndef SERVER_STATE_H
#define SERVER_STATE_H

#include <winsock2.h>
#include <windows.h>

#include "linked_list.h"
#include "user.h"

/* 서버 전체가 공유하는 데이터와 실행 상태입니다.
   여러 클라이언트 스레드가 함께 접근하므로 data_mutex로 보호합니다. */
typedef struct ServerState {
    UserStore users;
    Student *students;
    HANDLE data_mutex;
    int last_saved_yday;
    int running;
} ServerState;

/* 클라이언트 한 명의 접속 상태를 저장해 요청 처리 함수에 전달합니다. */
typedef struct ClientContext {
    SOCKET socket;
    int logged_in;
    char user_id[40];
    ServerState *state;
} ClientContext;

#endif
