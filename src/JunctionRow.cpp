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
#include "JunctionRow.h"

std::ostream & operator << (std::ostream & os, const cJunctionRow & o)
{
  os << std::left << std::setw(11) << o.m_iJunctionId \
     << std::left << std::setw(13) << o.m_iConnectionId \
     << std::left << std::setw(17) << o.m_iConnectingRoadId \
     << std::left << std::setw(15) << o.m_iIncomingRoadId \
     << std::left << std::setw(11) << o.m_iFromLaneId \
     << std::left << std::setw(9)  << o.m_iToLaneId \
     << std::left << std::setw(13) << mapContactPointToString.at(o.m_eContactPoint) \
     << "\n";
  return os;
}
