/*
 * Copyright 2024 Želimir Marojević
 *
 * Distributed under the Boost Software License, Version 1.0.
 * (See accompanying file LICENSE_1_0.txt
 * or copy at http://www.boost.org/LICENSE_1_0.txt)
 *
 * For more information, see http://www.boost.org
 */

#include "RoadLaneSectionRow.hpp"
#include "helper.hpp"
#include <iostream>

std::ostream& operator<<(std::ostream& os, const cRoadLaneSectionRow& o)
{
    using namespace printer;

    os << leftcol(2) << o._processed
       << leftcol(4) << o._id
       << leftcol(5) << o._road
       << leftcol(5) << o._lane
       << leftcol(5) << o._section
       << leftcol(9) << o._junction
       << leftcol(7) << o._length
       << leftcol(7) << o._succ
       << leftcol(14) << mapContactPointToString[o.m_eSuccRoadContactPt]
       << leftcol(13) << mapLinkTypeToString[o.m_eSuccRoadLinkType]
       << leftcol(7) << o._pred
       << leftcol(14) << mapContactPointToString[o.m_ePredRoadContactPt]
       << leftcol(13) << mapLinkTypeToString[o.m_ePredRoadLinkType]
       << leftcol(14) << std::boolalpha << o._pred_lane_valid
       << leftcol(9) << o._pred_lane
       << leftcol(14) << std::boolalpha << o._succ_lane_valid
       << leftcol(9) << o._succ_lane
       << leftcol(9) << o.m_sLaneType
       << leftcol(10) << o._no_sections << '\n';
    return os;
}
