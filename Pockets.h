
#ifndef _POCKETS_H_
#define _POCKETS_H_

//"Pockets.h"
#include "atypes.h"

#define TABLCOUNT 2300
#define SIZEOFTABL (((TABLCOUNT)*4)+12)
#define SIZEOFTABL32 ((SIZEOFTABL)>>2)
#define TOTAL_FLASH_SIZE_DIAGN 1048576	// размер флэшки на диагностику 1 мб
#define POCKETPARAMSIZE 20	// размер параметров truncate - структуры до структуры оэдтоцвс

extern Uint32 nPocket;
typedef struct POCKETBEGIN
{
	Uint8 typeOfStruct;			// тип структуры
	Uint8 rez1;					// резерв
	Uint16 N;				// количество пакетов
	Uint32 SizeStruct;	// размер всех данных
	Uint32 next[TABLCOUNT];				// смящение
	Uint32 KC;				// Контрольная сумма
} POCKETBEGIN ,*PPOCKETBEGIN;

typedef struct POINTINT 
{  
	short int   x;				// координата X
	short int   y;				// координата Y
} POINTINT;

typedef struct OEDTOCVS 
{
	short int CL1;		// сигналы
	short int CL2;		// датчик канала 1 по координате q
	short int CL3;		// датчик канала 1 по координате fi
	short int CL4;		// координатор канала 1 по координате q
	short int CL5;		// координатор канала 1 по координате fi
	short int CL6;		// датчик канала 2 по координате q
	short int CL7;		// датчик канала 2 по координате fi
	short int CL8;		// координатор канала 2 по координате q
	short int CL9;		// координатор канала 2 по координате fi
	short int CL10;		// дальность до изделия
	Uint8 CL11a;					// угловой размер источника в канале 1 по координате  q
	Uint8 CL11b;					// угловой размер источника в канале 1 по координате  fi
	Uint8 CL12a;					// угловой размер источника в канале 2 по координате  q
	Uint8 CL12b;					// угловой размер источника в канале 2 по координате  fi
	short int CL13;		// уровень сигнала в канале 1
	short int CL14;		// уровень сигнала в канале 2
	short int CL15;		// уровень сигнала в дальномерном канале
	short int CL16;		// размер поля анализа в канале 1
	short int CL17;		// размер поля анализа в канале 2
	short int CL18;		// отношение сигнал/порог в канале 1
	short int CL19;		// отношение сигнал/порог в канале 2
	short int CL20;		// отношение сигнал/порог в дальномерном канале
	short int CL21;		// фокус ПС
	short int CL22;		// площадь диафрагмы канала1
	short int CL23;		// время накопления в канале 1
	short int CL24;		// текущая температура ОЭД
	
}OEDTOCVS ;

typedef struct CVSTOOED 
{
	unsigned short int CL1;			// сигналы
	short int CL2;					// установка в канале 1 по координате q
	short int CL3;					// установка в канале 1 по координате fi
	short int CL4;					// установка в канале 2 по координате q
	short int CL5;					// установка в канале 2 по координате fi
	short int CL6;					// скорость ПОД по координате q
	short int CL7;					// скорость ПОД по координате fi
	unsigned short int CL8;			// программная дальность
	signed short int CL9;			// ГДУ по координате q
	signed short int CL10;			// ГДУ по координате fi
	unsigned short int CL11;		// технологическая команда
	unsigned short int CL12;		// технологическая команда
	short int CL13;					// смещение АОП по координате q
	short int CL14;					// смещение АОП по координате fi
}CVSTOOED;

typedef struct TEXTDATA 
{
	OEDTOCVS oedToCvs;				// Информация от ОЭД к ЦВС
	CVSTOOED  cvsToOed;				// Информация от ЦВСк ОЭД 
}TEXTDATA;

// Структура для Владимира
typedef struct POCKETINFOTRUNCATED
{
//	Uint8 o2cSize;
//	Uint8 c2oSize;
	POINTINT XY[2];				// Размеры изображенияя
	Uint8 dx1;					// см структуру данных
	Uint8 dy1;
	Uint8 dx2;
	Uint8 dy2;
	
	Uint16 x1;
	Uint16 y1;
	Uint16 x2;
	Uint16 y2;
	
//	OEDTOCVS OverheadInformation;
	OEDTOCVS oedToCvs;
	CVSTOOED  cvsToOed;
}POCKETINFOTRUNCATED,*PPOCKETINFOTRUNCATED;

typedef struct POCKETINFO
{
	Uint32 BeginningOfData;		// Признак начала пакета
	Uint32 NextPocket;			// Смящение на следующий пакет
//	Uint32 Counter;				// Номер пакета
	Uint16 Counter;				// Номер пакета
	Uint16 rez2;
	POCKETINFOTRUNCATED PocketTruncatedInfo;
/*	POINTINT XY[2];				// Размеры изображенияя
	TEXTDATA textData;			// Текстовая информация
	Uint8* AdditionData;		// Дополнительная отладочная информация
*/	
//	Uint8* graphicInfo;			// Графические данные
	Uint32 EndOfData;				// Признак конца пакета
	Uint32 KC;				// Контрольная сумма
} POCKETINFO, *PPOCKETINFO;

#endif /* _POCKETS_H_ */
