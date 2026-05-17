#ifndef SAVE_CONFIG_H
#define SAVE_CONFIG_H

/* 학생 정보를 파일로 자동 저장할 시각입니다.
   이 값만 바꾸면 서버와 클라이언트의 자동 저장 시간 안내도 같이 바뀝니다. */
#define SAVE_HOUR 20
#define SAVE_MINUTE 50

/* 실행 중에도 설정 파일을 다시 읽기 위해 사용하는 헤더 파일 경로입니다. */
#define SAVE_CONFIG_PATH "include/save_config.h"

/* save_config.h에 적힌 SAVE_HOUR, SAVE_MINUTE 값을 읽어 옵니다. */
int read_save_time(int *hour, int *minute);

#endif
