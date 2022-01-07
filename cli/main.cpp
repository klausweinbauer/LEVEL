#include <boost/program_options.hpp>
#include <boost/filesystem.hpp>
#include <c2xcam.h>
#include <c2xdenm.h>
#include <iostream>
#include <fstream>
#include <string>
#include <regex>
#include <ctime>
#include <chrono>

#define BUFFER_SIZE 65535
#define CAM_LOG_DIR "log_cam"
#define DENM_LOG_DIR "log_denm"

namespace po = boost::program_options;
namespace fs = boost::filesystem;

static bool use_filter = false;
static std::string filter;
static bool log_enabled = false;
static std::string log_path;

void throwError(int err);
void cliTransmitter(bool is_cam, int port, double f, int id, int seqNr);
void cliReceiver(bool is_cam, int port);
void initOptions(po::option_description *cmdline_options);
void run(po::variables_map &vm);
void initCAMContainer(std::string desc, int id);
void initDENMContainer(std::string desc, int id, int seqNr);
void exportCAM(bool use_file, std::string file, int id);
void exportDENM(bool use_file, std::string file, int id, int seqNr);
void import(std::string file, bool is_cam, int *id, int *seqNr);
void logCAM(int id);
void logDENM(int id, int seqNr);

void throwError(int err) {
    char buffer[256];
    int len;
    c2x::getLastErrMsg(buffer, 256, &len);
    c2x::setLastErrMsg({}, 0);

    if (len == 0) {
        c2x::getErrMsg(err, buffer, 256);
    }

    std::stringstream ss;
    ss << buffer << std::endl;
    throw std::invalid_argument(ss.str());
}

void exportCAM(bool use_file, std::string file, int id) 
{
    uint8_t buffer[BUFFER_SIZE];
    int len;
    int err = c2x::encodeCAM(id, buffer, BUFFER_SIZE, &len, c2x::XER_BASIC);
    if (err < 0) {
        throwError(err);
    }

    if (use_file) {
        std::ofstream f;
        f.open(file);
        f << buffer;
        f.close();
    } else {
        std::cout << buffer << std::endl;
    }
}

void exportDENM(bool use_file, std::string file, int id, int seqNr) 
{
    uint8_t buffer[BUFFER_SIZE];
    int len;
    int err = c2x::encodeDENM(id, seqNr, buffer, BUFFER_SIZE, &len, c2x::XER_BASIC);
    if (err < 0) {
        throwError(err);
    } 

    if (use_file) {
        std::ofstream f;
        f.open(file);
        f << buffer;
        f.close();
    } else {
        std::cout << buffer << std::endl;
    }
}

fs::path initLogDirectory(std::string type, std::string group) {
    fs::path base(log_path);
    fs::path type_dir(type);
    fs::path group_dir(group);
    fs::path path = base / type_dir / group_dir;

    if (!fs::is_directory(path)) {
        if (!fs::create_directories(path)) {
            std::cout << "[ERROR] Log directory initialization failed." << std::endl; 
        }
    }

    return path;
}

void logCAM(int id) {
    std::stringstream grp_s;
    grp_s << "StationID_" << id;
    fs::path path = initLogDirectory(CAM_LOG_DIR, grp_s.str());

    char buffer[BUFFER_SIZE];
    int len;
    int ret = c2x::encodeCAM(id, (uint8_t*)buffer, BUFFER_SIZE, &len, c2x::EncodingType::XER_BASIC);
    if (ret < 0) {
        throwError(ret);
    }

    auto now = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
    std::string now_str(ctime(&now));
    now_str.pop_back();
    fs::path file_name(now_str + ".txt");
    fs::path file_path = path / file_name;
    fs::ofstream file(file_path);
    file << buffer;
    file.close();
    if (!fs::exists(file_path)) {
        std::cout << "[ERROR] Writing log file failed." << std::endl;
    }
}

void logDENM(int id, int seqNr) {
    std::stringstream grp_s;
    grp_s << "StationID_" << id << " " << "SeqNr_" << seqNr;
    fs::path path = initLogDirectory(DENM_LOG_DIR, grp_s.str());

    char buffer[BUFFER_SIZE];
    int len;
    int ret = c2x::encodeDENM(id, seqNr, (uint8_t*)buffer, BUFFER_SIZE, &len, c2x::EncodingType::XER_BASIC);
    if (ret < 0) {
        throwError(ret);
    }

    auto now = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
    std::string now_str(ctime(&now));
    now_str.pop_back();
    fs::path file_name(now_str + ".txt");
    fs::path file_path = path / file_name;
    fs::ofstream file(file_path);
    file << buffer;
    file.close();
    if (!fs::exists(file_path)) {
        std::cout << "[ERROR] Writing log file failed." << std::endl;
    }
}

void initCAMContainer(std::string desc, int id) {
    int err = 0;
    if (desc == "bvhf") {
        err = c2x::setCAMBasicVehicleContainerHighFrequency(id, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
        err = c2x::setCAMBasicVehicleContainerHighFrequencyAccelerationControl(id, new uint8_t[0], 0);
        err = c2x::setCAMBasicVehicleContainerHighFrequencyCenDsrcTollingZone(id, 0, 0, 0);
        err = c2x::setCAMBasicVehicleContainerHighFrequencyLanePosition(id, 0);
        err = c2x::setCAMBasicVehicleContainerHighFrequencyLateralAcceleration(id, 0, 0);
        err = c2x::setCAMBasicVehicleContainerHighFrequencyPerformanceClass(id, 0);
        err = c2x::setCAMBasicVehicleContainerHighFrequencySteeringWheelAngle(id, 0, 0);
        err = c2x::setCAMBasicVehicleContainerHighFrequencyVerticalAcceleration(id, 0, 0);
    } else if (desc == "rsuhf") {
        err = c2x::addCAMRSUContainerHighFrequencyProtectedCommunicationZone(id, 0, 0, 0, 0, 0, 0);
    } else if (desc == "bvlf") {
        err = c2x::setCAMBasicVehicleContainerLowFrequency(id, 0, new uint8_t[0], 0);
    } else if (desc == "ptc") {
        err = c2x::setCAMPublicTransportContainer(id, 0, 0, new uint8_t[0], 0);
    } else if (desc == "stc") {
        err = c2x::setCAMSpecialTransportContainer(id, new uint8_t[0], 0, new uint8_t[0], 0);
    } else if (desc == "dgc") {
        err = c2x::setCAMDangerousGoodsContainer(id, 0);
    } else if (desc == "rwcb") {
        err = c2x::setCAMRoadWorksContainerBasic(id, 0, new uint8_t[0], 0, 0, 0, new uint8_t[0], 0);
    } else if (desc == "resc") {
        err = c2x::setCAMRescueContainer(id, new uint8_t[0], 0);
    } else if (desc == "emc") {
        err = c2x::setCAMEmergencyContainer(id, new uint8_t[0], 0, 0, 0, new uint8_t[0], 0);
    } else if (desc == "scc") {
        err = c2x::setCAMSafetyCarContainer(id, new uint8_t[0], 0, 0, 0, 0, 0);
    } else {
        std::cout << "[ERROR] '" << desc << "' is not a valid CAM container." << std::endl;
    }
    if (err) {
        throwError(err);
    }
}

void initDENMContainer(std::string desc, int id, int seqNr) {
    int err = 0;
    if (desc == "sitc") {
        err = c2x::setDENMSituationContainer(id, seqNr, 0, 0, 0);
        err = c2x::setDENMSituationContainerLinkedCause(id, seqNr, 0, 0);
    } else if (desc == "locc") {
        err = c2x::setDENMLocationContainerHeading(id, seqNr, 0, 0);
        err = c2x::setDENMLocationContainerRoadType(id, seqNr, 0);
        err = c2x::setDENMLocationContainerSpeed(id, seqNr, 0, 0);
    } else if (desc == "alcc") {
        err = c2x::setDENMAlacarteContainer(id, seqNr, 0, 0, 0);
        err = c2x::setDENMImpactReductionContainer(id, seqNr, 0, 0, 0, 0, new int[0], 0, 0, 0, 0, 0, new uint8_t[0], 0, 0, 0);
        err = c2x::setDENMRoadWorksContainerExtendedClosedLanes(id, seqNr, 0, 0, 0);
        err = c2x::setDENMRoadWorksContainerExtendedIncidentIndication(id, seqNr, 0, 0);
        err = c2x::setDENMRoadWorksContainerExtendedLightBarSiren(id, seqNr, new uint8_t[0], 0);
        err = c2x::setDENMRoadWorksContainerExtendedRecommendedPath(id, seqNr, new int[0], 0);
        err = c2x::setDENMRoadWorksContainerExtendedReferenceDenms(id, seqNr, new int[0], 0);
        err = c2x::setDENMRoadWorksContainerExtendedRestriction(id, seqNr, new int[0], 0);
        err = c2x::setDENMRoadWorksContainerExtendedSpeedLimit(id, seqNr, 0);
        err = c2x::setDENMRoadWorksContainerExtendedStartingPointSpeedLimit(id, seqNr, 0, 0, 0);
        err = c2x::setDENMRoadWorksContainerExtendedTrafficFlowRule(id, seqNr, 0);
    } else {
        std::cout << "[ERROR] '" << desc << "' is not a valid DENM container." << std::endl;
    }
    if (err) {
        throwError(err);
    }
}

void import(std::string file, bool is_cam, int *id, int *seqNr) {
    std::cout << "[INFO] Read message from file '" << file << "' ..." << std::endl;
    std::ifstream stream(file);
    std::stringstream buffer;
    buffer << stream.rdbuf();
    if (!buffer.str().size()) {
        throw std::invalid_argument("Empty or non existing file.");
    }
    
    int err;
    std::stringstream info;
    if (is_cam) {
        err = c2x::decodeCAM(id, (uint8_t*)buffer.str().c_str(), buffer.str().size(), c2x::XER_BASIC);
        info << "CAM Message (StationId=" << *id << ")";
    } else {
        err = c2x::decodeDENM(id, seqNr, (uint8_t*)buffer.str().c_str(), buffer.str().size(), c2x::XER_BASIC);
        info << "DENM Message (StationId=" << *id << ", SequenceNr=" << *seqNr << ")";
    }

    if (err < 0) {
        throwError(err);
    } else {
        std::cout << "[INFO] Successfully read " 
            << info.str() << " from file." << std::endl;
    }
}

static void transmitterCallbackCAM(int id)
{
    static int n = 0;
    std::cout << ++n << " | Send CAM (StationId=" << id << ")" << std::endl << "> ";
}

static void transmitterCallbackDENM(int id, int seqNr)
{
    static int n = 0;
    std::cout << ++n << " | Send DENM (StationId=" << id << ", SequenceNr=" << seqNr << ")" << std::endl << "> ";
}

void cliTransmitter(bool is_cam, int port, double f, int id, int seqNr) {
    int err;
    if (is_cam) {
        err = c2x::setCAMTransmissionFrequency(f);
        int id_array[1] = {id};
        err = c2x::setCAMIDsForTransmission(id_array, 1);
        c2x::setCAMSendCallback(transmitterCallbackCAM);
        err = c2x::startCAMTransmitter(port, c2x::XER_BASIC);
        std::cout << "[INFO] Start CAM Transmitter." << std::endl;
    } else {
        err = c2x::setDENMTransmissionFrequency(f);
        err = c2x::setDENMTransmissionSource(id, seqNr);
        c2x::setDENMSendCallback(transmitterCallbackDENM);
        err = c2x::startDENMTransmitter(port, c2x::XER_BASIC);
        std::cout << "[INFO] Start DENM Transmitter." << std::endl;
    }
    if (err) {
        throwError(err);
    }
    std::cout << "[INFO] Transmitter opened on port " << port << "." << std::endl;
    std::cout << "========== Transmitter ==========" << std::endl << "Use 'stop' or 'exit' to stop the transmission." << std::endl;

    std::string input;
    do {
        std::cout << "> ";
        std::getline(std::cin, input);
    } while(input != "exit" && input != "stop");

    std::cout << std::endl << "=================================" << std::endl;
    if (is_cam) {        
        c2x::stopCAMTransmitter();
        std::cout << "[INFO] Stop CAM Transmitter." << std::endl;
    } else {
        c2x::stopDENMTransmitter();
        std::cout << "[INFO] Stop DENM Transmitter." << std::endl;
    }
}

static void receiverCallbackCAM(int id)
{
    static int n = 0;
    std::cout << ++n << " | Receive CAM (StationId=" << id << ")";
    if (use_filter) {
        char buffer[BUFFER_SIZE];
        std::cmatch cm;
        std::stringstream re_builder;
        re_builder << "<" << filter << ">(.*)</" << filter << ">";
        std::regex re(re_builder.str(), std::regex_constants::icase);
        c2x::encodeCAM(id, (uint8_t*)buffer, BUFFER_SIZE, nullptr, c2x::XER_BASIC);
        if (std::regex_search(buffer, cm, re)) {
            std::cout << " | " << filter << "=" << cm[1].str();
        } else {
            std::cout << " | '" << filter << "' is not a valid filter property";
        }
    }

    if (log_enabled) {
        logCAM(id);
    }

    std::cout << std::endl << "> ";
}

static void receiverCallbackDENM(int id, int seqNr)
{
    static int n = 0;
    std::cout << ++n << " | Receive DENM (StationId=" << id << ", SequenceNr=" << seqNr << ")";
    if (use_filter) {
        char buffer[BUFFER_SIZE];
        std::cmatch cm;
        std::stringstream re_builder;
        re_builder << "<" << filter << ">(.*)</" << filter << ">";
        std::regex re(re_builder.str(), std::regex_constants::icase);
        c2x::encodeDENM(id, seqNr, (uint8_t*)buffer, BUFFER_SIZE, nullptr, c2x::XER_BASIC);
        if (std::regex_search(buffer, cm, re)) {
            std::cout << " | " << filter << "=" << cm[1].str();
        } else {
            std::cout << " | '" << filter << "' is not a valid filter property";
        }
    }

    if (log_enabled) {
        logDENM(id, seqNr);
    }

    std::cout << std::endl << "> ";
}

void cliReceiver(bool is_cam, int port)
{
    int err;
    if (is_cam) {
        c2x::setCAMRecvCallback(receiverCallbackCAM);
        err = c2x::startCAMReceiver(port, c2x::XER_BASIC);
        std::cout << "[INFO] Start CAM Receiver." << std::endl;
    } else {
        c2x::setDENMRecvCallback(receiverCallbackDENM);
        err = c2x::startDENMReceiver(port, c2x::XER_BASIC);
        std::cout << "[INFO] Start DENM Receiver." << std::endl;
    }
    if (err) {
        throwError(err);
    }
    std::cout << "[INFO] Receiver opened on port " << port << "." << std::endl;
    std::cout << "========== Receiver ==========" << std::endl << "Use 'stop' or 'exit' to stop the transmission." << std::endl;

    std::string input;
    do {
        std::cout << "> ";
        std::getline(std::cin, input);
    } while(input != "exit" && input != "stop");

    std::cout << "==============================" << std::endl;
    if (is_cam) {        
        c2x::stopCAMReceiver();
        std::cout << "[INFO] Stop CAM Receiver." << std::endl;
    } else {
        c2x::stopDENMReceiver();
        std::cout << "[INFO] Stop DENM Receiver." << std::endl;
    }
}

void run(int argc, char** argv) 
{
    po::options_description desc_usage("Allowed usage");
    desc_usage.add_options()
        ("help,h", "Shows this help message.")
        ("version,v", "Shows the program version.")
        ("transmitter", "Starts a server and transmitts messages. The message could be imported from a file.")
        ("receiver", "Starts a client and listens for messages.")
    ;
    po::options_description desc_options("Configuration");
    desc_options.add_options()
        ("type,t", po::value<std::string>()->default_value("cam"), "Message type to be used. Possible values: cam, denm")
        ("message,m", po::value<std::string>()->value_name("file"), "Import message from file.")
        ("export", po::value<std::string>()->value_name("file")->implicit_value(""), "Create and export xml skeleton for CAM or DENM.")
        ("container,c", po::value<std::vector<std::string>>()->multitoken(), 
            "Specify which containers should be included.\n"
            "Values for CAM: (bvhf or rsuhf is required)\n"
            "  bvhf  - Basic Vehicle Container High Frequency\n"
            "  rsuhf - RSU Container High Frequency\n"
            "  bvlf  - Basic Vehicle Container Low Frequency\n"
            "  ptc   - PublicTransportContainer\n"
            "  stc   - SpecialTransportContainer\n"
            "  dgc   - DangerousGoodsContainer\n"
            "  rwcb  - RoadWorksContainerBasic\n"
            "  resc  - RescueContainer\n"
            "  emc   - EmergencyContainer\n"
            "  scc   - SafetyCarContainer\n"
            "Values for DENM:\n"
            "  sitc  - Situation Container\n"
            "  locc  - Location Container\n"
            "  alcc  - Alacarte Container\n")
        ("port,p", po::value<int>()->default_value(1997), "Set udp port for transmitter and receiver.")
        ("frequency,f", po::value<double>()->default_value(1), "Set transmission frequency.")
        ("id", po::value<int>()->default_value(0), "Set station id for operations. "
            "This value is overwritten when a message gets imported from a file.")
        ("seqNr", po::value<int>()->default_value(0), "Set sequence number for operations. This value is only used for"
            " DENMs and gets overwritten when a message gets imported from a file.")
        ("filter", po::value<std::string>(), "Name of property to print from received messages.")
        ("log", po::value<std::string>()->value_name("path"), "Set path for packet logs.")
    ;
    po::options_description cmdline_options;
    cmdline_options.add(desc_usage).add(desc_options);
    po::variables_map vm;
    po::store(po::parse_command_line(argc, argv, cmdline_options), vm);
    po::notify(vm);

    bool do_export = vm.count("export");
    bool use_export_file = false;
    std::string export_file;
    bool is_cam;
    std::string type = vm["type"].as<std::string>();
    int id = vm["id"].as<int>();
    int seqNr = vm["seqNr"].as<int>();
    int port = vm["port"].as<int>();
    double f = vm["frequency"].as<double>();
    bool import_msg = vm.count("message");
    use_filter = vm.count("filter");
    log_enabled = vm.count("log");

    // set log path
    if (log_enabled) {
        log_path = vm["log"].as<std::string>();
    }

    // type
    if (type == "cam") {
        is_cam = true;
    } else if (type == "denm") { 
        is_cam = false;
    } else {
        std::stringstream ss;
        ss << "'" << type << "' is not a valid message type. Use cam or denm instead.";
        throw std::invalid_argument(ss.str());
    }

    // message
    if (import_msg) {
        import(vm["message"].as<std::string>(), is_cam, &id, &seqNr);
    } else {
        if (is_cam) {
            c2x::createCAM(id);
            std::cout << "[INFO] Create CAM Message (StationId=" << id << ")." << std::endl;
        } else {
            c2x::createDENM(id, seqNr);
            std::cout << "[INFO] Create DENM Message (StationId=" << id << ", SequenceNr=" << seqNr << ")." << std::endl;
        }
    }
    
    // container
    if (is_cam && !import_msg) {
        if (!vm.count("container")) {
            initCAMContainer("bvhf", id);
        }
        else {
            for (const auto& it : vm["container"].as<std::vector<std::string>>()) {
                initCAMContainer(it, id);
            }
        }
    } else if (!is_cam && !import_msg) {
        if (vm.count("container")) {
            for (const auto& it : vm["container"].as<std::vector<std::string>>()) {
                initDENMContainer(it, id, seqNr);
            }
        }
    }

    // export
    if (do_export) {
        export_file = vm["export"].as<std::string>();
        use_export_file = !export_file.empty();
        if (is_cam) {
            exportCAM(use_export_file, export_file, id);
        } else { 
            exportDENM(use_export_file, export_file, id, seqNr);
        }
    }

    // filter
    if (use_filter) {
        filter = vm["filter"].as<std::string>();
    }

    // Exclusive usage
    if (vm.count("help")) {
        std::cout << cmdline_options << std::endl;
    } else if (vm.count("version")) {
        std::cout << "clic2x version 0.1.0" << std::endl;
    } else if (vm.count("transmitter")) {
        cliTransmitter(is_cam, port, f, id, seqNr);
    } else if (vm.count("receiver")) {
        cliReceiver(is_cam, port);
    }

    if (is_cam) {
        c2x::deleteCAM(id);
    } else {
        c2x::deleteDENM(id, seqNr);
    }
}

int main(int argc, char** argv) 
{
    try
    {
        run(argc, argv);
        return 0;
    }
    catch(const std::exception& e)
    {
        std::cout << "[ERROR] " << e.what() << '\n';
        return -1;
    }    
}