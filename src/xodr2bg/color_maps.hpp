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

#include <string>

#include <boost/assign.hpp>
#include <boost/bimap.hpp>

using string_string_bimap_t = boost::bimap<std::string, std::string>;

static const string_string_bimap_t lane_type_color_bimap = boost::assign::list_of<string_string_bimap_t::relation>
   ("biking", "gold")
   ("border", "darkgreen")
   ("bus", "peru")
   ("connectingRamp", "")
   ("curb", "darkgreen")
   ("driving", "indigo")
   ("entry", "indigo")
   ("exit", "indigo")
   ("median", "indigo")
   ("mwyEntry", "indigo")
   ("mwyExit", "indigo")
   ("none", "black")
   ("offRamp", "indigo")
   ("onRamp", "indigo")
   ("parking", "dodgerblue")
   ("rail", "peru")
   ("restricted", "orangered")
   ("roadWorks", "yellow")
   ("shared", "indigo")
   ("shoulder", "green")
   ("sidewalk", "green")
   ("slipLane", "yellow")
   ("special1", "yellow")
   ("special2", "yellow")
   ("special3", "yellow")
   ("stop", "yellow")
   ("taxi", "yellow")
   ("tram", "peru")
   ("walking", "lightseagreen");

static std::string get_lane_type_color(const std::string &type)
{
   auto it = lane_type_color_bimap.left.find(type);
   if ( it != lane_type_color_bimap.left.end())
   {
      return it->second;
   }
   return "black";
}
