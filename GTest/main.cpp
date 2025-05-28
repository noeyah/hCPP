#include <gtest/gtest.h>
#include "Memory/Memory.h"

// https://github.com/google/googletest
// gtest-all.cc ÇÊ¿ä

int main(int argc, char** argv)
{
	// hlib init
	hlib::Memory::Instance();

	::testing::InitGoogleTest(&argc, argv);

	return RUN_ALL_TESTS();
}