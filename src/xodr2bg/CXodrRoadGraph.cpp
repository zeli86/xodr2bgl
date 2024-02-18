/*
 * Copyright 2024 Želimir Marojević
 *
 * Distributed under the Boost Software License, Version 1.0.
 * (See accompanying file LICENSE_1_0.txt
 * or copy at http://www.boost.org/LICENSE_1_0.txt)
 *
 * For more information, see http://www.boost.org
 */

#include "CXodrRoadGraph.hpp"
#include "CXodrRoadGraphImpl.hpp"
#include <ostream>

CXodrRoadGraph::CXodrRoadGraph() : _pimpl(new CXodrRoadGraphImpl)
{
}

CXodrRoadGraph::~CXodrRoadGraph() = default;

bool CXodrRoadGraph::init(const std::string& name)
{
   return _pimpl->init(name);
}

void CXodrRoadGraph::dump(std::ostream& os)
{
   _pimpl->dump(os);
}

void CXodrRoadGraph::to_graphviz(const std::string& filepath)
{
   _pimpl->to_graphviz(filepath);
}

void CXodrRoadGraph::to_txt(std::ostream& os)
{
   _pimpl->to_txt(os);
}

void CXodrRoadGraph::set_lane_type_filter(const CLaneTypeFilter& filter)
{
   _pimpl->set_lane_type_filter(filter);
}

void CXodrRoadGraph::set_log_level(const unsigned lvl)
{
   _pimpl->set_log_level(lvl);
}