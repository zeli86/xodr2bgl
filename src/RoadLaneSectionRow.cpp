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
#include "RoadLaneSectionRow.h"

std::ostream & operator << (std::ostream & os, const cRoadLaneSectionRow & o)
{
  os << std::left << std::setw(6)  << o.m_iRowId
     << std::left << std::setw(7)  << o.m_iRoadId
     << std::left << std::setw(7)  << o.m_iLaneId
     << std::left << std::setw(11) << o.m_iJunctionId
     << std::left << std::setw(11) << o.m_rRoadLength
     << std::left << std::setw(16) << std::boolalpha<< o.m_bSuccRoadIdValid
     << std::left << std::setw(11) << o.m_iSuccRoadId
     << std::left << std::setw(14) << mapContactPointToString[o.m_eSuccRoadContactPt]
     << std::left << std::setw(13) << mapLinkTypeToString[o.m_eSuccRoadLinkType]
     << std::left << std::setw(16) << std::boolalpha<< o.m_bPredRoadIdValid
     << std::left << std::setw(11) << o.m_iPredRoadId
     << std::left << std::setw(14) << mapContactPointToString[o.m_ePredRoadContactPt]
     << std::left << std::setw(13) << mapLinkTypeToString[o.m_ePredRoadLinkType]
     << std::left << std::setw(16) << std::boolalpha << o.m_bPredLaneIdValid
     << std::left << std::setw(11) << o.m_iPredLaneId
     << std::left << std::setw(16) << std::boolalpha << o.m_bSuccLaneIdValid
     << std::left << std::setw(11) << o.m_iSuccLaneId
     << std::left << std::setw(9) << o.m_sLaneType
     << std::left << std::setw(10) << o.m_iNoOfLaneSections;
    //  << std::left << std::setw(15) << o.m_pRoadNetVertex1
    //  << std::left << std::setw(15) << o.m_pRoadNetVertex2
    //  << std::left << std::setw(15) << o.m_pRoadNetEdge

  for( const auto rVal : o.m_vLaneSectionStarts )
  {
    os << rVal << " ";
  }
  os << "\n";
return os;
}
