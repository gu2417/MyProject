# 요구사항 추적표

## 1. 기준

본 문서는 `requirements.md`의 주요 요구사항이 `docs/` 하위 문서에 어디에 반영되는지 추적한다.

## 2. 추적 매트릭스

| 요구사항 | 반영 문서 |
|------|------|
| Windows C 콘솔 프로그램 | `overview/project_summary.md`, `development/implementation_guide.md` |
| 서버/클라이언트 분리 | `architecture/system_structure_design.md`, `architecture/server_architecture.md`, `architecture/client_architecture.md` |
| 소켓 통신 | `architecture/system_structure_design.md`, `architecture/module_common.md`, `features/functional_specification.md` |
| 로그인 인증 | `features/functional_specification.md`, `architecture/server_architecture.md` |
| 사용자 추가 및 삭제 | `features/functional_specification.md`, `database/file_schema.md` |
| 학생 조회, 추가, 수정, 삭제 | `features/functional_specification.md`, `database/in_memory_structures.md` |
| 학과명 기준 조회 | `features/functional_specification.md` |
| 단과대학 기준 검색 | `features/functional_specification.md` |
| 연결 리스트 | `database/in_memory_structures.md`, `architecture/server_architecture.md` |
| 파일 입출력 | `database/file_schema.md`, `architecture/module_common.md` |
| `time.h` 라이브러리 | `development/implementation_guide.md`, `database/file_schema.md` |
| 학생 데이터 10명 | `data_samples/SMU_Students.txt`, `database/file_schema.md` |
| 사용자 데이터 3명 | `data_samples/users.txt`, `database/file_schema.md` |
| 파일 분리 및 헤더 파일 구조 | `development/development_plan.md`, `architecture/module_common.md` |

## 3. 충돌 처리 원칙

문서 간 내용이 다를 경우 `requirements.md`를 우선한다. 하위 문서는 구현 편의를 위한 세부화 문서이며, `requirements.md`에 없는 기능을 추가 요구사항으로 확장하지 않는다.
