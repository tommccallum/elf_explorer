!#/bin/bash

#https://cirosantilli.com/elf-hello-world#minimal-elf-file

nasm -w+all -f elf64 -o 'hello_world.o' 'hello_world.asm'
ld -o 'hello_world.out' 'hello_world.o'