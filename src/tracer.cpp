#include <iostream>
#include <thread>
#include <string>
#include <fstream>
#include <vector>

#include <boost/program_options.hpp>
#include <boost/filesystem.hpp>

#include "log.hpp"
#include "build.hpp"
#include "tracer.hpp"

/**
    TraceView: command line tracetool
**/
int main(int argc, const char* argv[]) {
    namespace po = boost::program_options;
    namespace fs = boost::filesystem;

    unsigned int num_threads = 0;
    std::string host_list = "";

    // declare the supported options.
    po::options_description desc("tracer - ...\n\nSteve Göring 2016\nParameter");
    desc.add_options()
        ("help,h", "produce help message")
        ("hostlist,l", po::value<std::string>(&host_list)->required(), "list of all hosts that should be traced")
        ("threads,t", po::value<unsigned int>(&num_threads)->default_value(std::thread::hardware_concurrency()), "number of trace-threads");

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

    mt.start_trace();


    return 0;
}
