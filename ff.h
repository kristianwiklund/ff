#ifndef FF_H
#define FF_H

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>

#define CORENAME "forthcore"
#define SCREENDIR "screens"

#define TPA_SIZE 2000

/* type def's for amoeba compat */

typedef char BOOL;
typedef long ULONG;
typedef short UWORD;
typedef char UBYTE;

/* the wordlist structure */

struct wordlist
{
  struct wordlist *next;
  char name[16];
  ULONG *does;
  UWORD flags;
  UWORD doesl;
};

/* wordlist prototypes */

BOOL forget_words(char *str);
void list_list();
void rehash();
BOOL make_word_header(char *name);
void update_pointers(ULONG n);
struct wordlist *find_word(char *what);
void set_builds_prt(ULONG *p);
void set_flags(UWORD f);

char *strs(char *a);

/* flags for the wordlist */

#define W_IMMEDIATE 0x0001
#define W_INLINE    0x0002
#define W_SMUDGE    0x0004
#define W_SYSCALL   0x0008
#define W_SYSVAR    0x0010

#define M_SYSCALL W_SYSCALL|W_SMUDGE
#define M_SYSVAR W_SYSVAR|W_SMUDGE

/* hashtable defs, structs and protos */

#define H_FREE          0x0000
#define H_OCCUP         0x0100
#define H_RELEAS        0x0200

struct hashentry
{
        char *key;
        struct wordlist *code;
        UWORD flags;
};

#define HASHSIZE        1000

void clear_table();
int hash(int key,int rehash);
int scmp(char *a,char *b);
struct hashentry *find_entry(char *key);
struct hashentry *find_empty(char *key);
BOOL enter_hash(char *key,struct wordlist *code,UBYTE flags);
void test_hash();
void remove_hashentry(char *key);

/* */

void parse (char *);

/* virtual CPU defs, structs and protos */

#define NFLAG 0x1
#define ZFLAG 0x2

#define SYSSTACKSIZE    50  
#define USERSTACKSIZE   10000 /* this is also the workspace */

ULONG fetch();
void set_up_stacks(); 
void remove_sm();
void init_fn_tab();
void init_sm();
void exec_opcode();
void exec(ULONG *what);

void sm_monitor();

void set_does_ptr(ULONG *does);

ULONG *here();

/* debug messages, select one */
#define debuglog(...)
//#define debuglog(...) fprintf(stderr, __VA_ARGS__)
#endif FF_H



