#include <stdio.h>
#include "opcodedef.h"
#include "ff.h"

char *opcodes[] =
{
        "loadim","load","loada","loads","test",
        "pop","push","beq","blt","bgt","ble","bge",
        "loadst","loadat","store","storea",
        "stores","storest","storeat","pd","pa","pt","dp",
        "ap","tp","call","callc","tta","ttt","tfa","tft",
        "adt","sbt","cmp","rts","bne","jmp","clr","nop",
	"bra",0
};

extern ULONG dreg,areg,treg,*ssp,*usp,*pc,*stssp,*stusp;
extern UWORD ccreg;

void monitor()
{
  char cmd[250];
  ULONG i,from,to,q,t;
  ULONG *ws;
  struct wordlist *tt;

  ws = stusp;
        
  while(1)
    {
      printf("\nws:%x (pc:%x,dr:%x,tr:%x,ar:%x,usp:%x,ssp:%x) >"
             ,ws,pc,dreg,treg,areg,usp,ssp);
      
      scanf("%s",cmd);
  
      switch(*cmd)
        {
	case 'w': list_list();
	  break;
        case 'm': scanf("%x",&from);
          scanf("%x",&to);
          
          printf("%x: ",from);
          for(i=from;i<to;i++)
            {
              printf("%x ",*(i + ws));
              if(!((i-from+1)%4))
                printf("\n%x: ",i);
            }   
          break;
          
        case 'q':
          return;
	case 'p': gets(cmd);
	  parse(cmd);
	  break;
        case 'a': scanf("%x",&from);
          scanf("%x",&to);
          gets(cmd);
                                
          for(i=from;i<to;i++)
            {
              printf("%x: ",i);
              gets(cmd);
              if(*cmd=='.')
                break;
              
              sscanf(cmd,"%x",&t);
              if(*cmd=='!')
                {
                  sscanf(cmd+1,"%x",&t);
                  t = (ULONG)(ws+t);
                }
              
              for(q=0;q<NROPCODES;q++)
                if(!strcmp(cmd,opcodes[q]))
                  t = q;
              
              printf("->%x\n",t);
              *(ws+i)=t;
            }
          break;

	case 'l': 
	  if(!(tt = find_word(cmd+1)))
	    puts("<nil>");
	  else
	    {
	      puts(cmd);

	      if(tt->does)
	      printf("Does: %x\n",((ULONG)tt->does-(ULONG)ws)/sizeof(ULONG));
	      printf("Header: %x\n\n",(((ULONG)tt)-(ULONG)ws)/sizeof(ULONG));
	    }
	  break;
	case 'e': 
	  if(!(tt = find_word(cmd+1)))
	  puts("fail!");
	  else
	    exec(tt->does);
	  break;
	case 's': if(usp==stusp+USERSTACKSIZE)
	  puts("<stack is empty>");
	  else
	    {
	      printf("%x->%x\n",usp,stusp+USERSTACKSIZE);
	      for(t=(ULONG)usp;t<(ULONG)(stusp+USERSTACKSIZE);t+=sizeof(ULONG))
		printf("%x: %x\n",(t-(ULONG)ws)/sizeof(ULONG),*((ULONG *)t));
	    }
	  break;
	  
        case 'd': scanf("%x",&from);
          scanf("%x",&to);
                        
          for(i=from;i<to;i++)
            {
              printf("%x (%x): ",i,ws + i);
                                
              if(*(i+ws)<NROPCODES)
                printf("%s ",opcodes[*(ws+i)]);
              else
                printf("*** (%x)",*(ws+i));
                                
              switch(*(ws+i))
                {
                case LOADIM:
                case LOADS:
                case LOADA:
                case STORES:
                case STOREA: i++;
                  printf("%x\n",*(ws+i));
                        
                  break;
                case BNE:
                case BEQ:
                case BLT:
                case BGT:
                case BLE:
                case BGE:
		case BRA:
		  i++;
		  printf("%x\n",i+*(ws+i));
			 break;
		case LOAD:
                case CALL:
                case CALLC:
                case JMP:
                case STORE: i++;
                  printf("%x\n", (*(ws+i)-(ULONG)ws)/sizeof(ULONG));
                  break;
                default: puts("");
                }
            }
          break;                                
          
        case 'j': scanf("%x",&i);
          printf("\nExecuting with pc = %x.\n",ws+i);
          exec(ws+i);
          break;
                
        default: puts("bad command!");
        }
    }   
}       

