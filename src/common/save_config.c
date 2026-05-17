#include "save_config.h"

#include <stdio.h>

/* 시간은 0~23시, 분은 0~59분 범위 안에 있어야 합니다. */
static int is_valid_save_time(int hour, int minute) {
    return hour >= 0 && hour <= 23 && minute >= 0 && minute <= 59;
}

/* save_config.h 파일을 열어 현재 저장 시간 설정을 읽습니다. */
int read_save_time(int *out_hour, int *out_minute) {
    FILE *file = fopen(SAVE_CONFIG_PATH, "r");
    char line[256];
    int hour = SAVE_HOUR;
    int minute = SAVE_MINUTE;
    int found_hour = 0;
    int found_minute = 0;

    if (out_hour == NULL || out_minute == NULL) {
        return -1;
    }

    /* 설정 파일을 읽지 못하면 헤더에 컴파일된 기본값을 사용합니다. */
    if (file == NULL) {
        *out_hour = SAVE_HOUR;
        *out_minute = SAVE_MINUTE;
        return -1;
    }

    /* 한 줄씩 보면서 SAVE_HOUR와 SAVE_MINUTE 값을 찾습니다. */
    while (fgets(line, sizeof(line), file) != NULL) {
        int value;

        if (sscanf(line, "#define SAVE_HOUR %d", &value) == 1) {
            hour = value;
            found_hour = 1;
        } else if (sscanf(line, "#define SAVE_MINUTE %d", &value) == 1) {
            minute = value;
            found_minute = 1;
        }
    }

    fclose(file);

    /* 값이 없거나 범위를 벗어나면 기본값으로 돌려보냅니다. */
    if (!found_hour || !found_minute || !is_valid_save_time(hour, minute)) {
        *out_hour = SAVE_HOUR;
        *out_minute = SAVE_MINUTE;
        return -1;
    }

    *out_hour = hour;
    *out_minute = minute;
    return 0;
}
