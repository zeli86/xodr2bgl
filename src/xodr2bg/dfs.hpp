/*
 * Copyright 2024 Želimir Marojević
 *
 * Distributed under the Boost Software License, Version 1.0.
 * (See accompanying file LICENSE_1_0.txt
 * or copy at http://www.boost.org/LICENSE_1_0.txt)
 *
 * For more information, see http://www.boost.org
 */

#include "types.hpp"
#include <boost/graph/depth_first_search.hpp>
#include <stack>
#include <list>

class dfs_visitor : boost::default_dfs_visitor
{
  public:

    explicit dfs_visitor()
    {}

    void initialize_vertex(RoadNetVertex v, const RoadNet& g) const;
    void start_vertex(RoadNetVertex v, const RoadNet& g) const;
    void finish_vertex(RoadNetVertex v, const RoadNet& g);
    void discover_vertex(RoadNetVertex v, const RoadNet& g);

    void on_tree(RoadNetEdge e, const RoadNet& g) const;
    void on_back(RoadNetEdge e, const RoadNet& g) const;
    void forward_or_cross_edge(RoadNetEdge e, const RoadNet& g) const;
    void examine_edge(RoadNetEdge e, const RoadNet& g) const;
    void tree_edge(RoadNetEdge e, const RoadNet& g);
    void back_edge(RoadNetEdge e, const RoadNet& g) const;
protected:
    std::stack<RoadNetVertex> _stack;
    std::list<RoadNetEdge> _list;
};