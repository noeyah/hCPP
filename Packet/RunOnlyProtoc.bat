@echo off

REM proto -> c++, c# 컴파일
REM PacketID 제외임

cd /D "%~dp0"

set PROTOC_EXE=.\Protobuf\protoc.exe
set PROTO_SOURCE_DIR=.\proto
set CPP_OUT_DIR=.\cpp
set CSHARP_OUT_DIR=.\csharp

if not exist "%CPP_OUT_DIR%" (
	mkdir "%CPP_OUT_DIR%"
)
if not exist "%CSHARP_OUT_DIR%" (
	mkdir "%CSHARP_OUT_DIR%"
)

echo start
echo.
echo ==========================================
echo.

FOR %%P IN ("%PROTO_SOURCE_DIR%"\*.proto) DO (
	echo compiling ... "%%P"

	"%PROTOC_EXE%" --proto_path="%PROTO_SOURCE_DIR%" --cpp_out="%CPP_OUT_DIR%" "%%P"

	if errorlevel 1 (
		echo ##########################################
		echo ## Error : C++ compile failed for "%%P" ##
		echo ##########################################
		echo.
		pause
		exit /b 1
	)
	echo c++ success

	"%PROTOC_EXE%" --proto_path="%PROTO_SOURCE_DIR%" --csharp_out="%CSHARP_OUT_DIR%" "%%P"

	if errorlevel 1 (
		echo #########################################
		echo ## Error : C# compile failed for "%%P" ##
		echo #########################################
		echo.
		pause
		exit /b 1
	)
	echo c# success

	echo.
)

echo ==========================================
echo.
echo finished
echo.

pause
