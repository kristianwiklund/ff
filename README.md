Forth with virtual machine.

Originally written in 1993. Be afraid :-)

--

Things that used to work in 1993 that I had to fix now:


* Critical:
  * t->doesl = (ULONG) t->does = 0; // this doesn't compile with gcc7, had to be changed to two lines. It assigns zero to both doesl and does.
  * The software compiles (with the assignment change above) on both Linux and Openindiana (Solaris), but *segfaults on Linux*. It *works perfectly on Openindiana*.
    * The cause of the error is that something bad happens from the strtok on line 53 in loadcore.c and onwards when encountering the line "inline + 0x05 0x1c 0x05 0x1f 0x06 ;" which is the first "real" line in the core definitions.
    * For some reason, the strtok breaks on Linux. --> Fix: #include <string.h>, not needed on solaris
  * Another segfault, caused by the really weird line               t->does = (ULONG *)((ULONG)get_last()+sizeof(struct wordlist));
    * This line sets the pointer does to the next element in the list, which seems to be (t+1) in normalspeak. I have no idea what the thinking was behind this.
    * Again, works on solaris, need change on linux.
    * Which, turned out to be because my Solaris box is 32 bit, the Linux box is 64. So sloppy pointer arithmetics is causing a lot of the problems in this code.
  * Segfault when stashing a pointer to the stack - works on Solaris.
    * Due to not having the "here()" function defined with a proper prototype (i.e. same root cause as the string.h include problem)

* Non-critical:
  * There is a crapload of compiler warnings. It still builds. 
    * I disabled a few of them that should be safe to display, see the Makefile for details
  * "12 emit" clears the screen, it used to do a newline
  * I use cpp to preprocess the core definitions. "Modern" cpp pukes at some of the text
  
