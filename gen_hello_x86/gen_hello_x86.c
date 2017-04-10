#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <elf.h>

#define BASE_ADDR    0x08000000
#define N_PROGRAM_HEADER  1
#define N_SECTION_HEADER  0
#define HEADER_SIZE  (sizeof(Elf32_Ehdr) + sizeof(Elf32_Phdr) * N_PROGRAM_HEADER)


int
main(void)
{
  unsigned int str_addr = BASE_ADDR + HEADER_SIZE + 30;
  Elf32_Ehdr ehdr;
  Elf32_Phdr phdr;
  char code[] =
    /* mov $0x04,%eax */
    "\xb8\x04\x00\x00\x00"
    /* mov $0x0d,%edx */
    "\xba\x0d\x00\x00\x00"
    /* mov $0x********,%ecx */
    "\xb9\x00\x00\x00\x00"
    /* mov $0x01,%ebx */
    "\xbb\x01\x00\x00\x00"
    /* int 0x80 */
    "\xcd\x80"
    /* mov 0x04,%eax */
    "\xb8\x01\x00\x00\x00"
    /* dec %ebx */
    "\x4b"
    /* int 0x80 */
    "\xcd\x80"
    /* String data */
    "Hello World!\n";
  FILE *f = fopen("a.out", "wb");
  if (f == NULL) {
    fputs("Failed to open a.out\n", stderr);
    return EXIT_FAILURE;
  }

  /* ELF header */
  ehdr.e_ident[EI_MAG0] = ELFMAG0;
  ehdr.e_ident[EI_MAG1] = ELFMAG1;
  ehdr.e_ident[EI_MAG2] = ELFMAG2;
  ehdr.e_ident[EI_MAG3] = ELFMAG3;
  ehdr.e_ident[EI_CLASS] = ELFCLASS32;
  ehdr.e_ident[EI_DATA] = ELFDATA2LSB;
  ehdr.e_ident[EI_VERSION] = EV_CURRENT;
  ehdr.e_ident[EI_OSABI] = ELFOSABI_LINUX;
  ehdr.e_ident[EI_ABIVERSION] = 0x00;
  ehdr.e_ident[EI_PAD] = 0x00;
  ehdr.e_type = ET_EXEC;
  ehdr.e_machine = EM_386;
  ehdr.e_version = EV_CURRENT;
  ehdr.e_entry = BASE_ADDR + HEADER_SIZE;
  ehdr.e_phoff = sizeof(Elf32_Ehdr);
  ehdr.e_shoff = 0;
  ehdr.e_flags = 0x00000000;
  ehdr.e_ehsize = sizeof(Elf32_Ehdr);
  ehdr.e_phentsize = sizeof(Elf32_Phdr);
  ehdr.e_phnum = N_PROGRAM_HEADER;
  ehdr.e_shentsize = 0;
  ehdr.e_shnum = N_SECTION_HEADER;
  ehdr.e_shstrndx = SHN_UNDEF;
  fwrite(&ehdr, sizeof(ehdr), 1, f);

  /* Program header */
  phdr.p_type = PT_LOAD;
  phdr.p_flags = PF_R | PF_X;
  phdr.p_offset = 0x00000000;
  phdr.p_vaddr = BASE_ADDR;
  phdr.p_paddr = BASE_ADDR;
  phdr.p_filesz = HEADER_SIZE + sizeof(code);
  phdr.p_memsz = HEADER_SIZE + sizeof(code);
  phdr.p_align = 0x00000100;
  fwrite(&phdr, sizeof(phdr), 1, f);

  /* Put string data address into code */
  memcpy(code + 11, &str_addr, sizeof(str_addr));
  /* Write .text section */
  fwrite(code, 1, sizeof(code), f);

  fclose(f);
  f = NULL;

  /* Show file size */
  printf("Size of a.out: %u bytes\n", HEADER_SIZE + sizeof(code));
  fflush(stdout);

  /* Give execution permission */
  chmod("a.out", 0755);
  /* Execute created binary */
  system("./a.out");

  return EXIT_SUCCESS;
}
