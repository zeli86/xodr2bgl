/*
 * Copyright 2019 Želimir Marojević
 *
 * Distributed under the Boost Software License, Version 1.0.
 * (See accompanying file LICENSE_1_0.txt
 * or copy at http://www.boost.org/LICENSE_1_0.txt)
 *
 * For more information, see http://www.boost.org
 */

#include "Ixodr2bgl.h"

void Ixodr2bgl::registerType(int32_t iID, Ixodr2bglFactory *pFactory) 
{
  getFactoryInstance()[iID] = pFactory;
}

std::unique_ptr<Ixodr2bgl> Ixodr2bgl::create(int32_t iID) 
{
  std::unique_ptr<Ixodr2bgl> pInstance = std::move(getFactoryInstance()[iID]->create());
  return pInstance;
}

std::map<int32_t, Ixodr2bglFactory *> & Ixodr2bgl::getFactoryInstance()
{
  static std::map<int32_t, Ixodr2bglFactory*> mInstance;
return mInstance;
}