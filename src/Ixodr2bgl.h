/*
 * Copyright 2019 Želimir Marojević
 *
 * Distributed under the Boost Software License, Version 1.0.
 * (See accompanying file LICENSE_1_0.txt
 * or copy at http://www.boost.org/LICENSE_1_0.txt)
 *
 * For more information, see http://www.boost.org
 */

#pragma once 

#include <iostream>
#include <string>
#include <map>
#include <memory>

#if defined BUILDING_SHARED || defined BUILDING_STATIC
  #if defined BUILDING_SHARED
    #if defined _WIN32 || defined __CYGWIN__
      #ifdef BUILDING_SHARED
        #ifdef __GNUC__
          #define DLL_PUBLIC __attribute__((dllexport))
        #else
          #define DLL_PUBLIC __declspec(dllexport)
        #endif
      #else
        #ifdef __GNUC__
          #define DLL_PUBLIC __attribute__((dllimport))
        #else
          #define DLL_PUBLIC __declspec(dllimport)
        #endif
      #endif
      #define DLL_LOCAL
    #else
      #if __GNUC__ >= 4
        #define DLL_PUBLIC __attribute__((visibility("default")))
        #define DLL_LOCAL  __attribute__((visibility("hidden")))
      #else
        #define DLL_PUBLIC
        #define DLL_LOCAL
      #endif
    #endif
  #endif
  #if defined BUILDING_STATIC
    #define DLL_PUBLIC
    #define DLL_LOCAL
  #endif
#else
  #define DLL_PUBLIC
  #define DLL_LOCAL
#endif

class Ixodr2bgl;

class Ixodr2bglFactory
{
public:
	Ixodr2bglFactory() {};
	virtual std::unique_ptr<Ixodr2bgl> create() = 0;
};

class Ixodr2bgl
{
public:
  DLL_PUBLIC Ixodr2bgl() = default;

  DLL_PUBLIC virtual ~Ixodr2bgl() = default;

  DLL_PUBLIC virtual bool init(const std::string &) = 0;

  DLL_PUBLIC virtual void dump(std::ostream&) = 0;

	DLL_PUBLIC static void registerType(int32_t, Ixodr2bglFactory*);
  DLL_PUBLIC static std::unique_ptr<Ixodr2bgl> create(int32_t);

	DLL_PUBLIC virtual void setID(int32_t) = 0;

protected:
	int32_t m_iID = 0;	
  static std::map<int32_t, Ixodr2bglFactory *> & getFactoryInstance();
};
