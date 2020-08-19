#include "ElfReader.hpp"
#include "Elf_Header_Fields.hpp"
#include "Elf_Program_Header_Fields.hpp"
#include "Elf_Section_Header_Fields.hpp"
#include "Elf_Sym.hpp"
#include "StringTable.hpp"
#include "SymbolTable.hpp"
#include "elf.hpp"
#include "machines.hpp"
#include "osabi.hpp"
#include "section_attribute_flags.hpp"
#include "section_types.hpp"


ElfReader::ElfReader(container_ref bytes) : bytes(bytes) { init(); };
ElfReader::ElfReader(const std::string filename)
{
  std::ifstream elf_file(filename, std::ios::in | std::ios::binary);
  try {
    if (elf_file.is_open()) {
      // this is required otherwise when we read it in we do not get the
      // newline characters and our array comes up short.
      elf_file.unsetf(std::ios::skipws);

      // get the filesize by moving to the end of the file and back again
      elf_file.seekg(0, std::ios::end);
      filesize = elf_file.tellg();
      elf_file.seekg(0, std::ios::beg);

      // set our vector size accordingly and read in
      bytes.reserve(filesize);
      bytes.insert(bytes.begin(), std::istream_iterator<uint8_t>(elf_file), std::istream_iterator<uint8_t>());
    }
    elf_file.close();
  } catch (const std::exception &ex) {
    std::cout << ex.what() << std::endl;
    elf_file.close();
    return;
  }
  init();
}

bool ElfReader::is_32bit() const noexcept
{
  return byte_size == ELFCLASS32;
}

bool ElfReader::is_64bit() const noexcept
{
  return byte_size == ELFCLASS64;
}

bool ElfReader::is_elf() const noexcept
{
  return bytes[EI_MAG0] == ELFMAG0 && bytes[EI_MAG1] == ELFMAG1 && bytes[EI_MAG2] == ELFMAG2 && bytes[EI_MAG3] == ELFMAG3;
}

/**
     * @brief Get the section count object, 0 before sections are read in
     * 
     * @return size_t 
     */
size_t ElfReader::get_section_count() const
{
  return section_headers.size();
}

/**
   * @brief Get the base address object
   * 
   * lowest p_vaddr of all the LOAD segments
   * 
   * @return ELF_ULONG 
   */
ELF_ULONG ElfReader::get_start_address() const
{
  if (program_headers.size() > 0) {
    ELF_ULONG v_addr = 0L - 1;
    ELF_ULONG p_addr = 0L - 1;
    ELF_ULONG mem_size = 0L - 1;
    ELF_ULONG os_pagesize = getpagesize();// POSIX specific call
    size_t which_lowest = 0;
    size_t counter = 0;
    for (auto p : program_headers) {
      if (p.p_type == PT_LOAD) {
        if (p.p_vaddr < v_addr) {
          v_addr = p.p_vaddr;
          which_lowest = counter;
        }
      }
      counter++;
    }
    ELF_ULONG trunc_paddr = static_cast<ELF_ULONG>(p_addr / os_pagesize) * os_pagesize;
    ELF_ULONG trunc_vaddr = static_cast<ELF_ULONG>(program_headers[which_lowest].p_vaddr / os_pagesize) * os_pagesize;
    // std::cout << "Page size: "<< std::hex << os_pagesize << std::dec << std::endl;
    // std::cout << "Lowest mem size: "<< std::hex << p_addr << std::dec << std::endl;
    // std::cout << "Associated vaddr: "<< std::hex << program_headers[which_lowest].p_vaddr << std::dec << std::endl;
    // std::cout << "Truncated paddr: "<< std::hex << trunc_paddr << std::dec << std::endl;
    // std::cout << "Truncated vaddr: "<< std::hex << trunc_vaddr << std::dec << std::endl;
    return trunc_vaddr;
  }
  return 0L;
}

/**
     * @brief Read in the ELF file separating out the information we want to record
     *          Called on construction
     */
void ElfReader::init()
{
  data_encoding = static_cast<uint32_t>(get_data_encoding());
  byte_size = static_cast<uint32_t>(get_class());
  if (is_32bit()) {
    if (data_encoding == ELFDATA2LSB) {
      read_bytes = std::bind(&ElfReader::read_lsb64, this, std::placeholders::_1, std::placeholders::_2);
    } else if (data_encoding == ELFDATA2MSB) {
      read_bytes = std::bind(&ElfReader::read_msb64, this, std::placeholders::_1, std::placeholders::_2);
    } else {
      throw new std::runtime_error("no valid data encoding found");
    }
  } else if (is_64bit()) {
    if (data_encoding == ELFDATA2LSB) {
      read_bytes = std::bind(&ElfReader::read_lsb64, this, std::placeholders::_1, std::placeholders::_2);
    } else if (data_encoding == ELFDATA2MSB) {
      read_bytes = std::bind(&ElfReader::read_msb64, this, std::placeholders::_1, std::placeholders::_2);
    } else {
      throw new std::runtime_error("no valid data encoding found");
    }
  } else {
    // no class specified
    return;
  }
  read_elf_header();
  std::vector<size_t> offsets;
  size_t section_name_string_table_index;
  offsets.push_back(header.e_shoff);
  for (int ii = 1; ii < header.e_shnum; ii++) {
    offsets.push_back(header.e_shoff + (header.e_shentsize * ii));
  }
  section_name_string_table_index = header.e_shstrndx;
  for (auto offset : offsets) {
    read_section_header(offset);
  }
  assert(section_name_string_table_index < get_section_count());
  read_section_names();
  read_section_tables();
  read_program_headers();
}

byte ElfReader::get_class() const
{
  return bytes[EI_CLASS];
}

byte ElfReader::get_data_encoding() const
{
  return bytes[EI_DATA];
}

byte ElfReader::get_version() const
{
  return bytes[EI_VERSION];
}

byte ElfReader::get_osabi() const
{
  return bytes[EI_OSABI];
}

byte ElfReader::get_abiversion() const
{
  return bytes[EI_ABIVERSION];
}

void ElfReader::read_symbol_table(Elf_Shdr section)
{
  std::cout << "Reading symbol table '" << section.name << "'\n";
  auto sti = std::make_unique<SymbolTable>();
  std::vector<Elf_Sym> entries;
  size_t start = section.sh_offset;
  size_t end = section.sh_offset + section.sh_size;
  size_t cursor = start;
  size_t fixed_cursor = cursor;
  while (cursor < end) {
    auto entry = Elf_Sym{};
    cursor = fixed_cursor;
    if (is_64bit()) {
      entry.st_name = read_bytes(cursor, elf_symbol_table_fields[0].sz[byte_size] / SZ_UCHAR);
      entry.st_info = read_bytes(cursor, elf_symbol_table_fields[1].sz[byte_size] / SZ_UCHAR);
      entry.st_other = read_bytes(cursor, elf_symbol_table_fields[2].sz[byte_size] / SZ_UCHAR);
      entry.st_shndx = read_bytes(cursor, elf_symbol_table_fields[3].sz[byte_size] / SZ_UCHAR);
      entry.st_value = read_bytes(cursor, elf_symbol_table_fields[4].sz[byte_size] / SZ_UCHAR);
      entry.st_size = read_bytes(cursor, elf_symbol_table_fields[5].sz[byte_size] / SZ_UCHAR);
    } else {
      entry.st_name = read_bytes(cursor, elf_symbol_table_fields[0].sz[byte_size] / SZ_UCHAR);
      entry.st_value = read_bytes(cursor, elf_symbol_table_fields[1].sz[byte_size] / SZ_UCHAR);
      entry.st_size = read_bytes(cursor, elf_symbol_table_fields[2].sz[byte_size] / SZ_UCHAR);
      entry.st_info = read_bytes(cursor, elf_symbol_table_fields[3].sz[byte_size] / SZ_UCHAR);
      entry.st_other = read_bytes(cursor, elf_symbol_table_fields[4].sz[byte_size] / SZ_UCHAR);
      entry.st_shndx = read_bytes(cursor, elf_symbol_table_fields[5].sz[byte_size] / SZ_UCHAR);
    }
    entry.file_type = header.e_type;
    if (section.get_associated_string_table() != 0) {
      size_t ptr = section_headers[section.get_associated_string_table()].sh_offset + entry.st_name;
      entry.name = read_from_string_table(ptr);
    }
    entries.push_back(entry);
    fixed_cursor += section.sh_entsize;
  }
  sti->entries = entries;
  section_table_info.push_back(std::move(sti));
}

void ElfReader::read_string_table(Elf_Shdr section)
{
  std::cout << "Reading string table '" << section.name << "'\n";
  auto sti = std::make_unique<StringTable>();
  std::vector<std::string> entries;
  size_t start = section.sh_offset;
  size_t end = section.sh_offset + section.sh_size;
  size_t cursor = start;
  while (cursor < end) {
    // skip through null pointers
    while (bytes[cursor] == '\0') {
      cursor++;
      if (cursor >= end) break;
    }
    if (cursor >= end) break;
    // save start of new name
    auto start_cursor = cursor;
    // skip through everything but null to end of name
    while (bytes[cursor] != '\0') {
      cursor++;
      if (cursor >= end) break;
    }
    // save name
    auto entry = std::string{ begin(bytes) + start_cursor, begin(bytes) + cursor };
    entries.push_back(entry);
  }
  sti->entries = entries;
  section_table_info.push_back(std::move(sti));
}

void ElfReader::read_section_tables()
{
  for (auto sh : section_headers) {
    switch (sh.sh_type) {
    case SHT_SYMTAB:
      read_symbol_table(sh);
      break;
    case SHT_STRTAB:
      read_string_table(sh);
      break;
    default:
      section_table_info.push_back(std::make_unique<SectionTableInfo>());
    }
  }
}

void ElfReader::read_section_names()
{
  auto string_section_header = section_headers[header.e_shstrndx];
  size_t start = string_section_header.sh_offset;
  size_t end = string_section_header.sh_offset + string_section_header.sh_size;
  for (auto &sh : section_headers) {
    Elf64_Word ptr = sh.sh_name;
    assert(start + ptr < end);
    sh.name = read_from_string_table(start + ptr);
  }
}

std::string ElfReader::read_from_string_table(size_t ptr)
{
  size_t start = ptr;
  while (bytes[ptr] != '\0')
    ptr++;
  auto name = std::string{ bytes.begin() + start, bytes.begin() + ptr };
  return name;
}

void ElfReader::read_elf_header()
{
  size_t cursor = EI_NIDENT;
  header.e_type = read_bytes(cursor, elf_header_fields[0].sz[byte_size] / SZ_UCHAR);
  header.e_machine = read_bytes(cursor, elf_header_fields[1].sz[byte_size] / SZ_UCHAR);
  header.e_version = read_bytes(cursor, elf_header_fields[2].sz[byte_size] / SZ_UCHAR);
  header.e_entry = read_bytes(cursor, elf_header_fields[3].sz[byte_size] / SZ_UCHAR);
  header.e_phoff = read_bytes(cursor, elf_header_fields[4].sz[byte_size] / SZ_UCHAR);
  header.e_shoff = read_bytes(cursor, elf_header_fields[5].sz[byte_size] / SZ_UCHAR);
  header.e_flags = read_bytes(cursor, elf_header_fields[6].sz[byte_size] / SZ_UCHAR);
  header.e_ehsize = read_bytes(cursor, elf_header_fields[7].sz[byte_size] / SZ_UCHAR);
  header.e_phentsize = read_bytes(cursor, elf_header_fields[8].sz[byte_size] / SZ_UCHAR);
  header.e_phnum = read_bytes(cursor, elf_header_fields[9].sz[byte_size] / SZ_UCHAR);
  header.e_shentsize = read_bytes(cursor, elf_header_fields[10].sz[byte_size] / SZ_UCHAR);
  header.e_shnum = read_bytes(cursor, elf_header_fields[11].sz[byte_size] / SZ_UCHAR);
  header.e_shstrndx = read_bytes(cursor, elf_header_fields[12].sz[byte_size] / SZ_UCHAR);
}

void ElfReader::read_section_header(size_t offset)
{
  struct Elf_Shdr section_header;
  section_header.sh_name = read_bytes(offset, elf_section_header_fields[0].sz[byte_size] / SZ_UCHAR);
  section_header.sh_type = read_bytes(offset, elf_section_header_fields[1].sz[byte_size] / SZ_UCHAR);
  section_header.sh_flags = read_bytes(offset, elf_section_header_fields[2].sz[byte_size] / SZ_UCHAR);
  section_header.sh_addr = read_bytes(offset, elf_section_header_fields[3].sz[byte_size] / SZ_UCHAR);
  section_header.sh_offset = read_bytes(offset, elf_section_header_fields[4].sz[byte_size] / SZ_UCHAR);
  section_header.sh_size = read_bytes(offset, elf_section_header_fields[5].sz[byte_size] / SZ_UCHAR);
  section_header.sh_link = read_bytes(offset, elf_section_header_fields[6].sz[byte_size] / SZ_UCHAR);
  section_header.sh_info = read_bytes(offset, elf_section_header_fields[7].sz[byte_size] / SZ_UCHAR);
  section_header.sh_addralign = read_bytes(offset, elf_section_header_fields[8].sz[byte_size] / SZ_UCHAR);
  section_header.sh_entsize = read_bytes(offset, elf_section_header_fields[9].sz[byte_size] / SZ_UCHAR);
  section_header.index = section_headers.size();
  section_headers.push_back(section_header);
}

void ElfReader::read_program_headers()
{
  if (header.e_phoff == 0) return;
  size_t offset = header.e_phoff;
  for (int ii = 0; ii < header.e_phnum; ii++) {
    size_t start = offset;
    Elf_Phdr program_header;
    program_header.p_type = read_bytes(offset, elf_program_header_fields[0].sz[byte_size] / SZ_UCHAR);
    if (is_64bit()) {
      program_header.p_flags = read_bytes(offset, elf_program_header_fields[1].sz[byte_size] / SZ_UCHAR);
      program_header.p_offset = read_bytes(offset, elf_program_header_fields[2].sz[byte_size] / SZ_UCHAR);
      program_header.p_vaddr = read_bytes(offset, elf_program_header_fields[3].sz[byte_size] / SZ_UCHAR);
      program_header.p_paddr = read_bytes(offset, elf_program_header_fields[4].sz[byte_size] / SZ_UCHAR);
      program_header.p_filesz = read_bytes(offset, elf_program_header_fields[5].sz[byte_size] / SZ_UCHAR);
      program_header.p_memsz = read_bytes(offset, elf_program_header_fields[6].sz[byte_size] / SZ_UCHAR);
    } else {
      program_header.p_offset = read_bytes(offset, elf_program_header_fields[1].sz[byte_size] / SZ_UCHAR);
      program_header.p_vaddr = read_bytes(offset, elf_program_header_fields[2].sz[byte_size] / SZ_UCHAR);
      program_header.p_paddr = read_bytes(offset, elf_program_header_fields[3].sz[byte_size] / SZ_UCHAR);
      program_header.p_filesz = read_bytes(offset, elf_program_header_fields[4].sz[byte_size] / SZ_UCHAR);
      program_header.p_memsz = read_bytes(offset, elf_program_header_fields[5].sz[byte_size] / SZ_UCHAR);
      program_header.p_flags = read_bytes(offset, elf_program_header_fields[6].sz[byte_size] / SZ_UCHAR);
    }
    program_header.p_align = read_bytes(offset, elf_program_header_fields[7].sz[byte_size] / SZ_UCHAR);
    program_headers.push_back(program_header);
    offset = start + header.e_phentsize;
  }
}

int64_t ElfReader::read_lsb64(size_t &index, size_t count)
{
  assert(count == 1 || count == 2 || count == 4 || count == 8);
  int64_t result = 0LL;
  for (size_t ii = 0; ii < count; ii++) {
    int64_t p = bytes[index++];// convert byte to larger integer size
    int64_t q = p;
    if (ii > 0) {
      q = p << (8 * (ii));// shift the value up
    }
    //std::cout << (8*(ii-1)) << " " << std::hex << "0x" << std::setfill('0') << std::setw(8) << p << " " << q  << std::dec << std::endl;
    result |= (q);
  }
  return result;
}

int64_t ElfReader::read_msb64(size_t &index, size_t count)
{
  assert(count == 1 || count == 2 || count == 4 || count == 8);
  int64_t result = 0LL;
  for (size_t ii = 0; ii < count; ii++) {
    int64_t p = bytes[index++];// convert byte to larger integer size
    int64_t q = p;
    if (count - ii - 1 > 0) {
      q = p << (8 * (count - ii - 1));// shift the value up
    }
    //std::cout << (8*(ii-1)) << " " << std::hex << "0x" << std::setfill('0') << std::setw(8) << p << " " << q  << std::dec << std::endl;
    result |= (q);
  }
  return result;
}


/**
 * @brief Print the details about the ELF file
 * 
 * @param out 
 * @param elf 
 * @return std::ostream& 
 */
std::ostream &operator<<(std::ostream &out, const ElfReader &elf)
{
  out << "[INFO] filesize: " << elf.filesize << " in: " << elf.bytes.size() << " capacity: " << elf.bytes.capacity() << " bytes\n";
  out << (elf.byte_size == ELFCLASS32 ? "32" : "64") << "-bit ELF Header" << std::endl;
  out << "-----------------" << std::endl;
  size_t count = EI_NIDENT;
  out << "[MAGIC] ";
  for (size_t ii = 0; ii < count; ii++) {
    out << std::hex << std::setw(2) << std::setfill('0') << static_cast<uint64_t>(elf.bytes[ii]) << " ";
  }
  out << std::dec << "\n";
  out << "Class: " << std::hex << std::setw(2) << std::setfill('0') << "0x" << static_cast<uint64_t>(elf.get_class()) << std::dec << std::endl;
  out << "Data Encoding: " << std::hex << std::setw(2) << std::setfill('0') << "0x" << static_cast<uint64_t>(elf.get_data_encoding()) << std::dec << std::endl;
  out << "Version: " << std::hex << std::setw(2) << std::setfill('0') << "0x" << static_cast<uint64_t>(elf.get_version()) << std::dec << std::endl;
  out << "OS ABI: " << std::hex << std::setw(2) << std::setfill('0') << "0x" << static_cast<uint64_t>(elf.get_osabi()) << std::dec << std::endl;
  out << "ABI Version: " << std::hex << std::setw(2) << std::setfill('0') << "0x" << static_cast<uint64_t>(elf.get_abiversion()) << std::dec << std::endl;
  int64_t result = elf.header.e_type;
  out << "Type: " << e_type_to_string(result) << " (Dec: " << result << " " << std::hex << "0x" << result << std::dec << ")" << std::endl;
  result = elf.header.e_machine;
  out << "Machine: " << e_machines_to_string(result) << " (Dec: " << result << " Hex: " << std::hex << "0x" << result << std::dec << ")" << std::endl;
  result = elf.header.e_version;
  out << "Version: " << result << " " << std::hex << "0x" << result << std::dec << std::endl;
  result = elf.header.e_entry;
  out << "Entry Address: " << result << " " << std::hex << "0x" << result << std::dec << std::endl;
  result = elf.header.e_phoff;
  out << "Program header offset: " << result << " " << std::hex << "0x" << result << std::dec << std::endl;
  result = elf.header.e_shoff;
  out << "Section header offset: " << result << " " << std::hex << "0x" << result << std::dec << std::endl;
  result = elf.header.e_flags;
  out << "Flags: " << result << " " << std::hex << "0x" << result << std::dec << std::endl;
  result = elf.header.e_ehsize;
  out << "Elf header size: " << result << " " << std::hex << "0x" << result << std::dec << std::endl;
  result = elf.header.e_phentsize;
  out << "Program header entry size: " << result << " " << std::hex << "0x" << result << std::dec << std::endl;
  result = elf.header.e_phnum;
  out << "Program header entry count: " << result << " " << std::hex << "0x" << result << std::dec << std::endl;
  result = elf.header.e_shentsize;
  out << "Section header entry size: " << result << " " << std::hex << "0x" << result << std::dec << std::endl;
  result = elf.header.e_shnum;
  out << "Section header entry count: " << result << " " << std::hex << "0x" << result << std::dec << std::endl;
  result = elf.header.e_shstrndx;
  out << "Index of string table with section names in: " << result << " " << std::hex << "0x" << result << std::dec << std::endl;

  size_t counter = 0;
  for (auto sh : elf.section_headers) {
    out << sh << std::endl;
    out << *(elf.section_table_info[counter]) << std::endl;
    counter++;
  }

  out << "Start address: "
      << "0x" << std::hex << std::setw(8) << std::setfill('0') << elf.get_start_address() << std::dec << std::endl;
  counter = 0;
  for (auto p : elf.program_headers) {
    out << "[" << counter << "] Program Header" << std::endl;
    out << p << std::endl;
    counter++;
  }
  return out;
}
