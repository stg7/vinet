#include <iostream>

#include "tracer.hpp"
/**
    TraceView: command line tracetool
**/
int main(int argc, const char* argv[]) {
    traceview::Tracer t;

    // todo(stg7) read list of hosts from file, and start multiple threads for getting all traces
    std::cout << "hello "
        << t.trace("google.de")
        << std::endl;

    return 0;
}