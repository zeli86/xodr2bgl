/*
 * Copyright 2024 Želimir Marojević
 *
 * Distributed under the Boost Software License, Version 1.0.
 * (See accompanying file LICENSE_1_0.txt
 * or copy at http://www.boost.org/LICENSE_1_0.txt)
 *
 * For more information, see http://www.boost.org
 */

#include <catch2/catch_test_macros.hpp>
#include <catch2/matchers/catch_matchers.hpp>
#include <catch2/matchers/catch_matchers_floating_point.hpp>

#include "RoadLaneSectionRow.hpp"

TEST_CASE("road_connectivity_checks", "[road][lane][sections]")
{
    SECTION("default constructor")
    {
        cRoadLaneSectionRow row;

        CHECK_FALSE(row.has_pred());
        CHECK_FALSE(row.has_succ());
        CHECK_FALSE(row.has_pred_road());
        CHECK_FALSE(row.has_succ_road());
        CHECK_FALSE(row.has_pred_junction());
        CHECK_FALSE(row.has_succ_junction());
        CHECK_FALSE(row.has_sections());
        CHECK_FALSE(row.has_pred_section());
        CHECK_FALSE(row.has_succ_section());

        INFO(row);
    }

    SECTION("pred road")
    {
        cRoadLaneSectionRow row;
        row._lane = 1;
        row.m_ePredRoadLinkType = eLinkType::road;

        CHECK(row.has_pred());
        CHECK_FALSE(row.has_succ());
        CHECK(row.has_pred_road());
        CHECK_FALSE(row.has_succ_road());
        CHECK_FALSE(row.has_pred_junction());
        CHECK_FALSE(row.has_succ_junction());
        CHECK_FALSE(row.has_sections());
        CHECK_FALSE(row.has_pred_section());
        CHECK_FALSE(row.has_succ_section());

        INFO(row);
    }

    SECTION("succ road")
    {
        cRoadLaneSectionRow row;
        row._lane = 1;
        row.m_eSuccRoadLinkType = eLinkType::road;

        CHECK_FALSE(row.has_pred());
        CHECK(row.has_succ());
        CHECK_FALSE(row.has_pred_road());
        CHECK(row.has_succ_road());
        CHECK_FALSE(row.has_pred_junction());
        CHECK_FALSE(row.has_succ_junction());
        CHECK_FALSE(row.has_sections());
        CHECK_FALSE(row.has_pred_section());
        CHECK_FALSE(row.has_succ_section());

        INFO(row);
    }

    SECTION("pred junction")
    {
        cRoadLaneSectionRow row;
        row._lane = 1;
        row.m_ePredRoadLinkType = eLinkType::junction;

        CHECK(row.has_pred());
        CHECK_FALSE(row.has_succ());
        CHECK_FALSE(row.has_pred_road());
        CHECK_FALSE(row.has_succ_road());
        CHECK(row.has_pred_junction());
        CHECK_FALSE(row.has_succ_junction());
        CHECK_FALSE(row.has_sections());
        CHECK_FALSE(row.has_pred_section());
        CHECK_FALSE(row.has_succ_section());

        INFO(row);
    }

    SECTION("pred junction")
    {
        cRoadLaneSectionRow row;
        row._lane = 1;
        row.m_eSuccRoadLinkType = eLinkType::junction;

        CHECK_FALSE(row.has_pred());
        CHECK(row.has_succ());
        CHECK_FALSE(row.has_pred_road());
        CHECK_FALSE(row.has_succ_road());
        CHECK_FALSE(row.has_pred_junction());
        CHECK(row.has_succ_junction());
        CHECK_FALSE(row.has_sections());
        CHECK_FALSE(row.has_pred_section());
        CHECK_FALSE(row.has_succ_section());

        INFO(row);
    }

    SECTION("road two lane sections, first lane section")
    {
        cRoadLaneSectionRow row;
        row._road = 1;
        row._lane = 1;
        row._section = 0;
        row._no_sections = 2;
        row._succ_lane_valid = true;
        row._succ_lane = 1;

        CHECK_FALSE(row.has_pred());
        CHECK_FALSE(row.has_succ());
        CHECK_FALSE(row.has_pred_road());
        CHECK_FALSE(row.has_succ_road());
        CHECK_FALSE(row.has_pred_junction());
        CHECK_FALSE(row.has_succ_junction());
        CHECK(row.has_sections());
        CHECK_FALSE(row.has_pred_section());
        CHECK(row.has_succ_section());

        INFO(row);
    }

    SECTION("road two lane sections, second lane section")
    {
        cRoadLaneSectionRow row;
        row._road = 1;
        row._lane = 1;
        row._section = 1;
        row._no_sections = 2;
        row._pred_lane_valid = true;
        row._pred_lane = 1;

        CHECK_FALSE(row.has_pred());
        CHECK_FALSE(row.has_succ());
        CHECK_FALSE(row.has_pred_road());
        CHECK_FALSE(row.has_succ_road());
        CHECK_FALSE(row.has_pred_junction());
        CHECK_FALSE(row.has_succ_junction());
        CHECK(row.has_sections());
        CHECK(row.has_pred_section());
        CHECK_FALSE(row.has_succ_section());

        INFO(row);
    }

    SECTION("road three lane sections, first lane section")
    {
        cRoadLaneSectionRow row;
        row._road = 1;
        row._lane = 1;
        row._section = 0;
        row._no_sections = 3;
        row._succ_lane_valid = true;
        row._succ_lane = 1;

        CHECK_FALSE(row.has_pred());
        CHECK_FALSE(row.has_succ());
        CHECK_FALSE(row.has_pred_road());
        CHECK_FALSE(row.has_succ_road());
        CHECK_FALSE(row.has_pred_junction());
        CHECK_FALSE(row.has_succ_junction());
        CHECK(row.has_sections());
        CHECK_FALSE(row.has_pred_section());
        CHECK(row.has_succ_section());

        INFO(row);
    }

    SECTION("road three lane sections, second lane section")
    {
        cRoadLaneSectionRow row;
        row._road = 1;
        row._lane = 1;
        row._section = 1;
        row._no_sections = 3;
        row._pred_lane_valid = true;
        row._pred_lane = 1;
        row._succ_lane_valid = true;
        row._succ_lane = 1;

        CHECK_FALSE(row.has_pred());
        CHECK_FALSE(row.has_succ());
        CHECK_FALSE(row.has_pred_road());
        CHECK_FALSE(row.has_succ_road());
        CHECK_FALSE(row.has_pred_junction());
        CHECK_FALSE(row.has_succ_junction());
        CHECK(row.has_sections());
        CHECK(row.has_pred_section());
        CHECK(row.has_succ_section());

        INFO(row);
    }

    SECTION("road three lane sections, second lane section")
    {
        cRoadLaneSectionRow row;
        row._road = 1;
        row._lane = 1;
        row._section = 2;
        row._no_sections = 3;
        row._pred_lane_valid = true;
        row._pred_lane = 1;

        CHECK_FALSE(row.has_pred());
        CHECK_FALSE(row.has_succ());
        CHECK_FALSE(row.has_pred_road());
        CHECK_FALSE(row.has_succ_road());
        CHECK_FALSE(row.has_pred_junction());
        CHECK_FALSE(row.has_succ_junction());
        CHECK(row.has_sections());
        CHECK(row.has_pred_section());
        CHECK_FALSE(row.has_succ_section());

        INFO(row);
    }
}
