

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

    class Tracer {
     private:
        std::string _res;
     public:
        std::string trace(const std::string& url) {
            std::stringstream command;
            command << "traceroute -q 1 " << url;
            auto res = ssystem(command.str().c_str());
            std::cout << res << std::endl;
            // todo(stg7) do some string parsing of output
            std::vector<std::string> path;
            unsigned int i = 0;
            for (auto& l: split(res, '\n')) {
                if (i > 0) {
                    auto parts = split(l, ' ');
                    if (parts.size() >= 3) {
                        path.emplace_back("\"" + parts[3] + "\"");
                    }
                }
                i++;
            }
            _res = "[" + join(path, ", ") + "]";
            return _res;
        }

        std::string get_result() {
            return _res;
        }
    };
    class MultiThreadTracer{
     private:
        std::vector<std::string> _hosts;
        unsigned int _num_threads;

     public:
        MultiThreadTracer(std::vector<std::string> hosts, unsigned int num_threads): _hosts(hosts), _num_threads(num_threads) {
            LOG("create MultiThreadTracer: " << _num_threads << " threads and prepare trace for " << _hosts.size() << " hosts.");

        }
        void start_trace() {
            std::ofstream out;

            out.open("out.trace");

            unsigned int start = 0;
            unsigned int hosts_size = static_cast<unsigned int>(_hosts.size());
            unsigned int parts = std::min(_num_threads, hosts_size);
            unsigned int size = _hosts.size() / parts;

            // todo(stg7) maybe use some smartpointer instead of *
            std::vector<std::future<std::vector<std::shared_ptr<Tracer>>>> results;

            for(unsigned int i = 0; i < parts; i++) {
                results.emplace_back(std::async(
                    [](std::ofstream* out,const std::vector<std::string>& hosts, unsigned int start, unsigned int end) -> std::vector<std::shared_ptr<Tracer>> {
                        // start tracing for hosts subset from start to end - 1
                        std::vector<std::shared_ptr<Tracer>> tracers;
                        for(unsigned int i = start; i < end; i++) {
                            std::shared_ptr<Tracer> t = std::make_shared<Tracer>();
                            tracers.emplace_back(t);
                            *out << t->trace(hosts[i]) << "\n";
                            (*out).flush();
                        }
                        return tracers;
                    }, &out, _hosts, start, std::min(start + size, hosts_size)));

                start += size;
            }
            for(auto& r : results) {
                for(auto i : r.get()) {
                    std::cout << i->get_result() << std::endl;
                }
            }
            out.close();
            LOG("done");
        }
    };

}
#endif