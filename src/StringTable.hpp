#ifndef STRINGTABLE_HPP
#define STRINGTABLE_HPP

#include "SectionTableInfo.hpp"
#include <vector>

class StringTable : public SectionTableInfo
{
public:
  std::vector<std::string> entries;

  virtual void print(std::ostream &out) const noexcept override;
};


#endif /* STRINGTABLE_HPP */
