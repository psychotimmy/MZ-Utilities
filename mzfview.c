/***********************************************/
/* mzfview - a program to view Sharp MZ series */
/*           digital (mzf,m12,mzt) tape files  */
/*                                             */
/*           (c) Tim Holyoake, February 2025   */
/*                                             */
/***********************************************/
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

#define MZFHEADERSIZE 128         /* Size of a .mzf file header in bytes */
#define DISPLAYLEN     16

/* Convert Sharp 'ASCII' to a printable 'ASCII' character */
/* Incomplete coverage, but good enough for most purposes */
uint8_t mzascii2ascii(uint8_t sharpchar)
{
  uint8_t asciichar=0x2d; /* Default anything not in if or switch to a dash */

  /* ! " # $ % & ' ( ) 0-9 and A-Z are ok, contiguous & have a true ASCII value */
  if ((sharpchar >= 0x20) && (sharpchar <= 0x5a))
    asciichar=sharpchar;
  else
    switch(sharpchar) {

      /* Sharp lower case letters are all ok */
      /* but are not contiguous ... convert  */

      case 0xa1: asciichar=0x61; //a
                 break;
      case 0x9a: asciichar=0x62; //b
                 break;
      case 0x9f: asciichar=0x63; //c
                 break;
      case 0x9c: asciichar=0x64; //d
                 break;
      case 0x92: asciichar=0x65; //e
                 break;
      case 0xaa: asciichar=0x66; //f
                 break;
      case 0x97: asciichar=0x67; //g
                 break;
      case 0x98: asciichar=0x68; //h
                 break;
      case 0xa6: asciichar=0x69; //i
                 break;
      case 0xaf: asciichar=0x6a; //j
                 break;
      case 0xa9: asciichar=0x6b; //k
                 break;
      case 0xb8: asciichar=0x6c; //l
                 break;
      case 0xb3: asciichar=0x6d; //m
                 break;
      case 0xb0: asciichar=0x6e; //n
                 break;
      case 0xb7: asciichar=0x6f; //o
                 break;
      case 0x9e: asciichar=0x70; //p
                 break;
      case 0xa0: asciichar=0x71; //q
                 break;
      case 0x9d: asciichar=0x72; //r
                 break;
      case 0xa4: asciichar=0x73; //s
                 break;
      case 0x96: asciichar=0x74; //t
                 break;
      case 0xa5: asciichar=0x75; //u
                 break;
      case 0xab: asciichar=0x76; //v
                 break;
      case 0xa3: asciichar=0x77; //w
                 break;
      case 0x9b: asciichar=0x78; //x
                 break;
      case 0xbd: asciichar=0x79; //y
                 break;
      case 0xa2: asciichar=0x7a; //z
                 break;

    }

  return(asciichar);
}

uint16_t process_mzf_header(FILE *fp, char *mzf)
{
  uint8_t header[MZFHEADERSIZE];
  uint16_t i;

  /* Read and store tape header */
  for (i=0; i<MZFHEADERSIZE; i++)
    header[i]=getc(fp);

  printf("\nTape header information for %s\n",mzf);
  printf("============================");
  for (i=0;i<strlen(mzf);i++)
    printf("=");
  printf("\n\nFile type: 0x%02x",header[0]);
  switch (header[0]) {
    case 0x01: printf(" - machine code\n");
               break;
    case 0x02: printf(" - MZ-80 BASIC or other high level language\n");
               break;
    case 0x03: printf(" - MZ-80 data file\n");
               break;
    case 0x04: printf(" - MZ-700 data file\n");
               break;
    case 0x05: printf(" - MZ-700 BASIC or other high level language\n");
               break;
    case 0x06: printf(" - Chalkwell 3K BASIC\n");
               break;
    default:   printf(" - unknown file type\n");
               break;
  }

  i=1;
  printf("File name: ");
  while ((header[i] != 0x0d) && (i<18)) {
    printf("%c",mzascii2ascii(header[i]));
    ++i;
  }

  printf("\nFile size: ");
  i=((header[19]<<8)&0xff00)|header[18];
  printf("0x%04x (%d) bytes\n",i,i);

  printf("Load addr: ");
  i=((header[21]<<8)&0xff00)|header[20];
  printf("0x%04x (%d)\n",i,i);

  printf("Exec addr: ");
  i=((header[23]<<8)&0xff00)|header[22];
  printf("0x%04x (%d)\n",i,i);

  printf("\nFull 128 byte header in hexadecimal\n");
  printf("-----------------------------------\n\n");
  for (i=0;i<MZFHEADERSIZE;i++) {
    printf("%02x ",header[i]);
    if ((i+1)%DISPLAYLEN==0)
      printf("\n");
  }

  printf("\n");
  return(((header[19]<<8)&0xff00)|header[18]);
}

void process_mzf_body(FILE *fp, uint16_t fs)
{
  int32_t i;
  uint8_t body[fs];

  printf("\nFile body in hexadecimal and ASCII\n");
  printf("----------------------------------\n\n");
  for (i=0;i<fs;i++)
    body[i]=getc(fp);

  for (i=0;i<fs;i++) {
    printf("%02x ",body[i]);
    if ((i+1)%DISPLAYLEN==0) {
      printf("    ");
      for (uint8_t j=DISPLAYLEN;j>0;j--) 
        printf("%c",mzascii2ascii(body[(i-j)+1]));
      printf("\n");
    }
  }

  if (fs%DISPLAYLEN!=0) {
    int32_t j=i;
    while ((j++)%DISPLAYLEN!=0) 
      printf("   ");
    while (i%DISPLAYLEN!=0)
      --i;
    printf("    ");
    for (j=i;j<fs;j++)
      printf("%c",mzascii2ascii(body[j]));
  }

  printf("\n");
  return;
}

int main(int argc, char **argv)
{

  FILE *fp;
  uint16_t filesize;

  /* Check we have one and only one argument */
  if (argc !=2) {
    fprintf(stderr,"Usage: %s <mzf file>\n",argv[0]);
    exit(1);
  }

  printf("%s %s\n",argv[0],argv[1]);

  /* Open file passed in as argument argv[1] if it exists and is readable */
  fp = fopen(argv[1], "r");
  if (fp == NULL) {
    fprintf(stderr,"Error: %s not found\n",argv[1]);
    exit(1);
  }

  /* Read contents of file header and process it */
  filesize=process_mzf_header(fp,argv[1]);

  /* Read contents of file body and process it */
  process_mzf_body(fp,filesize);

  /* Tidy up */
  fclose(fp);

  return (0);
}
