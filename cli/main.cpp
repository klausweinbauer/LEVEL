#include <boost/program_options.hpp>
#include <c2xcam.h>
#include <c2xdenm.h>
#include <iostream>
#include <fstream>
#include <string>

#define BUFFER_SIZE 65535

namespace po = boost::program_options;

void printError(int err) {
    char buffer[256];
    int len;
    c2x::getLastErrMsg(buffer, 256, &len);
    c2x::setLastErrMsg({}, 0);

    if (len == 0) {
        c2x::getErrMsg(err, buffer, 256);
    }

    std::cout << "[ERROR] " << buffer << std::endl;
}

int exportCAM(std::string path) 
{
    int err = c2x::createCAM(0);
    c2x::setCAMBasicVehicleContainerHighFrequency(0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
    if (err) printError(err);
    uint8_t buffer[BUFFER_SIZE];
    int len;
    err = c2x::encodeCAM(0, buffer, BUFFER_SIZE, &len);
    if (err < 0) {
        printError(err);
    } else {
        std::ofstream f;
        f.open(path);
        f << buffer;
        f.close();
    }

    c2x::deleteCAM(0);
}

void cliExport(const po::variables_map *vm) {
    if (!vm->count("file")) {
        std::cout << "You need to specify 'file' argument for output." << std::endl;
        return;
    }
    if (!vm->count("type")) {
        std::cout << "You have to specify a type." << std::endl;
    }
    std::string t = (*vm)["type"].as<std::string>();
    std::string f = (*vm)["file"].as<std::string>();
    if (t == "cam") {
        exportCAM(f);
    } else {
        std::cout << "Not implemented yet." << std::endl;
    }
}

int main(int argc, char** argv) 
{
    po::options_description desc_usage("Allowed usage");
    desc_usage.add_options()
        ("help", "shows this help message")
        ("version", "shows the program version")
        ("export", "export xml skeleton fro CAM or DENM")
    ;
    po::options_description desc_options("Configuration");
    desc_options.add_options()
        ("file", po::value<std::string>(), "file path")
        ("type", po::value<std::string>()->default_value("cam"), "message type; possible values: cam, denm")
    ;
    po::options_description cmdline_options;
    cmdline_options.add(desc_usage).add(desc_options);
    po::variables_map vm;
    po::store(po::parse_command_line(argc, argv, cmdline_options), vm);
    po::notify(vm);

    if (vm.count("help")) {
        std::cout << cmdline_options << std::endl;
    } else if (vm.count("version")) {
        std::cout << "clic2x version 0.1.0" << std::endl;
    } else if (vm.count("export")) {
        cliExport(&vm);
    } else {
        std::cout << cmdline_options << std::endl;
    }

    return 0;
}