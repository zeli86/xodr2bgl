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
#include "enums.h"

#pragma once

class cJunctionRow
{
  public:
    cJunctionRow() = default;
    ~cJunctionRow() = default;

    friend std::ostream & operator << (std::ostream &, const cJunctionRow &);

    int32_t m_iJunctionId = -1;
    int32_t m_iConnectionId = -1;
    int32_t m_iConnectingRoadId = -1;
    int32_t m_iIncomingRoadId = -1;
    int32_t m_iFromLaneId = 0;
    int32_t m_iToLaneId = 0;
    eContactPoint m_eContactPoint = eContactPoint::unknown;

  protected:
    char m_iSeparator = '\t';
};

std::ostream & operator << (std::ostream &, const cJunctionRow &);

