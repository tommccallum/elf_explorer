#ifndef ELF_COMMON_HPP
#define ELF_COMMON_HPP

#include <cstdint>
#include <vector>
#include <string>
#include "machines.hpp"
#include "osabi.hpp"
#include "section_attribute_flags.hpp"
#include "section_types.hpp"

using byte = uint8_t;
using container = std::vector<byte>;
using container_ref = std::vector<byte> &;
using const_container_ref = std::vector<byte> const &;
using ELF_SLONG = uint64_t;
using ELF_ULONG = uint64_t;
using UNSIGNED_CHAR = uint8_t;



constexpr size_t SZ_UCHAR = sizeof(UNSIGNED_CHAR);

constexpr byte ELFCLASSNONE = 0;// Invalid class
constexpr byte ELFCLASS32 = 1;// 32-bit objects
constexpr byte ELFCLASS64 = 2;// 64-bit objects

enum e_type {
  ET_NONE = 0,// no file type
  ET_REL = 1,// relocatable file
  ET_EXEC = 2,// executable file
  ET_DYN = 3,// shared object file
  ET_CORE = 4,// core file
  ET_LOPROC = 0xFF00,// processor specific
  ET_HIPROC = 0xFFFF,// processor specific
  END_OF_ELF_TYPES
};

enum elf_parts {
  ELF_HEADER,
  PROGRAM_HEADER,
  SECTION,
  SECTION_HEADER,
  END_OF_ELF
};

struct data_types_t
{
  std::string name;
  size_t size;
  size_t alignment;
};



enum e_ident {
  EI_MAG0 = 0,//File identification
  EI_MAG1 = 1,//File identification
  EI_MAG2 = 2,//File identification
  EI_MAG3 = 3,//File identification
  EI_CLASS = 4,//File class
  EI_DATA = 5,//Data encoding
  EI_VERSION = 6,//File version
  EI_OSABI = 7,// 	Operating system/ABI identification
  EI_ABIVERSION = 8,// 	ABI version
  EI_PAD = 9,//Start of padding bytes
  EI_RESERVED_1 = 10,
  EI_RESERVED_2 = 11,
  EI_RESERVED_3 = 12,
  EI_RESERVED_4 = 13,
  EI_RESERVED_5 = 14,
  EI_RESERVED_6 = 15,
  EI_NIDENT = 16// size of e_ident
};



enum elf_sym_binding {
  STB_LOCAL = 0,
  STB_GLOBAL = 1,
  STB_WEAK = 2,
  STB_LOOS = 10,
  STB_HIOS = 12,
  STB_LOPROC = 13,
  STB_HIPROC = 15
};


enum elf_sym_type {
  STT_NOTYPE = 0,
  STT_OBJECT = 1,
  STT_FUNC = 2,
  STT_SECTION = 3,
  STT_FILE = 4,
  STT_COMMON = 5,
  STT_TLS = 6,
  STT_LOOS = 10,
  STT_HIOS = 12,
  STT_LOPROC = 13,
  STT_HIPROC = 15
};


enum elf_sym_visibility {
  STV_DEFAULT = 0,
  STV_INTERNAL = 1,
  STV_HIDDEN = 2,
  STV_PROTECTED = 3
};


constexpr ELF_ULONG PT_NULL = 0L;
constexpr ELF_ULONG PT_LOAD = 1L;
constexpr ELF_ULONG PT_DYNAMIC = 2L;
constexpr ELF_ULONG PT_INTERP = 3L;
constexpr ELF_ULONG PT_NOTE = 4L;
constexpr ELF_ULONG PT_SHLIB = 5L;
constexpr ELF_ULONG PT_PHDR = 6L;
constexpr ELF_ULONG PT_TLS = 7L;
constexpr ELF_ULONG PT_LOOS = 0x60000000;
constexpr ELF_ULONG PT_HIOS = 0x6fffffff;
constexpr ELF_ULONG PT_LOPROC = 0x70000000;
constexpr ELF_ULONG PT_HIPROC = 0x7fffffff;


constexpr ELF_ULONG PF_X = 0x1;// Execute
constexpr ELF_ULONG PF_W = 0x2;// Write
constexpr ELF_ULONG PF_R = 0x4;// Read
constexpr ELF_ULONG PF_MASKOS = 0x0ff00000;// Unspecified
constexpr ELF_ULONG PF_MASKPROC = 0xf0000000;// Unspecified

// SECTIONS

// special section types
constexpr uint64_t SHN_UNDEF = 0;// no section header
constexpr uint64_t SHN_LORESERVE = 0xff00;// lower bound of the reserved indexes
constexpr uint64_t SHN_LOPROC = 0xff00;// from this value, processor-specific semantics
constexpr uint64_t SHN_HIPROC = 0xff1f;// up to this value, processor-specific semantics
constexpr uint64_t SHN_LOOS = 0xff20;// from this value, operating system specific semantics
constexpr uint64_t SHN_HIOS = 0xff3f;// up to this value, operating system specific semantics
constexpr uint64_t SHN_ABS = 0xfff1;// symbols defined in this section have absolute value and not affected by relocation
constexpr uint64_t SHN_COMMON = 0xfff2;// common symbols, unallocated C external variables
constexpr uint64_t SHN_XINDEX = 0xffff;// indicates the actual section header index is too large to fit and found in another location
constexpr uint64_t SHN_HIRESERVE = 0xffff;// upper bound of the reserved indexes

constexpr byte ELFMAG0 = 0x7F;
constexpr byte ELFMAG1 = 'E';
constexpr byte ELFMAG2 = 'L';
constexpr byte ELFMAG3 = 'F';


// Data encoding of the process-specific data in the object file
constexpr byte ELFDATANONE = 0;// Invalid class
constexpr byte ELFDATA2LSB = 1;// 2's complement, least significant byte is lowest address
constexpr byte ELFDATA2MSB = 2;// 2's complement, most significant byte is lowest address



#endif /* ELF_COMMON_HPP */
