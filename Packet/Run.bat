@echo off

REM proto -> desc, PacketID, c++, c# 哪颇老

cd /D "%~dp0"

set PROTOC_EXE=.\Protobuf\protoc.exe
set PYTHON_EXE=python
set PYTHON_SCRIPT=.\generate_packet.py

set PROTO_SOURCE_DIR=.\proto
set CPP_OUT_DIR=.\cpp
set CSHARP_OUT_DIR=.\csharp
set DESCRIPTOR_FILE=packet.desc

REM j2
set CPP_J2_DIR=.\jinja2\cpp
set CPP_J2_OUT=..\TestServer\Packet
set CSHARP_J2_DIR=.\jinja2\csharp
set CSHARP_J2_OUT=.\csharp
set CPP_CLIENT_J2_DIR=.\jinja2\cpp_client
set CPP_CLIENT_J2_OUT=..\DummyClient\Packet

set PACKET_ID_START=1

if not exist "%PROTOC_EXE%" (
	echo Error : "%PROTOC_EXE%"
	echo.
	pause
	exit /b 1
)

if not exist "%PYTHON_SCRIPT%" (
	echo error : "%PYTHON_SCRIPT%"
	echo.
	pause
	exit /b 1
)

if not exist "%CPP_OUT_DIR%" (
	mkdir "%CPP_OUT_DIR%"
)

if not exist "%CSHARP_OUT_DIR%" (
	mkdir "%CSHARP_OUT_DIR%"
)

echo start
echo.
echo =================================================
echo.

setlocal enabledelayedexpansion

REM .proto 颇老 捞抚 历厘
set PROTO_FILES_TO_COMPILE=
FOR %%P IN ("%PROTO_SOURCE_DIR%\*.proto") DO (
	set PROTO_FILES_TO_COMPILE=!PROTO_FILES_TO_COMPILE! %%~nxP
)

if "%PROTO_FILES_TO_COMPILE%"=="" (
	echo Error : not found proto file
) else (
	echo Compiling desc file ... %PROTO_FILES_TO_COMPILE%

	"%PROTOC_EXE%" --proto_path="%PROTO_SOURCE_DIR%" --descriptor_set_out="%DESCRIPTOR_FILE%" %PROTO_FILES_TO_COMPILE%

	if errorlevel 1 (
		echo ###############################
		echo ## Error: generate desc file ##
		echo ###############################
		echo.
		pause
		exit /b 1
	)
	echo Success desc file
)
endlocal
echo.

REM PacketID enum + j2 坊歹 积己
echo Start Python Script ...

%PYTHON_EXE% "%PYTHON_SCRIPT%" "%DESCRIPTOR_FILE%" "%PACKET_ID_START%" "%CPP_J2_DIR%" "%CPP_J2_OUT%" "%CSHARP_J2_DIR%" "%CSHARP_J2_OUT%" "%CPP_CLIENT_J2_DIR%" "%CPP_CLIENT_J2_OUT%"

if errorlevel 1 (
	echo ###########################
	echo ## Error : Python Script ##
	echo ###########################
	echo.
	pause
	exit /b 1
)
echo Finish Python Script
echo.

REM C++ 内靛 积己
echo Compiling C++ ...

FOR %%P IN ("%PROTO_SOURCE_DIR%\*.proto") DO (
	echo Compiling C++ ... "%%~nxP"

	"%PROTOC_EXE%" --proto_path="%PROTO_SOURCE_DIR%" --cpp_out="%CPP_OUT_DIR%" "%%~nxP"

	if errorlevel 1 (
		echo ##############################################
		echo ## Error: Protobuf C++ compile for "%%~nxP" ##
		echo ##############################################
		echo.
		pause
		exit /b 1
	)
)
echo Success C++
echo.

REM C# 内靛 积己
echo Compiling C# ...

FOR %%P IN ("%PROTO_SOURCE_DIR%\*.proto") DO (
	echo Compiling C# ... "%%~nxP"

	"%PROTOC_EXE%" --proto_path="%PROTO_SOURCE_DIR%" --csharp_out="%CSHARP_OUT_DIR%" "%%~nxP"

	if errorlevel 1 (
		echo #############################################
		echo ## Error: Protobuf C# compile for "%%~nxP" ##
		echo #############################################
		echo.
		pause
		exit /b 1
	)
)

echo Success C#
echo.

echo =================================================
echo.
echo finished
echo.

pause