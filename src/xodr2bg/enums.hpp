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
#include <string>

#include <boost/assign.hpp>
#include <boost/bimap.hpp>

enum class eContactPoint : int32_t
{
    unknown,
    start,
    end
};

using contact_point_bimap_t = boost::bimap<eContactPoint, std::string>;

static const contact_point_bimap_t contact_point_bimap = boost::assign::list_of<contact_point_bimap_t::relation>
(eContactPoint::unknown, "unknown")
(eContactPoint::start, "start")
(eContactPoint::end, "end" );

enum class eLinkType : int32_t
{
    unknown,
    road,
    junction
};

using link_type_bimap_t = boost::bimap<eLinkType, std::string>;

static const link_type_bimap_t link_type_bimap = boost::assign::list_of<link_type_bimap_t::relation>
(eLinkType::unknown, "unknown")
(eLinkType::road, "road")
(eLinkType::junction, "junction" );

enum class eSide : int32_t
{
    unknown,
    left,
    right
};

using side_bimap_t = boost::bimap<eSide, std::string>;

static const side_bimap_t side_bimap = boost::assign::list_of<side_bimap_t::relation>
(eSide::unknown, "unknown")
(eSide::left, "left")
(eSide::right, "right" );

enum class eJunctionType : int32_t
{
    normal, // default
    direct, // since 1.7.0
    unreal, // virtual
    crossing // since 1.8.0
};

using junction_type_bimap_t = boost::bimap<eJunctionType, std::string>;

static const junction_type_bimap_t junction_type_bimap = boost::assign::list_of<junction_type_bimap_t::relation>
(eJunctionType::normal, "default")
(eJunctionType::direct, "direct")
(eJunctionType::unreal, "virtual")
(eJunctionType::crossing, "crossing");
