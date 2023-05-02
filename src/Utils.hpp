#pragma once

#include <string>

// подразумевается, что SOURCE_PATH установлен через cmake
#ifndef SOURCE_PATH
#define SOURCE_PATH "ERROR"
#endif

namespace Utils
{
std::string getFilePath(std::string path);
};