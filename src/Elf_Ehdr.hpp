#ifndef ELF_EHDR_HPP
#define ELF_EHDR_HPP


#include "elf_common.hpp"

/**
     * @brief Standardised header on 64-bit
     * 
     */
struct Elf_Ehdr
{
  UNSIGNED_CHAR e_ident[EI_NIDENT];
  ELF_ULONG e_type;
  ELF_ULONG e_machine;
  ELF_ULONG e_version;
  ELF_ULONG e_entry;
  ELF_ULONG e_phoff;
  ELF_ULONG e_shoff;
  ELF_ULONG e_flags;
  ELF_ULONG e_ehsize;
  ELF_ULONG e_phentsize;
  ELF_ULONG e_phnum;
  ELF_ULONG e_shentsize;
  ELF_ULONG e_shnum;
  ELF_ULONG e_shstrndx;
};

#endif /* ELF_EHDR_HPP */
