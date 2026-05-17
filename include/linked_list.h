#ifndef LINKED_LIST_H
#define LINKED_LIST_H

#include "student.h"

/* Student 노드를 연결 리스트로 다루기 위한 기본 연산들입니다. */
Student *student_create(const char *name, const char *student_id, const char *department, int grade, double gpa, const char *email, const char *mobile_phone);
void student_list_append(Student **head, Student *student);
Student *student_list_find_by_id(Student *head, const char *student_id);
int student_list_update(Student *head, const Student *student);
int student_list_remove_by_id(Student **head, const char *student_id);
int student_list_count(Student *head);
void student_list_free(Student *head);
int student_matches_keyword(const Student *student, const char *keyword);
int student_matches_department(const Student *student, const char *department);
int student_format_record(const Student *student, char *buffer, unsigned int buffer_size);

#endif
