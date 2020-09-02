#include <stdio.h>
#include "ff.h"
#include "opcodedef.h"

ULONG *hre;

int defmode;

ULONG *here()
{
  return hre;
}

void parse(char *line)
{
  char *tok,buf[8192];
  struct wordlist *co;
  ULONG *tt,*loc,*ttt,*mytpa;
  int i;
  void (*apa)();

  defmode = 0;

  debuglog( "parse: %s\n", line);
  
/*  hre = tt = tpa; */
  
  tt = here();
  mytpa = here();

  line = strs(line); /* remove leading blanks */
  
  tok = (char *)strtok(line," \t\n");

  while(1)
    {
      if((tok && *tok=='\n') || (tok && *tok=='#'))
	{
	  if(!defmode)
	    {
	      *tt = RTS;
	      update_pointers(sizeof(ULONG));
	      exec(mytpa);
	      return;
	    }
	  else
	    {
	      line = strs(line);
	      tok = (char *)strtok(line," \t\n");
	    }
	}
      if(!tok)
	{
	  *tt = RTS;
	  update_pointers(sizeof(ULONG));
	  exec(mytpa);
	  return;
	}

      co = find_word(tok);
      if(!co || !(co->flags & W_SMUDGE))
	{
	  if(sscanf(tok,"%d",&i))
	    {
	      if(i==0 && !strncmp("0x",tok,2))
		sscanf(tok,"0x%x",&i);
	      *tt = LOADIM;
	      tt++;
	      *tt = (ULONG)i;
	      tt++;
	      *tt = PUSH;
	      tt++;
	      update_pointers(3*sizeof(ULONG));
	    }
	  else
	    {
	      printf("Bad word: %s\n",tok);
	      return;
	    }
	}
      else
	{
	  if(co->flags & W_IMMEDIATE)
	    {

	      if(co->flags & W_SYSCALL)
		{
		  apa = co->does;
		  apa();
		}
	      else
		exec(co->does);
	      tt = here();
	    }
	  else
	    if(co->flags & W_INLINE)
	      for(i=0;i<co->doesl;i++)
		{
		  *tt=*(co->does+i);
		  tt++;
		  update_pointers(sizeof(ULONG));
		}
	    else
	      if(co->flags & W_SYSCALL)
		{
		  *tt = CALLC;
		  tt++;
		  *tt = (ULONG)co->does;
		  tt++;
		  update_pointers(2*sizeof(ULONG));
		}
	      else
		if(co->flags & W_SYSVAR)
		  {
		    *tt = LOADIM;
		    tt++;
		    *tt = (ULONG)co->does;
		    tt++;
		    *tt = PUSH;
		    tt++;
		    update_pointers(3*sizeof(ULONG));
		  }
		else
		  {
		    *tt = CALL;
		    tt++;
		    *tt = (ULONG)co->does;
		    tt++;
		    update_pointers(sizeof(ULONG)*2);
		  }
	}
      tok = (char *)strtok(0," \t\n");
    }
  
}



