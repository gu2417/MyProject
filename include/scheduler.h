#ifndef SCHEDULER_H
#define SCHEDULER_H

#include <time.h>

/* 하루에 한 번 정해진 시각에 저장해야 하는지 판단하는 함수들입니다. */
int is_daily_save_due(time_t now, int target_hour, int target_minute, int *last_saved_yday);
void mark_daily_save_done(time_t now, int *last_saved_yday);

#endif
