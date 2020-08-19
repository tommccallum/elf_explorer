#include "SectionTableInfo.hpp"

void SectionTableInfo::print(std::ostream &out) const noexcept
{
  out << "  ** Empty section table detail **";
}

std::ostream &operator<<(std::ostream &out, SectionTableInfo const &section)
{
  section.print(out);
  return out;
}
