#include "type.h"

#ifndef PAKETOEDTAETH_H_
#define PAKETOEDTAETH_H_

#define NUMBEROED		1		//номер текущего прибора
#define DISTANSEDELAY	26.5	//минимальная задержки вспышки ЛО при 0 дальности

#define TRUE   1
#define FALSE  0

//Фокуса каналов 1 и 2
#define FOKUSK1	50.8
#define FOKUSK2	331.7
/////////////////////////////////////////////////////
#define VerPO  4	//версия программного обеспечения
/////////////////////////////////////////////////////
/////////////////////////////////////////////////////
//Значения визирных осей канала 1 и 2
#define VizirChan1X			1024
#define VizirChan1Y			544

#define VizirChan2X			1024
#define VizirChan2Y			544
//Цена пиксела матрицы в микронах
#define PIXEL				5.5
#define PI					3.14159265

/////////////////////////////////////////////////////
//////////////////////////Глобальные настройки характеристик матриц//////////////////////
#define GAIN		40
#define FOTReg73	10
#define OffsetChan1	16332
#define OffsetChan2	16330


#define REG32(addr) (*(volatile unsigned int*)(addr))
#define REG16(addr) (*(volatile unsigned short*)(addr))
#define REG8(addr)  (*(volatile unsigned char*)(addr))

#define SizeBuffOED 		70					//размер пакета ОЭД в байтах без CRC
#define SizeBuffTA 			36					//размер пакета ТА в байтах без CRC
#define SizeKadrTechRejX	480					//количество пикселей в строке
#define SizeKadrTechRejY	480					//количество строк


//форматы протоколов
typedef struct OEDTOTA {
	union SignalOED1{
			uint16 sw;
			struct {
				uint16 Ready 		:1;	//Готов
				uint16 Healthy 		:1;	//Исправен
				uint16 IndLO1 		:1; //Индикатор ЛО в канале 1
				uint16 IndLO2 		:1;	//Индикатор ЛО в канале 2
				uint16 Light 		:1; //Засветка
				uint16 TNorm 		:1; //Температура в норме
				uint16 StrobK 		:1; //Наличие строба
				uint16 UZK2 		:1; //Переключение канала 2 на узкое поле зрения
				uint16 res01 		:1;
				uint16 res02 		:1;
				uint16 res03 		:1;
				uint16 res04 		:1;
				uint16 res05 		:1;
				uint16 res06 		:1;
				uint16 NumTarget	:1; //Номер изделия
				uint16 SavePusk 	:1; //Пуск будет сохранен
			} s;
		} SignalOED1;

	uint16 reserv1;		//Предположительно для отработки состояний тех. режима

	int16 qd1;			// датчик канала 1 по координате q
	int16 fd1;			// датчик канала 1 по координате fi
	int16 qk1;			// координатор канала 1 по координате q
	int16 fk1;			// координатор канала 1 по координате fi

	int16 qd2;			// датчик канала 2 по координате q
	int16 fd2;			// датчик канала 2 по координате fi
	int16 qk2;			// координатор канала 2 по координате q
	int16 fk2;			// координатор канала 2 по координате fi

	uint8 Sizeq11;		// угловой размер источника в канале 1 по координате  q
	uint8 Sizef11;		// угловой размер источника в канале 1 по координате  fi
	uint8 Sizeq12;		// угловой размер источника в канале 2 по координате  q
	uint8 Sizef12;		// угловой размер источника в канале 2 по координате  fi

	uint16 US1;			// уровень сигнала в канале 1
	uint16 US2;			// уровень сигнала в канале 2

	uint16 A1;			// размер поля анализа в канале 1
	uint16 A2;			// размер поля анализа в канале 2

	uint16 Ntime;		//Время наработки в часах
	uint16 Ncycle;		//Количество отработанных циклов

	uint16 FokusK1;		//Фокус канала 1
	uint16 FokusK2;		//Фокус канала 2

	uint16 STK1;		//Задержка старта времени накопления в канале 1 относительно сигнала STROBK1
	uint16 STK2;		//Задержка старта времени накопления в канале 2 относительно сигнала STROBK1

	uint16 Ksp1;		// отношение сигнал/порог в канале 1
	uint16 Ksp2;		// отношение сигнал/порог в канале 2

	uint16 OEDVerPO;	// Версия программного обеспечения

	uint16 reserv2;
	uint16 reserv3;
	uint16 reserv4;
	uint16 reserv5;
	uint16 reserv6;

	uint16 NumPaket;	//Номер пакета
	int16 Temper;		//текущая температура ОЭД

	uint16 Tstrob;		//Период строба
	uint16 DeltaTstrob;	//Задержка между стробами

	uint16 reserv7;

} OEDTOTA;

typedef struct TATOOED {

	//командное слово управление ОЭД
	union CommandTA1{
		uint16 sw;
		struct {
			uint16 Pusk 		:1;	//Пуск ракеты
			uint16 Sxod 		:1; //Сход ракеты
			uint16 SoprLO1	 	:1;	//Разрешение сопровождения в канале 1
			uint16 SoprLO2 		:1; //Разрешение сопровождения в канале 2
			uint16 ResetK1 		:1; //Сброс канала 1
			uint16 Ground 		:1; //Работа по наземной цели
			uint16 res01 		:1;
			uint16 res02 		:1;
			uint16 res03 		:1;
			uint16 res04 		:1;
			uint16 res05 		:1;
			uint16 res06 		:1;
			uint16 res07 		:1;
			uint16 res08 		:1;
			uint16 NumTarget	:1; //Номер изделия
			uint16 SavePusk 	:1; //Разрешение сохранения пуска
		} s;
	} CommandTA1;

	//командное слово для тестового режима
	union CommandTA2{
		uint16 sw;
		struct {
			uint16 TechMode	:4;	//Код тех. режима
			uint16 TimeExp	:5; //Код времени накопления в тех режиме
			uint16 res1 	:1;
			uint16 res2 	:1;
			uint16 res3 	:1;
			uint16 res4 	:1;
			uint16 res5 	:1;
			uint16 res6 	:1;
			uint16 KolUp 	:1;//включить коллиматор в боевом режиме
		} s;
	} CommandTA2;

	int32 qk1;					// установка в канале 1 по координате q
	int32 fk1;					// установка в канале 1 по координате fi
	int16 qk2;					// установка в канале 2 по координате q
	int16 fk2;					// установка в канале 2 по координате fi
	int32 qPod;					// скорость ПОД по координате q
	int32 fPod;					// скорость ПОД по координате fi
	uint16 Distance;			// программная дальность

	int16 qGDU;					// ГДУ по координате q
	int16 fGDU;					// ГДУ по координате fi

	uint16 res1;
	uint16 res2;
	uint16 res3;
	uint16 res4;
	uint16 res5;

	uint16 NumPaket;			// Номер пакета
	uint16 res6;

} TATOOED;

typedef struct VideoTechParam
{
	union Word1{
				uint32 sw;
				struct {
					uint32 KBP 			:5;	//Количество бит на пиксель изображения
					uint32 res1 		:27;
				} s;
			} Word1;

	union Word2{
				uint32 sw;
				struct {
					uint32 TechRej 		:4;	//Подтверждение технологического режима
					uint32 TimeExp 		:5;	//Время накопления
					uint32 res1 		:23;
				} s;
			} Word2;

	uint32 NPK;			//номер передаваемого кадра
	uint16 RKB;			//размер кадра по вертикали
	uint16 RKG;			//размер кадра по горизонту
	int16 kq2;			//координата нижнего левого угла в секундах по азимуту
	int16 kf2;			//координата нижнего левого угла в секундах по углу места
	uint16 dq2;			//цена пикселя в углах по азимуту
	uint16 df2;			//цена пикселя в углах по углу места
	uint16 res1;
	uint16 res2;
	uint16 res3;
	uint16 res4;

} VideoTechParam;

typedef struct VideoTechPacket
{
	uint16 res1;
	uint16 IMD;			//Идентификатор массива видеоданных
	uint32 NPK;			//номер передаваемого кадра
	uint16 KSS;			//Количество строк в текущем сообщении
	uint16 NPS;			//номер первой строки в текущем пакете
	uint32 res2;
} VideoTechPacket;

//Протокол обмена с визуализатором////////////////////////////////////////////////
typedef struct VisualKommandPaket
{
		union Kommand{
					uint32 sw;
					struct {
						uint32 VP 			:1;	//Команда на повтор пакета видео
						uint32 SM 			:1;	//Команда на суммирование x2 изображения
						uint32 ImB 			:1;	//Команда на имитацию боевого режима по коллиматору без синха.
						uint32 ImBS1 		:1;	//Команда на имитацию боевого режима по синх1.
						uint32 ImBS2 		:1;	//Команда на имитацию боевого режима по синх2.
						uint32 res1 		:26;
						uint32 RS	 		:1;	//Команда на рестарт 2 ядра
					} s;
				} KommandWord;

		union Control{
					uint32 sw;
					struct {
						uint32 S1	 		:1;	//Включение синхронизации относительно Синх1
						uint32 S2	 		:1;	//Включение синхронизации относительно Синх2
						uint32 KN	 		:1;	//Включение коллиматора в непрерывном режиме
						uint32 KI	 		:1;	//Включение коллиматора в импульсном режиме
						uint32 B	 		:1;	//Включение передачи 16-битного изображения
						uint32 KO	 		:4;	//Уменьшение скорости передачи
						uint32 Imp			:1;	//Запрос на передачу кадра с импульсом по имитации по синхросигналам
						uint32 res1 		:21;
						uint32 RS	 		:1;	//Команда на рестарт 2 ядра
					} s;
				} ControlWord;

		union DataWord1{
					uint32 sw;
					struct {
							uint32 SizeX1 		:12;//Размер X поля анализа в канале 1
							uint32 SizeY1 		:12;//Размер Y поля анализа в канале 1
							uint32 StartExp1	:8;	//Смещение начала времени накопления в мкс
						} s;
					} DataWord1;

		union DataWord2{
					uint32 sw;
					struct {
							uint32 SizeX2 		:12;//Размер X поля анализа в канале 2
							uint32 SizeY2 		:12;//Размер Y поля анализа в канале 2
							uint32 StartExp2	:8;	//Смещение начала времени накопления в мкс
						} s;
					} DataWord2;

		union DataWord3{
					uint32 sw;
					struct {
							uint32 CoordX1 		:11;//Координата X начала поля анализа в канале 1
							uint32 CoordY1 		:11;//Координата Y начала поля анализа в канале 1
							uint32 Exp1			:10;//Размер времени накопления в мкс
						} s;
					} DataWord3;

		union DataWord4{
					uint32 sw;
					struct {
							uint32 CoordX2 		:11;//Координата X начала поля анализа в канале 2
							uint32 CoordY2 		:11;//Координата Y начала поля анализа в канале 2
							uint32 Exp2			:10;//Размер времени накопления в мкс
						} s;
					} DataWord4;

		union CMOS1{
					uint32 sw;
					struct {
							uint32 Reg73 		:8;//Значение регистра 73
							uint32 ADC_GAIN		:8;//Коэффициент усиления
							uint32 Offset		:14;//Смещение по уровню черного
							uint32 res	 		:2;
							} s;
						} CMOS1;
		uint32 res1;
		uint32 res2;

		union CMOS2{
					uint32 sw;
					struct {
							uint32 Reg73 		:8;//Значение регистра 73
							uint32 ADC_GAIN		:8;//Коэффициент усиления
							uint32 Offset		:14;//Смещение по уровню черного
							uint32 res	 		:2;
							} s;
						} CMOS2;

		uint32 res3;
		uint32 res4;

		union DataWord5{
					uint32 sw;
					struct {
							uint32 NumPacket	:16;//Номер пакета
							uint32 Distance		:16;//Программная дальность
						} s;
					} DataWord5;

		union MathKommand{
					uint32 sw;
					struct {
							uint32 Kommand		:16;//Команды управления математикой
							uint32 K_s_sh		:16;//Коэффициент сигнал/шум
						} s;
					} MathKommand;

		uint32 res7;
		uint32 res8;
		uint32 res9;
		uint32 res10;
		uint32 res11;
		uint32 res12;
		uint32 res13;
		uint32 res14;
		uint32 res15;
		uint32 res16;
		uint32 res17;
		uint32 res18;
		uint32 res19;

} VisualKommandPaket;

typedef struct VisualStatusPaket11
{
//цель 1, канал 1, кадр фоновый - 1
uint16 TimeJob111;				//время обработки кадра по цели 1 в канале 1 кадр 1
uint16 NPointPorg111;			//количество точек выше порога по цели 1 в канале 1 кадр 1
uint16 SKO111;					//CKO по цели 1 в канале 1 кадр 1
uint16 NOBJ111;					//количество объектов по цели 1 в канале 1 кадр 1
int16 CordX111;					//координата цели по X цели 1 в канале 1 кадр 1
int16 CordY111;					//координата цели по Y цели 1 в канале 1 кадр 1
uint16 SUFon111;				//средний уровень фона цели 1 в канале 1 кадр 1
uint16 LocUFon111;				//локальный уровень фона цели 1 в канале 1 кадр 1
uint16 LocSKO111;				//CKO локальной окрестности по цели 1 в канале 1 кадр 1
uint16 res111_1;
uint16 res111_2;
uint16 res111_3;
uint16 res111_4;
uint16 res111_5;
} VisualStatusPaket11;


typedef struct VisualStatusPaketProcPar
{
//цель 1, канал 1, кадр фоновый - 1
uint16 TimeJob;				//время обработки кадра по цели 1 в канале 1 кадр 1
uint16 NPointPorg;			//количество точек выше порога по цели 1 в канале 1 кадр 1
uint16 SKO;					//CKO по цели 1 в канале 1 кадр 1
uint16 NOBJ;					//количество объектов по цели 1 в канале 1 кадр 1
int16 CordX;					//координата цели по X цели 1 в канале 1 кадр 1
int16 CordY;					//координата цели по Y цели 1 в канале 1 кадр 1
uint16 SUFon;				//средний уровень фона цели 1 в канале 1 кадр 1
uint16 LocUFon;				//локальный уровень фона цели 1 в канале 1 кадр 1
uint16 LocSKO;				//CKO локальной окрестности по цели 1 в канале 1 кадр 1
uint16 res_1;
uint16 res_2;
uint16 res_3;
uint16 res_4;
uint16 res_5;
} VisualStatusPaketProcPar;

typedef struct VisualStatusPaket112
{
//цель 1, канал 1, кадр импульсный - 2
uint16 TimeJob112;				//время обработки кадра по цели 1 в канале 1 кадр 2
uint16 NPointPorg112;			//количество точек выше порога по цели 1 в канале 1 кадр 2
uint16 SKO112;					//CKO по цели 1 в канале 1 кадр 2
uint16 NOBJ112;					//количество объектов по цели 1 в канале 1 кадр 2
int16 CordX112;					//координата цели по X цели 1 в канале 1 кадр 2
int16 CordY112;					//координата цели по Y цели 1 в канале 1 кадр 2
uint16 SUFon112;				//средний уровень фона цели 1 в канале 1 кадр 2
uint16 LocUFon112;				//локальный уровень фона цели 1 в канале 1 кадр 2
uint16 LocSKO112;				//CKO локальной окрестности по цели 1 в канале 1 кадр 2
uint16 res112_1;
uint16 res112_2;
uint16 res112_3;
uint16 res112_4;
uint16 res112_5;
} VisualStatusPaket112;


typedef struct VisualStatusPaket
{
	uint32 NumKadr;		//Номер текущего кадра

	union ControlWord1{
			uint32 sw;
			struct {
					uint32 B	 		:1;//Битность изображения
					uint32 STROB1 		:1;//Наличие синхронизации СТРОБ1
					uint32 STROB2 		:1;//Наличие синхронизации СТРОБ2
					uint32 WithLO		:1;//Наличие ответчика в кадре
					uint32 Res			:28;
				} s;
			} ControlWord1;

	union Data1{
			uint32 sw;
			struct {
					uint32 CoordX1 		:11;//Координата X начала поля анализа в канале 1
					uint32 CoordY1 		:11;//Координата Y начала поля анализа в канале 1
					uint32 Exp1			:10;//Время накопления в мкс
				} s;
			} Data1;

	union Data2{
			uint32 sw;
			struct {
					uint32 CoordX2 		:11;//Координата X начала поля анализа в канале 2
					uint32 CoordY2 		:11;//Координата Y начала поля анализа в канале 2
					uint32 Exp2	 		:10;//Время накопления в мкс
				} s;
			} Data2;

	union Data3{
			uint32 sw;
			struct {
					uint32 SizeX1 		:12;//Размер X поля анализа в канале 1
					uint32 SizeY1 		:12;//Размер Y поля анализа в канале 1
					int32 Temper		:8;//Температура прибора
				} s;
			} Data3;

	union Data4{
			uint32 sw;
			struct {
					uint32 SizeX2 		:12;//Размер X поля анализа в канале 2
					uint32 SizeY2 		:12;//Размер Y поля анализа в канале 2
					uint32 res			:8;
				} s;
			} Data4;

	union Data5{
			uint32 sw;
			struct {
					uint32 Fokus1		:16;//Фокус канала 1
					uint32 Fokus2		:16;//Фокус канала 2
				} s;
			} Data5;

	union StatVPUXilinx{
			uint32 sw;
			struct {
					uint32 StatusK1		:16;//Статус srio канала 1
					uint32 StatusK2		:16;//Статус srio канала 2
				} s;
			} StatVPUXilinx;

	uint32 TimeShtampGlobalX;				//время создания прошивки Xilinx VPU

	uint32 Tvpu;							//температурные датчики vpu

	union Data6{
			uint32 sw;
			struct {
					uint32 Distance		:16;//Програмная дальность
					uint32 NumPribor	:16;//Номер прибора
				} s;
			} Data6;

	union StatPVPK1_1{
			uint32 sw;
			struct {
					uint8 Status0		:8;//Параметры статуса
					uint8 Status1		:8;//Параметры статуса
					uint8 Status2		:8;//Параметры статуса
					uint8 Status3		:8;//Параметры статуса
				} s;
			} StatPVPK1_1;

	union StatPVPK1_2{
			uint32 sw;
			struct {
					uint8 Status4		:8;//Параметры статуса
					uint8 Status5		:8;//Параметры статуса
					uint8 Status6		:8;//Параметры статуса
					uint8 Status7		:8;//Параметры статуса
				} s;
			} StatPVPK1_2;

	union StatPVPK2_1{
			uint32 sw;
			struct {
					uint8 Status0		:8;//Параметры статуса
					uint8 Status1		:8;//Параметры статуса
					uint8 Status2		:8;//Параметры статуса
					uint8 Status3		:8;//Параметры статуса
				} s;
			} StatPVPK2_1;

	union StatPVPK2_2{
			uint32 sw;
			struct {
					uint8 Status4		:8;//Параметры статуса
					uint8 Status5		:8;//Параметры статуса
					uint8 Status6		:8;//Параметры статуса
					uint8 Status7		:8;//Параметры статуса
				} s;
			} StatPVPK2_2;

	uint32 Tvpu2;							//температурные датчики vpu 2

	union STROBPERIOD{
			uint32 sw;
			struct {
					uint32 STR1PERIOD	:16;//Период следования импульса STROB1
					uint32 STR2PERIOD	:16;//Период следования импульса STROB2
				} s;
			} STRPERIOD;
	union STROBDELAY{
			uint32 sw;
			struct {
					uint32 DelayStr1Str2	:16;//Задержка между STROB1 и STROB2
					uint32 SigImitBattleJob	:16;//Сигналы отработки иммитации боевого режима
				} s;
			} STRDELAY;

	//цель 1, канал 1, кадр фоновый - 1
	uint16 TimeJob111;				//время обработки кадра по цели 1 в канале 1 кадр 1
	uint16 NPointPorg111;			//количество точек выше порога по цели 1 в канале 1 кадр 1
	uint16 SKO111;					//CKO по цели 1 в канале 1 кадр 1
	uint16 NOBJ111;					//количество объектов по цели 1 в канале 1 кадр 1
	int16 CordX111;					//координата цели по X цели 1 в канале 1 кадр 1
	int16 CordY111;					//координата цели по Y цели 1 в канале 1 кадр 1
	uint16 SUFon111;				//средний уровень фона цели 1 в канале 1 кадр 1
	uint16 LocUFon111;				//локальный уровень фона цели 1 в канале 1 кадр 1
	uint16 LocSKO111;				//CKO локальной окрестности по цели 1 в канале 1 кадр 1
	uint16 res111_1;
	uint16 res111_2;
	uint16 res111_3;
	uint16 res111_4;
	uint16 res111_5;

	//цель 1, канал 1, кадр импульсный - 2
	uint16 TimeJob112;				//время обработки кадра по цели 1 в канале 1 кадр 2
	uint16 NPointPorg112;			//количество точек выше порога по цели 1 в канале 1 кадр 2
	uint16 SKO112;					//CKO по цели 1 в канале 1 кадр 2
	uint16 NOBJ112;					//количество объектов по цели 1 в канале 1 кадр 2
	int16 CordX112;					//координата цели по X цели 1 в канале 1 кадр 2
	int16 CordY112;					//координата цели по Y цели 1 в канале 1 кадр 2
	uint16 SUFon112;				//средний уровень фона цели 1 в канале 1 кадр 2
	uint16 LocUFon112;				//локальный уровень фона цели 1 в канале 1 кадр 2
	uint16 LocSKO112;				//CKO локальной окрестности по цели 1 в канале 1 кадр 2
	uint16 res112_1;
	uint16 res112_2;
	uint16 res112_3;
	uint16 res112_4;
	uint16 res112_5;

	//цель 1, канал 2, кадр фоновый - 1
	uint16 TimeJob121;				//время обработки кадра по цели 1 в канале 2 кадр 1
	uint16 NPointPorg121;			//количество точек выше порога по цели 1 в канале 2 кадр 1
	uint16 SKO121;					//CKO по цели 1 в канале 2 кадр 1
	uint16 NOBJ121;					//количество объектов по цели 1 в канале 2 кадр 1
	int16 CordX121;					//координата цели по X цели 1 в канале 2 кадр 1
	int16 CordY121;					//координата цели по Y цели 1 в канале 2 кадр 1
	uint16 SUFon121;				//средний уровень фона цели 1 в канале 2 кадр 1
	uint16 LocUFon121;				//локальный уровень фона цели 1 в канале 2 кадр 1
	uint16 LocSKO121;				//CKO локальной окрестности по цели 1 в канале 2 кадр 1
	uint16 res121_1;
	uint16 res121_2;
	uint16 res121_3;
	uint16 res121_4;
	uint16 res121_5;

	//цель 1, канал 2, кадр фоновый - 2
	uint16 TimeJob122;				//время обработки кадра по цели 1 в канале 2 кадр 2
	uint16 NPointPorg122;			//количество точек выше порога по цели 1 в канале 2 кадр 2
	uint16 SKO122;					//CKO по цели 1 в канале 2 кадр 2
	uint16 NOBJ122;					//количество объектов по цели 1 в канале 2 кадр 2
	int16 CordX122;					//координата цели по X цели 1 в канале 2 кадр 2
	int16 CordY122;					//координата цели по Y цели 1 в канале 2 кадр 2
	uint16 SUFon122;				//средний уровень фона цели 1 в канале 2 кадр 2
	uint16 LocUFon122;				//локальный уровень фона цели 1 в канале 2 кадр 2
	uint16 LocSKO122;				//CKO локальной окрестности по цели 1 в канале 2 кадр 2
	uint16 res122_1;
	uint16 res122_2;
	uint16 res122_3;
	uint16 res122_4;
	uint16 res122_5;

	//цель 2, канал 1, кадр фоновый - 1
	uint16 TimeJob211;				//время обработки кадра по цели 2 в канале 1 кадр 1
	uint16 NPointPorg211;			//количество точек выше порога по цели 2 в канале 1 кадр 1
	uint16 SKO211;					//CKO по цели 2 в канале 1 кадр 1
	uint16 NOBJ211;					//количество объектов по цели 2 в канале 1 кадр 1
	int16 CordX211;					//координата цели по X цели 2 в канале 1 кадр 1
	int16 CordY211;					//координата цели по Y цели 2 в канале 1 кадр 1
	uint16 SUFon211;				//средний уровень фона цели 2 в канале 1 кадр 1
	uint16 LocUFon211;				//локальный уровень фона цели 2 в канале 1 кадр 1
	uint16 LocSKO211;				//CKO локальной окрестности по цели 2 в канале 1 кадр 1
	uint16 res211_1;
	uint16 res211_2;
	uint16 res211_3;
	uint16 res211_4;
	uint16 res211_5;

	//цель 2, канал 1, кадр импульсный - 2
	uint16 TimeJob212;				//время обработки кадра по цели 2 в канале 1 кадр 2
	uint16 NPointPorg212;			//количество точек выше порога по цели 2 в канале 1 кадр 2
	uint16 SKO212;					//CKO по цели 2 в канале 1 кадр 2
	uint16 NOBJ212;					//количество объектов по цели 2 в канале 1 кадр 2
	int16 CordX212;					//координата цели по X цели 2 в канале 1 кадр 2
	int16 CordY212;					//координата цели по Y цели 2 в канале 1 кадр 2
	uint16 SUFon212;				//средний уровень фона цели 2 в канале 1 кадр 2
	uint16 LocUFon212;				//локальный уровень фона цели 2 в канале 1 кадр 2
	uint16 LocSKO212;				//CKO локальной окрестности по цели 2 в канале 1 кадр 2
	uint16 res212_1;
	uint16 res212_2;
	uint16 res212_3;
	uint16 res212_4;
	uint16 res212_5;

	//цель 2, канал 2, кадр фоновый - 1
	uint16 TimeJob221;				//время обработки кадра по цели 2 в канале 2 кадр 1
	uint16 NPointPorg221;			//количество точек выше порога по цели 2 в канале 2 кадр 1
	uint16 SKO221;					//CKO по цели 2 в канале 2 кадр 1
	uint16 NOBJ221;					//количество объектов по цели 2 в канале 2 кадр 1
	int16 CordX221;					//координата цели по X цели 2 в канале 2 кадр 1
	int16 CordY221;					//координата цели по Y цели 2 в канале 2 кадр 1
	uint16 SUFon221;				//средний уровень фона цели 2 в канале 2 кадр 1
	uint16 LocUFon221;				//локальный уровень фона цели 2 в канале 2 кадр 1
	uint16 LocSKO221;				//CKO локальной окрестности по цели 2 в канале 2 кадр 1
	uint16 res221_1;
	uint16 res221_2;
	uint16 res221_3;
	uint16 res221_4;
	uint16 res221_5;

	//цель 2, канал 2, кадр фоновый - 2
	uint16 TimeJob222;				//время обработки кадра по цели 2 в канале 2 кадр 2
	uint16 NPointPorg222;			//количество точек выше порога по цели 2 в канале 2 кадр 2
	uint16 SKO222;					//CKO по цели 2 в канале 2 кадр 2
	uint16 NOBJ222;					//количество объектов по цели 2 в канале 2 кадр 2
	int16 CordX222;					//координата цели по X цели 2 в канале 2 кадр 2
	int16 CordY222;					//координата цели по Y цели 2 в канале 2 кадр 2
	uint16 SUFon222;				//средний уровень фона цели 2 в канале 2 кадр 2
	uint16 LocUFon222;				//локальный уровень фона цели 2 в канале 2 кадр 2
	uint16 LocSKO222;				//CKO локальной окрестности по цели 2 в канале 2 кадр 2
	uint16 res222_1;
	uint16 res222_2;
	uint16 res222_3;
	uint16 res222_4;
	uint16 res222_5;

} VisualStatusPaket;

typedef struct VisualKadrPaket
{
	uint32 NumKadr;				//Номер текущего кадра
	uint32 OffsetInPacket;		//Смещение в кадре кадра

	union Data{
			uint32 sw;
			struct {
					uint32 SizePacket 	:11;//Размер пакета
					uint32 NumPacket	:21;//Номер пакета
				} s;
			} Data;
} VisualKadrPaket;
////////////////////////////////////////////////////////////////////////////////
//timeout
void CycleDelay (Int32 count);

//прототипы функций
void InitPaketBattleJob();
void InitPaketTech();
void assembleBuffOED(uint16* Buff,int target,int size);
int disassembleBuffTA(uint16* Buff,int target,int size);
void AssembleBuffersTechVideo(uint8* Buff,int size);
void spi_cmos_reset_enable();

//поток обработки UDP
int dtask_udp_visual(SOCKET s, UINT32 unused);
int dtask_udp_target1(SOCKET s, UINT32 unused);
int dtask_udp_target2(SOCKET s, UINT32 unused);
int dtask_udp_flash( SOCKET s, UINT32 unused );
int dtask_d_flash( SOCKET s, UINT32 unused );



uint16 ChecksumCalck(uint16* Buff,int size);	//подсчет контрольной суммы

void CreateVideo(int sizeX1,int sizeY1,int sizeX2,int sizeY2,int type);

void CreateVideoChanal2(int sizeX2,int sizeY2);	//копируем видео 2 канала для тех режима


#endif /* PAKETOEDTAETH_H_ */

