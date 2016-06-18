/**
    part of ViNet

    \author stg7

    \brief a undirected graph base class

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

#ifndef MATH_GRAPH_H
#define MATH_GRAPH_H

#include <iostream>
#include <string>
#include <map>

#include "utils/log.hpp"

namespace vinet {
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
            auto get_neighbor_nodes(long n) {
                std::vector<long> res;
                for (auto& e: _edges) {
                    if (e.first == n) {
                        res.emplace_back(e.second);
                    }
                }
                return res;
            }
            auto get_neighbor_nodes_rev(long n) {
                std::vector<long> res;
                for (auto& e: _edges) {
                    if (e.second == n) {
                        res.emplace_back(e.first);
                    }
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