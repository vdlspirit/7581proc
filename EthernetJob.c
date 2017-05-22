/*
 * EthernetJob.c

 *
 *  Created on: 12.04.2014
 *      Author: Admin
 */
#include <stdio.h>
#include <math.h>
/* BIOS6 include */
#include <ti/sysbios/BIOS.h>
#include <ti/ndk/inc/netmain.h>
#include <ti/ndk/inc/_stack.h>
#include <ti/ndk/inc/tools/servers.h>
#include <ti/sysbios/knl/Task.h>
#include <ti/sysbios/family/c66/Cache.h>
#include <ti/sysbios/knl/Clock.h>
#include <ti/sysbios/knl/Event.h>




/* Platform utilities include */
#include "ti/platform/platform.h"

/* Resource manager for QMSS, PA, CPPI */
#include "ti/platform/resource_mgr.h"

#include "PaketOEDTAEth.h"
#include "BattleJob.h"
#include "gpio.h"
#include "srio.h"
#include "cmosis.h"
#include "spi.h"
#include "flash_programm.h"

#pragma DATA_ALIGN(ta1,256)
#pragma DATA_ALIGN(oed1,256)
#pragma DATA_ALIGN(battle,256)
#pragma DATA_ALIGN(battle2,256)
#pragma DATA_ALIGN(cmos1config,256)
#pragma DATA_ALIGN(cmos2config,256)
#pragma DATA_ALIGN(vkp,256)
#pragma DATA_ALIGN(battlemathT1k1,256)
#pragma DATA_ALIGN(battlemathT1k2,256)
#pragma DATA_ALIGN(battlemathT2k1,256)
#pragma DATA_ALIGN(battlemathT2k2,256)

///Выравниваем Буфера в размер кратный 128
#pragma DATA_ALIGN(BuffOED1,128)
#pragma DATA_ALIGN(BuffTA1,128)
#pragma DATA_ALIGN(BuffOED2,128)
#pragma DATA_ALIGN(BuffTA2,128)

#pragma DATA_ALIGN(videotechparam,128)
#pragma DATA_ALIGN(videotechpacket,256)

#pragma DATA_ALIGN(ZKadr,128)

#pragma DATA_SECTION(battlemathT1k1, ".DataBuff")
#pragma DATA_SECTION(BuffOED1, ".DataBuff")
#pragma DATA_SECTION(battlemathT2k2, ".DataBuff")
#pragma DATA_SECTION(BuffTA1, ".DataBuff")
#pragma DATA_SECTION(battlemathT1k2, ".DataBuff")
#pragma DATA_SECTION(BuffOED2, ".DataBuff")
#pragma DATA_SECTION(battlemathT2k1, ".DataBuff")
#pragma DATA_SECTION(BuffTA2, ".DataBuff")
#pragma DATA_SECTION(BuffVideoParamPacket, ".DataBuff")
#pragma DATA_SECTION(ta1, ".DataBuff")
#pragma DATA_SECTION(oed1, ".DataBuff")
#pragma DATA_SECTION(battle, ".DataBuff")
#pragma DATA_SECTION(battle2, ".DataBuff")
#pragma DATA_SECTION(videotechparam, ".DataBuff")
#pragma DATA_SECTION(videotechpacket, ".DataBuff")
#pragma DATA_SECTION(vkp, ".DataBuff")
#pragma DATA_SECTION(vsp, ".DataBuff")
#pragma DATA_SECTION(ZKadr, ".DataBuff")
#pragma DATA_SECTION(cmos1config, ".DataBuff")
#pragma DATA_SECTION(cmos2config, ".DataBuff")
#pragma DATA_SECTION(GlobalVideoBuff, ".GlobalBuff")


#pragma DATA_ALIGN(vsp111,128)
#pragma DATA_SECTION(vsp111, ".DataBuff")
VisualStatusPaketProcPar vsp111;

#pragma DATA_ALIGN(vsp112,128)
#pragma DATA_SECTION(vsp112, ".DataBuff")
VisualStatusPaketProcPar vsp112;

#pragma DATA_ALIGN(vsp121,128)
#pragma DATA_SECTION(vsp121, ".DataBuff")
VisualStatusPaketProcPar vsp121;

#pragma DATA_ALIGN(vsp122,128)
#pragma DATA_SECTION(vsp122, ".DataBuff")
VisualStatusPaketProcPar vsp122;


#pragma DATA_ALIGN(vsp211,128)
#pragma DATA_SECTION(vsp211, ".DataBuff")
VisualStatusPaketProcPar vsp211;

#pragma DATA_ALIGN(vsp212,128)
#pragma DATA_SECTION(vsp212, ".DataBuff")
VisualStatusPaketProcPar vsp212;

#pragma DATA_ALIGN(vsp221,128)
#pragma DATA_SECTION(vsp221, ".DataBuff")
VisualStatusPaketProcPar vsp221;

#pragma DATA_ALIGN(vsp222,128)
#pragma DATA_SECTION(vsp222, ".DataBuff")
VisualStatusPaketProcPar vsp222;


extern volatile unsigned int cregister TSCL;
extern volatile unsigned int cregister TSCH;
extern void StartStopEth(int ss);
extern unsigned int GiveMePeriod(unsigned short chennal);
extern unsigned int GiveMeDeltaSinx();
extern Void funclktarget1 (UArg a0);
extern Void funclktarget2 (UArg a0);
extern Void funclk17target1 (UArg a0);
extern Void funclk17target2 (UArg a0);


extern unsigned int sinx1count;
extern unsigned int sinx2count;
extern unsigned int finishk1count;
extern unsigned int finishk2count;

extern unsigned short 	 sinx1;
extern unsigned short	 sinx2;
extern unsigned short	 blinksinx1;
extern unsigned short	 blinksinx2;
extern unsigned int BattleTimerSinx1;
extern unsigned int BattleTimerSinx2;

//настройка событий прихода таймеров
extern Event_Handle   eventclkhtarget1;
extern Event_Handle   eventclkhtarget2;

//настройки таймеров для выхода на обмен для цели 1 и 2
extern Clock_Handle clkhtarget1;
extern Clock_Params clkParamstarget1;

extern Clock_Handle clkhtarget2;
extern Clock_Params clkParamstarget2;

extern Clock_Handle clkh17target1;
extern Clock_Params clk17Paramstarget1;

extern Clock_Handle clkh17target2;
extern Clock_Params clk17Paramstarget2;

extern volatile DiagStruct DStruct;


Uint8 GlobalVideoBuff[0x800000+sizeof(VisualKadrPaket)];	//глобальный видео буффер 8Mб в DDR памяти

uint8 BuffOED1[SizeBuffOED];			//буффер на передачу покета ОЭД1
uint8 BuffTA1[SizeBuffTA];				//буффер на прием пакета ТА1
BattleMathT2K2 battlemathT2k2;
uint8 BuffOED2[SizeBuffOED];			//буффер на передачу покета ОЭД2
uint8 BuffTA2[SizeBuffTA];				//буффер на прием пакета ТА2
uint32 BuffVideoParamPacket[8];			//буффер параметров видео пакета

BattleMathT1K1 battlemathT1k1;
//Пакеты боевого обмена
TATOOED ta1;						//структура пакета ТА цель 1
OEDTOTA oed1;						//структура пакета ОЭД цель 1
extern TATOOED ta2;					//структура пакета ТА цель 2
extern OEDTOTA oed2;				//структура пакета ОЭД цель 2

BattleMathT1K2 battlemathT1k2;

int AbsBlock;		//блокировка всех портов кроме активного

uint GlobalNumDev=NUMBEROED;
uint GlobalVerPO=VerPO;

BattleMathT2K1 battlemathT2k1;

//цена пикселя в секундах в канале 1 и 2

double PIXELSECCHAN1;
double PIXELSECCHAN2;


VideoTechParam videotechparam;//структура пакета с параметрами технологического видео
VideoTechPacket videotechpacket;	//структура пакета с технологическим видео

BattleParam battle;					//боевая структура управления программой цель 1

VisualKommandPaket vkp;				//структура команд от визуализатора
VisualStatusPaket vsp;				//структура заголовка кадра
VisualKadrPaket ZKadr;				//заголовок кадра

CMOS cmos1config;	//регистры управления 1 матрицей

BattleParam2 battle2;				//боевая структура управления программой цель 2

CMOS cmos2config;	//регистры управления 2 матрицей


extern int timercount;
extern volatile UInt32*     isInitialized;
extern GPIOPACKET gpio;
extern SRIOPACKET srioUpac;
extern gpioStrobBlink();
uint8 FlagUpdatePaket = 0;	//обновить пакет перед обменом

uint32 GlobalTimeJob=0;			//глобальное время работы системы в секундах
uint32 GlobalTimeTick=0;			//глобальное время работы в текущем цикле в мс
uint32 GlobalBattleCount=0;

void NewVideoTechPaket()
{
	//заполнение пакета с параметрами кадра
	videotechparam.RKB = SizeKadrTechRejY;
	videotechparam.RKG = SizeKadrTechRejX;
	videotechparam.Word1.s.KBP = 8;
	videotechparam.Word2.s.TechRej = 15;
	videotechparam.Word2.s.TimeExp = 1;
	videotechparam.NPK = 0;
	videotechparam.kf2 = -775 * 4;
	videotechparam.kq2 = -775 * 4;
	videotechparam.df2 = 3.23 * 100;	//3,23 секунды при дискрете 0,01''
	videotechparam.dq2 = 3.23 * 100;	//3,23 секунды при дискрете 0,01''

	//заполнение пакета видео
	videotechpacket.IMD = 0xa5f0;
	videotechpacket.KSS = 2;

	Cache_wb((void *) &videotechpacket, sizeof(VideoTechPacket), Cache_Type_L1D, TRUE);
	Cache_wb((void *) &videotechparam, sizeof(VideoTechParam), Cache_Type_L1D, TRUE);
}
Void CycleDelay (Int32 count)
{
    UInt32                  TSCLin;

    if (count <= 0)
        return;

    /* Get the current TSC  */
    TSCLin = TSCL;

    while ((TSCL  - TSCLin) < (UInt32)count);
}

void CMOS_KONFIG(uint16 *buff)
{


	gpioWriteDataBus(MDIO_CMOS1_INIT);	//выбираем Cmos
	spi_init_cmos_oed(0);
	gpioWriteDataBus(MDIO_CMOS2_INIT);	//выбираем Cmos
	spi_init_cmos_oed(1);

}

void CreateVideoChanal2(int sizeX2,int sizeY2)
{
	int i=0;
		int GlobalZagolovok=sizeof(VideoTechPacket);
		uint32 * massvideo = (uint32*)(GlobalVideoBuff+GlobalZagolovok);

			uint32 * kadr2mem =(uint32*)(LOADKADR2MEM);
			uint32   combbyte=0;


			for(i=0;i<(sizeX2*sizeY2);i+=4)
			{
				combbyte= ((((*kadr2mem)&0x3FC)>>2)|((*kadr2mem>>10)&0xff00));
				combbyte|=((*(kadr2mem+1)&0x3FC)<<14)|((*(kadr2mem+1)&0x3fc0000)<<6);
				*massvideo=combbyte;
				kadr2mem+=2;
				massvideo++;

			}
}

void CreateVideo(int sizeX1,int sizeY1,int sizeX2,int sizeY2,int type)
{
	int i=0;
	int GlobalZagolovok=sizeof(VisualKadrPaket);
	uint32 time1=0,time2=0;
	uint32 * massvideo = (uint32*)(GlobalVideoBuff+GlobalZagolovok);
	time1=TSCL;
	time2=TSCH;

	if (type==0)						//формируем 8-битное изображение из 10 - битного
	{
		uint32 * kadr1mem =(uint32*)(LOADKADR1MEM);
		uint32 * kadr2mem =(uint32*)(LOADKADR2MEM);
		uint32   combbyte=0;


		for(i=0;i<(sizeX1*sizeY1);i+=4)
		{
			combbyte= ((((*kadr1mem)&0x3FC)>>2)|((*kadr1mem>>10)&0xff00));
			combbyte|=((*(kadr1mem+1)&0x3FC)<<14)|((*(kadr1mem+1)&0x3fc0000)<<6);
			*massvideo=combbyte;
			kadr1mem+=2;
			massvideo++;

		}
		for(i=0;i<(sizeX2*sizeY2);i+=4)
		{
			combbyte= ((((*kadr2mem)&0x3FC)>>2)|((*kadr2mem>>10)&0xff00));
			combbyte|=((*(kadr2mem+1)&0x3FC)<<14)|((*(kadr2mem+1)&0x3fc0000)<<6);
			*massvideo=combbyte;
			kadr2mem+=2;
			massvideo++;

		}
	}
	else
	{
		memcpy((void *)massvideo,(void*)LOADKADR1MEM,sizeX1*sizeY1*2);
		massvideo = (uint32*)(GlobalVideoBuff+GlobalZagolovok+sizeX1*sizeY1*2);
		memcpy((void *)massvideo,(void*)LOADKADR2MEM,sizeX2*sizeY2*2);

	}

	time1=TSCL-time1;
	time2=TSCH-time2;
}

void InitPaketBattleJob()
{
	uint32* base=(uint32*)FRAM_PARAM_OED;
	UInt16 test;

	test=gpioReadDataBus();

	gpioWriteDataBus(test|0x01);		//переключаемся для работы с FRAM
	if(*base==0xa5b6c7d8)
	{
		GlobalTimeJob=*(base+1);
		GlobalBattleCount=*(base+2);
	}
	else
	{
		*base=0xa5b6c7d8;
		CycleDelay(1000);
		*(base+1)=0;
		CycleDelay(1000);
		*(base+2)=0;
		CycleDelay(1000);
	}
	gpioWriteDataBus(test);
	PIXELSECCHAN1=	(((atan(PIXEL/(FOKUSK1*1000)))*180)/PI)*3600;
	PIXELSECCHAN2=	(((atan(PIXEL/(FOKUSK2*1000)))*180)/PI)*3600;

	oed1.SignalOED1.s.NumTarget=0;		//Номер цели
	oed1.FokusK1 = FOKUSK1 * 10;		//Фокус канала 1
	oed1.FokusK2 = FOKUSK2 * 10;		//фокус канала 2
	oed1.NumPaket = 0;					//Номер текущего пакета
	oed1.OEDVerPO = VerPO;				//версия программного обеспечения
	oed1.Temper = 25;					//текущая температура
	oed1.SignalOED1.s.TNorm = 1;		//температура в норме
	oed1.SignalOED1.s.Healthy = 0;		//Исправен

	oed2.SignalOED1.s.NumTarget=1;		//Номер цели
	oed2.FokusK1 = FOKUSK1 * 10;		//Фокус канала 1
	oed2.FokusK2 = FOKUSK2 * 10;		//фокус канала 2
	oed2.NumPaket = 0;					//Номер текущего пакета
	oed2.OEDVerPO = VerPO;				//версия программного обеспечения
	oed2.Temper = 25;					//текущая температура
	oed2.SignalOED1.s.TNorm = 1;		//температура в норме
	oed2.SignalOED1.s.Healthy = 0;		//Исправен

	battle.STAGE = sSTOP;
	battle2.STAGE2 = sSTOP2;
	battle.BMODETARGET1 = t1STOP;	//режим работы не боевой
	battle2.BMODETARGET2 = t2STOP;
	battle.VizirChan1_X=VizirChan1X;//настраиваем центр визирной оси
	battle.VizirChan1_Y=VizirChan1Y;
	battle.VizirChan2_X=VizirChan2X;
	battle.VizirChan2_Y=VizirChan2Y;
	battle.FokusK1=FOKUSK1;
	battle.FokusK2=FOKUSK2;
	battle2.VizirChan1_X=VizirChan1X;//настраиваем центр визирной оси
	battle2.VizirChan1_Y=VizirChan1Y;
	battle2.VizirChan2_X=VizirChan2X;
	battle2.VizirChan2_Y=VizirChan2Y;
	battle2.FokusK1=FOKUSK1;
	battle2.FokusK2=FOKUSK2;


	//выкачиваем содержимое flash программ точнее блок параметров если есть
	test=gpioReadDataBus();
	gpioWriteDataBus(test|0x78);
	spi_cmos_read_flash(1400, 0,(uint16_t*)ADDRRAMJOBFLASHPROGRAMM, MAXSIZEPROGRAMCPU-1400);
	gpioWriteDataBus(test);
	//и перезаписываем некоторые глобальные параметры.

	uint8* blockparamprosh=(uint8*)ADDRRAMJOBFLASHPROGRAMM;

	int count=0;
	int chkXor=0;
	for(count=0;count<(SIZEPAKETUDPFLASHREAD-1);count++)
	{
		chkXor^=*(blockparamprosh+count);
	}

	uint32* param=(uint32*)ADDRRAMJOBFLASHPROGRAMM;
	if((chkXor==0)&&(*(blockparamprosh+count)==0xa5))
	{
		double fokus1=0;
		double fokus2=0;
		fokus1=(double)(*(param+3))/10;
		fokus2=(double)(*(param+4))/10;
		battle.FokusK1=battle2.FokusK1=fokus1;
		battle.FokusK2=battle2.FokusK2=fokus2;

		oed1.OEDVerPO=oed2.OEDVerPO=(uint16)((*(param+1))&0x000000ff);
		GlobalNumDev=*param;
		GlobalVerPO=*(param+1);

		DStruct.XeCh1=*(param+5);
		DStruct.YeCh1=*(param+6);
		DStruct.XeCh2=*(param+7);
		DStruct.YeCh2=*(param+8);
	}

	Cache_wb((void *) &DStruct, sizeof(DiagStruct), Cache_Type_L1D, TRUE);
	Cache_wb((void *) &battle, sizeof(BattleParam), Cache_Type_L1D, TRUE);
	Cache_wb((void *) &battle2, sizeof(BattleParam2), Cache_Type_L1D, TRUE);
	Cache_wb((void *) &oed1, sizeof(OEDTOTA), Cache_Type_L1D, TRUE);
	Cache_wb((void *) &oed2, sizeof(OEDTOTA), Cache_Type_L1D, TRUE);


	//настраиваем таймера обмена
	Clock_Params_init(&clkParamstarget1);
	clkParamstarget1.period    = 0;		//однократный вызов
	clkParamstarget1.startFlag = FALSE;	//начинаем по команде
	clkhtarget1 = Clock_create(funclktarget1, 24, &clkParamstarget1, NULL);
	//ориентировочно таймера заряжены на 12 мс зависит от длины системного тика

	Clock_Params_init(&clkParamstarget2);
	clkParamstarget2.period    = 0;
	clkParamstarget2.startFlag = FALSE;	//начинаем по команде
	clkhtarget2 = Clock_create(funclktarget2, 24, &clkParamstarget2, NULL);

	//настраиваем таймера обмена
	Clock_Params_init(&clk17Paramstarget1);
	clk17Paramstarget1.period    = 0;		//однократный вызов
	clk17Paramstarget1.startFlag = FALSE;	//начинаем по команде
	clkh17target1 = Clock_create(funclk17target1, 32, &clk17Paramstarget1, NULL);
	//ориентировочно таймера заряжены на 16 мс зависит от длины системного тика

	Clock_Params_init(&clk17Paramstarget2);
	clk17Paramstarget2.period    = 0;
	clk17Paramstarget2.startFlag = FALSE;	//начинаем по команде
	clkh17target2 = Clock_create(funclk17target2, 32, &clk17Paramstarget2, NULL);

	//Настраиваем события
	eventclkhtarget1=Event_create(NULL, NULL);
	eventclkhtarget2=Event_create(NULL, NULL);

}

uint8 Target1MassSinx1sec[50];
uint8 Count1Sinx1sec=0;
uint8 Target2MassSinx1sec[50];
uint8 Count2Sinx1sec=0;

void InitPaketTech()
{
	memset((void *) &Target1MassSinx1sec, 1, sizeof(Target1MassSinx1sec));
	memset((void *) &Target2MassSinx1sec, 1, sizeof(Target2MassSinx1sec));
	memset((void *) &videotechparam, 0, sizeof(VideoTechParam));
	memset((void *) &videotechpacket, 0, sizeof(VideoTechPacket));
	memset((void *) &vkp, 0, sizeof(VisualKommandPaket));
	memset((void *) &vsp, 0, sizeof(VisualStatusPaket));
	memset((void *) &ZKadr, 0, sizeof(VisualKadrPaket));
	memset((void *) &GlobalVideoBuff, 0, sizeof(GlobalVideoBuff));
	memset((void *) isInitialized, 0, 0x400);
	Cache_wb((void *) &isInitialized[0], 0x400, Cache_Type_L1D, TRUE);

	//настраиваем адреса памяти для хранения и обработки полученных данных в MCSRAM

	int sizechan1	=	288*288*2;		//максимальный размер поля анализа без суммирования в канале 1
	int sizechan2	=	368*368*2;		//максимальный размер поля анализа без суммирования в канале 2

	isInitialized[68]=MCSRAMMYVIDEO;					//темновой кадр канала 1 цели 1 288X288
	isInitialized[69]=isInitialized[68]+sizechan1;		//темновой кадр канала 1 цели 2
	isInitialized[70]=isInitialized[69]+sizechan1;		//кадр с импульсом канала 1 цели 1
	isInitialized[71]=isInitialized[70]+sizechan1;		//кадр с импульсом канала 1 цели 2

	isInitialized[72]=isInitialized[71]+sizechan1;		//темновой кадр канала 2 цели 1 368X368
	isInitialized[73]=isInitialized[72]+sizechan2;		//темновой кадр канала 2 цели 2 368X368
	isInitialized[74]=isInitialized[73]+sizechan2;		//кадр с импульсом  канала 2 цели 1 368X368
	isInitialized[75]=isInitialized[74]+sizechan2;		//кадр с импульсом  канала 2 цели 2 368X368

	Cache_wb((void *) &isInitialized[64], 128, Cache_Type_L1D, TRUE);

	NewVideoTechPaket();

	AbsBlock=0;							//порты разблокированы для работы
}

uint16 countCh1=0;						//счетчики захватов
uint16 countCh2=0;
uint16 countCh1T2=0;						//счетчики захватов
uint16 countCh2T2=0;
uint32	NumInSinxXilinxOldCh1=0;			//счетчик полученных синхросигналов Xilinx
uint32	NumInSinxXilinxOldCh2=0;			//счетчик полученных синхросигналов Xilinx

void assembleBuffOED(uint16* Buff,int target,int size)
{
	uint8	NumInSinxXilinx=0;			//счетчик полученных синхросигналов Xilinx
	if(target==1)
	{
		uint32 NumClock=0;
		Cache_inv((void *) &oed1, sizeof(OEDTOTA), Cache_Type_L1D, TRUE);
		Cache_inv((void *) &isInitialized[0], 128, Cache_Type_L1D, TRUE);

		//oed1.SignalOED1.s.StrobK=blinksinx1;

		oed1.SignalOED1.s.Healthy=DStruct.Target1Ok;

		oed1.Tstrob=GiveMePeriod(1);

		if((sinx2!=0)&&(sinx1!=0))
		oed1.DeltaTstrob=GiveMeDeltaSinx();

		NumInSinxXilinx=(uint8)(isInitialized[10]&0xff);
		if(NumInSinxXilinx!=NumInSinxXilinxOldCh1)
		{
			NumInSinxXilinxOldCh1=NumInSinxXilinx;
			oed1.SignalOED1.s.StrobK=oed1.SignalOED1.s.StrobK^1;
			sinx1=1;
		}
		else
		{
			sinx1=0;
		}

		if(battle.BMODETARGET1==t1STOP)
		{
			if(Count1Sinx1sec>49)
				Count1Sinx1sec=0;

			if(sinx1>0)
			{
				Target1MassSinx1sec[Count1Sinx1sec]=0;
			}
			else
			{
				Target1MassSinx1sec[Count1Sinx1sec]=1;
			}
			Count1Sinx1sec++;

			int i=0;

			for(i=0;i<50;i++)
			{
				NumClock+=Target1MassSinx1sec[i];
			}

			sinx1=0;

			if((oed1.SignalOED1.s.Healthy==1)&&(NumClock<6))
			{
				oed1.SignalOED1.s.Ready=1;
			}
			else
			{
				if(oed1.SignalOED1.s.Ready==1)
					memset((void *) &Target1MassSinx1sec, 1, sizeof(Target1MassSinx1sec));

				oed1.SignalOED1.s.Ready=0;
			}
		}

	//	Cache_inv((void *) &isInitialized[0], 128, Cache_Type_L1D, TRUE);
		oed1.Temper = (((uint32)isInitialized[23]>>16)&0xff);
		oed1.Ntime=(GlobalTimeJob/3600);					//ВЫВОДИМ на верх часы
		oed1.Ncycle=GlobalBattleCount;
		oed1.SignalOED1.s.Light = (((uint32)isInitialized[20]>>11)&0x1);
		oed1.SignalOED1.s.TNorm = 1;

		switch(battle.BMODETARGET1)
		{
			case t1STOP:
			oed1.SignalOED1.s.IndLO1=battlemathT1k1.IndLO1=0;
			oed1.SignalOED1.s.IndLO2=battlemathT1k2.IndLO2=0;
			oed1.SignalOED1.s.UZK2=0;
			countCh1=0;
			countCh2=0;
			break;

			case t1GrabChan1:
			{
				if(battlemathT1k1.IndLO1==1)
				{
					countCh1++;
					if(countCh1>=3)
					{
						oed1.SignalOED1.s.IndLO1=1;
					}
				}
				else
				{
					if(countCh1<3)
						countCh1=0;

					oed1.SignalOED1.s.IndLO1=0;
				}

				if(battlemathT1k2.IndLO2==1)
				{
					countCh2++;
				}
				else
				{
					if(countCh2<3)
					{
						countCh2=0;
					}
				}
				oed1.SignalOED1.s.IndLO2=0;
				break;
			}

			case t1ConvoyChan1:
			{
			/*	if(battlemathT1k1.IndLO1==1)
				{
					countCh1++;
					if(countCh1>=3)
					{
						oed1.SignalOED1.s.IndLO1=1;
					}
				}
				else
				{
					if(countCh1<3)
						countCh1=0;

					oed1.SignalOED1.s.IndLO1=0;
				}*/
				oed1.SignalOED1.s.IndLO1=battlemathT1k1.IndLO1;
				if(battlemathT1k2.IndLO2==1)
				{
					countCh2++;
				}
				else
				{
					if(countCh2<3)
					{
						countCh2=0;
					}
				}
				oed1.SignalOED1.s.IndLO2=0;
				break;
			}

			case t1GrabChan2ConvoyChan1:
			{
				oed1.SignalOED1.s.IndLO1=battlemathT1k1.IndLO1;
			/*	if(battlemathT1k1.IndLO1==1)
				{
					countCh1++;
					if(countCh1>=3)
					{
						oed1.SignalOED1.s.IndLO1=1;
					}
				}
				else
				{
					if(countCh1<3)
						countCh1=0;

					oed1.SignalOED1.s.IndLO1=0;
				}*/

				if((ta1.qk2<=1200)&&(ta1.qk2>=-1200)&&(ta1.fk2<=2220)&&(ta1.fk2>=-1200))	//вошли во второе угловое поле
				{
					if(battlemathT1k2.IndLO2==1)
						{
							countCh2++;
							if(countCh2>=3)
								{
									oed1.SignalOED1.s.IndLO2=1;
								}
						}
						else
							{
								if(countCh2<3)
									countCh2=0;

								oed1.SignalOED1.s.IndLO2=0;
							}
				}
				else	oed1.SignalOED1.s.IndLO2=0;
				break;
			}
			case t1ConvoyChan1_2:
			{
				oed1.SignalOED1.s.IndLO1=battlemathT1k1.IndLO1;
				oed1.SignalOED1.s.IndLO2=battlemathT1k2.IndLO2;
				oed1.SignalOED1.s.UZK2=1;
				break;
			}
		}

		oed1.OEDVerPO = GlobalVerPO&0x000000ff;
		oed1.FokusK1 = battle.FokusK1*10;
		oed1.FokusK2 = battle.FokusK2*10;
		//корректируем для соответствия протоколу стыковки
		//oed1.NumPaket+=1;
		oed1.NumPaket=ta1.NumPaket;
		///////////////////////////////////////////////////

		oed1.Ksp1=battlemathT1k1.Ksp11;
		oed1.Ksp2=battlemathT1k2.Ksp21;
		oed1.Sizef11=battlemathT1k1.Sizef11;
		oed1.Sizef12=battlemathT1k2.Sizef12;
		oed1.Sizeq11=battlemathT1k1.Sizeq11;
		oed1.Sizeq12=battlemathT1k2.Sizeq12;
		oed1.US1=battlemathT1k1.US11;
		oed1.US2=battlemathT1k2.US21;

		oed1.fd1=battlemathT1k1.fd11;
		oed1.qd1=battlemathT1k1.qd11;
		oed1.fk1=battlemathT1k1.fk11;
		oed1.qk1=battlemathT1k1.qk11;

		oed1.fd2=battlemathT1k2.fd21;
		oed1.qd2=battlemathT1k2.qd21;
		oed1.fk2=battlemathT1k2.fk21;
		oed1.qk2=battlemathT1k2.qk21;

		oed1.reserv2=battlemathT1k1.reserv1;
		//oed1.reserv3=battlemathT1k1.reserv2;
		oed1.reserv4=battlemathT1k1.reserv3;
		oed1.reserv5=battlemathT1k2.reserv4;
		oed1.reserv6=battlemathT1k2.reserv5;
		oed1.reserv7=battlemathT1k2.reserv6;

		oed1.A1=battle.Target1SizeX1*PIXELSECCHAN1/10;
		oed1.A2=battle.Target1SizeX2*PIXELSECCHAN2;


		Cache_wb((void *) &oed1, sizeof(OEDTOTA), Cache_Type_L1D, TRUE);
		Cache_inv((void *) &oed1, sizeof(OEDTOTA), Cache_Type_L1D, TRUE);

		memcpy(Buff,&oed1,sizeof(OEDTOTA));
	}
	else
	{
		uint32 NumClock=0;
		Cache_inv((void *) &oed2, sizeof(OEDTOTA), Cache_Type_L1D, TRUE);
		Cache_inv((void *) &isInitialized[0], 128, Cache_Type_L1D, TRUE);

	//	oed2.SignalOED1.s.StrobK=blinksinx2;
		oed2.SignalOED1.s.Healthy=DStruct.Target2Ok;

		oed2.Tstrob=GiveMePeriod(2);

		if((sinx2!=0)&&(sinx1!=0))
			oed2.DeltaTstrob=GiveMeDeltaSinx();

		NumInSinxXilinx=(uint8)(isInitialized[12]&0xff);
		if(NumInSinxXilinx!=NumInSinxXilinxOldCh2)
		{
			NumInSinxXilinxOldCh2=NumInSinxXilinx;
			oed2.SignalOED1.s.StrobK=oed2.SignalOED1.s.StrobK^1;
			sinx2=1;
		}
		else
		{
			sinx2=0;
		}

		if(battle2.BMODETARGET2==t2STOP)
		{
			if(Count2Sinx1sec>49)
				Count2Sinx1sec=0;

			if(sinx2>0)
			{
				Target2MassSinx1sec[Count2Sinx1sec]=0;
			}
			else
			{
				Target2MassSinx1sec[Count2Sinx1sec]=1;
			}
			Count2Sinx1sec++;

			int i=0;

			for(i=0;i<50;i++)
			{
				NumClock+=Target2MassSinx1sec[i];
			}

			sinx2=0;

			if((oed2.SignalOED1.s.Healthy==1)&&(NumClock<6))
			{
				oed2.SignalOED1.s.Ready=1;
			}
			else
			{
				if(oed2.SignalOED1.s.Ready==1)
					memset((void *) &Target2MassSinx1sec, 1, sizeof(Target1MassSinx1sec));

				oed2.SignalOED1.s.Ready=0;
			}
		}

	//	Cache_inv((void *) &isInitialized[0], 128, Cache_Type_L1D, TRUE);
		oed2.Temper = (((uint32)isInitialized[23]>>16)&0xff);
		oed2.Ntime=(GlobalTimeJob/3600);					//ВЫВОДИМ на верх часы
		oed2.Ncycle=GlobalBattleCount;
		oed2.SignalOED1.s.Light = (((uint32)isInitialized[20]>>11)&0x1);
		oed2.SignalOED1.s.TNorm = 1;

		switch(battle2.BMODETARGET2)
		{
			case t2STOP:
			oed2.SignalOED1.s.IndLO1=battlemathT2k1.IndLO1=0;
			oed2.SignalOED1.s.IndLO2=battlemathT2k2.IndLO2=0;
			oed2.SignalOED1.s.UZK2=0;
			countCh1T2=0;
			countCh2T2=0;
			break;

			case t2GrabChan1:
				if(battlemathT2k1.IndLO1==1)
				{
					countCh1T2++;
					if(countCh1T2>=3)
					{
						oed2.SignalOED1.s.IndLO1=1;
					}
				}
				else
				{
					if(countCh1T2<3)
						countCh1T2=0;

					oed2.SignalOED1.s.IndLO1=0;
				}
				if(battlemathT2k2.IndLO2==1)
				{
					countCh2T2++;
				}
				else
				{
					if(countCh2T2<3)
					countCh2T2=0;
				}
				oed2.SignalOED1.s.IndLO2=0;
				break;

			case t2ConvoyChan1:

				oed2.SignalOED1.s.IndLO1=battlemathT2k1.IndLO1;
				/*if(battlemathT2k1.IndLO1==1)
				{
					countCh1T2++;
					if(countCh1T2>=3)
					{
						oed2.SignalOED1.s.IndLO1=1;
					}
				}
				else
				{
					if(countCh1T2<3)
						countCh1T2=0;

					oed2.SignalOED1.s.IndLO1=0;
				}*/

				if(battlemathT2k2.IndLO2==1)
				{
					countCh2T2++;
				}
				else
				{
					if(countCh2T2<3)
					countCh2T2=0;
				}
				oed2.SignalOED1.s.IndLO2=0;
				break;

			case t2GrabChan2ConvoyChan1:
				oed2.SignalOED1.s.IndLO1=battlemathT2k1.IndLO1;
				/*if(battlemathT2k1.IndLO1==1)
				{
					countCh1T2++;
					if(countCh1T2>=3)
					{
						oed2.SignalOED1.s.IndLO1=1;
					}
				}
				else
				{
					if(countCh1T2<3)
						countCh1T2=0;

					oed2.SignalOED1.s.IndLO1=0;
				}*/

				if((ta2.qk2<=1200)&&(ta2.qk2>=-1200)&&(ta2.fk2<=2220)&&(ta2.fk2>=-1200))	//вошли во второе угловое поле
				{
					if(battlemathT2k2.IndLO2==1)
						{
							countCh2T2++;
							if(countCh2T2>=3)
								{
									oed2.SignalOED1.s.IndLO2=1;
								}
						}
						else
							{
								if(countCh2T2<3)
									countCh2T2=0;

								oed2.SignalOED1.s.IndLO2=0;
							}
				}
				else	oed2.SignalOED1.s.IndLO2=0;
				break;
			case t2ConvoyChan1_2:
				oed2.SignalOED1.s.IndLO1=battlemathT2k1.IndLO1;
				oed2.SignalOED1.s.IndLO2=battlemathT2k2.IndLO2;
				oed2.SignalOED1.s.UZK2=1;
				break;
		}

		oed2.OEDVerPO = GlobalVerPO&0x000000ff;;
		oed2.FokusK1 = battle2.FokusK1 * 10;
		oed2.FokusK2 = battle2.FokusK2 * 10;
		//корректируем для соответствия протоколу стыковки
		//oed2.NumPaket+=1;
		oed2.NumPaket=ta2.NumPaket;
		//////////////////////////////////////////////////

		oed2.Ksp1=battlemathT2k1.Ksp12;
		oed2.Ksp2=battlemathT2k2.Ksp22;
		oed2.Sizef11=battlemathT2k1.Sizef21;
		oed2.Sizef12=battlemathT2k2.Sizef22;
		oed2.Sizeq11=battlemathT2k1.Sizeq21;
		oed2.Sizeq12=battlemathT2k2.Sizeq22;
		oed2.US1=battlemathT2k1.US12;
		oed2.US2=battlemathT2k2.US22;
		oed2.fd1=battlemathT2k1.fd12;
		oed2.qd1=battlemathT2k1.qd12;
		oed2.fk1=battlemathT2k1.fk12;
		oed2.qk1=battlemathT2k1.qk12;

		oed2.fd2=battlemathT2k2.fd22;
		oed2.qd2=battlemathT2k2.qd22;
		oed2.fk2=battlemathT2k2.fk22;
		oed2.qk2=battlemathT2k2.qk22;
		oed2.reserv2=battlemathT2k1.reserv1;
		//oed2.reserv3=battlemathT2k1.reserv2;
		oed2.reserv4=battlemathT2k1.reserv3;
		oed2.reserv5=battlemathT2k2.reserv4;
		oed2.reserv6=battlemathT2k2.reserv5;
		oed2.reserv7=battlemathT2k2.reserv6;

		oed2.A1=battle2.Target2SizeX1*PIXELSECCHAN1/10;	//размеры полей анализа
		oed2.A2=battle2.Target2SizeX2*PIXELSECCHAN2;

		Cache_wb((void *) &oed2, sizeof(OEDTOTA), Cache_Type_L1D, TRUE);
		Cache_inv((void *) &oed2, sizeof(OEDTOTA), Cache_Type_L1D, TRUE);

		memcpy(Buff,&oed2,sizeof(OEDTOTA));
	}

}

int disassembleBuffTA(uint16* Buff,int target, int size)
{

	if(target==1)
	{
		ta1.CommandTA1.sw = Buff[0];
		ta1.CommandTA2.sw = Buff[1];

		ta1.qk1 = ((int16)(Buff[2])) * 10;
		ta1.fk1 = ((int16)(Buff[3])) * 10;

		ta1.qk2 = (int16)Buff[4];
		ta1.fk2 = (int16)Buff[5];

		ta1.qPod = ((int16)(Buff[6]))* 2;
		ta1.fPod = ((int16)(Buff[7])) * 2;

		ta1.Distance = Buff[8];

		ta1.qGDU = ((int16)(Buff[9]))*0.1;
		ta1.fGDU = ((int16)(Buff[10]))*0.1;

		ta1.res1 = Buff[11];
		ta1.res2 = Buff[12];
		ta1.res3 = Buff[13];
		ta1.res4 = Buff[14];
		ta1.res5 = Buff[15];

		ta1.NumPaket = Buff[16];

		ta1.res6 = Buff[17];

		switch(battle.BMODETARGET1)
		{
			case t1GrabChan1:
				if(((ta1.CommandTA1.s.SoprLO1==1)&&(oed1.SignalOED1.s.IndLO1==1))||(BattleTimerSinx1>=200)||
						((ta1.qk1<=1470)&&(ta1.qk1>=-1470)&&(ta1.fk1<=2400)&&(ta1.fk1>=-1200)))
				{
					battle.BMODETARGET1=t1ConvoyChan1;
			    	battle.Target1SizeX1=144;				//цель 1 поле сопровождения
			    	battle.Target1SizeY1=144;

			    	battle.ControlState.s.SumChan1T1=0;	//выключено суммирование в канале 1 цели 1

				}
				break;
			case t1ConvoyChan1:
				if(((ta1.qk1<=1470)&&(ta1.qk1>=-1470)&&(ta1.fk1<=2400)&&(ta1.fk1>=-1200))||
					((oed1.fd1<=147)&&(oed1.fd1>=-147)&&(oed1.fd1<=240)&&(oed1.fd1>=-120)))	//вошли в первое угловое поле
				{
					battle.BMODETARGET1=t1GrabChan2ConvoyChan1;
				}
				break;
			case t1GrabChan2ConvoyChan1:
				if((ta1.CommandTA1.s.SoprLO2==1)&&(oed1.SignalOED1.s.IndLO2==1))
				{
					battle.BMODETARGET1=t1ConvoyChan1_2;
			    	battle.Target1SizeX1=144;				//цель 1 поле сопровождения
			    	battle.Target1SizeY1=144;
    	    		battle.Target1SizeX2=192;
    	    		battle.Target1SizeY2=192;

			    	battle.ControlState.s.SumChan1T1=0;	//выключено суммирование в канале 1 цели 1
			    	battle.ControlState.s.SumChan2T1=0;	//выключено суммирование в канале 2 цели 1
			    	countCh1=countCh2=0;
				}
				break;
			case t1ConvoyChan1_2:
			{
				battle.BMODETARGET1=t1ConvoyChan1_2;
		    	battle.Target1SizeX1=144;				//цель 1 поле сопровождения
		    	battle.Target1SizeY1=144;
	    		battle.Target1SizeX2=192;
	    		battle.Target1SizeY2=192;

		    	battle.ControlState.s.SumChan1T1=0;	//выключено суммирование в канале 1 цели 1
		    	battle.ControlState.s.SumChan2T1=0;	//выключено суммирование в канале 2 цели 1
			}
		}

		Cache_wb((void *) &ta1, sizeof(TATOOED), Cache_Type_L1D, TRUE);
		Cache_inv((void *) &ta1, sizeof(TATOOED), Cache_Type_L1D, TRUE);
	}
	else
	{
		ta2.CommandTA1.sw = Buff[0];
		ta2.CommandTA2.sw = Buff[1];

		ta2.qk1 = ((int16)(Buff[2])) * 10;
		ta2.fk1 = ((int16)(Buff[3])) * 10;

		ta2.qk2 = (int16)Buff[4];
		ta2.fk2 = (int16)Buff[5];

		ta2.qPod = ((int16)(Buff[6]))* 2;
		ta2.fPod = ((int16)(Buff[7])) * 2;

		ta2.Distance = Buff[8];

		ta2.qGDU = ((int16)(Buff[9]))*0.1;
		ta2.fGDU = ((int16)(Buff[10]))*0.1;

		ta2.res1 = Buff[11];
		ta2.res2 = Buff[12];
		ta2.res3 = Buff[13];
		ta2.res4 = Buff[14];
		ta2.res5 = Buff[15];

		ta2.NumPaket = Buff[16];

		ta2.res6 = Buff[17];

		switch(battle2.BMODETARGET2)
		{
			case t2GrabChan1:
				if(((ta2.CommandTA1.s.SoprLO1==1)&&(oed2.SignalOED1.s.IndLO1==1))||(BattleTimerSinx2>=200)||
						((ta2.qk1<=1470)&&(ta2.qk1>=-1470)&&(ta2.fk1<=2400)&&(ta2.fk1>=-1200)))
				{
					battle2.BMODETARGET2=t2ConvoyChan1;
			    	battle2.Target2SizeX1=144;				//цель 1 поле сопровождения
			    	battle2.Target2SizeY1=144;

			    	battle2.ControlState2.s.SumChan1T2=0;	//выключено суммирование в канале 1 цели 1
				}
				break;
			case t2ConvoyChan1:
				if(((ta2.qk1<=1470)&&(ta2.qk1>=-1470)&&(ta2.fk1<=2400)&&(ta2.fk1>=-1200))||
					((oed2.fd1<=147)&&(oed2.fd1>=-147)&&(oed2.fd1<=240)&&(oed2.fd1>=-120)))	//вошли в первое угловое поле
				{
					battle2.BMODETARGET2=t2GrabChan2ConvoyChan1;
				}
				break;
			case t2GrabChan2ConvoyChan1:
				if((ta2.CommandTA1.s.SoprLO2==1)&&(oed2.SignalOED1.s.IndLO2==1))
				{
					battle2.BMODETARGET2=t2ConvoyChan1_2;
			    	battle2.Target2SizeX1=144;				//цель 1 поле сопровождения
			    	battle2.Target2SizeY1=144;
    	    		battle2.Target2SizeX2=192;
    	    		battle2.Target2SizeY2=192;

			    	battle2.ControlState2.s.SumChan1T2=0;	//выключено суммирование в канале 1 цели 1
			    	battle2.ControlState2.s.SumChan2T2=0;	//выключено суммирование в канале 2 цели 1
			    	countCh1T2=countCh2T2=0;
				}
				break;
			case t2ConvoyChan1_2:
			{
				battle2.BMODETARGET2=t2ConvoyChan1_2;
		    	battle2.Target2SizeX1=144;				//цель 1 поле сопровождения
		    	battle2.Target2SizeY1=144;
	    		battle2.Target2SizeX2=192;
	    		battle2.Target2SizeY2=192;

		    	battle2.ControlState2.s.SumChan1T2=0;	//выключено суммирование в канале 1 цели 1
		    	battle2.ControlState2.s.SumChan2T2=0;	//выключено суммирование в канале 2 цели 1
			}
		}

		Cache_wb((void *) &ta2, sizeof(TATOOED), Cache_Type_L1D, TRUE);
		Cache_inv((void *) &ta2, sizeof(TATOOED), Cache_Type_L1D, TRUE);
	}

	return 1;
}

void AssembleBuffersTechVideo(uint8* Buff, int size) {
	uint16* parambuff;

	parambuff = (uint16*) Buff;

	//заполнение буффера параметрами видео пакета
	BuffVideoParamPacket[0] = videotechparam.Word1.sw;
	BuffVideoParamPacket[1] = videotechparam.Word2.sw;
	BuffVideoParamPacket[2] = videotechparam.NPK;
	BuffVideoParamPacket[3] = videotechparam.RKB + (videotechparam.RKG << 16);
	BuffVideoParamPacket[4] = (uint16)videotechparam.kq2 | (uint32)(videotechparam.kf2 << 16);
	BuffVideoParamPacket[5] = videotechparam.dq2 + (videotechparam.df2 << 16);
	BuffVideoParamPacket[6] = videotechparam.res1 + (videotechparam.res2 << 16);
	BuffVideoParamPacket[7] = videotechparam.res3 + (videotechparam.res4 << 16);

	//заполнение видеопакета
	parambuff[0] = videotechpacket.res1;
	parambuff[1] = videotechpacket.IMD;
	parambuff[2] = videotechpacket.NPK & 0xffff;
	parambuff[3] = videotechpacket.NPK >> 16;
	parambuff[4] = videotechpacket.KSS;
	parambuff[5] = videotechpacket.NPS;
	parambuff[6] = videotechpacket.res2 & 0xffff;
	parambuff[7] = videotechpacket.res2 >> 16;

//	Cache_wb((void *) &BuffVideoParamPacket[0], sizeof(BuffVideoParamPacket), Cache_Type_L1D, TRUE);
//	Cache_wb((void *) &parambuff[0], sizeof(parambuff), Cache_Type_L1D, TRUE);
}

#define PAKET_SIZE_VIDEO	1458		//размер максимального пакета возможного к передачи по Ethernet
int count=0;
int dtask_udp_visual(SOCKET s, UINT32 unused) {
	struct sockaddr_in sin1;
	struct timeval to;
	int i = 0, tmp = 0, size = 0;
	int sendsize=0;
	Uint32 localcountframestrob1=0,localcountframestrob2=0;
	(void) unused;
	volatile uint8  CadrImitLO=0;		//переменная характеризующая кадр с ЛО я отправляю или нет

	// Configure our socket timeout to be 3 seconds
	to.tv_sec = 10;
	to.tv_usec = 0;
	setsockopt(s, SOL_SOCKET, SO_RCVTIMEO, &to, sizeof(to));
	to.tv_sec = 10;
	to.tv_usec = 0;
	setsockopt(s, SOL_SOCKET, SO_SNDTIMEO, &to, sizeof(to));

	size = 7168;
	setsockopt(s, SOL_SOCKET, SO_SNDBUF, &size, sizeof(size));

	srioUpac.VideoRamK1=LOADKADR1MEM;
	srioUpac.VideoRamK2=LOADKADR2MEM;

	for (;;)
	{
		tmp = sizeof(sin1);
		i = (int) recvfrom(s, &vkp, sizeof(VisualKommandPaket), 0, (PSA) &sin1,	&tmp);
		count++;

		if(AbsBlock!=0)				//проверяем на блокировку порта
		    	break;


		if (i >= 0)
		{
			StartStopEth(1);
			if(vkp.KommandWord.s.VP==0)
			{

				spi_cmos_reset_enable();	//считать состояние матриц и перевести их в reset если необходимо
				if(!vkp.KommandWord.s.SM)
				{
					CreateVideo(vkp.DataWord1.s.SizeX1,vkp.DataWord1.s.SizeY1,
								vkp.DataWord2.s.SizeX2,vkp.DataWord2.s.SizeY2,vkp.ControlWord.s.B);
				}
				else
				{
					CreateVideo((vkp.DataWord1.s.SizeX1/2),(vkp.DataWord1.s.SizeY1/2),
													(vkp.DataWord2.s.SizeX2/2),(vkp.DataWord2.s.SizeY2/2),vkp.ControlWord.s.B);
				}


					vsp.NumKadr++;
					vsp.ControlWord1.s.B=vkp.ControlWord.s.B;		//разрядность 8 бит
					vsp.Data1.s.CoordX1=vkp.DataWord3.s.CoordX1;
					vsp.Data1.s.CoordY1=vkp.DataWord3.s.CoordY1;
					vsp.Data1.s.Exp1=vkp.DataWord3.s.Exp1;
					vsp.Data2.s.CoordX2=vkp.DataWord4.s.CoordX2;
					vsp.Data2.s.CoordY2=vkp.DataWord4.s.CoordY2;
					vsp.Data2.s.Exp2=vkp.DataWord4.s.Exp2;
					vsp.Data3.s.SizeX1=vkp.DataWord1.s.SizeX1;
					vsp.Data3.s.SizeY1=vkp.DataWord1.s.SizeY1;
					vsp.Data3.s.Temper=(((uint32)isInitialized[23]>>16)&0xff);
					vsp.Data4.s.SizeX2=vkp.DataWord2.s.SizeX2;
					vsp.Data4.s.SizeY2=vkp.DataWord2.s.SizeY2;
					vsp.Data5.s.Fokus1=oed1.FokusK1;
					vsp.Data5.s.Fokus2=oed1.FokusK2;
					vsp.Data6.s.Distance=vkp.DataWord5.s.Distance;
					vsp.Data6.s.NumPribor=GlobalNumDev;		//номер прибора

					//настраиваем управление дальностью
					double LocalDistanseNS=0;
					Uint16	NDelayXilinx=0;
					Uint16	NDelayXilinxKolStart=0;	//задержка поджига коллиматора

					LocalDistanseNS=(double)(DISTANSEDELAY+(double)((double)(2000000*(double)vsp.Data6.s.Distance)/(double)299792458))*1000;
					LocalDistanseNS-=3263.5;	//убираем минимальную задержку системы
					NDelayXilinxKolStart=(Uint16)(LocalDistanseNS/312.5);
					LocalDistanseNS-=5200;	//устанавливаем накопление по центру импульса
					NDelayXilinx=(Uint16)(LocalDistanseNS/312.5);

					//отрабатываем снятие кадра по импульсу синхронизации
					if((vkp.ControlWord.s.S1==1)&&(vkp.ControlWord.s.S2==0))
					{
						isInitialized[24]=NDelayXilinx;
						isInitialized[25]=0;
						//определяем необходимо ли включать коллиматор
						if(vkp.ControlWord.s.KI==1)
						{
							isInitialized[27]=NDelayXilinxKolStart;
							isInitialized[28]=0;
						}
						else
						{
							isInitialized[27]=isInitialized[28]=0;
						}
					}
					if((vkp.ControlWord.s.S1==0)&&(vkp.ControlWord.s.S2==1))
					{
						isInitialized[24]=0;
						isInitialized[25]=NDelayXilinx;
						if(vkp.ControlWord.s.KI==1)
						{
							isInitialized[27]=NDelayXilinxKolStart<<16;
							isInitialized[28]=0;
						}
						else
						{
							isInitialized[27]=isInitialized[28]=0;
						}
					}

					if((vkp.ControlWord.s.S1==1)||(vkp.ControlWord.s.S2==1))
					{
						isInitialized[26]=1;	//переключаем Xilinx на работу по синхросигналам
					}
					else
					{
						isInitialized[26]=0;
						if(vkp.ControlWord.s.KN==1)
						{
							isInitialized[27]=0;
							isInitialized[28]=12;	//задержка в 3,5 мкс от запроса
							vsp.ControlWord1.s.WithLO=0;
						}
						else
						{
							isInitialized[27]=isInitialized[28]=0;
							vsp.ControlWord1.s.WithLO=1;
						}
					}

					if(vkp.KommandWord.s.ImBS1==1)		//проводим имитацию боевой работы по первому синхросигналу
					{
						if(vkp.ControlWord.s.Imp==1)	//при синхронизации трактуем как снятие импульсного кадра
						{
							vsp.ControlWord1.s.WithLO=0;	//кадр без ответчика
						}
						else
						{
							isInitialized[24]=NDelayXilinx+3000;	//имитируем темновой кадр
							isInitialized[25]=0;
							isInitialized[27]=isInitialized[28]=0;
							vsp.ControlWord1.s.WithLO=1;	//кадр с ответчиком
						}
					}

					if(vkp.KommandWord.s.ImBS2==1)		//проводим имитацию боевой работы по второму синхросигналу
					{
						if(vkp.ControlWord.s.Imp==1)	//при синхронизации трактуем как снятие импульсного кадра
						{
							vsp.ControlWord1.s.WithLO=0;	//кадр без ответчика
						}
						else
						{
							isInitialized[24]=0;
							isInitialized[25]=NDelayXilinx+3000;	//имитируем темновой кадр
							isInitialized[27]=isInitialized[28]=0;
							vsp.ControlWord1.s.WithLO=1;	//кадр с ответчиком
						}
					}

					if((vkp.KommandWord.s.ImB==0)&&(vkp.KommandWord.s.ImBS1==0)&&(vkp.KommandWord.s.ImBS2==0))
					{
						vsp.ControlWord1.s.WithLO=0;
					}

////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////
					//копируем в память для мат. работы 4 ядер
					if((vkp.KommandWord.s.ImB==1)||(vkp.KommandWord.s.ImBS1==1)||(vkp.KommandWord.s.ImBS2==1))
					{
						//копирование кадра для канала 1
						if((vkp.DataWord1.s.SizeX1==vkp.DataWord1.s.SizeY1)&&((vkp.DataWord1.s.SizeX1==128)||(vkp.DataWord1.s.SizeX1==144)||(vkp.DataWord1.s.SizeX1==288)))
						{
							if(vsp.ControlWord1.s.WithLO == 0)	//темновой кадр канала 1
							{
								if(isInitialized[64]==0x01aa5501)	//запись по цели 1
								{
									if(isInitialized[68]!=0)
									{
										if(!vkp.KommandWord.s.SM)
											memcpy((void *)(isInitialized[68]),(void*)(isInitialized[1]),(vkp.DataWord1.s.SizeX1*vkp.DataWord1.s.SizeY1*2));
										else
											memcpy((void *)(isInitialized[68]),(void*)(isInitialized[1]),(vkp.DataWord1.s.SizeX1*vkp.DataWord1.s.SizeY1/2));

										isInitialized[64]=0x01005501;	//сбрасываем по кадру темновому
									}
								}
								if(isInitialized[65]==0x01aa5502)	//запись по цели 2
								{
									if(isInitialized[69]!=0)
									{
										if(!vkp.KommandWord.s.SM)
											memcpy((void *)(isInitialized[69]),(void*)(isInitialized[1]),(vkp.DataWord1.s.SizeX1*vkp.DataWord1.s.SizeY1*2));
										else
											memcpy((void *)(isInitialized[69]),(void*)(isInitialized[1]),(vkp.DataWord1.s.SizeX1*vkp.DataWord1.s.SizeY1/2));
										isInitialized[65]=0x01005502;	//сбрасываем по кадру с импульсом
									}
								}
							}
							else	//кадр с импульсом
							{
								if((isInitialized[64]==0x01aa5501)||(isInitialized[64]==0x01005501)) //запись по цели 1
								{
									if(isInitialized[70]!=0)
									{
										if(!vkp.KommandWord.s.SM)
											memcpy((void *)(isInitialized[70]),(void*)(isInitialized[1]),(vkp.DataWord1.s.SizeX1*vkp.DataWord1.s.SizeY1*2));
										else
											memcpy((void *)(isInitialized[70]),(void*)(isInitialized[1]),(vkp.DataWord1.s.SizeX1*vkp.DataWord1.s.SizeY1/2));
									}
									isInitialized[64]=0x01000001;	//сбрасываем по кадру с импульсом
								}
								if((isInitialized[65]==0x01aa5502)||(isInitialized[65]==0x01005502))	//запись по цели 2
								{
									if(isInitialized[71]!=0)
									{
										if(!vkp.KommandWord.s.SM)
											memcpy((void *)(isInitialized[71]),(void*)(isInitialized[1]),(vkp.DataWord1.s.SizeX1*vkp.DataWord1.s.SizeY1*2));
										else
											memcpy((void *)(isInitialized[71]),(void*)(isInitialized[1]),(vkp.DataWord1.s.SizeX1*vkp.DataWord1.s.SizeY1/2));
									}
									isInitialized[65]=0x01000002;	//сбрасываем по кадру с импульсом
								}
							}
						}

						//копирование кадра для канала 2
						if((vkp.DataWord2.s.SizeX2==vkp.DataWord2.s.SizeY2)&&((vkp.DataWord2.s.SizeX2==192)||(vkp.DataWord2.s.SizeX2==184)||(vkp.DataWord2.s.SizeX2==368)))
						{
							if(vsp.ControlWord1.s.WithLO == 0)	//темновой кадр канала 2
							{
								if(isInitialized[66]==0x02aa5501) //запись по цели 1
								{
									if(isInitialized[72]!=0)
									{
										if(!vkp.KommandWord.s.SM)
											memcpy((void *)(isInitialized[72]),(void*)(isInitialized[3]),(vkp.DataWord2.s.SizeX2*vkp.DataWord2.s.SizeY2*2));
										else
											memcpy((void *)(isInitialized[72]),(void*)(isInitialized[3]),(vkp.DataWord2.s.SizeX2*vkp.DataWord2.s.SizeY2/2));
										isInitialized[66]=0x02005501;
									}
								}
								if(isInitialized[67]==0x02aa5502) //запись по цели 2
								{
									if(isInitialized[73]!=0)
									{
										if(!vkp.KommandWord.s.SM)
											memcpy((void *)(isInitialized[73]),(void*)(isInitialized[3]),(vkp.DataWord2.s.SizeX2*vkp.DataWord2.s.SizeY2*2));
										else
											memcpy((void *)(isInitialized[73]),(void*)(isInitialized[3]),(vkp.DataWord2.s.SizeX2*vkp.DataWord2.s.SizeY2/2));
										isInitialized[67]=0x02005502;
									}
								}
							}
							else
							{
								if((isInitialized[66]==0x02aa5501)||(isInitialized[66]==0x02005501)) //запись по цели 1
								{
									if(isInitialized[74]!=0)
									{
										if(!vkp.KommandWord.s.SM)
											memcpy((void *)(isInitialized[74]),(void*)(isInitialized[3]),(vkp.DataWord2.s.SizeX2*vkp.DataWord2.s.SizeY2*2));
										else
											memcpy((void *)(isInitialized[74]),(void*)(isInitialized[3]),(vkp.DataWord2.s.SizeX2*vkp.DataWord2.s.SizeY2/2));
									}
									isInitialized[66]=0x02000001;
								}
								if((isInitialized[67]==0x02aa5502)||(isInitialized[67]==0x02005502)) //запись по цели 2
								{
									if(isInitialized[75]!=0)
									{
										if(!vkp.KommandWord.s.SM)
											memcpy((void *)(isInitialized[75]),(void*)(isInitialized[3]),(vkp.DataWord2.s.SizeX2*vkp.DataWord2.s.SizeY2*2));
										else
											memcpy((void *)(isInitialized[75]),(void*)(isInitialized[3]),(vkp.DataWord2.s.SizeX2*vkp.DataWord2.s.SizeY2/2));
									}
									isInitialized[67]=0x02000002;
								}
							}
						}
					}
////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////

					//определяем наличие синхронизиции
					if(sinx1count>0)
					{
						localcountframestrob1=0;
						sinx1count=0;
						vsp.ControlWord1.s.STROB1=1;
						vsp.STRPERIOD.s.STR1PERIOD=GiveMePeriod(1);
					}
					else
					{
						if(localcountframestrob1>11)
							vsp.ControlWord1.s.STROB1=0;
						localcountframestrob1++;
					}

					if(sinx2count>0)
					{
						localcountframestrob2=0;
						sinx2count=0;
						vsp.ControlWord1.s.STROB2=1;
						vsp.STRPERIOD.s.STR2PERIOD=GiveMePeriod(2);
					}
					else
					{
						if(localcountframestrob2>11)
							vsp.ControlWord1.s.STROB2=0;
						localcountframestrob2++;
					}

					if((vsp.ControlWord1.s.STROB1==1)&&(vsp.ControlWord1.s.STROB2==1))
					{
						vsp.STRDELAY.s.DelayStr1Str2=(uint32)GiveMeDeltaSinx();
					}

					vsp.StatVPUXilinx.sw=(uint32)isInitialized[20];
					vsp.TimeShtampGlobalX=(uint32)isInitialized[21];
					vsp.Tvpu=(uint32)isInitialized[22];
					vsp.Tvpu2=(uint32)isInitialized[23];

					//Проверяем наличие

					cmosxilinx_write_kadrpos();	//настраиваем положение 1024 байт строки на матрице

					if(vkp.KommandWord.s.SM)	//модифицируем координаты под суммирование на 2
					{
						//устанавливаем актуальный размер для мат. процессоров
						isInitialized[18]=(vkp.DataWord1.s.SizeX1/2)|((vkp.DataWord1.s.SizeY1/2)<<16);
						isInitialized[19]=(vkp.DataWord2.s.SizeX2/2)|((vkp.DataWord2.s.SizeY2/2)<<16);

						//////////////////////////////////////////////////////
						vsp.Data3.s.SizeX1/=2;		//размер кадра отправляеиого на верх в 2 раза меньше
						vsp.Data3.s.SizeY1/=2;
						vsp.Data4.s.SizeX2/=2;
						vsp.Data4.s.SizeY2/=2;

						//управление на XilinxVPU в два раза меньше
						vkp.DataWord3.s.CoordX1/=2;
						vkp.DataWord1.s.SizeX1/=2;
						vkp.DataWord4.s.CoordX2/=2;
						vkp.DataWord2.s.SizeX2/=2;
					}
					else
					{
						//устанавливаем актуальный размер для мат. процессоров
						isInitialized[18]=vkp.DataWord1.s.SizeX1|(vkp.DataWord1.s.SizeY1<<16);
						isInitialized[19]=vkp.DataWord2.s.SizeX2|(vkp.DataWord2.s.SizeY2<<16);
					}
					//для управления srio
					srioUpac.SrioDataKamera1.s.ShiftInRow=vkp.DataWord3.s.CoordX1/4;
					srioUpac.SrioDataKamera1.s.SizeRow=(vkp.DataWord1.s.SizeX1/4)-1;
					//srioUpac.SrioDataKamera1.s.SumX2=vkp.KommandWord.s.SM;
					srioUpac.SrioDataKamera1.s.SumX2=0;		//нет суммированию на XilinxVPU
					srioUpac.SrioDataKamera1.s.KamIm=1;

					srioUpac.SrioDataKamera2.s.ShiftInRow=vkp.DataWord4.s.CoordX2/4;
					srioUpac.SrioDataKamera2.s.SizeRow=(vkp.DataWord2.s.SizeX2/4)-1;
					//srioUpac.SrioDataKamera2.s.SumX2=vkp.KommandWord.s.SM;
					srioUpac.SrioDataKamera2.s.SumX2=0;		//нет суммированию на XilinxVPU
					srioUpac.SrioDataKamera2.s.KamIm=1;

					isInitialized[1]=srioUpac.VideoRamK1;
					isInitialized[2]=srioUpac.SrioDataKamera1.sw;
					isInitialized[3]=srioUpac.VideoRamK2;
					isInitialized[4]=srioUpac.SrioDataKamera2.sw;
					isInitialized[0] = 3;

					Cache_wb((void *) &isInitialized[0], 128, Cache_Type_L1D, TRUE);
					Task_sleep(4);		//2 мс
					Cache_inv((void *) &isInitialized[0], 128, Cache_Type_L1D, TRUE);

					if(vkp.CMOS1.s.ADC_GAIN==0)
						vkp.CMOS1.s.ADC_GAIN=GAIN;
					if(vkp.CMOS1.s.Reg73==0)
						vkp.CMOS1.s.Reg73=FOTReg73;
					if(vkp.CMOS1.s.Offset==0)
						vkp.CMOS1.s.Offset=OffsetChan1;
					if(vkp.DataWord3.s.Exp1==0)
						vkp.DataWord3.s.Exp1=1;

					if(vkp.CMOS2.s.ADC_GAIN==0)
						vkp.CMOS2.s.ADC_GAIN=GAIN;
					if(vkp.CMOS2.s.Reg73==0)
						vkp.CMOS2.s.Reg73=FOTReg73;
					if(vkp.CMOS2.s.Offset==0)
						vkp.CMOS2.s.Offset=OffsetChan2;
					if(vkp.DataWord4.s.Exp2==0)
						vkp.DataWord4.s.Exp2=1;

					//формирование параметров для настройки матрицы
					cmos1config.ADC_GAIN=vkp.CMOS1.s.ADC_GAIN;
					cmos1config.Reg_73=vkp.CMOS1.s.Reg73;
					cmos1config.OFFSET1=vkp.CMOS1.s.Offset&0xff;
					cmos1config.OFFSET2=(vkp.CMOS1.s.Offset>>8)&0xff;

					int32 Exp_time_Chan1=0;
					Exp_time_Chan1=(((vkp.DataWord3.s.Exp1*10)/2.6875)-(0.43*cmos1config.Reg_73));
					Exp_time_Chan1++;

					cmos1config.Exp_time1=Exp_time_Chan1&0xff;
					cmos1config.Exp_time2=(Exp_time_Chan1>>8)&0xff;
					cmos1config.Exp_time3=(Exp_time_Chan1>>16)&0xff;

					cmos1config.StartLinesLO = (Uint8)(vkp.DataWord3.s.CoordY1&0x00ff);		//номер строки с которой начинаем считывание
					cmos1config.StartLinesHI= (Uint8)((vkp.DataWord3.s.CoordY1>>8)&0xff);
					cmos1config.NumberLinesLO = (Uint8)(vkp.DataWord1.s.SizeY1&0x00ff);		//колличество считываемы строк
					cmos1config.NumberLinesHI = (Uint8)((vkp.DataWord1.s.SizeY1>>8)&0xff);

					cmos2config.StartLinesLO = (Uint8)(vkp.DataWord4.s.CoordY2&0x00ff);		//номер строки с которой начинаем считывание
					cmos2config.StartLinesHI= (Uint8)((vkp.DataWord4.s.CoordY2>>8)&0xff);
					cmos2config.NumberLinesLO = (Uint8)(vkp.DataWord2.s.SizeY2&0x00ff);		//колличество считываемы строк
					cmos2config.NumberLinesHI = (Uint8)((vkp.DataWord2.s.SizeY2>>8)&0xff);

					cmos2config.ADC_GAIN=vkp.CMOS2.s.ADC_GAIN;
					cmos2config.Reg_73=vkp.CMOS2.s.Reg73;
					cmos2config.OFFSET1=vkp.CMOS2.s.Offset&0xff;
					cmos2config.OFFSET2=(vkp.CMOS2.s.Offset>>8)&0xff;

					int32 Exp_time_Chan2=0;
					Exp_time_Chan2=(((vkp.DataWord4.s.Exp2*10)/2.6875)-(0.43*cmos2config.Reg_73));
					Exp_time_Chan2++;

					cmos2config.Exp_time1=Exp_time_Chan2&0xff;
					cmos2config.Exp_time2=(Exp_time_Chan2>>8)&0xff;
					cmos2config.Exp_time3=(Exp_time_Chan2>>16)&0xff;

					cmos_write_reg();			//настраиваем характеристики картинки
					cmos_write_sizekadr();		//настраиваем размер картинки

					finishk1count=0;
					finishk2count=0;

					gpioStrobBlink();	//команда на перерисову кадров с матриц
					if((vsp.ControlWord1.s.STROB1==1)&&(vsp.ControlWord1.s.STROB2==1)&&((vkp.ControlWord.s.S1+vkp.ControlWord.s.S2)>0))
					{
						Task_sleep((40+(vkp.DataWord3.s.Exp1)/100));
						if((finishk1count>0)&&(finishk2count>0))
						{
							isInitialized[24]=	0;
							isInitialized[25]=	0;
							isInitialized[26]=	0;	//блокируем работу по синхронизации
							isInitialized[0] =	3;
							Cache_wb((void *) &isInitialized[0], 128, Cache_Type_L1D, TRUE);
							Task_sleep(2);
						}
					}
					else
					{
						Task_sleep((6+(vkp.DataWord3.s.Exp1)/100));
					}
					StartStopEth(0);

					///////////////////////////////////////////////////////////////
					ZKadr.NumKadr=vsp.NumKadr;
					size=(int)(vsp.Data3.s.SizeX1*vsp.Data3.s.SizeY1+vsp.Data4.s.SizeX2*vsp.Data4.s.SizeY2);

					if(vkp.ControlWord.s.B) size*=2;

					if((size)>(PAKET_SIZE_VIDEO-12))
						ZKadr.Data.s.SizePacket=PAKET_SIZE_VIDEO;
					else
						ZKadr.Data.s.SizePacket=size+12;

					ZKadr.OffsetInPacket=0;
					ZKadr.Data.s.NumPacket=0;

					memcpy((void *)(&GlobalVideoBuff),(void*)&ZKadr,sizeof(VisualKadrPaket));
					sendsize=sendto(s, &vsp, sizeof(VisualStatusPaket), 0, (PSA) &sin1,sizeof(sin1));
					CycleDelay(10000*vkp.ControlWord.s.KO+20000);
					if(sendsize!=sizeof(VisualStatusPaket))
					{
						CycleDelay(10000*vkp.ControlWord.s.KO+40000);
					}
					sendsize=sendto(s, &GlobalVideoBuff, ZKadr.Data.s.SizePacket, 0, (PSA) &sin1,sizeof(sin1));
					CycleDelay(10000*vkp.ControlWord.s.KO+20000);
					if(sendsize!=ZKadr.Data.s.SizePacket)
					{
						CycleDelay(10000*vkp.ControlWord.s.KO+40000);
					}

					if(size >PAKET_SIZE_VIDEO)
					{

						tmp=size/(PAKET_SIZE_VIDEO-12);
						for(i=1;i<tmp;i++)
						{
							memcpy((void *)(&GlobalVideoBuff),(void*)&GlobalVideoBuff[(PAKET_SIZE_VIDEO-12)*i],sizeof(VisualKadrPaket));
							ZKadr.Data.s.NumPacket++;
							ZKadr.OffsetInPacket=(PAKET_SIZE_VIDEO-12)*i;
							memcpy((void *)&GlobalVideoBuff[(PAKET_SIZE_VIDEO-12)*i],(void*)&ZKadr,sizeof(VisualKadrPaket));
							sendsize=sendto(s, &GlobalVideoBuff[(PAKET_SIZE_VIDEO-12)*i], ZKadr.Data.s.SizePacket, 0, (PSA) &sin1,sizeof(sin1));

							CycleDelay(10000*vkp.ControlWord.s.KO+20000);
							if(sendsize!=ZKadr.Data.s.SizePacket)
							{
								CycleDelay(10000*vkp.ControlWord.s.KO+40000);
							}

							memcpy((void*)&GlobalVideoBuff[(PAKET_SIZE_VIDEO-12)*i],(void *)(&GlobalVideoBuff),sizeof(VisualKadrPaket));
						}
						if(tmp*(PAKET_SIZE_VIDEO-12)<size)
						{
							memcpy((void *)(&GlobalVideoBuff),(void*)&GlobalVideoBuff[(PAKET_SIZE_VIDEO-12)*i],sizeof(VisualKadrPaket));
							ZKadr.Data.s.NumPacket++;
							ZKadr.OffsetInPacket=(PAKET_SIZE_VIDEO-12)*i;
							ZKadr.Data.s.SizePacket=size+12-(tmp*(PAKET_SIZE_VIDEO-12));
							memcpy((void *)&GlobalVideoBuff[(PAKET_SIZE_VIDEO-12)*i],(void*)&ZKadr,sizeof(VisualKadrPaket));
							sendsize=sendto(s, &GlobalVideoBuff[(PAKET_SIZE_VIDEO-12)*i], ZKadr.Data.s.SizePacket, 0, (PSA) &sin1,sizeof(sin1));

							CycleDelay(10000*vkp.ControlWord.s.KO+20000);
							if(sendsize!=ZKadr.Data.s.SizePacket)
							{
								CycleDelay(10000*vkp.ControlWord.s.KO+40000);
							}

							memcpy((void*)&GlobalVideoBuff[(PAKET_SIZE_VIDEO-12)*i],(void *)(&GlobalVideoBuff),sizeof(VisualKadrPaket));
						}
					}
			}
			else
			{
				StartStopEth(0);
				ZKadr.Data.s.NumPacket=vkp.DataWord5.s.NumPacket;
				ZKadr.OffsetInPacket=(PAKET_SIZE_VIDEO-12)*ZKadr.Data.s.NumPacket;

				size=(int)(vsp.Data3.s.SizeX1*vsp.Data3.s.SizeY1+vsp.Data4.s.SizeX2*vsp.Data4.s.SizeY2);

				if(vkp.ControlWord.s.B) size*=2;

				tmp=size/(PAKET_SIZE_VIDEO-12);

				memcpy((void *)(&GlobalVideoBuff),(void*)&GlobalVideoBuff[(PAKET_SIZE_VIDEO-12)*ZKadr.Data.s.NumPacket],sizeof(VisualKadrPaket));
				if((size)>(PAKET_SIZE_VIDEO-12))
				{
					if(ZKadr.Data.s.NumPacket<=tmp)
						ZKadr.Data.s.SizePacket=PAKET_SIZE_VIDEO;
					else
					{
						ZKadr.Data.s.SizePacket=size+12-(tmp*(PAKET_SIZE_VIDEO-12));
					}
				}
				else
					ZKadr.Data.s.SizePacket=size+12;


				memcpy((void *)&GlobalVideoBuff[(PAKET_SIZE_VIDEO-12)*ZKadr.Data.s.NumPacket],(void*)&ZKadr,sizeof(VisualKadrPaket));
				sendsize=sendto(s, &GlobalVideoBuff[(PAKET_SIZE_VIDEO-12)*ZKadr.Data.s.NumPacket], ZKadr.Data.s.SizePacket, 0, (PSA) &sin1,sizeof(sin1));
				CycleDelay(10000*vkp.ControlWord.s.KO);
				if(sendsize!=ZKadr.Data.s.SizePacket)
				{
					CycleDelay(10000*vkp.ControlWord.s.KO);
				}
				memcpy((void*)&GlobalVideoBuff[(PAKET_SIZE_VIDEO-12)*ZKadr.Data.s.NumPacket],(void *)(&GlobalVideoBuff),sizeof(VisualKadrPaket));
			}

		}
		else
			break;
	}

	if(sendsize==-1)
	{
		for(;;)
		{
				//ловушка на слетание сервера
		}
	}
	return (1);
}

void spi_cmos_reset_enable()
{
	UInt8 BuffSPI[16];

	memset((void *) &BuffSPI, 0, sizeof(BuffSPI));

	gpio.GpioData.sw=gpioReadDataBus();

	gpio.GpioData.s.SPICS0=0x1;
	gpio.GpioData.s.SPICS1=0x1;
	gpioWriteDataBus(gpio.GpioData.sw);

	spi_claim_oed(0,SPI_MIN_FREQ,16);	//матрица 0

	int i=0;
	int mycount=0;


	for(mycount=0;mycount<7;mycount++)
	{
		spi_rwreg_oed(mycount,(uint16_t*)&BuffSPI[i],(uint16_t*)&BuffSPI[i],0);	//считываем все регистры
		i+=2;
	}

	spi_release_oed();

	vsp.StatPVPK1_1.s.Status0=BuffSPI[0];
	vsp.StatPVPK1_1.s.Status1=BuffSPI[2];
	vsp.StatPVPK1_1.s.Status2=BuffSPI[4];
	vsp.StatPVPK1_1.s.Status3=BuffSPI[6];

	vsp.StatPVPK1_2.s.Status4=BuffSPI[8];
	vsp.StatPVPK1_2.s.Status5=BuffSPI[10];
	vsp.StatPVPK1_2.s.Status6=BuffSPI[12];
	vsp.StatPVPK1_2.s.Status7=BuffSPI[14];

	spi_claim_oed(1,SPI_MIN_FREQ,16);	//матрица 1
	i=0;
	for(mycount=0;mycount<7;mycount++)
	{
		spi_rwreg_oed(mycount,(uint16_t*)&BuffSPI[i],(uint16_t*)&BuffSPI[i],0);
		i+=2;
	}

	spi_release_oed();

	vsp.StatPVPK2_1.s.Status0=BuffSPI[0];
	vsp.StatPVPK2_1.s.Status1=BuffSPI[2];
	vsp.StatPVPK2_1.s.Status2=BuffSPI[4];
	vsp.StatPVPK2_1.s.Status3=BuffSPI[6];

	vsp.StatPVPK2_2.s.Status4=BuffSPI[8];
	vsp.StatPVPK2_2.s.Status5=BuffSPI[10];
	vsp.StatPVPK2_2.s.Status6=BuffSPI[12];
	vsp.StatPVPK2_2.s.Status7=BuffSPI[14];

	if((vsp.StatPVPK1_1.s.Status0!=0xf)||(vsp.StatPVPK2_1.s.Status0!=0xf))	//ошибка работы матрицы производим ресет.
	{
			gpio.GpioData.s.SPICS0=0x1;
			gpio.GpioData.s.SPICS1=0x1;
			gpioWriteDataBus(gpio.GpioData.sw);

			spi_init_xilinx_cmos_oed(2,0);	//настройка Xilinx 0 матрицы
			spi_init_xilinx_cmos_oed(2,1);	//настройка Xilinx 1 матрицы

			for(i=0;i<500;i++)
			{
			}

			spi_init_xilinx_cmos_oed(0,0);	//настройка Xilinx 0 матрицы
			spi_init_xilinx_cmos_oed(0,1);	//настройка Xilinx 1 матрицы

			Task_sleep(2);

	}
}

void cmos_write_reg()
{
	int COL=7;
	uint16_t initbuff[7][2],i=0;

	gpio.GpioData.sw=gpioReadDataBus();
	gpio.GpioData.s.SPICS0=0x0;
	gpio.GpioData.s.SPICS1=0x0;
	gpioWriteDataBus(gpio.GpioData.sw);	//настраиваем матрицы

	initbuff[0][0]=ADCGAIN;	//цифровое усиление то же ставим равное 1
	initbuff[0][1]=cmos1config.ADC_GAIN;

	initbuff[1][0]=REG73;	//Значение 73 регистра
	initbuff[1][1]=cmos1config.Reg_73;

	initbuff[2][0]=OFFSETLO;	//Уровень черного
	initbuff[2][1]=cmos1config.OFFSET1;

	initbuff[3][0]=OFFSETHI;
	initbuff[3][1]=cmos1config.OFFSET2;

	initbuff[4][0]=EXPTIME1;	//Время накопления
	initbuff[4][1]=cmos1config.Exp_time1;

	initbuff[5][0]=EXPTIME2;
	initbuff[5][1]=cmos1config.Exp_time2;

	initbuff[6][0]=EXPTIME3;
	initbuff[6][1]=cmos1config.Exp_time3;

	spi_claim_oed(0,SPI_MIN_FREQ,16);

	for(i=0;i<COL;i++)
	{
		spi_rwreg_oed(initbuff[i][0],&initbuff[i][1],&initbuff[i][1],1);	//записываем все регистры
	}

	spi_release_oed();

	initbuff[0][0]=ADCGAIN;	//цифровое усиление то же ставим равное 1
	initbuff[0][1]=cmos2config.ADC_GAIN;

	initbuff[1][0]=REG73;	//Значение 73 регистра
	initbuff[1][1]=cmos2config.Reg_73;

	initbuff[2][0]=OFFSETLO;	//Уровень черного
	initbuff[2][1]=cmos2config.OFFSET1;

	initbuff[3][0]=OFFSETHI;
	initbuff[3][1]=cmos2config.OFFSET2;

	initbuff[4][0]=EXPTIME1;	//Время накопления
	initbuff[4][1]=cmos2config.Exp_time1;

	initbuff[5][0]=EXPTIME2;
	initbuff[5][1]=cmos2config.Exp_time2;

	initbuff[6][0]=EXPTIME3;
	initbuff[6][1]=cmos2config.Exp_time3;

	spi_claim_oed(1,SPI_MIN_FREQ,16);

	for(i=0;i<COL;i++)
	{
		spi_rwreg_oed(initbuff[i][0],&initbuff[i][1],&initbuff[i][1],1);	//записываем все регистры
	}

	spi_release_oed();
}

void cmos_write_sizekadr()
{
	int COL=4;
	uint16_t initbuff[4][2],i=0;

	gpio.GpioData.sw=gpioReadDataBus();
	gpio.GpioData.s.SPICS0=0x0;
	gpio.GpioData.s.SPICS1=0x0;
	gpioWriteDataBus(gpio.GpioData.sw);	//настраиваем матрицы

	//даем управление на 1 матрицу
	initbuff[0][0]=NUMBERLINESLO;	//количество строк
	initbuff[0][1]=cmos1config.NumberLinesLO;

	initbuff[1][0]=NUMBERLINESHI;	//количество строк
	initbuff[1][1]=cmos1config.NumberLinesHI;

	initbuff[2][0]=STARTLINELO;	//номер первой строки
	initbuff[2][1]=cmos1config.StartLinesLO;

	initbuff[3][0]=STARTLINEHI;	//номер первой строки
	initbuff[3][1]=cmos1config.StartLinesHI;

	spi_claim_oed(0,SPI_MIN_FREQ,16);

	for(i=0;i<COL;i++)
	{
		spi_rwreg_oed(initbuff[i][0],&initbuff[i][1],&initbuff[i][1],1);	//записываем все регистры
	}

   spi_release_oed();

   //даем управление на 2 матрицу
   	initbuff[0][0]=NUMBERLINESLO;	//количество строк
   	initbuff[0][1]=cmos2config.NumberLinesLO;

   	initbuff[1][0]=NUMBERLINESHI;	//количество строк
   	initbuff[1][1]=cmos2config.NumberLinesHI;

   	initbuff[2][0]=STARTLINELO;	//номер первой строки
   	initbuff[2][1]=cmos2config.StartLinesLO;

   	initbuff[3][0]=STARTLINEHI;	//номер первой строки
   	initbuff[3][1]=cmos2config.StartLinesHI;

   	spi_claim_oed(1,SPI_MIN_FREQ,16);

   	for(i=0;i<COL;i++)
   	{
   		spi_rwreg_oed(initbuff[i][0],&initbuff[i][1],&initbuff[i][1],1);	//записываем все регистры
   	}

    spi_release_oed();

}

void cmosxilinx_write_kadrpos()
{
	int COL=1;
	uint16_t initbuff[4][2],i=0;

	gpio.GpioData.sw=gpioReadDataBus();
	gpio.GpioData.s.SPICS0=0x1;
	gpio.GpioData.s.SPICS1=0x1;
	gpioWriteDataBus(gpio.GpioData.sw);	//настраиваем xilinx матриц

	uint16 PodStringCH1=0;	//подстрока для канала 1
	uint16 PodStringCH2=0;	//подстрока для канала 2
	uint16 mathtemp=0;

//	summ=vkp.KommandWord.s.SM;
	if(!vkp.KommandWord.s.SM)
	{
		mathtemp=vkp.DataWord3.s.CoordX1/128;	//определяем сколько подстрок необходимо сдвинуть
		if(mathtemp>8)
			mathtemp=8;

		PodStringCH1=mathtemp<<4;
		 vkp.DataWord3.s.CoordX1-=mathtemp*128;	// оставляем сдиг без отсчитанных подстрок

		 mathtemp=vkp.DataWord4.s.CoordX2/128;	//определяем сколько подстрок необходимо сдвинуть
		 if(mathtemp>8)
			 mathtemp=8;

		PodStringCH2=mathtemp<<4;
		vkp.DataWord4.s.CoordX2-=mathtemp*128;	// оставляем сдиг без отсчитанных подстрок
	}
	else
	{
		PodStringCH1=PodStringCH2=vkp.KommandWord.s.SM<<2;
	}
	//даем управление на 1 матрицу
	initbuff[0][0]=0;
	initbuff[0][1]=PodStringCH1;

	spi_claim_oed(0,SPI_MIN_FREQ,16);

	for(i=0;i<COL;i++)
	{
		spi_rwreg_oed(initbuff[i][0],&initbuff[i][1],&initbuff[i][1],1);	//записываем все регистры
	}

   spi_release_oed();

   //даем управление на 2 матрицу
   	initbuff[0][0]=0;
   	initbuff[0][1]=PodStringCH2;

   	spi_claim_oed(1,SPI_MIN_FREQ,16);

   	for(i=0;i<COL;i++)
   	{
   		spi_rwreg_oed(initbuff[i][0],&initbuff[i][1],&initbuff[i][1],1);	//записываем все регистры
   	}

    spi_release_oed();

}
void CallbackTimer() {

		if(CSL_chipReadReg (CSL_CHIP_DNUM)==0)
		{
			timercount++;
			GlobalTimeTick+=21;
			if(GlobalTimeTick>=10000)				//запись осуществляем раз в десять секунд
			{
				uint32* base=(uint32*)FRAM_PARAM_OED;
				UInt16 test;

				test=gpioReadDataBus();

				GlobalTimeTick-=10000;
				GlobalTimeJob+=10;
				gpioWriteDataBus(test|0x01);		//переключаемся для работы с FRAM

				*(base+1)=GlobalTimeJob;

				CycleDelay(400);

				gpioWriteDataBus(test);
			}
			if(battle.STAGE<=sSTANDBY)
				Cache_inv((void *) &DStruct, sizeof(DiagStruct), Cache_Type_L1D, TRUE);
			//Если к нам на один из боевых портов обратились с запросом начинаем отвечать
			//(при отсутствии внешней синхронизации) с периодом в 21ms
			if((timercount>50)&&(battle.STAGE==sSTOP))
			{
				battle.STAGE=sDIAG1;
				Cache_wb((void *) &battle, sizeof(BattleParam), Cache_Type_L1D, TRUE);
				//Cache_inv((void *) &battle, sizeof(BattleParam), Cache_Type_L1D, TRUE);
			}
			if(sinx1count==0)
			{
				if((ta1.CommandTA2.s.TechMode<=14))
				   {
						if(eventclkhtarget1!=NULL)
						{
							Event_post(eventclkhtarget1, Event_Id_21);
						//	Event_post(eventclkhtarget1, Event_Id_22);
						}
				   }

			}
			if(sinx2count==0)
			{
				if((ta1.CommandTA2.s.TechMode<=14))
					{
						if(eventclkhtarget2!=NULL)
						{
							Event_post(eventclkhtarget2, Event_Id_23);
						//	Event_post(eventclkhtarget2, Event_Id_24);
						}
					}
			}

			if(battle.STAGE<sSTANDBY)
			{
				Cache_inv((void *) &battle, sizeof(BattleParam), Cache_Type_L1D, TRUE);
				if(battle.STAGE==sSTANDBY)
				{
					Cache_inv((void *) &oed1, sizeof(OEDTOTA), Cache_Type_L1D, TRUE);
					Cache_inv((void *) &oed2, sizeof(OEDTOTA), Cache_Type_L1D, TRUE);
				}
			}
		}
		else
			{
				Cache_inv((void *) &battle, sizeof(BattleParam), Cache_Type_L1D, TRUE);
				if((battle.STAGE==sDIAG1)||(battle.STAGE==sDIAG2)||(battle.STAGE==sDIAG3))	//начинаем диагностику
					{
						//производим диагностику
						BattleFunc(1,1);
					}
			}
}
