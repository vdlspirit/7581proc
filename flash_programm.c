#include <ti/sysbios/BIOS.h>
#include <ti/ndk/inc/netmain.h>
#include <ti/ndk/inc/_stack.h>
#include <ti/ndk/inc/tools/servers.h>
#include "ti/platform/platform.h"
#include "ti/platform/resource_mgr.h"
#include <ti/sysbios/knl/Task.h>
#include "flash_programm.h"
#include "flashdata.h"
#include "PaketOEDTAEth.h"
#include "gpio.h"
#include "spi.h"

extern uint32 GlobalTimeJob;
extern uint32 GlobalBattleCount;
extern uint GlobalVerPO;
extern uint GlobalNumDev;
extern FramPuskStruct FPStarget1;
extern FramPuskStruct FPStarget2;

#define FLASH_DATA_PROG1_COMMAND		BASE_CE0_ADDR+0x4000
#define FLASH_DATA_PROG1_ADDRESS		BASE_CE0_ADDR+0x2000
#define FLASH_DATA_PROG1_DATA			BASE_CE0_ADDR

#define FLASH_DATA_PROG2_COMMAND		BASE_CE1_ADDR+0x4000
#define FLASH_DATA_PROG2_ADDRESS		BASE_CE1_ADDR+0x2000
#define FLASH_DATA_PROG2_DATA			BASE_CE1_ADDR

#define FLASH_DATA_PROG3_COMMAND		BASE_CE2_ADDR+0x4000
#define FLASH_DATA_PROG3_ADDRESS		BASE_CE2_ADDR+0x2000
#define FLASH_DATA_PROG3_DATA			BASE_CE2_ADDR


uint16 ErrasePuskFlash(uint16 NumPusk, uint32 Adrr, uint32 Size, uint16 Target)
{
	volatile int i,j,bad=0;
	uint16 status=0;
	volatile Uint16 slovo_data=0;
	uint16 R1=0,R2=0,R3=0;
	uint32 AddrFlash=0;
	uint32 NumBlock=0;

	AddrFlash=GetAddrPusk(NumPusk,Target);
	NumBlock=AddrFlash&0x00ffffff;

	if((AddrFlash&0xff000000)==0)
	{

		for(j=NumBlock;j<(NumBlock+256);j++)		//стираем всю NAND флеш по-блочно парами по 2 флешки пуск 512 Мб
		{
			R1=(uint16)(j&0x1)<<7;				//разбиваем номер блока на байты
			R1|=R1<<8;
			R2=(uint16)((j&0x1fe)>>1);
			R2|=R2<<8;
			R3=(uint16)((j&0x1fe00)>>9);
			R3|=R3<<8;

			REG_WRITE(FLASH_DATA_PROG1_COMMAND,0x6060);	//команду на стирание

			REG_WRITE(FLASH_DATA_PROG1_ADDRESS,R1);		//передаем адрес блока
			REG_WRITE(FLASH_DATA_PROG1_ADDRESS,R2);
			REG_WRITE(FLASH_DATA_PROG1_ADDRESS,R3);

			REG_WRITE(FLASH_DATA_PROG1_COMMAND,0xD0D0);	//команду на стирание завершение

			CycleDelay(1000);	//ожидаем минимум 1 микросекунду

			for(i=0;i<10000000;i++)		//максимум ожидаем 4 мс
			{
				status=ReadStatus(1);
				if(status==0xe0e0)
					break;
				else
				{
					bad++;
				}
			}
		}
	}
	else
	{
		for(j=NumBlock;j<(NumBlock+256);j++)		//стираем всю NAND флеш по-блочно парами по 2 флешки пуск 512 Мб
		{
			R1=(uint16)(j&0x1)<<7;				//разбиваем номер блока на байты
			R1|=R1<<8;
			R2=(uint16)((j&0x1fe)>>1);
			R2|=R2<<8;
			R3=(uint16)((j&0x1fe00)>>9);
			R3|=R3<<8;

			REG_WRITE(FLASH_DATA_PROG3_COMMAND,0x6060);	//команду на стирание

			REG_WRITE(FLASH_DATA_PROG3_ADDRESS,R1);		//передаем адрес блока
			REG_WRITE(FLASH_DATA_PROG3_ADDRESS,R2);
			REG_WRITE(FLASH_DATA_PROG3_ADDRESS,R3);

			REG_WRITE(FLASH_DATA_PROG3_COMMAND,0xD0D0);	//команду на стирание завершение

			CycleDelay(1000);	//ожидаем минимум 1 микросекунду

			for(i=0;i<10000000;i++)		//максимум ожидаем 4 мс
			{
				status=ReadStatus(3);
				if(status==0xe0e0)
					break;
				else
				{
					bad++;
				}
			}
		}
	}

	return 1;
}
uint16 ReadPuskFlash(uint16 NumPusk, uint32 Adrr, uint32 Size,uint16 Target)
{
	volatile uint32 i,j,bad=0, num=0;
	uint16 status=0;
	volatile Uint16 slovo_data=0;
	uint16 C1=0,C2=0,R1=0,R2=0,R3=0;
	uint16 *BuffPusk = (uint16*)Adrr;
	uint32 SizePuskMaxInPage = 32768;		//максимальный размер пуска в страницах
	uint32 SizePageInByte=4096;

	uint32 AddrFlash=0;
	uint32 NumBlock=0;
	uint32 NumPage=0;

	AddrFlash=GetAddrPusk(NumPusk,Target);
	NumBlock=AddrFlash&0x00ffffff;
	NumPage=NumBlock*128;

	if((AddrFlash&0xff000000)==0)
	{
			for(j=NumPage;j<(NumPage+SizePuskMaxInPage);j++)		//Читаем страницы
			{
				if(((j-NumPage)*SizePageInByte*2)>=Size)
				{
					return 1;
				}

				R1=(uint16)(j&0xff);					//формируем адрес записываемой страницы
				R1|=R1<<8;
				R2=(uint16)((j&0xff00)>>8);
				R2|=R2<<8;
				R3=(uint16)((j&0xff0000)>>16);
				R3|=R3<<8;

				REG_WRITE(FLASH_DATA_PROG1_COMMAND,0x0000);	//команду на начало чтения

				REG_WRITE(FLASH_DATA_PROG1_ADDRESS,C1);		//передаем адрес блока
				REG_WRITE(FLASH_DATA_PROG1_ADDRESS,C2);
				REG_WRITE(FLASH_DATA_PROG1_ADDRESS,R1);		//передаем адрес блока
				REG_WRITE(FLASH_DATA_PROG1_ADDRESS,R2);
				REG_WRITE(FLASH_DATA_PROG1_ADDRESS,R3);

				REG_WRITE(FLASH_DATA_PROG1_COMMAND,0x3030);	//команду на завершение чтения

				CycleDelay(40000);

				for(num=0;num<SizePageInByte;num++)
				{
					BuffPusk[(j-NumPage)*SizePageInByte+num]=REG_READ(FLASH_DATA_PROG1_DATA);
				}

				for(i=0;i<100000000;i++)
				{
					status=ReadStatus(1);
					if(status==0xe0e0)
						break;
					else
					{
						bad++;
					}
				}
			}
	}
	else
	{
		for(j=NumPage;j<(NumPage+SizePuskMaxInPage);j++)		//Читаем страницы
		{
			if(((j-NumPage)*SizePageInByte*2)>=Size)
			{
				return 1;
			}

			R1=(uint16)(j&0xff);					//формируем адрес записываемой страницы
			R1|=R1<<8;
			R2=(uint16)((j&0xff00)>>8);
			R2|=R2<<8;
			R3=(uint16)((j&0xff0000)>>16);
			R3|=R3<<8;

			REG_WRITE(FLASH_DATA_PROG3_COMMAND,0x0000);	//команду на запись

			REG_WRITE(FLASH_DATA_PROG3_ADDRESS,C1);		//передаем адрес блока
			REG_WRITE(FLASH_DATA_PROG3_ADDRESS,C2);
			REG_WRITE(FLASH_DATA_PROG3_ADDRESS,R1);		//передаем адрес блока
			REG_WRITE(FLASH_DATA_PROG3_ADDRESS,R2);
			REG_WRITE(FLASH_DATA_PROG3_ADDRESS,R3);

			REG_WRITE(FLASH_DATA_PROG3_COMMAND,0x3030);	//команду на завершение записи

			CycleDelay(30000);

			for(num=0;num<SizePageInByte;num++)
			{
				BuffPusk[(j-NumPage)*SizePageInByte+num]=REG_READ(FLASH_DATA_PROG3_DATA);
			}

			for(i=0;i<100000000;i++)
			{
				status=ReadStatus(3);
				if(status==0xe0e0)
					break;
				else
				{
					bad++;
				}
			}
		}
	}


		return 0;
}
uint16 WritePuskFlash(uint16 NumPusk, uint32 Adrr, uint32 Size, uint16 Target)
{
	volatile uint32 i,j,bad=0, num=0;
	uint16 status=0;
	volatile Uint16 slovo_data=0;
	uint16 C1=0,C2=0,R1=0,R2=0,R3=0;
	uint16 *BuffPusk = (uint16*)Adrr;
	uint32 SizePuskMaxInPage = 32768;		//максимальный размер пуска в страницах
	uint32 SizePageInByte=4096;

	uint32 AddrFlash=0;
	uint32 NumBlock=0;
	uint32 NumPage=0;

	AddrFlash=GetAddrPusk(NumPusk,Target);
	NumBlock=AddrFlash&0x00ffffff;
	NumPage=NumBlock*128;

	if((AddrFlash&0xff000000)==0)
	{
			for(j=NumPage;j<(NumPage+SizePuskMaxInPage);j++)					//Записываем страницы	на 512 Мб
			{
				if(((j-NumPage)*SizePageInByte*2)>=Size)
				{
					return 1;
				}

				R1=(uint16)(j&0xff);					//формируем адрес записываемой страницы
				R1|=R1<<8;
				R2=(uint16)((j&0xff00)>>8);
				R2|=R2<<8;
				R3=(uint16)((j&0xff0000)>>16);
				R3|=R3<<8;

				REG_WRITE(FLASH_DATA_PROG1_COMMAND,0x8080);	//команду на запись

				REG_WRITE(FLASH_DATA_PROG1_ADDRESS,C1);		//передаем адрес блока
				REG_WRITE(FLASH_DATA_PROG1_ADDRESS,C2);
				REG_WRITE(FLASH_DATA_PROG1_ADDRESS,R1);		//передаем адрес блока
				REG_WRITE(FLASH_DATA_PROG1_ADDRESS,R2);
				REG_WRITE(FLASH_DATA_PROG1_ADDRESS,R3);

				for(num=0;num<SizePageInByte;num++)
				{
					REG_WRITE(FLASH_DATA_PROG1_DATA,BuffPusk[(j-NumPage)*SizePageInByte+num]);
				}

				REG_WRITE(FLASH_DATA_PROG1_COMMAND,0x1010);	//команду на завершение записи

				for(i=0;i<100000000;i++)
				{
					status=ReadStatus(1);
					if(status==0xe0e0)
						break;
					else
					{
						bad++;
					}
				}
			}
	}
	else
	{
		for(j=NumPage;j<(NumPage+SizePuskMaxInPage);j++)					//Записываем страницы	на 512 Мб
		{
			if(((j-NumPage)*SizePageInByte*2)>=Size)
			{
				return 1;
			}

			R1=(uint16)(j&0xff);					//формируем адрес записываемой страницы
			R1|=R1<<8;
			R2=(uint16)((j&0xff00)>>8);
			R2|=R2<<8;
			R3=(uint16)((j&0xff0000)>>16);
			R3|=R3<<8;

			REG_WRITE(FLASH_DATA_PROG3_COMMAND,0x8080);	//команду на запись

			REG_WRITE(FLASH_DATA_PROG3_ADDRESS,C1);		//передаем адрес блока
			REG_WRITE(FLASH_DATA_PROG3_ADDRESS,C2);
			REG_WRITE(FLASH_DATA_PROG3_ADDRESS,R1);		//передаем адрес блока
			REG_WRITE(FLASH_DATA_PROG3_ADDRESS,R2);
			REG_WRITE(FLASH_DATA_PROG3_ADDRESS,R3);

			for(num=0;num<SizePageInByte;num++)
			{
				REG_WRITE(FLASH_DATA_PROG3_DATA,BuffPusk[(j-NumPage)*SizePageInByte+num]);
			}

			REG_WRITE(FLASH_DATA_PROG3_COMMAND,0x1010);	//команду на завершение записи
			for(i=0;i<100000000;i++)
			{
				status=ReadStatus(3);
				if(status==0xe0e0)
					break;
				else
				{
					bad++;
				}
			}
		}
	}


		return 0;
}
uint32 GetAddrPusk(uint16 NumPusk,uint16 Target)
{
	uint32 Addr=0;

	switch(NumPusk)
	{
		case 1:
		{
			Addr=0x00000000;	//Старший байт намер CE младший номер блока
			break;
		}
		case 2:
		{
			Addr=0x00000100;
			break;
		}
		case 3:
		{
			Addr=0x00000200;
			break;
		}
		case 4:
		{
			Addr=0x00000300;
			break;
		}
		case 5:
		{
			Addr=0x00000400;
			break;
		}
		case 6:
		{
			Addr=0x00000500;
			break;
		}
		case 7:
		{
			Addr=0x00000600;
			break;
		}
		case 8:
		{
			Addr=0x00000700;
			break;
		}
		case 9:
		{
			Addr=0x00000800;
			break;
		}
		case 10:
		{
			Addr=0x00000900;
			break;
		}
		case 11:
		{
			Addr=0x00000a00;
			break;
		}
		case 12:
		{
			Addr=0x00000b00;
			break;
		}
		case 13:
		{
			Addr=0x00000c00;
			break;
		}
		case 14:
		{
			Addr=0x00000d00;
			break;
		}
		default:
		{
			break;
		}
	}

	if(Target==1)
		return (Addr|0x01000000);
	else
		return  Addr;
}
uint16 SavePuskInDataFlash(uint16 NumPusk,uint32 Size, uint16 Target)
{
	//Проверяем Flash на работоспособность
	uint32 AddrSave=0;

	if(Target==0)
	{
		REG_WRITE(FLASH_DATA_PROG1_COMMAND,0xffff);
	}
	else
	{
		REG_WRITE(FLASH_DATA_PROG3_COMMAND,0xffff);
	}
	CycleDelay(500000);

	if(ReadIdDFlash()!=0)
	{
		return 0;
	}

	if(Target==0)
		AddrSave=0x82000000;	//Начинаем все записи с CE0
	else
		AddrSave=0x91A00000;	//Сохраняем на CE3
	//определяем адрес пуска с которым начинаем работать

//	AddrSave=GetAddrPusk(NumPusk);


	if(ErrasePuskFlash(NumPusk,AddrSave,Size,Target)!=1)
	{
		return 0;
	}

//	ReadPuskFlash(NumPusk,AddrSave,Size);

	WritePuskFlash(NumPusk,AddrSave,Size,Target);

//	ReadPuskFlash(NumPusk,AddrSave,Size);


	return 1;
}

uint16 ReadPuskOfDataFlash(uint16 NumPusk, uint32 Size,uint16 Target)
{
	//Проверяем Flash на работоспособность
		uint32 AddrSave=0;

		REG_WRITE(FLASH_DATA_PROG1_COMMAND,0xffff);
		CycleDelay(500000);

		if(ReadIdDFlash()!=0)
		{
			return 0;
		}

		AddrSave=0x82000000;	//Начинаем все записи пока с CE0
		ReadPuskFlash(NumPusk,AddrSave,Size,Target);
		return 1;
}

uint16 ReadStatus(uint32 Bank)
{
	uint16 Status=0;

	switch(Bank)
	{
		case 1:
		{
			REG_WRITE(FLASH_DATA_PROG1_COMMAND,0x7070);	//команду на считывание статуса
			Status=REG_READ(FLASH_DATA_PROG1_DATA);
			break;
		}
		case 2:
		{
			REG_WRITE(FLASH_DATA_PROG2_COMMAND,0x7070);	//команду на считывание статуса
			Status=REG_READ(FLASH_DATA_PROG2_DATA);
			break;
		}
		case 3:
		{
			REG_WRITE(FLASH_DATA_PROG3_COMMAND,0x7070);	//команду на считывание статуса
			Status=REG_READ(FLASH_DATA_PROG3_DATA);
			break;
		}
		default:
		{
			break;
		}
	}

	return Status;
}

void RWConfig(void)
{
	uint16 tmp[100], i=0;
	//reset
	REG_WRITE(FLASH_DATA_PROG1_COMMAND,0xffff);
	CycleDelay(500000);

	//Configuration
	REG_WRITE(FLASH_DATA_PROG1_COMMAND,0xEEEE);
	REG_WRITE(FLASH_DATA_PROG1_ADDRESS,0x1010);

	for(i=0;i<4;i++)
	{
		tmp[i]=REG_READ(FLASH_DATA_PROG1_DATA);
	}

	tmp[1]=0x0101;
	tmp[2]=0x0202;
	tmp[3]=0x0303;

	REG_WRITE(FLASH_DATA_PROG1_COMMAND,0xEFEF);
	REG_WRITE(FLASH_DATA_PROG1_ADDRESS,0x1010);

	for(i=0;i<4;i++)
	{
		REG_WRITE(FLASH_DATA_PROG1_DATA,tmp[i]);
	}

	REG_WRITE(FLASH_DATA_PROG1_COMMAND,0xEEEE);
	REG_WRITE(FLASH_DATA_PROG1_ADDRESS,0x1010);

	for(i=0;i<4;i++)
	{
		tmp[i]=REG_READ(FLASH_DATA_PROG1_DATA);
	}
}
uint8 ReadIdDFlash(void)
{
	uint16 tmp[4], i=0;

	//читаем ID для СЕ0
	REG_WRITE(FLASH_DATA_PROG1_COMMAND,0x9090);
	REG_WRITE(FLASH_DATA_PROG1_ADDRESS,0x2020);

	for(i=0;i<4;i++)
	{
		tmp[i]=REG_READ(FLASH_DATA_PROG1_DATA);
	}
	if((tmp[0]!=0x4f4f)||(tmp[1]!=0x4e4e)||(tmp[2]!=0x4646)||(tmp[3]!=0x4949))
	return 1;

	//читаем ID для СЕ1
	REG_WRITE(FLASH_DATA_PROG2_COMMAND,0x9090);
	REG_WRITE(FLASH_DATA_PROG2_ADDRESS,0x2020);

	for(i=0;i<4;i++)
	{
		tmp[i]=REG_READ(FLASH_DATA_PROG2_DATA);
	}

	if((tmp[0]!=0x4f4f)||(tmp[1]!=0x4e4e)||(tmp[2]!=0x4646)||(tmp[3]!=0x4949))
	return 2;

	//читаем ID для СЕ2
	REG_WRITE(FLASH_DATA_PROG3_COMMAND,0x9090);
	REG_WRITE(FLASH_DATA_PROG3_ADDRESS,0x2020);

	for(i=0;i<4;i++)
	{
		tmp[i]=REG_READ(FLASH_DATA_PROG3_DATA);
	}
	if((tmp[0]!=0x4f4f)||(tmp[1]!=0x4e4e)||(tmp[2]!=0x4646)||(tmp[3]!=0x4949))
	return 3;

	return 0;
}
void formatflash(uint32 metka)
{
/*	volatile int i,j;
	volatile int numblok,page;
	volatile unsigned int status,sboy=0;
	volatile Uint16 slovo_data=0;
	if(metka==0x43218765)
	{
		for(j=0;j<2048;j++)		//стираем всю NAND флеш
		{
			REG_WRITE(ADDR_NAND_ADDR,j);	//записываем адрес блока

			for(i=0;i<10000;i++)
			{
				status=(REG_READ(ADDR_STATUS)&0x0000ffff);
				if(status==0)
				break;
			}
			REG_WRITE(ADDR_NAND_COMMAND,2);	//записываем команду на стирание

			for(i=0;i<10000;i++)
			{
				status=(REG_READ(ADDR_STATUS)&0x0000ffff);
				if(status==0)
				break;
			}
			maserrorblok[j]=0xff;
		}

		for(numblok=0;numblok<2048;numblok++)	//считываем в каждом блоке все страницы и проверяем на 0xffff
		{
			REG_WRITE(ADDR_NAND_ADDR,numblok);	//записываем адрес блока

			for(page=0;page<64;page++)
			{
				for(i=0;i<10000;i++)
				{
					status=(REG_READ(ADDR_STATUS)&0x0000ffff);
					if(status==0)
					break;
				}
				REG_WRITE(ADDR_NAND_COMMAND,3);	//записываем команду на чтение

				for(i=0;i<10000;i++)
				{
					status=(REG_READ(ADDR_STATUS)&0x0000ffff);
					if(status==0)
					break;
				}

				for(j=0;j<2048;j++)
				{
					slovo_data=(REG_READ(ADDR_NAND_DATA_OUT)&0x0000ffff);
					if(slovo_data!=0xffff)
					{
					 	maserrorblok[numblok]=0x11;	//сбойный блок
					 	page=100;
					 	sboy++;
					 	break;
					}
				}

				for(i=0;i<10000;i++)
				{
					status=(REG_READ(ADDR_STATUS)&0x0000ffff);
					if(status==0)
					break;
				}

			}
		}

		for(numblok=0;numblok<2048;numblok++) //записываем во флеш данных тестовые значения
		{
			for(i=0;i<10000;i++)
				{
					status=(REG_READ(ADDR_STATUS)&0x0000ffff);
					if((status&0x2)==0)
					break;
				}

			REG_WRITE(ADDR_NAND_ADDR,numblok);	//записываем адрес блока

			for(page=0;page<64;page++)
			{
				for(i=0;i<10000;i++)
				{
					status=(REG_READ(ADDR_STATUS)&0x0000ffff);
					if((status&0x2)==0)
					break;
				}

				for(i=0;i<2048;i++)
				{
					REG_WRITE(ADDR_NAND_DATA_IN,0); //запись тестовых нулей
				}

				for(i=0;i<10000;i++)
				{
					status=(REG_READ(ADDR_STATUS)&0x0000ffff);
					if((status&0x1)==0)
					break;
				}

				REG_WRITE(ADDR_NAND_COMMAND,1);	//записываем команду на запись

			}
		}

		for(numblok=0;numblok<2048;numblok++)	//считываем в каждом блоке все страницы и проверяем на 0
		{
			REG_WRITE(ADDR_NAND_ADDR,numblok);	//записываем адрес блока

			for(page=0;page<64;page++)
			{
				for(i=0;i<10000;i++)
				{
					status=(REG_READ(ADDR_STATUS)&0x0000ffff);
					if(status==0)
					break;
				}
				REG_WRITE(ADDR_NAND_COMMAND,3);	//записываем команду на чтение

				for(i=0;i<10000;i++)
				{
					status=(REG_READ(ADDR_STATUS)&0x0000ffff);
					if(status==0)
					break;
				}

				for(j=0;j<2048;j++)
				{
					slovo_data=(REG_READ(ADDR_NAND_DATA_OUT)&0x0000ffff);
					if(slovo_data!=0)
					{
					 	maserrorblok[numblok]=0x11;	//сбойный блок
					 	page=100;
					 	sboy++;
					 	break;
					}
				}

				for(i=0;i<10000;i++)
				{
					status=(REG_READ(ADDR_STATUS)&0x0000ffff);
					if(status==0)
					break;
				}

			}
		}

		for(j=0;j<2048;j++)		//стираем всю NAND флеш
		{
			REG_WRITE(ADDR_NAND_ADDR,j);	//записываем адрес блока

			for(i=0;i<10000;i++)
			{
				status=(REG_READ(ADDR_STATUS)&0x0000ffff);
				if(status==0)
				break;
			}
			REG_WRITE(ADDR_NAND_COMMAND,2);	//записываем команду на стирание

			for(i=0;i<10000;i++)
			{
				status=(REG_READ(ADDR_STATUS)&0x0000ffff);
				if(status==0)
				break;
			}
		}

		maserrorblok[0]=0xff;	//0 блок сбоить не может
		REG_WRITE(ADDR_FRAM_ADDR_IN,0);	//0 блок данных таблица сбойных секторов
		for(j=0;j<10000;j++)
		{
			status=(REG_READ(ADDR_STATUS)&0x0000ffff);
			if(status==0)
				break;
		}

		for(i=0;i<2048;i++)
		{
			REG_WRITE(ADDR_FRAM_DATA_IN,maserrorblok[i]);

			for(j=0;j<10000;j++)
			{
				status=(REG_READ(ADDR_STATUS)&0x0000ffff);
				if(status==0)
					break;
			}

		}
	}*/
}

void MemCPYWithDelay(uint16* BuffOut, uint16* BuffIn, uint16 SizeByte)
{
	int i=0;

	for(i=0;i<(SizeByte/2);i++)
	{
		*(BuffOut+i)=*(BuffIn+i);
		CycleDelay(400);
	}
}

uint16 ChecksumCalck(uint16* Buff, int size)
{
	int i = 0;
	uint32 crc = 0;

	for (i = 0; i < size; i++) {
		crc += Buff[i];
	}
	crc = (crc & 0xffff) + (crc >> 16);
	crc = (crc & 0xffff) + (crc >> 16);
	crc = crc & 0xffff;
	crc = ~crc;
	crc = crc&0xffff;
	return crc;

}

void ReadFramParamData(FramPuskStruct * fps, uint16 Target)
{
	UInt16 test;
	Uint32 XOR=0;

	test=gpioReadDataBus();

	gpioWriteDataBus(test|0x01);		//переключаемся на работы с FRAM

	if(Target==0)
		memcpy(fps,(void *)FRAM_PARAM_Struct_NAND1,sizeof(FramPuskStruct));
	else
		memcpy(fps,(void *)FRAM_PARAM_Struct_NAND2,sizeof(FramPuskStruct));

	gpioWriteDataBus(test);

	XOR=XorChekSUMCalc32((uint32*)fps, (sizeof(FramPuskStruct)/4));

	fps->NumDev=GlobalNumDev;

	if((fps->MarkerStart!=0xABCDEF11)||(fps->MarkerStartStrPusk!=0xABAAAB22)||(XOR!=0))
	{
		//Структура пуста заполняем заново
		memset(fps,0,sizeof(FramPuskStruct));
		fps->MarkerStart=0xABCDEF11;
		fps->NumDev=GlobalNumDev;
		fps->PosNumRing=1;			//номер пуска в кольце
		fps->GlobalNumPusk=1;		//начинаем с пуска под номером 1
		fps->MarkerStartStrPusk=0xABAAAB22;

		//описываем первый пуск
		if(Target==0)
		{
			fps->NumPusk1=1;
			fps->AddrPusk1=0x70000000;		//CE0
		}
		else
		{
			fps->NumPusk1=1;
			fps->AddrPusk1=0x78000000;		//CE2
		}
	}
}

void WriteFramParamData(FramPuskStruct * fps,uint32 SizePusk,uint16 Target)
{
		switch(fps->PosNumRing)
		{
			case	1:
			{
				fps->NumPusk1=fps->GlobalNumPusk;
				if(Target==0)
				{
					fps->AddrPusk1=0x70000000;		//CE0
				}
				else
				{
					fps->AddrPusk1=0x78000000;		//CE2
				}
				fps->SizePusk1=SizePusk;
				break;
			}
			case	2:
			{
				fps->NumPusk2=fps->GlobalNumPusk;
				fps->AddrPusk2=fps->AddrPusk1+(fps->PosNumRing-1)*0xFA00000;		//CE0
				fps->SizePusk2=SizePusk;
				break;
			}
			case	3:
			{
				fps->NumPusk3=fps->GlobalNumPusk;
				fps->AddrPusk3=fps->AddrPusk1+(fps->PosNumRing-1)*0xFA00000;		//CE0
				fps->SizePusk3=SizePusk;
				break;
			}
			case	4:
			{
				fps->NumPusk4=fps->GlobalNumPusk;
				fps->AddrPusk4=fps->AddrPusk1+(fps->PosNumRing-1)*0xFA00000;		//CE0
				fps->SizePusk4=SizePusk;
				break;
			}
			case	5:
			{
				fps->NumPusk5=fps->GlobalNumPusk;
				fps->AddrPusk5=fps->AddrPusk1+(fps->PosNumRing-1)*0xFA00000;		//CE0
				fps->SizePusk5=SizePusk;
				break;
			}
			case	6:
			{
				fps->NumPusk6=fps->GlobalNumPusk;
				fps->AddrPusk6=fps->AddrPusk1+(fps->PosNumRing-1)*0xFA00000;		//CE0
				fps->SizePusk6=SizePusk;
				break;
			}
			case	7:
			{
				fps->NumPusk7=fps->GlobalNumPusk;
				fps->AddrPusk7=fps->AddrPusk1+(fps->PosNumRing-1)*0xFA00000;		//CE0
				fps->SizePusk7=SizePusk;
				break;
			}
			case	8:
			{
				fps->NumPusk8=fps->GlobalNumPusk;
				fps->AddrPusk8=fps->AddrPusk1+(fps->PosNumRing-1)*0xFA00000;		//CE0
				fps->SizePusk8=SizePusk;
				break;
			}
			case	9:
			{
				fps->NumPusk9=fps->GlobalNumPusk;
				fps->AddrPusk9=fps->AddrPusk1+(fps->PosNumRing-1)*0xFA00000;		//CE0
				fps->SizePusk9=SizePusk;
				break;
			}
			case	10:
			{
				fps->NumPusk10=fps->GlobalNumPusk;
				fps->AddrPusk10=fps->AddrPusk1+(fps->PosNumRing-1)*0xFA00000;		//CE0
				fps->SizePusk10=SizePusk;
				break;
			}
			case	11:
			{
				fps->NumPusk11=fps->GlobalNumPusk;
				fps->AddrPusk11=fps->AddrPusk1+(fps->PosNumRing-1)*0xFA00000;	//CE0
				fps->SizePusk11=SizePusk;
				break;
			}
			case	12:
			{
				fps->NumPusk12=fps->GlobalNumPusk;
				fps->AddrPusk12=fps->AddrPusk1+(fps->PosNumRing-1)*0xFA00000;		//CE0
				fps->SizePusk12=SizePusk;
				break;
			}
			case	13:
			{
				fps->NumPusk13=fps->GlobalNumPusk;
				fps->AddrPusk13=fps->AddrPusk1+(fps->PosNumRing-1)*0xFA00000;		//CE0
				fps->SizePusk13=SizePusk;
				break;
			}
			case	14:
			{
				fps->NumPusk14=fps->GlobalNumPusk;
				fps->AddrPusk14=fps->AddrPusk1+(fps->PosNumRing-1)*0xFA00000;		//CE0
				fps->SizePusk14=SizePusk;
				break;
			}
			default:{break;}
		}
		//fps->GlobalNumPusk++;
		fps->PosNumRing++;
		if(fps->PosNumRing>14)
			fps->PosNumRing=1;

		fps->XOR=XorChekSUMCalc32((uint32*)fps, (sizeof(FramPuskStruct)/4)-1);

		UInt16 test;

		test=gpioReadDataBus();

		gpioWriteDataBus(test|0x01);		//переключаемся на работы с FRAM

		if(Target==0)
			MemCPYWithDelay((uint16 *)FRAM_PARAM_Struct_NAND1,(uint16*)fps,sizeof(FramPuskStruct));
		else
			MemCPYWithDelay((uint16 *)FRAM_PARAM_Struct_NAND2,(uint16*)fps,sizeof(FramPuskStruct));

		gpioWriteDataBus(test);
}

uint16 Get_XorCheksum(uint8* Buff,uint16 SizeByte)
{
	int i=0;
	uint8 chksumbyte=0;
	SizeByte++;
	for(i=0;i<SizeByte;i++)
	{
		chksumbyte^=Buff[i];
	}
	Buff[i]=chksumbyte;

	uint16 mat=SizeByte/2;

	if((mat*2)==SizeByte)
	{
		SizeByte+=2;
		Buff[i+1]=0;
		SizeByte/=2;
		SizeByte|=0x8000;
	}
	else
	{
		SizeByte++;
		SizeByte/=2;
	}

	return SizeByte;
}
int Get_Descriptors(uint8* Buff,uint8 NumDes)
{
	int i=0;
	char NameProc[]="CPU VPU    ";
	char NameXVPU[]="XILINX VPU ";
	char NameXCh1[]="XILINX CH1 ";
	char NameXCh2[]="XILINX CH2 ";
   char NameFRAM1[]="OPTIONS - 1";
   char NameFRAM2[]="OPTIONS - 2";
   char NameFRAM3[]="OPTIONS - 3";
	Des prog;
	memset((void *) &prog, 0, sizeof(Des));

	prog.SizeBlockWrite=SIZEPAKETUDPFLASHWRITE;
	prog.SizeBlockRead=SIZEPAKETUDPFLASHREAD;
	prog.WordD1.s.TimeOutWrite=1;
	prog.WordD1.s.TimeOutProg=40;
	prog.WordD2.s.Restart=1;
	prog.VERPOH=0;

	switch(NumDes)
	{
		case 0:	//программирование центрального процессора
		{
			for(i=0;i<12;i++)
			{
				prog.NameProg[i]=NameProc[i];
			}
			prog.MaxSizeProg=MAXSIZEPROGRAMCPU;
			prog.WordD2.s.VERPO=GlobalVerPO;
			prog.VERPOH=(GlobalVerPO>>24)&0xff;
		}
		break;
		case 1:	//программирование Xilinx VPU
		{
			for(i=0;i<12;i++)
			{
				prog.NameProg[i]=NameXVPU[i];
			}
			prog.MaxSizeProg=MAXSIZEPROGRAMXVPU;
			prog.WordD2.s.VERPO=0;
		}
		break;
		case 2:	//программирование Xilinx канала 1
		{
			for(i=0;i<12;i++)
			{
				prog.NameProg[i]=NameXCh1[i];
			}
			prog.MaxSizeProg=MAXSIZEPROGRAMXPVP1;
			prog.WordD2.s.VERPO=0;
		}
		break;
		case 3:	//программирование Xilinx канала 2
		{
			for(i=0;i<12;i++)
			{
				prog.NameProg[i]=NameXCh2[i];
			}
			prog.MaxSizeProg=MAXSIZEPROGRAMXPVP2;
			prog.WordD2.s.VERPO=0;
		}
		break;
		case 4:	// FRAM - параметров внутренние ОЭД
		{
			for(i=0;i<12;i++)
			{
				prog.NameProg[i]=NameFRAM1[i];
			}
			prog.MaxSizeProg=MAXSIZEPROGRAMFRAM;
			prog.WordD2.s.VERPO=0;
			prog.WordD1.s.TimeOutProg=5;
		}
		break;
		case 5:	//FRAM - результатов диагностик
		{
			for(i=0;i<12;i++)
			{
				prog.NameProg[i]=NameFRAM2[i];
			}
			prog.MaxSizeProg=MAXSIZEPROGRAMFRAM;
			prog.WordD2.s.VERPO=0;
			prog.SizeBlockWrite=0;
			prog.WordD1.s.TimeOutProg=5;
		}
		break;
		case 6:	//FRAM - параметров математики
		{
			for(i=0;i<12;i++)
			{
				prog.NameProg[i]=NameFRAM3[i];
			}
			prog.MaxSizeProg=MAXSIZEPROGRAMFRAM;
			prog.WordD2.s.VERPO=0;
			prog.WordD1.s.TimeOutProg=5;
		}
		break;
		default: break;
	}

	uint8* tempbuff=(uint8*)&prog;
	for(i=0;i<(sizeof(Des)-3);i++)
	{
		Buff[i]=tempbuff[i];
	}



	return (sizeof(Des)-3);

}

int dtask_udp_flash( SOCKET s, UINT32 unused )
{
    struct sockaddr_in sin1;
    struct timeval     to;
    PcFlashPaket pcflashpak;
    OedFlashPaket oedflashpak;
    int                i=0,tmp=0;
    uint16 crccalc=0,crcsenddata=0;
    volatile uint16 kommand;
    uint8 *addrbuffmem=(uint8 *)ADDRRAMJOBFLASHPROGRAMM;
    uint8 FlagErraseFlash=0;

    (void)unused;

    // Configure our socket timeout to be 10 seconds
    to.tv_sec  = 10;
    to.tv_usec = 0;
    setsockopt( s, SOL_SOCKET, SO_SNDTIMEO, &to, sizeof( to ) );
    setsockopt( s, SOL_SOCKET, SO_RCVTIMEO, &to, sizeof( to ) );

    memset((void *) &pcflashpak, 0, sizeof(PcFlashPaket));
    memset((void *) &oedflashpak, 0, sizeof(oedflashpak));
    for(;;)
    {
        tmp = sizeof( sin1 );
   		i = (int) recvfrom(s, &pcflashpak, sizeof(PcFlashPaket), 0, (PSA) &sin1,	&tmp);

        // Spit any data back out
        if( i >= 8 )
        {
        	crccalc=ChecksumCalck((uint16*)&pcflashpak,2);
       		if(crccalc==pcflashpak.CheckSumTitle)
       			crccalc=ChecksumCalck((uint16*)&pcflashpak,((i-2)/2));

       		uint8 chet=0;
       			if((pcflashpak.SizePaket&0x8000)==0x8000)
       			{
       				pcflashpak.SizePaket=((pcflashpak.SizePaket&0xfff)*2)-1;
       			}
       			else
       			{
       				pcflashpak.SizePaket=((pcflashpak.SizePaket&0xfff)*2);
       				chet=1;
       			}

       			if(chet==1)
       			{
       				crcsenddata=pcflashpak.DataPak[pcflashpak.SizePaket];
       				crcsenddata|=pcflashpak.DataPak[pcflashpak.SizePaket+1]<<8;
       			}
       			else
       			{
       				crcsenddata=pcflashpak.DataPak[pcflashpak.SizePaket+1];
       				crcsenddata|=pcflashpak.DataPak[pcflashpak.SizePaket+2]<<8;
       			}

       		if(crccalc==crcsenddata)	//данные пришли правильные
       		{
       			kommand=pcflashpak.DataPak[0]|(pcflashpak.DataPak[1]<<8);

       			switch(kommand)
       			{

      				case TESTCHANNEL:
      					{
      							oedflashpak.Title=0x5a;
       							oedflashpak.Address=0;
       							oedflashpak.SizePaket=5;
       							crccalc=ChecksumCalck((uint16*)&oedflashpak,2);
       							oedflashpak.CheckSumTitle=crccalc;
       							oedflashpak.REQUEST=0;

     							uint8 *byteoed= (uint8*)&(oedflashpak.REQUEST);
       							uint8 chksumbyte=0;
       							for(i=0;i<(pcflashpak.SizePaket-3);i++)
       							{
       								oedflashpak.DataPak[i]=pcflashpak.DataPak[i+2];
       							}

       							for(i=0;i<9;i++)
       							{
       								chksumbyte^=byteoed[i];
       							}
       							byteoed[i]=chksumbyte;
       							crccalc=ChecksumCalck((uint16*)&oedflashpak,8);
       							oedflashpak.DataPak[9]=(crccalc&0xff);
       							oedflashpak.DataPak[10]=(crccalc>>8);

       							sendto(s, &oedflashpak, 18, 0, (PSA) &sin1,sizeof(sin1));
       							break;
      					}

       				case REQUESTINFO:
       					{
       						char mass[]="1OED1CM - 2015 ";
       						int size = sizeof(mass);
       						oedflashpak.Title=0x5a;
       						oedflashpak.Address=0;
       						oedflashpak.SizePaket=(size+2)/2;
       						crccalc=ChecksumCalck((uint16*)&oedflashpak,2);
       						oedflashpak.CheckSumTitle=crccalc;
       						oedflashpak.REQUEST=0;

       						uint8 *byteoed= (uint8*)&(oedflashpak.REQUEST);
       						uint8 chksumbyte=0;
       						for(i=0;i<size;i++)
       						{
       							oedflashpak.DataPak[i]=mass[i];
       						}
       						for(i=0;i<(size+1);i++)
       						{
       							chksumbyte^=byteoed[i];
       						}
       						byteoed[i]=chksumbyte;
       						crccalc=ChecksumCalck((uint16*)&oedflashpak,oedflashpak.SizePaket+3);

       						byteoed[i+1]=(crccalc&0xff);
       						byteoed[i+2]=(crccalc>>8);

       						sendto(s, &oedflashpak, (oedflashpak.SizePaket+4)*2, 0, (PSA) &sin1,sizeof(sin1));

       						break;
       					}

       				case REQUESTDIAG:
       				{
       					char mass[]="Result of diagnosis. ";
       					int size = sizeof(mass);
       					oedflashpak.Title=0x5a;
       					oedflashpak.Address=0;
       					oedflashpak.SizePaket=(size+2)/2;
       					crccalc=ChecksumCalck((uint16*)&oedflashpak,2);
       					oedflashpak.CheckSumTitle=crccalc;
       					oedflashpak.REQUEST=0;

       					uint8 *byteoed= (uint8*)&(oedflashpak.REQUEST);
       					uint8 chksumbyte=0;
       					for(i=0;i<size;i++)
       					{
       						oedflashpak.DataPak[i]=mass[i];
       					}
       					for(i=0;i<(size+1);i++)
       					{
       						chksumbyte^=byteoed[i];
       					}
       					byteoed[i]=chksumbyte;
       					crccalc=ChecksumCalck((uint16*)&oedflashpak,oedflashpak.SizePaket+3);

       					byteoed[i+1]=(crccalc&0xff);
       					byteoed[i+2]=(crccalc>>8);

       					sendto(s, &oedflashpak, (oedflashpak.SizePaket+4)*2, 0, (PSA) &sin1,sizeof(sin1));
   						break;
       				}

       				case NUMPROGRAMM:
       				{
       					oedflashpak.Title=0x5a;
       					oedflashpak.Address=0;
       					oedflashpak.SizePaket=2|0x8000;
       					crccalc=ChecksumCalck((uint16*)&oedflashpak,2);
       					oedflashpak.CheckSumTitle=crccalc;
       					oedflashpak.REQUEST=0;
       					oedflashpak.DataPak[0]=7;
       					oedflashpak.DataPak[1]=7;
       					oedflashpak.DataPak[2]=0;

       					crccalc=ChecksumCalck((uint16*)&oedflashpak,(oedflashpak.SizePaket&0xfff)+3);
       					oedflashpak.DataPak[3]=(crccalc&0xff);
       					oedflashpak.DataPak[4]=(crccalc>>8);
       					sendto(s, &oedflashpak, ((oedflashpak.SizePaket&0xfff)+4)*2, 0, (PSA) &sin1,sizeof(sin1));
       					break;
       				}
       				case READDESCRIPTOR:
       				{
       					oedflashpak.Title=0x5a;
       					oedflashpak.Address=0;
       					oedflashpak.REQUEST=0;

       					uint16 sizebyte=Get_Descriptors((uint8*)&oedflashpak.DataPak[0],pcflashpak.DataPak[2]);

       					uint8 *byteoed= (uint8*)&(oedflashpak.REQUEST);
       					sizebyte=Get_XorCheksum(byteoed,sizebyte);
       					oedflashpak.SizePaket=sizebyte;
       					sizebyte&=0xfff;
       					sizebyte*=2;

     					crccalc=ChecksumCalck((uint16*)&oedflashpak,2);
       					oedflashpak.CheckSumTitle=crccalc;

       				    crccalc=ChecksumCalck((uint16*)&oedflashpak,(oedflashpak.SizePaket&0xfff)+3);
       				    oedflashpak.DataPak[sizebyte-1]=(crccalc&0xff);
       				    oedflashpak.DataPak[sizebyte]=(crccalc>>8);
       				    sendto(s, &oedflashpak, ((oedflashpak.SizePaket&0xfff)+4)*2, 0, (PSA) &sin1,sizeof(sin1));
       				    break;
       				 }

       				case STARTPROGRAMM:
       				 {
       					FlagErraseFlash=pcflashpak.DataPak[3];
       					ReadDataFlash((uint16*)ADDRRAMJOBFLASHPROGRAMM,pcflashpak.DataPak[2],FlagErraseFlash);

       					addrbuffmem=(uint8 *)ADDRRAMJOBFLASHPROGRAMM;//указатель на начало памяти для чтения
       				 	oedflashpak.Title=0x5a;
       				 	oedflashpak.Address=0;
       				 	oedflashpak.SizePaket=1;
       				 	crccalc=ChecksumCalck((uint16*)&oedflashpak,2);
       				 	oedflashpak.CheckSumTitle=crccalc;
       				 	oedflashpak.REQUEST=0;
       				 	oedflashpak.DataPak[0]=0;

       				 	crccalc=ChecksumCalck((uint16*)&oedflashpak,(oedflashpak.SizePaket&0xfff)+3);
       				 	oedflashpak.DataPak[1]=(crccalc&0xff);
       				 	oedflashpak.DataPak[2]=(crccalc>>8);
       				 	sendto(s, &oedflashpak, ((oedflashpak.SizePaket&0xfff)+4)*2, 0, (PSA) &sin1,sizeof(sin1));

       				 	break;
       				 }
       				case FINISHPROGRAMM:
       				 {
       					addrbuffmem=(uint8 *)ADDRRAMJOBFLASHPROGRAMM;//указатель на начало памяти для чтения
       					ProgrammFlash(addrbuffmem,pcflashpak.DataPak[2],FlagErraseFlash);
       				 	oedflashpak.Title=0x5a;
       				 	oedflashpak.Address=0;
       				 	oedflashpak.SizePaket=1;
       				 	crccalc=ChecksumCalck((uint16*)&oedflashpak,2);
       				 	oedflashpak.CheckSumTitle=crccalc;
       				 	oedflashpak.REQUEST=0;
       				 	oedflashpak.DataPak[0]=0;

       				 	crccalc=ChecksumCalck((uint16*)&oedflashpak,(oedflashpak.SizePaket&0xfff)+3);
       				 	oedflashpak.DataPak[1]=(crccalc&0xff);
       				 	oedflashpak.DataPak[2]=(crccalc>>8);
       				 	sendto(s, &oedflashpak, ((oedflashpak.SizePaket&0xfff)+4)*2, 0, (PSA) &sin1,sizeof(sin1));
       				 	break;
       				 }
       				case READDATA:
       				 {
       					int j=0;
       					uint16 sizebyte=0;

       				 	oedflashpak.Title=0x5a;
       				 	oedflashpak.Address=0;
       				 	oedflashpak.SizePaket=SIZEPAKETUDPFLASHREAD/2;

       				 	oedflashpak.REQUEST=0;

       				 	for(j=0;j<SIZEPAKETUDPFLASHREAD;j++)
       				 	{
       				 		oedflashpak.DataPak[j]=addrbuffmem[j];	//копируем данные в буфер обмена
       				 	}
       				 	addrbuffmem+=SIZEPAKETUDPFLASHREAD;

       				 	uint8 *byteoed= (uint8*)&(oedflashpak.REQUEST);
       				    sizebyte=Get_XorCheksum(byteoed,((oedflashpak.SizePaket&0xfff)*2));

       				    oedflashpak.SizePaket=sizebyte;
       				 	crccalc=ChecksumCalck((uint16*)&oedflashpak,2);
       				 	oedflashpak.CheckSumTitle=crccalc;

       				 	crccalc=ChecksumCalck((uint16*)&oedflashpak,(oedflashpak.SizePaket&0xfff)+3);
       				 	oedflashpak.DataPak[j+1]=(crccalc&0xff);
       				 	oedflashpak.DataPak[j+2]=(crccalc>>8);
       				 	sendto(s, &oedflashpak, ((oedflashpak.SizePaket&0xfff)+4)*2, 0, (PSA) &sin1,sizeof(sin1));

       				 	break;
       				 }
       				case WRITEDATA:
       				 {
       					uint8 error=WriteDatatoDDR((UInt8*) &pcflashpak.DataPak[3],(uint8*)addrbuffmem,pcflashpak.DataPak[2]);
       					addrbuffmem+=SIZEPAKETUDPFLASHREAD;

       				 	oedflashpak.Title=0x5a;
       				 	oedflashpak.Address=0;
       				 	oedflashpak.SizePaket=1;
       				 	crccalc=ChecksumCalck((uint16*)&oedflashpak,2);
       				 	oedflashpak.CheckSumTitle=crccalc;
       				 	if(error==0)
       				 	{
							oedflashpak.REQUEST=0;
							oedflashpak.DataPak[0]=0;
       				 	}
       				 	else
       				 	{
							oedflashpak.REQUEST=0xff;
							oedflashpak.DataPak[0]=0xff;
       				 	}

       				 	crccalc=ChecksumCalck((uint16*)&oedflashpak,(oedflashpak.SizePaket&0xfff)+3);
       				 	oedflashpak.DataPak[1]=(crccalc&0xff);
       				 	oedflashpak.DataPak[2]=(crccalc>>8);
       				 	sendto(s, &oedflashpak, ((oedflashpak.SizePaket&0xfff)+4)*2, 0, (PSA) &sin1,sizeof(sin1));
       				 	break;
       				 }
       				case ERRASEPROGRAMM:
       				{
       					addrbuffmem=(uint8 *)ADDRRAMJOBFLASHPROGRAMM;//указатель на начало памяти для чтения

       					if(pcflashpak.DataPak[2]==0)
       					{
       						UInt16 test;

       						test=gpioReadDataBus();

		 	 	 			gpioWriteDataBus(test|0x78);
		 	 	 			spi_cmos_erase_all_flash(MAXSIZEPROGRAMCPU,0);
		 	 	 			gpioWriteDataBus(test);

							oedflashpak.Title=0x5a;
							oedflashpak.Address=0;
							oedflashpak.SizePaket=1;
							crccalc=ChecksumCalck((uint16*)&oedflashpak,2);
							oedflashpak.CheckSumTitle=crccalc;
							oedflashpak.REQUEST=0;
							oedflashpak.DataPak[0]=0;
       					}
       					else
       					{
       						oedflashpak.Title=0x5a;
       						oedflashpak.Address=0;
       						oedflashpak.SizePaket=1;
       						crccalc=ChecksumCalck((uint16*)&oedflashpak,2);
       						oedflashpak.CheckSumTitle=crccalc;
       						oedflashpak.REQUEST=0xff;
       						oedflashpak.DataPak[0]=0xff;
       					}

       					crccalc=ChecksumCalck((uint16*)&oedflashpak,(oedflashpak.SizePaket&0xfff)+3);
       					oedflashpak.DataPak[1]=(crccalc&0xff);
       					oedflashpak.DataPak[2]=(crccalc>>8);
       					sendto(s, &oedflashpak, ((oedflashpak.SizePaket&0xfff)+4)*2, 0, (PSA) &sin1,sizeof(sin1));
       					break;
       				}



       				default: {break;}
       			}
       		}


        }
        else
            break;
    }

    // Since the socket is still open, return "1"
    // (we need to leave UDP sockets open)
    return(1);
}

uint16 ReadDataFlash(UInt16* DataMass,uint8 NumDes,uint8 Flag)
{
	UInt16 test;

	test=gpioReadDataBus();

		 switch(NumDes)	//выбираем номер устройства
		 {
		 	 case	0:	//работаем с flash CPU
		 	 	 	 {
		 	 	 		 if(Flag==0)
		 	 	 		 {
		 	 	 			gpioWriteDataBus(test|0x78);
		 	 	 			spi_cmos_read_flash(MAXSIZEPROGRAMCPU, 0,(uint16_t*)ADDRRAMJOBFLASHPROGRAMM, 0);
		 	 	 			gpioWriteDataBus(test);
		 	 	 		 }
		 	 	 		 break;
		 	 	 	 }
		 	 case	1:{break;}
		 	 case	2:{break;}
		 	 case	3:{break;}
		 	 case	4:{//Параметры внутренние ОЭД
		 		 	 	 if(Flag==0)
		 				 {
		 				 	gpioWriteDataBus(test|0x01);		//переключаемся на работы с FRAM

		 				 	memcpy(DataMass,(void *)FRAM_PARAM_OED,SIZEPAKETUDPFLASHREAD);

		 				 	gpioWriteDataBus(test);
		 				  }
		 		 	 	 break;
		 	 	 	  }
		 	 case	5:{//Результаты последних диагностик
		 		 	 	 if(Flag==0)
		 				 {
		 				 	gpioWriteDataBus(test|0x01);		//переключаемся на работы с FRAM

		 				 	memcpy(DataMass,(void *)FRAM_PARAM_DIAGN,SIZEPAKETUDPFLASHREAD);

		 				 	gpioWriteDataBus(test);
		 				  }
		 		 	 	 break;
		 	 	 	  }
		 	 case	6:{//Блок параметров для математики
		 		 	 	 if(Flag==0)
		 				 {
		 				 	gpioWriteDataBus(test|0x01);		//переключаемся на работы с FRAM

		 				 	memcpy(DataMass,(void *)FRAM_PARAM_OUT,SIZEPAKETUDPFLASHREAD);

		 				 	gpioWriteDataBus(test);
		 				  }
		 		 	 	 break;
		 	 	 	  }
		 	 default:{break;}
		 }
	return 1;
}

uint8 WriteDatatoDDR(UInt8* DataMass,UInt8* DataMassDDR,uint8 NumDes)
{
	memcpy(DataMassDDR,DataMass,SIZEPAKETUDPFLASHREAD);

	return memcmp(DataMassDDR,DataMass,SIZEPAKETUDPFLASHREAD);
}

void ProgrammFlash(void *DataMass,uint8 NumDes,uint8 Flag)
{
	UInt16 test;

	test=gpioReadDataBus();

	switch(NumDes)	//выбираем номер устройства
			 {
			 	 case	0:	//работаем с flash CPU
			 	 	 	 {
			 	 	 		 if(Flag==1)//разрешение на программирование
			 	 	 		 {
			 	 	 			gpioWriteDataBus(test|0x78);
			 	 	 			spi_cmos_erase_all_flash(MAXSIZEPROGRAMCPU,0);
			 	 	 			spi_cmos_write_flash(MAXSIZEPROGRAMCPU,0,(uint8_t*)ADDRRAMJOBFLASHPROGRAMM,0);
			 	 	 			gpioWriteDataBus(test);
			 	 	 		 }
			 	 	 		 break;
			 	 	 	 }
			 	 case	1:{break;}
			 	 case	2:{break;}
			 	 case	3:{break;}
			 	 case	4:{
							 if(Flag==1)
							 {
								gpioWriteDataBus(test|0x01);		//переключаемся на работы с FRAM

								uint32* base=(uint32*)FRAM_PARAM_OED;

								MemCPYWithDelay((uint16 *)FRAM_PARAM_OED,DataMass,SIZEPAKETUDPFLASHREAD);

			 	 	 			GlobalTimeJob=*(base+1);
			 	 	 			GlobalBattleCount=*(base+2);

								gpioWriteDataBus(test);
							  }
	 				 	break;}
			 	 case	5:{break;}
			 	 case	6:
						 {
							 if(Flag==1)
							 {
								gpioWriteDataBus(test|0x01);		//переключаемся на работы с FRAM

								MemCPYWithDelay((uint16 *)FRAM_PARAM_OUT,DataMass,SIZEPAKETUDPFLASHREAD);

								gpioWriteDataBus(test);
							  }
							 break;
						 }

			 	 default:{break;}
			 }
}

int dtask_d_flash( SOCKET s, UINT32 unused )
{
    struct sockaddr_in sin1;
    struct timeval     to;
    FlashDataCommand FlDataCommmand;
    int                i=0,tmp=0;
    uint32 *addrbuffmem=(uint32 *)ADDRRAMJOBFLASHPROGRAMM;
    uint32 BuffOut[350],xor=0;

    (void)unused;

    // Configure our socket timeout to be 10 seconds
    to.tv_sec  = 120;
    to.tv_usec = 0;
    setsockopt( s, SOL_SOCKET, SO_SNDTIMEO, &to, sizeof( to ) );
    setsockopt( s, SOL_SOCKET, SO_RCVTIMEO, &to, sizeof( to ) );

    memset((void *) &FlDataCommmand, 0, sizeof(FlashDataCommand));
    memset((void *) &BuffOut, 0, 1400);
    for(;;)
    {
        tmp = sizeof( sin1 );
   		i = (int) recvfrom(s, &FlDataCommmand, sizeof(FlashDataCommand), 0, (PSA) &sin1,	&tmp);
   		memset((void *) &BuffOut, 0, 1400);

   		if(i==64)
   		{
   			//проверяем правильность командного пакета
   			xor=XorChekSUMCalc32((uint32*)&FlDataCommmand,sizeof(FlashDataCommand)/4);
   			if(xor==0)
   			{
   				switch((FlDataCommmand.Command.sw&0x0000001f))
   				{
   					case	1:	//считываем пуск из flash данных
   					{
   						if(FlDataCommmand.Command.s.NumTarget==0)
							if(ReadPuskOfDataFlash(FlDataCommmand.NumPusk,FlDataCommmand.SizePaket,0)==1)	//читаем по цели 1!!!!!!!!!
							{
								BuffOut[0]=0xaa55faaf;
								BuffOut[1]=0x10000001;
								BuffOut[2]=0xffaa11bb;
							}
							else
							{
								BuffOut[0]=0xaa55faaf;
								BuffOut[1]=0xbeddbedd;
								BuffOut[2]=0xffaa11bb;
							}
   						else
							if(ReadPuskOfDataFlash(FlDataCommmand.NumPusk,FlDataCommmand.SizePaket,1)==1)	//читаем по цели 2!!!!!!!!!
							{
								BuffOut[0]=0xaa55faaf;
								BuffOut[1]=0x10000001;
								BuffOut[2]=0xffaa11bb;
							}
							else
							{
								BuffOut[0]=0xaa55faaf;
								BuffOut[1]=0xbeddbedd;
								BuffOut[2]=0xffaa11bb;
							}

   						BuffOut[3]=XorChekSUMCalc32((uint32*)&BuffOut,3);
   						sendto(s, &BuffOut, 16, 0, (PSA) &sin1,sizeof(sin1));
   						break;
   					}
   					case	2:	//Запрос на чтение информации о пусках
   					{

   						if(FlDataCommmand.Command.s.NumTarget==0)
   						{
   							BuffOut[0]=0xaa55ccbb;
   							BuffOut[1]=0x10000001;
   							ReadFramParamData(&FPStarget1,0);


   							BuffOut[2]=FPStarget1.NumDev;
   							BuffOut[3]=FPStarget1.GlobalNumPusk-1;
   							if(BuffOut[3]>14)BuffOut[3]=14;
   							BuffOut[4]=1;
   							//описание диагностик
   							BuffOut[5]=0;
   							BuffOut[6]=0;
   							//описание пусков
   							BuffOut[7]=FPStarget1.NumPusk1;
   							BuffOut[8]=FPStarget1.SizePusk1;
   							BuffOut[9]=FPStarget1.NumPusk2;
   							BuffOut[10]=FPStarget1.SizePusk2;
   							BuffOut[11]=FPStarget1.NumPusk3;
   							BuffOut[12]=FPStarget1.SizePusk3;
   							BuffOut[13]=FPStarget1.NumPusk4;
   							BuffOut[14]=FPStarget1.SizePusk4;
   							BuffOut[15]=FPStarget1.NumPusk5;
   							BuffOut[16]=FPStarget1.SizePusk5;
   							BuffOut[17]=FPStarget1.NumPusk6;
   							BuffOut[18]=FPStarget1.SizePusk6;
   							BuffOut[19]=FPStarget1.NumPusk7;
   							BuffOut[20]=FPStarget1.SizePusk7;
   							BuffOut[21]=FPStarget1.NumPusk8;
   							BuffOut[22]=FPStarget1.SizePusk8;
   							BuffOut[23]=FPStarget1.NumPusk9;
   							BuffOut[24]=FPStarget1.SizePusk9;
   							BuffOut[25]=FPStarget1.NumPusk10;
   							BuffOut[26]=FPStarget1.SizePusk10;
   							BuffOut[27]=FPStarget1.NumPusk11;
   							BuffOut[28]=FPStarget1.SizePusk11;
   							BuffOut[29]=FPStarget1.NumPusk12;
   							BuffOut[30]=FPStarget1.SizePusk12;
   							BuffOut[31]=FPStarget1.NumPusk13;
   							BuffOut[32]=FPStarget1.SizePusk13;
   							BuffOut[33]=FPStarget1.NumPusk14;
   							BuffOut[34]=FPStarget1.SizePusk14;
   							BuffOut[35]=0xffaa11bb;
   							BuffOut[36]=XorChekSUMCalc32((uint32*)&BuffOut,36);
   							sendto(s, &BuffOut, 148, 0, (PSA) &sin1,sizeof(sin1));
   						}
   						else
   						{
   							BuffOut[0]=0xaa55ccbb;
   							BuffOut[1]=0x10000001;
   							ReadFramParamData(&FPStarget2,1);


   							BuffOut[2]=FPStarget2.NumDev;
   							BuffOut[3]=FPStarget2.GlobalNumPusk-1;
   							if(BuffOut[3]>14)BuffOut[3]=14;
   							BuffOut[4]=1;
   							//описание диагностик
   							BuffOut[5]=0;
   							BuffOut[6]=0;
   							//описание пусков
   							BuffOut[7]=FPStarget2.NumPusk1;
   							BuffOut[8]=FPStarget2.SizePusk1;
   							BuffOut[9]=FPStarget2.NumPusk2;
   							BuffOut[10]=FPStarget2.SizePusk2;
   							BuffOut[11]=FPStarget2.NumPusk3;
   							BuffOut[12]=FPStarget2.SizePusk3;
   							BuffOut[13]=FPStarget2.NumPusk4;
   							BuffOut[14]=FPStarget2.SizePusk4;
   							BuffOut[15]=FPStarget2.NumPusk5;
   							BuffOut[16]=FPStarget2.SizePusk5;
   							BuffOut[17]=FPStarget2.NumPusk6;
   							BuffOut[18]=FPStarget2.SizePusk6;
   							BuffOut[19]=FPStarget2.NumPusk7;
   							BuffOut[20]=FPStarget2.SizePusk7;
   							BuffOut[21]=FPStarget2.NumPusk8;
   							BuffOut[22]=FPStarget2.SizePusk8;
   							BuffOut[23]=FPStarget2.NumPusk9;
   							BuffOut[24]=FPStarget2.SizePusk9;
   							BuffOut[25]=FPStarget2.NumPusk10;
   							BuffOut[26]=FPStarget2.SizePusk10;
   							BuffOut[27]=FPStarget2.NumPusk11;
   							BuffOut[28]=FPStarget2.SizePusk11;
   							BuffOut[29]=FPStarget2.NumPusk12;
   							BuffOut[30]=FPStarget2.SizePusk12;
   							BuffOut[31]=FPStarget2.NumPusk13;
   							BuffOut[32]=FPStarget2.SizePusk13;
   							BuffOut[33]=FPStarget2.NumPusk14;
   							BuffOut[34]=FPStarget2.SizePusk14;
   							BuffOut[35]=0xffaa11bb;
   							BuffOut[36]=XorChekSUMCalc32((uint32*)&BuffOut,36);
   							sendto(s, &BuffOut, 148, 0, (PSA) &sin1,sizeof(sin1));
   						}
   						break;
   					}
   					case	4:	//Команда на форматирование flash памяти.
   					{
   						Task_sleep(60000);
   						BuffOut[0]=0xaa55faaf;
   						BuffOut[1]=0x10000001;
   						BuffOut[2]=0xffaa11bb;
   						BuffOut[3]=XorChekSUMCalc32((uint32*)&BuffOut,3);
   						sendto(s, &BuffOut, 16, 0, (PSA) &sin1,sizeof(sin1));
   						break;
   					}
   					case	8:	//Команда на удаление пусков из flash памяти.
   					{
   						Task_sleep(60000);
   						BuffOut[0]=0xaa55faaf;
   						BuffOut[1]=0x10000001;
   						BuffOut[2]=0xffaa11bb;
   						BuffOut[3]=XorChekSUMCalc32((uint32*)&BuffOut,3);
   						sendto(s, &BuffOut, 16, 0, (PSA) &sin1,sizeof(sin1));
   						break;
   					}
   					case	16:	//Команда на Чтение очередного пакета из DDR
   					{
   						if(FlDataCommmand.Command.s.NumTarget==0)
   						{
   							if(FlDataCommmand.SizePaket<=1380)
   							{
								BuffOut[0]=0xaa55ddee;
								BuffOut[1]=0x10000001;
								BuffOut[2]=FlDataCommmand.SizePaket;
								uint32 offset=(FlDataCommmand.OffsetPusk/4);
								for(i=3;i<((FlDataCommmand.SizePaket/4)+3);i++)
								{
									BuffOut[i]=addrbuffmem[offset+i-3];
								}
								BuffOut[i]=0xffaa11bb;
								BuffOut[i+1]=XorChekSUMCalc32((uint32*)&BuffOut,(FlDataCommmand.SizePaket/4)+5);
								sendto(s, &BuffOut, (FlDataCommmand.SizePaket+20), 0, (PSA) &sin1,sizeof(sin1));
   							}
   							else
   							{
   	   	   						BuffOut[0]=0xaa55faaf;
   	   	   						BuffOut[1]=0x10000001;
   	   	   						BuffOut[2]=0xffaa11bb;
   	   	   						BuffOut[3]=XorChekSUMCalc32((uint32*)&BuffOut,3);
   	   	   						sendto(s, &BuffOut, 16, 0, (PSA) &sin1,sizeof(sin1));
   							}

   						}
   						else
   						{
   							if(FlDataCommmand.SizePaket<=1380)
   							 {
   								BuffOut[0]=0xaa55ddee;
   								BuffOut[1]=0x10000001;
   								BuffOut[2]=FlDataCommmand.SizePaket;
   								uint32 offset=(FlDataCommmand.OffsetPusk/4);
   								for(i=3;i<((FlDataCommmand.SizePaket/4)+3);i++)
   								{
   									BuffOut[i]=addrbuffmem[offset+i-3];
   								}
   								BuffOut[i]=0xffaa11bb;
   								BuffOut[i+1]=XorChekSUMCalc32((uint32*)&BuffOut,(FlDataCommmand.SizePaket/4)+5);
   								sendto(s, &BuffOut, (FlDataCommmand.SizePaket+20), 0, (PSA) &sin1,sizeof(sin1));
   							}
   							else
   							{
   								BuffOut[0]=0xaa55faaf;
   								BuffOut[1]=0x10000001;
   								BuffOut[2]=0xffaa11bb;
   								BuffOut[3]=XorChekSUMCalc32((uint32*)&BuffOut,3);
   								sendto(s, &BuffOut, 16, 0, (PSA) &sin1,sizeof(sin1));
   							}
   						}
   						break;
   					}
   					default:{break;}
   				}
   			}
   		}
        else
            break;
    }

    // Since the socket is still open, return "1"
    // (we need to leave UDP sockets open)
    return(1);
}
void CreateTestDataProgramm(uint32 Size)
{
	uint32 * mass=(uint32 *)ADDRRAMJOBFLASHPROGRAMM;	//память где будет лежать программа
	int i=0;

	for(i=0;i<Size;i++)
	{
		mass[i]=i;
	}
}

