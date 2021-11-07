#include <c2xcam.h>
#include <iostream>

int main(int argc, char** argv) {
    int ret1 = c2x::createCAM(1);
    int ret2 = c2x::createCAM(1);
    c2x::deleteCAM(1);
    
    return 0;
}