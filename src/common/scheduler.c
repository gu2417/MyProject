#include "scheduler.h"

/* 지정한 시각이 되었는지 확인하고, 같은 날 중복 저장을 막습니다. */
int is_daily_save_due(time_t now, int target_hour, int target_minute, int *last_saved_yday) {
    struct tm *local_time;

    if (last_saved_yday == 0) {
        return 0;
    }

    local_time = localtime(&now);
    if (local_time == 0) {
        return 0;
    }

    if (local_time->tm_hour == target_hour &&
        local_time->tm_min == target_minute &&
        *last_saved_yday != local_time->tm_yday) {
        return 1;
    }

    return 0;
}

/* 저장이 끝난 날짜를 기록해서 하루에 한 번만 저장되도록 합니다. */
void mark_daily_save_done(time_t now, int *last_saved_yday) {
    struct tm *local_time;

    if (last_saved_yday == 0) {
        return;
    }

    local_time = localtime(&now);
    if (local_time == 0) {
        return;
    }

    *last_saved_yday = local_time->tm_yday;
}
