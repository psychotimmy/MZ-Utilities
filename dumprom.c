/**************************************************/
/* dumprom.c                                      */
/*                                                */
/* Utility to dump a Sharp MZ series ROM to       */
/* stdout for use in emulators.                   */
/*                                                */
/* Tim Holyoake, 19th October 2025.               */
/* MIT licence - see end of file for details.     */
/**************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>

#define DUMPWIDTH          8

int main(int argc, char **argv) 
{

  FILE *fp;
  uint8_t i;

  /* Check we have one and only one argument */
  if (argc != 2) {
    fprintf(stderr,"Usage: %s <Sharp MZ ROM file\n",argv[0]);
    exit(1);
  }

  /* Open file passed in as argument argv[1] if it exists and is readable */
  fp = fopen(argv[1], "r");
  if (fp == NULL) {
    fprintf(stderr,"Error: %s not found\n",argv[1]);
    exit(1);
  }

  /* Dump the ROM to stdout in lines of length DUMPWIDTH */
  /* Note last character dumped has a trailing comma!    */
  i=1;
  while (!feof(fp)) {
    char ch = fgetc(fp);
    if (i<DUMPWIDTH) {
      if (!feof(fp))
        printf ("0x%02x,",ch);
      ++i;
    }
    else {
      if (!feof(fp))
        printf ("0x%02x,\n",ch);
      i=1;
    }
  }

  return(0);
}

//MIT License

//Copyright (c) 2025 Tim Holyoake

//Permission is hereby granted, free of charge, to any person obtaining a copy
//of this software and associated documentation files (the "Software"), to deal
//in the Software without restriction, including without limitation the rights
//to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
//copies of the Software, and to permit persons to whom the Software is
//furnished to do so, subject to the following conditions:

//The above copyright notice and this permission notice shall be included in all
//copies or substantial portions of the Software.

//THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
//IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
//FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
//AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
//LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
//OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
//SOFTWARE.
