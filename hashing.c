#include <stdio.h>
#include "opcodedef.h"
#include "ff.h"

struct hashentry *hashtable;

/* fastforth hashing */

void clear_table()
{
        int size;
        char *t;
        
        size = HASHSIZE;
        
        if(!(hashtable = 
        (struct hashentry *)malloc(sizeof(struct hashentry)*HASHSIZE)))
                exit(256);
        
        t = (char *)hashtable;
        
        while(size)
                *(t+(--size))=0;
}

int hash(int key,int rehash)
{
        return (key*rehash - rehash*(key>1)) % HASHSIZE;
}
int scmp(char *a,char *b)
{
  if(!a || !b)
    return -1;

  return strcmp(a,b);
}
struct hashentry *find_entry(char *key)
{
        int rehash = 0;
        int h,code = 1;
        
        for(h = 0;h<strlen(key);h++)
                code+=key[h];
        
        while(scmp(key,hashtable[(h = hash(code,rehash))].key) ||
                hashtable[h].flags & H_RELEAS)
        {
                rehash++;
                if(rehash >= HASHSIZE)
                        return (struct hashentry *)0;
                
                if(!(hashtable[h].flags & 0xff00))
                        return (struct hashentry *)0;
        }
        
        return (struct hashentry *)(hashtable + h);
}

struct hashentry *find_empty(char *key)
{
        int rehash = 0;
        int h,code = 1;
                
        for(h=0;h<strlen(key);h++)
                code+=key[h];
        
        while(hashtable[(h = hash(code,rehash))].flags & H_OCCUP)
        {
                rehash++;
                
                if(rehash >= HASHSIZE)
                        return (struct hashentry *)0;
        }       

        return (struct hashentry *)(hashtable + h);
}

BOOL enter_hash(char *key,struct wordlist *code,UBYTE flags)
{       
        struct hashentry *t;
        
        if(!(t = find_entry(key)))
                if(!(t = find_empty(key)))
                        return (BOOL)0;
        
        t->key = key;
        t->flags = H_OCCUP | flags;
        t->code = code;
        
        return (BOOL)1;
}

void remove_hashentry(char *key)
{
  struct hashentry *t;

  if(!(t = find_entry(key)))
    return;

  t->flags = H_RELEAS;
}
