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
#include <stdbool.h>
#include <string.h>

#define MZFHEADERSIZE 128         /* Size of a .mzf file header in bytes */
#define DISPLAYLEN     16

uint8_t header[MZFHEADERSIZE];    /* Store header globally */

/* Print Sharp 'ASCII' to stdout */
void mzascii2utf8(uint8_t sharpchar)
{
  if ((sharpchar >= 0x20) && (sharpchar <= 0x5d))
    printf("%c",sharpchar);
  else
    switch(sharpchar) {

      /* Sharp lower case letters are all ok */
      /* but are not contiguous ... convert  */

      case 0xa1: printf("a"); //a
                 break;
      case 0x9a: printf("b"); //b
                 break;
      case 0x9f: printf("c"); //c
                 break;
      case 0x9c: printf("d"); //d
                 break;
      case 0x92: printf("e"); //e
                 break;
      case 0xaa: printf("f"); //f
                 break;
      case 0x97: printf("g"); //g
                 break;
      case 0x98: printf("h"); //h
                 break;
      case 0xa6: printf("i"); //i
                 break;
      case 0xaf: printf("j"); //j
                 break;
      case 0xa9: printf("k"); //k
                 break;
      case 0xb8: printf("l"); //l
                 break;
      case 0xb3: printf("m"); //m
                 break;
      case 0xb0: printf("n"); //n
                 break;
      case 0xb7: printf("o"); //o
                 break;
      case 0x9e: printf("p"); //p
                 break;
      case 0xa0: printf("q"); //q
                 break;
      case 0x9d: printf("r"); //r
                 break;
      case 0xa4: printf("s"); //s
                 break;
      case 0x96: printf("t"); //t
                 break;
      case 0xa5: printf("u"); //u
                 break;
      case 0xab: printf("v"); //v
                 break;
      case 0xa3: printf("w"); //w
                 break;
      case 0x9b: printf("x"); //x
                 break;
      case 0xbd: printf("y"); //y
                 break;
      case 0xa2: printf("z"); //z
                 break;

      /* Other stuff - mostly graphics */

      case 0x5e: printf("\u2191"); // upwards arrow 
                 break;
      case 0x5f: printf("\u2190"); // leftwards arrow
                 break;
      case 0x70: printf("\u2593"); // block dark shade (75%)
                 break;
      case 0x7b: printf("\u00b0"); // degree symbol
                 break;
      case 0x81: printf("\u253c"); // box drawing vert and horiz
                 break;
      case 0xa8: printf("\u00d6"); // O+umlaut
                 break;
      case 0xad: printf("\u00fc"); // u+umlaut 
                 break;
      case 0xae: printf("\u00df"); // eszet
                 break;
      case 0xb2: printf("\u00dc"); // U+umlaut
                 break;
      case 0xb9: printf("\u00c4"); // A+umlaut
                 break;
      case 0xba: printf("\u00f6"); // o+umlaut
                 break;
      case 0xbb: printf("\u00e4"); // a+umlaut
                 break;
      case 0xbe: printf("\u00a5"); // yen
                 break;
      case 0xc1: printf("\u2590"); // right half block;
                 break;
      case 0xc2: printf("\u2584"); // lower half block
                 break;
      case 0xc6: printf("\u2192"); // rightwards arrow
                 break;
      case 0xc8: printf("\u2588"); // full block
                 break;
      case 0xc9: printf("\u25e4"); // filled upper left triangle
                 break;
      case 0xcb: printf("\u251c"); // box drawing vertical + right horizontal
                 break;
      case 0xcc: printf("\u25d8"); // inverse bullet
                 break;
      case 0xcd: printf("\u2514"); // box drawing upper right quadrant
                 break;
      case 0xce: printf("\u2510"); // box drawing lower left quadrant
                 break;
      case 0xd0: printf("\u250c"); // box drawing lower right quadrant
                 break; 
      case 0xd1: printf("\u2534"); // box drawing horiz + upper vertical
                 break;
      case 0xd2: printf("\u252c"); // box drawing horiz + lower vertical
                 break;
      case 0xd3: printf("\u2524"); // box drawing vertical + left horizontal
                 break;
      case 0xd5: printf("\u258c"); // left half block
                 break;
      case 0xdd: printf("\u2518"); // box drawing upper left quadrant
                 break;
      case 0xde: printf("\u259e"); // quadrant upper right and lower left
                 break;
      case 0xdf: printf("\u259a"); // quadrant upper left and lower right
                 break;
      case 0xe1: printf("\u2660"); // spade
                 break;
      case 0xe9: printf("\u25e3"); // filled lower left triangle
                 break;
      case 0xee: printf("\u2572"); // box drawing ul to lr diagonal
                 break;
      case 0xef: printf("\u2571"); // box drawing ur to ll diagonal
                 break;
      case 0xf1: printf("\u25cf"); // filled circle
                 break;
      case 0xf3: printf("\u2665"); // heart
                 break;
      case 0xf5: printf("\u25e2"); // filled lower right triangle
                 break;
      case 0xf6: printf("\u2573"); // box drawing diagonal cross
                 break;
      case 0xf7: printf("\u25cb"); // circle
                 break;
      case 0xf8: printf("\u2663"); // club
                 break;
      case 0xfa: printf("\u2666"); // diamond
                 break;
      case 0xfb: printf("£");      // £
                 break;
      case 0xfc: printf("\u2193"); // downwards arrow
                 break;
      case 0xfe: printf("\u25e5"); // filled upper right triangle
                 break;
      case 0xff: printf("\u03c0"); // pi
                 break;

      /* Clear, Home, arrow keys */
      case 0x11: printf("\u2b07"); // cursor down
                 break;
      case 0x12: printf("\u2b06"); // cursor up
                 break;
      case 0x13: printf("\u2b95"); // cursor right
                 break;
      case 0x14: printf("\u2b05"); // cursor left
                 break;
      case 0x15: printf("\u24bd"); // cursor home (circled H)
                 break;
      case 0x16: printf("\u24b8"); // clear screen (circled C)
                 break;

      /* For other Sharp ASCII codes */
      default:   printf("~");
                 break;
    }

  return;
}

void print5025(uint8_t *body, uint16_t fs)
{
  bool instr=false;
  uint8_t licount=0;
  uint8_t linum[4];

  printf("\n\n");

  for (int32_t i=0;i<fs;i++) {
    if ((body[i]==0x0d)&&(licount==4)) {
      licount=0;
      instr=false;
      printf("\n");
    }
    else if (instr) {
      mzascii2utf8(body[i]);
      if (body[i]==0x22)
        instr=false;
    }
    else if (licount < 4) {
      linum[licount++]=body[i];
      if (licount == 4) {
        uint16_t linenumber=((linum[3]<<8)|linum[2])&0xffff;
        printf(" %d ",linenumber);
      }
    }
    else {
      switch(body[i]) {
      case 0x22: instr=true;
                   printf("%c",body[i]);
                   break;
        case 0x80: printf("REM");
                   break;
        case 0x81: printf("DATA");
                   break;
        case 0x82: printf("LIST");
                   break;
        case 0x83: printf("RUN");
                   break;
        case 0x84: printf("NEW");
                   break;
        case 0x85: printf("PRINT");
                   break;
        case 0x86: printf("LET");
                   break;
        case 0x87: printf("FOR");
                   break;
        case 0x88: printf("IF");
                   break;
        case 0x89: printf("GOTO");
                   break;
        case 0x8a: printf("READ");
                   break;
        case 0x8b: printf("GOSUB");
                   break;
        case 0x8c: printf("RETURN");
                   break;
        case 0x8d: printf("NEXT");
                   break;
        case 0x8e: printf("STOP");
                   break;
        case 0x8f: printf("END");
                   break;
        case 0x90: printf("ON");
                   break;
        case 0x91: printf("LOAD");
                   break;
        case 0x92: printf("SAVE");
                   break;
        case 0x93: printf("VERIFY");
                   break;
        case 0x94: printf("POKE");
                   break;
        case 0x95: printf("DIM");
                   break;
        case 0x96: printf("DEF FN");
                   break;
        case 0x97: printf("INPUT");
                   break;
        case 0x98: printf("RESTORE");
                   break;
        case 0x99: printf("CLR");
                   break;
        case 0x9a: printf("MUSIC");
                   break;
        case 0x9b: printf("TEMPO");
                   break;
        case 0x9c: printf("USR(");
                   break;
        case 0x9d: printf("WOPEN");
                   break;
        case 0x9e: printf("ROPEN");
                   break;
        case 0x9f: printf("CLOSE");
                   break;
        case 0xa0: printf("BYE");
                   break;
        case 0xa1: printf("LIMIT");
                   break;
        case 0xa2: printf("CONT");
                   break;
        case 0xa3: printf("SET");
                   break;
        case 0xa4: printf("RESET");
                   break;
        case 0xa5: printf("GET");
                   break;
        case 0xa6: printf("INP#");
                   break;
        case 0xa7: printf("OUT#");
                   break;
        case 0xad: printf("THEN");
                   break;
        case 0xae: printf("TO");
                   break;
        case 0xaf: printf("STEP");
                   break;
        case 0xb0: printf("><");
                   break;
        case 0xb1: printf("<>");
                   break;
        case 0xb2: printf("=<");
                   break;
        case 0xb3: printf("<=");
                   break;
        case 0xb4: printf("=>");
                   break;
        case 0xb5: printf(">=");
                   break;
        case 0xb6: printf("=");
                   break;
        case 0xb7: printf(">");
                   break;
        case 0xb8: printf("<");
                   break;
        case 0xb9: printf("AND");
                   break;
        case 0xba: printf("OR");
                   break;
        case 0xbb: printf("NOT");
                   break;
        case 0xbc: printf("+");
                   break;
        case 0xbd: printf("-");
                   break;
        case 0xbe: printf("*");
                   break;
        case 0xbf: printf("/");
                   break;
        case 0xc0: printf("LEFT$(");
                   break;
        case 0xc1: printf("RIGHT$(");
                   break;
        case 0xc2: printf("MID$(");
                   break;
        case 0xc3: printf("LEN(");
                   break;
        case 0xc4: printf("CHR$(");
                   break;
        case 0xc5: printf("STR$(");
                   break;
        case 0xc6: printf("ASC(");
                   break;
        case 0xc7: printf("VAL(");
                   break;
        case 0xc8: printf("PEEK(");
                   break;
        case 0xc9: printf("TAB(");
                   break;
        case 0xca: printf("SPC(");
                   break;
        case 0xcb: printf("SIZE");
                   break;
        case 0xcf: printf("\u2191"); // up arrow = exponentiation
                   break;
        case 0xd0: printf("RND(");
                   break;
        case 0xd1: printf("SIN(");
                   break;
        case 0xd2: printf("COS(");
                   break;
        case 0xd3: printf("TAN(");
                   break;
        case 0xd4: printf("ATN(");
                   break;
        case 0xd5: printf("EXP(");
                   break;
        case 0xd6: printf("INT(");
                   break;
        case 0xd7: printf("LOG(");
                   break;
        case 0xd8: printf("LN(");
                   break;
        case 0xd9: printf("ABS(");
                   break;
        case 0xda: printf("SGN(");
                   break;
        case 0xdb: printf("SQR(");
                   break;
        default:   mzascii2utf8(body[i]);
                   break;
      }
    }
  }

  printf("\n");
}

uint16_t process_mzf_header(FILE *fp, char *mzf)
{
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
    mzascii2utf8(header[i]);
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

  printf("\nFile body in hexadecimal and UTF-8\n");
  printf("---------------------------------\n\n");
  for (i=0;i<fs;i++)
    body[i]=getc(fp);

  for (i=0;i<fs;i++) {
    printf("%02x ",body[i]);
    if ((i+1)%DISPLAYLEN==0) {
      printf("    ");
      for (uint8_t j=DISPLAYLEN;j>0;j--) 
        mzascii2utf8(body[(i-j)+1]);
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
      mzascii2utf8(body[j]);
  }

  /* Convert BASIC tokens and print file again if the file type is 0x02 */
  if (header[0]==0x02)
    print5025(body,fs);

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
