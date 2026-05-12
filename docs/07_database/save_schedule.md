# 저장 스케줄

## 1. 기준

서버는 매일 01:00에 학생정보 저장을 수행한다.

## 2. 시간 처리

`time.h` 를 사용하여 현재 시간을 확인한다.

```c
time_t now = time(NULL);
struct tm *local = localtime(&now);
```

## 3. 저장 대상

서버 메모리의 학생 연결 리스트 전체를 `SMU_Students.txt` 에 반영한다.
