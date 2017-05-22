/*
 * flashdata.h
 *
 *  Created on: 22.10.2015
 *      Author: Admin
 */
#include "type.h"

#ifndef FLASHDATA_H_
#define FLASHDATA_H_

#define MarkerStartTitle 	0xA5B6C7D8
#define MarkerStartFrame	0xE4AABB4E
#define MarkerStopFrame		0xA5CCDD5A

typedef struct FlashDataFramStruct	//структура сохраненных пакетов во fram памяти
{
	uint32 	MarkerStart;
	uint32 	NumDev;					//номер прибора
	uint32	PosNumRing;				//номер следующей позиции в кольце
	uint32  GlobalNumPusk;			//глобальный следующий номер сохраненного пуска
	uint32	MarkerStartStrPusk;		//маркер начала перечня информации о пусках

	uint32	NumPusk1;				//номер пуска сохраненного в первый банк
	uint32	AddrPusk1;				//Глобальный адрес пуска во flash
	uint32 	SizePusk1;				//размер пуска в байтах

	uint32	NumPusk2;				//номер пуска сохраненного в первый банк
	uint32	AddrPusk2;				//Глобальный адрес пуска во flash
	uint32 	SizePusk2;				//размер пуска в байтах

	uint32	NumPusk3;				//номер пуска сохраненного в первый банк
	uint32	AddrPusk3;				//Глобальный адрес пуска во flash
	uint32 	SizePusk3;				//размер пуска в байтах

	uint32	NumPusk4;				//номер пуска сохраненного в первый банк
	uint32	AddrPusk4;				//Глобальный адрес пуска во flash
	uint32 	SizePusk4;				//размер пуска в байтах

	uint32	NumPusk5;				//номер пуска сохраненного в первый банк
	uint32	AddrPusk5;				//Глобальный адрес пуска во flash
	uint32 	SizePusk5;				//размер пуска в байтах

	uint32	NumPusk6;				//номер пуска сохраненного в первый банк
	uint32	AddrPusk6;				//Глобальный адрес пуска во flash
	uint32 	SizePusk6;				//размер пуска в байтах

	uint32	NumPusk7;				//номер пуска сохраненного в первый банк
	uint32	AddrPusk7;				//Глобальный адрес пуска во flash
	uint32 	SizePusk7;				//размер пуска в байтах

	uint32	NumPusk8;				//номер пуска сохраненного в первый банк
	uint32	AddrPusk8;				//Глобальный адрес пуска во flash
	uint32 	SizePusk8;				//размер пуска в байтах

	uint32	NumPusk9;				//номер пуска сохраненного в первый банк
	uint32	AddrPusk9;				//Глобальный адрес пуска во flash
	uint32 	SizePusk9;				//размер пуска в байтах

	uint32	NumPusk10;				//номер пуска сохраненного в первый банк
	uint32	AddrPusk10;				//Глобальный адрес пуска во flash
	uint32 	SizePusk10;				//размер пуска в байтах

	uint32	NumPusk11;				//номер пуска сохраненного в первый банк
	uint32	AddrPusk11;				//Глобальный адрес пуска во flash
	uint32 	SizePusk11;				//размер пуска в байтах

	uint32	NumPusk12;				//номер пуска сохраненного в первый банк
	uint32	AddrPusk12;				//Глобальный адрес пуска во flash
	uint32 	SizePusk12;				//размер пуска в байтах

	uint32	NumPusk13;				//номер пуска сохраненного в первый банк
	uint32	AddrPusk13;				//Глобальный адрес пуска во flash
	uint32 	SizePusk13;				//размер пуска в байтах

	uint32	NumPusk14;				//номер пуска сохраненного в первый банк
	uint32	AddrPusk14;				//Глобальный адрес пуска во flash
	uint32 	SizePusk14;				//размер пуска в байтах

	uint32 XOR;						//контрольная сумма пакета
}FramPuskStruct;

typedef struct FlashDataCommand				//структура командного пакет для чтения флешки данных
{
	uint32 MarkerStart;	//маркер начала структуры командного пакета

	union Command	//параметры кадра в канале 2
	{
		uint32 sw;
		struct
		{
			uint32 ReadFlash	:1;		//Команда на чтение пуска из Flash Данных
			uint32 ReadInfo		:1;		//Запрос информации о пусках
			uint32 Format 		:1;		//Команда на форматирование FLASH Данных
			uint32 Delete		:1;		//Команда на очистку Flash от пусков
			uint32 ReadPaketDDR	:1;		//Считываем по Ethernet очередной пакет данных
			uint32 Res			:26;
			uint32 NumTarget	:1;		//Номер цели с которой работаем
		} s;
	} Command;

	uint32 NumPusk;
	uint32 OffsetPusk;
	uint32 SizePaket;
	uint32 Res1;
	uint32 Res2;
	uint32 Res3;
	uint32 Res4;
	uint32 Res5;
	uint32 Res6;
	uint32 Res7;
	uint32 Res8;
	uint32 Res9;
	uint32 MarkerStop;	//маркер конца структуры командного пакета
	uint32 Xor;
} FlashDataCommand;



typedef struct DataFileTitle				//структура заголовка файла телеметрии
{
	uint32 MarkerStart;	//маркер начала структуры заголовка
	uint16 NumDev;							//номер прибора
	uint16 NumCycle;						//номер текущего цикла боевой работы
	uint16 NumPaket;						//количество пакетов в файле
	uint16 Res1;							//резервное поле
	uint32 SizeStructTitle;					//размер структуры заголовка в байтах
	uint32* Adrr;							//адрес массива указателей на смещения пакетов
} DataFileTitle;

typedef struct DataFileFrame
{
	uint32 MarkerStart;	//маркер начала структуры пакета
	uint32 OffsetNextFrame;					//Смещение следующего пакета от начала структуры заголовка

	uint16 NumPaketOEDTAT1;					//номер пакета ОЭД-TA для цели 1
	uint16 NumPaketTAOEDT1;					//номер пакета TA-ОЭД для цели 1
	uint16 NumPaketOEDTAT2;					//номер пакета ОЭД-TA для цели 2
	uint16 NumPaketTAOEDT2;					//номер пакета TA-ОЭД для цели 2
	uint16 NumCycle;						//номер текущего цикла боевой работы
	uint16 SizePaketOEDTA;					//размер пакета ОЭД-TA
	uint16 SizePaketTAOED;					//размер пакета TA-OED
	uint16 CoordX1;							//координата поля анализа канала 1 по q
	uint16 CoordY1;							//координата поля анализа канала 1 по fi
	uint16 CoordX2;							//координата поля анализа канала 2 по q
	uint16 CoordY2;							//координата поля анализа канала 2 по fi
	uint16 Reserv1;

	union KadrCh1	//параметры кадра в канале 1
	{
		uint32 sw;
		struct
		{
			uint32 SizeY1		:10;	//Размер кадра по q
			uint32 SizeX1		:10;	//Размер кадра по fi
			uint32 SUMM 		:12;	//Признак наличия суммирования
		} s;
	} KadrCh1;

	union KadrCh2	//параметры кадра в канале 2
	{
		uint32 sw;
		struct
		{
			uint32 SizeY2		:10;	//Размер кадра по q
			uint32 SizeX2		:10;	//Размер кадра по fi
			uint32 SUMM 		:12;		//Признак наличия суммирования
		} s;
	} KadrCh2;

}DataFileFrame;

uint8 ReadIdDFlash(void);
void formatflash(uint32 metka);
void ReadFramParamData(FramPuskStruct * fps,uint16 Target);
void WriteFramParamData(FramPuskStruct * fps,uint32 SizePusk,uint16 Target);
uint32 XorChekSUMCalc32(uint32 *Buff, uint32 Size);
uint16 MyMemCPY(void *_s1, const void *_s2, uint32 size);
void FillDataTitle1(DataFileTitle*,uint16 Paket,uint32 SizeFrame, uint16 Target);	//функция заполнения структуры заголовка файла телеметрии
uint32 FillDataFrame1(DataFileFrame*,uint16 Frame,uint16 Param,uint16 Target);	//функция заполнения кадра с данными
#endif /* FLASHDATA_H_ */
