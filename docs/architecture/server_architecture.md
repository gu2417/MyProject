# 서버 아키텍처

## 1. 서버 역할

서버는 학생정보를 관리하고 클라이언트의 학생정보 관리 요청을 처리한다. 서버는 시작 시 학생정보 관리 초기화 작업을 수행하며, 학생정보를 연결 리스트 기반으로 관리한다.

## 2. 서버 파일 구조

```text
src/server/
  server_main.c
  request_handler.c

src/common/
  protocol.h
  student.c
  student.h
  user.c
  user.h
  file_io.c
  file_io.h
  scheduler.c
  scheduler.h
```

## 3. 모듈별 책임

| 모듈 | 주요 책임 |
|------|------|
| `server_main.c` | 서버 소켓 초기화, 클라이언트 연결 수락, 서버 루프 |
| `request_handler.c` | 요청 메시지 파싱, 명령 분기, 응답 생성 |
| `student.c/h` | 학생 연결 리스트 관리 |
| `user.c/h` | 로그인, 사용자 추가, 사용자 삭제 |
| `file_io.c/h` | `users.txt`, `SMU_Students.txt` 파일 입출력 |
| `scheduler.c/h` | `time.h` 기반 매일 01:00 저장 판단 |
| `protocol.h` | 메시지 포맷과 명령 코드 정의 |

## 4. 서버 시작 흐름

1. 소켓 통신을 위한 초기화를 수행한다.
2. `SMU_Students.txt`를 읽어 학생 연결 리스트를 구성한다.
3. 클라이언트 연결을 대기한다.
4. 클라이언트 요청을 수신한다.
5. 요청 명령에 따라 기능을 처리한다.
6. 처리 결과를 응답 메시지로 반환한다.

## 5. 요청 처리 범위

| 명령 | 처리 내용 |
|------|------|
| LOGIN | `users.txt` 기준 로그인 검증 |
| USER_ADD | 로그인된 사용자의 신규 사용자 추가 |
| USER_DELETE | 사용자 삭제 및 `users.txt` 반영 |
| STUDENT_SEARCH | 연결 리스트 기반 학생 검색 |
| STUDENT_ADD | 학생 연결 리스트 추가 및 파일 반영 |
| STUDENT_DELETE | 학생 연결 리스트 삭제 및 파일 반영 |
| STUDENT_UPDATE | 학생 연결 리스트 수정 및 파일 반영 |
| DEPT_LIST | 학과명 기반 전체 학생 조회 |

## 6. 매일 01:00 저장

서버는 `time.h`를 사용하여 현재 시간을 확인하고, 매일 01:00에 학생정보를 `SMU_Students.txt`에 저장한다. 추가, 수정, 삭제된 학생 데이터는 저장 시 파일에 반영되어야 한다.
