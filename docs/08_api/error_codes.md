# 오류 코드

## 1. STATUS

| STATUS | 의미 |
|--------|------|
| OK | 요청 처리 성공 |
| FAIL | 요청 처리 실패 |

## 2. 실패 상황

| 상황 | 처리 |
|------|------|
| 로그인 실패 | `RES<TAB>LOGIN<TAB>FAIL<TAB>message` |
| 중복 사용자ID | `RES<TAB>USER_ADD<TAB>FAIL<TAB>message` |
| 파일 입출력 실패 | 해당 명령의 `FAIL` 응답 |
| 잘못된 명령 | `RES<TAB>UNKNOWN<TAB>FAIL<TAB>message` |
