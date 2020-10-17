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
#include "xodr2bglBuilder.h"

#include "../ThirdParty/pugixml-1.10/src/pugiconfig.hpp"
#include "../ThirdParty/pugixml-1.10/src/pugixml.hpp"

using namespace std;
using namespace boost;

const std::string xodr2bglBuilder::m_arrLeftRight[2] = {"left","right"};

xodr2bglBuilder::xodr2bglBuilder()
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

bool xodr2bglBuilder::init(const std::string & sFilePath)
{
  bool bResult = populateDataTables(sFilePath);

  if(bResult)
  {
    populateGraph();
    this->dump(std::cout);
  }

  return bResult;
}

bool xodr2bglBuilder::populateDataTables(const std::string & filepath)
{
  pugi::xml_document doc;
  pugi::xml_parse_result result = doc.load_file(filepath.c_str());

  clear();

  cout << "Load result: " << result.description() << endl;

  for (const auto & junctionEl : doc.child("OpenDRIVE").children("junction"))
  {
    cJunctionRow tmpRow;
    tmpRow.m_iJunctionId = junctionEl.attribute("id").as_int();

    for (const auto & connectionEl : junctionEl.children("connection"))
    {
      tmpRow.m_iConnectionId = connectionEl.attribute("id").as_int();
      tmpRow.m_iConnectingRoadId = connectionEl.attribute("connectingRoad").as_int(-1);
      tmpRow.m_iIncomingRoadId = connectionEl.attribute("incomingRoad").as_int(-1);
      tmpRow.m_eContactPoint = m_mapContactPoint[std::string(connectionEl.attribute("contactPoint").as_string(""))];

      for (const auto & laneLinkEl : connectionEl.children("laneLink"))
      {
        tmpRow.m_iFromLaneId = laneLinkEl.attribute("from").as_int();
        tmpRow.m_iToLaneId = laneLinkEl.attribute("to").as_int();
        m_miJunctionTable.insert(tmpRow);
      }
    }
  }

  int32_t iRowCounter = 1;
  for (const auto & roadEl : doc.child("OpenDRIVE").children("road"))
  {
    cRoadLaneSectionRow tmpRow;

    tmpRow.m_iRoadId = roadEl.attribute("id").as_int();
    tmpRow.m_iJunctionId = roadEl.attribute("junction").as_int();
    tmpRow.m_rRoadLength = roadEl.attribute("length").as_double();

    auto node = roadEl.child("link").child("predecessor");
    if( node != nullptr )
    {
      tmpRow.m_bPredRoadIdValid = true;
      tmpRow.m_ePredRoadContactPt = m_mapContactPoint[node.attribute("contactPoint").as_string()];
      tmpRow.m_ePredRoadLinkType =  m_mapLinkType[node.attribute("elementType").as_string()];
      tmpRow.m_iPredRoadId = node.attribute("elementId").as_int();
    }

    node = roadEl.child("link").child("successor");
    if( node != nullptr )
    {
      tmpRow.m_bSuccRoadIdValid = true;
      tmpRow.m_eSuccRoadContactPt = m_mapContactPoint[node.attribute("contactPoint").as_string()];
      tmpRow.m_eSuccRoadLinkType = m_mapLinkType[node.attribute("elementType").as_string()];
      tmpRow.m_iSuccRoadId = node.attribute("elementId").as_int();
    }

    tmpRow.m_iNoOfLaneSections = std::distance(roadEl.child("lanes").children("laneSection").begin(),roadEl.child("lanes").children("laneSection").end());

    for (const auto & laneSectionEl : roadEl.child("lanes").children("laneSection"))
    {
      const double aux_rLaneSectionsStart = laneSectionEl.attribute("s").as_double(0);
      tmpRow.m_vLaneSectionStarts.push_back(aux_rLaneSectionsStart);
      for (const auto & sEl : m_arrLeftRight)
      {
        tmpRow.m_eLaneSide = m_mapSide[sEl];
        for (const auto & laneEl : laneSectionEl.child(sEl.c_str()).children("lane"))
        {
          tmpRow.m_iLaneId = laneEl.attribute("id").as_int();
          tmpRow.m_sLaneType = laneEl.attribute("type").as_string();

          if( tmpRow.m_sLaneType == "parking" ) continue;

          tmpRow.m_bSuccLaneIdValid = (laneEl.child("link").child("successor") != nullptr);
          if(tmpRow.m_bSuccLaneIdValid)
          {
            tmpRow.m_iSuccLaneId = laneEl.child("link").child("successor").attribute("id").as_int();
          }
          else
          {
            tmpRow.m_iPredLaneId = 0;
          }

          tmpRow.m_bPredLaneIdValid = (laneEl.child("link").child("predecessor") != nullptr);
          if(tmpRow.m_bPredLaneIdValid)
          {
            tmpRow.m_iPredLaneId = laneEl.child("link").child("predecessor").attribute("id").as_int();
          }
          else
          {
            tmpRow.m_iPredLaneId = 0;
          }

          tmpRow.m_iRowId = iRowCounter+1;
          const auto paResult = m_miRoadTable.insert(tmpRow);
          if( paResult.second == false )
          {
            m_miRoadTable.get<0>().modify(paResult.first, [aux_rLaneSectionsStart](cRoadLaneSectionRow & cRow)
            {
              cRow.m_vLaneSectionStarts.push_back(aux_rLaneSectionsStart);
            });
          }
          else
          {
            iRowCounter++;
          }
        }
      }
    }
  }
return true;
}


void xodr2bglBuilder::clear()
{
  m_miRoadTable.clear();
  m_miJunctionTable.clear();
}


void xodr2bglBuilder::dump(std::ostream& os)
{
  dumpRoadTable(os);
  dumpJunctionTable(os);
}


void xodr2bglBuilder::dumpRoadTable(std::ostream& os)
{
  os << std::left << std::setw(6) << "RowId"
     << std::left << std::setw(7) << "RoadId"
     << std::left << std::setw(7) << "LaneId"
     << std::left << std::setw(11) << "JunctionId"
     << std::left << std::setw(11) << "RoadLength"
     << std::left << std::setw(16) << "SuccRoadIdValid"
     << std::left << std::setw(11) << "SuccRoadId"
     << std::left << std::setw(14) << "SuccContactPt"
     << std::left << std::setw(13) << "SuccLinkType"
     << std::left << std::setw(16) << "PredRoadIdValid"
     << std::left << std::setw(11) << "PredRoadId"
     << std::left << std::setw(14) << "PredContactPt"
     << std::left << std::setw(13) << "PredLinkType"
     << std::left << std::setw(16) << "PredLaneIdValid"
     << std::left << std::setw(11) << "PredLaneId"
     << std::left << std::setw(16) << "SuccLaneIdValid"
     << std::left << std::setw(11) << "SuccLaneId"
     << std::left << std::setw(9)  << "LaneType"
     << std::left << std::setw(10) << "NoLaneSec"
     << "\n";

  for( const auto & el : m_miRoadTable )
  {
    os << el;
  }
}


void xodr2bglBuilder::dumpJunctionTable(std::ostream& os)
{
  os << std::left << std::setw(11) << "JunctionId"
     << std::left << std::setw(13) << "ConnectionId"
     << std::left << std::setw(17) << "ConnectingRoadId"
     << std::left << std::setw(15) << "IncomingRoadId"
     << std::left << std::setw(11) << "FromLaneId"
     << std::left << std::setw(9)  << "ToLaneId"
     << std::left << std::setw(13) << "ContactPoint"
     << "\n";

  for( const auto & el : m_miJunctionTable )
  {
    os << el;
  }
}


void xodr2bglBuilder::populateGraph()
{
/*  int32_t iVertexCounter = 1;

  auto & cIndex = m_miRoadTable.get<0>();
  for( auto cIt = cIndex.begin(); cIt != cIndex.end(); ++cIt )
  {
    std::cout << std::string(200,'*') << "\n";
    std::cout << *cIt;

    if( cIt->m_pRoadNetVertex1 == nullptr )
    {
      auto cItLow = cIndex.upper_bound(std::make_tuple(cIt->m_iRoadId,cIt->m_iLaneId,cIt->m_rLaneSectionStart));
      const bool bPredLaneSecExists = (cIt->m_iRoadId == cItLow->m_iRoadId) && (cIt->m_iLaneId == cItLow->m_iLaneId) && (cIt->m_rLaneSectionStart < cItLow->m_rLaneSectionStart);
      const bool bPredRoadExists = !bPredLaneSecExists && cIt->m_bPredRoadIdValid && cIt->m_bPredLaneIdValid; //  && cIt->m_iJunctionId == -1;
      const bool bPredJuncExists = !bPredLaneSecExists && cIt->bIsPredJunction();
      const bool bDeadEnd = !(bPredLaneSecExists || bPredRoadExists || bPredJuncExists);

      //std::cout << "cItLow : " << cItLow->m_iRowId << ", " << cItLow->m_iRoadId << ", " << cItLow->m_iLaneId << ", " << cItLow->m_rLaneSectionStart << "\n";
      std::cout << std::boolalpha << "bPredLaneSecExists : "  << bPredLaneSecExists << "\n";
      std::cout << std::boolalpha << "bPredRoadExists    : "  << bPredRoadExists << "\n";
      std::cout << std::boolalpha << "bPredJuncExists    : "  << bPredJuncExists << "\n";
      std::cout << std::boolalpha << "bDeadEnd           : "  << bDeadEnd << "\n";

      if( bPredLaneSecExists )
      {
        if( cItLow->m_pRoadNetVertex2 == nullptr)
        {
          cIndex.modify(cIt,[this,iVertexCounter](cRoadLaneSectionRow & cRow){cRow.m_pRoadNetVertex1 = boost::add_vertex(cRoadNetVertexData{iVertexCounter},this->m_RoadNet);});
          iVertexCounter++;
        }
        else
        {
          cIndex.modify(cIt,[cItLow,iVertexCounter](cRoadLaneSectionRow & cRow){cRow.m_pRoadNetVertex1 = cItLow->m_pRoadNetVertex2;});
        }
      }

      if( bPredRoadExists )
      {
        if( cIt->m_ePredRoadLinkType == eLinkType::road && cItLow->m_ePredRoadContactPt == eContactPoint::start )
        {
          const auto iPredRoadId = cIt->m_iPredRoadId;
          const auto iPredLaneId = cIt->m_iPredLaneId;

          const auto cItTarget = cIndex.find(std::make_tuple(iPredRoadId,iPredLaneId,0.0));
          if( cItTarget->m_pRoadNetVertex2 == nullptr)
          {
            cIndex.modify(cIt,[this,iVertexCounter](cRoadLaneSectionRow & cRow){cRow.m_pRoadNetVertex1 = boost::add_vertex(cRoadNetVertexData{iVertexCounter},this->m_RoadNet);});
            iVertexCounter++;
          }
          else
          {
            cIndex.modify(cIt,[cItTarget,iVertexCounter](cRoadLaneSectionRow & cRow){cRow.m_pRoadNetVertex1 = cItTarget->m_pRoadNetVertex2;});
          }
          //std::cout << "cItLow : " << cItLow->m_iRowId << ", " << cItLow->m_iRoadId << ", " << cItLow->m_iLaneId << ", " << cItLow->m_rLaneSectionStart << "\n";
          //std::cout << "cItTarget : " << cItTarget->m_iRowId << ", " << cItTarget->m_iRoadId << ", " << cItTarget->m_iLaneId << ", " << cItTarget->m_rLaneSectionStart << "\n";
        }

      }

      if( bPredJuncExists )
      {
        auto cPairIt = m_miJunctionTable.get<1>().equal_range(cIt->m_iRoadId);

        for( ; cPairIt.first != cPairIt.second; ++cPairIt.first )
        {
          const auto & cJunctionRow = *(cPairIt.first);

          if( cIt->m_iLaneId == cJunctionRow.m_iFromLaneId)
          {
             const auto cItTarget = m_miRoadTable.get<0>().find(std::make_tuple(cJunctionRow.m_iConnectingRoadId,cJunctionRow.m_iToLaneId,0.0));
             if( cItTarget != m_miRoadTable.get<0>().end() )
             {
               if( cItTarget->m_pRoadNetVertex1 == nullptr)
               {
                 cIndex.modify(cIt,[this,iVertexCounter](cRoadLaneSectionRow & cRow){cRow.m_pRoadNetVertex1 = boost::add_vertex(cRoadNetVertexData{iVertexCounter},this->m_RoadNet);});
                 iVertexCounter++;
               }
               else
               {
                 cIndex.modify(cIt,[cItTarget,iVertexCounter](cRoadLaneSectionRow & cRow){cRow.m_pRoadNetVertex1 = cItTarget->m_pRoadNetVertex2;});
               }
             }
             std::cout << cJunctionRow.m_iConnectingRoadId << ", "
                       << cJunctionRow.m_iIncomingRoadId << ", "
                       << cJunctionRow.m_iFromLaneId << ", "
                       << cJunctionRow.m_iToLaneId
                       << "\n";
          }
        }
      }

      if( bDeadEnd )
      {
        cIndex.modify(cIt,[this,iVertexCounter](cRoadLaneSectionRow & cRow){cRow.m_pRoadNetVertex1 = boost::add_vertex(cRoadNetVertexData{iVertexCounter},this->m_RoadNet);});
        iVertexCounter++;
      }
    }


    if( cIt->m_pRoadNetVertex2 == nullptr )
    {
      auto cItLow = cIndex.upper_bound(std::make_tuple(cIt->m_iRoadId,cIt->m_iLaneId,cIt->m_rLaneSectionStart));
      const bool bSuccLaneSecExists = (cIt->m_iRoadId == cItLow->m_iRoadId) && (cIt->m_iLaneId == cItLow->m_iLaneId) && (cIt->m_rLaneSectionStart < cItLow->m_rLaneSectionStart);
      const bool bSuccRoadExists = !bSuccLaneSecExists && cIt->m_bSuccRoadIdValid && cIt->m_bSuccLaneIdValid; //  && cIt->m_iJunctionId == -1;
      const bool bSuccJuncExists = !bSuccLaneSecExists && cIt->bIsSuccJunction();
      const bool bDeadEnd = !(bSuccLaneSecExists || bSuccRoadExists || bSuccJuncExists);

      //std::cout << "cItLow : " << cItLow->m_iRowId << ", " << cItLow->m_iRoadId << ", " << cItLow->m_iLaneId << ", " << cItLow->m_rLaneSectionStart << "\n";
      std::cout << std::boolalpha << "bNextLaneSecExists : "  << bSuccLaneSecExists << "\n";
      std::cout << std::boolalpha << "bSuccRoadExists    : "  << bSuccRoadExists << "\n";
      std::cout << std::boolalpha << "bSuccJuncExists    : "  << bSuccJuncExists << "\n";
      std::cout << std::boolalpha << "bDeadEnd           : "  << bDeadEnd << "\n";

      if( bSuccLaneSecExists )
      {
        if( cItLow->m_pRoadNetVertex1 == nullptr)
        {
          cIndex.modify(cIt,[this,iVertexCounter](cRoadLaneSectionRow & cRow){cRow.m_pRoadNetVertex2 = boost::add_vertex(cRoadNetVertexData{iVertexCounter},this->m_RoadNet);});
          iVertexCounter++;
        }
        else
        {
          cIndex.modify(cIt,[cItLow,iVertexCounter](cRoadLaneSectionRow & cRow){cRow.m_pRoadNetVertex2 = cItLow->m_pRoadNetVertex1;});
        }
      }

      if( bSuccRoadExists )
      {
        if( cIt->m_eSuccRoadLinkType == eLinkType::road && cItLow->m_eSuccRoadContactPt == eContactPoint::start )
        {
          const auto iSuccRoadId = cIt->m_iSuccRoadId;
          const auto iSuccLaneId = cIt->m_iSuccLaneId;

          const auto cItTarget = cIndex.find(std::make_tuple(iSuccRoadId,iSuccLaneId,0.0));
          if( cItTarget->m_pRoadNetVertex1 == nullptr)
          {
            cIndex.modify(cIt,[this,iVertexCounter](cRoadLaneSectionRow & cRow){cRow.m_pRoadNetVertex2 = boost::add_vertex(cRoadNetVertexData{iVertexCounter},this->m_RoadNet);});
            iVertexCounter++;
          }
          else
          {
            cIndex.modify(cIt,[cItTarget,iVertexCounter](cRoadLaneSectionRow & cRow){cRow.m_pRoadNetVertex2 = cItTarget->m_pRoadNetVertex1;});
          }
          //std::cout << "cItLow : " << cItLow->m_iRowId << ", " << cItLow->m_iRoadId << ", " << cItLow->m_iLaneId << ", " << cItLow->m_rLaneSectionStart << "\n";
          //std::cout << "cItTarget : " << cItTarget->m_iRowId << ", " << cItTarget->m_iRoadId << ", " << cItTarget->m_iLaneId << ", " << cItTarget->m_rLaneSectionStart << "\n";
        }

      }

      if( bSuccJuncExists )
      {
        auto cPairIt = m_miJunctionTable.get<1>().equal_range(cIt->m_iRoadId);

        for( ; cPairIt.first != cPairIt.second; ++cPairIt.first )
        {
          const auto & cJunctionRow = *(cPairIt.first);

          if( cIt->m_iLaneId == cJunctionRow.m_iFromLaneId)
          {
             const auto cItTarget = m_miRoadTable.get<0>().find(std::make_tuple(cJunctionRow.m_iConnectingRoadId,cJunctionRow.m_iToLaneId,0.0));
             if( cItTarget != m_miRoadTable.get<0>().end() )
             {
               if( cItTarget->m_pRoadNetVertex1 == nullptr)
               {
                 cIndex.modify(cIt,[this,iVertexCounter](cRoadLaneSectionRow & cRow){cRow.m_pRoadNetVertex2 = boost::add_vertex(cRoadNetVertexData{iVertexCounter},this->m_RoadNet);});
                 iVertexCounter++;
               }
               else
               {
                 cIndex.modify(cIt,[cItTarget,iVertexCounter](cRoadLaneSectionRow & cRow){cRow.m_pRoadNetVertex2 = cItTarget->m_pRoadNetVertex1;});
               }
             }
             std::cout << cJunctionRow.m_iConnectingRoadId << ", "
                       << cJunctionRow.m_iIncomingRoadId << ", "
                       << cJunctionRow.m_iFromLaneId << ", "
                       << cJunctionRow.m_iToLaneId
                       << "\n";
          }
        }
      }

      if( bDeadEnd )
      {
        cIndex.modify(cIt,[this,iVertexCounter](cRoadLaneSectionRow & cRow){cRow.m_pRoadNetVertex2 = boost::add_vertex(cRoadNetVertexData{iVertexCounter},this->m_RoadNet);});
        iVertexCounter++;
      }
    }
  }

  for( auto cIt = cIndex.begin(); cIt != cIndex.end(); ++cIt )
  {
    if( cIt->m_pRoadNetVertex1 != nullptr && cIt->m_pRoadNetVertex2 != nullptr )
    {
      auto cNewEdge = boost::add_edge(cIt->m_pRoadNetVertex1,cIt->m_pRoadNetVertex2,m_RoadNet);
      //auto blah = cNewEdge.first;
      //cIndex.modify(cIt,[this](cRoadLaneSectionRow & cRow){cRow.m_pRoadNetEdge = ;});
    }
  }

  //std::ofstream gv("/home/zeli/roadnet.dot");
  //boost::write_graphviz(gv, m_RoadNet,
  //                      boost::make_label_writer(boost::get(&cRoadNetVertexData::m_iVertexId, m_RoadNet)),
  //                      boost::make_label_writer(boost::get(&cRoadNetEdgeData::m_iRowId, m_RoadNet)));
  */
}

/*
void xodr_bgl_builder::populateGraph()
{
  decltype (cItLow) cItOld;

  //std::ofstream gv("/home/zeli/roadnet.gv");
  //boost::write_graphviz(gv, m_RoadNet,boost::make_label_writer(boost::get(&JunctionVertexData::name, m_RoadNet)));
}
*/

