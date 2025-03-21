#include <stdarg.h>
#include <unistd.h>

#include "elf.h"


int main(int argc, char **argv)
{
    Elf64_File elf_file;
    
    int ret = read_elf_file(argv[1], &elf_file);
    if (!ret) {
        print_elf_header(stdout, &elf_file);
        print_program_headers(stdout, &elf_file);
        print_section_headers(stdout, &elf_file);
        print_symbol_table(stdout, &elf_file);
    }

    return close_elf_file(&elf_file);
}