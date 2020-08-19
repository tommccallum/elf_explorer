#ifndef ELF32_HPP
#define ELF32_HPP

#include <cstdint>
#include "elf_common.hpp"

using Elf32_Addr = uint32_t;
using Elf32_Off = uint32_t;
using Elf32_Half = uint16_t;
using Elf32_Word = uint32_t;
using Elf32_Sword = uint32_t;

struct Elf32_Ehdr
{
  UNSIGNED_CHAR e_ident[EI_NIDENT];
  Elf32_Half e_type;
  Elf32_Half e_machine;
  Elf32_Word e_version;// version of file format
  Elf32_Addr e_entry;// virtual address to start program, 0 otherwise
  Elf32_Off e_phoff;// program header table offset, 0 if none
  Elf32_Off e_shoff;// section header table offset, 0 if none
  Elf32_Word e_flags;// processor specific flags
  Elf32_Half e_ehsize;// elf headers size in bytes
  Elf32_Half e_phentsize;// sizeof(program header table entry)
  Elf32_Half e_phnum;// number of program headers in program header table
  Elf32_Half e_shentsize;// sizeof(section header table entry)
  Elf32_Half e_shnum;// number of section header entries
  Elf32_Half e_shstrndx;// index into the section header entry associated with
    // the section name, SHN_UNDEF if no section name
};


// NOTE:
// 1 section has 1 section header
// 1 section header may have 0 sections
// sections do NOT overlap
// object file may have unused space and sections may not cover every byte in an object file

struct Elf32_Shdr
{
  Elf32_Word sh_name;// section header name, it is an index into the section header string table
  Elf32_Word sh_type;// section contents type
  Elf32_Word sh_flags;// bit flags
  Elf32_Addr sh_addr;// if used in memory image of process, this gives the address of where it should reside
  Elf32_Off sh_offset;// offset of entry from start of file
  Elf32_Word sh_size;// size of section in bytes
  Elf32_Word sh_link;// section header table index link
  Elf32_Word sh_info;// extra info depending on section type
  Elf32_Word sh_addralign;// aalignment constraints
  Elf32_Word sh_entsize;// some sections are fixed-size entries, this value is the size of each entry, 0 otherwise
};

/**
 * @brief Use for compressed data sections
 * 
 */
struct Elf32_Chdr
{
  Elf32_Word ch_type;
  Elf32_Word ch_size;
  Elf32_Word ch_addralign;
};

// typedef struct {
// 	Elf64_Word	ch_type;
// 	Elf64_Word	ch_reserved;
// 	Elf64_Xword	ch_size;
// 	Elf64_Xword	ch_addralign;
// } Elf64_Chdr;

struct special_sections_32_t
{
  std::string name;
  ELF_ULONG type;
  ELF_ULONG attributes;
};

#define ELF32_ST_BIND(i) ((i) >> 4)
#define ELF32_ST_TYPE(i) ((i)&0xf)
#define ELF32_ST_INFO(b, t) (((b) << 4) + ((t)&0xf))


struct Elf32_Sym
{
  Elf32_Word st_name;
  Elf32_Addr st_value;
  Elf32_Word st_size;
  unsigned char st_info;
  unsigned char st_other;
  Elf32_Half st_shndx;
};


/**
 * Relocation Structures
 * 
 */

#define ELF32_R_SYM(i) ((i) >> 8)
#define ELF32_R_TYPE(i) ((unsigned char)(i))
#define ELF32_R_INFO(s, t) (((s) << 8) + (unsigned char)(t))


struct Elf32_Rel
{
  // the location at which to apply the relocation action,
  // offset from section start to the virtual address of the storage unit
  Elf32_Addr r_offset;
  // gives both the symbol table index and type of relocation to do
  // relocation types are cpu-specific
  Elf32_Word r_info;
};

struct Elf32_Rela
{
  Elf32_Addr r_offset;
  Elf32_Word r_info;
  // specifies a constant added used to compute the value to be stored into the relocatable field.
  Elf32_Sword r_addend;
};


struct Elf32_Phdr
{
  Elf32_Word p_type;
  Elf32_Off p_offset;
  Elf32_Addr p_vaddr;
  Elf32_Addr p_paddr;
  Elf32_Word p_filesz;
  Elf32_Word p_memsz;
  Elf32_Word p_flags;
  Elf32_Word p_align;
};




constexpr Elf32_Word EV_NONE = 0;// invalid version
constexpr Elf32_Word EV_CURRENT = 1;// current version
// higher numbers are permitted

#endif /* ELF32_HPP */
