# Online Student Management System — 설계 문서

**기준**: [`../requirements.md`](../requirements.md)  
**언어**: 한국어 (기술 용어만 English)  
**대상**: Windows Console 기반 C 프로그램  
**구조**: Socket Server-Client

## 문서 구성

| # | 섹션 | 목적 |
|---|------|------|
| 01 | [overview](./01_overview/) | 프로젝트 목적, 범위, 플랫폼, NFR, 용어 |
| 02 | [features](./02_features/) | 기능 요구사항 상세 |
| 03 | [architecture](./03_architecture/) | 서버/클라이언트 구조, 데이터 흐름 |
| 04 | [file_structure](./04_file_structure/) | 소스 트리, 모듈 책임, 헤더 분리 |
| 05 | [security](./05_security/) | 로그인, 권한, 입력 검증 |
| 06 | [ui_ux](./06_ui_ux/) | 콘솔 메뉴, 명령, 화면 흐름 |
| 07 | [database](./07_database/) | 텍스트 파일 스키마와 연결 리스트 |
| 08 | [api](./08_api/) | 소켓 메시지 포맷과 명령 |
| 09 | [exception](./09_exception/) | 예외 분류와 처리 기준 |
| 10 | [todo_list](./10_todo_list/) | 구현 단계, 체크리스트, 수용 기준 |

## 읽는 순서

**처음 읽는 사람**: `01 → 02 → 03 → 07 → 08 → 06 → 05 → 09 → 04 → 10`  
**구현자**: `04 → 08 → 07 → 03 → 02 → 09` 순서로 참조  
**리뷰어**: `01 → 02 → 05 → 09 → 10`

## 문서 규약

- 파일/함수/매크로/명령 코드는 `backtick` 으로 표기.
- 요구사항 ID는 `requirements.md` 의 ID 를 그대로 사용한다.
- 다이어그램은 mermaid 코드블록으로 작성한다.
- `requirements.md` 에 없는 기능은 문서에 추가하지 않는다.
