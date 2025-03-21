/****
 *
 */
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <string.h>
#include <unistd.h>

#include "elf.h"

/****
 *
 */
#define EID_OK           0
#define EID_SYS_ERROR    errno
#define EID_NO_EHDR      -1
#define EID_NO_PHDR      -2
#define EID_NO_SHDR      -3


/****
 *
 */
static void trim_n_format(FILE *ostream, char *src, char *dst, int maxlen)
{
    int len = strlen(src);

    if (len > maxlen) {
        strncpy(dst, src, maxlen - 4);
        strcpy(dst + maxlen - 4, "[...]");
    } else {
        strcpy(dst, src);
    }
}


/****
 *
 */
int read_elf_file(const char *filename, Elf64_File *elf_file)
{
    memset(elf_file, 0, sizeof(Elf64_File));

    elf_file->fd = open(filename, O_RDONLY);
    if (elf_file->fd < 0) 
        return EID_SYS_ERROR;

    int count = read(elf_file->fd, (void *) &elf_file->ehdr, sizeof(Elf64_Ehdr));
    if (count < 0)
        return EID_SYS_ERROR;
    
    return EID_OK;
}


/****
 *
 */
int close_elf_file(Elf64_File *elf_file)
{
    if (elf_file->shstrtab) {
        free(elf_file->shstrtab);
        elf_file->shstrtab = NULL;
    }

    if (elf_file->ph_table) {
        free(elf_file->ph_table);
        elf_file->ph_table = NULL;
    }
    
    if (elf_file->sh_table) {
        free(elf_file->sh_table);
        elf_file->sh_table = NULL;
    }
    
    close(elf_file->fd);
    
    return EID_OK;
}


/****
 *
 */
int print_elf_header(FILE *ostream, Elf64_File *elf_file)
{
    fprintf(ostream, "ELF Header:\n");

    // Magic
    fprintf(ostream, "  Magic:  ");
    for (int i = 0; i < EI_NIDENT; ++i)
        fprintf(ostream, " %02x", elf_file->ehdr.e_ident[i]);
    fprintf(ostream, "\n");

    // Class
    fprintf(ostream, "  Class:                             ");
    if (elf_file->ehdr.e_ident[EI_CLASS] == ELFCLASSNONE)
        fprintf(ostream, "Invalid Class\n");
    else if (elf_file->ehdr.e_ident[EI_CLASS] == ELFCLASS32)
        fprintf(ostream, "ELF32\n");
    else if (elf_file->ehdr.e_ident[EI_CLASS] == ELFCLASS64)
        fprintf(ostream, "ELF64\n");
    else
        fprintf(ostream, "Bad ELF Class!\n");

    // Data
    fprintf(ostream, "  Data:                              ");
    if (elf_file->ehdr.e_ident[EI_DATA] == ELFDATANONE)
        fprintf(ostream, "Invalid Data Encoding\n");
    else if (elf_file->ehdr.e_ident[EI_DATA] == ELFDATA2LSB)
        fprintf(ostream, "2's complement, little endian\n");
    else if (elf_file->ehdr.e_ident[EI_DATA] == ELFDATA2MSB)
        fprintf(ostream, "2's complement, big endian\n");
    else
        fprintf(ostream, "Bad ELF Data Encoding!\n");

    // Version
    fprintf(ostream, "  Version:                           ");
    if (elf_file->ehdr.e_ident[EI_VERSION] == 0x01)
        fprintf(ostream, "1 (current)\n");
    else
        fprintf(ostream, "Bad ELF Version\n");

    // OS/ABI
    fprintf(ostream, "  OS/ABI:                            ");
    if (elf_file->ehdr.e_ident[EI_OSABI] == ELFOSABI_NONE)
        fprintf(ostream, "UNIX System V ABI\n");
    else if (elf_file->ehdr.e_ident[EI_OSABI] == ELFOSABI_HPUX)
        fprintf(ostream, "HP-UX operating system\n");
    else if (elf_file->ehdr.e_ident[EI_OSABI] == ELFOSABI_NETBSD)
        fprintf(ostream, "NetBSD\n");
    else if (elf_file->ehdr.e_ident[EI_OSABI] == ELFOSABI_GNU)
        fprintf(ostream, "GNU\n");
    else if (elf_file->ehdr.e_ident[EI_OSABI] == ELFOSABI_LINUX)
        fprintf(ostream, "Alias for ELFOSABI_GNU\n");
    else if (elf_file->ehdr.e_ident[EI_OSABI] == ELFOSABI_SOLARIS)
        fprintf(ostream, "Solaris\n");
    else if (elf_file->ehdr.e_ident[EI_OSABI] == ELFOSABI_AIX)
        fprintf(ostream, "AIX\n");
    else if (elf_file->ehdr.e_ident[EI_OSABI] == ELFOSABI_IRIX)
        fprintf(ostream, "IRIX\n");
    else if (elf_file->ehdr.e_ident[EI_OSABI] == ELFOSABI_FREEBSD)
        fprintf(ostream, "FreeBSD\n");
    else if (elf_file->ehdr.e_ident[EI_OSABI] == ELFOSABI_TRU64)
        fprintf(ostream, "TRU64 UNIX\n");
    else if (elf_file->ehdr.e_ident[EI_OSABI] == ELFOSABI_MODESTO)
        fprintf(ostream, "Novell Modesto\n");
    else if (elf_file->ehdr.e_ident[EI_OSABI] == ELFOSABI_OPENBSD)
        fprintf(ostream, "OpenBSD\n");
    else if (elf_file->ehdr.e_ident[EI_OSABI] == ELFOSABI_OPENVMS)
        fprintf(ostream, "OpenVMS\n");
    else if (elf_file->ehdr.e_ident[EI_OSABI] == ELFOSABI_NSK)
        fprintf(ostream, "Hewlett-Packard Non-Stop Kernel\n");
    else if (elf_file->ehdr.e_ident[EI_OSABI] == ELFOSABI_AROS)
        fprintf(ostream, "AROS\n");
    else if (elf_file->ehdr.e_ident[EI_OSABI] == ELFOSABI_FENIXOS)
        fprintf(ostream, "FenixOS\n");
    else if (elf_file->ehdr.e_ident[EI_OSABI] == ELFOSABI_CLOUDABI)
        fprintf(ostream, "Nuxi CloudABI\n");
    else if (elf_file->ehdr.e_ident[EI_OSABI] == ELFOSABI_OPENVOS)
        fprintf(ostream, "Stratus Technologies OpenVOS\n");
    else if (elf_file->ehdr.e_ident[EI_OSABI] == ELFOSABI_CUDA)
        fprintf(ostream, "NVIDIA CUDA architecture\n");
    else if (elf_file->ehdr.e_ident[EI_OSABI] == ELFOSABI_C6000_ELFABI)
        fprintf(ostream, "Bare-metal TMS320C6000\n");
    else if (elf_file->ehdr.e_ident[EI_OSABI] == ELFOSABI_AMDGPU_HSA)
        fprintf(ostream, "AMD HSA Runtime\n");
    else if (elf_file->ehdr.e_ident[EI_OSABI] == ELFOSABI_C6000_LINUX)
        fprintf(ostream, "Linux TMS320C6000\n");
    else if (elf_file->ehdr.e_ident[EI_OSABI] == ELFOSABI_AMDGPU_PAL)
        fprintf(ostream, "AMD PAL Runtime\n");
    else if (elf_file->ehdr.e_ident[EI_OSABI] == ELFOSABI_ARM_FDPIC)
        fprintf(ostream, "ARM FDPIC\n");
    else if (elf_file->ehdr.e_ident[EI_OSABI] == ELFOSABI_AMDGPU_MESA3D)
        fprintf(ostream, "AMD Mesa3D Runtime\n");
    else if (elf_file->ehdr.e_ident[EI_OSABI] == ELFOSABI_ARM)
        fprintf(ostream, "ARM\n");
    else if (elf_file->ehdr.e_ident[EI_OSABI] == ELFOSABI_STANDALONE)
        fprintf(ostream, "Standalone (embedded) application\n");
    else
        fprintf(ostream, "Bad ELF OS/ABI\n");

    // ABI Version
    fprintf(ostream, "  ABI Version:                       %x\n", elf_file->ehdr.e_ident[EI_ABIVERSION]);

    // File type
    fprintf(ostream, "  Type:                              ");
    if (elf_file->ehdr.e_type == ET_NONE)
        fprintf(ostream, "No file type\n");
    else if (elf_file->ehdr.e_type == ET_REL)
        fprintf(ostream, "Relocatable file\n");
    else if (elf_file->ehdr.e_type == ET_EXEC)
        fprintf(ostream, "Position-dependent executable file\n");
    else if (elf_file->ehdr.e_type == ET_DYN)
        fprintf(ostream, "Position-independent executable or shared object file\n");
    else if (elf_file->ehdr.e_type == ET_CORE)
        fprintf(ostream, "Core file\n");
    else
        fprintf(ostream, "Bad ELF Type\n");

    fprintf(ostream, "  Machine:                           %d\n", elf_file->ehdr.e_machine);
    fprintf(ostream, "  Version:                           0x%x\n", elf_file->ehdr.e_version);
    fprintf(ostream, "  Entry point address:               0x%lx\n", elf_file->ehdr.e_entry);
    fprintf(ostream, "  Start of program headers:          %ld (bytes into file)\n", elf_file->ehdr.e_phoff);
    fprintf(ostream, "  Start of section headers:          %ld (bytes into file)\n", elf_file->ehdr.e_shoff);
    fprintf(ostream, "  Flags:                             0x%x\n", elf_file->ehdr.e_flags);
    fprintf(ostream, "  Size of this header:               %ld (bytes)\n", sizeof(elf_file->ehdr));
    fprintf(ostream, "  Size of program headers:           %d (bytes)\n", elf_file->ehdr.e_phentsize);
    fprintf(ostream, "  Number of program headers:         %d\n", elf_file->ehdr.e_phnum);
    fprintf(ostream, "  Size of section headers:           %d (bytes)\n", elf_file->ehdr.e_shentsize);
    fprintf(ostream, "  Number of section headers:         %d\n", elf_file->ehdr.e_shnum);
    fprintf(ostream, "  Section header string table index: %d\n", elf_file->ehdr.e_shstrndx);
    fprintf(ostream, "\n");

    return EID_OK;
}


/****
 *
 */
int print_program_headers(FILE *ostream, Elf64_File *elf_file)
{
    int count = -1;

    // Program headers
    if (elf_file->ehdr.e_phnum == 0 || elf_file->ehdr.e_phoff == 0)
        return EID_NO_PHDR;
    
    elf_file->ph_table = (Elf64_Phdr *) calloc(elf_file->ehdr.e_phnum, sizeof(Elf64_Phdr));
    Elf64_Phdr *ph_table = elf_file->ph_table;
    if (elf_file->ph_table == NULL)
        return EID_SYS_ERROR;
    
    lseek(elf_file->fd, elf_file->ehdr.e_phoff, SEEK_SET);
    count = read(elf_file->fd, (void *) elf_file->ph_table, sizeof(Elf64_Phdr) * elf_file->ehdr.e_phnum);
    if (count < 0)
        return EID_SYS_ERROR;

    // Section headers
    if (elf_file->ehdr.e_shnum == 0 || elf_file->ehdr.e_shnum == 0)
        return EID_NO_SHDR;

    elf_file->sh_table = (Elf64_Shdr *) calloc(elf_file->ehdr.e_shnum, sizeof(Elf64_Shdr));
    Elf64_Shdr *sh_table = elf_file->sh_table;
    if (elf_file->sh_table == NULL)
        return EID_SYS_ERROR;

    lseek(elf_file->fd, elf_file->ehdr.e_shoff, SEEK_SET);
    count = read(elf_file->fd, (void *) elf_file->sh_table, sizeof(Elf64_Shdr) * elf_file->ehdr.e_shnum);
    if (count < 0)
        return EID_SYS_ERROR;

    // String table
    elf_file->shstrtab = calloc(1, sh_table[elf_file->ehdr.e_shstrndx].sh_size);
    char *shstrtab = elf_file->shstrtab;
    if (elf_file->shstrtab == NULL)
        return EID_SYS_ERROR;
    
    lseek(elf_file->fd, sh_table[elf_file->ehdr.e_shstrndx].sh_offset, SEEK_SET);
    count = read(elf_file->fd, (void *) elf_file->shstrtab, sh_table[elf_file->ehdr.e_shstrndx].sh_size);
    if (count < 0)
        return EID_SYS_ERROR;

    fprintf(ostream, "Program Headers:\n"
                                    "  Type           Offset             VirtAddr           PhysAddr\n"
                                    "                 FileSiz            MemSiz              Flags  Align\n");
    for (int i = 0; i < elf_file->ehdr.e_phnum; ++i) {
        fprintf(ostream, "  %-15d", elf_file->ph_table[i].p_type);
        fprintf(ostream, "0x%016lx ", elf_file->ph_table[i].p_offset);
        fprintf(ostream, "0x%016lx ", elf_file->ph_table[i].p_vaddr);
        fprintf(ostream, "0x%016lx\n", elf_file->ph_table[i].p_paddr);
        fprintf(ostream, "                 ");
        fprintf(ostream, "0x%016lx ", elf_file->ph_table[i].p_filesz);
        fprintf(ostream, "0x%016lx ", elf_file->ph_table[i].p_memsz);
        fprintf(ostream, " 0x%04x", elf_file->ph_table[i].p_flags);
        fprintf(ostream, " 0x%lx\n", elf_file->ph_table[i].p_align);
    }


    fprintf(ostream, " Section to Segment mapping:\n");
    fprintf(ostream, "  Segment Sections...\n");
    for (int i = 0; i < elf_file->ehdr.e_phnum; ++i) {
        fprintf(ostream, "   %02d     ", i);
        for (int j = 0; j < elf_file->ehdr.e_shnum; ++j) {
            // fprintf(ostream, "0x%016lx 0x%016lx\n", ph_table[i].p_offset, ph_table[i].p_filesz);
            // fprintf(ostream, "0x%016lx 0x%016lx\n", sh_table[j].sh_offset, sh_table[j].sh_size);
            if (sh_table[j].sh_type != SHT_NULL &&
                sh_table[j].sh_offset >= ph_table[i].p_offset &&
                (sh_table[j].sh_offset + sh_table[j].sh_size) <= (ph_table[i].p_offset + ph_table[i].p_filesz)) {
                    fprintf(ostream, "%s ", &shstrtab[sh_table[j].sh_name]);
            }
        }
        fprintf(ostream, "\n");
    }
    fprintf(ostream, "\n");

    return EID_OK;
}


/****
 *
 */
 int print_section_headers(FILE *ostream, Elf64_File *elf_file)
 {
    char tmp_str[18] = { 0 };
    char *shstrtab = elf_file->shstrtab;
    Elf64_Shdr *sh_table = elf_file->sh_table;

    fprintf(ostream, "There are %d section headers, starting at offset 0x%lx:\n\n",
        elf_file->ehdr.e_shnum,
        elf_file->ehdr.e_shoff);
    fprintf(ostream, "Section Headers:\n"
                                    "  [Nr] Name              Type             Address           Offset\n"
                                    "       Size              EntSize          Flags  Link  Info  Align\n");
  
    for (int i = 0; i < elf_file->ehdr.e_shnum; ++i) {
        memset(tmp_str, 0, sizeof(tmp_str));
        trim_n_format(ostream, &shstrtab[sh_table[i].sh_name], tmp_str, 16);
        fprintf(ostream, "  [%2d] %-17s %08x         %016lx  %08lx\n", i, tmp_str,
            sh_table[i].sh_type, sh_table[i].sh_addr, sh_table[i].sh_offset);
        fprintf(ostream, "       %016lx  %016lx   %-4ld %4d  %4d  %4ld\n",
            sh_table[i].sh_size, sh_table[i].sh_entsize, sh_table[i].sh_flags,
            sh_table[i].sh_link, sh_table[i].sh_info, sh_table[i].sh_addralign);
    }
    
    return EID_OK;
 }