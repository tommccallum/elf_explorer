#ifndef ELFREADER_HPP
#define ELFREADER_HPP

#include "Elf_Phdr.hpp"
#include "Elf_Shdr.hpp"
#include "SectionTableInfo.hpp"
#include "elf_common.hpp"
#include "Elf_Ehdr.hpp"

#include <cassert>
#include <fstream>
#include <functional>
#include <iomanip>
#include <iostream>
#include <iterator>
#include <stdexcept>
#include <string>
#include <string_view>
#include <unistd.h>
#include <vector>

class ElfReader
{
public:
  uint64_t filesize;
  uint32_t byte_size;
  uint32_t data_encoding;
  Elf_Ehdr header;
  std::vector<Elf_Shdr> section_headers;
  std::vector<Elf_Phdr> program_headers;

private:
  std::vector<byte> bytes;
  std::function<ELF_SLONG(size_t &index, size_t count)> read_bytes;
  std::vector<std::unique_ptr<SectionTableInfo>> section_table_info;

public:
  explicit ElfReader(container_ref bytes);
  explicit ElfReader(const std::string filename);
  bool is_32bit() const noexcept;

  bool is_64bit() const noexcept;

  bool is_elf() const noexcept;

  /**
     * @brief Get the section count object, 0 before sections are read in
     * 
     * @return size_t 
     */
  size_t get_section_count() const;

  friend std::ostream &operator<<(std::ostream &out, const ElfReader &elf);

  /**
   * @brief Get the base address object
   * 
   * lowest p_vaddr of all the LOAD segments
   * 
   * @return ELF_ULONG 
   */
  ELF_ULONG get_start_address() const;
  void init();
  byte get_class() const;
  byte get_data_encoding() const;
  byte get_version() const;
  byte get_osabi() const;
  byte get_abiversion() const;
  void read_symbol_table(Elf_Shdr section);
  void read_string_table(Elf_Shdr section);
  void read_section_tables();
  void read_section_names();
  std::string read_from_string_table(size_t ptr);
  void read_elf_header();
  void read_section_header(size_t offset);
  void read_program_headers();
  int64_t read_lsb64(size_t &index, size_t count);
  int64_t read_msb64(size_t &index, size_t count);
};

/**
 * @brief Print the details about the ELF file
 * 
 * @param out 
 * @param elf 
 * @return std::ostream& 
 */
std::ostream &operator<<(std::ostream &out, const ElfReader &elf);

#endif /* ELFREADER_HPP */
