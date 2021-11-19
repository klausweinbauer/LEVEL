#include <c2xcam.h>
#include <iostream>
#include <c2xdenm.h>
#include <sstream>

#ifndef WIN32
#include <unistd.h>
#endif

int main(int argc, char** argv) {
    std::stringstream ss;
    ss << "Hello World" << std::endl;
    c2x::setLastErrMsg(ss.str().c_str(), ss.str().size());

    char buffer[128];
    int actualSize = 0;
    c2x::getLastErrMsg(buffer, 128, &actualSize);
    return 0;
}