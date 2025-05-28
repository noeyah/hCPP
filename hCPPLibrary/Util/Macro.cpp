#include "Macro.h"
#include <iostream>
#include <syncstream>
#include <cstdlib>
#include <filesystem>

namespace hlib::macro
{
	void _Crash(const std::string& message, const std::source_location& location)
	{
		auto fullPath = location.file_name();
		std::filesystem::path filePath(fullPath);
		auto fileNamePath = filePath.filename();

		{
			std::osyncstream stream(std::cerr);

			stream << "[CRASH] " << message << "\n";
			stream << std::format("    at {}:{}:{} ({})",
								  fileNamePath.string(),
								  location.line(),
								  location.column(),
								  location.function_name()) << std::endl;
		}

		__debugbreak();
		std::abort();
	}
}