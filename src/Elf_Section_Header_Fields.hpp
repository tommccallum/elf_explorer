#ifndef ELF_SECTION_HEADER_FIELDS_HPP
#define ELF_SECTION_HEADER_FIELDS_HPP

#include <cstdint>
#include <array>

// aligns the sizes so we can merge the code for the reading of the fields
struct elf_section_header_fields_t
{
  uint8_t index;
  std::array<uint8_t, 3> sz;// NONE, 32-bit, 64-bit
};

#endif /* ELF_SECTION_HEADER_FIELDS_HPP */
