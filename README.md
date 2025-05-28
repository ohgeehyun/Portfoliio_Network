
# 🧵 IOCP 기반 비동기 네트워크 서버 구조

이 프로젝트는 Windows의 IOCP(I/O Completion Port)를 활용하여 고성능 비동기 네트워크 서버를 구현합니다.

## 구성 요소

- **`IocpObject`**
  - IOCP에서 사용할 키(Key) 역할을 하며, `GetHandle()`과 `Dispatch()`를 순수 가상 함수로 갖는 베이스 클래스입니다.
  - 이 객체를 IOCP에 등록합니다.

- **`IocpCore`**
  - `CreateIoCompletionPort()`를 통해 IOCP 핸들을 생성 및 관리합니다.
  - `Dispatch()`를 통해 `GetQueuedCompletionStatus()`로 이벤트를 받아 처리합니다.
  - `Register()` 함수로 `IocpObject`를 IOCP에 연결합니다.

- **`IocpEvent`**
  - 실제 Overlapped 구조체 기반의 이벤트 정보를 담는 클래스입니다.
  - 이벤트가 완료되면 `Dispatch()`를 통해 원 소유자(`IocpObject`)에게 전달됩니다.

---

## 🌐 전체 처리 흐름

```mermaid
flowchart TD
    A[시작 - 서버 초기화] --> B[IOCP 핸들 생성CreateIoCompletionPort]
    B --> C[IocpObject 소켓 등록]
    C -->|소켓 핸들 전달| D[IOCP와 연결]
    D --> E[비동기 I/O 요청 시작]
    E --> F[OS 커널에서 I/O 완료 이벤트 대기]
    F --> G[이벤트 완료 시<br>GetQueuedCompletionStatus 호출]
    G --> H[IocpEvent 추출]
    H --> I[IocpObject::Dispatch 호출]
    I --> J[이벤트 처리 완료]
    J --> K[다음 이벤트 처리 또는 종료]
```

---

## 🔄 예외 처리 흐름

```mermaid
flowchart TD
    A[GetQueuedCompletionStatus 실패] --> B{에러 코드 확인}
    B -->|WAIT_TIMEOUT| C[타임아웃 - 아무 작업 없음]
    B -->|기타 에러| D[IocpObject::Dispatch로 에러 처리 위임]
```

---

## ✅ 기타 참고

- IOCP는 높은 동시성을 지원하며, 이벤트 기반 구조 덕분에 불필요한 스레드 사용 없이도 높은 성능을 발휘합니다.
- `shared_from_this()`를 이용한 스마트 포인터 사용으로 객체 생명 주기를 안정적으로 관리합니다.
