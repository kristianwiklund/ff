#include <stdio.h>
#include "ff.h"
#include "opcodedef.h"

ULONG tpa[TPA_SIZE];
ULONG *here;

void parse(char *line)
{
  int defmode = 0;
  char *tok,buf[8192];
  struct wordlist *t,*co;
  ULONG *tt,*loc,*ttt;
  int i;
  
  here = tpa;

  line = strs(line); /* remove blanks */

  if(*line==':')
    {
      tok = (char *)strtok(line," ");
      defmode = 1;
      
      if(!(tok = (char *)strtok(0," ")))
	{
	  puts("Bad definition.");
	  return;
	}
    }
  else 
    {
      strcpy(buf,"temp_word ");
      strcat(buf,line);
      tok = (char *)strtok(buf," ");
    }
      
  if(!(make_word_header(tok)))
    {
      puts("out of memory.");
      return;
    }
  
  t = (struct wordlist *)get_last();
  tt = (ULONG *)t->next;
  t->does = tt;
 

  while(1)
    {
      tok = (char *)strtok(0," ");
      
      if(tok && *tok=='\n')
	{
	  if(!defmode)
	    {
	      *tt = RTS;
	      exec(t->does);
	      forget_words("temp_word");
	      return;
	    }
	  else
	    {
	      line = strs(line);
	      tok = (char *)strtok(line," ");
	    }
	}
      if(!tok || *tok==';')
	{
	  *tt = RTS;
	  update_pointers(sizeof(ULONG));
	  t->flags = W_SMUDGE;

	  if(!defmode)
	    {
	      exec(t->does);
	      forget_words("temp_word");
	    }
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
	      puts(tok);
	      puts("bad word");
	      return;
	    }
	}
      else
	{
	  if(co->flags & W_IMMEDIATE)
	    {
	      exec(co->does);
	      tt = (ULONG *)here();
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
    }
}

