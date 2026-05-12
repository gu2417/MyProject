# 네이밍 규칙

## 1. 파일명

| 대상 | 규칙 | 예 |
|------|------|----|
| C 소스 | snake_case | `request_handler.c` |
| 헤더 | snake_case | `file_io.h` |
| 데이터 파일 | 요구사항 파일명 유지 | `SMU_Students.txt` |

## 2. 함수명

| 범주 | 예 |
|------|----|
| 학생 관리 | `student_add`, `student_delete`, `student_find` |
| 사용자 관리 | `user_login`, `user_add`, `user_delete` |
| 파일 입출력 | `load_students`, `save_students`, `load_users`, `save_users` |
| 스케줄 | `should_save_students_at_0100` |

## 3. 매크로

명령 코드와 상수는 대문자 snake case를 사용한다.

```c
#define CMD_LOGIN "LOGIN"
#define STATUS_OK "OK"
```
