# Student Management System — 문서 인덱스

**언어**: C | **플랫폼**: Windows | **UI**: Console | **기준 문서**: `requirements.md`

본 문서 묶음은 `requirements.md`를 단일 기준으로 하여 C 언어 기반 Windows 콘솔 학생관리 시스템의 요구사항, 구조, 데이터 파일, 기능 명세, 개발 계획, 샘플 데이터를 정리한다.

---

## 기술 스택

| 항목 | 내용 |
|------|------|
| 언어 | C |
| UI | Windows 콘솔 |
| 네트워크 | 소켓 통신 |
| 데이터 구조 | 학생 연결 리스트 |
| 영속 저장 | 텍스트 파일 입출력 |
| 시간 처리 | `time.h` 라이브러리 |
| 데이터 파일 | `SMU_Students.txt`, `users.txt` |

---

## 문서 구조

| 폴더 | 파일 | 내용 |
|------|------|------|
| `overview/` | `requirements_analysis.md` | 최종 요구사항 요약 |
| `overview/` | `project_summary.md` | 프로젝트 개요와 범위 |
| `overview/` | `requirements_traceability.md` | `requirements.md`와 하위 문서 추적표 |
| `architecture/` | `system_structure_design.md` | 전체 서버/클라이언트 구조 |
| `architecture/` | `server_architecture.md` | 서버 모듈 책임과 처리 흐름 |
| `architecture/` | `client_architecture.md` | 클라이언트 메뉴와 요청 흐름 |
| `architecture/` | `module_common.md` | 공통 헤더와 공통 모듈 |
| `database/` | `file_schema.md` | 데이터 파일 스키마와 처리 정책 |
| `database/` | `in_memory_structures.md` | 연결 리스트와 인메모리 구조 |
| `development/` | `development_plan.md` | 단계별 개발 계획 |
| `development/` | `development_phases.md` | 구현 단계와 완료 기준 |
| `development/` | `implementation_guide.md` | 구현 지침 |
| `development/` | `nfr_checklist.md` | 비기능 요구사항 점검표 |
| `features/` | `functional_specification.md` | 기능별 상세 명세 |
| `data_samples/` | `SMU_Students.txt` | 초기 학생 샘플 데이터 10명 |
| `data_samples/` | `users.txt` | 초기 사용자 샘플 데이터 3명 |

---

## 핵심 범위

- 서버와 클라이언트가 분리된 C 언어 기반 Windows 콘솔 프로그램
- 클라이언트 입력을 서버 요청으로 전송하고 서버 응답을 출력
- 서버에서 인증, 학생 관리, 사용자 관리 요청 처리
- 학생 정보는 연결 리스트로 관리
- 학생 및 사용자 정보는 텍스트 파일로 로딩/저장
- 소켓 통신, 파일 입출력, `time.h` 사용

---

## 데이터 파일

```text
data/SMU_Students.txt
data/users.txt
```

`docs/data_samples/`의 파일은 초기 데이터 예시이다. 실제 실행 파일의 데이터 경로는 구현 시 상수 또는 설정값으로 분리한다.
