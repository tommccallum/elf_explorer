#ifndef ELF_SYM_HPP
#define ELF_SYM_HPP

#include "elf_common.hpp"

struct elf_symbol_table_fields_t
{
  size_t index;
  ELF_ULONG sz[3];
};


struct Elf_Sym
{
  ELF_ULONG st_name;
  ELF_ULONG st_value;
  ELF_ULONG st_size;
  unsigned char st_info;
  unsigned char st_other;
  ELF_ULONG st_shndx;
  std::string name;
  ELF_ULONG file_type;

  bool is_in_symtab_shndx() const;
};

std::ostream &operator<<(std::ostream &out, Elf_Sym const &header);

#endif /* ELF_SYM_HPP */
