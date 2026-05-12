# 프로젝트 레이아웃

```text
MyProject/
  requirements.md
  src/
    server/
      server_main.c
      request_handler.c
    client/
      client_main.c
      client_ui.c
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
  docs/
```

## 1. 분리 기준

| 영역 | 기준 |
|------|------|
| `server/` | 서버 실행과 요청 처리 |
| `client/` | 콘솔 UI와 요청 송신 |
| `common/` | 구조체, 메시지 포맷, 파일 입출력, 도메인 로직 |
| `data/` | 텍스트 데이터 파일 |
