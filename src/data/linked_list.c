#include "linked_list.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* 구조체의 문자열 배열 크기를 넘지 않도록 안전하게 복사합니다. */
static void copy_text(char *dest, size_t size, const char *src) {
    if (size == 0) {
        return;
    }
    snprintf(dest, size, "%s", src ? src : "");
}

/* 새 학생 노드를 만들고 기본 정보를 채워 넣습니다. */
Student *student_create(const char *name, const char *student_id,
                        const char *department, int grade, double gpa,
                        const char *email, const char *mobile_phone) {
    Student *student = (Student *)calloc(1, sizeof(Student));
    if (student == NULL) {
        return NULL;
    }

    copy_text(student->name, sizeof(student->name), name);
    copy_text(student->student_id, sizeof(student->student_id), student_id);
    copy_text(student->department, sizeof(student->department), department);
    student->grade = grade;
    student->gpa = gpa;
    copy_text(student->email, sizeof(student->email), email);
    copy_text(student->mobile_phone, sizeof(student->mobile_phone), mobile_phone);
    student->next = NULL;

    return student;
}

/* 연결 리스트의 마지막에 새 학생 노드를 붙입니다. */
void student_list_append(Student **head, Student *student) {
    Student *current;

    if (head == NULL || student == NULL) {
        return;
    }

    if (*head == NULL) {
        *head = student;
        return;
    }

    current = *head;
    while (current->next != NULL) {
        current = current->next;
    }
    current->next = student;
}

/* 학번을 기준으로 연결 리스트에서 학생을 찾습니다. */
Student *student_list_find_by_id(Student *head, const char *student_id) {
    if (student_id == NULL) {
        return NULL;
    }

    while (head != NULL) {
        if (strcmp(head->student_id, student_id) == 0) {
            return head;
        }
        head = head->next;
    }
    return NULL;
}

/* 같은 학번의 학생을 찾아 새 정보로 교체합니다. */
int student_list_update(Student *head, const Student *student) {
    Student *target;

    if (student == NULL) {
        return 0;
    }

    target = student_list_find_by_id(head, student->student_id);
    if (target == NULL) {
        return 0;
    }

    snprintf(target->name, sizeof(target->name), "%s", student->name);
    snprintf(target->department, sizeof(target->department), "%s", student->department);
    target->grade = student->grade;
    target->gpa = student->gpa;
    snprintf(target->email, sizeof(target->email), "%s", student->email);
    snprintf(target->mobile_phone, sizeof(target->mobile_phone), "%s", student->mobile_phone);
    return 1;
}

/* 학번이 일치하는 학생 노드를 연결 리스트에서 제거합니다. */
int student_list_remove_by_id(Student **head, const char *student_id) {
    Student *current;
    Student *previous = NULL;

    if (head == NULL || student_id == NULL) {
        return 0;
    }

    current = *head;
    while (current != NULL) {
        if (strcmp(current->student_id, student_id) == 0) {
            if (previous == NULL) {
                *head = current->next;
            } else {
                previous->next = current->next;
            }
            free(current);
            return 1;
        }
        previous = current;
        current = current->next;
    }

    return 0;
}

/* 현재 연결 리스트에 들어 있는 학생 수를 셉니다. */
int student_list_count(Student *head) {
    int count = 0;
    while (head != NULL) {
        ++count;
        head = head->next;
    }
    return count;
}

/* 서버 종료 시 연결 리스트에 할당된 메모리를 모두 해제합니다. */
void student_list_free(Student *head) {
    while (head != NULL) {
        Student *next = head->next;
        free(head);
        head = next;
    }
}

/* 학생 검색에서 학번이나 이름에 검색어가 포함되는지 확인합니다. */
int student_matches_keyword(const Student *student, const char *keyword) {
    if (student == NULL || keyword == NULL) {
        return 0;
    }

    return strstr(student->student_id, keyword) != NULL ||
           strstr(student->name, keyword) != NULL;
}

/* 학과별 조회에서 학생의 학과가 입력 학과와 같은지 확인합니다. */
int student_matches_department(const Student *student, const char *department) {
    if (student == NULL || department == NULL) {
        return 0;
    }

    return strcmp(student->department, department) == 0;
}

/* 학생 한 명의 정보를 통신에 사용할 탭 구분 문자열로 만듭니다. */
int student_format_record(const Student *student, char *buffer, unsigned int buffer_size) {
    int written;

    if (student == NULL || buffer == NULL || buffer_size == 0) {
        return -1;
    }

    written = snprintf(buffer, buffer_size, "%s\t%s\t%s\t%d\t%.2f\t%s\t%s",
                       student->name, student->student_id, student->department,
                       student->grade, student->gpa, student->email,
                       student->mobile_phone);
    if (written < 0 || (unsigned int)written >= buffer_size) {
        return -1;
    }

    return written;
}
