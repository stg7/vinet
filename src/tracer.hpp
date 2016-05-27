

#ifndef TRACER_H
#define TRACER_H

#include <cstdlib>
#include <cstdio>
#include <iostream>
#include <memory>
#include <stdexcept>
#include <string>
#include <sstream>

namespace traceview {

    std::string ssystem(const char* cmd) {
        char buffer[128];
        std::string result = "";
        std::shared_ptr<FILE> pipe(popen(cmd, "r"), pclose);
        if (!pipe)
            throw std::runtime_error("popen() failed!");
        while (!feof(pipe.get())) {
            if (fgets(buffer, 128, pipe.get()) != NULL)
                result += buffer;
        }
        return result;
    }

    class Tracer {
     public:
        std::string trace(const std::string& url) {
            std::stringstream command;
            command << "traceroute " << url;
            auto res = ssystem(command.str().c_str());
            // todo(stg7) do some string parsing of output
            return res;
        }
    };

}
#endif