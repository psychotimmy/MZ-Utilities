/***************************************************/
/* mzfview.c                                       */
/*                                                 */
/* Utility to view Sharp MZ series digital tape    */
/* files (mzf, m12, mzt).                          */
/*                                                 */
/* Relies on the mz-ascii.ttf being active in the  */
/* terminal running the program.                   */
/*                                                 */
/* (c) Tim Holyoake, February-March 2025.          */
/*                                                 */
/* MIT licence - see end of this file for details. */
/*                                                 */
/***************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include <locale.h>
#include <wchar.h>
#include <math.h>

#define MZFHEADERSIZE 128      // Size of a .mzf file header in bytes
#define DISPLAYLEN     16      // Number of bytes to display per hex row

#define MZ80K 1                // Code numbers used for different MZ
#define MZ80A 2                // series machine types
#define MZ700 3

uint8_t header[MZFHEADERSIZE]; // Store header as a global variable
uint8_t mzmc;                  // MZ machine type

/* Print Sharp 'ASCII' to stdout. Requires mz-ascii.ttf to be active */
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

      /* Other stuff is in the Unicode private area 1 at E000 onwards   */
      /* and in private area 2 at F000 onwards for variants from MZ-80K */
      /* ASCII table on the MZ80A and MZ-700 */
      /* Done to make the default printf statement easy ...             */

      default:   wchar_t tstr;
                 if (mzmc == MZ80A) {
                   if ((sharpchar == 0x80) ||
                       (sharpchar == 0x8b) ||
                       (sharpchar == 0x90) ||
                       (sharpchar == 0x93) ||
                       (sharpchar == 0x94) ||
                       (sharpchar == 0xbe)) 
                     tstr=0xF000+sharpchar;
                    else
                     tstr=0xE000+sharpchar;
                 } else if (mzmc == MZ700) {
                   if ((sharpchar == 0x6c) ||
                       (sharpchar == 0x7f) ||
                       (sharpchar == 0x80) ||
                       (sharpchar == 0x8b) ||
                       (sharpchar == 0x90) ||
                       (sharpchar == 0x93) ||
                       (sharpchar == 0x94) ||
                       (sharpchar == 0xbe)) 
                     tstr=0xF000+sharpchar;
                    else
                     tstr=0xE000+sharpchar;
                 } else
                     tstr=0xE000+sharpchar;
                 printf("%lc",tstr);
                 break;
    }

  return;
}

void print5025(uint8_t *body, uint16_t fs)
{
  bool instr=false;
  bool inrem=false;
  uint8_t licount=0;
  uint8_t linum[4];

  printf("\n\n");

  for (int32_t i=0;i<fs;i++) {
    /* BASIC SP-5025 lines are terminated by 0x0d */
    if ((body[i]==0x0d)&&(licount==4)) {
      licount=0;
      instr=false;
      inrem=false;
      printf("\n");
    }
    else if (instr) {
      mzascii2utf8(body[i]);
      if (body[i]==0x22)
        instr=false;
    }
    else if (inrem) {
      mzascii2utf8(body[i]);
      if (body[i]==0x3a)      // :
        inrem=false;
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
                   inrem=true;
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
        case 0xcf: printf("\ue05e"); // up arrow = exponentiation
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
        default:   /* Not a token - use as a literal value */
                   mzascii2utf8(body[i]);
                   break;
      }
    }
  }

  printf("\n");
}

void print5510(uint8_t *body, uint16_t fs)
{
  bool instr=false;
  bool inrem=false;
  uint8_t licount=0;
  uint8_t linum[4];

  printf("\n\n");

  for (int32_t i=0;i<fs;i++) {
    /* BASIC SA-5510 lines are terminated by 0x0d */
    if ((body[i]==0x0d)&&(licount==4)) {
      licount=0;
      instr=false;
      inrem=false;
      printf("\n");
    }
    else if (instr) {
      mzascii2utf8(body[i]);
      if (body[i]==0x22)
        instr=false;
    }
    else if (inrem) {
      mzascii2utf8(body[i]);
      if (body[i]==0x3a)      // :
        inrem=false;
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
        case 0x80: /* Two byte token found */
                   switch(body[++i]) {
                     case 0x80: printf("REM");
                                inrem=true;
                                break;
                     case 0x81: printf("DATA");
                                break;
                     case 0x84: printf("READ");
                                break;
                     case 0x85: printf("LIST");
                                break;
                     case 0x86: printf("RUN");
                                break;
                     case 0x87: printf("NEW");
                                break;
                     case 0x88: printf("PRINT");
                                break;
                     case 0x89: printf("LET");
                                break;
                     case 0x8a: printf("FOR");
                                break;
                     case 0x8b: printf("IF");
                                break;
                     case 0x8c: printf("THEN");
                                break;
                     case 0x8d: printf("GOTO");
                                break;
                     case 0x8e: printf("GOSUB");
                                break;
                     case 0x8f: printf("RETURN");
                                break;
                     case 0x90: printf("NEXT");
                                break;
                     case 0x91: printf("STOP");
                                break;
                     case 0x92: printf("END");
                                break;
                     case 0x94: printf("ON");
                                break;
                     case 0x95: printf("LOAD");
                                break;
                     case 0x96: printf("SAVE");
                                break;
                     case 0x97: printf("VERIFY");
                                break;
                     case 0x98: printf("POKE");
                                break;
                     case 0x99: printf("DIM");
                                break;
                     case 0x9a: printf("DEF FN");
                                break;
                     case 0x9b: printf("INPUT");
                                break;
                     case 0x9c: printf("RESTORE");
                                break;
                     case 0x9d: printf("CLR");
                                break;
                     case 0x9e: printf("MUSIC");
                                break;
                     case 0x9f: printf("TEMPO");
                                break;
                     case 0xa0: printf("USR(");
                                break;
                     case 0xa1: printf("WOPEN");
                                break;
                     case 0xa2: printf("ROPEN");
                                break;
                     case 0xa3: printf("CLOSE");
                                break;
                     case 0xa4: printf("MON");
                                break;
                     case 0xa5: printf("LIMIT");
                                break;
                     case 0xa6: printf("CONT");
                                break;
                     case 0xa7: printf("GET");
                                break;
                     case 0xa8: printf("INP@");
                                break;
                     case 0xa9: printf("OUT@");
                                break;
                     case 0xaa: printf("CURSOR");
                                break;
                     case 0xab: printf("SET");
                                break;
                     case 0xac: printf("RESET");
                                break;
                     case 0xb3: printf("AUTO");
                                break;
                     case 0xb6: printf("COPY/P");
                                break;
                     case 0xb7: printf("PAGE/P");
                                break;
                     default:   break;
                   }
                   break;
                   /* Single byte tokens */
        case 0x22: instr=true;
                   printf("%c",body[i]);
                   break;
        case 0x2a: printf("*");
                   break;
        case 0x2b: printf("+");
                   break;
        case 0x2d: printf("-");
                   break;
        case 0x2f: printf("/");
                   break;
        case 0x5e: printf("\ue05e"); // up arrow = exponentiation
                   break;
        case 0x83: printf("><");
                   break;
        case 0x84: printf("<>");
                   break;
        case 0x85: printf("=<");
                   break;
        case 0x86: printf("<=");
                   break;
        case 0x87: printf("=>");
                   break;
        case 0x88: printf(">=");
                   break;
        case 0x89: printf("=");
                   break;
        case 0x8a: printf(">");
                   break;
        case 0x8b: printf("<");
                   break;
        case 0x9e: printf("TO");
                   break;
        case 0x9f: printf("STEP");
                   break;
        case 0xa0: printf("LEFT$(");
                   break;
        case 0xa1: printf("RIGHT$(");
                   break;
        case 0xa2: printf("MID$(");
                   break;
        case 0xa3: printf("LEN(");
                   break;
        case 0xa4: printf("CHR$");
                   break;
        case 0xa5: printf("STR$(");
                   break;
        case 0xa6: printf("ASC(");
                   break;
        case 0xa7: printf("VAL(");
                   break;
        case 0xa8: printf("PEEK(");
                   break;
        case 0xa9: printf("TAB(");
                   break;
        case 0xaa: printf("SPACE$(");
                   break;
        case 0xab: printf("SIZE");
                   break;
        case 0xaf: printf("STRING$(");
                   break;
        case 0xb1: printf("CHARACTER$(");
                   break;
        case 0xb2: printf("CSR");
                   break;
        case 0xc0: printf("RND(");
                   break;
        case 0xc1: printf("SIN(");
                   break;
        case 0xc2: printf("COS(");
                   break;
        case 0xc3: printf("TAN(");
                   break;
        case 0xc4: printf("ATN(");
                   break;
        case 0xc5: printf("EXP(");
                   break;
        case 0xc6: printf("INT(");
                   break;
        case 0xc7: printf("LOG(");
                   break;
        case 0xc8: printf("LN(");
                   break;
        case 0xc9: printf("ABS(");
                   break;
        case 0xca: printf("SGN(");
                   break;
        case 0xcb: printf("SQR(");
                   break;
        default:   /* Not a token - use as a literal value */
                   mzascii2utf8(body[i]);
                   break;
      }
    }
  }

  printf("\n");
}

void printsbasic(uint8_t *body, uint16_t fs)
{
  bool instr=false;
  bool inrem=false;
  uint8_t licount=0;
  uint8_t linum[4];

  printf("\n\n");

  for (int32_t i=0;i<fs;i++) {
    /* S-BASIC lines are terminated by 0x00 */
    if ((body[i]==0x00)&&(licount==4)) {
      licount=0;
      instr=false;
      inrem=false;
      printf("\n");
    }
    else if (instr) {
      mzascii2utf8(body[i]);
      if (body[i]==0x22)
        instr=false;
    }
    else if (inrem) {
      mzascii2utf8(body[i]);
      if (body[i]==0x3a)      // :
        inrem=false;
    }
    else if (licount < 4) {
      linum[licount++]=body[i];
      if (licount == 4) {
        uint16_t linenumber=((linum[3]<<8)|linum[2])&0xffff;
        printf(" %d ",linenumber);
      }
    }
    else {
      uint8_t leng;
      switch(body[i]) {
        case 0x03: /* String variable - length of name in next byte */
                   leng=body[++i];
                   /* Output string variable name */
                   for (uint8_t j=0;j<leng;j++)
                      printf("%c",body[++i]);
                   /* Output a $ symbol */
                   printf("$");
                   break;
        case 0x05: /* Numeric variable - length of name in next byte */
                   leng=body[++i];
                   int exponent, mantissa;
                   /* Output numeric variable name */
                   for (uint8_t j=0;j<leng;j++)
                      printf("%c",body[++i]);
        case 0x15: /* Next byte is exponent plus exponent's sign - base 2 */
                   /* If this is 0x00, then value of the number is 0 */
                   if (body[++i] == 0x00)
                     exponent=0;
                   else
                     exponent=body[i]-0x80;
                   /* Next 4 bytes is mantissa plus mantissa's sign - base 2 */
                   float value=0.5;
                   bool positivemantissa=true;
                   uint8_t bits=body[++i];
                   /* Deal with the sign of the mantissa in msb */
                   if (bits & 0x80)
                     positivemantissa=false;
                   bits<<=1;
                   /* Deal with the remaining 7 bits */
                   for (uint8_t k=2;k<=8;k++) {
                     if (bits & 0x80)
                       value += powf(2,-k);
                     bits<<=1;
                   }
                   /* Deal with the remaining 3 bytes */
                   for (uint8_t j=1;j<=3;j++) {
                     bits=body[++i];
                     for (uint8_t k=1;k<=8;k++) {
                       if (bits & 0x80)
                         value += powf(2,-(j*8+k));
                       bits<<=1;
                     }
                   }
                   /* Multiply by the exponent and print if not 0 */
                   if (exponent == 0)     // S-BASIC 0 indicator
                     printf("0");
                   else {
                     if (positivemantissa)
                       value *= powf(2,exponent);
                     else
                       value *= powf(2,-1*exponent);
                     printf("%g",value);  // %g removes trailing zeros
                   }                      // after the decimal point
                   break;
        case 0x11: /* Hex value */
                   if (body[i+2] != 0x00)
                     printf("$%02X%02X",body[i+2],body[i+1]);
                   else
                     printf("$%X",body[i+1]);
                   i+=2;
                   break;
        case 0x0b: /* GOTO or GOSUB line number held in next 2 bytes */
                   printf("%d",((body[i+2]<<8)|body[i+1]))&0xffff;
                   i+=2;
                   break;
        case 0x22: instr=true;
                   printf("%c",body[i]);
                   break;
        case 0x8b: printf("AUTO");
                   break;
        case 0xb3: printf("AXIS");
                   break;
        case 0xc4: printf("BYE");
                   break;
        case 0xbb: printf("CIRCLE");
                   break;
        case 0xcf: printf("CLOSE");
                   break;
        case 0x9b: printf("CLS");
                   break;
        case 0x9a: printf("CONT");
                   break;
        case 0xb8: printf("CONSOLE");
                   break;
        case 0x94: printf("DATA");
                   break;
        case 0xc7: printf("DEF");
                   break;
        case 0x89: printf("DELETE");
                   break;
        case 0x96: printf("DIM");
                   break;
        case 0x98: printf("END");
                   break;
        case 0xc0: printf("ERASE");
                   break;
        case 0xc1: printf("ERROR");
                   break;
        case 0x8d: printf("FOR");
                   break;
        case 0xad: printf("GET");
                   break;
        case 0x81: printf("GOSUB");
                   break;
        case 0x80: printf("GOTO");
                   break;
        case 0xb1: printf("GPRINT");
                   break;
        case 0xb0: printf("HSET");
                   break;
        case 0x93: printf("IF");
                   break;
        case 0x91: printf("INPUT");
                   break;
        case 0xab: printf("INP#");
                   break;
        case 0xb2: printf("KEY");
                   break;
        case 0xd9: printf("KILL");
                   break;
        case 0x9e: printf("LET");
                   break;
        case 0xa5: printf("LINE");
                   break;
        case 0x87: printf("LIST");
                   break;
        case 0xb4: printf("LOAD");
                   break;
        case 0xb6: printf("MERGE");
                   break;
        case 0xa2: printf("MODE");
                   break;
        case 0xa7: printf("MOVE");
                   break;
        case 0x9f: printf("NEW");
                   break;
        case 0x8e: printf("NEXT");
                   break;
        case 0xa1: printf("OFF");
                   break;
        case 0x9d: printf("ON");
                   break;
        case 0xba: printf("OUT#");
                   break;
        case 0xbd: printf("PAGE");
                   break;
        case 0xae: printf("PCOLOR");
                   break;
        case 0xaf: printf("PHOME");
                   break;
        case 0xa4: printf("PLOT");
                   break;
        case 0xa0: printf("POKE");
                   break;
        case 0x8f: printf("PRINT");
                   break;
        case 0x95: printf("READ");
                   break;
        case 0x97: printf("REM");
                   inrem=true;
                   break;
        case 0x8a: printf("RENUM");
                   break;
        case 0x85: printf("RESTORE");
                   break;
        case 0x86: printf("RESUME");
                   break;
        case 0x84: printf("RETURN");
                   break;
        case 0xa6: printf("RLINE");
                   break;
        case 0xa8: printf("RMOVE");
                   break;
        case 0xd0: printf("ROPEN");
                   break;
        case 0x83: printf("RUN");
                   break;
        case 0xb5: printf("SAVE");
                   break;
        case 0xa3: printf("SKIP");
                   break;
        case 0x99: printf("STOP");
                   break;
        case 0xbc: printf("TEST");
                   break;
        case 0xaa: printf("TROFF");
                   break;
        case 0xa9: printf("TRON");
                   break;
        case 0xc3: printf("USR");
                   break;
        case 0xce: printf("WOPEN");
                   break;
        case 0xec: printf("AND");
                   break;
        case 0xeb: printf("OR");
                   break;
        case 0xe7: printf("SPC");
                   break;
        case 0xe1: printf("STEP");
                   break;
        case 0xe6: printf("TAB");
                   break;
        case 0xe2: printf("THEN");
                   break;
        case 0xe0: printf("TO");
                   break;
        case 0xe3: printf("USING");
                   break;
        case 0xd2: printf("\ue0ff"); // pi
                   break;
        case 0xee: printf("><");
                   break;
        case 0xef: printf("<>");
                   break;
        case 0xf0: printf("=<");
                   break;
        case 0xf1: printf("<=");
                   break;
        case 0xf2: printf("=>");
                   break;
        case 0xf3: printf(">=");
                   break;
        case 0xf4: printf("=");
                   break;
        case 0xf5: printf(">");
                   break;
        case 0xf6: printf("<");
                   break;
        case 0xf7: printf("+");
                   break;
        case 0xf8: printf("-");
                   break;
        case 0xfb: printf("/");
                   break;
        case 0xfc: printf("*");
                   break;
        case 0xfd: printf("\ue05e"); // up arrow = exponentiation
                   break;
        case 0xfe: switch(body[++i]) {
                     case 0xae: printf("BOOT");
                                break;
                     case 0xa6: printf("CLR");
                                break;
                     case 0x83: printf("COLOR");
                                break;
                     case 0xa4: printf("CURSOR");
                                break;
                     case 0xa7: printf("LIMIT");
                                break;
                     case 0xa2: printf("MUSIC");
                                break;
                     case 0x82: printf("RESET");
                                break;
                     case 0x81: printf("SET");
                                break;
                     case 0xa3: printf("TEMPO");
                                break;
                     case 0xa5: printf("VERIFY");
                                break;
                     default:   printf("UNKNOWN FE TOKEN");
                                break;
                   }
                   break;
        case 0xff: switch(body[++i]) {
                     case 0x81: printf("ABS");
                                break;
                     case 0xab: printf("ASC");
                                break;
                     case 0x8a: printf("ATN");
                                break;
                     case 0xa0: printf("CHR$");
                                break;
                     case 0x83: printf("COS");
                                break;
                     case 0x86: printf("EXP");
                                break;
                     case 0xc7: printf("FN");
                                break;
                     case 0xa2: printf("HEX$");
                                break;
                     case 0x80: printf("INT");
                                break;
                     case 0x9e: printf("JOY");
                                break;
                     case 0xba: printf("LEFT$");
                                break;
                     case 0xac: printf("LEN");
                                break;
                     case 0x85: printf("LN");
                                break;
                     case 0x8c: printf("LOG");
                                break;
                     case 0xbc: printf("MID$");
                                break;
                     case 0x8e: printf("PAI");
                                break;
                     case 0x89: printf("PEEK");
                                break;
                     case 0x8f: printf("RAD");
                                break;
                     case 0xbb: printf("RIGHT$");
                                break;
                     case 0x88: printf("RND");
                                break;
                     case 0x8b: printf("SGN");
                                break;
                     case 0x82: printf("SIN");
                                break;
                     case 0xb5: printf("SIZE");
                                break;
                     case 0x87: printf("SQR");
                                break;
                     case 0xc3: printf("STRING$");
                                break;
                     case 0x84: printf("TAN");
                                break;
                     case 0xad: printf("VAL");
                                break;
                     case 0x95: printf("EOF");
                                break;
                     case 0xb4: printf("ERL");
                                break;
                     case 0xb3: printf("ERN");
                                break;
                     case 0xc4: printf("TI$");
                                break;
                     default:   printf("UNKNOWN FF TOKEN");
                                break;
                   }
                   break;
        default:   /* Not a token - use as a literal value */
                   mzascii2utf8(body[i]);
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

  /* Convert SP-BASIC tokens and print file again if the file type is 0x02 */
  /* SP-5025 BASIC programs are loaded from 0x4806 onwards                 */
  if ((header[0]==0x02) && (header[20]==0x06) && (header[21]==0x48)) {
    mzmc=MZ80K;
    print5025(body,fs);
  }

  /* Convert SA-BASIC tokens and print file again if the file type is 0x02 */
  /* SA-5510 BASIC programs are loaded from 0x505C onwards                 */
  else if ((header[0]==0x02) && (header[20]==0x5C) && (header[21]==0x50)) {
    mzmc=MZ80A;
    print5510(body,fs);
  }

  else if ((header[0]==0x02))
    printf("\n\nUnable to determine BASIC (?) type from file header\n");

  /* Convert S-BASIC tokens and print file again if the file type is 0x05 */
  else if (header[0]==0x05) {
    mzmc=MZ700;
    printsbasic(body,fs);
  }

  printf("\n");
  return;
}

int main(int argc, char **argv)
{

  FILE *fp;
  uint16_t filesize;

  /* Set locale */
  setlocale(LC_CTYPE, "");

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
