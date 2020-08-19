#include "StringTable.hpp"


void StringTable::print(std::ostream &out) const noexcept
{
  out << "\n   [StringTable] Entries: " << entries.size() << std::endl;
  for (auto entry : entries) {
    out << "     " << entry << std::endl;
  }
}
