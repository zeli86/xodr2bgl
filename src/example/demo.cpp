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

#include <cstdlib>
#include <iostream>
#include <string>
#include <filesystem>

// void run(const std::string& xodr_file)
// {
//    std::string dot_file = xodr_file;
//    dot_file.erase(dot_file.end()-5,dot_file.end());
//    dot_file += ".dot";

//    CLaneTypeFilter filter;
//    filter.clear().set(eLaneFilterFlags::driving);

//    CXodrRoadGraph oMyGraph1;
//    oMyGraph1.set_lane_type_filter(filter);
//    oMyGraph1.init(xodr_file);
//    oMyGraph1.to_graphviz(dot_file);
// }

// int main(int argc, char* argv[])
// {
//    if (argc != 2)
//    {
//       return EXIT_FAILURE;
//    }
//    std::string xodr_file = argv[1];
//    run(xodr_file);
//    return EXIT_SUCCESS;
// }

void run(const std::filesystem::path& xodr_file)
{
   std::filesystem::path dot_file = xodr_file;
   dot_file.replace_extension(".dot");

   CLaneTypeFilter filter;
   //filter.clear().set(eLaneFilterFlags::driving);

   CXodrRoadGraph oMyGraph1;
   oMyGraph1.set_log_level(0);
   oMyGraph1.set_lane_type_filter(filter);
   oMyGraph1.init(xodr_file.native(), true);
   oMyGraph1.to_graphviz(dot_file.native());
   //oMyGraph1.dfs(1,-1,0);
}

int main(int argc, char* argv[])
{
   if (argc != 2)
   {
      return EXIT_FAILURE;
   }
   std::filesystem::path xodr_file = argv[1];
   run(xodr_file);
   return EXIT_SUCCESS;
}
