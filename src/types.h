/*
 * Copyright 2019 Želimir Marojević
 *
 * Distributed under the Boost Software License, Version 1.0.
 * (See accompanying file LICENSE_1_0.txt
 * or copy at http://www.boost.org/LICENSE_1_0.txt)
 *
 * For more information, see http://www.boost.org
 */

#include "stdafx.h"
#include <boost/graph/adjacency_list.hpp>

#pragma once

class cRoadNetVertexData
{
public:
  cRoadNetVertexData() = default;
  cRoadNetVertexData(int32_t iVertexId) : m_iVertexId(iVertexId) {};
  ~cRoadNetVertexData() = default;

  int32_t m_iVertexId = -1;
};

class cRoadNetEdgeData
{
public:
  cRoadNetEdgeData() = default;
  cRoadNetEdgeData(int32_t iRowId) : m_iRowId(iRowId) {};
  ~cRoadNetEdgeData() = default;

  int32_t m_iRowId = -1;
};

using RoadNet = boost::adjacency_list<boost::setS, boost::setS, boost::directedS, cRoadNetVertexData, cRoadNetEdgeData, boost::no_property>;

using RoadNetEdge = boost::graph_traits<RoadNet>::edge_descriptor;

using RoadNetVertex = boost::graph_traits<RoadNet>::vertex_descriptor;
