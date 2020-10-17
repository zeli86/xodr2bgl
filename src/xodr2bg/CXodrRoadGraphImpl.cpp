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
#include "enums.hpp"
#include "types.hpp"

#include <pugiconfig.hpp>
#include <pugixml.hpp>

#include <boost/graph/adjacency_list_io.hpp>
#include <boost/graph/directed_graph.hpp>
#include <boost/graph/graphml.hpp>
#include <boost/graph/graphviz.hpp>
#include <boost/log/trivial.hpp>

#include <limits>
#include <utility>

using namespace std;
using namespace boost;

const std::string CXodrRoadGraphImpl::m_arrLeftRight[2] = {"left", "right"};

namespace {
    boost::optional<RoadNetEdgeItr> move_or_none(boost::optional<RoadNetEdgeItr>&& opt)
    {
      if(opt) return opt;
      return{};
    }
}

CXodrRoadGraphImpl::CXodrRoadGraphImpl()
{
    m_mapContactPoint["start"] = eContactPoint::start;
    m_mapContactPoint["end"] = eContactPoint::end;
    m_mapContactPoint[""] = eContactPoint::unknown;

    m_mapLinkType["junction"] = eLinkType::junction;
    m_mapLinkType["road"] = eLinkType::road;
    m_mapLinkType[""] = eLinkType::unknown;

    m_mapSide["left"] = eSide::left;
    m_mapSide["right"] = eSide::right;
}

bool CXodrRoadGraphImpl::init(const std::string& sFilePath)
{
    BOOST_LOG_TRIVIAL(info) << "Init: " << sFilePath;
    bool bResult = populateDataTables(sFilePath);

    if (bResult)
    {
        populateGraph();
        this->dump(std::cout);
        BOOST_LOG_TRIVIAL(info) << '\n'
                                << boost::write(m_RoadNet);
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

    for (const auto& junctionEl : doc.child("OpenDRIVE").children("junction"))
    {
        cJunctionRow tmpRow;
        tmpRow._junction = junctionEl.attribute("id").as_int();

        for (const auto& connectionEl : junctionEl.children("connection"))
        {
            tmpRow._connection = connectionEl.attribute("id").as_int();
            tmpRow._connecting_road = connectionEl.attribute("connectingRoad").as_int(-1);
            tmpRow._incoming_road = connectionEl.attribute("incomingRoad").as_int(-1);
            tmpRow._linked_road = connectionEl.attribute("linkedRoad").as_int(-1);
            tmpRow.m_eContactPoint = m_mapContactPoint[std::string(connectionEl.attribute("contactPoint").as_string(""))];

            for (const auto& laneLinkEl : connectionEl.children("laneLink"))
            {
                tmpRow._from_lane = laneLinkEl.attribute("from").as_int();
                tmpRow._to_lane = laneLinkEl.attribute("to").as_int();
                m_miJunctionTable.insert(tmpRow);
            }
        }
    }

    int32_t iRowCounter = 1;
    for (const auto& roadEl : doc.child("OpenDRIVE").children("road"))
    {
        cRoadLaneSectionRow tmpRow;

        tmpRow._road = roadEl.attribute("id").as_int();
        tmpRow._junction = roadEl.attribute("junction").as_int();
        tmpRow._length = roadEl.attribute("length").as_double();

        auto node = roadEl.child("link").child("predecessor");
        if (node != nullptr)
        {
            tmpRow.m_ePredRoadContactPt = m_mapContactPoint[node.attribute("contactPoint").as_string()];
            tmpRow.m_ePredRoadLinkType = m_mapLinkType[node.attribute("elementType").as_string()];
            tmpRow._pred = node.attribute("elementId").as_int();
        }

        node = roadEl.child("link").child("successor");
        if (node != nullptr)
        {
            tmpRow.m_eSuccRoadContactPt = m_mapContactPoint[node.attribute("contactPoint").as_string()];
            tmpRow.m_eSuccRoadLinkType = m_mapLinkType[node.attribute("elementType").as_string()];
            tmpRow._succ = node.attribute("elementId").as_int();
        }

        auto lanesections = roadEl.child("lanes").children("laneSection");
        tmpRow._no_sections = std::distance(lanesections.begin(), lanesections.end());

        std::vector<double> aux_sections;
        aux_sections.reserve(20);
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
                tmpRow.m_eLaneSide = m_mapSide[sEl];
                for (const auto& laneEl : laneSectionEl.child(sEl.c_str()).children("lane"))
                {
                    tmpRow._lane = laneEl.attribute("id").as_int();
                    tmpRow.m_sLaneType = laneEl.attribute("type").as_string();
                    tmpRow._section = iLaneSectionCounter;
                    tmpRow._length = aux_sections.at(1+iLaneSectionCounter) - aux_sections.at(iLaneSectionCounter);

                    if (tmpRow.m_sLaneType == "parking")
                        continue;

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
    return true;
}

void CXodrRoadGraphImpl::clear()
{
    m_miRoadTable.clear();
    m_miJunctionTable.clear();
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
        add_new_edges(*it, vertex_counter, edge_counter);
        cIndex.modify(it, [](cRoadLaneSectionRow & row)
        {
            row._processed = 1;
        });
    }

    // loop over all road, that are connecting roads inside a junction
    BOOST_LOG_TRIVIAL(debug) << "Processing junction roads";
    auto it = cIndex.lower_bound(0);
    auto it_end = cIndex.upper_bound(std::numeric_limits<int32_t>::max());
    for (; it != it_end; ++it)
    {
        add_new_edges(*it, vertex_counter, edge_counter);
        cIndex.modify(it, [](cRoadLaneSectionRow & row)
        {
            row._processed = 2;
        });
    }
}

void CXodrRoadGraphImpl::add_new_edges(const cRoadLaneSectionRow& row, int32_t& vertex_counter, int32_t& edge_counter)
{
    BOOST_LOG_TRIVIAL(info) << "Processing " << printer::tupleX("#", "road id", "lane id", "section id") << " := "
                            << printer::tupleX(row._id, row._road, row._lane, row._section);

    boost::optional<RoadNetEdgeItr> pred_edge, succ_edge;
    RoadNetVertex v0, v1;

    if (row.has_sections())
    {
        BOOST_LOG_TRIVIAL(debug) << "Multiple sections\n";

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
            BOOST_LOG_TRIVIAL(debug) << "Has pred roads";
            auto pred_edge_0 = find(row._pred, row._pred_lane, 0);
            if( pred_edge_0.has_value() == true )
            {
                const auto& edge = *(pred_edge_0.value());
                switch(m_RoadNet[edge]._no_sections)
                {
                    case 1: pred_edge = std::move(pred_edge_0);
                            break;
                    default:
                            auto pred_edge_n = find(row._pred, row._pred_lane, m_RoadNet[edge]._no_sections - 1);
                            pred_edge = move_or_none(std::move(pred_edge_n));
                }
            } 
        }
    }

    if (row.has_succ())
    {
        if (row.has_succ_road())
        {
            BOOST_LOG_TRIVIAL(debug) << "Has succ roads";
            auto succ_edge_0 = find(row._succ, row._succ_lane, 0);
            if( succ_edge_0.has_value() == true)
            {
                const auto& edge = *(succ_edge_0.value());
                switch(m_RoadNet[edge]._no_sections)
                {
                    case 1: succ_edge = std::move(succ_edge_0);
                            break;
                    default:
                            auto succ_edge_n = find(row._succ, row._succ_lane, m_RoadNet[edge]._no_sections - 1);
                            succ_edge = move_or_none(std::move(succ_edge_n));
                }
            }
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

void CXodrRoadGraphImpl::to_graphml(const std::string& filepath)
{
    using namespace boost;
    dynamic_properties dp;
    dp.property("weight", boost::get(&cRoadNetEdgeData::_weight, m_RoadNet));
    dp.property("Text", boost::get(&cRoadNetEdgeData::_name, m_RoadNet));

    std::ofstream gv(filepath);
    write_graphml(gv, m_RoadNet, dp);
}

void CXodrRoadGraphImpl::to_txt(std::ostream& os)
{
    os << boost::write(m_RoadNet);
}

void CXodrRoadGraphImpl::to_graphviz(const std::string& filepath)
{
    using namespace boost;
    std::ofstream gv(filepath);

    write_graphviz(gv, m_RoadNet, make_label_writer(get(&cRoadNetVertexData::_vertex_id, m_RoadNet)),
                   make_label_writer(get(&cRoadNetEdgeData::_name, m_RoadNet)));
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

    os << leftcol(11) << "JunctionId"
       << leftcol(13) << "ConnectionId"
       << leftcol(17) << "ConnectingRoadId"
       << leftcol(15) << "IncomingRoadId"
       << leftcol(15) << "LinkedRoadId"
       << leftcol(11) << "FromLaneId"
       << leftcol(9) << "ToLaneId"
       << leftcol(13) << "ContactPoint"
       << "\n";

    for (const auto& el : m_miJunctionTable)
    {
        os << el;
    }
}
