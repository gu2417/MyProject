# 메시지 포맷

## 1. 프레임

```text
TYPE<TAB>COMMAND<TAB>STATUS<TAB>PAYLOAD\n
```

- 필드는 탭 문자로 구분한다.
- 메시지 끝은 줄바꿈 문자로 구분한다.
- 요청 메시지는 `TYPE=REQ`, `STATUS=-` 를 사용한다.
- 응답 메시지는 `TYPE=RES`, `STATUS=OK` 또는 `FAIL` 을 사용한다.

## 2. 필드

| 필드 | 설명 |
|------|------|
| TYPE | `REQ` 또는 `RES` |
| COMMAND | 명령 코드 |
| STATUS | 요청은 `-`, 응답은 `OK` 또는 `FAIL` |
| PAYLOAD | 명령별 데이터 |

## 3. 명령 코드

| COMMAND | 설명 |
|---------|------|
| LOGIN | 로그인 |
| USER_ADD | 신규 사용자 추가 |
| USER_DELETE | 사용자 삭제 |
| STUDENT_SEARCH | 학생 검색 |
| STUDENT_ADD | 학생 추가 |
| STUDENT_DELETE | 학생 삭제 |
| STUDENT_UPDATE | 학생 정보 수정 |
| DEPT_LIST | 학과명 기반 전체 학생 조회 |
