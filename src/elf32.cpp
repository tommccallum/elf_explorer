#include "elf32.hpp"
#include "elf_common.hpp"

/**
     * @brief Get the section table entry index 0 object
     * The first section header entry 0 exists even though the index marks undefined section references.
     * 
     * @return struct Elf32_Shdr 
     */
struct Elf32_Shdr get_section_table_entry_index_0()
{
  struct Elf32_Shdr header;
  header.sh_name = 0;
  header.sh_type = SHT_NULL;
  header.sh_flags = 0;
  header.sh_addr = 0;
  header.sh_offset = 0;
  header.sh_size = 0;
  header.sh_link = SHN_UNDEF;
  header.sh_info = 0;
  header.sh_addralign = 0;
  header.sh_entsize = 0;
  return header;
}
