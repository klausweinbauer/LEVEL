#include <c2xcam.h>
#include <iostream>

int main(int argc, char** argv) {
    int id = c2x::createCAM();
    std::cout << "Id: " << id << std::endl;
    c2x::deleteCAM(id);
    return 0;
}