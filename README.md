# hCPP

## ✨ 개요

이 프로젝트는 서버 개발을 위한 C++ 라이브러리(네트워크, 태스트, 메모리, DB 등)와 C# 라이브러리(네트워크)를 직접 설계 및 구현합니다. 
C++ 라이브러리는 Google Test를 통해 유닛 테스트를 수행하여 안정성을 확보하고자 했습니다. 
이 라이브러리들의 네트워크 통신 검증 및 예제를 위해 테스트 서버 및 다양한 테스트 클라이언트를 포함합니다. 

### 💻 개발 환경
- OS : Windows 10
- IDE : Visual Studio 2022

### 🛠 사용 기술
- 언어 & 프레임워크 : C++ 20, C#, .NET 8.0
- 라이브러리 & 툴 : Protobuf, Jinja2, Google Test
- 스크립트 언어 : Python


## ⚙ 빌드 가이드

### 🔨 외부 라이브러리/툴
- [google test](https://github.com/google/googletest)
- [Protobuf 5.29.3](https://github.com/protocolbuffers/protobuf)
- Python 3.13.3
	- Jinja2 라이브러리 설치 : pip install Jinja2
	- Protobuf 라이브러리 설치 : pip install protobuf

### 🌞 빌드 방법

1. 소스 코드 다운로드
```
git clone https://github.com/noeyah/hCPP.git
```
2. 외부 라이브러리 및 프로젝트 참조 설정
	- 본 프로젝트를 빌드하고 실행하기 위해서는 다음과 같은 외부 라이브러리 설정 및 프로젝트 참조가 필요합니다.
	- Protobuf
		- Packet 프로젝트에서 Protobuf 사용 준비가 되어야 합니다.
	- Google Test
		- GTest 프로젝트에 gtest-all.cc 파일을 직접 추가하여 함께 컴파일합니다.
	- 프로젝트 간 참조 설정
		- 다음 프로젝트 참조가 올바르게 설정되어 있는지 확인합니다.
		- TestServer : hCPPLibrary(C++라이브러리), Packet 프로젝트 참조, Packet 프로젝트의 cpp 폴더 포함
		- DummyClient : hCPPLibrary(C++라이브러리), Packet 프로젝트 참조, Packet 프로젝트의 cpp 폴더 포함
		- TestClient : hCSharpLibrary(C#라이브러리), Packet 프로젝트의 csharp 폴더 내 cs 파일 포함
3. Visual Studio 솔루션 빌드
	- hCPP.sln 파일을 Visual Studio 2022로 엽니다.
	- 솔루션 빌드를 진행합니다.
	- 빌드된 파일을 실행합니다.

## 📚 프로젝트 구성
```
hCPP
├─DummyClient (C++ 라이브러리 테스트용 클라이언트)
├─GTest (C++ 라이브러리 유닛 테스트)
├─hCPPLibrary (⭐C++ 라이브러리)
│  ├─Command
│  ├─DB
│  ├─Lock
│  ├─Log
│  ├─Memory
│  ├─Network
│  ├─Task
│  ├─Thread
│  └─Util
├─hCSharpLibrary (C# 라이브러리)
│  └─Network
├─Packet (C++)
│  ├─cpp
│  ├─csharp
│  ├─jinja2
│  └─proto
├─TestClient (C# 라이브러리 테스트용 클라이언트 - WinForm)
└─TestServer (C++ 라이브러리 테스트용 서버)
```

### hCPPLibrary (C++ 라이브러리)

- Network : IOCP 기반 비동기 네트워크 및 서비스 인터페이스 제공
- Task : 비동기 작업 처리를 위한 잡큐 및 스레드풀, 스케줄러 제공
- Lock : 스핀락, 공유 스핀락 등 동시성 처리 지원
- Memory : PMR 기반의 메모리 리소스 관리, 커스텀 할당자 제공
- DB : ODBC를 사용하여 DB 연결 및 SQL 실행 기능 제공
- Thread : 스레드 관리 지원
- Log : 파일 및 콘솔 출력 로그 시스템 제공
- 그 외 Command, Util : 보조 유틸리티


### GTest

- 실행 화면

![GTest](https://github.com/user-attachments/assets/6a9370b3-fb66-44b4-9bce-a2f8ec7d875d)


### 테스트용 프로젝트
- TestServer, TestClient, DummyClient

- TestServer 실행 화면

![TestServer](https://github.com/user-attachments/assets/79358804-77c3-4a15-bb88-53466a425e2a)

- TestClient 실행 화면

![TestClient](https://github.com/user-attachments/assets/0cf0df3b-1b1c-4669-908f-87338a02eb75)



## 참고 자료 및 출처
- MSDN
- 인프런 강의 : Rookiss님의 C++ 프로그래밍 입문 / 게임 서버
- https://tango1202.github.io/categories/cpp-stl/
- ssanta님의 포트폴리오
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