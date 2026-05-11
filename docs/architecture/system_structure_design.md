# 시스템 구조 설계

## 1. 전체 구조

시스템은 클라이언트 프로그램과 서버 프로그램으로 분리한다.

```text
Client Console Program
        |
        | socket request / response
        v
Server Program
        |
        | load / save
        v
Text Data Files
```

## 2. 클라이언트 책임

- 사용자 ID와 비밀번호 입력
- 로그인 요청 전송
- 로그인 성공 후 메뉴 출력
- 학생 정보 조회, 추가, 수정, 삭제 요청 생성
- 학과명 기준 조회 요청 생성
- 단과대학 기반 검색 요청 생성
- 사용자 계정 추가, 삭제 요청 생성
- 서버 응답 메시지와 조회 결과 출력

## 3. 서버 책임

- 서버 소켓 초기화와 클라이언트 연결 수락
- 클라이언트 요청 수신 및 명령 분기
- `SMU_Students.txt` 로딩
- `users.txt` 기반 인증 및 사용자 관리
- 학생 연결 리스트 생성, 탐색, 추가, 수정, 삭제
- 학과명 및 단과대학 기준 검색 처리
- 변경된 학생 정보와 사용자 정보를 파일에 저장
- 요청 성공/실패 결과 반환

## 4. 주요 모듈

| 영역 | 모듈 | 책임 |
|------|------|------|
| client | `client_main.c` | 클라이언트 진입점, 서버 연결 |
| client | `client_ui.c` | 콘솔 메뉴, 입력, 결과 출력 |
| server | `server_main.c` | 서버 진입점, 소켓 초기화, accept 루프 |
| server | `request_handler.c` | 요청 명령 분기와 처리 호출 |
| common | `protocol.h` | 요청/응답 코드, 공통 상수 |
| common | `student.c/h` | 학생 구조체와 연결 리스트 관리 |
| common | `auth.c/h` | 로그인, 사용자 추가, 사용자 삭제 |
| common | `file_io.c/h` | 텍스트 파일 로딩과 저장 |
| common | `validation.c/h` | 필수값, 범위, 중복, 형식 검증 |

## 5. 데이터 흐름

```text
사용자 입력
  -> client_ui
  -> socket send
  -> request_handler
  -> auth/student/file_io
  -> socket response
  -> client_ui 결과 출력
```

## 6. 설계 원칙

- `requirements.md`에 없는 기능은 구현 범위에 추가하지 않는다.
- 학생 데이터의 기준 자료구조는 연결 리스트이다.
- 파일 경로, 버퍼 크기, 요청 코드는 헤더 파일 상수로 분리한다.
- 요청 및 응답의 구체적인 문자열 형식은 요구사항 미정이므로 `protocol.h`에서 일관되게 확정한다.
- 단과대학과 소속 학과 매핑의 구체적인 구조와 초기값은 요구사항 미정이다.
