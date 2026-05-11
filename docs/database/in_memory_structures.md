# 인메모리 구조

## 1. 개요

서버는 학생정보를 연결 리스트 기반으로 관리한다. 연결 리스트는 학생 검색, 학생 추가, 학생 삭제, 학생 정보 수정, 학과명 기반 전체 학생 조회에 사용된다.

## 2. 학생 구조체

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
```

## 3. 사용자 구조체

```c
typedef struct User {
    char user_id[40];
    char password[80];
    char department[80];
    char name[50];
} User;
```

## 4. 메시지 구조체

```c
typedef struct Message {
    char type[8];
    char command[32];
    char status[16];
    char payload[1024];
} Message;
```

## 5. 학생 연결 리스트 연산

| 연산 | 처리 기준 |
|------|------|
| 초기화 | `SMU_Students.txt`를 읽어 노드 생성 |
| 검색 | 연결 리스트 순회 |
| 추가 | 새 학생 노드를 연결 리스트에 추가 |
| 삭제 | 대상 학생 노드를 연결 리스트에서 제거 |
| 수정 | 대상 학생 노드의 정보를 갱신 |
| 학과 조회 | 소속학과가 일치하는 모든 노드 반환 |
| 저장 | 연결 리스트 내용을 `SMU_Students.txt`에 기록 |

## 6. 규모 기준

서버는 약 9,000명의 학생 수를 기준으로 연결 리스트 기반 학생정보를 관리한다.
