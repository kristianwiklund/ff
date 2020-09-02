#include <stdio.h>
#include "opcodedef.h"
#include "ff.h"

extern ULONG *stusp; /* this is really the start of the wordlist */
extern ULONG *usp;
extern ULONG *hre;
extern int defmode;

struct wordlist *lastentered;
char *topoflist;

void init_wordlist()
{
  struct wordlist *t;
  topoflist = (char *)stusp;
  t = (struct wordlist *)topoflist;

  lastentered = 0;
  t->next = 0;
  t->flags = 0;
  t->name[0] = 0;

  clear_table(); /* init hashtable */
}

BOOL make_word_header(char *name)
{
  struct wordlist *t;
  
  defmode = 1;

  t = (struct wordlist *)topoflist;

  if(topoflist>=(char *)usp)
    return (BOOL)0; /* mem fail */

  lastentered = t;

  strncpy(t->name,name,16);
  t->name[15] = 0;

  t->flags = 0;
  t->doesl = 0;
  t->does = 0;

  update_pointers(sizeof(struct wordlist));
  return enter_hash(t->name,t,0);
}
void set_flags(UWORD flags)
{
  lastentered->flags = flags;
}
void set_does_ptr(ULONG *does)
{
  lastentered->does = does;
}

ULONG get_last()
{
  return (ULONG)lastentered;
}

struct wordlist *find_word(char *str)
{
  struct hashentry *t;
  
  t = find_entry(str);

  if(t)
    return t->code;
  else
    return 0;
}

BOOL forget_words(char *str)
{
  struct wordlist *t,*o;
  char *p,*q;

  if(!(t = o = find_word(str))) /* find last occurance of 'str' */
    return (BOOL)0;
  
  while(t)
    {
      if(!t->name[0])
	break;

      remove_hashentry(t->name);
      p = (char *)t;
      t=t->next;
    }
  for(q=(char *)o;q<(char *)t;q++)
    *q=0;
    
  topoflist = (char *)o;
  lastentered = 0; /* it gets autovoided */

  rehash();

  return (BOOL)1;
}

void list_list()
{
  struct wordlist *t = (struct wordlist *)stusp;
  int cnt = 0;

  puts("Wordlist:");

  if(!t || !t->name[0])
    {
      puts("<empty>");
      return;
    }
  
  while(t)
    {
      if(!t->name[0])
	return;

      printf("%s ",t->name);
      cnt++;
      if(cnt>=5)
	{
	  cnt=0;
	  printf("\n");
	}
      t=t->next;
    }
  printf("\n\n");
}

void rehash()
{
  /* a somewhat unefficient way to do this, but what the hell... */

  struct wordlist *t = (struct wordlist *)stusp;

  while(t)
    {
      if(!t->name[0])
	return;

      enter_hash(t->name,t,0);

      t=t->next;
    }
}

  
void update_pointers(ULONG n)
{
  ULONG apa;
  apa = (ULONG)hre;
  apa+=n;
  hre = (ULONG *)apa;

  if(!defmode)
    return;

  topoflist = topoflist + n;
  lastentered->next = (struct wordlist *)topoflist;
}
