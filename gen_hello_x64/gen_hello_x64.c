#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <elf.h>

#define BASE_ADDR    0x08000000
#define N_PROGRAM_HEADER  1
#define N_SECTION_HEADER  0
#define HEADER_SIZE  (sizeof(Elf64_Ehdr) + sizeof(Elf64_Phdr) * N_PROGRAM_HEADER)


int
main(void)
{
  unsigned int str_addr = BASE_ADDR + HEADER_SIZE + 37;
  Elf64_Ehdr ehdr;
  Elf64_Phdr phdr;
  char code[] =
    /* mov $0x01,%rax  # System call number (write) */
    "\x48\xc7\xc0\x01\x00\x00\x00"
    /* mov $0x0d,%edx  # Third argument */
    "\xba\x0d\x00\x00\x00"
    /* mov $0x********,%rsi  # Second argument */
    "\x48\xc7\xc6\x00\x00\x00\x00"
    /* mov $0x01,%edi  # First argument */
    "\xbf\x01\x00\x00\x00"
    /* syscall */
    "\x0f\x05"
    /* mov $0x3c,%rax  # System call number (exit) */
    "\x48\xc7\xc0\x3c\x00\x00\x00"
    /* xor %edi,%edi  # First argument */
    "\x31\xff"
    /* syscall */
    "\x0f\x05"
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
  ehdr.e_ident[EI_CLASS] = ELFCLASS64;
  ehdr.e_ident[EI_DATA] = ELFDATA2LSB;
  ehdr.e_ident[EI_VERSION] = EV_CURRENT;
  ehdr.e_ident[EI_OSABI] = ELFOSABI_LINUX;
  ehdr.e_ident[EI_ABIVERSION] = 0x00;
  ehdr.e_ident[EI_PAD] = 0x00;
  ehdr.e_type = ET_EXEC;
  ehdr.e_machine = EM_X86_64;
  ehdr.e_version = EV_CURRENT;
  ehdr.e_entry = BASE_ADDR + HEADER_SIZE;
  ehdr.e_phoff = sizeof(Elf64_Ehdr);
  ehdr.e_shoff = 0;
  ehdr.e_flags = 0x00000000;
  ehdr.e_ehsize = sizeof(Elf64_Ehdr);
  ehdr.e_phentsize = sizeof(Elf64_Phdr);
  ehdr.e_phnum = N_PROGRAM_HEADER;
  ehdr.e_shentsize = 0;
  ehdr.e_shnum = N_SECTION_HEADER;
  ehdr.e_shstrndx = SHN_UNDEF;
  fwrite(&ehdr, sizeof(ehdr), 1, f);

  /* Program header */
  phdr.p_type = PT_LOAD;
  phdr.p_flags = PF_R | PF_X;
  phdr.p_offset = 0x000000000000000000;
  phdr.p_vaddr = BASE_ADDR;
  phdr.p_paddr = BASE_ADDR;
  phdr.p_filesz = HEADER_SIZE + sizeof(code);
  phdr.p_memsz = HEADER_SIZE + sizeof(code);
  phdr.p_align = 0x0000000000000100;
  fwrite(&phdr, sizeof(phdr), 1, f);

  /* Put string data address into code */
  memcpy(code + 15, &str_addr, sizeof(str_addr));
  /* Write .text section */
  fwrite(code, 1, sizeof(code), f);

  fclose(f);
  f = NULL;

  /* Show file size */
  printf("Size of a.out: %lu bytes\n", HEADER_SIZE + sizeof(code));
  fflush(stdout);

  /* Give execution permission */
  chmod("a.out", 0755);
  /* Execute created binary */
  system("./a.out");

  return EXIT_SUCCESS;
}

