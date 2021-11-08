#include <c2xcam.h>
#include <iostream>
#include <c2xdenm.h>

#ifndef WIN32
#include <unistd.h>
#endif

int main(int argc, char** argv) {
    c2x::createDENM(1, 1);
    uint8_t buffer[4096];

    int ret = c2x::encodeDENM(1, 1, buffer, 4096, nullptr);


    c2x::deleteDENM(1, 1);
    return 0;
}