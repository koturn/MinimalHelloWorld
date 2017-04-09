const char main[] =
  /* mov $0x01,%rax */
  "\x48\xc7\xc0\x01\x00\x00\x00"
  /* mov $0x0d,%edx */
  "\xba\x0d\x00\x00\x00"
  /* mov $0x01,%edi */
  "\xbf\x01\x00\x00\x00"
  /* callq */
  "\xe8\x0c\x00\x00\x00"
  /* add $0x0f,%rsi */
  "\x48\x81\xc6\x11\x00\x00\x00"
  /* syscall */
  "\x0f\x05"
  /* xor %eax,%eax */
  "\x31\xc0"
  /* ret */
  "\xc3"
  /* mov (%rsp),%rsi */
  "\x48\x8b\x34\x24"
  /* ret */
  "\xc3"
  /* String data */
  "Hello World!\n";
