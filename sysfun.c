#include <stdio.h>
#include "ff.h"
#include "opcodedef.h"

/* some sysfun and sysvar definitions, also code to enter them into
   wordlist */

extern struct wordlist *lastentered;
extern ULONG dreg,areg,treg;
extern ULONG *ssp,*usp,*hre;
extern char *topoflist;
extern int defmode;

void sf_load()
{
  int screen;
  char fn[250];
  FILE *in;

  screen = *usp;
  usp++;
  
  sprintf(fn,"%s/%d",SCREENDIR,screen);
  printf("Loading %s\n",fn);

  if(!(in = fopen(fn,"rb")))
    {
      puts("File open error.");
      return;
    }

  while(!feof(in))
    {
      if(!fgets(fn,250,in))
	break;
      parse(fn);
    }
  fclose(in);
}

/* : */

void sf_defword()
{
  char *a;

  usp--;
  *usp = here();
  
  a = (char *)strtok(0," ");
  make_word_header(a);
  hre = (ULONG *)topoflist;
  lastentered->does = hre;


}

/* ; */

void sf_endword()
{
  *hre = RTS;

  update_pointers(sizeof(ULONG));
  hre = (ULONG *)*usp;
  usp++;
  defmode = 0;
  lastentered->flags|=W_SMUDGE;
}

void sf_immediate()
{
  lastentered->flags|=W_IMMEDIATE;
}

/* push 'here' to stack */

void sf_here()
{
  usp--;
  *usp = (ULONG)here();
}

/* 'comp' */

void sf_comp()
{
  int i,j;
  
  ULONG *apa;
  apa = (ULONG *)here();

  i = *usp;
  usp++;

  for(j=0;j<i;j++)
    {
      *apa = *usp;
      usp++;
      apa++;
      update_pointers(sizeof(ULONG));
    }
}

void sf_emit()
{
  putchar((char)*usp);
  usp++;
}

void sf_dot()
{
  printf("%d",*usp);
  usp++;
}

void create_simple_word(char *name,ULONG *ptr,UWORD flags)
{
  printf("Creating sys word %s at %x.\n",name,ptr);

  make_word_header(name);
  set_does_ptr(ptr);
  set_flags(flags);
}

sf_div()
{
  usp++;
  *usp = (*usp / (UWORD)*(usp - 1));

}

sf_mul()
{
  usp++;
  *usp = *usp * (*(usp-1));
}

sf_quit()
{
  puts("Quitting...");
  exit(0);
}

/* load the sysfun's into the owrdlist */

void load_sysfun()
{
  create_simple_word("immediate",(ULONG *)sf_immediate,M_SYSCALL);
  create_simple_word("here",(ULONG *)sf_here,M_SYSCALL);
  create_simple_word("comp",(ULONG *)sf_comp,M_SYSCALL);
  create_simple_word("emit",(ULONG *)sf_emit,M_SYSCALL);
  create_simple_word(".",(ULONG *)sf_dot,M_SYSCALL);
  create_simple_word("list",(ULONG *)list_list,M_SYSCALL);
  create_simple_word("/",(ULONG *)sf_div,M_SYSCALL);
  create_simple_word("*",(ULONG *)sf_mul,M_SYSCALL);
  create_simple_word(":",(ULONG *)sf_defword,M_SYSCALL|W_IMMEDIATE);
  create_simple_word(";",(ULONG *)sf_endword,M_SYSCALL|W_IMMEDIATE);
  create_simple_word("quit",(ULONG *)sf_quit,M_SYSCALL);
  create_simple_word("monitor",(ULONG *)monitor,M_SYSCALL);
  create_simple_word("load",(ULONG *)sf_load,M_SYSCALL);

  /* sysvars */

  create_simple_word("'sp",(ULONG *)&usp,M_SYSVAR);
  create_simple_word("'here",(ULONG *)&hre,M_SYSVAR);
  create_simple_word("il",(ULONG *)sizeof(ULONG),M_SYSVAR);

}








