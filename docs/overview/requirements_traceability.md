# 요구사항 추적표

## 1. 기준

본 문서는 `requirements.md`의 주요 요구사항이 `docs/` 하위 문서에 반영된 위치를 추적한다.

## 2. 추적 매트릭스

| 요구사항 | 반영 문서 |
|------|------|
| Windows C 콘솔 프로그램 | `overview/project_summary.md`, `development/implementation_guide.md` |
| 서버/클라이언트 구조 | `architecture/system_structure_design.md`, `architecture/server_architecture.md`, `architecture/client_architecture.md` |
| 소켓 통신 | `architecture/system_structure_design.md`, `architecture/module_common.md` |
| 로그인 | `features/functional_specification.md`, `architecture/server_architecture.md` |
| 인가 사용자만 학생정보 관리 | `features/functional_specification.md` |
| 로그인된 사용자만 신규 사용자 추가 | `features/functional_specification.md` |
| 사용자 삭제 | `features/functional_specification.md` |
| `users.txt` 탭 구분 구조 | `database/file_schema.md` |
| `SMU_Students.txt` 탭 구분 구조 | `database/file_schema.md` |
| 학생 검색, 추가, 삭제, 수정 | `features/functional_specification.md` |
| 학과명 기반 전체 학생 조회 | `features/functional_specification.md` |
| 연결 리스트 | `database/in_memory_structures.md` |
| 매일 01:00 저장 | `features/functional_specification.md`, `development/implementation_guide.md` |
| `time.h` | `development/implementation_guide.md` |
| 메시지 포맷 | `architecture/module_common.md` |
| 구조체 및 구조체 멤버 | `architecture/module_common.md`, `database/in_memory_structures.md` |

## 3. 충돌 처리 원칙

문서 간 내용이 다를 경우 `requirements.md`를 우선한다. 최신 요구사항에 없는 기능, 설명, 조건, 예시, 설계 내용은 하위 문서에 유지하지 않는다.
