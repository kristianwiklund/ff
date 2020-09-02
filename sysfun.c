#include <stdio.h>
#include "ff.h"
#include "opcodedef.h"

/* some sysfun and sysvar definitions, also code to enter them into
   wordlist */

extern struct wordlist *lastentered;
extern ULONG dreg,areg,treg;
extern ULONG *ssp,*usp,*hre,*pc;
extern char *topoflist;
extern int defmode;

void sf_print()
{
  char *apa;

  apa = (char *)*usp;
  usp++;
  printf("%s",apa);
}

void sf_dotquote()
{
  char *apa,*bepa,*baba;
  ULONG *h;

  apa = baba = (char *)strtok(0,"\"\n");
  if(!apa)
    return;

  h = hre;

  *h = BRA;
  h++;
  *h = 0;
  h++;
  bepa = (char *)h;

  while(*apa)
    {
      *bepa = *apa;
      apa++;
      bepa++;
    }
  *bepa = 0;

  bepa = (char *)((strlen(baba)+1)/sizeof(ULONG));
  *(h-1)=(ULONG)bepa+2;

  apa = (char *)h;
  h +=(ULONG)bepa+1;
  *h = LOADIM;
  h++;
  *h = (ULONG)apa;
  h++;
  *h = PUSH;
  h++;
  *h = CALLC;
  h++;
  *h = (ULONG)sf_print;
  h++;

  update_pointers(sizeof(ULONG)*(8+(int)bepa));
}
void sf_load()
{
  int screen;
  char fn[250];
  FILE *in;
  ULONG *opc;

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
      opc = pc;
      parse(fn);
      pc = opc;
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

void sf_forget()
{
  char *apa;
  
  apa = (char *)strtok(0," \t\n");
  forget_words(apa);
}

/* ; */

void sf_endword()
{
  *hre = RTS;
  hre++;
  *hre = 0;
  update_pointers(sizeof(ULONG));
  hre = (ULONG *)*usp; // pull PC from stack. 
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
  printf("%ld",*usp);
  usp++;
}

void create_simple_word(char *name,ULONG *ptr,UWORD flags)
{
  printf("Creating sys word %s at %lx.\n",name,ptr);
 
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
  create_simple_word("forget",(ULONG *)sf_forget,M_SYSCALL|W_IMMEDIATE);
  create_simple_word(".\"",(ULONG *)sf_dotquote,M_SYSCALL|W_IMMEDIATE);
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
  create_simple_word("monitor",(ULONG *)sm_monitor,M_SYSCALL);
  create_simple_word("load",(ULONG *)sf_load,M_SYSCALL);
  create_simple_word("print",(ULONG *)sf_print,M_SYSCALL);

  /* sysvars */

  create_simple_word("'sp",(ULONG *)&usp,M_SYSVAR);
  create_simple_word("'here",(ULONG *)&hre,M_SYSVAR);
  create_simple_word("il",(ULONG *)sizeof(ULONG),M_SYSVAR);

}








