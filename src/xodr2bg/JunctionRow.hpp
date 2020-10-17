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

#include "enums.hpp"
#include "helper.hpp"

class cJunctionRow
{
  public:
    friend std::ostream& operator<<(std::ostream&, const cJunctionRow&);

    int32_t _junction = -1;
    int32_t _connection = -1;
    int32_t _connecting_road = -1;
    int32_t _incoming_road = -1;
    int32_t _linked_road = -1; // since OpenDRIVE 1.7
    int32_t _from_lane = 0;
    int32_t _to_lane = 0;
    eContactPoint m_eContactPoint = eContactPoint::unknown;
};

std::ostream& operator<<(std::ostream&, const cJunctionRow&);
