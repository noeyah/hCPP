#include <gtest/gtest.h>
#include <WinCommon.h>
#include "Memory/Memory.h"

// https://github.com/google/googletest
// gtest-all.cc ÇÊ¿ä

int main(int argc, char** argv)
{
	SetConsoleOutputCP(CP_UTF8);
	SetConsoleCP(CP_UTF8);

	// hlib init
	hlib::Memory::Instance();

	::testing::InitGoogleTest(&argc, argv);

	return RUN_ALL_TESTS();
}