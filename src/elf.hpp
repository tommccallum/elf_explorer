#ifndef ELF_HPP
#define ELF_HPP

#include "Elf_Program_Header_Fields.hpp"
#include "Elf_Section_Header_Fields.hpp"
#include "Elf_Sym.hpp"
#include "elf32.hpp"
#include "elf64.hpp"
#include "elf_common.hpp"
#include <array>
#include <iostream>

constexpr std::array<struct elf_program_header_fields_t, 8> elf_program_header_fields{ { { 0, { 0, sizeof(Elf32_Word), sizeof(Elf64_Word) } },
  { 1, { 0, sizeof(Elf32_Off), sizeof(Elf64_Word) } },
  { 2, { 0, sizeof(Elf32_Addr), sizeof(Elf64_Off) } },
  { 3, { 0, sizeof(Elf32_Addr), sizeof(Elf64_Addr) } },
  { 4, { 0, sizeof(Elf32_Word), sizeof(Elf64_Addr) } },
  { 5, { 0, sizeof(Elf32_Word), sizeof(Elf64_Xword) } },
  { 6, { 0, sizeof(Elf32_Word), sizeof(Elf64_Xword) } },
  { 7, { 0, sizeof(Elf32_Word), sizeof(Elf64_Xword) } } } };

constexpr std::array<struct elf_section_header_fields_t, 10> elf_section_header_fields{ { { 0, { 0, sizeof(Elf32_Word), sizeof(Elf64_Word) } },
  { 1, { 0, sizeof(Elf32_Word), sizeof(Elf64_Word) } },
  { 2, { 0, sizeof(Elf32_Word), sizeof(Elf64_Xword) } },
  { 3, { 0, sizeof(Elf32_Addr), sizeof(Elf64_Addr) } },
  { 4, { 0, sizeof(Elf32_Off), sizeof(Elf64_Off) } },
  { 5, { 0, sizeof(Elf32_Word), sizeof(Elf64_Xword) } },
  { 6, { 0, sizeof(Elf32_Word), sizeof(Elf64_Word) } },
  { 7, { 0, sizeof(Elf32_Word), sizeof(Elf64_Word) } },
  { 8, { 0, sizeof(Elf32_Word), sizeof(Elf64_Xword) } },
  { 9, { 0, sizeof(Elf32_Word), sizeof(Elf64_Xword) } } } };


constexpr std::array<struct elf_symbol_table_fields_t, 6> elf_symbol_table_fields{ { { 0, { 0, sizeof(Elf32_Word), sizeof(Elf64_Word) } },
  { 1, { 0, sizeof(Elf32_Addr), sizeof(UNSIGNED_CHAR) } },
  { 2, { 0, sizeof(Elf32_Word), sizeof(UNSIGNED_CHAR) } },
  { 3, { 0, sizeof(UNSIGNED_CHAR), sizeof(Elf64_Half) } },
  { 4, { 0, sizeof(UNSIGNED_CHAR), sizeof(Elf64_Addr) } },
  { 5, { 0, sizeof(Elf32_Half), sizeof(Elf64_Xword) } } } };


// Sections can have names without a prefixed '.' these are application specific
// Sections can appear more than once with same name
// Processor specific sections are e_machine.section e.g. .EM_386.data
// @todo what are the preexisting extensions (bottom of https://www.sco.com/developers/gabi/latest/ch4.sheader.html)?
extern special_sections_32_t special_sections_32[];

extern data_types_t data_types[];


std::string e_type_to_string(uint64_t value);
std::string elf_sym_binding_to_string(uint64_t value);
std::string elf_special_section_types_to_string(uint64_t value);
std::string elf_special_section_types_to_string(uint64_t value);
std::string elf_program_flag_to_string(ELF_ULONG type);
void write_elf_ident(container_ref bytes);
std::string elf_sym_type_to_string(uint64_t value);
std::string elf_sym_visibility_to_string(uint64_t value);
std::string elf_program_type_to_string(ELF_ULONG type);
std::string elf_program_flag_to_string(ELF_ULONG type);


#endif /* ELF_HPP */
