#ifndef STUDENT_H
#define STUDENT_H

/* 학생 한 명의 정보를 저장하는 노드입니다.
   next 포인터를 이용해 여러 학생을 연결 리스트로 이어 붙입니다. */
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

#endif
