#include "Elf_Phdr.hpp"
#include "elf.hpp"

std::ostream &operator<<(std::ostream &out, Elf_Phdr const &header)
{
  out << "Type: " << elf_program_type_to_string(header.p_type) << std::endl;
  out << "Offset: " << std::hex << "0x" << header.p_offset << std::dec << std::endl;
  out << "Virtual Address: " << std::hex << "0x" << header.p_vaddr << std::dec << std::endl;
  out << "Physical Address: " << std::hex << "0x" << header.p_paddr << std::dec << std::endl;
  out << "File size: " << std::hex << "0x" << header.p_filesz << std::dec << std::endl;
  out << "Memory size: " << std::hex << "0x" << header.p_memsz << std::dec << std::endl;
  out << "Flags: " << std::hex << "0x" << header.p_flags << std::dec << std::endl;
  if (header.p_flags > 0) {
    out << elf_program_flag_to_string(header.p_flags) << std::endl;
  }
  out << "Alignment: " << std::hex << "0x" << header.p_align << std::dec << std::endl;
  return out;
}
