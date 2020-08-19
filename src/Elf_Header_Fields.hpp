#ifndef ELF_HEADER_FIELDS_HPP
#define ELF_HEADER_FIELDS_HPP

#include "elf32.hpp"
#include "elf64.hpp"
#include <cstdint>
#include <array>

// aligns the sizes so we can merge the code for the reading of the fields
struct elf_header_fields_t
{
  uint8_t index;
  std::array<uint8_t, 3> sz;// NONE, 32-bit, 64-bit
};

constexpr std::array<struct elf_header_fields_t, 13> elf_header_fields{ { { 0, { 0, sizeof(Elf32_Half), sizeof(Elf64_Half) } },
  { 1, { 0, sizeof(Elf32_Half), sizeof(Elf64_Half) } },
  { 2, { 0, sizeof(Elf32_Word), sizeof(Elf64_Word) } },
  { 3, { 0, sizeof(Elf32_Addr), sizeof(Elf64_Addr) } },
  { 4, { 0, sizeof(Elf32_Off), sizeof(Elf64_Off) } },
  { 5, { 0, sizeof(Elf32_Off), sizeof(Elf64_Off) } },
  { 6, { 0, sizeof(Elf32_Word), sizeof(Elf64_Word) } },
  { 7, { 0, sizeof(Elf32_Half), sizeof(Elf64_Half) } },
  { 8, { 0, sizeof(Elf32_Half), sizeof(Elf64_Half) } },
  { 9, { 0, sizeof(Elf32_Half), sizeof(Elf64_Half) } },
  { 10, { 0, sizeof(Elf32_Half), sizeof(Elf64_Half) } },
  { 11, { 0, sizeof(Elf32_Half), sizeof(Elf64_Half) } },
  { 12, { 0, sizeof(Elf32_Half), sizeof(Elf64_Half) } } } };


#endif /* ELF_HEADER_FIELDS_HPP */
