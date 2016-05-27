

#ifndef TRACER_H
#define TRACER_H

#include <cstdlib>
#include <cstdio>
#include <iostream>
#include <fstream>
#include <memory>
#include <stdexcept>
#include <string>
#include <vector>
#include <sstream>
#include <thread>
#include <future>

#include "log.hpp"

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
    // from phrasit/helper.hpp:
    inline std::vector<std::string>& split(const std::string& s, char delim, std::vector<std::string>& elems) {
        std::stringstream ss(s);
        std::string item;
        while (std::getline(ss, item, delim)) {
            elems.emplace_back(item);
        }
        return elems;
    }

    /*
    *   split a string by delimiter and return result in a vector of strings
    *   \param s string to split
    *   \param delim using this character as a delimiter
    *   \return splitted strings as a vector
    */
    inline std::vector<std::string> split(const std::string& s, char delim) {
        std::vector<std::string> elems;
        split(s, delim, elems);
        return elems;
    }
    inline std::string join(const std::vector<std::string> values, const std::string& connector) {
        std::string res = "";

        for (size_t i = 0; i < values.size(); i++) {
            res += values[i];
            if (i != values.size() - 1) {
                res += connector;
            }
        }
        return res;
    }
    /*
    *   trim spaces from a string
    *
    *   \param s string for trimming
    *   \return string s without leading/trailing spaces
    */
    inline std::string trim(const std::string& s) {
        std::string::const_iterator it = s.begin();
        while (it != s.end() && *it == ' ') {
            it++;
        }

        std::string::const_reverse_iterator rit = s.rbegin();
        while (rit.base() != it && *rit == ' ') {
            rit++;
        }

        return std::string(it, rit.base());
    }

    class Tracer {
     private:
        std::string _res;
     public:
        std::string trace(const std::string& url) {
            std::stringstream command;
            command << "traceroute -q 1 " << url;
            auto res = ssystem(command.str().c_str());
            // todo(stg7) do some string parsing of output
            std::vector<std::string> path;
            unsigned int i = 0;
            for (auto& l: split(res, '\n')) {
                if (i > 0) {
                    auto parts = split(trim(l), ' ');
                    if (parts.size() >= 2) {
                        path.emplace_back("\"" + parts[2] + "\"");
                    }
                }
                i++;
            }
            _res = "\"" + url + "\" : [" + join(path, ", ") + "]";
            return _res;
        }

        std::string get_result() {
            return _res;
        }
    };
    class MultiThreadTracer {
     private:
        std::vector<std::string> _hosts;
        unsigned int _num_threads;

     public:
        MultiThreadTracer(std::vector<std::string> hosts, unsigned int num_threads): _hosts(hosts), _num_threads(num_threads) {
            LOG("create MultiThreadTracer: " << _num_threads << " threads and prepare trace for " << _hosts.size() << " hosts.");

        }
        void start_trace(const std::string& outfilename) {
            std::ofstream out;

            out.open(outfilename.c_str());

            unsigned int start = 0;
            unsigned int hosts_size = static_cast<unsigned int>(_hosts.size());
            unsigned int parts = std::min(_num_threads, hosts_size);
            unsigned int step = std::ceil((_hosts.size() + 0.0) / parts);

            std::vector<std::thread> threads;
            for (unsigned int i = 0; i < parts; i++) {
                auto do_trace = [](std::ofstream* out, const std::vector<std::string>& hosts, unsigned int start, unsigned int end) {
                        // start tracing for hosts subset from start to end - 1
                        Tracer t;
                        for (unsigned int i = start; i < end; i++) {
                            *out << t.trace(hosts[i]) << "\n";
                            (*out).flush();
                            LOG("done: " << hosts[i]);
                        }
                    };

                threads.emplace_back(std::thread(do_trace, &out, _hosts, start, std::min(start + step, hosts_size)));

                start += step;
            }

            for (auto& t: threads) {
                t.join();
            }
            out.close();
            LOG("done");
        }
    };

}
#endif