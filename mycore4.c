#include "mycore.h"
#include "math.h"
#include "ti/csl/src/intc/csl_intc.h"
#include <stdio.h>
#include <ti/sysbios/family/c66/Cache.h>
#include <csl_ipcAux.h>
#include "signalProc.h"
#include "signalProcCom.h"
extern BattleMathT2K2 battlemathT2k2;
extern BattleParam2 battle2;			//боевая структура управления программой
extern ALGINFO alginf;
extern TATOOED ta2;						//структура пакета ТА цель 1
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
int memadd(void * dst,void * src,int size);

//volatile int bStop=1;
void MyStartCore4()
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
	double kpixang;
//	while(bStop);
	for(i=0;i<200000000;i++) {
//	for(i=0;i<400000000;i++) {
				asm("  nop");
	//			asm
			}
	setInterrupt();
//	InBuff = (unsigned int*)(0x00800400);
//	OutBuff = (unsigned int*)(0x80000000);
//	nSetupInterrupt();
	isInitialized[75]=0xc148000; // aim 1 signal frame
	isInitialized[85]=0xc1b4000; //
//	isInitialized[72]=0xc100000; // aim 1 dark frame
	isInitialized[73]=0xc15a000; // aim 1 dark frame
	isInitialized[83]=0xc1b9000; //

	Cache_wb((void *) &isInitialized[75], 4, Cache_Type_L1D, TRUE);
	Cache_wb((void *) &isInitialized[73], 4, Cache_Type_L1D, TRUE);

	isInitialized[67]=0x02aa5502;
	//			CACHE_wbL1d ((void *) &isInitialized[8], 4, CACHE_WAIT);	Cache_wb((void *) &isInitialized[66], 4, Cache_Type_L1D, TRUE);
	//		Cache_inv((void *) &isInitialized[19], 4, Cache_Type_L1D, TRUE);
//	size=((isInitialized[19]>>16)&0xfff)*(isInitialized[19]&0xfff)*2;
//	rj=ri;
	isInitialized[67]=0x02aa5502;
	CACHE_wbL1d ((void *) &isInitialized[66], 4, CACHE_WAIT);
	pbgrnd=(UInt16 *)0xc15a000;
	kpixang=3.42;
	while(1) {
		Cache_inv((void *) &battle2, sizeof(BattleParam), Cache_Type_L1D, TRUE);
		if(battle2.STAGE2==sSTANDBY2) {
			kpixang=(double)(3600.0*57.29578*0.0055)/battle.FokusK2;
			break;
			}
		}

	for(;;)
	{
//		CACHE_invL1d ((void *) &isInitialized[66], 4, CACHE_WAIT);

		 Cache_inv((void *) &isInitialized[67], 4, Cache_Type_L1D, TRUE);
		 init=isInitialized[67];

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
		 if(init== 0x02000002) {
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
//					alginf.ksmr=50;
					alginf.ksmr=50;
				if(vkp.MathKommand.s.K_s_sh>0)
					alginf.kbgrnd=vkp.res7;
				else
//					alginf.kbgrnd=20;
					alginf.kbgrnd=30;
//					alginf.kbgrnd=2;

				Cache_inv((void *) &battle2, sizeof(BattleParam2), Cache_Type_L1D, TRUE);
				if(battle2.STAGE2 == sBATTLE2) {
//					Cache_inv((void *) &battle, sizeof(BattleParam), Cache_Type_L1D, TRUE);
//					 Cache_inv((void *) &ta1.CommandTA1.s, 4, Cache_Type_L1D, TRUE);
					 Cache_inv((void *) &ta2.CommandTA1, sizeof(TATOOED), Cache_Type_L1D, TRUE);

//					alginf.command=(0x1 <<6) | (0x1 <<4) | (0x1 <<13) | (0x1 <<11);
//					if(ta2.CommandTA1.s.ResetK1)

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
					}
				else
					alginf.command=vkp.MathKommand.s.Kommand;
				ksum=2;
				if(battle2.BMODETARGET2==t2ConvoyChan1_2 || battle2.BMODETARGET2==t2ConvoyChan2)
						ksum=1;

				 Cache_inv((void *) &isInitialized[17], 4, Cache_Type_L1D, TRUE);
				 size=isInitialized[17];
				 if( size== ((192<<16) +192)) {
//				 if( size== 0xc000c0)
					if( ta2.CommandTA1.s.res03 && ta2.CommandTA1.s.res01)
							alginf.command|=(0x1 <<14);
					else
							alginf.command&=~(0x1 <<14);
					 retval=signalProcess((void *)(0x800400),(void*)0xc148000,20000);
				 	 }
				 if( size== ((184<<16) +184)) {
					 retval=signalProcessLC2((void *)(0x800400),(void*)0xc148000,20000);
				 	 }

				vsp.TimeJob221=2*alginf.tm;
				vsp.NOBJ221=alginf.nobj;
				vsp.NPointPorg221=alginf.npt;

				vsp.NOBJ222=alginf.nobj2;
				vsp.SUFon222= alginf.uimean;
				vsp.SKO222=alginf.uismr;
				vsp.CordX221=alginf.x;
				vsp.CordY221=alginf.y;
				vsp.CordX222=alginf.x2;
				vsp.CordY222=alginf.y2;
				vsp.LocUFon221=alginf.bgrnd;
				vsp.LocSKO221=alginf.rms;

				vsp.res221_1=alginf.sz;
				vsp.res222_1=alginf.niter;

//				battlemathT2k2.reserv1=alginf.;
				battlemathT2k2.reserv4=2*alginf.tm;
				battlemathT2k2.reserv5=alginf.x;
				battlemathT2k2.reserv6=alginf.y;
				battlemathT2k2.IndLO2=(retval>0);

				if(bTargeting == 1 || ksum==2) {
					coordAngleQX=4*ta2.qk2;
					coordAngleFY=4*ta2.fk2;
					}
				else {
					coordAngleQX=battlemathT2k2.Target2CoordAngX2;
					coordAngleFY=battlemathT2k2.Target2CoordAngY2;
					}
				bTargeting =1;

				battlemathT2k2.qd22= coordAngleQX;			// датчик канала 2 по координате q
				battlemathT2k2.fd22 =coordAngleFY;			// датчик канала 2 по координате fi



				if(battlemathT2k2.IndLO2) {

//					battlemathT2k2.Target2CoordAngX2=round(1.368*(double)(battle2.Target2CoordY2*10+ksum*alginf.y - battle2.VizirChan2_Y*10));
//					battlemathT2k2.Target2CoordAngY2=-round(1.368*(double)(battle2.Target2CoordX2*10+ksum*alginf.x - battle2.VizirChan2_X*10));
					battlemathT2k2.Target2CoordAngX2=round(0.4*kpixang*(double)(battle2.Target2CoordY2*10+ksum*alginf.y - battle2.VizirChan2_Y*10));
					battlemathT2k2.Target2CoordAngY2=-round(0.4*kpixang*(double)(battle2.Target2CoordX2*10+ksum*alginf.x - battle2.VizirChan2_X*10));

//					battlemathT2k2.qk22=round(1.368*(double)(battle2.Target2CoordY2*10+ksum*alginf.y - battle2.VizirChan2_Y*10) - coordAngleQX);	// координатор канала 2 по координате q
//					battlemathT2k2.fk22=-round(1.368*(double)(battle2.Target2CoordX2*10+ksum*alginf.x - battle2.VizirChan2_X*10) + coordAngleFY);	// координатор канала 2 по координате fi
					battlemathT2k2.qk22=round(0.4*kpixang*(double)(battle2.Target2CoordY2*10+ksum*alginf.y - battle2.VizirChan2_Y*10) - coordAngleQX);	// координатор канала 2 по координате q
					battlemathT2k2.fk22=-round(0.4*kpixang*(double)(battle2.Target2CoordX2*10+ksum*alginf.x - battle2.VizirChan2_X*10) + coordAngleFY);	// координатор канала 2 по координате fi


					if(ta2.CommandTA1.s.SoprLO2) {
						bTargeting =0;
						}

//					battlemathT2k2.Sizeq22=round(ksum*3.4*(double)(alginf.size>>8));		// угловой размер источника в канале 2 по координате  q
//					battlemathT2k2.Sizef22=round(ksum*3.4*(double)(alginf.size&0xff));		// угловой размер источника в канале 2 по координате  fi
					battlemathT2k2.Sizeq22=round(ksum*kpixang*(double)(alginf.size>>8));		// угловой размер источника в канале 2 по координате  q
					battlemathT2k2.Sizef22=round(ksum*kpixang*(double)(alginf.size&0xff));		// угловой размер источника в канале 2 по координате  fi
					battlemathT2k2.Ksp22=alginf.snr;								// отношение сигнал/порог в канале 2 ракета 1
					battlemathT2k2.US22=alginf.level;		// уровень сигнала в канале 2
					}
				else {
					battlemathT2k2.Sizeq22=0;		// угловой размер источника в канале 2 по координате  q
					battlemathT2k2.Sizef22=0;		// угловой размер источника в канале 2 по координате  fi
					battlemathT2k2.US22=0;		// уровень сигнала в канале 2
					battlemathT2k2.Ksp22=0;
					}

//				battlemathT2k2.reserv5=alginf.nobj2;
//				battlemathT2k2.reserv6=alginf.nobj;
				Cache_wb((void *) &battlemathT2k2, sizeof(BattleMathT2K2), Cache_Type_L1D, TRUE);


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
				isInitialized[67]=0x02aa5502;
				CACHE_wbL1d ((void *) &isInitialized[67], 4, CACHE_WAIT);
//				IPCGR_REG[0]=1;
//				IPCGR_REG[1]=1;
				//isInitialized[75]=0xc148000; isInitialized[85]=0xc1b4000; 	isInitialized[73]=0xc15a000; isInitialized[83]=0xc1b9000;

				if(ksum==1) {
					zoom2_192((unsigned short *)0xc148000,(unsigned short *)0xc1b4000);
					zoom2_192((unsigned short *)0xc15a000,(unsigned short *)0xc1b9000);
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
/*
#pragma CODE_SECTION(intIsr, ".csl_vect");

void intIsr(void *eventId )
{
//	a = 1;
//	rj++;
	return ;
}
*/
/*
CSL_IntcContext             intcContext;
CSL_IntcEventHandlerRecord  EventHandler[30];
CSL_IntcObj                 intcObj;
CSL_IntcHandle              hTest;
CSL_IntcHandle      		hIntc91;
CSL_IntcGlobalEnableState   state;
CSL_IntcEventHandlerRecord  EventRecord;
CSL_IntcParam               vectId;
*/

//void event91Handler(CSL_IntcHandle hIntc)
void event91Handler(void *eventId);
/*
void event91Handler(void *eventId){



//	ri++;
}
*/

//void event91
