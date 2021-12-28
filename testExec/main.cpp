#include <c2xcam.h>
#include <iostream>
#include <c2xdenm.h>
#include <sstream>

#ifndef WIN32
#include <unistd.h>
#endif

void client()
{
    c2x::startCAMReceiver(1997);
    int speed;
    
    while (1)
    {
        int err = c2x::getCAMBasicVehicleContainerHighFrequency(1, nullptr, nullptr, &speed, nullptr, nullptr, nullptr, nullptr, 
            nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr);
        if (err) {
            char buf[128];
            c2x::getErrMsg(err, buf, 128);
            std::cout << "[WARN] " << buf << std::endl;
        } else {
            std::cout << "[INFO] Speed=" << speed << std::endl;
        }
        usleep(500000);
    }
}

void server(int t, double f)
{
    std::cout << "[INFO] Start server" << std::endl;
    c2x::createDENM(2, 1);
    c2x::setDENMManagementContainer(2, 1, 2, 0, 0, 0, 0, 0, 0, 0, 15);
    c2x::setDENMSituationContainer(2, 1, 7, 18, 0);

    c2x::setDENMTransmissionSource(2, 1);
    c2x::setDENMTransmissionFrequency(f);
    std::cout << "[INFO] Start transmitting" << std::endl;
    c2x::startDENMTransmitter(1998);
    usleep(t * 1000000);
    c2x::setDENMManagementContainer(2, 1, 2, 0, 0, 1, 0, 0, 0, 0, 15);
    usleep(1000000 / f);
    std::cout << "[INFO] Stop transmitting" << std::endl;
    c2x::stopDENMTransmitter();
    std::cout << "[INFO] Stop server" << std::endl;
    c2x::deleteDENM(2, 1);
}

void printHelp() {
    std::cout << "Application for receiving CAMs and sending DENMs." << std::endl << std::endl;
    std::cout << "Usage: " << std::endl;
    std::cout << "  test-exec {c|s} for use as CAM Client or DENM Server" << std::endl;
}

int main(int argc, char** argv) {
    
    if (argc != 2) {
        printHelp();
    } else {
        if (*argv[1] == 'c') {
            client();
        } else if (*argv[1] == 's') {
            server(5, 5);
        } else {
            printHelp();
        }
    }

    return 0;
}