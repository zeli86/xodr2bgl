/*
 * Copyright 2024 Želimir Marojević
 *
 * Distributed under the Boost Software License, Version 1.0.
 * (See accompanying file LICENSE_1_0.txt
 * or copy at http://www.boost.org/LICENSE_1_0.txt)
 *
 * For more information, see http://www.boost.org
 */

#include "JunctionRow.hpp"

std::ostream& operator<<(std::ostream& os, const cJunctionRow& o)
{
    os << printer::leftcol(11) << o._junction
       << printer::leftcol(13) << o._connection
       << printer::leftcol(17) << o._connecting_road
       << printer::leftcol(15) << o._incoming_road
       << printer::leftcol(15) << o._linked_road
       << printer::leftcol(11) << o._from_lane
       << printer::leftcol(9) << o._to_lane
       << printer::leftcol(13) << mapContactPointToString.at(o.m_eContactPoint)
       << "\n";
    return os;
}
