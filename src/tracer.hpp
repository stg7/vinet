

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

#include "utils/log.hpp"
#include "utils/helper.hpp"

namespace traceview {


    class Tracer {
     private:
        std::string _res;
     public:
        std::string trace(const std::string& url) {
            std::stringstream command;
            command << "traceroute -q 1 " << url;
            auto res = helper::ssystem(command.str().c_str());
            std::vector<std::string> path;
            unsigned int i = 0;
            for (auto& l: helper::split(res, '\n')) {
                if (i > 0) {
                    auto parts = helper::split(helper::trim(l), ' ');
                    if (parts.size() >= 2) {
                        path.emplace_back("\"" + parts[2] + "\"");
                    }
                }
                i++;
            }
            _res = "\"" + url + "\" : [" + helper::join(path, ", ") + "]";
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
            // wait for all threads
            for (auto& t: threads) {
                t.join();
            }
            out.close();
            LOG("done");
        }
    };

}
#endif