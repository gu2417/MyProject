#ifndef FILE_IO_H
#define FILE_IO_H

#include "linked_list.h"
#include "user.h"

/* 사용자 정보와 학생 정보가 저장되는 텍스트 파일 경로입니다. */
#define USERS_FILE_PATH "data/users.txt"
#define STUDENTS_FILE_PATH "data/SMU_Students.txt"

/* 파일과 메모리 자료구조 사이에서 데이터를 읽고 쓰는 함수들입니다. */
int load_students(const char *path, Student **head);
int save_students(const char *path, Student *head);
int load_users(const char *path, UserStore *store);
int save_users(const char *path, const UserStore *store);

#endif
