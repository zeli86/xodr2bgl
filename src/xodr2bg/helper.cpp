
#include "helper.hpp"
#include <iomanip>

namespace printer
{

  std::ostream& operator<<(std::ostream& os, const leftcol& o)
  {
    os << std::left << std::setw(o._w);
    return os;
  }

}