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
            std::map<std::string, long> _nodes;
            std::multimap<long, long> _edges;


            long add_node(const std::string& n) {
                auto it = _nodes.find(n);
                if (it == _nodes.end()) {
                    _nodes.insert(std::pair<std::string, long>(n, _nodes.size()));
                }
                return _nodes[n];
            }
         public:
            Graph() {
                LOG("build graph");
            }

            void add_edge(std::string n1, std::string n2) {
                LOG("edge from: " << n1 << " to " << n2);
                long i1 = add_node(n1);
                long i2 = add_node(n2);
                _edges.insert(std::pair<long, long>(i1, i2));
                LOG("edge from: " << i1 << " to " << i2);
            }

            std::vector<long> get_nodes_ids() {
                std::vector<long> res;
                for (auto e: _nodes) {
                    res.emplace_back(e.second);
                }
                return res;
            }

            auto get_edges() {
                return _edges;
            }

        };
    }
}
#endif