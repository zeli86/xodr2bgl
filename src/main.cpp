/*
 * Copyright 2019 Želimir Marojević
 *
 * Distributed under the Boost Software License, Version 1.0.
 * (See accompanying file LICENSE_1_0.txt
 * or copy at http://www.boost.org/LICENSE_1_0.txt)
 *
 * For more information, see http://www.boost.org
 */

#include <iostream>
#include <fstream>
#include <string>

#include "Ixodr2bgl.h"

int main(int argc, char *argv[])
{
   std::unique_ptr<Ixodr2bgl> pInstance = std::move(Ixodr2bgl::create(1));

   //pInstance->init("TCross_wLaneSections.xodr");
   //pInstance->init("Motorway.xodr");

return EXIT_SUCCESS;
}
