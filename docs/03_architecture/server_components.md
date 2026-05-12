# 서버 컴포넌트

## 1. 서버 계층

| 계층 | 모듈 | 책임 |
|------|------|------|
| Entry | `server_main.c` | 서버 시작, 소켓 초기화, accept 루프 |
| Protocol | `request_handler.c` | 메시지 파싱, 명령 분기, 응답 생성 |
| Domain | `student.c` | 학생 연결 리스트 관리 |
| Domain | `user.c` | 로그인, 사용자 추가, 사용자 삭제 |
| Storage | `file_io.c` | `users.txt`, `SMU_Students.txt` 입출력 |
| Schedule | `scheduler.c` | `time.h` 기반 01:00 저장 판단 |

## 2. 서버 시작 순서

1. 소켓 초기화.
2. `SMU_Students.txt` 로딩.
3. 학생 연결 리스트 구성.
4. 클라이언트 연결 대기.
5. 요청 수신 후 명령 처리.
6. 응답 송신.
7. 매일 01:00 학생정보 저장.
