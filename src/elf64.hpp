#ifndef ELF64_HPP
#define ELF64_HPP

#include <cstdint>
#include "elf_common.hpp"

using Elf64_Addr = uint64_t;
using Elf64_Off = uint64_t;
using Elf64_Half = uint16_t;
using Elf64_Word = uint32_t;
using Elf64_Sword = uint32_t;
using Elf64_Xword = uint64_t;
using Elf64_Sxword = uint64_t;

struct Elf64_Ehdr
{
  UNSIGNED_CHAR e_ident[EI_NIDENT];
  Elf64_Half e_type;
  Elf64_Half e_machine;
  Elf64_Word e_version;
  Elf64_Addr e_entry;
  Elf64_Off e_phoff;
  Elf64_Off e_shoff;
  Elf64_Word e_flags;
  Elf64_Half e_ehsize;
  Elf64_Half e_phentsize;
  Elf64_Half e_phnum;
  Elf64_Half e_shentsize;
  Elf64_Half e_shnum;
  Elf64_Half e_shstrndx;
};


#define ELF64_ST_BIND(i) ((i) >> 4)
#define ELF64_ST_TYPE(i) ((i)&0xf)
#define ELF64_ST_INFO(b, t) (((b) << 4) + ((t)&0xf))


struct Elf64_Sym
{
  Elf64_Word st_name;
  Elf64_Xword st_size;
  unsigned char st_info;
  unsigned char st_other;
  Elf64_Addr st_value;
  Elf64_Half st_shndx;
};

/**
 * Relocation Structures
 * 
 */
#define ELF64_R_SYM(i) ((i) >> 32)
#define ELF64_R_TYPE(i) ((i)&0xffffffffL)
#define ELF64_R_INFO(s, t) (((s) << 32) + ((t)&0xffffffffL))




struct Elf64_Shdr
{
  Elf64_Word sh_name;
  Elf64_Word sh_link;
  Elf64_Word sh_info;
  Elf64_Xword sh_addralign;
  Elf64_Xword sh_entsize;
};

struct Elf64_Rel
{
  Elf64_Addr r_offset;
  Elf64_Xword r_info;
};

struct Elf64_Rela
{
  Elf64_Addr r_offset;
  Elf64_Xword r_info;
  Elf64_Sxword r_addend;
};


struct Elf64_Phdr
{
  Elf64_Word p_type;
  Elf64_Word p_flags;// NOTE THIS IS DIFFERENT
  Elf64_Off p_offset;
  Elf64_Addr p_vaddr;
  Elf64_Addr p_paddr;
  Elf64_Xword p_filesz;
  Elf64_Xword p_memsz;
  Elf64_Xword p_align;
};


#endif /* ELF64_HPP */
