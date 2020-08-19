add_executable(elf
    main.cpp
    Elf_Phdr.cpp
    Elf_Shdr.cpp
    Elf_Sym.cpp
    elf.cpp
    elf32.cpp
    elf64.cpp
    ElfReader.cpp
    StringTable.cpp
    SectionTableInfo.cpp
    SymbolTable.cpp
    machines.cpp
    section_attribute_flags.cpp
    section_types.cpp
)
target_link_libraries(elf PRIVATE project_options)