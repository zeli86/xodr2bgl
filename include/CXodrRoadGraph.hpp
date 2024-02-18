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

#include <iostream>
#include <memory>
#include <ostream>

// #if defined BUILDING_SHARED || defined BUILDING_STATIC
//   #if defined BUILDING_SHARED
//     #if defined _WIN32 || defined __CYGWIN__
//       #ifdef BUILDING_SHARED
//         #ifdef __GNUC__
//           #define DLL_PUBLIC __attribute__((dllexport))
//         #else
//           #define DLL_PUBLIC __declspec(dllexport)
//         #endif
//       #else
//         #ifdef __GNUC__
//           #define DLL_PUBLIC __attribute__((dllimport))
//         #else
//           #define DLL_PUBLIC __declspec(dllimport)
//         #endif
//       #endif
//       #define DLL_LOCAL
//     #else
//       #if __GNUC__ >= 4
//         #define DLL_PUBLIC __attribute__((visibility("default")))
//         #define DLL_LOCAL  __attribute__((visibility("hidden")))
//       #else
//         #define DLL_PUBLIC
//         #define DLL_LOCAL
//       #endif
//     #endif
//   #endif
//   #if defined BUILDING_STATIC
//     #define DLL_PUBLIC
//     #define DLL_LOCAL
//   #endif
// #else
//   #define DLL_PUBLIC
//   #define DLL_LOCAL
// #endif

class CXodrRoadGraphImpl;

class CXodrRoadGraph
{
 public:
   CXodrRoadGraph();
   ~CXodrRoadGraph();

   bool init(const std::string&);
   void dump(std::ostream&);
   void to_txt(std::ostream&);
   void to_graphviz(const std::string&);
   void set_lane_type_filter(const CLaneTypeFilter&);
   void set_log_level(const unsigned=7);

 private:
   std::unique_ptr<CXodrRoadGraphImpl> _pimpl;
};
