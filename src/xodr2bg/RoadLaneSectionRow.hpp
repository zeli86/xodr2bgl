
#include "enums.hpp"
#include "types.hpp"

#pragma once

class cRoadLaneSectionRow
{
  public:
    friend std::ostream& operator<<(std::ostream&, const cRoadLaneSectionRow&);

    bool has_succ_junction() const
    {
        return (m_eSuccRoadLinkType == eLinkType::junction && _section == _no_sections - 1);
    }

    bool has_pred_junction() const
    {
        return (m_ePredRoadLinkType == eLinkType::junction && _section == 0);
    }

    bool has_succ_direct_junction() const
    {
        return (m_eSuccRoadLinkType == eLinkType::junction && _section == _no_sections - 1 && _junction == -1);
    }

    bool has_pred_direct_junction() const
    {
        return (m_ePredRoadLinkType == eLinkType::junction && _section == 0 && _junction == -1);
    }

    bool has_succ_road() const
    {
        return (m_eSuccRoadLinkType == eLinkType::road && _section == _no_sections - 1);
    }

    bool has_pred_road() const
    {
        return (m_ePredRoadLinkType == eLinkType::road); // && _section == 0);
    }

    bool has_pred_section() const
    {
        return (_section > 0 && _pred_lane_valid);
    }

    bool has_succ_section() const
    {
        return (_section < _no_sections - 1 && _succ_lane_valid);
    }

    bool has_sections() const
    {
        return (has_pred_section() || has_succ_section());
    }

    bool has_pred() const
    {
        return (m_ePredRoadLinkType != eLinkType::unknown && _section == 0);
    }

    bool has_succ() const
    {
        return (m_eSuccRoadLinkType != eLinkType::unknown && _section == _no_sections - 1);
    }

    bool in_junction() const
    {
        return (_junction >= 0);
    }

  public:
    int32_t _processed = 0;
    int32_t _id = -1;         // in [0,oo]
    int32_t _road = -1;       // in [0,oo]
    int32_t _lane = 0;        // in [-oo,oo]
    int32_t _section = 0;     // in [0,oo]
    int32_t _junction = -1;   // in [-1,oo]
    int32_t _no_sections = 1; // in [1,oo]
    double _length = 0;       // in [0,oo]

    eContactPoint m_eSuccRoadContactPt = eContactPoint::unknown;
    eLinkType m_eSuccRoadLinkType = eLinkType::unknown;
    int32_t _succ = -1; // road or junction

    eContactPoint m_ePredRoadContactPt = eContactPoint::unknown;
    eLinkType m_ePredRoadLinkType = eLinkType::unknown;
    int32_t _pred = -1; // road or junction

    bool _succ_lane_valid = false;
    int32_t _succ_lane = 0;
    bool _pred_lane_valid = false;
    int32_t _pred_lane = 0;

    std::string m_sLaneType;

    eSide m_eLaneSide = eSide::unknown;
};

std::ostream& operator<<(std::ostream&, const cRoadLaneSectionRow&);
