/*
 * Copyright 2024 Želimir Marojević
 *
 * Distributed under the Boost Software License, Version 1.0.
 * (See accompanying file LICENSE_1_0.txt
 * or copy at http://www.boost.org/LICENSE_1_0.txt)
 *
 * For more information, see http://www.boost.org
 */

#include "CXodrRoadGraphImpl.hpp"
#include "types.hpp"
#include "color_maps.hpp"

#include <boost/tuple/tuple.hpp>
#include <pugiconfig.hpp>
#include <pugixml.hpp>

#include <boost/graph/edge_connectivity.hpp>
#include <boost/graph/adjacency_list_io.hpp>
#include <boost/graph/directed_graph.hpp>
#include <boost/log/trivial.hpp>
#include <boost/log/core.hpp>
#include <boost/log/common.hpp>
#include <boost/log/expressions.hpp>

#include <boost/log/utility/setup/common_attributes.hpp>
#include <boost/log/attributes/timer.hpp>
#include <boost/log/attributes/named_scope.hpp>

// #include <boost/log/sources/logger.hpp>
// #include <boost/log/support/date_time.hpp>

#include <limits>
#include <utility>
#include <fstream>
#include <iomanip>

namespace
{
   boost::optional<RoadNetEdgeItr> move_or_none(boost::optional<RoadNetEdgeItr>&& opt)
   {
      if (opt) return opt;
      return{};
   }
}

CXodrRoadGraphImpl::CXodrRoadGraphImpl()
{
   set_log_level();
};

CXodrRoadGraphImpl::~CXodrRoadGraphImpl() = default;

bool CXodrRoadGraphImpl::init(const std::string& sFilePath)
{
   BOOST_LOG_TRIVIAL(info) << "Init: " << sFilePath;
   bool bResult = populateDataTables(sFilePath);

   if (bResult)
   {
      populateGraph();
      //this->dump(std::cout);
      //BOOST_LOG_TRIVIAL(info) << '\n' << boost::write(m_RoadNet);
   }
   return bResult;
}

boost::optional<RoadNetEdgeItr> CXodrRoadGraphImpl::find(const int32_t road, const int32_t lane, const int32_t section)
{
   RoadNetEdgeItr it, it_end;
   boost::tie(it, it_end) = edges(m_RoadNet);

   for (; it != it_end; ++it)
   {
      const auto& data = m_RoadNet[*it];
      if (data._road_id == road && data._lane_id == lane && data._lane_section_id == section)
         return boost::optional<RoadNetEdgeItr>(it);
   }
   return boost::none;
}

bool CXodrRoadGraphImpl::populateDataTables(const std::string& filepath)
{
   pugi::xml_document doc;
   pugi::xml_parse_result result = doc.load_file(filepath.c_str());

   clear();

   BOOST_LOG_TRIVIAL(info) << "Load result: " << result.description();

   int32_t iRowCounter = 1;
   for (const auto& junctionEl : doc.child("OpenDRIVE").children("junction"))
   {
      cJunctionRow tmpRow;
      tmpRow._junction = junctionEl.attribute("id").as_int();
      tmpRow._junction_type = junction_type_bimap.right.at(junctionEl.attribute("type").as_string("default"));

      for (const auto& connectionEl : junctionEl.children("connection"))
      {
         tmpRow._connection = connectionEl.attribute("id").as_int();
         tmpRow._connecting_road = connectionEl.attribute("connectingRoad").as_int(-1);
         tmpRow._incoming_road = connectionEl.attribute("incomingRoad").as_int(-1);
         tmpRow._linked_road = connectionEl.attribute("linkedRoad").as_int(-1);
         tmpRow._eContactPoint = contact_point_bimap.right.at(std::string(connectionEl.attribute("contactPoint").as_string("unknown")));

         for (const auto& laneLinkEl : connectionEl.children("laneLink"))
         {
            tmpRow._from_lane = laneLinkEl.attribute("from").as_int();
            tmpRow._to_lane = laneLinkEl.attribute("to").as_int();
            tmpRow._id = iRowCounter++;
            m_miJunctionTable.insert(tmpRow);
         }
      }
   }

   iRowCounter = 1;
   for (const auto& roadEl : doc.child("OpenDRIVE").children("road"))
   {
      cRoadLaneSectionRow tmpRow;

      tmpRow._road = roadEl.attribute("id").as_int();
      tmpRow._junction = roadEl.attribute("junction").as_int();
      tmpRow._length = roadEl.attribute("length").as_double();

      auto node = roadEl.child("link").child("predecessor");
      if (node != nullptr)
      {
         tmpRow.m_ePredRoadContactPt  = contact_point_bimap.right.at(node.attribute("contactPoint").as_string("unknown"));
         tmpRow.m_ePredRoadLinkType = link_type_bimap.right.at(node.attribute("elementType").as_string("unknown"));
         tmpRow._pred = node.attribute("elementId").as_int();
      }

      node = roadEl.child("link").child("successor");
      if (node != nullptr)
      {
         tmpRow.m_eSuccRoadContactPt = contact_point_bimap.right.at(node.attribute("contactPoint").as_string("unknown"));
         tmpRow.m_eSuccRoadLinkType = link_type_bimap.right.at(node.attribute("elementType").as_string("unknown"));
         tmpRow._succ = node.attribute("elementId").as_int();
      }

      auto lanesections = roadEl.child("lanes").children("laneSection");
      tmpRow._no_sections = std::distance(lanesections.begin(), lanesections.end());

      std::vector<double> aux_sections;
      aux_sections.reserve(25);
      for (const auto& laneSectionEl : lanesections)
      {
         const double s_val = laneSectionEl.attribute("s").as_double(0);
         aux_sections.push_back(s_val);
      }
      aux_sections.push_back(tmpRow._length);

      int32_t iLaneSectionCounter = 0;
      for (const auto& laneSectionEl : lanesections)
      {
         for (const auto& sEl : m_arrLeftRight)
         {
            tmpRow.m_eLaneSide = side_bimap.right.at(sEl);
            for (const auto& laneEl : laneSectionEl.child(sEl.c_str()).children("lane"))
            {
               tmpRow._lane = laneEl.attribute("id").as_int();
               tmpRow.m_sLaneType = laneEl.attribute("type").as_string();
               tmpRow._section = iLaneSectionCounter;
               tmpRow._length = aux_sections.at(1+iLaneSectionCounter) - aux_sections.at(iLaneSectionCounter);

               // if (tmpRow.m_sLaneType == "parking")
               //    continue;

               tmpRow._succ_lane_valid = (laneEl.child("link").child("successor") != nullptr);
               if (tmpRow._succ_lane_valid)
               {
                  tmpRow._succ_lane = laneEl.child("link").child("successor").attribute("id").as_int();
               }
               else
               {
                  tmpRow._succ_lane = 0;
               }

               tmpRow._pred_lane_valid = (laneEl.child("link").child("predecessor") != nullptr);
               if (tmpRow._pred_lane_valid)
               {
                  tmpRow._pred_lane = laneEl.child("link").child("predecessor").attribute("id").as_int();
               }
               else
               {
                  tmpRow._pred_lane = 0;
               }

               tmpRow._id = iRowCounter;
               const auto paResult = m_miRoadTable.insert(tmpRow);
               if (paResult.second == true)
               {
                  ++iRowCounter;
               }
            }
         }
         ++iLaneSectionCounter;
      }
   }

   updateRoadTable();
   return true;
}


void CXodrRoadGraphImpl::updateRoadTable()
{
   auto& index_junctions = m_miJunctionTable.get<0>();
   auto& index_roads = m_miRoadTable.get<0>();

   auto r = index_junctions.equal_range(eJunctionType::direct);
   for (auto it = r.first; it != r.second; it++)
   {
      const auto& junction = *it;
      auto in = index_roads.upper_bound(boost::make_tuple(junction._incoming_road,junction._from_lane, 1048576));
      --in;

      if (in == index_roads.end())
      {
         continue;
      }

      decltype(in) out;
      switch (junction._eContactPoint)
      {
         case eContactPoint::start:
            out = index_roads.find(boost::make_tuple(junction._linked_road,junction._to_lane, 0));
            break;
         case eContactPoint::end:
            out = index_roads.upper_bound(boost::make_tuple(junction._linked_road,junction._to_lane, 1048576));
            --out;
            break;
         case eContactPoint::unknown:
            continue;
      }

      if (out == index_roads.end())
      {
         continue;
      }

      index_roads.modify(in, [junction](cRoadLaneSectionRow & row)
      {
         row._succ = junction._linked_road;
         row._succ_lane_valid = true;
         row._succ_lane = junction._to_lane;
         row.m_eSuccRoadContactPt = junction._eContactPoint;
         row.m_eSuccRoadLinkType = eLinkType::road;
      });

      index_roads.modify(out, [junction](cRoadLaneSectionRow & row)
      {
         row._pred = junction._incoming_road;
         row._pred_lane_valid = true;
         row._pred_lane = junction._from_lane;
         row.m_ePredRoadContactPt = eContactPoint::end;
         row.m_ePredRoadLinkType = eLinkType::road;
      });
   }
}

void CXodrRoadGraphImpl::populateGraph()
{
   int32_t vertex_counter{0};
   int32_t edge_counter{0};

   auto& cIndex = m_miRoadTable.get<2>();

   BOOST_LOG_TRIVIAL(debug) << "Processing regular roads";
   // loop over all regular roads
   auto r = cIndex.equal_range(-1);
   for (auto it = r.first; it != r.second; it++)
   {
      if (is_enabled(*it))
      {
         add_new_edges(*it, vertex_counter, edge_counter);
         cIndex.modify(it, [](cRoadLaneSectionRow & row)
         {
            row._processed = 1;
         });
      }
   }

   // loop over all road, that are connecting roads inside a junction
   BOOST_LOG_TRIVIAL(debug) << "Processing junction roads";
   auto it = cIndex.lower_bound(0);
   auto it_end = cIndex.upper_bound(std::numeric_limits<int32_t>::max());
   for (; it != it_end; ++it)
   {
      if (is_enabled(*it))
      {
         add_new_edges(*it, vertex_counter, edge_counter);
         cIndex.modify(it, [](cRoadLaneSectionRow & row)
         {
            row._processed = 2;
         });
      }
   }
}

void CXodrRoadGraphImpl::add_new_edges(const cRoadLaneSectionRow& row, int32_t& vertex_counter, int32_t& edge_counter)
{
   BOOST_LOG_TRIVIAL(info) << "Processing row " << row._id;
   BOOST_LOG_TRIVIAL(info) << printer::tupleX("road", "lane", "section") << " := " << printer::tupleX(row._road, row._lane, row._section);

   boost::optional<RoadNetEdgeItr> pred_edge, succ_edge;
   RoadNetVertex v0, v1;

   if (row.has_sections())
   {
      BOOST_LOG_TRIVIAL(debug) << "+ multiple lane sections";

      if (row.has_pred_section())
      {
         pred_edge = find(row._road, row._pred_lane, row._section - 1);
      }
      if (pred_edge.has_value())
      {
         const auto& edge = *(pred_edge.value());
         v0 = target(edge, m_RoadNet);
      }

      if (row.has_succ_section())
      {
         succ_edge = find(row._road, row._pred_lane, row._section + 1);
      }

      if (succ_edge.has_value())
      {
         const auto& edge = *(succ_edge.value());
         v1 = source(edge, m_RoadNet);
      }
   }

   if (row.has_pred())
   {
      if (row.has_pred_road())
      {
         BOOST_LOG_TRIVIAL(debug) << "+ predecessor road";
         auto pred_edge_0 = find(row._pred, row._pred_lane, 0);
         if (pred_edge_0.has_value() == true)
         {
            switch (row.m_ePredRoadContactPt)
            {
               case eContactPoint::start:
               {
                  pred_edge = std::move(pred_edge_0);
               }
               break;
               case eContactPoint::end:
               {
                  const auto& edge = *(pred_edge_0.value());
                  auto pred_edge_n = find(row._pred, row._pred_lane, m_RoadNet[edge]._no_sections - 1);
                  pred_edge = move_or_none(std::move(pred_edge_n));
               }
               break;
               case eContactPoint::unknown:
                  break;
            }
         }
      }
      if (row.has_pred_direct_junction())
      {
         BOOST_LOG_TRIVIAL(debug) << "+ predecessor direct junction";
      }
   }

   if (row.has_succ())
   {
      if (row.has_succ_road())
      {
         BOOST_LOG_TRIVIAL(debug) << "+ successor road";
         auto succ_edge_0 = find(row._succ, row._succ_lane, 0);
         if (succ_edge_0.has_value() == true)
         {
            switch (row.m_eSuccRoadContactPt)
            {
               case eContactPoint::start:
               {
                  succ_edge = std::move(succ_edge_0);
               }
               break;
               case eContactPoint::end:
               {
                  const auto& edge = *(succ_edge_0.value());
                  auto succ_edge_n = find(row._succ, row._succ_lane, m_RoadNet[edge]._no_sections - 1);
                  succ_edge = move_or_none(std::move(succ_edge_n));
               }
               break;
               case eContactPoint::unknown:
                  break;
            }
         }
      }
      if (row.has_succ_direct_junction())
      {
         BOOST_LOG_TRIVIAL(debug) << "+ succecessor direct junction";
      }
   }

   if (pred_edge.has_value() == true)
   {
      if (row._section == 0)
      {
         const auto& edge = *(pred_edge.value());
         if (row.m_ePredRoadContactPt == eContactPoint::start)
         {
            v0 = source(edge, m_RoadNet);
         }
         else
         {
            v0 = target(edge, m_RoadNet);
         }
      }
   }
   else
   {
      v0 = boost::add_vertex(cRoadNetVertexData(vertex_counter++), m_RoadNet);
   }

   if (succ_edge.has_value() == true)
   {
      if (1 + row._section == row._no_sections)
      {
         const auto& edge = *(succ_edge.value());
         if (row.m_eSuccRoadContactPt == eContactPoint::start)
         {
            v1 = source(edge, m_RoadNet);
         }
         else
         {
            v1 = target(edge, m_RoadNet);
         }
      }
   }
   else
   {
      v1 = boost::add_vertex(cRoadNetVertexData(vertex_counter++), m_RoadNet);
   }
   boost::add_edge(v0, v1, cRoadNetEdgeData(++edge_counter, row), m_RoadNet);
}

void CXodrRoadGraphImpl::clear()
{
   m_miRoadTable.clear();
   m_miJunctionTable.clear();
}

void CXodrRoadGraphImpl::to_txt(std::ostream& os)
{
   os << boost::write(m_RoadNet);
}

void CXodrRoadGraphImpl::to_graphviz(const std::string& filepath)
{
   using namespace boost;
   std::ofstream gv(filepath);

   RoadNetVertexItr vi, vi_end;
   RoadNetEdgeItr ei, ei_end;

   gv << "digraph G {\n";
   for (boost::tie(vi, vi_end) = vertices(m_RoadNet); vi != vi_end; ++vi)
   {
      const auto id = m_RoadNet[*vi]._vertex_id;
      gv << id << "[label=" << id << "];\n";
   }
   for (boost::tie(ei, ei_end) = edges(m_RoadNet); ei != ei_end; ++ei)
   {
      const auto id0 = m_RoadNet[source(*ei, m_RoadNet)]._vertex_id;
      const auto id1 = m_RoadNet[target(*ei, m_RoadNet)]._vertex_id;
      const auto ed = m_RoadNet[*ei];
      gv << id0 << "->" << id1
         << "[label=\"" << printer::tupleX(ed._road_id, ed._lane_id, ed._lane_section_id) << "\\n"
         << std::fixed << std::setprecision(2) << ed._length << "\\n"
         << ed._road_lane_type
         << "\",color=" << get_lane_type_color(ed._road_lane_type)
         <<  "];\n";
   }
   gv << "}\n";
}

void CXodrRoadGraphImpl::set_lane_type_filter(const CLaneTypeFilter& filter)
{
   _lane_type_filter = filter;
}

void CXodrRoadGraphImpl::set_log_level(const unsigned lvl)
{
   using namespace boost;

   log::add_common_attributes();
   log::core::get()->add_thread_attribute("Scope", log::attributes::named_scope());

   log::core::get()->set_filter
   (
      log::trivial::severity >= lvl
   );
}

bool CXodrRoadGraphImpl::is_enabled(const cRoadLaneSectionRow& row)
{
   auto it = lane_type_filter_map.find(row.m_sLaneType);
   if (it == lane_type_filter_map.end())
   {
      return false;
   }
   return _lane_type_filter.test(it->second);
}

void CXodrRoadGraphImpl::dump(std::ostream& os)
{
   dumpRoadTable(os);
   os << '\n';
   dumpJunctionTable(os);
   os << '\n';
}

void CXodrRoadGraphImpl::dumpRoadTable(std::ostream& os)
{
   using namespace printer;

   os << leftcol(2) << "? "
      << leftcol(4) << "#"
      << leftcol(5) << "Road"
      << leftcol(5) << "Lane"
      << leftcol(5) << "Sct"
      << leftcol(9) << "Junction"
      << leftcol(7) << "Length"
      << leftcol(7) << "SuccId"
      << leftcol(14) << "SuccContactPt"
      << leftcol(13) << "SuccLinkType"
      << leftcol(7) << "PredId"
      << leftcol(14) << "PredContactPt"
      << leftcol(13) << "PredLinkType"
      << leftcol(14) << "PredLaneValid"
      << leftcol(9) << "PredLane"
      << leftcol(14) << "SuccLaneValid"
      << leftcol(9) << "SuccLane"
      << leftcol(9) << "LaneType"
      << leftcol(10) << "NoLaneSec"
      << "\n";

   const auto& idx = m_miRoadTable.get<1>();
   for (auto it = idx.cbegin(); it != idx.cend(); ++it)
   {
      os << *it;
   }
}

void CXodrRoadGraphImpl::dumpJunctionTable(std::ostream& os)
{
   using namespace printer;

   os << leftcol(4) << "#"
      << leftcol(9) << "Junction"
      << leftcol(11) << "Connection"
      << leftcol(15) << "ConnectingRoad"
      << leftcol(13) << "IncomingRoad"
      << leftcol(13) << "LinkedRoad"
      << leftcol(9) << "FromLane"
      << leftcol(9) << "ToLane"
      << leftcol(9) << "Type"
      << leftcol(13) << "ContactPoint"
      << "\n";

   for (const auto& el : m_miJunctionTable)
   {
      os << el;
   }
}
