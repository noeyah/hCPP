# C++/C# Portfolio

## ✨ 프로젝트 개요

 C++/C# 서버 개발을 위해 설계한 범용 라이브러리입니다.
 구현한 라이브러리를 사용하여 동일한 프로토콜로 통신하는 C++ 채팅 서버와 C# 채팅 서버를 각각 제작했습니다. 
 C#으로 작성된 클라이언트는 두 서버 중 하나에 선택적으로 접속할 수 있습니다.

### 📌 주요 기능
- C++ 라이브러리
	- IOCP 기반 비동기 네트워크
	- 스레드풀 + Job Queue 기반 작업 분배
	- 스핀락, Reader-Writer Lock 등 동시성 도구 구현
	- 비동기 로깅 및 ODBC DB 연결  풀 제공
	- ✔GTest : Google Test를 사용한 유닛 테스트
- C# 라이브러리
	- SocketAsyncEventArgs 기반 비동기 네트워크

### 💻 개발 환경
- OS : Windows 10
- IDE : Visual Studio 2022

### 🛠 사용 기술
- 언어 & 프레임워크 : C++ 20, C#, .NET 8.0
- 라이브러리 : [Protobuf 5.29.3](https://github.com/protocolbuffers/protobuf), [Google Test](https://github.com/google/googletest)
- 스크립트 언어 : Python 3.13.3


## ⚙ 빌드 가이드

### 🌞 빌드 방법

1. 소스 코드 다운로드
```
git clone https://github.com/noeyah/hCPP.git
```
2. Visual Studio에서 `hCPP.sln` 솔루션 파일 열기
	
3. 시작 프로젝트 설정 후 빌드 및 실행
	- C++ 채팅 서버 실행 시, `TestServer` + `TestClient` 프로젝트를 시작 프로젝트로 설정
	- C# 채팅 서버 실행 시, `TestServer_CS` + `TestClient` 프로젝트를 시작 프로젝트로 설정


### 🎀 실행 화면

#### C++ 채팅 서버 테스트
- C++ 서버(TestServer)에 C# 클라이언트(TestClient)와 C++ 클라이언트(DummyClient) 100개가 동시 접속하여 메시지를 전송합니다.
![hcpp_test](https://github.com/user-attachments/assets/277952f9-debb-48e2-ba0f-41b08e1494e3)


## 📚 프로젝트 구성

```
hCPP
├─hCPPLibrary : ⭐ C++ 라이브러리
│  └─Network, DB, Task, Lock, Memory, Thread, Log, ...
├─hCSharpLibrary : ⭐ C# 라이브러리
│  └─Network
├─GTest : hCPPLibrary 유닛 테스트
├─Packet : 패킷 정의 및 코드 생성 관련
│  └─cpp, csharp, jinja2, proto
├─TestServer : C++ 채팅 서버
├─TestServer_CS : C# 채팅 서버
├─DummyClient : C++ 더미 클라이언트
└─TestClient : C# 채팅 클라이언트 - WinForm
```

#### Libraries
- `hCPPLibrary` : C++로 작성된 핵심 기능 라이브러리
- `hCSharpLibrary` : C#으로 작성된 네트워크 라이브러리

#### Applications
- `TestServer` : C++ 채팅 서버
- `TestServer_CS` : C# 채팅 서버
- `DummyClient` : C++ 테스트용 클라이언트
- `TestClient` : C# WinForm 채팅 클라이언트

#### Tools & Tests
- `GTest` : hCPPLibrary의 유닛 테스트
- `Packet` : Protobuf 기반 패킷 정의 파일 및 코드 생성 스크립트 저장소



### hCPPLibrary : C++ 라이브러리

- 주요 기능

![hCPPLibrary](https://github.com/user-attachments/assets/80a93407-3101-47a4-8fd1-0e46addc3e65)

- 네트워크 구조

<img width="741" height="479" alt="Image" src="https://github.com/user-attachments/assets/e0c2c91c-28f9-46bc-8d75-40086d178cbb" />

### GTest : C++ 라이브러리의 유닛 테스트

- hCPPLibrary 모듈의 검증을 위한 유닛 테스트
- 실행 화면

![GTest](https://github.com/user-attachments/assets/6a9370b3-fb66-44b4-9bce-a2f8ec7d875d)


### TestClient : WinForm 클라이언트

- C# 네트워크 라이브러리 기능 검증을 위한 클라이언트
- 채팅 UI 제공

![TestClient](https://github.com/user-attachments/assets/0cf0df3b-1b1c-4669-908f-87338a02eb75)



## 참고 자료 및 출처
- [google test](https://github.com/google/googletest)
- [Protobuf 5.29.3](https://github.com/protocolbuffers/protobuf)
- MSDN
- 인프런 강의 : Rookiss님의 C++ 프로그래밍 입문 / 게임 서버
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
