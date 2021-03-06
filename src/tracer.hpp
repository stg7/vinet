/**
    part of ViNet

    \author stg7

    \brief trace tool definition

    \date 27.05.2016

    Copyright 2016 Steve Göring

    This file is part of ViNet.

    ViNet is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    ViNet is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with ViNet.  If not, see <http://www.gnu.org/licenses/>.
**/

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

namespace vinet {

    /*
        perform a trace
        TODO(stg7): replace call to traceroute with own created tracing call
    */
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

    /*
        handle a list of hosts and trace each in several threads
    */
    class MultiThreadTracer {
     private:
        std::vector<std::string> _hosts;
        unsigned int _num_threads;

     public:
        MultiThreadTracer(std::vector<std::string> hosts, unsigned int num_threads): _hosts(hosts), _num_threads(num_threads) {
            LOG("create MultiThreadTracer: " << _num_threads << " threads and prepare trace for "
                << _hosts.size() << " hosts.");
        }

        /*
            start a trace and store results line separated in `outfilename`

            file format is:
                each line is one trace and result is json encoded, e.g.
                    "url" : ["host1", "host2", ...]
        */
        void start_trace(const std::string& outfilename) {
            std::ofstream out;

            out.open(outfilename.c_str());

            unsigned int start = 0;
            unsigned int hosts_size = static_cast<unsigned int>(_hosts.size());
            unsigned int parts = std::min(_num_threads, hosts_size);
            unsigned int step = std::ceil((_hosts.size() + 0.0) / parts);

            std::vector<std::thread> threads;
            for (unsigned int i = 0; i < parts; i++) {
                auto do_trace = [](std::ofstream* out, const std::vector<std::string>& hosts,
                    unsigned int start, unsigned int end) {
                        // start tracing for hosts subset from start to end - 1
                        Tracer t;
                        for (unsigned int i = start; i < end; i++) {
                            *out << t.trace(hosts[i]) << "\n";
                            (*out).flush();
                            LOG("done: " << hosts[i]);
                        }
                    };

                threads.emplace_back(std::thread(do_trace, &out, _hosts, start,
                    std::min(start + step, hosts_size)));
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