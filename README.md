# Optimus KV Store (toy)

간단한 로그 구조 키-값 저장소 예제입니다.  
`data.log`에 put/get/del 연산을 기록하며, get은 로그를 처음부터 끝까지 스캔해 최종 상태를 계산합니다.

---

## 특징
- append-only 로그 방식
- 텍스트 기반 (탭, 개행 구분)
- C++17 표준 라이브러리만 사용
- 학습·프로토타입용, 동시성/인덱스 미지원

---

## 빌드

```bash
clang++ -std=c++17 -O2 -o optimus_kv src/main.cpp
# 또는
g++ -std=c++17 -O2 -o optimus_kv src/main.cpp
```

---

## 실행 방법

```bash
./optimus_kv put <key> <value>    # 값 저장
./optimus_kv get <key>             # 값 조회
./optimus_kv del <key>             # 키 삭제
```

*키/값에 탭(\t), 개행(\n) 불가*

---

## 입출력 예시

```bash
$ ./optimus_kv put user alice
OK

$ ./optimus_kv get user
alice

$ ./optimus_kv del user
OK

$ ./optimus_kv get user
(nil)

$ ./optimus_kv put note ""
OK

$ ./optimus_kv get note
# (빈 줄 출력)
```

---

## 데이터 파일 포맷

- 쓰기(put): `P<TAB>key<TAB>value<NEWLINE>`
- 삭제(del): `D<TAB>key<NEWLINE>`

---

## 반환 코드

- put, del: 성공 0, 실패 1
- get: 항상 0 (없으면 `(nil)` 출력)

---

## 동작 원리

- 쓰기/삭제: 로그 파일에 한 줄씩 append
- 조회: 로그 파일 처음부터 끝까지 스캔해 최종 값 계산

---

## 제약 사항 & 주의점

- 읽기 성능 저하: 로그 크기 증가 시 느려짐
- 키/값에 탭, 개행 불가
- 동시성, 내구성, 무결성, 인덱스 미지원
- 원자성/격리성 미보장

---

## 향후 개선 아이디어

- 로그 컴팩션
- 인덱스/세그먼트 도입
- 동시성 지원
- 내구성 강화 (fsync, 체크섬)
- 포맷 개선 (이스케이프, 길이 접두)
- 에러 복구, CLI 확장, 테스트 강화

---

## 정리/초기화

```bash
rm -f data.log
# 또는
: > data.log
```

---

## 라이선스

이 프로젝트는 [MIT License](LICENSE) 하에 배포됩니다.

---

## 빠른 참고 (치트시트)

```bash
# 빌드
clang++ -std=c++17 -O2 -o optimus_kv src/main.cpp

# 사용
./optimus_kv put foo bar
./optimus_kv get foo
./optimus_kv del foo

# 로그 확인
cat data.log

# 초기화
: > data.log
```

---

문의 및 개선 아이디어 환영합니다. 간단한 컴팩션/인덱싱 버전도 제작 가능합니다.