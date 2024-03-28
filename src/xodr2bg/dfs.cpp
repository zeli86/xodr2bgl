/*
 * Copyright 2024 Želimir Marojević
 *
 * Distributed under the Boost Software License, Version 1.0.
 * (See accompanying file LICENSE_1_0.txt
 * or copy at http://www.boost.org/LICENSE_1_0.txt)
 *
 * For more information, see http://www.boost.org
 */

#include "dfs.hpp"
#include <iostream>

void dfs_visitor::initialize_vertex(RoadNetVertex v, const RoadNet& g) const
{
    // std::cout << "init         " << g[v]._vertex_id << "\n";
}

void dfs_visitor::discover_vertex(RoadNetVertex v, const RoadNet& g)
{
    // std::cout << "discover     " << g[v]._vertex_id << "\n";
    _stack.push(v);
}

void dfs_visitor::start_vertex(RoadNetVertex v, const RoadNet& g) const
{
    // std::cout << "start_vertex     " << g[v]._vertex_id << "\n";
}

void dfs_visitor::finish_vertex(RoadNetVertex v, const RoadNet& g)
{
    // std::cout << "finish_vertex     " << g[v]._vertex_id << "\n";
    _stack.pop();
    if (_stack.size() == 1)
    {
        std::cout << "++++++++++++++++++++++++++++++++\n";
        for (auto e : _list)
        {
            const auto& data = g[e];
            std::cout << " " << data._road_id << ", " << data._lane_id << ", " << data._lane_section_id << "\n";
        }
        _list.clear();
    }
}

void dfs_visitor::examine_edge(RoadNetEdge e, const RoadNet& g) const
{
    // const auto& data = g[e];
    // std::cout << "  examine_edge " << data._road_id << ", " << data._lane_id << ", " << data._lane_section_id << "\n";
}

void dfs_visitor::tree_edge(RoadNetEdge e, const RoadNet& g)
{
    _list.push_back(e);
    // const auto& data = g[e];
    // std::cout << "  tree_edge    " << data._road_id << ", " << data._lane_id << ", " << data._lane_section_id << "\n";
}

void dfs_visitor::back_edge(RoadNetEdge e, const RoadNet& g) const
{
    // const auto& data = g[e];
    // std::cout << "  back_edge    " << data._road_id << ", " << data._lane_id << ", " << data._lane_section_id << "\n";
}

void dfs_visitor::on_tree(RoadNetEdge e, const RoadNet& g) const
{
    // const auto& data = g[e];
    // std::cout << "  on_tree      " << data._road_id << ", " << data._lane_id << ", " << data._lane_section_id << "\n";
}

void dfs_visitor::on_back(RoadNetEdge e, const RoadNet& g) const
{
    // const auto& data = g[e];
    // std::cout << "  on_back      " << data._road_id << ", " << data._lane_id << ", " << data._lane_section_id << "\n";
}

void dfs_visitor::forward_or_cross_edge(RoadNetEdge e, const RoadNet& g) const
{
    // const auto& data = g[e];
    // std::cout << "  forward_or_cross_edge " << data._road_id << ", " << data._lane_id << ", " << data._lane_section_id << "\n";
}