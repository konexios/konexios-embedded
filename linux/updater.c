/* Copyright (c) 2017 Arrow Electronics, Inc.
 * All rights reserved. This program and the accompanying materials
 * are made available under the terms of the Apache License 2.0
 * which accompanies this distribution, and is available at
 * http://apache.org/licenses/LICENSE-2.0
 * Contributors: Arrow Electronics, Inc.
 */

#include <stdio.h>
#include <unistd.h>

#if !defined(EXE_FILE)
#define EXE_FILE "konexios.bin"
#endif

int main(int argc, char* argv[]) {
  if ( argc < 2 ) return -1;

  FILE *exe_file, *up_file;
  char buffer[512];
  exe_file=fopen(EXE_FILE,"wb");
  if (!exe_file) {
    printf("Unable to open exe file!\r\n");
    return 1;
  }
  up_file = fopen(argv[1], "rb");
  
  int len = 0;
  int ret = 0;
  do {
    len = fread(buffer, 1, sizeof(buffer), up_file);
//    printf("read %d\r\n", len);
    if ( len > 0 ) {
      ret = fwrite(buffer, 1, len,  exe_file);
//      printf("write %d\r\n", ret);
      if ( ret < 0 ) return -1;
    } 
  } while(len > 0);

  fclose(up_file);
  fclose(exe_file);
  unlink(argv[1]);

  printf("Reboot...\r\n");
  char *arg[] = { "./" EXE_FILE }; 
  return execvp(*arg, arg);
}
