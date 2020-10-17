/*
 * Copyright 2024 Želimir Marojević
 *
 * Distributed under the Boost Software License, Version 1.0.
 * (See accompanying file LICENSE_1_0.txt
 * or copy at http://www.boost.org/LICENSE_1_0.txt)
 *
 * For more information, see http://www.boost.org
 */

#include <boost/graph/adjacency_list.hpp>
#include <ostream>

#pragma once

class cRoadLaneSectionRow;

class cRoadNetVertexData
{
  public:
    cRoadNetVertexData() = default;
    cRoadNetVertexData(const int32_t);
    ~cRoadNetVertexData() = default;

    int32_t _vertex_id = -1;
};

std::ostream& operator<<(std::ostream&, const cRoadNetVertexData&);

class cRoadNetEdgeData
{
  public:
    cRoadNetEdgeData(const int32_t);
    cRoadNetEdgeData(const int32_t, const cRoadLaneSectionRow&);

    int32_t _id = -1;
    int32_t _row_id = -1;
    int32_t _road_id = -1;
    int32_t _lane_id = 0;
    int32_t _lane_section_id = 0;
    int32_t _no_sections = 0;

    std::string _name = "";
    double _weight = 0;
    double _length = 0;
};

std::ostream& operator<<(std::ostream&, const cRoadNetEdgeData&);

using RoadNet = boost::adjacency_list<boost::listS, boost::vecS, boost::directedS, cRoadNetVertexData, cRoadNetEdgeData>;
using RoadNetEdge = boost::graph_traits<RoadNet>::edge_descriptor;
using RoadNetEdgeItr = boost::graph_traits<RoadNet>::edge_iterator;
using RoadNetVertex = boost::graph_traits<RoadNet>::vertex_descriptor;
using RoadNetVertexItr = boost::graph_traits<RoadNet>::vertex_iterator;
