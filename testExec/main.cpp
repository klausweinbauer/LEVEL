#include <c2xcam.h>
#include <iostream>

int main(int argc, char** argv) {
    std::cout << "Hello World" << std::endl;
    int id = c2x::createCAM();
    c2x::setCAMHeader(id, 1, 2, 3);

    uint8_t buffer[4096];    
    int size = c2x::encodeCAM(id, buffer, 4096);
    int newId, newProtVers, newMsgId, newStatId;
    c2x::decodeCAM(&newId, buffer, size);
    c2x::getCAMHeader(newId, &newProtVers, &newMsgId, &newStatId);
    std::cout << newProtVers << " " << newMsgId << " " << newStatId << std::endl;

    
    return 0;
}