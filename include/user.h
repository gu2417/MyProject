#ifndef USER_H
#define USER_H

#include <stddef.h>

/* 파일에서 읽어 올 수 있는 최대 사용자 수입니다. */
#define MAX_USERS 256

/* 로그인 가능한 사용자 한 명의 계정 정보입니다. */
typedef struct User {
    char user_id[40];
    char password[80];
    char department[80];
    char name[50];
} User;

/* 사용자 배열과 현재 저장된 개수를 함께 관리하는 저장소입니다. */
typedef struct UserStore {
    User items[MAX_USERS];
    size_t count;
} UserStore;

int user_find_index(const UserStore *store, const char *user_id);
int user_check_login(const UserStore *store, const char *user_id, const char *password);
int user_add(UserStore *store, const User *user);
int user_delete(UserStore *store, const char *user_id);

#endif
