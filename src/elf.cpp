#include "elf.hpp"
#include "elf32.hpp"
#include "elf64.hpp"
#include "elf_common.hpp"
#include "machines.hpp"
#include "osabi.hpp"
#include "section_attribute_flags.hpp"
#include "section_types.hpp"
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


data_types_t data_types[] = {
  { "ELF32_ADDR", 4, 4 },// unsigned program address
  { "ELF32_HALF", 2, 2 },// unsigned medium integer
  { "ELF32_OFF", 4, 4 },// unsigned file offset
  { "ELF32_SWORD", 4, 4 },// signed large integer
  { "ELF32_WORD", 4, 4 },// unsigned large integer
  { "UNSIGNED_CHAR", 1, 1 }// unsigned small integer
};

// Sections can have names without a prefixed '.' these are application specific
// Sections can appear more than once with same name
// Processor specific sections are e_machine.section e.g. .EM_386.data
// @todo what are the preexisting extensions (bottom of https://www.sco.com/developers/gabi/latest/ch4.sheader.html)?
special_sections_32_t special_sections_32[] = {
  { ".bss", SHT_NOBITS, SHF_ALLOC | SHF_WRITE },// uninitisalised data, occupes no file space
  { ".comment", SHT_PROGBITS, 0x0 },// version control information
  { ".data", SHT_PROGBITS, SHF_ALLOC | SHF_WRITE },// initialised data that are part of the program
  { ".data1", SHT_PROGBITS, SHF_ALLOC | SHF_WRITE },// initialised data that are part of the program
  { ".debug", SHT_PROGBITS, 0x0 },// symbolic debugging information
  { ".dynamic", SHT_DYNAMIC, SHF_ALLOC },// dynamic linking information, SHF_WRITE depending on cpu
  { ".dynstr", SHT_STRTAB, SHF_ALLOC },// strings associated with dynamic linking
  { ".dynsym", SHT_DYNSYM, SHF_ALLOC },// symbols associated with dynamic linking
  { ".fini", SHT_PROGBITS, SHF_ALLOC | SHF_EXECINSTR },// instructions associated with termination
  { ".fini_array", SHT_FINI_ARRAY, SHF_ALLOC | SHF_WRITE },// array of function pointers that handle termination
  { ".got", SHT_PROGBITS, 0x0 },// global offset table
  { ".hash", SHT_HASH, SHF_ALLOC },// symbol hash table
  { ".init", SHT_PROGBITS, SHF_ALLOC | SHF_EXECINSTR },// instructions for starting the program
  { ".init_array", SHT_PROGBITS, SHF_ALLOC | SHF_WRITE },// array of function pointers that start the program, called before main
  { ".interp", SHT_PROGBITS, 0x0 },// holds pathname of program interpreter
  { ".line", SHT_PROGBITS, 0x0 },// line number information for symbolic debugging
  { ".note", SHT_PROGBITS, 0x0 },
  { ".plt", SHT_PROGBITS, 0x0 },// procedure linkage table
  { ".preinit_array", SHT_PROGBITS, SHF_ALLOC | SHF_WRITE },// holds function pointers that contribute to a single pre-initialisation array for the executable/shared object
  { ".rel", SHT_PROGBITS, 0x0 },// relocation information, names are .rel+name e.g. .rel.text
  { ".rela", SHT_PROGBITS, 0x0 },// relocation information, names are .rel+name e.g. .rel.text
  { ".rodata", SHT_PROGBITS, SHF_ALLOC },// readonly information for non-writable segment in the process image
  { ".rodata1", SHT_PROGBITS, SHF_ALLOC },// readonly information for non-writable segment in the process image
  { ".shstrtab", SHT_PROGBITS, 0x0 },// holds section names
  { ".strtab", SHT_PROGBITS, 0x0 },// holds strings, mostly associated with symbol table, SHF_ALLOC will be set if loadable
  { ".symtab", SHT_PROGBITS, 0x0 },// holds a symbol table, SHF_ALLOC will be set if loadable
  { ".symtab_shndx", SHT_PROGBITS, 0x0 },// special symbol table index array, SHF_ALLOC will be set if loadable
  { ".tbss", SHT_PROGBITS, SHF_ALLOC | SHF_WRITE | SHF_TLS },// uninitialised thread local data
  { ".tdata", SHT_PROGBITS, SHF_ALLOC | SHF_WRITE | SHF_TLS },// initialised thread local data
  { ".tdata1", SHT_PROGBITS, SHF_ALLOC | SHF_WRITE | SHF_TLS },// initialised thread local data
  { ".text", SHT_PROGBITS, SHF_ALLOC | SHF_EXECINSTR }// executable instructions of a program
};

std::string e_type_to_string(uint64_t value)
{
  switch (value) {
  case static_cast<uint64_t>(ET_NONE):
    return "No type (ET_NONE)";
  case static_cast<uint64_t>(ET_REL):
    return "Object file (ET_REL)";
  case static_cast<uint64_t>(ET_EXEC):
    return "Executable (ET_EXEC)";
  case static_cast<uint64_t>(ET_DYN):
    return "Dynamic Library (ET_DYN)";
  case static_cast<uint64_t>(ET_CORE):
    return "Core file (ET_CORE)";
  case static_cast<uint64_t>(ET_LOPROC):
    return "Processor Specific (ET_LOPROC)";
  case static_cast<uint64_t>(ET_HIPROC):
    return "processor specific (ET_LOPROC)";
  default:
    return "unknown";
  }
}

void write_elf_ident(container_ref bytes)
{
  bytes.push_back(ELFMAG0);
  bytes.push_back(ELFMAG1);
  bytes.push_back(ELFMAG2);
  bytes.push_back(ELFMAG3);
}

std::string elf_special_section_types_to_string(uint64_t value)
{
  switch (value) {
  case static_cast<uint64_t>(SHN_UNDEF):
    return "SHN_UNDEF";
  case static_cast<uint64_t>(SHN_LORESERVE):
    return "SHN_LORESERVE | SHN_LOPROC";
  case static_cast<uint64_t>(SHN_HIPROC):
    return "SHN_HIPROC";
  case static_cast<uint64_t>(SHN_LOOS):
    return "SHN_LOOS";
  case static_cast<uint64_t>(SHN_HIOS):
    return "SHN_HIOS";
  case static_cast<uint64_t>(SHN_ABS):
    return "SHN_ABS";
  case static_cast<uint64_t>(SHN_COMMON):
    return "SHN_COMMON";
  case static_cast<uint64_t>(SHN_XINDEX):
    return "SHN_XINDEX | SHN_HIRESERVE";
  default:
    return "INDEX";
  }
}




std::string elf_sym_binding_to_string(uint64_t value)
{
  switch (value) {
  case static_cast<uint64_t>(STB_LOCAL):
    return "STB_LOCAL";
  case static_cast<uint64_t>(STB_GLOBAL):
    return "STB_GLOBAL";
  case static_cast<uint64_t>(STB_WEAK):
    return "STB_WEAK";
  case static_cast<uint64_t>(STB_LOOS):
    return "STB_LOOS";
  case static_cast<uint64_t>(STB_HIOS):
    return "STB_HIOS";
  case static_cast<uint64_t>(STB_LOPROC):
    return "STB_LOPROC";
  case static_cast<uint64_t>(STB_HIPROC):
    return "STB_HIPROC";
  default:
    return "unknown";
  }
}

std::string elf_sym_type_to_string(uint64_t value)
{
  switch (value) {
  case static_cast<uint64_t>(STT_NOTYPE):
    return "STT_NOTYPE";
  case static_cast<uint64_t>(STT_OBJECT):
    return "STT_OBJECT";
  case static_cast<uint64_t>(STT_FUNC):
    return "STT_FUNC";
  case static_cast<uint64_t>(STT_SECTION):
    return "STT_SECTION";
  case static_cast<uint64_t>(STT_FILE):
    return "STT_FILE";
  case static_cast<uint64_t>(STT_COMMON):
    return "STT_COMMON";
  case static_cast<uint64_t>(STT_TLS):
    return "STT_TLS";
  case static_cast<uint64_t>(STT_LOOS):
    return "STT_LOOS";
  case static_cast<uint64_t>(STT_HIOS):
    return "STT_HIOS";
  case static_cast<uint64_t>(STT_LOPROC):
    return "STT_LOPROC";
  case static_cast<uint64_t>(STT_HIPROC):
    return "STT_HIPROC";

  default:
    return "unknown";
  }
}

std::string elf_sym_visibility_to_string(uint64_t value)
{
  switch (value) {
  case static_cast<uint64_t>(STV_DEFAULT):
    return "STV_DEFAULT";
  case static_cast<uint64_t>(STV_INTERNAL):
    return "STV_INTERNAL";
  case static_cast<uint64_t>(STV_HIDDEN):
    return "STV_HIDDEN";
  case static_cast<uint64_t>(STV_PROTECTED):
    return "STV_PROTECTED";

  default:
    return "unknown";
  }
}







std::string elf_program_type_to_string(ELF_ULONG type)
{
  switch (type) {
  case PT_NULL:
    return "PT_NULL";
  case PT_LOAD:
    return "PT_LOAD";
  case PT_DYNAMIC:
    return "PT_DYNAMIC";
  case PT_INTERP:
    return "PT_INTERP";
  case PT_NOTE:
    return "PT_NOTE";
  case PT_PHDR:
    return "PT_PHDR";
  case PT_TLS:
    return "PT_TLS";
  case PT_LOOS:
    return "PT_LOOS";
  case PT_HIOS:
    return "PT_HIOS";
  case PT_LOPROC:
    return "PT_LOPROC";
  case PT_HIPROC:
    return "PT_HIPROC";
  default:
    return "unknown";
  }
}


std::string elf_program_flag_to_string(ELF_ULONG type)
{
  std::string flags = "";
  if ((type & PF_R) == PF_R) {
    if (flags.empty() == false) flags += " ";
    flags += "READ";
  }
  if ((type & PF_W) == PF_W) {
    if (flags.empty() == false) flags += " ";
    flags += "WRITE";
  }
  if ((type & PF_X) == PF_X) {
    if (flags.empty() == false) flags += " ";
    flags += "EXECUTE";
  }

  if ((type & PF_MASKOS) == PF_MASKOS) {
    if (flags.empty() == false) flags += " ";
    flags += "OS_MASK";
  }
  if ((type & PF_MASKPROC) == PF_MASKPROC) {
    if (flags.empty() == false) flags += " ";
    flags += "CPU_MASK";
  }
  return flags;
}

