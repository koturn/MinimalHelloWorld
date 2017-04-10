const char main[] =
  /* mov $0x04,%eax */
  "\xb8\x04\x00\x00\x00"
  /* mov $0x0d,%edx */
  "\xba\x0d\x00\x00\x00"
  /* mov $0x01,%ebx */
  "\xbb\x01\x00\x00\x00"
  /* call 0x08 */
  "\xe8\x08\x00\x00\x00"
  /* add $0x0c,%ecx */
  "\x83\xc1\x0c"
  /* int 0x80 */
  "\xcd\x80"
  /* xor %eax,%eax */
  "\x31\xc0"
  /* ret */
  "\xc3"
  /* mov (%esp),%ecx */
  "\x8b\x0c\x24"
  /* ret */
  "\xc3"
  /* String data */
  "Hello World!\n";
