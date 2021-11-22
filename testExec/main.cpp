#include <c2xcam.h>
#include <iostream>
#include <c2xdenm.h>
#include <sstream>

#ifndef WIN32
#include <unistd.h>
#endif

int main(int argc, char** argv) {
    int id = 1;c2x::createCAM(2, 1);
    uint8_t buffer[65000];
    int size;
    int ret = c2x::encodeCAM(id, buffer, 65000, &size);
    std::cout << ret << std::endl;

    return 0;
}