#ifndef SECTIONTABLEINFO_HPP
#define SECTIONTABLEINFO_HPP

#include <iostream>

class SectionTableInfo
{
public:
  virtual void print(std::ostream &out) const noexcept;
  virtual ~SectionTableInfo() = default;
};

std::ostream &operator<<(std::ostream &out, SectionTableInfo const &section);


#endif /* SECTIONTABLEINFO_HPP */
