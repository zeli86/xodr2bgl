
#include "stdafx.h"
#include "enums.h"
#include "types.h"

#pragma once

class cRoadLaneSectionRow
{
  public:
    cRoadLaneSectionRow() = default;
    ~cRoadLaneSectionRow() = default;

    friend std::ostream & operator << (std::ostream &, const cRoadLaneSectionRow &);

    bool bIsSuccJunction() const
    {
      return (m_eSuccRoadLinkType == eLinkType::junction);
    }

    bool bIsPredJunction() const
    {
      return (m_ePredRoadLinkType == eLinkType::junction);
    }

    bool bIsSuccRoad() const
    {
      return (m_eSuccRoadLinkType == eLinkType::road);
    }

    bool bIsPredRoad() const
    {
      return (m_ePredRoadLinkType == eLinkType::road);
    }

  public:

    int32_t m_iRowId = -1;
    int32_t m_iRoadId = -1;
    int32_t m_iLaneId = 0;
    int32_t m_iJunctionId = -1;
    int32_t m_iNoOfLaneSections = 0;
    double m_rRoadLength = 0;

    std::vector<double> m_vLaneSectionStarts;

    bool          m_bSuccRoadIdValid = false;
    eContactPoint m_eSuccRoadContactPt = eContactPoint::unknown;
    eLinkType     m_eSuccRoadLinkType = eLinkType::unknown;
    int64_t       m_iSuccRoadId = -1;

    bool          m_bPredRoadIdValid = false;
    eContactPoint m_ePredRoadContactPt = eContactPoint::unknown;
    eLinkType     m_ePredRoadLinkType = eLinkType::unknown;
    int64_t       m_iPredRoadId = -1;

    bool m_bSuccLaneIdValid = false;
    int32_t m_iSuccLaneId = 0;
    bool m_bPredLaneIdValid = false;
    int32_t m_iPredLaneId = 0;

    std::string m_sLaneType;

    eSide m_eLaneSide = eSide::unknown;

    RoadNetVertex m_pRoadNetVertex1 = nullptr;
    RoadNetVertex m_pRoadNetVertex2 = nullptr;
    RoadNetEdge   * m_pRoadNetEdge = nullptr;
};

std::ostream & operator << (std::ostream &out, const cRoadLaneSectionRow &c);
