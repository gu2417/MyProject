#include "user.h"

#include <string.h>

/* 사용자 아이디가 저장소의 몇 번째 위치에 있는지 찾습니다. */
int user_find_index(const UserStore *store, const char *user_id) {
    size_t index;

    if (store == NULL || user_id == NULL) {
        return -1;
    }

    for (index = 0; index < store->count; ++index) {
        if (strcmp(store->items[index].user_id, user_id) == 0) {
            return (int)index;
        }
    }

    return -1;
}

/* 로그인할 때 아이디와 비밀번호가 맞는지 단계별로 확인합니다. */
int user_check_login(const UserStore *store, const char *user_id, const char *password) {
    int index = user_find_index(store, user_id);

    if (index < 0) {
        return 1;
    }

    if (strcmp(store->items[index].password, password) != 0) {
        return 2;
    }

    return 0;
}

/* 새 사용자를 배열 끝에 추가하고, 중복 아이디는 거부합니다. */
int user_add(UserStore *store, const User *user) {
    if (store == NULL || user == NULL) {
        return 0;
    }

    if (store->count >= MAX_USERS || user_find_index(store, user->user_id) >= 0) {
        return 0;
    }

    store->items[store->count] = *user;
    ++store->count;
    return 1;
}

/* 사용자 아이디를 찾아 삭제하고 뒤쪽 데이터를 앞으로 당깁니다. */
int user_delete(UserStore *store, const char *user_id) {
    int index;
    size_t pos;

    if (store == NULL || user_id == NULL) {
        return 0;
    }

    index = user_find_index(store, user_id);
    if (index < 0) {
        return 0;
    }

    for (pos = (size_t)index; pos + 1 < store->count; ++pos) {
        store->items[pos] = store->items[pos + 1];
    }
    --store->count;
    return 1;
}
