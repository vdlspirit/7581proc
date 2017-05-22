#include "mycore.h"
#include "math.h"
#include "ti/csl/src/intc/csl_intc.h"
#include <stdio.h>
#include <ti/sysbios/family/c66/Cache.h>
#include <csl_ipcAux.h>
#include "signalProc.h"
#include "signalProcCom.h"

extern BattleMathT2K1 battlemathT2k1;

extern ALGINFO alginf;
extern VisualStatusPaketProcPar vsp211;
extern VisualStatusPaketProcPar vsp212;

int setInterrupt(void);
extern TATOOED ta2;						//структура пакета ТА цель 2
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

//volatile unsigned char gcnt=0;
void MyStartCore5()
{
//	volatile unsigned int *InBuff;
//	volatile unsigned int *OutBuff;
//	int size;
	isSimpleThr=0;
	ndgtm=0;
	int retval;
	errtm=0;
	TSCLsav=0;
	int ksum=1;
	unsigned char bTargeting=1;
	short coordAngleQX;
	short coordAngleFY;
	double k01pixang=2.23;

	 volatile int nObjFin=0;
	int init=0,i;
	for(i=0;i<200000000;i++) {
				asm("  nop");
	//			asm
			}
	setInterrupt();
//	InBuff = (unsigned int*)(0x00800400);
//	OutBuff = (unsigned int*)(0x80000000);
//	nSetupInterrupt();
	isInitialized[71]=0xc16c000; // aim 2 signal frame
	isInitialized[81]=0xc1be000; //

//	isInitialized[71]=0xc124000; // aim 1 dark frame
	isInitialized[69]=0xc17e000; // aim 2 dark frame
	isInitialized[79]=0xc1c3000; //

//	isInitialized[69]=0xc136000; // aim 2 dark frame


	Cache_wb((void *) &isInitialized[69], 4, Cache_Type_L1D, TRUE);
	Cache_wb((void *) &isInitialized[71], 4, Cache_Type_L1D, TRUE);

	isInitialized[65]=0x01aa5502;
	//			CACHE_wbL1d ((void *) &isInitialized[8], 4, CACHE_WAIT);	Cache_wb((void *) &isInitialized[66], 4, Cache_Type_L1D, TRUE);
	//		Cache_inv((void *) &isInitialized[19], 4, Cache_Type_L1D, TRUE);
//	size=((isInitialized[19]>>16)&0xfff)*(isInitialized[19]&0xfff)*2;
//	rj=ri;
	isInitialized[65]=0x01aa5502;
	CACHE_wbL1d ((void *) &isInitialized[65], 4, CACHE_WAIT);
	pbgrnd=(UInt16 *)0xc17e000;

	while(1) {
		Cache_inv((void *) &battle, sizeof(BattleParam), Cache_Type_L1D, TRUE);
		if(battle2.STAGE2==sSTANDBY2) {
			k01pixang=0.1*3600*57.29578*0.0055/battle.FokusK1;
			break;
			}
		}

	for(;;)
	{

		 Cache_inv((void *) &isInitialized[65], 4, Cache_Type_L1D, TRUE);
		 init=isInitialized[65];
		 ksum=1;
		 if(init== 0x01000002) {
//		 if(1) {

				if(vkp.MathKommand.s.K_s_sh>0)
					alginf.ksmr=vkp.MathKommand.s.K_s_sh;
				else
					alginf.ksmr=50;
				if(vkp.MathKommand.s.K_s_sh>0)
					alginf.kbgrnd=vkp.res7;
				else
//					alginf.kbgrnd=20;
					alginf.kbgrnd=30;
				Cache_inv((void *) &battle2, sizeof(BattleParam2), Cache_Type_L1D, TRUE);
				if(battle2.STAGE2 == sBATTLE2) {
				//	alginf.command=(0x1 <<6) | (0x1 <<4) | (0x1 <<13) | (0x1 <<11);
					 Cache_inv((void *) &ta2.CommandTA1.s, 4, Cache_Type_L1D, TRUE);
						if(ta2.CommandTA1.s.res03) { // Test mode
								if(ta2.CommandTA1.s.res02)
									alginf.command=(0x1 <<6) | (0x1 <<4) | (0x1 <<13) | (0x1 <<11) ;
								else
									alginf.command=(0x1 <<6) | (0x1 <<4) | (0x1 <<13) | (0x1 <<11)  | (0x1 <<10) ;
								}
							else {
								if(ta2.Distance>6000) {
									alginf.command=(0x1 <<6) | (0x1 <<4) | (0x1 <<13) | (0x1 <<11) ;
									}
								else {
									alginf.command=(0x1 <<6) | (0x1 <<4) | (0x1 <<13) | (0x1 <<11)  | (0x1 <<10) ;
									}
								}
						if(battle2.BMODETARGET2==t2GrabChan1)
							ksum=2;
					}
				else
					alginf.command=vkp.MathKommand.s.Kommand;


//				signalProcess((void *)(0x800400),(void*)0xc100000,20000);
//				retval=signalProcessLC1((void *)(0x800400),(void*)0xc16c000,20000);
//				retval=signalProcessLC1((void *)(0x800400),(void*)0xc124000,20000);

				retval=signalProcessLC1((void *)(0x800400),(void*)0xc16c000,20000);

//				vsp.TimeJob111= alginf.tm;
/*
				vsp.TimeJob111= gcnt++;
				vsp.NOBJ111=alginf.nobj;
				vsp.NPointPorg111=alginf.npt;

				vsp.NOBJ112=alginf.nobj2;
				vsp.SUFon112= alginf.uimean;
				vsp.SKO112=alginf.uismr;
				vsp.CordX111=alginf.x;
				vsp.CordY111=alginf.y;
				vsp.CordX112=alginf.x2;
				vsp.CordY112=alginf.y2;
				vsp.LocUFon111=alginf.bgrnd;
				vsp.LocSKO111=alginf.rms;

				vsp.res111_1=alginf.sz;
				vsp.res112_1=alginf.niter;
*/

				if(alginf.x>200) {
					vsp211.TimeJob= 2*alginf.tm;

					vsp211.NOBJ=alginf.nobj;
					vsp211.NPointPorg=alginf.npt;

					vsp212.NOBJ=alginf.nobj2;
					vsp212.SUFon= alginf.uimean;
					vsp212.SKO=alginf.uismr;
					vsp211.CordX=alginf.x;
					vsp211.CordY=alginf.y;
					vsp212.CordX=alginf.x2;
					vsp212.CordY=alginf.y2;
					vsp211.LocUFon=alginf.bgrnd;
					vsp211.LocSKO=alginf.rms;

					vsp211.res_1=alginf.sz;
					vsp212.res_1=alginf.niter;
					}
				else {
					vsp211.TimeJob= 2*alginf.tm;

					vsp211.NOBJ=alginf.nobj;
					vsp211.NPointPorg=alginf.npt;

					vsp212.NOBJ=alginf.nobj2;
					vsp212.SUFon= alginf.uimean;
					vsp212.SKO=alginf.uismr;
					vsp211.CordX=alginf.x;
					vsp211.CordY=alginf.y;
					vsp212.CordX=alginf.x2;
					vsp212.CordY=alginf.y2;
					vsp211.LocUFon=alginf.bgrnd;
					vsp211.LocSKO=alginf.rms;

					vsp211.res_1=alginf.sz;
					vsp212.res_1=alginf.niter;
//					asm("  nop");

				}




				if(!ta2.CommandTA1.s.Pusk)
					Cache_wb((void *) &vsp, sizeof(VisualStatusPaket), Cache_Type_L1D, TRUE);


//				battle.reserv1=alginf.tm;
//				battle.reserv2=alginf.npt;
//				battle.reserv3=alginf.x2;
//				battle.reserv4=alginf.y2;
//				battle.reserv5=alginf.nobj2;
//				battle.reserv6=alginf.nobj;

				battlemathT2k1.reserv1=2*alginf.tm;
				battlemathT2k1.reserv2=alginf.x;
				battlemathT2k1.reserv3=alginf.y;
				battlemathT2k1.IndLO1=(retval>0);

				if(bTargeting == 1 || ksum==2) {
//					coordAngleQX=round(ta1.qk1/10);
//					coordAngleFY=round(ta1.fk1/10);
					coordAngleQX=ta2.qk1;
					coordAngleFY=ta2.fk1;
					}
				else {
//					coordAngleQX=round(battlemathT1k1.Target1CoordAngX1/10);
//					coordAngleFY=round(battlemathT1k1.Target1CoordAngY1/10);
					coordAngleQX=battlemathT2k1.Target2CoordAngX1;
					coordAngleFY=battlemathT2k1.Target2CoordAngY1;
					}
				bTargeting =1;

				battlemathT2k1.qd12= round((coordAngleQX)/10.0);			// датчик канала 1 по координате q
				battlemathT2k1.fd12 =round((coordAngleFY)/10.0);			// датчик канала 1 по координате fi

				if(battlemathT2k1.IndLO1) {
//					battlemathT2k1.Target2CoordAngX1=round(2.23*(double)(battle2.Target2CoordY1*10+ksum*alginf.y - battle2.VizirChan1_Y*10));	//
//					battlemathT2k1.Target2CoordAngY1=-round(2.23*(double)(battle2.Target2CoordX1*10+ksum*alginf.x - battle2.VizirChan1_X*10));	//
					battlemathT2k1.Target2CoordAngX1=round(k01pixang*(double)(battle2.Target2CoordY1*10+ksum*alginf.y - battle2.VizirChan1_Y*10));	//
					battlemathT2k1.Target2CoordAngY1=-round(k01pixang*(double)(battle2.Target2CoordX1*10+ksum*alginf.x - battle2.VizirChan1_X*10));	//

//					battlemathT2k1.qk12=round(0.1*(2.23*(double)(battle2.Target2CoordY1*10+ksum*alginf.y - battle2.VizirChan1_Y*10) - coordAngleQX));	// координатор канала 1 по координате q
//					battlemathT2k1.fk12=-round(0.1*(2.23*(double)(battle2.Target2CoordX1*10+ksum*alginf.x - battle2.VizirChan1_X*10) + coordAngleFY));	// координатор канала 1 по координате fi
					battlemathT2k1.qk12=round(0.1*(k01pixang*(double)(battle2.Target2CoordY1*10+ksum*alginf.y - battle2.VizirChan1_Y*10) - coordAngleQX));	// координатор канала 1 по координате q
					battlemathT2k1.fk12=-round(0.1*(k01pixang*(double)(battle2.Target2CoordX1*10+ksum*alginf.x - battle2.VizirChan1_X*10) + coordAngleFY));	// координатор канала 1 по координате fi
					if(ta2.CommandTA1.s.SoprLO1) {
							bTargeting =0;
							}
//					battlemathT2k1.Sizeq21=round(ksum*2.23*(double)(alginf.size>>8));		// угловой размер источника в канале 1 по координате  q
//					battlemathT2k1.Sizef21=round(ksum*2.23*(double)(alginf.size&0xff));		// угловой размер источника в канале 1 по координате  fi
					battlemathT2k1.Sizeq21=round(ksum*k01pixang*(double)(alginf.size>>8));		// угловой размер источника в канале 1 по координате  q
					battlemathT2k1.Sizef21=round(ksum*k01pixang*(double)(alginf.size&0xff));		// угловой размер источника в канале 1 по координате  fi

					battlemathT2k1.Ksp12=alginf.snr;								// отношение сигнал/порог в канале 1 ракета 1

					battlemathT2k1.US12=alginf.level;		// уровень сигнала в канале 1

					}
				else {
					battlemathT2k1.Sizeq21=0;		// угловой размер источника в канале 2 по координате  q
					battlemathT2k1.Sizef21=0;		// угловой размер источника в канале 2 по координате  fi
					battlemathT2k1.US12=0;		// уровень сигнала в канале 2
					battlemathT2k1.Ksp12=0;
					}


				Cache_wb((void *) &battlemathT2k1, sizeof(BattleMathT2K1), Cache_Type_L1D, TRUE);


				 Cache_inv((void *) &vsp, 64, Cache_Type_L1D, TRUE);
//				alginf.command=vkp.MathKommand.s.Kommand;
				isInitialized[65]=0x01aa5502;
				CACHE_wbL1d ((void *) &isInitialized[65], 4, CACHE_WAIT);

				//isInitialized[71]=0xc16c000; isInitialized[81]=0xc1be000; isInitialized[69]=0xc17e000; 	isInitialized[79]=0xc1c3000;

				if(ksum==1) {
					zoom2_144((unsigned short *)0xc16c000,(unsigned short *)0xc1be000);
					zoom2_144((unsigned short *)0xc17e000,(unsigned short *)0xc1c3000);
					}

			}

	}
}
/*
#pragma CODE_SECTION(intIsr, ".csl_vect");

void intIsr(void *eventId )
{
//	a = 1;
//	rj++;
	return ;
}
*/
extern CSL_IntcContext             intcContext;
extern CSL_IntcEventHandlerRecord  EventHandler[30];
extern CSL_IntcObj                 intcObj;
extern CSL_IntcHandle              hTest;
extern CSL_IntcHandle      		hIntc91;
extern CSL_IntcGlobalEnableState   state;
extern CSL_IntcEventHandlerRecord  EventRecord;
extern CSL_IntcParam               vectId;


void event91Handler(void *eventId);
//void event91Handler(CSL_IntcHandle hIntc)
/*
void event91Handler(void *eventId){


//	ri++;
}
*/



