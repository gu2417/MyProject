# 공통 모듈

| 파일 | 책임 |
|------|------|
| `protocol.h` | 메시지 구조체, 명령 코드, 상태 코드, 버퍼 크기 |
| `student.h` | `Student` 구조체와 학생 함수 원형 |
| `student.c` | 연결 리스트 기반 학생 검색/추가/삭제/수정 |
| `user.h` | `User` 구조체와 사용자 함수 원형 |
| `user.c` | 로그인 검증, 사용자 추가, 사용자 삭제 |
| `file_io.h` | 파일 입출력 함수 원형 |
| `file_io.c` | `users.txt`, `SMU_Students.txt` 로딩/저장 |
| `scheduler.h` | 01:00 저장 판단 함수 원형 |
| `scheduler.c` | `time.h` 기반 저장 시점 판단 |

## 헤더 파일 원칙

- 구조체와 함수 원형은 헤더에 정의한다.
- 소스 파일은 해당 헤더를 include 한다.
- 메시지 포맷 관련 상수는 `protocol.h` 에 모은다.
