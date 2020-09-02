Forth with virtual machine.

Originally written in 1993. Be afraid :-)

--

Things that used to work in 1993 but do not work now:

**   t->doesl = (ULONG) t->does = 0; // this doesn't compile with gcc7, had to be changed to two lines. It assigns zero to both doesl and does.

