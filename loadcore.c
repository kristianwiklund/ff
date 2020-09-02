#include <stdio.h>
#include "ff.h"
#include "opcodedef.h"

extern ULONG areg,dreg;
extern char *instream;

char *strs(char *a)
{
  while(*a==' ' || *a==9)
    a++;

  return a;
}

void loadcore(char *filename)
{
  FILE *in;
  struct wordlist *t,*co;
  char buf[8192],*tok,*tn,*tmp;
  ULONG *pt;
  
  tmp = tmpnam(0);

  puts("Loading core definitions.");

  /* invoke cpp */
  
  sprintf(buf,"cpp %s %s 2>/dev/null",filename,tmp);
  system(buf);

  if(!(in = fopen(tmp,"r")))
    {
      puts("Problem opening temporary core file.");
      exit(-1);
    }

  while(!(feof(in)))
    {
      fgets(buf,8192,in);


 
      if(*buf=='p')
	{
	  printf("Parsing %s",buf+1);
	  parse(buf+1);
/*	  ((struct wordlist *)get_last())->flags = W_SMUDGE | W_IMMEDIATE;*/
	}
      else
      if(buf[0]!='*' && buf[0]!='#' && buf[0]!='\n')
	{
	  tn = (char *)strtok(strs(buf)," \t");
	  
	  if(tn)
	    {
	      make_word_header((char *)strtok(0," "));
	      t = (struct wordlist *)get_last();

	      t->does = (ULONG *)((ULONG)get_last()+sizeof(struct wordlist));
	      t->flags = W_SMUDGE | (W_INLINE * !strcmp(tn,"inline"));
	      pt = (ULONG *)t->does;
	      
	      printf("Loading %s word %s\n",tn,t->name);

	      while(1)
		{
		  tok = (char *)strtok(0," ");
		  
		  if(!tok || *tok=='\n')
		    do
		      {
			if(feof(in))
			  {
			    puts("LOADCORE: unexpected end of file.");
			    exit(-12);
			  }
			fgets(buf,8192,in);
			tok = (char *)strtok(strs(buf)," ");
		      } while(*buf=='\n' || *buf=='#' || *buf=='*');

		  if(*tok==';')
		    break;
		  else
		    {
		      if(*tok=='-' || *tok=='+')
			{
			  sscanf(tok,"%d",pt);

			  *pt=*pt * (1-2*(*tok=='-'));

			}
		      else
			if(co = find_word(tok))
			  {
			    printf("%s",co->name);
			    *pt = CALLC;
			    pt++;
			    *pt = (ULONG)co->does;
			    update_pointers(sizeof(ULONG));
			  }
			else
			  sscanf(tok,"0x%x",pt);
		    }
		  pt++;
		  printf("0x%x ",*(pt-1));
		  update_pointers(sizeof(ULONG));
		  t->doesl++;
		}
	    }
	  printf("\n");
	}
    }
  
  unlink(tmp);
  puts("*** Loading completed");
}

