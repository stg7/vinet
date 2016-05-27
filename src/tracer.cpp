#include <iostream>
#include <thread>
#include <string>

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

    unsigned int threads = 0;
    // declare the supported options.
    po::options_description desc("tracer - ...\n\nSteve Göring 2016\nParameter");
    desc.add_options()
        ("help,h", "produce help message")
        ("hostlist,l", po::value<std::string>()->required(), "list of all hosts that should be traced")
        ("threads,t", po::value<unsigned int>(&threads)->default_value(std::thread::hardware_concurrency()), "number of trace-threads");

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

    std::string host_list = vm["hostlist"].as<std::string>();

    LOG("run with: " << threads << " and handle host list: " << host_list);


    traceview::Tracer t;

    // todo(stg7) read list of hosts from file, and start multiple threads for getting all traces
    std::cout << "hello "
        << t.trace("google.de")
        << std::endl;

    return 0;
}
