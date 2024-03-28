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
#include "enums.hpp"

struct junctionKey
   : boost::multi_index::composite_key<
     cJunctionRow,
  BOOST_MULTI_INDEX_MEMBER(cJunctionRow, int32_t, _connecting_road),
  BOOST_MULTI_INDEX_MEMBER(cJunctionRow, int32_t, _from_lane)>
  {
  };

using JuctionTable = boost::multi_index::multi_index_container<
                     cJunctionRow,
                     boost::multi_index::indexed_by<
                     boost::multi_index::ordered_non_unique<BOOST_MULTI_INDEX_MEMBER(cJunctionRow, eJunctionType, _junction_type)>,
                     boost::multi_index::ordered_unique<junctionKey>>>;

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
                  boost::multi_index::ordered_unique<BOOST_MULTI_INDEX_MEMBER(cRoadLaneSectionRow, int32_t, _id)>,
                  boost::multi_index::ordered_non_unique<BOOST_MULTI_INDEX_MEMBER(cRoadLaneSectionRow, int32_t, _junction)>>>;

class CXodrRoadGraphImpl
{
   public:
   CXodrRoadGraphImpl();
   ~CXodrRoadGraphImpl();

   bool init(const std::string&, const bool);

   void set_lane_type_filter(const CLaneTypeFilter&);

   void dump(std::ostream&);

   void to_graphviz(const std::string&);

   void to_txt(std::ostream&);

   void set_log_level(const unsigned=7);

   void dfs(const int32_t, const int32_t, const int32_t, const bool);

   private:
   void clear();
   bool populateDataTables(const std::string&);
   void updateRoadTable();
   void populateGraph();

   void add_new_edges(const cRoadLaneSectionRow&, int32_t&, int32_t&);
   void add_new_edge(const boost::optional<RoadNetEdgeItr>&, const boost::optional<RoadNetEdgeItr>&);

   void dumpRoadTable(std::ostream&);
   void dumpJunctionTable(std::ostream&);

   [[nodiscard]] bool is_enabled(const cRoadLaneSectionRow&);

   boost::optional<RoadNetEdgeItr> find(const int32_t, const int32_t, const int32_t);

   bool _auto_connect_lane_sections = false;

   CLaneTypeFilter _lane_type_filter;
   RoadTable m_miRoadTable;
   JuctionTable m_miJunctionTable;
   RoadNet m_RoadNet;

   const std::string m_arrLeftRight[2] = {"left", "right"};
   const std::map<eSDirection,const char*> _line_type_map = {{eSDirection::plus, "solid"},{eSDirection::minus, "dashed"}};
};
