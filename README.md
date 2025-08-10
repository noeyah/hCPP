# C++/C# Portfolio

## ✨ 프로젝트 개요

C++과 C#으로 서버 개발에 필요한 네트워크, DB 등의 라이브러리를 구현한 프로젝트입니다.

구현한 라이브러리를 사용하여 동일한 프로토콜로 통신하는 C++ 채팅 서버와 C# 채팅 서버를 각각 제작했습니다. C#으로 작성된 클라이언트는 두 서버 중 하나에 선택적으로 접속할 수 있습니다.

### 📌 주요 기능
- 📚 자체 제작 라이브러리
	- C++ 라이브러리
		- IOCP 기반 비동기 네트워크 서비스
		- ODBC를 사용한 DB 연결 및 쿼리 실행
		- Job Queue를 이용한 비동기 병렬 처리 시스템
		- GTest : Google Test를 사용한 유닛 테스트
	- C# 라이브러리
		- SocketAsyncEventArgs 기반 비동기 네트워크 서비스
- 🖥 구현 애플리케이션
	- C++ 채팅 서버 및 C# 채팅 서버
	- C# WinForm 채팅 클라이언트
	- C++ 더미 클라이언트
- 🛠 개발 도구
	- Python/Jinja2 스크립트를 이용한 패킷 코드 생성 자동화
		- .proto 파일로부터 C# 및 C++ 패킷 ID, 핸들러 등록 코드 등을 생성

### 💻 개발 환경
- OS : Windows 10
- IDE : Visual Studio 2022

### 🛠 사용 기술
- 언어 & 프레임워크 : C++ 20, C#, .NET 8.0
- 라이브러리 & 툴 : Protobuf, Jinja2, Google Test
- 스크립트 언어 : Python


## ⚙ 빌드 가이드

### 🔨 외부 라이브러리/툴
- Protobuf : 5.29.3 버전 사용 ([Link](https://github.com/protocolbuffers/protobuf))
- Google Test : GTest 프로젝트에서 사용 ([Link](https://github.com/google/googletest))
- Python : 패킷 자동 생성 스크립트 실행에 Python 3.13.3 사용 (Protobuf, Jinja2 라이브러리 필요)


### 🌞 빌드 방법

1. 소스 코드 다운로드
```
git clone https://github.com/noeyah/hCPP.git
```
2. Visual Studio에서 `hCPP.sln` 솔루션 파일 열기
	
3. 시작 프로젝트 설정 후 빌드 및 실행
	- C# 채팅 앱 실행 시, `TestServer_CS` + `TestClient` 프로젝트를 시작 프로젝트로 설정
	- C++ 채팅 앱 실행 시, `TestServer` + `TestClient` 프로젝트를 시작 프로젝트로 설정

### 🎀 실행 화면

#### C++ 서버 테스트
- C++ 서버(TestServer)에 C# 클라이언트(TestClient)와 C++ 클라이언트(DummyClient) 100개가 동시 접속하여 메시지를 전송합니다.
![hcpp_test](https://github.com/user-attachments/assets/277952f9-debb-48e2-ba0f-41b08e1494e3)


## 📚 프로젝트 구성

```
hCPP
├─hCPPLibrary (⭐ C++ 라이브러리)
│  └─Network, DB, Task, Lock, Memory, Thread, Log, ...
├─hCSharpLibrary (⭐ C# 라이브러리)
│  └─Network
├─GTest (hCPPLibrary 유닛 테스트)
├─Packet (패킷 정의 및 코드 생성 관련)
│  └─cpp, csharp, jinja2, proto
├─TestServer (C++ 채팅 서버)
├─TestServer_CS_ (C# 채팅 서버)
├─DummyClient (C++ 더미 클라이언트)
└─TestClient (C# 채팅 클라이언트 - WinForm)
```

### Libraries
- `hCPPLibrary` : C++로 작성된 핵심 기능 라이브러리
- `hCSharpLibrary` : C#으로 작성된 네트워크 라이브러리

### Applications
- `TestServer` : hCPPLibrary를 사용하는 C++ 채팅 서버
- `TestServer_CS` : hCSharpLibrary를 사용하는 C# 채팅 서버
- `DummyClient` : 스트레스 테스트를 위한 C++ 더미 클라이언트
- `TestClient` : hCSharpLibrary를 사용하는 C# WinForm 채팅 클라이언트

### Tools & Tests
- `GTest` : hCPPLibrary의 유닛 테스트 프로젝트
- `Packet` : Protobuf 기반 패킷 정의 파일 및 코드 생성 스크립트 저장소



#### hCPPLibrary : C++ 라이브러리
- 주요 기능

![hCPPLibrary](https://github.com/user-attachments/assets/80a93407-3101-47a4-8fd1-0e46addc3e65)

- 네트워크 구조

<img width="741" height="479" alt="Image" src="https://github.com/user-attachments/assets/e0c2c91c-28f9-46bc-8d75-40086d178cbb" />

#### GTest : C++ 라이브러리의 유닛 테스트

- hCPPLibrary 모듈의 검증을 위한 유닛 테스트
- 실행 화면

![GTest](https://github.com/user-attachments/assets/6a9370b3-fb66-44b4-9bce-a2f8ec7d875d)


#### Packet

- 패킷 저장소
- Python 스크립트를 사용하여 proto 파일에서 추출한 메시지로 PacketID, 패킷 매핑, Enum 문자열 변환 등 C++, C# 코드를 생성합니다.

![ProtoToCode](https://github.com/user-attachments/assets/24b19802-524b-462a-a339-352ad18cdca0)


#### TestClient : WinForm 클라이언트

- C# 네트워크 라이브러리 기능 검증을 위한 클라이언트
- 채팅 UI 제공

![TestClient](https://github.com/user-attachments/assets/0cf0df3b-1b1c-4669-908f-87338a02eb75)

## 라이브러리 사용 예시

### hCPPLibrary

#### Task

- 잡큐에 작업 등록
```cpp
#include "Task/TaskUtil.h"
IJobQueue& jobQueue_;

PushJob(jobQueue_, [](){
	// 스레드풀의 워커 스레드에 의해 실행될 작업
	std::cout << “Hello” << std::endl;
});
```
- 스케줄러를 통한 예약 작업 등록
```cpp
#include "Task/Scheduler.h"

// Scheduler 객체에 IJobQueue 주입 필요
scheduler_.PushAfter(1234, [](){
	// 1234ms 후 스레드풀의 워커 스레드에 의해 실행될 작업
	std::cout << “World” << std::endl;
});
```

#### Database

- 커넥션풀 세팅 이후 Select 예시
```cpp
#include "DB/DBConnectionPool.h"
#include "DB/DBConnection.h"
#include "DB/DBQuery.h"

auto conn = pool_.Pop();
DBQuery<1, 2> query(*conn.get(), “SELECT ?, ? FROM ... WHERE ... = ?”);
query.Param(1, param1)
	.Column(1, col1)
	.Column(2, col2); 
query.Execute();
while (query.Fetch())
{
	// row data (col1, col2)
}
// 이후 conn 스마트 포인터 소멸 시 풀에 자동 반납
```

#### Network

- NetServer 초기화 및 시작 예시
```cpp
#include "Task/JobQueue.h"
#include "Task/ThreadPool.h"
#include "Network/NetConfig.h"
#include "Network/NetService/NetServer.h"

class MainServer
{
public:
	MainServer(NetServerConfig config) 
		: threadPool_(jobQueue_), 
		// NetServer에 NetServerConfig, IJobQueue, Session 팩토리 주입
		netServer_(config, jobQueue_, []() -> std::shared_ptr<ClientSession> {
			// 사용자 Session 팩토리
			return MakeSharedPtr<ClientSession>(); 
		}) 
	{
	}
	void Start()
	{
		threadPool_.Start();
		netServer_.Start();
	}
private:
	JobQueue jobQueue_;
	ThreadPool threadPool_;
	NetServer netServer_;
}
```
- 사용자가 Session 클래스 가상 함수 재정의
```cpp
#include "Network/Connection/Session.h"

class ClientSession : public Session
{
protected:
	// 재정의 필요
	virtual void OnConnected() override;
	virtual void OnDisconnected() override;
	virtual void OnReceiveainServer(uint16_t packetId, std::span<const std::byte> packet) override;
}
```

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
