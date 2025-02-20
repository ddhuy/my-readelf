#include <stdio.h>
#include <unistd.h>


#include "elf.h"


int read_elf_header(const char *filename, Elf64_File *elf_file_ptr)
{
    FILE *pfile = fopen(filename, "rb");
    if (!pfile)
        return -1;

    int count = fread((void *) &elf_file_ptr->ehdr, sizeof(Elf64_Ehdr), 1, pfile);
    if (count < 0)
        return -2;

    return 0;
}


void print_elf_header(FILE *ostream, Elf64_File *elf_file_ptr)
{
    fprintf(ostream, "ELF Header:\n");
    
    // Magic
    fprintf(ostream, "  Magic:  ");
    for (int i = 0; i < EI_NIDENT; ++i)
        fprintf(ostream, " %02x", elf_file_ptr->ehdr.e_ident[i]);
    fprintf(ostream, "\n");

    // Class
    fprintf(ostream, "  Class:                             ");
    if (elf_file_ptr->ehdr.e_ident[EI_CLASS] == ELFCLASSNONE)
        fprintf(ostream, "Invalid Class\n");
    else if (elf_file_ptr->ehdr.e_ident[EI_CLASS] == ELFCLASS32)
        fprintf(ostream, "ELF32\n");
    else if (elf_file_ptr->ehdr.e_ident[EI_CLASS] == ELFCLASS64)
        fprintf(ostream, "ELF64\n");
    else
        fprintf(ostream, "Bad ELF Class!\n");

    // Data
    fprintf(ostream, "  Data:                              ");
    if (elf_file_ptr->ehdr.e_ident[EI_DATA] == ELFDATANONE)
        fprintf(ostream, "Invalid Data Encoding\n");
    else if (elf_file_ptr->ehdr.e_ident[EI_DATA] == ELFDATA2LSB)
        fprintf(ostream, "2's complement, little endian\n");
    else if (elf_file_ptr->ehdr.e_ident[EI_DATA] == ELFDATA2MSB)
        fprintf(ostream, "2's complement, big endian\n");
    else
        fprintf(ostream, "Bad ELF Data Encoding!\n");

    // Version
    fprintf(ostream, "  Version:                           ");
    if (elf_file_ptr->ehdr.e_ident[EI_VERSION] == 0x01)
        fprintf(ostream, "1 (current)\n");
    else
        fprintf(ostream, "Bad ELF Version\n");

    // OS/ABI
    fprintf(ostream, "  OS/ABI:                            ");
    if (elf_file_ptr->ehdr.e_ident[EI_OSABI] == ELFOSABI_NONE)
        fprintf(ostream, "UNIX System V ABI\n");
    else if (elf_file_ptr->ehdr.e_ident[EI_OSABI] == ELFOSABI_HPUX)
        fprintf(ostream, "HP-UX operating system\n");
    else if (elf_file_ptr->ehdr.e_ident[EI_OSABI] == ELFOSABI_NETBSD)
        fprintf(ostream, "NetBSD\n");
    else if (elf_file_ptr->ehdr.e_ident[EI_OSABI] == ELFOSABI_GNU)
        fprintf(ostream, "GNU\n");
    else if (elf_file_ptr->ehdr.e_ident[EI_OSABI] == ELFOSABI_LINUX)
        fprintf(ostream, "Alias for ELFOSABI_GNU\n");
    else if (elf_file_ptr->ehdr.e_ident[EI_OSABI] == ELFOSABI_SOLARIS)
        fprintf(ostream, "Solaris\n");
    else if (elf_file_ptr->ehdr.e_ident[EI_OSABI] == ELFOSABI_AIX)
        fprintf(ostream, "AIX\n");
    else if (elf_file_ptr->ehdr.e_ident[EI_OSABI] == ELFOSABI_IRIX)
        fprintf(ostream, "IRIX\n");
    else if (elf_file_ptr->ehdr.e_ident[EI_OSABI] == ELFOSABI_FREEBSD)
        fprintf(ostream, "FreeBSD\n");
    else if (elf_file_ptr->ehdr.e_ident[EI_OSABI] == ELFOSABI_TRU64)
        fprintf(ostream, "TRU64 UNIX\n");
    else if (elf_file_ptr->ehdr.e_ident[EI_OSABI] == ELFOSABI_MODESTO)
        fprintf(ostream, "Novell Modesto\n");
    else if (elf_file_ptr->ehdr.e_ident[EI_OSABI] == ELFOSABI_OPENBSD)
        fprintf(ostream, "OpenBSD\n");
    else if (elf_file_ptr->ehdr.e_ident[EI_OSABI] == ELFOSABI_OPENVMS)
        fprintf(ostream, "OpenVMS\n");
    else if (elf_file_ptr->ehdr.e_ident[EI_OSABI] == ELFOSABI_NSK)
        fprintf(ostream, "Hewlett-Packard Non-Stop Kernel\n");
    else if (elf_file_ptr->ehdr.e_ident[EI_OSABI] == ELFOSABI_AROS)
        fprintf(ostream, "AROS\n");
    else if (elf_file_ptr->ehdr.e_ident[EI_OSABI] == ELFOSABI_FENIXOS)
        fprintf(ostream, "FenixOS\n");
    else if (elf_file_ptr->ehdr.e_ident[EI_OSABI] == ELFOSABI_CLOUDABI)
        fprintf(ostream, "Nuxi CloudABI\n");
    else if (elf_file_ptr->ehdr.e_ident[EI_OSABI] == ELFOSABI_OPENVOS)
        fprintf(ostream, "Stratus Technologies OpenVOS\n");
    else if (elf_file_ptr->ehdr.e_ident[EI_OSABI] == ELFOSABI_CUDA)
        fprintf(ostream, "NVIDIA CUDA architecture\n");
    else if (elf_file_ptr->ehdr.e_ident[EI_OSABI] == ELFOSABI_C6000_ELFABI)
        fprintf(ostream, "Bare-metal TMS320C6000\n");
    else if (elf_file_ptr->ehdr.e_ident[EI_OSABI] == ELFOSABI_AMDGPU_HSA)
        fprintf(ostream, "AMD HSA Runtime\n");
    else if (elf_file_ptr->ehdr.e_ident[EI_OSABI] == ELFOSABI_C6000_LINUX)
        fprintf(ostream, "Linux TMS320C6000\n");
    else if (elf_file_ptr->ehdr.e_ident[EI_OSABI] == ELFOSABI_AMDGPU_PAL)
        fprintf(ostream, "AMD PAL Runtime\n");
    else if (elf_file_ptr->ehdr.e_ident[EI_OSABI] == ELFOSABI_ARM_FDPIC)
        fprintf(ostream, "ARM FDPIC\n");
    else if (elf_file_ptr->ehdr.e_ident[EI_OSABI] == ELFOSABI_AMDGPU_MESA3D)
        fprintf(ostream, "AMD Mesa3D Runtime\n");
    else if (elf_file_ptr->ehdr.e_ident[EI_OSABI] == ELFOSABI_ARM)
        fprintf(ostream, "ARM\n");
    else if (elf_file_ptr->ehdr.e_ident[EI_OSABI] == ELFOSABI_STANDALONE)
        fprintf(ostream, "Standalone (embedded) application\n");
    else
        fprintf(ostream, "Bad ELF OS/ABI\n");

    // ABI Version
    fprintf(ostream, "  ABI Version:                       %x\n", elf_file_ptr->ehdr.e_ident[EI_ABIVERSION]);

    // File type
    fprintf(ostream, "  Type:                              ");
    if (elf_file_ptr->ehdr.e_type == ET_NONE)
        fprintf(ostream, "No file type\n");
    else if (elf_file_ptr->ehdr.e_type == ET_REL)
        fprintf(ostream, "Relocatable file\n");
    else if (elf_file_ptr->ehdr.e_type == ET_EXEC)
        fprintf(ostream, "Position-dependent executable file\n");
    else if (elf_file_ptr->ehdr.e_type == ET_DYN)
        fprintf(ostream, "Position-independent executable or shared object file\n");
    else if (elf_file_ptr->ehdr.e_type == ET_CORE)
        fprintf(ostream, "Core file\n");
    else
        fprintf(ostream, "Bad ELF Type\n");

    fprintf(ostream, "  Machine:                           %d\n", elf_file_ptr->ehdr.e_machine);
    fprintf(ostream, "  Version:                           0x%x\n", elf_file_ptr->ehdr.e_version);
    fprintf(ostream, "  Entry point address:               0x%lx\n", elf_file_ptr->ehdr.e_entry);
    fprintf(ostream, "  Start of program headers:          %ld (bytes into file)\n", elf_file_ptr->ehdr.e_phoff);
    fprintf(ostream, "  Start of section headers:          %ld (bytes into file)\n", elf_file_ptr->ehdr.e_shoff);
    fprintf(ostream, "  Flags:                             0x%x\n", elf_file_ptr->ehdr.e_flags);
    fprintf(ostream, "  Size of this header:               %ld (bytes)\n", sizeof(elf_file_ptr->ehdr));
    fprintf(ostream, "  Size of program headers:           %d (bytes)\n", elf_file_ptr->ehdr.e_phentsize);
    fprintf(ostream, "  Number of program headers:         %d\n", elf_file_ptr->ehdr.e_phnum);
    fprintf(ostream, "  Size of section headers:           %d (bytes)\n", elf_file_ptr->ehdr.e_shentsize);
    fprintf(ostream, "  Number of section headers:         %d\n", elf_file_ptr->ehdr.e_shnum);
    fprintf(ostream, "  Section header string table index: %d\n", elf_file_ptr->ehdr.e_shstrndx);
    fprintf(ostream, "\n");
}