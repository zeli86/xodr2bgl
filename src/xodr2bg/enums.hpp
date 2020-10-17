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

#include <cstdint>
#include <map>
#include <string>

enum class eContactPoint : int32_t
{
    unknown,
    start,
    end
};

static std::map<eContactPoint, std::string> mapContactPointToString = {
    {eContactPoint::unknown, "unkown"},
    {eContactPoint::start, "start"},
    {eContactPoint::end, "end"}};

enum class eLinkType : int32_t
{
    unknown,
    road,
    junction
};

static std::map<eLinkType, std::string> mapLinkTypeToString = {
    {eLinkType::unknown, "unkown"},
    {eLinkType::road, "road"},
    {eLinkType::junction, "junction"}};

enum class eSide : int32_t
{
    unknown,
    left,
    right
};

static std::map<eSide, std::string> mapSideToString = {
    {eSide::unknown, "unkown"},
    {eSide::left, "left"},
    {eSide::right, "right"}};
