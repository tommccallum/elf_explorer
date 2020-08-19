#ifndef ELF_PHDR_HPP
#define ELF_PHDR_HPP

#include "elf.hpp"

struct Elf_Phdr
{
  ELF_ULONG p_type;
  ELF_ULONG p_offset;
  ELF_ULONG p_vaddr;
  ELF_ULONG p_paddr;
  ELF_ULONG p_filesz;
  ELF_ULONG p_memsz;
  ELF_ULONG p_flags;
  ELF_ULONG p_align;

  friend std::ostream &operator<<(std::ostream &out, Elf_Phdr const &header);
};

std::ostream &operator<<(std::ostream &out, Elf_Phdr const &header);


#endif /* ELF_PHDR_HPP */
