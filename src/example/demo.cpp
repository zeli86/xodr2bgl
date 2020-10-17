/*
 * Copyright 2024 Želimir Marojević
 *
 * Distributed under the Boost Software License, Version 1.0.
 * (See accompanying file LICENSE_1_0.txt
 * or copy at http://www.boost.org/LICENSE_1_0.txt)
 *
 * For more information, see http://www.boost.org
 */

#include <cstdlib>
#include <iostream>
#include <string>

#include "CXodrRoadGraph.hpp"

void run( const std::string& base_name )
{
    CXodrRoadGraph oMyGraph1;
    std::string tmp = "/home/zeli/XODR/" + base_name + ".xodr";
    oMyGraph1.init(tmp);
    tmp = base_name + ".dot";
    oMyGraph1.to_graphviz(tmp);
    tmp = "dot -Tpng " + base_name + ".dot > " + base_name + ".png";
    std::system(tmp.c_str());
}

int main(int argc, char* argv[])
{
    run("3_arm_junction");
    run("full_junction0");

    return EXIT_SUCCESS;
}
