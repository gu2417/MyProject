# 빌드와 이식성

## 1. 기준

본 프로젝트는 Windows PC에서 동작하는 C 언어 기반 콘솔 프로그램이다.

## 2. 빌드 산출물

| 실행 파일 | 설명 |
|-----------|------|
| `server.exe` | 학생정보 관리 서버 |
| `client.exe` | 학생 관리자용 클라이언트 |

## 3. 포터빌리티

| 항목 | 기준 |
|------|------|
| OS | Windows |
| UI | Console |
| Socket | Windows 소켓 API 사용 가능 |
| Time | C 표준 `time.h` |
| Storage | Text file I/O |
