# 파일 스키마

## 1. 개요

본 시스템은 `users.txt`와 `SMU_Students.txt` 파일을 사용한다. 두 파일 모두 탭 문자로 필드를 구분한다.

## 2. `SMU_Students.txt`

학생 정보 파일은 다음 필드를 탭으로 구분하여 저장한다.

```text
name<TAB>student_id<TAB>department<TAB>grade<TAB>gpa<TAB>email<TAB>mobile_phone
```

| 순서 | 필드 | 설명 |
|------|------|------|
| 1 | name | 이름 |
| 2 | student_id | 학번 |
| 3 | department | 소속학과 |
| 4 | grade | 학년 |
| 5 | gpa | GPA(평균학점) |
| 6 | email | 이메일 연락처 |
| 7 | mobile_phone | 모바일 폰 연락처 |

## 3. `users.txt`

사용자 정보 파일은 다음 필드를 탭으로 구분하여 저장한다.

```text
user_id<TAB>password<TAB>department<TAB>name
```

| 순서 | 필드 | 설명 |
|------|------|------|
| 1 | user_id | 사용자ID |
| 2 | password | 사용자암호 |
| 3 | department | 사용자 소속부서 |
| 4 | name | 사용자 이름 |

## 4. 파일 처리 정책

| 상황 | 처리 기준 |
|------|------|
| 서버 시작 | `SMU_Students.txt`를 읽어 학생 연결 리스트 초기화 |
| 로그인 | `users.txt`의 사용자ID와 사용자암호 검증 |
| 신규 사용자 추가 | `users.txt`에 탭 구분 형식으로 반영 |
| 사용자 삭제 | `users.txt`에 삭제 결과 반영 |
| 학생 추가/삭제/수정 | 연결 리스트와 `SMU_Students.txt`에 반영 |
| 매일 01:00 | `time.h` 기반으로 학생정보 저장 수행 |

## 5. 파일 입출력 함수

| 함수 | 역할 |
|------|------|
| `load_students` | 학생 파일을 읽어 연결 리스트 구성 |
| `save_students` | 연결 리스트 내용을 학생 파일에 저장 |
| `load_users` | 사용자 파일을 읽어 로그인 검증에 사용 |
| `save_users` | 사용자 추가/삭제 결과를 사용자 파일에 저장 |
