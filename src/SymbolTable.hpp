#ifndef SYMBOLTABLE_HPP
#define SYMBOLTABLE_HPP

#include "SectionTableInfo.hpp"
#include <vector>
#include <string>
#include "elf_common.hpp"
#include "Elf_Sym.hpp"



class SymbolTable : public SectionTableInfo
{
public:
  std::vector<Elf_Sym> entries;
  virtual void print(std::ostream &out) const noexcept override;
};


#endif /* SYMBOLTABLE_HPP */
