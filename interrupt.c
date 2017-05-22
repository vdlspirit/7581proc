#include "interrupt.h"
#include "BattleJob.h"

#include "resource_mgr.h"
#include <ti/sysbios/family/c64p/Hwi.h>
#include <ti/sysbios/knl/Clock.h>
#include <ti/sysbios/knl/Event.h>
#include <ti/sysbios/family/c66/Cache.h>

unsigned short	 blinksinx1=0;
unsigned short	 blinksinx2=0;

unsigned int TimeTarget1Old=0;
unsigned int TimeTarget1New=0;
unsigned int TimeTarget2Old=0;
unsigned int TimeTarget2New=0;

unsigned int PeriodTarget1=0;
unsigned int PeriodTarget2=0;
unsigned int DeltaPeriodTarget12=0;

unsigned int	deltasinx=0;

unsigned int sinx1count=0;
unsigned int sinx2count=0;
unsigned int finishk1count=0;
unsigned int finishk2count=0;

unsigned int BattleTimerSinx1=0;
unsigned int BattleTimerSinx2=0;

unsigned short sinx1=0;
unsigned short sinx2=0;

extern volatile unsigned int cregister TSCL;
//extern volatile unsigned int cregister TSCH;

extern BattleMathT1K1 battlemathT1k1;
extern BattleMathT1K2 battlemathT1k2;
extern BattleMathT2K1 battlemathT2k1;
extern BattleMathT2K2 battlemathT2k2;
extern BattleParam battle;					//боевая структура управления программой
extern BattleParam2 battle2;				//боевая структура управления программой цель 2
extern volatile UInt32*     isInitialized;

//настройки таймеров для выхода на обмен для цели 1 и 2
Clock_Handle clkhtarget1;
Clock_Params clkParamstarget1;

Clock_Handle clkhtarget2;
Clock_Params clkParamstarget2;

Clock_Handle clkh17target1;
Clock_Params clk17Paramstarget1;

Clock_Handle clkh17target2;
Clock_Params clk17Paramstarget2;

//настройка событий прихода таймеров
Event_Handle   eventclkhtarget1;
Event_Handle   eventclkhtarget2;

//обработчик прерывания от линии GPIO15 СИНХ2
void GPIO15interrupt()
{
	if (CSL_chipReadReg (CSL_CHIP_DNUM) == 0)
	{
		Clock_start(clkhtarget2);		//стартуем таймер перед отправкой пакета на верх
		Clock_start(clkh17target2);		//стартуем таймер все пакеты должны быть получены

			TimeTarget2Old=TimeTarget2New;
			TimeTarget2New=TSCL;

			if(TimeTarget2New>TimeTarget2Old)
			PeriodTarget2=(TimeTarget2New-TimeTarget2Old)/1000;


		sinx2count++;
		BattleTimerSinx2++;
		sinx2++;
		blinksinx2=blinksinx2^1;
		battle2.PropertyKadr2Ch1=Target2LightCh1;	//ожидаем получение кадра с целью 2
		battle2.PropertyKadr2Ch2=Target2LightCh2;	//ожидаем получение кадра с целью 2
		Cache_wb((void *) &battle2.PropertyKadr2Ch1, sizeof(battle2.PropertyKadr2Ch1), Cache_Type_L1D, TRUE);
		Cache_wb((void *) &battle2.PropertyKadr2Ch2, sizeof(battle2.PropertyKadr2Ch2), Cache_Type_L1D, TRUE);
	}
}

//обработчик прерывания от линии GPIO14 СИНХ1
void GPIO14interrupt()
{
	if (CSL_chipReadReg (CSL_CHIP_DNUM) == 0)
	{
		Clock_start(clkhtarget1);		//стартуем таймер перед отправкой пакета на верх
		Clock_start(clkh17target1);		//стартуем таймер все пакеты должны быть получены


			TimeTarget1Old=TimeTarget1New;
			TimeTarget1New=TSCL;
		if(TimeTarget1New>TimeTarget1Old)
			PeriodTarget1=(TimeTarget1New-TimeTarget1Old)/1000;

		sinx1count++;
		sinx1++;
		BattleTimerSinx1++;
		blinksinx1=blinksinx1^1;
		battle.PropertyKadrCh1=Target1LightCh1;	//ожидаем получение кадра с целью 1
		battle.PropertyKadrCh2=Target1LightCh2;	//ожидаем получение кадра с целью 1
		Cache_wb((void *) &battle.PropertyKadrCh1, sizeof(battle.PropertyKadrCh1), Cache_Type_L1D, TRUE);
		Cache_wb((void *) &battle.PropertyKadrCh2, sizeof(battle.PropertyKadrCh2), Cache_Type_L1D, TRUE);
	}
}

//обработчик прерывания от линии GPIO13 канал 2
void GPIO13interrupt()
{
	if (CSL_chipReadReg (CSL_CHIP_DNUM) == 0)
	{
		finishk2count++;
	}
	else 	//копирование памяти производим из первого ядра
	{
		Cache_inv((void *) &battle, sizeof(BattleParam), Cache_Type_L1D, TRUE);
		Cache_inv((void *) &battle2, sizeof(BattleParam2), Cache_Type_L1D, TRUE);
		//копирование кадров по памяти для дальнейшей обработки
		if((battle.STAGE==sBATTLE)||(battle2.STAGE2==sBATTLE2))
		{
			Cache_inv((void *) &isInitialized[64], 16, Cache_Type_L1D, TRUE);
			Cache_inv((void *) &isInitialized[16], 16, Cache_Type_L1D, TRUE);

			int size2chT1,size2chT2;
			size2chT1=((isInitialized[19]&0x0000ffff)*(isInitialized[19]>>16))*2;
			size2chT2=((isInitialized[17]&0x0000ffff)*(isInitialized[17]>>16))*2;

			switch(battle.PropertyKadrCh2)
			{
				case Target1DarkCh2:	//темновой кадр канала 2 цель 1
					if(isInitialized[66]==0x02aa5501) //запись по цели 1
					{
						memcpy((void *)(isInitialized[72]),(void*)(isInitialized[3]),size2chT1);
						isInitialized[66]=0x02005501;
					}
					break;
				case Target1LightCh2:	//кадр канала 2 с импульсом
					if((isInitialized[66]==0x02aa5501)||(isInitialized[66]==0x02005501)) //запись по цели 1
					{
						memcpy((void *)(isInitialized[74]),(void*)(isInitialized[3]),size2chT1);
						isInitialized[66]=0x02000001;
					}
					battle.PropertyKadrCh2=NotKadrCh2;	//следующие кадры не ожидаем
					Cache_wb((void *) &battle.PropertyKadrCh2, sizeof(battle.PropertyKadrCh2), Cache_Type_L1D, TRUE);
					break;

				default:			break;
			}
			switch(battle2.PropertyKadr2Ch2)
			{
				case Target2DarkCh2:	//темновой кадр канала 2 цель 2
					if(isInitialized[67]==0x02aa5502) //запись по цели 2
					{
						memcpy((void *)(isInitialized[73]),(void*)(isInitialized[3]),size2chT2);
						isInitialized[67]=0x02005502;
					}
					break;
				case Target2LightCh2:	//кадр канала 2 с импульсом цель 2
					if((isInitialized[67]==0x02aa5502)||(isInitialized[67]==0x02005502))
					{
						memcpy((void *)(isInitialized[75]),(void*)(isInitialized[3]),size2chT2);
						isInitialized[67]=0x02000002;
					}
					battle2.PropertyKadr2Ch2=NotKadrT2Ch2;	//следующие кадры не ожидаем
					Cache_wb((void *) &battle2.PropertyKadr2Ch2, sizeof(battle2.PropertyKadr2Ch2), Cache_Type_L1D, TRUE);
					break;
				default:			break;
				}

			Cache_wb((void *) &isInitialized[64], 16, Cache_Type_L1D, TRUE);
		}
	}
}

//обработчик прерывания от линии GPIO12 канал 1
void GPIO12interrupt()
{
	if (CSL_chipReadReg (CSL_CHIP_DNUM) == 0)
	{
		finishk1count++;
	}
	else 	//копирование памяти производим из первого ядра
	{
		Cache_inv((void *) &battle, sizeof(BattleParam), Cache_Type_L1D, TRUE);
		Cache_inv((void *) &battle2, sizeof(BattleParam2), Cache_Type_L1D, TRUE);
		if((battle.STAGE==sBATTLE)||(battle2.STAGE2==sBATTLE2))
		{
			Cache_inv((void *) &isInitialized[64], 16, Cache_Type_L1D, TRUE);
			Cache_inv((void *) &isInitialized[16], 16, Cache_Type_L1D, TRUE);

			int size1chT1,size1chT2;
			size1chT1=((isInitialized[18]&0x0000ffff)*(isInitialized[18]>>16))*2;
			size1chT2=((isInitialized[16]&0x0000ffff)*(isInitialized[16]>>16))*2;


			switch(battle.PropertyKadrCh1)
			{
				case Target1DarkCh1:	//темновой кадр канала 1 цель 1
					if(isInitialized[64]==0x01aa5501)
					{
						memcpy((void *)(isInitialized[68]),(void*)(isInitialized[1]),size1chT1);
						isInitialized[64]=0x01005501;	//сбрасываем по кадру темновому
					}
					break;
				case Target1LightCh1:	//кадр канала 1 с импульсом
					if((isInitialized[64]==0x01aa5501)||(isInitialized[64]==0x01005501))
					{
						memcpy((void *)(isInitialized[70]),(void*)(isInitialized[1]),size1chT1);
						isInitialized[64]=0x01000001;	//сбрасываем по кадру с импульсом
					}
					battle.PropertyKadrCh1=NotKadrCh1;	//следующие кадры не ожидаем
					Cache_wb((void *) &battle.PropertyKadrCh1, sizeof(battle.PropertyKadrCh1), Cache_Type_L1D, TRUE);
					break;

				default:			break;
			}

			switch(battle2.PropertyKadr2Ch1)
			{
				case Target2DarkCh1:	//темновой кадр канала 1 цель 2
					if(isInitialized[65]==0x01aa5502)	//запись по цели 2
					{
						memcpy((void *)(isInitialized[69]),(void*)(isInitialized[1]),size1chT2);
						isInitialized[65]=0x01005502;	//сбрасываем по кадру с импульсом
					}
					break;
				case Target2LightCh1:	//кадр канала 1 с импульсом цель 2
					if((isInitialized[65]==0x01aa5502)||(isInitialized[65]==0x01005502))	//запись по цели 2
					{
						memcpy((void *)(isInitialized[71]),(void*)(isInitialized[1]),size1chT2);
						isInitialized[65]=0x01000002;	//сбрасываем по кадру с импульсом
					}
					battle2.PropertyKadr2Ch1=NotKadrT2Ch1;	//следующие кадры не ожидаем
					Cache_wb((void *) &battle2.PropertyKadr2Ch1, sizeof(battle2.PropertyKadr2Ch1), Cache_Type_L1D, TRUE);
					break;
				default:			break;
			}

			Cache_wb((void *) &isInitialized[64], 16, Cache_Type_L1D, TRUE);
		}
	}
}

unsigned int GiveMePeriod(unsigned short chennal)
{
	if(chennal==1)
	{
		return (unsigned int)PeriodTarget1;
	}
	else
	{
		return (unsigned int)PeriodTarget2;
	}

//	return Period;
}

unsigned int Delta=0;
unsigned int GiveMeDeltaSinx()
{
	unsigned int tmp=0;
	if(TimeTarget2New>TimeTarget1New)
		tmp=(unsigned int)(TimeTarget2New-TimeTarget1New)/1000;
	else
		tmp=(unsigned int)(TimeTarget1New-TimeTarget2New)/1000;

	if((tmp<=12000)&&(tmp>=8000))
		Delta=tmp;

	return Delta;
}

//обработчики таймеров
Void funclktarget1 (UArg a0)
{
		Event_post(eventclkhtarget1, Event_Id_21);
}

Void funclktarget2 (UArg a0)
{
		Event_post(eventclkhtarget2, Event_Id_23);
}

Void funclk17target1 (UArg a0)
{
//		Event_post(eventclkhtarget1, Event_Id_22);
		battle.PropertyKadrCh1=Target1DarkCh1;	//ожидаем получение кадра с целью 1
		battle.PropertyKadrCh2=Target1DarkCh2;	//ожидаем получение кадра с целью 1
		Cache_wb((void *) &battle.PropertyKadrCh1, sizeof(battle.PropertyKadrCh1), Cache_Type_L1D, TRUE);
		Cache_wb((void *) &battle.PropertyKadrCh2, sizeof(battle.PropertyKadrCh2), Cache_Type_L1D, TRUE);
}

Void funclk17target2 (UArg a0)
{
//		Event_post(eventclkhtarget2, Event_Id_24);
		battle2.PropertyKadr2Ch1=Target2DarkCh1;	//ожидаем получение кадра с целью 2
		battle2.PropertyKadr2Ch2=Target2DarkCh2;	//ожидаем получение кадра с целью 2
		Cache_wb((void *) &battle2.PropertyKadr2Ch1, sizeof(battle2.PropertyKadr2Ch1), Cache_Type_L1D, TRUE);
		Cache_wb((void *) &battle2.PropertyKadr2Ch2, sizeof(battle2.PropertyKadr2Ch2), Cache_Type_L1D, TRUE);
}
