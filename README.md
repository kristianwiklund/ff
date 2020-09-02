Forth with virtual machine.

Originally written in 1993. Be afraid :-)

--

Things that used to work in 1993 but do not work now:


* Critical:
  * t->doesl = (ULONG) t->does = 0; // this doesn't compile with gcc7, had to be changed to two lines. It assigns zero to both doesl and does.
  * The software compiles (with the assignment change above) on both Linux and Openindiana (Solaris), but *segfaults on Linux*. It *works perfectly on Openindiana*.
    * The cause of the error is that something bad happens from the strtok on line 53 in loadcore.c and onwards when encountering the line "inline + 0x05 0x1c 0x05 0x1f 0x06 ;" which is the first "real" line in the core definitions.
    * For some reason, the strtok breaks on Linux. --> Fix: #include <string.h>, not needed on solaris


* Non-critical:
  * There is a crapload of compiler warnings. It still builds.
  * "12 emit" clears the screen, it used to do a newline
  * I use cpp to preprocess the core definitions. "Modern" cpp pukes at some of the text
  
