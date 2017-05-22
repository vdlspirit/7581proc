/*
 * flasshdata.c
 *
 *  Created on: 22.10.2015
 *      Author: Admin
 */
#include <ti/sysbios/BIOS.h>
#include <ti/ndk/inc/netmain.h>
#include <ti/ndk/inc/_stack.h>
#include <ti/ndk/inc/tools/servers.h>
#include "ti/platform/platform.h"
#include "ti/platform/resource_mgr.h"
#include "flash_programm.h"
#include "PaketOEDTAEth.h"
#include "gpio.h"
#include "spi.h"
#include "flashdata.h"
#include "BattleJob.h"

extern volatile UInt32*     isInitialized;

extern uint GlobalNumDev;
extern uint32 GlobalBattleCount;

extern TATOOED ta1;						//структура пакета ТА цель 1
extern OEDTOTA oed1;					//структура пакета ОЭД цель 1
extern TATOOED ta2;							//структура пакета ТА цель 2
extern OEDTOTA oed2;							//структура пакета ОЭД цель 2
extern BattleParam battle;
extern BattleParam2 battle2;
extern uint32 Data1StopSave;
extern uint32 Data2StopSave;

#define MaxSizePuskInByte 251608240

uint16 MyMemCPY(void * output, const void * input, uint32 size)
{
	uint16 XorBuff=0, i=0;
	uint16 *in=(uint16*)input;
	uint16 *out=(uint16*)output;

	for(i=0;i<size;i++)
	{
		XorBuff^=(uint32)in[i];
		out[i]=in[i];
	}

	return XorBuff;
}

uint32 XorChekSUMCalc32(uint32 *Buff, uint32 Size)
{
	int i;
	uint32 Summ=0;

	for(i=0;i<Size;i++)
	{
		Summ^=Buff[i];
	}
	return Summ;
}

uint16 XorChekSUMCalc16(uint16 *Buff, uint32 Size)
{
	int i;
	uint16 Summ1=0;

	for(i=0;i<Size;i+=2)
	{
		Summ1^=Buff[i];
		Summ1^=Buff[i+1];
	}
	return Summ1;
}

void FillDataTitle1(DataFileTitle *datatitle,uint16 Paket,uint32 SizeOldFrame,uint16 Target)
{
	uint32 MAXCOLPAKET=16000;
	uint32 *XorChekSumm;
	uint32 *StartTitle=(uint32*)ADDRRAMJOBFLASHPROGRAMM;

	if(Target==1)
		StartTitle=(uint32*)ADDRRAMJOBFLASHPROGRAMMT2;

	datatitle->NumCycle=GlobalBattleCount;
	datatitle->NumPaket=Paket;
	datatitle->MarkerStart=MarkerStartTitle;
	datatitle->NumDev=GlobalNumDev;
	datatitle->SizeStructTitle=MAXCOLPAKET+sizeof(DataFileTitle)-4;		//вся структура для максимальных 4000 пакетов
	datatitle->Res1=0;

	if(Target==0)
	{
		if(Paket==1)	//создаем структуру заголовка
		{
			datatitle->Adrr=(uint32*)(ADDRRAMJOBFLASHPROGRAMM+16);
			memset((void *) ADDRRAMJOBFLASHPROGRAMM, 0, datatitle->SizeStructTitle);
			memcpy((void*)ADDRRAMJOBFLASHPROGRAMM,datatitle,(sizeof(DataFileTitle)-4));
			*(datatitle->Adrr)=datatitle->SizeStructTitle;
			XorChekSumm=(uint32*)(ADDRRAMJOBFLASHPROGRAMM+datatitle->SizeStructTitle-4);
			*XorChekSumm=XorChekSUMCalc32((uint32*)ADDRRAMJOBFLASHPROGRAMM,((datatitle->SizeStructTitle-4)/4));
		}
		else
		{
			if(*StartTitle==MarkerStartTitle)
			if((Paket<=4000)&&(Data1StopSave==0))
			{
				memcpy((void*)ADDRRAMJOBFLASHPROGRAMM,datatitle,(sizeof(DataFileTitle)-4));
				*((datatitle->Adrr)+Paket-1)=SizeOldFrame;
				XorChekSumm=(uint32*)(ADDRRAMJOBFLASHPROGRAMM+datatitle->SizeStructTitle-4);
				*XorChekSumm=XorChekSUMCalc32((uint32*)ADDRRAMJOBFLASHPROGRAMM,((datatitle->SizeStructTitle-4)/4));
			}
		}
	}
	else
	{
		if(Paket==1)	//создаем структуру заголовка
		{
			datatitle->Adrr=(uint32*)(ADDRRAMJOBFLASHPROGRAMMT2+16);
			memset((void *) ADDRRAMJOBFLASHPROGRAMMT2, 0, datatitle->SizeStructTitle);
			memcpy((void*)ADDRRAMJOBFLASHPROGRAMMT2,datatitle,(sizeof(DataFileTitle)-4));
			*(datatitle->Adrr)=datatitle->SizeStructTitle;
			XorChekSumm=(uint32*)(ADDRRAMJOBFLASHPROGRAMMT2+datatitle->SizeStructTitle-4);
			*XorChekSumm=XorChekSUMCalc32((uint32*)ADDRRAMJOBFLASHPROGRAMMT2,((datatitle->SizeStructTitle-4)/4));
		}
		else
		{
			if(*StartTitle==MarkerStartTitle)
			if((Paket<=4000)&&(Data2StopSave==0))
			{
				memcpy((void*)ADDRRAMJOBFLASHPROGRAMMT2,datatitle,(sizeof(DataFileTitle)-4));
				*((datatitle->Adrr)+Paket-1)=SizeOldFrame;
				XorChekSumm=(uint32*)(ADDRRAMJOBFLASHPROGRAMMT2+datatitle->SizeStructTitle-4);
				*XorChekSumm=XorChekSUMCalc32((uint32*)ADDRRAMJOBFLASHPROGRAMMT2,((datatitle->SizeStructTitle-4)/4));
			}
		}
	}

}
uint32 FillDataFrame1(DataFileFrame *dataframe,uint16 Frame,uint16 Param,uint16 Target)
{
	if(Target==0)
	{
		volatile uint32 *StartTitle=(uint32*)ADDRRAMJOBFLASHPROGRAMM;
		volatile uint32* addr=(uint32*)(ADDRRAMJOBFLASHPROGRAMM+16+(Frame-1)*4);
		volatile uint32 SizeByteCh1=0;
		volatile uint32 SizeByteCh2=0;
		volatile uint32 offset=0;
		//uint16 XorFrame=0;

		if((*StartTitle==MarkerStartTitle)&&(Frame<=4000)&&(Data1StopSave==0))
		{
				if(*addr==0xffffffff)
					return 0xffffffff;

				//добавляем патч на постоянное симмирование для уменьшения размеров
			//	if(battle.ControlState.s.SumChan1T1)
					SizeByteCh1=battle.Target1SizeX1*battle.Target1SizeY1;
			//	else
			//		SizeByteCh1=battle.Target1SizeX1*battle.Target1SizeY1*4;

				if(ta1.CommandTA1.s.ResetK1==1)
					SizeByteCh1=0;

			//	if(battle.ControlState.s.SumChan2T1)
					SizeByteCh2=battle.Target1SizeX2*battle.Target1SizeY2;
			//	else
			//		SizeByteCh2=battle.Target1SizeX2*battle.Target1SizeY2*4;

				dataframe->MarkerStart=MarkerStartFrame;

				dataframe->OffsetNextFrame=*addr+sizeof(DataFileFrame)+512+sizeof(OEDTOTA)
						+sizeof(TATOOED)+SizeByteCh1+SizeByteCh2+6;

				if(dataframe->OffsetNextFrame>=MaxSizePuskInByte)	//чуть меньше 240 мегабайт
					return 0xffffffff;

				dataframe->NumPaketOEDTAT1=oed1.NumPaket;
				dataframe->NumPaketTAOEDT1=ta1.NumPaket;
				dataframe->NumPaketOEDTAT2=oed2.NumPaket;
				dataframe->NumPaketTAOEDT2=ta2.NumPaket;
				dataframe->NumCycle=GlobalBattleCount;
				dataframe->SizePaketOEDTA=sizeof(OEDTOTA);
				dataframe->SizePaketTAOED=sizeof(TATOOED);
				dataframe->CoordX1=battle.Target1CoordX1;
				dataframe->CoordY1=battle.Target1CoordY1;
				dataframe->CoordX2=battle.Target1CoordX2;
				dataframe->CoordY2=battle.Target1CoordY2;
				dataframe->Reserv1=0;

				if(ta1.CommandTA1.s.ResetK1!=1)
				{
					//dataframe->KadrCh1.s.SUMM=battle.ControlState.s.SumChan1T1;
					dataframe->KadrCh1.s.SUMM=1;
					dataframe->KadrCh1.s.SizeX1=battle.Target1SizeX1;
					dataframe->KadrCh1.s.SizeY1=battle.Target1SizeY1;
				}
				else
					dataframe->KadrCh1.sw=0;

				//dataframe->KadrCh2.s.SUMM=battle.ControlState.s.SumChan2T1;
				dataframe->KadrCh2.s.SUMM=1;
				dataframe->KadrCh2.s.SizeX2=battle.Target1SizeX2;
				dataframe->KadrCh2.s.SizeY2=battle.Target1SizeY2;

				offset=sizeof(DataFileFrame);
				uint32 addrframe=(uint32)(*addr+ADDRRAMJOBFLASHPROGRAMM);
				memcpy((void*)addrframe,dataframe,offset);
				//XorFrame^=MyMemCPY((void*)addrframe,dataframe,(offset/2));
				memset((void*)(addrframe+offset), 0, 512);	//забиваем структуры info 0
				offset+=512;

				memcpy((void*)(addrframe+offset),&oed1,dataframe->SizePaketOEDTA);
				//XorFrame^=MyMemCPY((void*)(addrframe+offset),&oed1,(dataframe->SizePaketOEDTA/2));
				offset+=dataframe->SizePaketOEDTA;
				memcpy((void*)(addrframe+offset),&ta1,dataframe->SizePaketTAOED);
				//XorFrame^=MyMemCPY((void*)(addrframe+offset),&ta1,(dataframe->SizePaketTAOED/2));
				offset+=dataframe->SizePaketTAOED;

				//копируем изображения
				SizeByteCh1/=2;
				SizeByteCh2/=2;

				//канал 1
				if(battle.ControlState.s.SumChan1T1==1)
				{
					memcpy((void*)(addrframe+offset),(void *)(isInitialized[68]),SizeByteCh1);
					offset+=SizeByteCh1;
					memcpy((void*)(addrframe+offset),(void *)(isInitialized[70]),SizeByteCh1);
					offset+=SizeByteCh1;
				}
				else
				{
					memcpy((void*)(addrframe+offset),(void *)(isInitialized[78]),SizeByteCh1);
					offset+=SizeByteCh1;
					memcpy((void*)(addrframe+offset),(void *)(isInitialized[80]),SizeByteCh1);
					offset+=SizeByteCh1;
				}

				//канал 2

				if(battle.ControlState.s.SumChan2T1==1)
				{
					memcpy((void*)(addrframe+offset),(void *)(isInitialized[72]),SizeByteCh2);
					offset+=SizeByteCh2;
					memcpy((void*)(addrframe+offset),(void *)(isInitialized[74]),SizeByteCh2);
					offset+=SizeByteCh2;
				}
				else
				{
					memcpy((void*)(addrframe+offset),(void *)(isInitialized[82]),SizeByteCh2);
					offset+=SizeByteCh2;
					memcpy((void*)(addrframe+offset),(void *)(isInitialized[84]),SizeByteCh2);
					offset+=SizeByteCh2;
				}

				uint32 marker=(uint32)MarkerStopFrame;
				memcpy((void *)(addrframe+offset),&marker,4);
				//XorFrame^=MyMemCPY((void *)(addrframe+offset),&marker,2);
				offset+=4;
				uint16 Xor=0;
				uint8 * AdrXorChkSum = (uint8*)(addrframe+offset);
				memcpy((void*)AdrXorChkSum,&Xor,2);

				Xor=XorChekSUMCalc16((uint16*)addrframe,(offset/2));
				memcpy((void*)AdrXorChkSum,&Xor,2);

			//	memcpy((void*)AdrXorChkSum,&XorFrame,2);
				return dataframe->OffsetNextFrame;
		}
		else
			return 0;
	}
	else
	{
		volatile uint32 *StartTitle=(uint32*)ADDRRAMJOBFLASHPROGRAMMT2;
		volatile uint32* addr=(uint32*)(ADDRRAMJOBFLASHPROGRAMMT2+16+(Frame-1)*4);
		volatile uint32 SizeByteCh1=0;
		volatile uint32 SizeByteCh2=0;
		volatile uint32 offset=0;
		//uint16 XorFrame=0;

		if((*StartTitle==MarkerStartTitle)&&(Frame<=4000)&&(Data2StopSave==0))
		{
				if(*addr==0xffffffff)
					return 0xffffffff;

				//добавляем патч на постоянное симмирование для уменьшения размеров
			//	if(battle.ControlState.s.SumChan1T1)
					SizeByteCh1=battle2.Target2SizeX1*battle2.Target2SizeY1;
			//	else
			//		SizeByteCh1=battle.Target1SizeX1*battle.Target1SizeY1*4;

				if(ta2.CommandTA1.s.ResetK1==1)
					SizeByteCh1=0;

			//	if(battle.ControlState.s.SumChan2T1)
					SizeByteCh2=battle2.Target2SizeX2*battle2.Target2SizeY2;
			//	else
			//		SizeByteCh2=battle.Target1SizeX2*battle.Target1SizeY2*4;

				dataframe->MarkerStart=MarkerStartFrame;

				dataframe->OffsetNextFrame=*addr+sizeof(DataFileFrame)+512+sizeof(OEDTOTA)
						+sizeof(TATOOED)+SizeByteCh1+SizeByteCh2+6;

				if(dataframe->OffsetNextFrame>=MaxSizePuskInByte)	//чуть меньше 240 мегабайт
					return 0xffffffff;

				dataframe->NumPaketOEDTAT1=oed1.NumPaket;
				dataframe->NumPaketTAOEDT1=ta1.NumPaket;
				dataframe->NumPaketOEDTAT2=oed2.NumPaket;
				dataframe->NumPaketTAOEDT2=ta2.NumPaket;
				dataframe->NumCycle=GlobalBattleCount;
				dataframe->SizePaketOEDTA=sizeof(OEDTOTA);
				dataframe->SizePaketTAOED=sizeof(TATOOED);
				dataframe->CoordX1=battle2.Target2CoordX1;
				dataframe->CoordY1=battle2.Target2CoordY1;
				dataframe->CoordX2=battle2.Target2CoordX2;
				dataframe->CoordY2=battle2.Target2CoordY2;
				dataframe->Reserv1=0;

				if(ta2.CommandTA1.s.ResetK1!=1)
				{
					//dataframe->KadrCh1.s.SUMM=battle.ControlState.s.SumChan1T1;
					dataframe->KadrCh1.s.SUMM=1;
					dataframe->KadrCh1.s.SizeX1=battle2.Target2SizeX1;
					dataframe->KadrCh1.s.SizeY1=battle2.Target2SizeY1;
				}
				else
					dataframe->KadrCh1.sw=0;

				//dataframe->KadrCh2.s.SUMM=battle.ControlState.s.SumChan2T1;
				dataframe->KadrCh2.s.SUMM=1;
				dataframe->KadrCh2.s.SizeX2=battle2.Target2SizeX2;
				dataframe->KadrCh2.s.SizeY2=battle2.Target2SizeY2;

				offset=sizeof(DataFileFrame);
				uint32 addrframe=(uint32)(*addr+ADDRRAMJOBFLASHPROGRAMMT2);
				memcpy((void*)addrframe,dataframe,offset);
				//XorFrame^=MyMemCPY((void*)addrframe,dataframe,(offset/2));
				memset((void*)(addrframe+offset), 0, 512);	//забиваем структуры info 0
				offset+=512;

				memcpy((void*)(addrframe+offset),&oed2,dataframe->SizePaketOEDTA);
				//XorFrame^=MyMemCPY((void*)(addrframe+offset),&oed1,(dataframe->SizePaketOEDTA/2));
				offset+=dataframe->SizePaketOEDTA;
				memcpy((void*)(addrframe+offset),&ta2,dataframe->SizePaketTAOED);
				//XorFrame^=MyMemCPY((void*)(addrframe+offset),&ta1,(dataframe->SizePaketTAOED/2));
				offset+=dataframe->SizePaketTAOED;

				//копируем изображения
				SizeByteCh1/=2;
				SizeByteCh2/=2;

				//канал 1
				if(battle2.ControlState2.s.SumChan1T2==1)
				{
					memcpy((void*)(addrframe+offset),(void *)(isInitialized[69]),SizeByteCh1);
					offset+=SizeByteCh1;
					memcpy((void*)(addrframe+offset),(void *)(isInitialized[71]),SizeByteCh1);
					offset+=SizeByteCh1;
				}
				else
				{
					memcpy((void*)(addrframe+offset),(void *)(isInitialized[79]),SizeByteCh1);
					offset+=SizeByteCh1;
					memcpy((void*)(addrframe+offset),(void *)(isInitialized[81]),SizeByteCh1);
					offset+=SizeByteCh1;
				}

				//канал 2

				if(battle2.ControlState2.s.SumChan2T2==1)
				{
					memcpy((void*)(addrframe+offset),(void *)(isInitialized[73]),SizeByteCh2);
					offset+=SizeByteCh2;
					memcpy((void*)(addrframe+offset),(void *)(isInitialized[75]),SizeByteCh2);
					offset+=SizeByteCh2;
				}
				else
				{
					memcpy((void*)(addrframe+offset),(void *)(isInitialized[83]),SizeByteCh2);
					offset+=SizeByteCh2;
					memcpy((void*)(addrframe+offset),(void *)(isInitialized[85]),SizeByteCh2);
					offset+=SizeByteCh2;
				}

				uint32 marker=(uint32)MarkerStopFrame;
				memcpy((void *)(addrframe+offset),&marker,4);
				//XorFrame^=MyMemCPY((void *)(addrframe+offset),&marker,2);
				offset+=4;
				uint16 Xor=0;
				uint8 * AdrXorChkSum = (uint8*)(addrframe+offset);
				memcpy((void*)AdrXorChkSum,&Xor,2);

				Xor=XorChekSUMCalc16((uint16*)addrframe,(offset/2));
				memcpy((void*)AdrXorChkSum,&Xor,2);

			//	memcpy((void*)AdrXorChkSum,&XorFrame,2);
				return dataframe->OffsetNextFrame;
		}
		else
			return 0;

	}
}


