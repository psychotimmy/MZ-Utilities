/**************************************************/
/* cgromchars.c                                   */
/*                                                */
/* Utility to show each of the display characters */
/* in a Sharp MZ series 2K character graphics ROM */
/*                                                */
/* Tim Holyoake, 21st February 2025.              */
/* MIT licence - see end of file for details.     */
/**************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>

#define CROMSIZE 	2048
#define CHRBYTES	   8
#define CHRWIDTH           8

int main(int argc, char **argv) 
{

  FILE *fp;
  uint16_t i,j,k;
  uint8_t cgrom[CROMSIZE];

  /* Check we have one and only one argument */
  if (argc != 2) {
    fprintf(stderr,"Usage: %s <Sharp MZ CGROM file>\n",argv[0]);
    exit(1);
  }

  /* Open file passed in as argument argv[1] if it exists and is readable */
  fp = fopen(argv[1], "r");
  if (fp == NULL) {
    fprintf(stderr,"Error: %s not found\n",argv[1]);
    exit(1);
  }

  /*Read and store the CGROM */
  for (i=0; i<CROMSIZE; i++)
    cgrom[i]=getc(fp);

  /* Print the 8x8 character layout */
  for (i=0; i<CROMSIZE/CHRBYTES; i++) {
    printf("Sharp MZ display character %d\n\n",i);
    for (j=0; j<CHRBYTES; j++) {
      printf("Row %d is %02x  ",j,cgrom[i*CHRBYTES+j]);
      for (k=0; k<CHRWIDTH; k++) {
        if ((((cgrom[i*CHRBYTES+j]<<k)&0x80)>>7)&0x01==1) 
          printf("X");
        else 
          printf(".");
      }
      printf("\n");
    }
    printf("\n");
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
