/*
 * Copyright 2024 Želimir Marojević
 *
 * Distributed under the Boost Software License, Version 1.0.
 * (See accompanying file LICENSE_1_0.txt
 * or copy at http://www.boost.org/LICENSE_1_0.txt)
 *
 * For more information, see http://www.boost.org
 */

#include <cstdint>
#include <iostream>
#include <ostream>
#include <sstream>
#include <utility>

namespace
{
  template<typename T>
  void print2 (std::ostream& out, T arg)
  {
    out << ", " <<  arg;
  }

  template<typename T, typename... Types>
  void print2 (std::ostream& out, T firstArg, Types... args)
  {
    print2(out, firstArg);
    print2(out, args...);
  }
}

namespace printer
{
  template<typename T, typename... Types>
  std::ostream& tuple(std::ostream& out, T firstArg, Types... args)
  {
    out << "(" << firstArg;
    print2(out, args...);
    out << ")";
    return out;
  }

  template<typename T>
  std::ostream& tuple(std::ostream& out, T firstArg)
  {
    out << "(" << firstArg << ")";
    return out;
  }

  template<typename T, typename... Types>
  std::string tuple(T firstArg, Types... args)
  {
    std::stringstream result;
    result << "(" << firstArg;
    print2(result, args...);
    result << ")";
    return result.str();
  }

  template<typename... Types>
  class tupleX
  {
   public:
    tupleX(Types& ... args)
    {
      _str = tuple(std::forward<Types>(args)...);
    };

    template<typename... U>
    friend std::ostream& operator<<(std::ostream&, const tupleX<U...>&);
   private:
    std::string _str;
  };

  template<typename... U>
  std::ostream& operator<<(std::ostream& os, const tupleX<U...>& in)
  {
    os << in._str;
    return os;
  }

  class leftcol
  {
   public:
    leftcol(const int32_t w) : _w(w) {};

    friend std::ostream& operator<<(std::ostream&, const leftcol&);
   private:
    int32_t _w;
  };

  std::ostream& operator<<(std::ostream&, const leftcol&);
}