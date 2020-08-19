#include "Elf_Sym.hpp"
#include "elf.hpp"

bool Elf_Sym::is_in_symtab_shndx() const
{
  return st_shndx == SHN_XINDEX;
}


std::ostream &operator<<(std::ostream &out, Elf_Sym const &header)
{
  out << "[" << header.name << "] value:";
  out << std::hex << "0x" << header.st_value << std::dec << " ";
  if (header.st_shndx == SHN_COMMON) {
    out << "ALIGNMENT_CONSTRAINT ";
  }
  if (header.file_type == ET_REL) {
    out << "DEFINED_SYMBOL_OFFSET ";
  }
  if (header.file_type == ET_EXEC || header.file_type == ET_DYN) {
    out << "VIRTUAL_ADDRESS ";
  }
  out << "\n";
  out << "       ";
  out << "size: " << std::hex << "0x" << header.st_size << std::dec << "\n";
  out << "       ";
  out << "info: " << std::hex << "0x" << static_cast<uint64_t>(header.st_info) << std::dec << " ";
  out << "Bind: " << elf_sym_binding_to_string(ELF64_ST_BIND(header.st_info)) << " ";
  out << "Type: " << elf_sym_type_to_string(ELF64_ST_TYPE(header.st_info)) << "\n";
  out << "       ";
  out << "visibility: " << elf_sym_visibility_to_string(header.st_other) << " (" << std::hex << "0x" << static_cast<uint64_t>(header.st_other) << std::dec << ")\n";
  out << "       ";
  out << "section: " << header.st_shndx << " " << elf_special_section_types_to_string(header.st_shndx) << " SHN_XINDEX: " << (header.is_in_symtab_shndx() == true ? "true" : "false");
  return out;
}
