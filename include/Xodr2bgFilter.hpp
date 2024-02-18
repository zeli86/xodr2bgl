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

#include <bitset>
#include <string>
#include <type_traits>

enum class eLaneFilterFlags : unsigned char
{
   driving = 0,
   boundary = 1,
   public_transport = 2,
   civilian = 3,
   prohibited = 5,
   other = 6
};

template<class T, class = typename std::enable_if<std::is_enum<T>::value>::type>
class CFlags
{
 public:
   CFlags()
   {
      _flags.flip();
   }

   CFlags& set(const T& bit)
   {
      _flags.set(static_cast<T_under>(bit), true);
      return *this;
   }

   CFlags& unset(const T& bit)
   {
      _flags.set(static_cast<T_under>(bit), false);
      return *this;
   }

   CFlags& clear()
   {
      _flags.reset();
      return *this;
   }

   bool test(const T& bit) const
   {
      return _flags.test(static_cast<T_under>(bit));
   }

   std::string to_string()
   {
      return _flags.to_string();
   }
 private:
   using T_under = typename std::underlying_type<T>::type;
   std::bitset<8*sizeof(T_under)> _flags;
};

using CLaneTypeFilter = CFlags<eLaneFilterFlags>;