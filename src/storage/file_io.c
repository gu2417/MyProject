#include "file_io.h"

#include <io.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* 파일에서 읽은 한 줄 끝의 개행 문자를 제거합니다. */
static void trim_newline(char *text) {
    size_t length;

    if (text == NULL) {
        return;
    }

    length = strlen(text);
    while (length > 0 && (text[length - 1] == '\n' || text[length - 1] == '\r')) {
        text[length - 1] = '\0';
        --length;
    }
}

/* UTF-8 BOM이 있으면 실제 데이터가 시작되는 위치로 건너뜁니다. */
static char *skip_utf8_bom(char *text) {
    unsigned char *bytes = (unsigned char *)text;

    if (bytes != NULL && strlen(text) >= 3 &&
        bytes[0] == 0xEF && bytes[1] == 0xBB && bytes[2] == 0xBF) {
        return text + 3;
    }

    return text;
}

/* Windows 메모장에서도 UTF-8 파일로 인식되도록 BOM을 기록합니다. */
static void write_utf8_bom(FILE *file) {
    fputs("\xEF\xBB\xBF", file);
}

/* 파일 버퍼의 내용을 디스크에 바로 반영되도록 처리합니다. */
static int flush_file(FILE *file) {
    if (fflush(file) != 0) {
        return -1;
    }

    return _commit(_fileno(file));
}

/* 학생 한 명의 정보를 탭으로 구분해서 파일에 기록합니다. */
static void write_student_row(FILE *file, const Student *student) {
    fprintf(file, "%s\t%s\t%s\t%d\t%.2f\t%s\t%s\n",
            student->name, student->student_id, student->department,
            student->grade, student->gpa, student->email,
            student->mobile_phone);
}

/* 학생 데이터 파일을 읽어 연결 리스트를 구성합니다. */
int load_students(const char *path, Student **head) {
    FILE *file;
    char line[512];
    int loaded = 0;

    if (path == NULL || head == NULL) {
        return -1;
    }

    file = fopen(path, "r");
    if (file == NULL) {
        return -1;
    }

    while (fgets(line, sizeof(line), file) != NULL) {
        char *name;
        char *student_id;
        char *department;
        char *grade_text;
        char *gpa_text;
        char *email;
        char *mobile_phone;
        Student *student;

        trim_newline(line);
        if (line[0] == '\0') {
            continue;
        }

        name = strtok(skip_utf8_bom(line), "\t");
        student_id = strtok(NULL, "\t");
        department = strtok(NULL, "\t");
        grade_text = strtok(NULL, "\t");
        gpa_text = strtok(NULL, "\t");
        email = strtok(NULL, "\t");
        mobile_phone = strtok(NULL, "\t");

        if (name == NULL || student_id == NULL || department == NULL ||
            grade_text == NULL || gpa_text == NULL || email == NULL ||
            mobile_phone == NULL) {
            continue;
        }

        student = student_create(name, student_id, department, atoi(grade_text),
                                 atof(gpa_text), email, mobile_phone);
        if (student == NULL) {
            fclose(file);
            return -1;
        }
        student_list_append(head, student);
        ++loaded;
    }

    fclose(file);
    return loaded;
}

/* 현재 연결 리스트에 있는 학생 정보를 파일 전체에 다시 저장합니다. */
int save_students(const char *path, Student *head) {
    FILE *file;

    if (path == NULL) {
        return -1;
    }

    file = fopen(path, "w");
    if (file == NULL) {
        return -1;
    }

    write_utf8_bom(file);
    while (head != NULL) {
        write_student_row(file, head);
        head = head->next;
    }

    if (flush_file(file) != 0) {
        fclose(file);
        return -1;
    }

    fclose(file);
    return 0;
}

/* 사용자 데이터 파일을 읽어 배열 형태의 UserStore에 담습니다. */
int load_users(const char *path, UserStore *store) {
    FILE *file;
    char line[512];

    if (path == NULL || store == NULL) {
        return -1;
    }

    store->count = 0;
    file = fopen(path, "r");
    if (file == NULL) {
        return -1;
    }

    while (store->count < MAX_USERS && fgets(line, sizeof(line), file) != NULL) {
        char *user_id;
        char *password;
        char *department;
        char *name;

        trim_newline(line);
        if (line[0] == '\0') {
            continue;
        }

        user_id = strtok(skip_utf8_bom(line), "\t");
        password = strtok(NULL, "\t");
        department = strtok(NULL, "\t");
        name = strtok(NULL, "\t");

        if (user_id == NULL || password == NULL || department == NULL || name == NULL) {
            continue;
        }

        snprintf(store->items[store->count].user_id,
                 sizeof(store->items[store->count].user_id), "%s", user_id);
        snprintf(store->items[store->count].password,
                 sizeof(store->items[store->count].password), "%s", password);
        snprintf(store->items[store->count].department,
                 sizeof(store->items[store->count].department), "%s", department);
        snprintf(store->items[store->count].name,
                 sizeof(store->items[store->count].name), "%s", name);
        ++store->count;
    }

    fclose(file);
    return 0;
}

/* 현재 UserStore에 있는 사용자 목록을 users.txt에 저장합니다. */
int save_users(const char *path, const UserStore *store) {
    FILE *file;
    size_t index;

    if (path == NULL || store == NULL) {
        return -1;
    }

    file = fopen(path, "w");
    if (file == NULL) {
        return -1;
    }

    write_utf8_bom(file);
    for (index = 0; index < store->count; ++index) {
        fprintf(file, "%s\t%s\t%s\t%s\n",
                store->items[index].user_id, store->items[index].password,
                store->items[index].department, store->items[index].name);
    }

    if (flush_file(file) != 0) {
        fclose(file);
        return -1;
    }

    fclose(file);
    return 0;
}
