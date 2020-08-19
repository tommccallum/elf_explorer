#ifndef ELF_PROGRAM_HEADER_FIELDS_HPP
#define ELF_PROGRAM_HEADER_FIELDS_HPP

#include <cstdlib>
#include <array>

struct elf_program_header_fields_t
{
  uint8_t index;
  std::array<uint8_t, 3> sz;// NONE, 32-bit, 64-bit
};
#endif /* ELF_PROGRAM_HEADER_FIELDS_HPP */
