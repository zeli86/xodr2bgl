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

#include "Xodr2bgFilter.hpp"

//#include "xodr_road_graph_export.h"

#include <iostream>
#include <memory>
#include <ostream>

class CXodrRoadGraphImpl;

class CXodrRoadGraph
{
 public:
   CXodrRoadGraph();
   ~CXodrRoadGraph();

   bool init(const std::string&, const bool=true);
   void dump(std::ostream&);
   void to_txt(std::ostream&);
   void to_graphviz(const std::string&);
   void set_lane_type_filter(const CLaneTypeFilter&);
   void set_log_level(const unsigned=7);
   void dfs(const int32_t, const int32_t, const int32_t, const bool=true);

 private:
   std::unique_ptr<CXodrRoadGraphImpl> _pimpl;
};
