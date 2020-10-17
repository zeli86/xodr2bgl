/*
 * Copyright 2019 Želimir Marojević
 *
 * Distributed under the Boost Software License, Version 1.0.
 * (See accompanying file LICENSE_1_0.txt
 * or copy at http://www.boost.org/LICENSE_1_0.txt)
 *
 * For more information, see http://www.boost.org
 */

#pragma once

#if defined BUILDING_SHARED || defined BUILDING_STATIC
  #if defined BUILDING_SHARED
    #if defined _WIN32 || defined __CYGWIN__
      #ifdef BUILDING_SHARED
        #ifdef __GNUC__
          #define DLL_PUBLIC __attribute__((dllexport))
        #else
          #define DLL_PUBLIC __declspec(dllexport)
        #endif
      #else
        #ifdef __GNUC__
          #define DLL_PUBLIC __attribute__((dllimport))
        #else
          #define DLL_PUBLIC __declspec(dllimport)
        #endif
      #endif
      #define DLL_LOCAL
    #else
      #if __GNUC__ >= 4
        #define DLL_PUBLIC __attribute__((visibility("default")))
        #define DLL_LOCAL  __attribute__((visibility("hidden")))
      #else
        #define DLL_PUBLIC
        #define DLL_LOCAL
      #endif
    #endif
  #endif
  #if defined BUILDING_STATIC
    #define DLL_PUBLIC
    #define DLL_LOCAL
  #endif
#else
  #define DLL_PUBLIC
  #define DLL_LOCAL
#endif

#include "stdafx.h"

#include <boost/config.hpp>
#include <boost/utility.hpp>
#include <boost/call_traits.hpp>
#include <boost/multi_index_container.hpp>
#include <boost/multi_index/composite_key.hpp>
#include <boost/multi_index/member.hpp>
#include <boost/multi_index/ordered_index.hpp>

#include <boost/graph/directed_graph.hpp>
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/graph_utility.hpp>
#include <boost/graph/graphml.hpp>
#include <boost/graph/graphviz.hpp>
#include <boost/property_map/property_map.hpp>

#include "enums.h"
#include "types.h"
#include "JunctionRow.h"
#include "RoadLaneSectionRow.h"
#include "Ixodr2bgl.h"

struct junctionKey : boost::multi_index::composite_key<
  cJunctionRow,
  BOOST_MULTI_INDEX_MEMBER(cJunctionRow,int32_t,m_iJunctionId),
  BOOST_MULTI_INDEX_MEMBER(cJunctionRow,int32_t,m_iConnectionId)
>{};


using JuctionTable = boost::multi_index::multi_index_container<cJunctionRow,
  boost::multi_index::indexed_by<
  boost::multi_index::ordered_non_unique<junctionKey>,
  boost::multi_index::ordered_non_unique<BOOST_MULTI_INDEX_MEMBER(cJunctionRow,int32_t,m_iIncomingRoadId)>
>>;


struct roadLaneSectionKey : boost::multi_index::composite_key<
  cRoadLaneSectionRow,
  BOOST_MULTI_INDEX_MEMBER(cRoadLaneSectionRow,int32_t,m_iRoadId),
  BOOST_MULTI_INDEX_MEMBER(cRoadLaneSectionRow,int32_t,m_iLaneId)
>{};


using RoadTable = boost::multi_index::multi_index_container<cRoadLaneSectionRow,
  boost::multi_index::indexed_by<
  boost::multi_index::ordered_unique<roadLaneSectionKey>,
  boost::multi_index::ordered_unique<BOOST_MULTI_INDEX_MEMBER(cRoadLaneSectionRow,int32_t,m_iRowId)>
>>;


class xodr2bglBuilder : public Ixodr2bgl
{
  public:
    DLL_PUBLIC xodr2bglBuilder();

    DLL_PUBLIC bool init(const std::string &);

    DLL_PUBLIC void dump(std::ostream&);

    DLL_PUBLIC void setID(int32_t iID)
    {
      m_iID = iID;
    }

    RoadNet m_RoadNet;

  private:

    bool populateDataTables(const std::string &);
    void populateGraph();
    void clear();
    void dumpRoadTable(std::ostream&);
    void dumpJunctionTable(std::ostream&);

    RoadTable m_miRoadTable;
    JuctionTable m_miJunctionTable;

    static const std::string m_arrLeftRight[2];

    std::map<std::string,eSide> m_mapSide;
    std::map<std::string,eLinkType> m_mapLinkType;
    std::map<std::string,eContactPoint> m_mapContactPoint;
};

class cxodr2bglFactory : public Ixodr2bglFactory
{
  public:
  cxodr2bglFactory() { Ixodr2bgl::registerType(1, this); }
  std::unique_ptr<Ixodr2bgl> create() 
  { 
    std::unique_ptr<Ixodr2bgl> pInstance(new xodr2bglBuilder());
    return pInstance; 
  }
};

static cxodr2bglFactory oxodr2bglFactory;
