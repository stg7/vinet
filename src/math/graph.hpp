#ifndef MATH_GRAPH_H
#define MATH_GRAPH_H

#include <iostream>
#include <string>
#include <map>

#include "utils/log.hpp"

namespace traceview {
    namespace math {
        class Graph {
         private:
            std::map<std::string, int> _nodes;

            // todo(stg7) use a multimap for edge storing

            int addNode(const std::string& n) {
                auto it = _nodes.find(n);
                if (it == _nodes.end()) {
                    _nodes.insert(std::pair<std::string, int>(n, _nodes.size()));
                }
                return _nodes[n];
            }
         public:
            Graph() {
                LOG("build graph");
            }

            void addEdge(std::string n1, std::string n2) {
                LOG("edge from: " << n1 << " to " << n2);
                int i1 = addNode(n1);
                int i2 = addNode(n2);
                LOG("edge from: " << i1 << " to " << i2);
            }

        };
    }
}
#endif