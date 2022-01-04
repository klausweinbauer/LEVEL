#pragma once

#include <string>

namespace c2x {

    class Logger {
        private:
            std::string path_;

        public:
            Logger(std::string path);

            virtual int writeLog(std::string group_name, char* buffer, int size);
    };
};