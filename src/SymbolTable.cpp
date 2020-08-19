#include "SymbolTable.hpp"

void SymbolTable::print(std::ostream &out) const noexcept
{
  out << "\n   [SymbolTable] Entries: " << entries.size() << std::endl;
  for (auto entry : entries) {
    out << "     " << entry << std::endl;
  }
}
