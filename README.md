Forth with virtual machine.

Originally written in 1993. Be afraid :-)

--

Things that used to work in 1993 but do not work now:


* Critical:
  * t->doesl = (ULONG) t->does = 0; // this doesn't compile with gcc7, had to be changed to two lines. It assigns zero to both doesl and does.
  * The software compiles (with the assignment change above) on both Linux and Openindiana (Solaris), but *segfaults on Linux*. It *works perfectly on Openindiana*.

* Non-critical:
  * There is a crapload of compiler warnings. It still builds.
  * "12 emit" clears the screen, it used to do a newline
  
