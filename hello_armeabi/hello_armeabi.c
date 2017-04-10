const char main[] =
  /* mov r7, #4 */
  "\x04\x70\xa0\xe3"
  /* mov r2, #1 */
  "\x0d\x20\xa0\xe3"
  /* mov r1, pc */
  "\x0f\x10\xa0\xe1"
  /* add r1, r1, #12 */
  "\x10\x10\x81\xe2"
  /* mov r0, #1 */
  "\x01\x00\xa0\xe3"
  /* svc 0x00000000 */
  "\x00\x00\x00\xef"
  /* mov r0, #0*/
  "\x00\x00\xa0\xe3"
  /* bx lr */
  "\x1e\xff\x2f\xe1"
  /* String data */
  "Hello World!\n";
