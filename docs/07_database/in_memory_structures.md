# 인메모리 구조

## 1. Student

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

## 2. User

```c
typedef struct User {
    char user_id[40];
    char password[80];
    char department[80];
    char name[50];
} User;
```

## 3. Message

```c
typedef struct Message {
    char type[8];
    char command[32];
    char status[16];
    char payload[1024];
} Message;
```

## 4. 연결 리스트 연산

| 연산 | 기준 |
|------|------|
| 초기화 | `SMU_Students.txt` 로딩 후 노드 생성 |
| 검색 | 연결 리스트 순회 |
| 추가 | 새 노드 삽입 |
| 삭제 | 대상 노드 제거 |
| 수정 | 대상 노드 필드 갱신 |
| 학과 조회 | `department` 일치 노드 전체 반환 |
| 저장 | 리스트 전체를 `SMU_Students.txt` 에 기록 |
