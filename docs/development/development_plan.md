# 개발 계획

## 1. 개발 목표

최신 `requirements.md`를 기준으로 C 언어 기반 Windows 콘솔 서버/클라이언트 학생관리 시스템을 구현한다.

## 2. 단계별 계획

| 단계 | 작업 | 산출물 |
|------|------|------|
| 1 | 요구사항 및 파일 구조 정리 | 최종 요구사항 문서, 파일 스키마 |
| 2 | 서버/클라이언트 기본 구조 작성 | `server_main.c`, `client_main.c` |
| 3 | 공통 헤더 작성 | `protocol.h`, `student.h`, `user.h`, `file_io.h`, `scheduler.h` |
| 4 | 학생 연결 리스트 구현 | 학생 검색, 추가, 삭제, 수정, 학과 조회 함수 |
| 5 | 파일 입출력 구현 | `SMU_Students.txt`, `users.txt` 로딩/저장 |
| 6 | 로그인 및 사용자 관리 구현 | 로그인, 신규 사용자 추가, 사용자 삭제 |
| 7 | 소켓 메시지 송수신 구현 | 요청/응답 메시지 포맷 처리 |
| 8 | 01:00 저장 구현 | `time.h` 기반 학생정보 저장 |
| 9 | 클라이언트 메뉴 구현 | 사용자 명령 입력, 서버 결과 출력 |
| 10 | 통합 검증 | 주요 기능과 파일 반영 확인 |

## 3. 권장 파일 구조

```text
MyProject/
  src/
    client/
      client_main.c
      client_ui.c
    server/
      server_main.c
      request_handler.c
    common/
      protocol.h
      student.c
      student.h
      user.c
      user.h
      file_io.c
      file_io.h
      scheduler.c
      scheduler.h
  data/
    SMU_Students.txt
    users.txt
```

## 4. 테스트 계획

| 구분 | 테스트 항목 |
|------|------|
| 로그인 | 성공 로그인, 실패 로그인 |
| 사용자 추가 | 로그인된 사용자 신규 사용자 추가, 중복 사용자ID |
| 사용자 삭제 | 사용자 삭제 후 `users.txt` 반영 |
| 학생 검색 | 학생 검색 요청과 결과 출력 |
| 학생 추가 | 연결 리스트와 `SMU_Students.txt` 반영 |
| 학생 삭제 | 연결 리스트와 `SMU_Students.txt` 반영 |
| 학생 수정 | 연결 리스트와 `SMU_Students.txt` 반영 |
| 학과 조회 | 학과명 기반 전체 학생 조회 |
| 통신 | 정의된 메시지 포맷 송수신 |
| 저장 | 매일 01:00 학생정보 저장 |

## 5. 완료 기준

- Windows 콘솔에서 서버와 클라이언트가 실행된다.
- 클라이언트가 사용자 명령 기반 요청을 서버에 송신한다.
- 서버가 요청을 처리하고 결과를 반환한다.
- 학생정보는 연결 리스트로 관리된다.
- `users.txt`와 `SMU_Students.txt`는 탭 구분 형식을 따른다.
- 서버는 매일 01:00에 학생정보 저장을 수행한다.
