#include "section_types.hpp"


std::string e_section_types_to_string(int64_t value)
{
  switch(value) {
    case 0:
      return "SHT_NULL";
    case 1:
      return "SHT_PROGBITS";
    case 2:
      return "SHT_SYMTAB";
    case 3:
      return "SHT_STRTAB";
    case 4:
      return "SHT_RELA";
    case 5:
      return "SHT_HASH";
    case 6:
      return "SHT_DYNAMIC";
    case 7:
      return "SHT_NOTE";
    case 8:
      return "SHT_NOBITS";
    case 9:
      return "SHT_REL";
    case 10:
      return "SHT_SHLIB";
    case 11:
      return "SHT_DYNSYM";
    case 14:
      return "SHT_INIT_ARRAY";
    case 15:
      return "SHT_FINI_ARRAY";
    case 16:
      return "SHT_PREINIT_ARRAY";
    case 17:
      return "SHT_GROUP";
    case 18:
      return "SHT_SYMTAB_SHNDX";
    case 0x60000000:
      return "SHT_LOOS";
    case 0x6FFFFFFF:
      return "SHT_HIOS";
    case 0x70000000:
      return "SHT_LOPROC";
    case 0x7FFFFFFF:
      return "SHT_HIPROC";
    case 0x80000000:
      return "SHT_LOUSER";
    case 0xFFFFFFFF:
      return "SHT_HIUSER";
    default:
      return "unknown";
  }
}

