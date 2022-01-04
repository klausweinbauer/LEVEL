#include <Logger.hpp>

using namespace c2x;

Logger::Logger(std::string path) : path_(path) {}

int Logger::writeLog(std::string group_name, char* buffer, int size)
{
    return 0;
}