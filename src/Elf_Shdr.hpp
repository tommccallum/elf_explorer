#ifndef ELF_SHDR_HPP
#define ELF_SHDR_HPP

#include "elf_common.hpp"
#include <iostream>

/**
 * @brief We standardise on 64-bit format to save typing.
 * 
 */
struct Elf_Shdr
{
  ELF_ULONG sh_name;
  ELF_ULONG sh_type;
  ELF_ULONG sh_flags;
  ELF_ULONG sh_addr;
  ELF_ULONG sh_offset;
  ELF_ULONG sh_size;
  ELF_ULONG sh_link;
  ELF_ULONG sh_info;
  ELF_ULONG sh_addralign;
  ELF_ULONG sh_entsize;
  std::string name;
  size_t index;

  bool is_writable() const;
  bool is_allocated() const;
  bool is_executable() const;
  bool is_merged() const;
  bool is_strings() const;
  bool is_info_link() const;
  bool is_link_order() const;
  bool is_os_nonconforming() const;
  bool is_grouped() const;
  bool is_tls() const;
  bool is_compressed() const;
  bool has_os_flags() const;
  bool has_cpu_flags() const;
  uint64_t get_associated_string_table() const;
  /**
   * @brief Get the associated symbol table object
   * 
   * @return uint64_t 
   */
  uint64_t get_associated_symbol_table() const;
  /**
     * @brief Get the index for relocation object
     * The section header index of the section to which the relocation applies.
     * In other words the section we apply the relocation to.
     * @return uint64_t 
     */
  uint64_t get_index_for_relocation() const;
  /**
     * @brief Get the index last local symbol object
     * One greater than the symbol table index of the last local symbol (binding STB_LOCAL).
     * @return uint64_t 
     */
  uint64_t get_index_last_local_symbol() const;
  /**
     * @brief Get the index of symbol table object
     * The symbol table index of an entry in the associated symbol table. The name of the specified symbol table entry provides a signature for the section group.
     * @return uint64_t 
     */
  uint64_t get_index_of_symbol_table() const;
};

std::ostream &operator<<(std::ostream &out, Elf_Shdr const &header);

#endif /* ELF_SHDR_HPP */
