
## ✨ 프로젝트 개요

 이 프로젝트는 Windows 환경에서 동작하는 C++ 서버 프레임워크입니다.
 확장성, 안정성, 그리고 생산성을 목표로 서버의 핵심 컴포넌트를 직접 설계하고 구현했습니다.
 구현한 라이브러리를 사용하여 동일한 프로토콜로 통신하는 C++/C# 테스트 서버를 각각 제작했습니다. C#으로 작성된 클라이언트는 두 서버 중 하나에 선택적으로 접속할 수 있습니다.

### 📌 주요 기능
- **비동기 네트워크** : Windows의 IOCP(I/O Completion Port) 기반 비동기 소켓 통신
- **I/O와 로직의 분리** : Job Queue를 이용해 네트워크 I/O 스레드와 게임 로직 스레드를 분리하여 병목 현상 최소화
- **멀티스레드 및 동시성** : SpinLock, SharedSpinLock 및 Thread Pool을 통한 안정적인 동시성 제어
- **유연한 비즈니스 로직** : Behavior Tree(행동 트리) 라이브러리를 구현하여 AI와 같은 복잡한 로직을 모듈화
- **프로토콜 및 직렬화** : Protobuf를 이용한 패킷 직렬화 및 파서 자동 생성 스크립트 작성
- **기타 서버 기반 요소** : 시간 기반 작업을 위한 Scheduler, DB Connection Pool, Memory Pool 등 구현


### 💻 개발 환경
- OS : Windows 10
- IDE : Visual Studio 2022

### 🛠 사용 기술
- 언어 & 프레임워크 : C++ 20, C#, .NET 8.0
- 라이브러리 : [Protobuf 5.29.3](https://github.com/protocolbuffers/protobuf), [Google Test](https://github.com/google/googletest)
- 스크립트 언어 : Python 3.13.3


## 📚 프로젝트 구조

- **hCPPLibrary** : 핵심 공용 라이브러리
  - `Network/` : IOCP 기반 네트워크 엔진
  - `Thread/`, `Lock/` : 스레드 풀, 스핀락 등 동시성 관련 모듈
  - `Behavior/` : 행동 트리(Behavior Tree) 라이브러리
  - `DB/` : 데이터베이스 커넥션 풀
  - `Memory/` : 메모리 풀, 오브젝트 풀
- **Packet**: Protobuf 기반 패킷 정의 및 코드 생성 스크립트
  - `proto/` : `.proto` 패킷 원형 파일
  - `generate_packet.py` : C++/C# 코드를 생성하는 Python 스크립트
- **TestServer** : `hCPPLibrary`를 사용하여 만든 테스트용 서버
- **DummyClient** : 테스트용 C++ 클라이언트
- **TestClient** : 테스트용 WinForm 클라이언트
- **GTest**: 라이브러리 기능 단위 테스트 (Google Test)


## 🧩 아키텍처

- I/O 작업과 로직 처리를 분리하는 멀티스레드 모델입니다.

<img width="401" height="271" alt="Image" src="https://github.com/user-attachments/assets/f571711f-2208-4387-80c4-5ace806826fc" />

- **Network (IOCP)** : I/O 스레드가 클라이언트의 접속 및 데이터 수신을 처리합니다.
- **Job Queue** : 처리해야 할 작업(수신된 패킷, 예약된 작업 등)이 쌓이는 중앙 큐입니다.
- **Thread Pool** : 워커 스레드들이 Job Queue에서 작업을 꺼내와 실제 비즈니스 로직(패킷 처리, AI 업데이트 등)을 실행합니다.
- **Scheduler** : 시간 기반 작업을 Job Queue에 등록합니다.



## ⚙ 빌드 가이드

### 🌞 빌드 방법

1. 소스 코드 다운로드합니다.
```
git clone https://github.com/noeyah/hCPP.git
```
2. Visual Studio에서 `hCPP.sln` 솔루션 파일을 엽니다.
	
3. 시작 프로젝트 설정 후 빌드 및 실행합니다.
	- C++ 서버 실행 시, `TestServer` + `TestClient` 프로젝트를 시작 프로젝트로 설정합니다.
	- 더미 클라이언트 필요 시, `DummyClient` 프로젝트를 추가합니다.
	- C# 서버 실행 시, `TestServer_CS` + `TestClient` 프로젝트를 시작 프로젝트로 설정합니다.


## 🎀 실행 화면

### C++ 서버 테스트
- C++ 서버(TestServer), C# 클라이언트(TestClient), C++ 클라이언트(DummyClient)
- 더미 100개가 접속하여 방 입장, 퇴장, 채팅을 합니다.

![hcpp_test](https://github.com/user-attachments/assets/f75131fb-177c-42a1-acbc-c8644f87d3ed)


### GTest : C++ 라이브러리 기능 단위 테스트

![GTest](https://github.com/user-attachments/assets/6a9370b3-fb66-44b4-9bce-a2f8ec7d875d)


### TestClient : WinForm 클라이언트
- 채팅 UI 제공

![TestClient](https://github.com/user-attachments/assets/0cf0df3b-1b1c-4669-908f-87338a02eb75)



## 참고 자료 및 출처
- [google test](https://github.com/google/googletest)
- [Protobuf 5.29.3](https://github.com/protocolbuffers/protobuf)
- MSDN
- 인프런 강의
- https://tango1202.github.io/categories/cpp-stl/
- 모두의 코드 C++
- [ReaderWriterLockSlim.cs](https://github.com/microsoft/referencesource/blob/master/System.Core/System/threading/ReaderWriterLockSlim/ReaderWriterLockSlim.cs)
- https://github.com/zeliard/Dispatcher
- https://github.com/jacking75/SuperSocketLite
- https://github.com/jacking75/edu_cpp_IOCP
- https://github.com/sunduk/FreeNet
- Microsoft IOCP 서버 예제
- https://laivy.tistory.com/38
- https://bio200127.tistory.com/107
- https://minttea25.tistory.com/128
- TestClient 프로필 이미지 출처 : [FLATICON](https://www.flaticon.com/)
