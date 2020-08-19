#include "Elf_Shdr.hpp"


bool Elf_Shdr::is_writable() const
{
  return (sh_flags & SHF_WRITE) == SHF_WRITE;
}
bool Elf_Shdr::is_allocated() const
{
  return (sh_flags & SHF_ALLOC) == SHF_ALLOC;
}
bool Elf_Shdr::is_executable() const
{
  return (sh_flags & SHF_EXECINSTR) == SHF_EXECINSTR;
}
bool Elf_Shdr::is_merged() const
{
  return (sh_flags & SHF_MERGE) == SHF_EXECINSTR;
}
bool Elf_Shdr::is_strings() const
{
  return (sh_flags & SHF_STRINGS) == SHF_EXECINSTR;
}
bool Elf_Shdr::is_info_link() const
{
  return (sh_flags & SHF_INFO_LINK) == SHF_EXECINSTR;
}
bool Elf_Shdr::is_link_order() const
{
  return (sh_flags & SHF_LINK_ORDER) == SHF_EXECINSTR;
}
bool Elf_Shdr::is_os_nonconforming() const
{
  return (sh_flags & SHF_OS_NONCONFORMING) == SHF_EXECINSTR;
}
bool Elf_Shdr::is_grouped() const
{
  return (sh_flags & SHF_GROUP) == SHF_EXECINSTR;
}
bool Elf_Shdr::is_tls() const
{
  return (sh_flags & SHF_TLS) == SHF_EXECINSTR;
}
bool Elf_Shdr::is_compressed() const
{
  return (sh_flags & SHF_COMPRESSED) == SHF_EXECINSTR;
}
bool Elf_Shdr::has_os_flags() const
{
  return (sh_flags & SHF_MASKOS) != 0;
}
bool Elf_Shdr::has_cpu_flags() const
{
  return (sh_flags & SHF_MASKPROC) != 0;
}
uint64_t Elf_Shdr::get_associated_string_table() const
{
  if (sh_type == SHT_SYMTAB || sh_type == SHT_DYNSYM || sh_type == SHT_DYNAMIC) {
    return sh_link;
  }
  return 0;
}
uint64_t Elf_Shdr::get_associated_symbol_table() const
{
  if (sh_type == SHT_REL || sh_type == SHT_RELA || sh_type == SHT_GROUP || sh_type == SHT_SYMTAB_SHNDX || sh_type == SHT_HASH) {
    return sh_link;
  }
  return 0;
}
uint64_t Elf_Shdr::get_index_for_relocation() const
{
  if (sh_type == SHT_REL || sh_type == SHT_RELA) {
    return sh_info;
  }
  return 0;
}
uint64_t Elf_Shdr::get_index_last_local_symbol() const
{
  if (sh_type == SHT_SYMTAB || sh_type == SHT_DYNSYM) {
    return sh_info;
  }
  return 0;
}
uint64_t Elf_Shdr::get_index_of_symbol_table() const
{
  if (sh_type == SHT_GROUP) {
    return sh_info;
  }
  return 0;
}


std::ostream &operator<<(std::ostream &out, Elf_Shdr const &header)
{
  out << "[" << header.index << "] " << e_section_types_to_string(header.sh_type) << " Name: " << header.name << "\n";
  out << "  "
      << "flags:" << std::hex << "0x" << header.sh_addr << std::dec << " ";
  out << (header.is_writable() ? "WRITE " : "");
  out << (header.is_allocated() ? "ALLOC " : "");
  out << (header.is_executable() ? "EXEC " : "");
  out << (header.is_merged() ? "MERGE " : "");
  out << (header.is_strings() ? "STRINGS " : "");
  out << (header.is_info_link() ? "INFO " : "");
  out << (header.is_link_order() ? "LINK " : "");
  out << (header.is_os_nonconforming() ? "OS_NONCONFORMING" : "");
  out << (header.is_grouped() ? "GROUPED " : "");
  out << (header.is_tls() ? "THREAD " : "");
  out << (header.is_compressed() ? "COMPRESS " : "");
  out << (header.has_os_flags() ? "OS_MASK " : "");
  out << (header.has_cpu_flags() ? "CPU_MASK " : "");
  out << "\n";
  out << "  ";
  out << "mem_addr:" << std::hex << "0x" << header.sh_addr << std::dec << " ";
  out << "offset:" << std::hex << "0x" << header.sh_offset << std::dec << " ";
  out << "size:" << std::hex << "0x" << header.sh_size << std::dec << " ";
  out << "\n";
  out << "  ";
  out << "link:" << std::hex << "0x" << header.sh_link << std::dec << " ";
  out << "info:" << std::hex << "0x" << header.sh_info << std::dec << "\n";
  if (header.get_associated_string_table() != 0) {
    out << "  ";
    out << "  ";
    out << "String table: " << header.get_associated_string_table() << std::endl;
  }
  if (header.get_associated_symbol_table() != 0) {
    out << "  ";
    out << "  ";
    out << "Symbol table: " << header.get_associated_symbol_table() << std::endl;
  }
  if (header.get_index_for_relocation() != 0) {
    out << "  ";
    out << "  ";
    out << "Table index that relocation applies to: " << header.get_index_for_relocation() << std::endl;
  }
  if (header.get_index_last_local_symbol() != 0) {
    out << "  ";
    out << "  ";
    out << "Table index of last local symbol+1: " << header.get_index_last_local_symbol() << std::endl;
  }
  if (header.get_index_of_symbol_table() != 0) {
    out << "  ";
    out << "  ";
    out << "Table index of an entry in the associated symbol table: " << header.get_index_of_symbol_table() << std::endl;
  }
  out << "  ";
  out << "addralign:" << std::hex << "0x" << header.sh_addralign << std::dec << " ";
  out << "entry size:" << std::hex << "0x" << header.sh_entsize << std::dec << " ";
  return out;
}
