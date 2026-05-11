# 공통 모듈

## 1. 목적

공통 모듈은 서버와 클라이언트가 함께 사용하는 메시지 포맷, 명령 코드, 구조체, 파일 경로 상수, 함수 원형을 정의한다.

## 2. 메시지 포맷

서버/클라이언트 송수신용 메시지는 탭 문자로 필드를 구분하고, 줄바꿈으로 메시지 끝을 구분한다.

```text
TYPE<TAB>COMMAND<TAB>STATUS<TAB>PAYLOAD
```

| 필드 | 설명 |
|------|------|
| TYPE | `REQ` 또는 `RES` |
| COMMAND | LOGIN, USER_ADD, USER_DELETE, STUDENT_SEARCH, STUDENT_ADD, STUDENT_DELETE, STUDENT_UPDATE, DEPT_LIST |
| STATUS | 요청은 `-`, 응답은 `OK` 또는 `FAIL` |
| PAYLOAD | 명령별 데이터 |

## 3. 구조체 정의

```c
typedef struct Student {
    char name[50];
    char student_id[20];
    char department[80];
    int grade;
    double gpa;
    char email[100];
    char mobile_phone[30];
    struct Student *next;
} Student;

typedef struct User {
    char user_id[40];
    char password[80];
    char department[80];
    char name[50];
} User;

typedef struct Message {
    char type[8];
    char command[32];
    char status[16];
    char payload[1024];
} Message;
```

## 4. 헤더 파일 원칙

| 파일 | 포함 내용 |
|------|------|
| `protocol.h` | 메시지 포맷, 명령 코드, 응답 상태, 버퍼 크기 |
| `student.h` | `Student` 구조체와 학생 관리 함수 원형 |
| `user.h` | `User` 구조체와 사용자 관리 함수 원형 |
| `file_io.h` | 파일 입출력 함수 원형 |
| `scheduler.h` | 01:00 저장 확인 함수 원형 |

## 5. 시간 처리

매일 01:00 학생정보 저장 시점 판단은 C 표준 `time.h`를 사용한다.
