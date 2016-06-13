#ifndef GFX_VGRAPH_H
#define GFX_VGRAPH_H

#include <iostream>
#include <tuple>
#include <map>
#include <vector>
#include <set>
#include <cmath>

// opengl & sdl
#include <GL/gl.h>
#include <GL/glu.h>
#include <SDL.h>

#include "gfx/circle.hpp"
#include "gfx/line.hpp"
#include "utils/log.hpp"
#include "math/graph.hpp"

namespace traceview {
    namespace gfx {
        class VGraph {
         private:
            int _max_x;
            int _max_y;
            Circle _circle;
            Line _line;
            math::Graph _g;
            std::map<long, std::tuple<int, int>> _node_pos;

            std::mt19937 _generator;
            std::uniform_int_distribution<> _dist_x;
            std::uniform_int_distribution<> _dist_y;
            void update_positions() {
                auto norm_v = [](auto& u, auto& v) -> double {
                    auto tmp_x = std::get<0>(v) - std::get<0>(u);
                    auto tmp_y = std::get<1>(v) - std::get<1>(u);
                    return std::sqrt(tmp_x * tmp_x + tmp_y * tmp_y);
                };

                std::map<long, std::tuple<double, double>> forces;
                auto all_nodes = _g.get_nodes_ids();
                const double l = 120.0; // 1.0
                const double c_spring = 2.5;//  20; // 0.2
                const double c_rep = 5.0; //-80; // 1.0
                const double delta = 0.01;//4; // 0.1

                for (auto& v: _g.get_nodes_ids()) {
                    double f_x = 0;
                    double f_y = 0;
                    std::set<long> remaining_nodes(all_nodes.begin(), all_nodes.end());
                    remaining_nodes.erase(v);
                    // calculate spring force between connected nodes
                    auto v_pos = _node_pos[v];
                    for (auto& u: _g.get_neighbor_nodes_rev(v)) {
                        auto u_pos = _node_pos[u];
                        auto norm = norm_v(u_pos, v_pos) + 0.01;
                        //double f = c_spring * std::log(norm) / l;
                        double f = c_spring * (norm - l);

                        f_x = (std::get<0>(u_pos) - std::get<0>(v_pos)) * f / norm;
                        f_y = (std::get<1>(u_pos) - std::get<1>(v_pos)) * f / norm;

                        remaining_nodes.erase(u);
                    }

                    // calcuale force between remaining nodes
                    for (auto& u: remaining_nodes) {
                        auto u_pos = _node_pos[u];
                        auto norm = norm_v(u_pos, v_pos) + 0.01;
                        double f = c_rep / (norm * norm);
                        f_x += (std::get<0>(v_pos) - std::get<0>(u_pos)) * f / norm;
                        f_y += (std::get<1>(v_pos) - std::get<1>(u_pos)) * f / norm;


                    }
                    forces.insert(
                        std::pair<long, std::tuple<int, int>>(v, std::make_tuple (f_x, f_y))
                        );
                }
                for (auto kv: forces) {
                    auto v = kv.first;
                    auto f = kv.second;
                    auto old = _node_pos[v];
                    double x = std::round(std::get<0>(old) + delta * std::get<0>(f));
                    double y = std::round(std::get<1>(old) + delta * std::get<1>(f));
                    _node_pos[v] = std::make_tuple((int) x, (int) y);

                }
            }
         public:
            VGraph() {
                std::random_device rd;
                _generator = std::mt19937(rd());
            }
            void init(int max_x, int max_y, const math::Graph& g) {
                _max_x = max_x;
                _max_y = max_y;
                _dist_x = std::uniform_int_distribution<>(0, _max_x);
                _dist_y = std::uniform_int_distribution<>(0, _max_y);

                _g = g;
                for (auto& n: _g.get_nodes_ids()) {
                    int x = _dist_x(_generator);
                    int y = _dist_y(_generator);

                    auto it = _node_pos.find(n);
                    if (it == _node_pos.end()) {
                        _node_pos.insert(
                            std::pair<long, std::tuple<int, int>>(n, std::make_tuple (x, y))
                            );
                        continue;
                    }
                    _node_pos[n] = std::make_tuple (x, y);
                }
            }
            void dooIT() {
                for (int i = 0; i < 200; i++) {
                    update_positions();
                }
            }
            void draw() {
                glColor4f(1, 0, 0, 1);

                update_positions();
                for (auto& edge: _g.get_edges()) {
                    auto s = _node_pos[edge.first];
                    auto e = _node_pos[edge.second];

                    _line.draw(std::get<0>(s), std::get<1>(s), std::get<0>(e), std::get<1>(e));
                }
                glColor4f(0, 0, 1, 1);
                for (auto& n: _node_pos) {
                    auto pos = n.second;
                    _circle.draw(std::get<0>(pos), std::get<1>(pos), 15);
                }
            }
        };
    }
}
#endif