# 패킷 레퍼런스

## 1. LOGIN

| 방향 | 형식 |
|------|------|
| Request | `REQ<TAB>LOGIN<TAB>-<TAB>user_id<TAB>password` |
| Response | `RES<TAB>LOGIN<TAB>OK|FAIL<TAB>message` |

## 2. USER_ADD

| 방향 | 형식 |
|------|------|
| Request | `REQ<TAB>USER_ADD<TAB>-<TAB>user_id<TAB>password<TAB>department<TAB>name` |
| Response | `RES<TAB>USER_ADD<TAB>OK|FAIL<TAB>message` |

## 3. USER_DELETE

삭제 대상 식별 기준은 요구사항 미정이다.

| 방향 | 형식 |
|------|------|
| Request | `REQ<TAB>USER_DELETE<TAB>-<TAB>target` |
| Response | `RES<TAB>USER_DELETE<TAB>OK|FAIL<TAB>message` |

## 4. STUDENT_ADD

| 방향 | 형식 |
|------|------|
| Request | `REQ<TAB>STUDENT_ADD<TAB>-<TAB>name<TAB>student_id<TAB>department<TAB>grade<TAB>gpa<TAB>email<TAB>mobile_phone` |
| Response | `RES<TAB>STUDENT_ADD<TAB>OK|FAIL<TAB>message` |

## 5. STUDENT_SEARCH / STUDENT_DELETE / STUDENT_UPDATE

학생 검색 조건, 삭제 대상 식별 기준, 수정 대상 식별 기준과 수정 가능 필드는 요구사항 미정이다. 구현 시 `requirements.md` 범위를 벗어나지 않게 확정한다.

## 6. DEPT_LIST

| 방향 | 형식 |
|------|------|
| Request | `REQ<TAB>DEPT_LIST<TAB>-<TAB>department` |
| Response | `RES<TAB>DEPT_LIST<TAB>OK|FAIL<TAB>student_list_or_message` |
