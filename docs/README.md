# Online Student Management System — 문서 인덱스

**기준 문서**: `requirements.md` | **언어**: C | **플랫폼**: Windows | **UI**: Console

본 문서 묶음은 대학 교직원을 위한 온라인 학생관리 시스템의 요구사항, 서버/클라이언트 구조, 파일 구조, 메시지 포맷, 구조체 설계, 개발 계획을 정리한다. 모든 하위 문서는 `requirements.md`를 단일 최종 기준으로 따른다.

---

## 기술 스택

| 항목 | 내용 |
|------|------|
| 언어 | C |
| 운영체제 | Windows |
| UI | 콘솔 메뉴 |
| 구조 | 서버/클라이언트 분리 |
| 통신 | 소켓 통신 |
| 자료구조 | 학생 연결 리스트 |
| 저장 방식 | 텍스트 파일 입출력 |
| 시간 처리 | `time.h` |

---

## 문서 구조

| 폴더 | 파일 | 내용 |
|------|------|------|
| `overview/` | `requirements_analysis.md` | 최신 요구사항 요약 |
| `overview/` | `project_summary.md` | 프로젝트 개요 |
| `overview/` | `requirements_traceability.md` | 요구사항 추적표 |
| `architecture/` | `system_structure_design.md` | 전체 시스템 구조 |
| `architecture/` | `server_architecture.md` | 서버 구조와 처리 흐름 |
| `architecture/` | `client_architecture.md` | 클라이언트 구조와 메뉴 흐름 |
| `architecture/` | `module_common.md` | 공통 헤더, 메시지 포맷, 구조체 |
| `database/` | `file_schema.md` | `users.txt`, `SMU_Students.txt` 파일 구조 |
| `database/` | `in_memory_structures.md` | 연결 리스트와 인메모리 구조 |
| `development/` | `development_plan.md` | 개발 계획 |
| `development/` | `development_phases.md` | 단계별 구현 범위 |
| `development/` | `implementation_guide.md` | 구현 지침 |
| `development/` | `nfr_checklist.md` | 비기능 요구사항 점검표 |
| `features/` | `functional_specification.md` | 기능 명세 |

---

## 핵심 요구사항

- Windows PC에서 정상 동작하는 C 언어 기반 콘솔 프로그램
- 학생정보를 관리하는 서버와 사용자 명령을 송신하는 클라이언트 분리
- 소켓 통신 기반 요청/응답 처리
- 로그인된 인가 사용자만 학생정보 관리 기능 사용
- 로그인된 사용자만 신규 사용자 추가
- 사용자 삭제 기능
- `users.txt`와 `SMU_Students.txt` 탭 구분 파일 관리
- 학생 검색, 학생 추가, 학생 삭제, 학생 정보 수정
- 학과명 기반 전체 학생 조회
- 서버의 연결 리스트 기반 학생정보 관리
- 매일 01:00 `time.h` 기반 학생정보 저장
- 송수신용 메시지 포맷과 서버 기능 구현용 구조체 정의
