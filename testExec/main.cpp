#include <c2xcam.h>
#include <iostream>
#include <unistd.h>

int main(int argc, char** argv) {
    c2x::createCAM(1);
    c2x::createCAM(2);
    int ids[]{1, 2};

    c2x::startCAMReceiver(1997);
    c2x::startCAMTransmitter(1997);
    c2x::CAMTransmitter(ids, 2);
    usleep(2000000);
    c2x::stopCAMTransmitter();
    c2x::stopCAMReceiver();

    c2x::deleteCAM(1);
    c2x::deleteCAM(2);
    
    return 0;
}