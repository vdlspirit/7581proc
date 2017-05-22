#include "mycore.h"
#include "math.h"
#include "ti/csl/src/intc/csl_intc.h"
#include <stdio.h>
#include <ti/sysbios/family/c66/Cache.h>
#include <csl_ipcAux.h>
#include "signalProc.h"
#include "signalProcCom.h"
extern BattleMathT1K2 battlemathT1k2;
extern BattleParam battle;			//боевая структура управления программой
#pragma DATA_SECTION(alginf, ".data")
ALGINFO alginf;

//#pragma DATA_SECTION(pbgrnd, ".data")
UInt16 * pbgrnd;

extern TATOOED ta1;						//структура пакета ТА цель 1
int setInterrupt(void);
#define IPCGR_REG  ((volatile unsigned long*)0x02620240)

//#pragma DATA_SECTION(rj, ".l2Buff")
//volatile int rj=0;
//#pragma DATA_SECTION(ri, ".l2Buff")
//volatile int ri=0;
//#pragma DATA_SECTION(rk, ".l2Buff")
//volatile int rk=0;

//volatile unsigned int *InBuff;#pragma DATA_SECTION(intm, ".l2Buff")
//volatile int intm[100];

//interrupt void intIsr(void )
#include "ti/platform/resource_mgr.h"
extern volatile UInt32*     isInitialized;
void nSetupInterrupt(void);
void int3Isr(void *eventId );
int memadd(void * dst,void * src,int size) {
//long long *ldst=(long long *)dst;
long long *lsrc=(long long *)src;
int i;
int j;
long long lsum;
for(i=0;i<size;i++) {
	for(j=0;j<100;j++) {
		asm("	nop	");
//		asm("	nop	");
		}


	lsum+=lsrc[i];
	}
return lsum/size;
}

//volatile int bStop=1;
void MyStartCore2()
{
//	volatile unsigned int *InBuff;
//	volatile unsigned int *OutBuff;
//	int size;
	int init=0,i;
	unsigned int size;
	int retval;
	int ksum;
	unsigned char bTargeting=1;
	short coordAngleQX;
	short coordAngleFY;

	double kpixang=3.42;

//	while(bStop);
	for(i=0;i<200000000;i++) {
//	for(i=0;i<400000000;i++) {
				asm("  nop");
	//			asm
			}
	setInterrupt();
//	InBuff = (unsigned int*)(0x00800400);
//	OutBuff = (unsigned int*)(0x80000000);
//	nSetupInterrupt(); //stopwatch
	isInitialized[74]=0xc100000; // aim 1 signal frame
	isInitialized[84]=0xc1a0000; // aim 1 signal frame
//	isInitialized[72]=0xc100000; // aim 1 dark frame

	isInitialized[72]=0xc112000; // aim 1 dark frame
	isInitialized[82]=0xc1a5000; // aim 1 dark frame

	Cache_wb((void *) &isInitialized[74], 4, Cache_Type_L1D, TRUE);
	isInitialized[66]=0x02aa5501;
	//			CACHE_wbL1d ((void *) &isInitialized[8], 4, CACHE_WAIT);	Cache_wb((void *) &isInitialized[66], 4, Cache_Type_L1D, TRUE);
	//		Cache_inv((void *) &isInitialized[19], 4, Cache_Type_L1D, TRUE);
//	size=((isInitialized[19]>>16)&0xfff)*(isInitialized[19]&0xfff)*2;
//	rj=ri;
	isInitialized[66]=0x02aa5501;
	CACHE_wbL1d ((void *) &isInitialized[66], 4, CACHE_WAIT);
	pbgrnd=(UInt16 *)0xc112000;
	while(1) {
		Cache_inv((void *) &battle, sizeof(BattleParam), Cache_Type_L1D, TRUE);
		if(battle.STAGE==sSTANDBY) {
			kpixang=3600*57.29578*0.0055/battle.FokusK2;
			break;
			}
		}

	for(;;)
	{
//		CACHE_invL1d ((void *) &isInitialized[66], 4, CACHE_WAIT);

		 Cache_inv((void *) &isInitialized[66], 4, Cache_Type_L1D, TRUE);
		 init=isInitialized[66];

//		if(isInitialized[66] == 0x02000001) {
//			memcpy((void *)(0x0C112000),(void*)0xc100000,73728); //Bug
//		}
//			if(size!=0) {
//				if(init== 0x02000001) {
//			if(isInitialized[66] == 0x02000001)
		 /*
		 for(i=0;i<1000;i++) {
			asm("	nop	");
			asm("	nop	");
			asm("	nop	");
			asm("	nop	");
		 }
		 */
		 if(init== 0x02000001) {
//				memcpy((void *)(0x800400),(void*)0xc100000,73728); //Bug
//				memcpy((void *)(0x800400),(void*)0x815000,73728); //Bug
//				mymcpy((void *)(0x800400),(void*)0xc100000,20000); //Bug !!!!!!!!!!!!

//				mymcpy((void *)(0x800400),(void*)0x815000,1000); //Bug
//				mymcpy((void *)(0x800400),(void*)0x815000,5000); //Bug
//				mymcpy((void *)(0x800400),(void*)0x815000,5000); //Bug
//				mymcpy((void *)(0x800400),(void*)0x815000,5000); //Bug
//		 }
			 if(CSL_IPC_isGEMInterruptSourceSet(2,0)) {
				 CSL_IPC_isGEMInterruptSourceSet(0,2);
//				 ri++;
			 }
//				memcpy((void *)(0x0C112000),(void*)0xc100000,20000); //Bug
//				mymcpy((void *)(0x800400),(void*)0xc100000,20000); //Bug !!!!!!!!!!!!
//				mymcpy((void *)(0x800400),(void*)0xc100000,9216); //Bug !!!!!!!!!!!!

//				mymcpy((void *)(0x800400),(void*)0xc112000,9216); //Bug !!!!!!!!!!!!

//				signalProcess((void *)(0x800400),(void*)0xc112000,20000);
				if(vkp.MathKommand.s.K_s_sh>0)
					alginf.ksmr=vkp.MathKommand.s.K_s_sh;
				else
					alginf.ksmr=50;
				if(vkp.MathKommand.s.K_s_sh>0)
					alginf.kbgrnd=vkp.res7;
				else
//					alginf.kbgrnd=20;
					alginf.kbgrnd=30;
				Cache_inv((void *) &battle, sizeof(BattleParam), Cache_Type_L1D, TRUE);
				if(battle.STAGE==sBATTLE) {
//					Cache_inv((void *) &battle, sizeof(BattleParam), Cache_Type_L1D, TRUE);
//					 Cache_inv((void *) &ta1.CommandTA1.s, 4, Cache_Type_L1D, TRUE);
					 Cache_inv((void *) &ta1.CommandTA1, sizeof(TATOOED), Cache_Type_L1D, TRUE);

//					alginf.command=(0x1 <<6) | (0x1 <<4) | (0x1 <<13) | (0x1 <<11);
//					if(ta1.CommandTA1.s.ResetK1)
						if(ta1.CommandTA1.s.res03) { // Test mode
								if(ta1.CommandTA1.s.res02)
									alginf.command=(0x1 <<6) | (0x1 <<4) | (0x1 <<13) | (0x1 <<11) ;
								else
									alginf.command=(0x1 <<6) | (0x1 <<4) | (0x1 <<13) | (0x1 <<11)  | (0x1 <<10) ;

								}
							else {
								if(ta1.Distance>6000) {
									alginf.command=(0x1 <<6) | (0x1 <<4) | (0x1 <<13) | (0x1 <<11) ;
									}
								else {
									alginf.command=(0x1 <<6) | (0x1 <<4) | (0x1 <<13) | (0x1 <<11)  | (0x1 <<10) ;
									}
								}
							}
				else
					alginf.command=vkp.MathKommand.s.Kommand;
				ksum=2;
				if(battle.BMODETARGET1==t1ConvoyChan1_2 || battle.BMODETARGET1==t1ConvoyChan2)
						ksum=1;

				 Cache_inv((void *) &isInitialized[19], 4, Cache_Type_L1D, TRUE);
				 size=isInitialized[19];
				 if( size== ((192<<16) +192)) {
//				 if( size== 0xc000c0)
					if( ta1.CommandTA1.s.res03 && ta1.CommandTA1.s.res01)
							alginf.command|=(0x1 <<14);
					else
						alginf.command&=~(0x1 <<14);
					 retval=signalProcess((void *)(0x800400),(void*)0xc100000,20000);
				 	 }
				 if( size== ((184<<16) +184)) {
					 retval=signalProcessLC2((void *)(0x800400),(void*)0xc100000,20000);
				 	 }

				vsp.TimeJob121= 2*alginf.tm;
				vsp.NOBJ121=alginf.nobj;
				vsp.NPointPorg121=alginf.npt;

				vsp.NOBJ122=alginf.nobj2;
				vsp.SUFon122= alginf.uimean;
				vsp.SKO122=alginf.uismr;
				vsp.CordX121=alginf.x;
				vsp.CordY121=alginf.y;
				vsp.CordX122=alginf.x2;
				vsp.CordY122=alginf.y2;
				vsp.LocUFon121=alginf.bgrnd;
				vsp.LocSKO121=alginf.rms;

				vsp.res121_1=alginf.sz;
				vsp.res122_1=alginf.niter;

//				battlemathT1k2.reserv1=alginf.;
				battlemathT1k2.reserv4=2*alginf.tm;
				battlemathT1k2.reserv5=alginf.x;
				battlemathT1k2.reserv6=alginf.y;
				battlemathT1k2.IndLO2=(retval>0);
				if(bTargeting == 1 || ksum==2) {
					coordAngleQX=4*ta1.qk2;
					coordAngleFY=4*ta1.fk2;
					}
				else {
					coordAngleQX=battlemathT1k2.Target1CoordAngX2;
					coordAngleFY=battlemathT1k2.Target1CoordAngY2;
					}
				bTargeting =1;

				battlemathT1k2.qd21= coordAngleQX;			// датчик канала 2 по координате q
				battlemathT1k2.fd21 =coordAngleFY;			// датчик канала 2 по координате fi

				if(battlemathT1k2.IndLO2) {

//					battlemathT1k2.Target1CoordAngX2=round(1.368*(double)(battle.Target1CoordY2*10+ksum*alginf.y - battle.VizirChan2_Y*10));
//					battlemathT1k2.Target1CoordAngY2=-round(1.368*(double)(battle.Target1CoordX2*10+ksum*alginf.x - battle.VizirChan2_X*10));
					battlemathT1k2.Target1CoordAngX2=round(0.4*kpixang*(double)(battle.Target1CoordY2*10+ksum*alginf.y - battle.VizirChan2_Y*10));
					battlemathT1k2.Target1CoordAngY2=-round(0.4*kpixang*(double)(battle.Target1CoordX2*10+ksum*alginf.x - battle.VizirChan2_X*10));

//					battlemathT1k2.qk21=round(1.368*(double)(battle.Target1CoordY2*10+ksum*alginf.y - battle.VizirChan2_Y*10) - coordAngleQX);	// координатор канала 2 по координате q
//					battlemathT1k2.fk21=-round(1.368*(double)(battle.Target1CoordX2*10+ksum*alginf.x - battle.VizirChan2_X*10) + coordAngleFY);	// координатор канала 2 по координате fi

					battlemathT1k2.qk21=round(0.4*kpixang*(double)(battle.Target1CoordY2*10+ksum*alginf.y - battle.VizirChan2_Y*10) - coordAngleQX);	// координатор канала 2 по координате q
					battlemathT1k2.fk21=-round(0.4*kpixang*(double)(battle.Target1CoordX2*10+ksum*alginf.x - battle.VizirChan2_X*10) + coordAngleFY);	// координатор канала 2 по координате fi

					if(ta1.CommandTA1.s.SoprLO2) {
						bTargeting =0;
						}

//					battlemathT1k2.Sizeq12=round(ksum*3.4*(double)(alginf.size>>8));		// угловой размер источника в канале 2 по координате  q
//					battlemathT1k2.Sizef12=round(ksum*3.4*(double)(alginf.size&0xff));		// угловой размер источника в канале 2 по координате  fi
					battlemathT1k2.Sizeq12=round(ksum*kpixang*(double)(alginf.size>>8));		// угловой размер источника в канале 2 по координате  q
					battlemathT1k2.Sizef12=round(ksum*kpixang*(double)(alginf.size&0xff));		// угловой размер источника в канале 2 по координате  fi
					battlemathT1k2.Ksp21=alginf.snr;								// отношение сигнал/порог в канале 2 ракета 1
					battlemathT1k2.US21=alginf.level;		// уровень сигнала в канале 2
					}
				else {
					battlemathT1k2.Sizeq12=0;		// угловой размер источника в канале 2 по координате  q
					battlemathT1k2.Sizef12=0;		// угловой размер источника в канале 2 по координате  fi
					battlemathT1k2.US21=0;		// уровень сигнала в канале 2
					battlemathT1k2.Ksp21=0;
					}

//				battlemathT1k2.reserv5=alginf.nobj2;
//				battlemathT1k2.reserv6=alginf.nobj;
				Cache_wb((void *) &battlemathT1k2, sizeof(BattleMathT1K2), Cache_Type_L1D, TRUE);


				 Cache_inv((void *) &vsp, 64, Cache_Type_L1D, TRUE);
//				vsp.TimeJob121=vkp.MathKommand.s.Kommand;
//				isInitialized[66]=0x02aa5501;

//				mymcpy((void *)(0x800400),(void*)0x815000,5000); //Bug !!!!!!!!!!!!
//				memadd((void *)(0x800400),(void*)0xc100000,20000); //Bug !!!!!!!!!!!!
//				memadd((void *)(0x800400),(void*)0xc100000,5000); //Bug !!!!!!!!!!!!
//				memadd((void *)(0x800400),(void*)0x815000,5000); //Bug !!!!!!!!!!!!
//				memadd((void *)(0x800400),(void*)0x815000,20000); //Bug !!!!!!!!!!!!
//				mymcpy((void *)(0x800400),(void*)0xc100000,5000); //Bug !!!!!!!!!!!!
//				mymcpy((void *)(0x800400),(void*)0xc100000,500); //work !!!!!!!!!!!!

//				mymcpy((void *)(0x800400),(void*)0xc100000,5000); //Bug !!!!!!!!!!!!
//				mymcpy((void *)(0x800400),(void*)0xc100000,5000); //Bug !!!!!!!!!!!!

//				memcpy((void *)(0x0C112000),(void*)0xc100000,20000); //Bug
//				memcpy((void *)(0x0C112000),(void*)0xc100000,20000); //Bug
//				memcpy((void *)(0x0C112000),(void*)0xc100000,20000); //Bug

//				mymcpy((void *)(0x0C112000),(void*)0xc100000,20000); //Bug
//				mymcpy((void *)(0x0C112000),(void*)0xc100000,5000); //Bug
				isInitialized[66]=0x02aa5501;
				CACHE_wbL1d ((void *) &isInitialized[66], 4, CACHE_WAIT);
//				IPCGR_REG[0]=1;
//				IPCGR_REG[1]=1;

				//isInitialized[74]=0xc100000; // aim 1 signal frame
				//isInitialized[84]=0xc1a0000; // aim 1 signal frame
				//isInitialized[72]=0xc112000; // aim 1 dark frame
				//isInitialized[82]=0xc1a5000; // aim 1 dark frame

				if(ksum==1) {
					zoom2_192((unsigned short *)0xc100000,(unsigned short *)0xc1a0000);
					zoom2_192((unsigned short *)0xc112000,(unsigned short *)0xc1a5000);
					}
//				rj++;

			}
//		isInitialized[66]=0x02aa5501;
//			CACHE_wbL1d ((void *) &isInitialized[8], 4, CACHE_WAIT);
//		Cache_wb((void *) &isInitialized[66], 4, Cache_Type_L1D, TRUE);
//				CACHE_wbL1d ((void *) &isInitialized[66], 4, CACHE_WAIT);

			//		asm("	IDLE	");
			//while(ri==rj);
		 if(CSL_IPC_isGEMInterruptSourceSet(2,0)) {
			 CSL_IPC_clearGEMInterruptSource(2,0);
			 CSL_IPC_genGEMInterrupt(0,2);
//			 ri++;
		 	 }

//			ri=rj;
//					asm("	IDLE	");

	}
}

#pragma CODE_SECTION(intIsr, ".csl_vect");

void intIsr(void *eventId )
{
//	a = 1;
//	rj++;
	return ;
}

CSL_IntcContext             intcContext;
CSL_IntcEventHandlerRecord  EventHandler[30];
CSL_IntcObj                 intcObj;
CSL_IntcHandle              hTest;
CSL_IntcHandle      		hIntc91;
CSL_IntcGlobalEnableState   state;
CSL_IntcEventHandlerRecord  EventRecord;
CSL_IntcParam               vectId;

int setInterrupt(void)
{
//	int pinNum = 0;
//	int bankNum = 0;

    /************************************************
     *************** INTC Configuration *************
     ************************************************/

    printf ("Debug: GEM-INTC Configuration...\n");

     // INTC module initialization
    intcContext.eventhandlerRecord = EventHandler;
//    intcContext.eventhandlerRecord = NULL;
    intcContext.numEvtEntries      = 10;
  //  intcContext.numEvtEntries      = 0;
    if (CSL_intcInit(&intcContext) != CSL_SOK)
    {
        printf("Error: GEM-INTC initialization failed\n");
        return 1;
    }

    // Enable NMIs
    if (CSL_intcGlobalNmiEnable() != CSL_SOK)
    {
        printf("Error: GEM-INTC global NMI enable failed\n");
        return 2;
    }

//     Enable global interrupts
    if (CSL_intcGlobalEnable(&state) != CSL_SOK)
    {
        printf ("Error: GEM-INTC global enable failed\n");
        return 3;
    }

    /* Open the INTC Module for Vector ID: 5 and Event ID: 90 (IPC_LOCAL in C6678)*/
    vectId = CSL_INTC_VECTID_5;
    hTest = CSL_intcOpen (&intcObj, 91, &vectId , NULL);
    if (hTest == NULL)
    {
        printf("Error: GEM-INTC Open failed\n");
        return 4;
    }

    /* Register an call-back handler which is invoked when the event occurs. */
/*
	if (CSL_chipReadReg (CSL_CHIP_DNUM) == 2)
		EventRecord.handler = &int2Isr;
	if (CSL_chipReadReg (CSL_CHIP_DNUM) == 3)
		EventRecord.handler = &int3Isr;
*/
    EventRecord.handler = &intIsr;
    EventRecord.arg = 0;
    if (CSL_intcPlugEventHandler(hTest,&EventRecord) != CSL_SOK)
    {
        printf("Error: GEM-INTC Plug event handler failed\n");
        return 5;
    }

    /* Enabling the events. */
    if (CSL_intcHwControl(hTest,CSL_INTC_CMD_EVTENABLE, NULL) != CSL_SOK)
    {
        printf("Error: GEM-INTC CSL_INTC_CMD_EVTENABLE command failed\n");
        return 6;
    }

    printf ("Debug: GEM-INTC Configuration Completed\n");


	// 1. Init Flag

//	printf("a = %d\n",a);


	// 3. Wait for entering into ISR
//	while(a!=1){}


//	printf("a = %d\n",a);
//	printf("GPIO interrupt occurs\n");

	return 0;
}


//void event91Handler(CSL_IntcHandle hIntc)
void event91Handler(void *eventId){



//	ri++;
}


void nSetupInterrupt(void)
{
   CSL_IntcObj intcObj63;
//   CSL_IntcGlobalEnableState state;

   CSL_IntcContext context;

//   CSL_Status intStat;

   CSL_IntcParam vectId;

   context.numEvtEntries = 0;
   context.eventhandlerRecord = NULL;
   if (CSL_intcInit(&context) != CSL_SOK)
   {
       printf("Error: GEM-INTC initialization failed\n");
       return ;
   }
//   CSL_intcInit(&context);

   CSL_intcGlobalNmiEnable();
//   intStat = CSL_intcGlobalEnable(&state);



   vectId = CSL_INTC_VECTID_4;
   hIntc91 = CSL_intcOpen (&intcObj63, 91, &vectId, NULL);

   if (hIntc91 == NULL)
    {
        printf("Error: GEM-INTC Open failed\n");
        return ;
    }
   //hIntc91 = CSL_intcOpen (&intcObj63, CSL_INTC_EVENTID_91, &vectId, NULL);

   EventRecord.handler = &event91Handler;
   EventRecord.arg = hIntc91;

   if (CSL_intcPlugEventHandler(hTest,&EventRecord) != CSL_SOK)
      {
          printf("Error: GEM-INTC Plug event handler failed\n");
          return ;
      }

//   CSL_intcPlugEventHandler(hIntc91,&EventRecord);

   if (CSL_intcHwControl(hTest,CSL_INTC_CMD_EVTENABLE, NULL) != CSL_SOK)
   {
       printf("Error: GEM-INTC CSL_INTC_CMD_EVTENABLE command failed\n");
//       return 6;
   }

//   CSL_intcHwControl(hIntc91,CSL_INTC_CMD_EVTENABLE,NULL);
   printf ("Debug: GEM-INTC Configuration Completed\n");

//   CSL_IntcClose(hIntc63);
}

//void event91
