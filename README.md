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

### 📚 프로젝트 구성
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

## 빌드 방법



## 출처
- 아이콘