#include "Utils.hpp"

std::string Utils::getFilePath(std::string path)
{
    static std::string _SOURCE_PATH(SOURCE_PATH);
    return _SOURCE_PATH + path;
}