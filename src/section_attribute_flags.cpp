#include "section_attribute_flags.hpp"

std::string e_sh_flags_to_string(int64_t value)
{
  switch(value) {
    case 0x1:
      return "SHF_WRITE";
    case 0x2:
      return "SHF_ALLOC";
    case 0x4:
      return "SHF_EXECINSTR";
    case 0x10:
      return "SHF_MERGE";
    case 0x20:
      return "SHF_STRINGS";
    case 0x40:
      return "SHF_INFO_LINK";
    case 0x80:
      return "SHF_LINK_ORDER";
    case 0x100:
      return "SHF_OS_NONCONFORMING";
    case 0x200:
      return "SHF_GROUP";
    case 0x400:
      return "SHF_TLS";
    case 0x800:
      return "SHF_COMPRESSED";
    case 0x0FF00000:
      return "SHF_MASKOS";
    case 0xF0000000:
      return "SHF_MASKPROC";
    default:
      return "unknown";
  }
}
