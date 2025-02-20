#include <stdarg.h>
#include <stdlib.h>
#include <unistd.h>

#include "elf.h"

int main(int argc, char **argv)
{
  Elf64_File elf_file;

  int ret = read_elf_header(argv[1], &elf_file);
  if (!ret) {
    print_elf_header(stdout, &elf_file);
  }

  return EXIT_SUCCESS;
}