#include <stdio.h>
#include "opcodedef.h"
#include "ff.h"

ULONG dreg,areg,treg,*ssp,*usp,*pc,*stssp,*stusp;
UWORD ccreg;

void (*func[NROPCODES])();

void loadim()
{
	dreg = fetch();
}

void load()
{
	dreg = *((ULONG *)fetch());
}

void loada()
{
	dreg = *(((ULONG *)areg)+fetch());
}

void loads()
{
	dreg = *(usp+fetch());
}

void test()
{
	ccreg = (!dreg*ZFLAG) | ((dreg<0)*NFLAG);
}
 
void pop()
{
	dreg = *usp++;
}

void push()
{
	usp--;
	*usp = dreg;
}

void beq()
{
	if(ccreg & ZFLAG)
		pc += ((ULONG)fetch()-1);
	else
		pc++;
}

void blet()
{
	if(!(ccreg & ZFLAG))
	{
		if((ccreg & NFLAG))
			pc += ((ULONG)fetch()-1);
		else
			pc++;
	}
}

void bgt()
{
	if(~ccreg & (NFLAG | ZFLAG))
	  pc += ((ULONG)fetch()-1);
	else 
		pc++;
}

void ble()
{
	if(ccreg & NFLAG)
		pc += ((ULONG)fetch()-1);
	else
		pc++;
}

void bge()
{
	if(~ccreg & NFLAG)
		pc += ((ULONG)fetch()-1);
	else
		pc++;
}

void loadst()
{
	dreg = *(usp + treg);
}

void loadat()
{
	dreg = *((ULONG *)(areg + treg));
}

void store()
{
	*((ULONG *)fetch()) = dreg;
}

void storea()
{
	*(((ULONG *)areg)+fetch()) = dreg;
}

void stores()
{
	*(usp+fetch()) = dreg;
}

void storest()
{
	*(treg + usp) = dreg;
}

void storeat()
{
	*((ULONG *)(treg + areg)) = dreg;
}

void pd()
{
	*(--ssp) = dreg;
}

void pa()
{
	*(--ssp) = areg;
}

void pt()
{
	*(--ssp) = treg;
}

void dp()
{
	dreg = *ssp;
	ssp++;
}

void ap()
{
	areg = *ssp;
	ssp++;
}

void tp()
{
	treg = *ssp;
	ssp++;
}

void call()
{
  ssp--;
  *ssp = (ULONG)(pc+1);
  pc = (ULONG *)fetch();
}

void callc()
{
	void (*fn)();
	
	fn = fetch();
	
	fn();
}

void tta()
{
	areg = dreg;
}

void ttt()
{
	treg = dreg;
}

void tfa()
{
	dreg = areg;
}

void tft()
{
	dreg = treg;
}

void adt()
{
	dreg+=treg;
	test();
}

void sbt()
{
	dreg-=treg;
	test();
}

void cmp()
{
	int a;
	
	a = dreg;
	sbt();
	test();
	dreg = a;
}

void rts()
{
	pc = (ULONG *)*ssp;
	ssp++;
}

void bne()
{
	if(!(ccreg & ZFLAG))
		pc += ((ULONG)fetch()-1);
	else
		pc++;
}

void nop(){}
	
void clr()
{
	dreg = 0;
}

void jmp()
{
	pc = (ULONG *)fetch();
}
void bra()
{
  pc+=(ULONG)(fetch()-1);
}


void set_up_stacks()
{
	if(!(ssp = (ULONG *)malloc(sizeof(ULONG)*SYSSTACKSIZE)))
		exit(-128);
	
	if(!(usp = (ULONG *)malloc(sizeof(ULONG)*USERSTACKSIZE)))
		exit(-64);
	
	stssp=ssp;
	stusp=usp;
	
	ssp+=SYSSTACKSIZE;
	usp+=USERSTACKSIZE;
}

void remove_sm()
{
	free(stssp);
	free(stusp);
}

void init_fn_tab()
{
	/* yes I know, don't say it. */
	
	func[LOADIM] 	= loadim;
	func[LOAD] 	= load;
	func[LOADA] 	= loada;
	func[LOADS] 	= loads;
	func[TEST] 	= test;
	func[POP] 	= pop;
	func[PUSH] 	= push;
	func[BEQ]	= beq;
	func[BLT]	= blet;
	func[BGT]	= bgt;
	func[BLE]	= ble;
	func[BGE]	= bge;
	func[LOADST]	= loadst;
	func[LOADAT]	= loadat;
	func[STORE]	= store;
	func[STOREA]	= storea;
	func[STORES]	= stores;
	func[STOREST]	= storest;
	func[STOREAT]	= storeat;
	func[PD]	= pd;
	func[PA]	= pa;
	func[PT]	= pt;
	func[DP]	= dp;
	func[TP]	= tp;
	func[AP]	= ap;
	func[CALL]	= call;
	func[CALLC]	= callc;
	func[TTA]	= tta;
	func[TTT]	= ttt;
	func[TFA]	= tfa;
	func[TFT]	= tft;
	func[ADT]	= adt;
	func[SBT]	= sbt;
	func[CMP]	= cmp;
	func[RTS]	= rts;
	func[BNE]	= bne;
	func[NOP]	= nop;
	func[CLR]	= clr;
	func[JMP]	= jmp;
	func[BRA]       = bra;
}

void init_sm()
{
	set_up_stacks();
	init_fn_tab();
}
	
ULONG fetch()
{
  int t;
  t = *(ULONG *)pc;
  pc++;
  return t;
}

void exec_opcode()
{
	ULONG tmp;
	if((tmp = fetch())>=NROPCODES)
	{
		puts("Illegal opcode.");
		pc = 0;
		return;
	}
	func[tmp]();
}

void exec(ULONG *what)
{
	dreg = 0;
	pd();
	
	pc = what;
	
	while(pc)
		exec_opcode();
}

editor()
{
  char buf[8192];
  
  while(1)
    {
      printf("> ");
      gets(buf);
      parse(buf);
    }
}

main()
{
  puts("\nFisherman's Friend V0.89");
  puts("(C) K. Wiklund 1993");

  init_sm();
  init_wordlist();

  load_sysfun();
  loadcore(CORENAME);

/*monitor();*/
editor();



  remove_sm();

}
	


