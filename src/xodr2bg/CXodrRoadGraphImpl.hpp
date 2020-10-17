/*
 * Copyright 2024 Želimir Marojević
 *
 * Distributed under the Boost Software License, Version 1.0.
 * (See accompanying file LICENSE_1_0.txt
 * or copy at http://www.boost.org/LICENSE_1_0.txt)
 *
 * For more information, see http://www.boost.org
 */

#pragma once

#include <boost/config.hpp>

#include <boost/multi_index/composite_key.hpp>
#include <boost/multi_index/member.hpp>
#include <boost/multi_index/ordered_index.hpp>
#include <boost/multi_index_container.hpp>
#include <boost/utility.hpp>

#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/directed_graph.hpp>
#include <boost/graph/graph_utility.hpp>
#include <boost/property_map/property_map.hpp>

#include <boost/call_traits.hpp>
#include <boost/optional.hpp>
#include <ostream>

#include "JunctionRow.hpp"
#include "RoadLaneSectionRow.hpp"

struct junctionKey
    : boost::multi_index::composite_key<
          cJunctionRow,
          BOOST_MULTI_INDEX_MEMBER(cJunctionRow, int32_t, _incoming_road),
          BOOST_MULTI_INDEX_MEMBER(cJunctionRow, int32_t, _from_lane)>
{
};

using JuctionTable = boost::multi_index::multi_index_container<
    cJunctionRow,
    boost::multi_index::indexed_by<
        boost::multi_index::ordered_non_unique<junctionKey>,
        boost::multi_index::ordered_non_unique<BOOST_MULTI_INDEX_MEMBER(
            cJunctionRow, int32_t, _incoming_road)>>>;

struct roadLaneSectionKey
    : boost::multi_index::composite_key<
          cRoadLaneSectionRow,
          BOOST_MULTI_INDEX_MEMBER(cRoadLaneSectionRow, int32_t, _road),
          BOOST_MULTI_INDEX_MEMBER(cRoadLaneSectionRow, int32_t, _lane),
          BOOST_MULTI_INDEX_MEMBER(cRoadLaneSectionRow, int32_t, _section)>
{
};

using RoadTable = boost::multi_index::multi_index_container<
    cRoadLaneSectionRow,
    boost::multi_index::indexed_by<
        boost::multi_index::ordered_unique<roadLaneSectionKey>,
        boost::multi_index::ordered_unique<BOOST_MULTI_INDEX_MEMBER(
            cRoadLaneSectionRow, int32_t, _id)>,
        boost::multi_index::ordered_non_unique<BOOST_MULTI_INDEX_MEMBER(
            cRoadLaneSectionRow, int32_t, _junction)>>>;

class CXodrRoadGraphImpl
{
  public:
    CXodrRoadGraphImpl();

    bool init(const std::string&);

    void dump(std::ostream&);

    void to_graphml(const std::string&);
    void to_graphviz(const std::string&);

    void to_txt(std::ostream&);

    boost::optional<RoadNetEdgeItr> find(const int32_t, const int32_t, const int32_t);

    RoadNet m_RoadNet;

  private:
    bool populateDataTables(const std::string&);
    void populateGraph();

    void add_new_edges(const cRoadLaneSectionRow&, int32_t&, int32_t&);
    void add_new_edge(const boost::optional<RoadNetEdgeItr>&, const boost::optional<RoadNetEdgeItr>&);

    void clear();
    void dumpRoadTable(std::ostream&);
    void dumpJunctionTable(std::ostream&);

    RoadTable m_miRoadTable;
    JuctionTable m_miJunctionTable;

    static const std::string m_arrLeftRight[2];

    std::map<std::string, eSide> m_mapSide;
    std::map<std::string, eLinkType> m_mapLinkType;
    std::map<std::string, eContactPoint> m_mapContactPoint;
};
