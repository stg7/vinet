/**
    part of TraceView

    \author stg7

    \brief visualizer

    \date 27.05.2016

    Copyright 2016 Steve Göring

    This file is part of TraceView.

    TraceView is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    TraceView is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with TraceView.  If not, see <http://www.gnu.org/licenses/>.
**/
#include <iostream>
#include <thread>
#include <string>
#include <fstream>
#include <vector>
#include <experimental/filesystem>

#include <boost/program_options.hpp>

#include "utils/log.hpp"
#include "build.hpp"
#include "tracer.hpp"

/**
    TraceView: command line tracetool
**/
int main(int argc, const char* argv[]) {
    namespace po = boost::program_options;
    namespace fs = std::experimental::filesystem;

    unsigned int num_threads = 0;
    std::string host_list = "";
    std::string outfilename = "";

    // declare the supported options.
    po::options_description desc("tracer - multithreaded tracetool\n\nSteve Göring 2016\nParameter");
    desc.add_options()
        ("help,h", "produce help message")
        ("hostlist,l", po::value<std::string>(&host_list)->required(), "list of all hosts that should be traced")
        ("threads,t", po::value<unsigned int>(&num_threads)->default_value(std::thread::hardware_concurrency()), "number of trace-threads")
        ("outfile,o", po::value<std::string>(&outfilename)->default_value("out.trace"), "output file for trace results")
    ;

    po::variables_map vm;
    try {
        po::store(po::parse_command_line(argc, argv, desc), vm);
        po::notify(vm);
    } catch (po::error& e) {
        ERR("error: " << e.what());
        std::cout << desc << std::endl;
        return -1;
    }
    if (vm.count("help") != 0) {
        std::cout << desc << std::endl;
        return -1;
    }
    if (!fs::exists(host_list)) {
        ERR(host_list << " does not exist.");
        std::cout << desc << std::endl;
        return -1;
    }
    OUT("[version] " << version);
    OUT("[branch]  " << branch);

    LOG("run with: " << num_threads << " and handle host list: " << host_list);

    std::vector<std::string> hosts;
    {
        std::ifstream f(host_list);
        std::string line;
        while (std::getline(f, line)) {
            if (line != "") {
                hosts.emplace_back(line);
            }
        }

    }
    traceview::MultiThreadTracer mt(hosts, num_threads);

    mt.start_trace(outfilename);

    return 0;
}
