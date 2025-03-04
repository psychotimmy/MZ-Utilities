# MZ-Utilities
Utility programs to help with Sharp MZ series emulators and preservation activities.

To compile: cc -o \<executable name\> \<source\>.c -lm

**cgromchars \<Sharp MZ series CGROM file\>** - Print all of the 256 display characters in a 2K Sharp MZ series CGROM file.

**mzfview \<mzf file name\>** - Examine the header and body of a mzf/m12/mzt digital tape file from a Sharp MZ series computer. If the tape is MZ-80K SP-5025 BASIC, MZ-80A SA-5510 BASIC or MZ-700 S-BASIC, display a listing as well as the hex bytes. Needs the mz-ascii true type font installing and active in your shell to work correctly.
