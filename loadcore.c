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

/* some specials */

void gh()
{
  dreg = (ULONG)here();
  push();
}

void comp()
{
  int i,j;
  
  ULONG *apa;
  apa = (ULONG *)here();

  pop();
  i = dreg;

  for(j=0;j<i;j++)
    {
      pop();
      *apa=dreg;
      apa++;
      update_pointers(sizeof(ULONG));
    }
}

void emit()
{
  pop();
  printf("%c",dreg);
}

void dot()
{
  pop();
  printf("%d",dreg);
}

void str()
{
  printf("%s",(char *)areg);
}

void name()
{
  char *apa,*where,bepa[2];

  pop();

  bepa[1] = 0;
  bepa[0] = (char)(dreg&0xff);
 
  apa = (char *)strtok(0,bepa);
  where  = (char *)here();

  while(*apa)
    {
      *where = *apa;
      where++;
      apa++;
    }
  where  = (char *)((((ULONG)where)/4 + 1)*4);
  update_pointers((ULONG)where-(ULONG)here());
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
  
  sprintf(buf,"cpp %s %s",filename,tmp);
  system(buf);

  if(!(in = fopen(tmp,"r")))
    {
      puts("Problem opening temporary core file.");
      exit(-1);
    }

  while(!(feof(in)))
    {
      fgets(buf,8192,in);

/*      buf = (char *)strs(buf);
*/ 
      if(*buf==':')
	{
	  printf("Parsing %s",buf);
	  parse(buf);
	  ((struct wordlist *)get_last())->flags = W_SMUDGE | W_IMMEDIATE;
	}
      else
      if(buf[0]!='*' && buf[0]!='#' && buf[0]!='\n')
	{
	
	  tn = (char *)strtok(strs(buf)," \t");
	  
	  if(tn)
	    {
	      make_word_header((char *)strtok(0," "));
	      t = (struct wordlist *)get_last();
	      t->builds=0;
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
/*			  *pt = *pt * sizeof(ULONG); */
			  *pt=*pt * (1-2*(*tok=='-'));
/*			  *pt=*pt + here(); */
			}
		      else
			if(co = find_word(tok))
			  {
			    printf("%s",co->name);
			    *pt = CALLC;
			    pt++;
			    *pt = co->does;
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

