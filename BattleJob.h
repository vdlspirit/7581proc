#include "type.h"

#ifndef BATTLEJOB_H_
#define BATTLEJOB_H_

#define MCSRAMCASH			0x0C000000
#define MCSRAMMYVIDEO		0x0C100000

typedef struct BattleStructMathT1K1
{
	uint16	Status;					//состояние ядер по текущей цели
	//Координаты центра полей анализа Цель 1 каналы 1 и 2 от процессоров
	int16 Target1CoordAngX1;
	int16 Target1CoordAngY1;

	//Предыдущие координаты центра полей анализа Цель 1 каналы 1 и 2 от процессоров
	int16 PTarget1CoordAngX1;
	int16 PTarget1CoordAngY1;

	//Координаты центра источника в пикселах Цель 1 каналы 1 и 2 от процессора 1/10 пиксела
	int16 Target1CoordX1;
	int16 Target1CoordY1;

	//Параметры передаваемые от OED на ТА
	uint8 IndLO1;

	int16 qd11;			// датчик канала 1 по координате q
	int16 fd11;			// датчик канала 1 по координате fi
	int16 qk11;			// координатор канала 1 по координате q
	int16 fk11;			// координатор канала 1 по координате fi

	uint8 Sizeq11;		// угловой размер источника в канале 1 по координате  q
	uint8 Sizef11;		// угловой размер источника в канале 1 по координате  fi

	uint16 US11;		// уровень сигнала в канале 1
	uint16 A11;			// размер поля анализа в канале 1

	uint16 Ksp11;		// отношение сигнал/порог в канале 1 ракета 1

	uint16 reserv1;
	uint16 reserv2;
	uint16 reserv3;
} BattleMathT1K1;

typedef struct BattleStructMathT1K2
{
	uint16	Status;					//состояние ядер по текущей цели
	//Координаты центра полей анализа Цель 1 каналы 1 и 2 от процессоров
	int16 Target1CoordAngX2;
	int16 Target1CoordAngY2;

	//Предыдущие координаты центра полей анализа Цель 1 каналы 1 и 2 от процессоров
	int16 PTarget1CoordAngX2;
	int16 PTarget1CoordAngY2;


	//Координаты центра источника в пикселах Цель 1 каналы 1 и 2 от процессора 1/10 пиксела
	int16 Target1CoordX2;
	int16 Target1CoordY2;

	//Параметры передаваемые от OED на ТА
	uint8 IndLO2;
	uint8 UZK2;

	int16 qd21;			// датчик канала 2 по координате q
	int16 fd21;			// датчик канала 2 по координате fi
	int16 qk21;			// координатор канала 2 по координате q
	int16 fk21;			// координатор канала 2 по координате fi

	uint8 Sizeq12;		// угловой размер источника в канале 2 по координате  q
	uint8 Sizef12;		// угловой размер источника в канале 2 по координате  fi

	uint16 US21;		// уровень сигнала в канале 2

	uint16 A21;			// размер поля анализа в канале 2

	uint16 Ksp21;		// отношение сигнал/порог в канале 2 ракета 1

	uint16 reserv4;
	uint16 reserv5;
	uint16 reserv6;

} BattleMathT1K2;

typedef struct BattleStructMathT2K1
{
	uint16	Status;					//состояние ядер по текущей цели

	//Координаты центра полей анализа Цель 1 каналы 1 и 2 от процессоров
	int16 Target2CoordAngX1;
	int16 Target2CoordAngY1;

	//Предыдущие координаты центра полей анализа Цель 2 каналы 1 и 2 от процессоров
	int16 PTarget2CoordAngX1;
	int16 PTarget2CoordAngY1;


	//Координаты центра источника в пикселах Цель 1 каналы 1 и 2 от процессора 1/10 пиксела
	int16 Target2CoordX1;
	int16 Target2CoordY1;

	//Параметры передаваемые от OED на ТА

	uint16 IndLO1;

	int16 qd12;			// датчик канала 1 по ракете 2 по координате q
	int16 fd12;			// датчик канала 1 по ракете 2 по координате fi
	int16 qk12;			// координатор канала 1 по ракете 2 по координате q
	int16 fk12;			// координатор канала 1 по ракете 2 по координате fi


	uint8 Sizeq21;		// угловой размер источника по ракете 2 в канале 1 по координате  q
	uint8 Sizef21;		// угловой размер источника по ракете 2 в канале 1 по координате  fi

	uint16 US12;		// уровень сигнала в канале 1 по ракете 2
	uint16 A12;			// размер поля анализа в канале 1 по ракете 2
	uint16 Ksp12;		// отношение сигнал/порог в канале 1 ракета 2

	uint16 reserv1;
	uint16 reserv2;
	uint16 reserv3;
} BattleMathT2K1;

typedef struct BattleStructMathT2K2
{
	uint16	Status;					//состояние ядер по текущей цели

	//Координаты центра полей анализа Цель 1 каналы 1 и 2 от процессоров
	int16 Target2CoordAngX2;
	int16 Target2CoordAngY2;

	//Предыдущие координаты центра полей анализа Цель 2 каналы 1 и 2 от процессоров
	int16 PTarget2CoordAngX2;
	int16 PTarget2CoordAngY2;

	//Координаты центра источника в пикселах Цель 1 каналы 1 и 2 от процессора 1/10 пиксела
	int16 Target2CoordX2;
	int16 Target2CoordY2;

	//Параметры передаваемые от OED на ТА

	uint16 IndLO2;
	uint16 UZK2;

	int16 qd22;			// датчик канала 2 по ракете 2 по координате q
	int16 fd22;			// датчик канала 2 по ракете 2 по координате fi
	int16 qk22;			// координатор канала 2 по ракете 2 по координате q
	int16 fk22;			// координатор канала 2 по ракете 2 по координате fi

	uint8 Sizeq22;		// угловой размер источника по ракете 2 в канале 2 по координате  q
	uint8 Sizef22;		// угловой размер источника по ракете 2 в канале 2 по координате  fi

	uint16 US22;		// уровень сигнала в канале 2 по ракете 2
	uint16 A22;			// размер поля анализа в канале 2 по ракете 2
	uint16 Ksp22;		// отношение сигнал/порог в канале 2 ракета 2

	uint16 reserv4;
	uint16 reserv5;
	uint16 reserv6;

} BattleMathT2K2;

typedef struct BattleStruct
{

	enum {sSTOP=0,sDIAG1,sDIAG2,sDIAG3,sSTANDBY,sBATTLE,sTEXREJ,sIMITBATTLE} STAGE;							//стадии работы
	enum {t1STOP=0,t1GrabChan1,t1ConvoyChan1,t1GrabChan2ConvoyChan1,t1ConvoyChan1_2,t1ConvoyChan2} BMODETARGET1;	//режимы работы в боевом режиме для цели 1
	enum {NotKadrCh1=0,Target1DarkCh1,Target1LightCh1}PropertyKadrCh1;
	enum {NotKadrCh2=0,Target1DarkCh2,Target1LightCh2}PropertyKadrCh2;

	//Значения визирных осей канала 1 и 2
	uint16 VizirChan1_X;
	uint16 VizirChan1_Y;
	uint16 VizirChan2_X;
	uint16 VizirChan2_Y;

	//Значения фокусов
	double FokusK1;
	double FokusK2;

	//Размеры полей анализа Цель1 Канал 1 и 2
	uint16 Target1SizeX1;
	uint16 Target1SizeY1;
	uint16 Target1SizeX2;
	uint16 Target1SizeY2;

	//Координаты левого верхнего угла полей анализа Цель 1 каналы 1 и 2
	int16 Target1CoordX1;
	int16 Target1CoordY1;
	int16 Target1CoordX2;
	int16 Target1CoordY2;

	union ControlBattle	//контроль состоянию работы системы
	{
		uint8 sw;
		struct
		{
			uint8 SumChan1T1	:1;	//Включено суммирование в канале 1 для цели 1
			uint8 SumChan2T1	:1;	//Включено суммирование в канале 2 для цели 1
			uint8 KOLT1 		:1;	//Включен коллиматор в режими имитации цели 1
			uint8 res1 			:5;
		} s;
	} ControlState;

}BattleParam;

typedef struct BattleStruct2
{
	enum {sSTOP2=0,sSTANDBY2,sBATTLE2,sTEXREJ2,sIMITBATTLE2} STAGE2;
	enum {t2STOP=0,t2GrabChan1,t2ConvoyChan1,t2GrabChan2ConvoyChan1,t2ConvoyChan1_2,t2ConvoyChan2} BMODETARGET2;	//режимы работы в боевом режиме для цели 2
	enum {NotKadrT2Ch1=0,Target2DarkCh1,Target2LightCh1}PropertyKadr2Ch1;
	enum {NotKadrT2Ch2=0,Target2DarkCh2,Target2LightCh2}PropertyKadr2Ch2;

	//Значения визирных осей канала 1 и 2
	uint16 VizirChan1_X;
	uint16 VizirChan1_Y;
	uint16 VizirChan2_X;
	uint16 VizirChan2_Y;

	//Значения фокусов
	double FokusK1;
	double FokusK2;

	//Размеры полей анализа Цель2 Канал 1 и 2
	uint16 Target2SizeX1;
	uint16 Target2SizeY1;
	uint16 Target2SizeX2;
	uint16 Target2SizeY2;

	//Координаты левого верхнего угла полей анализа Цель 2 каналы 1 и 2
	int16 Target2CoordX1;
	int16 Target2CoordY1;
	int16 Target2CoordX2;
	int16 Target2CoordY2;

	union ControlBattle2	//контроль состоянию работы системы
	{
		uint8 sw;
		struct
		{
			uint8 SumChan1T2	:1;	//Включено суммирование в канале 1 для цели 2
			uint8 SumChan2T2	:1;	//Включено суммирование в канале 2 для цели 2
			uint8 KOLT2	 		:1;	//Включен коллиматор в режими имитации цели 2
			uint8 res1 			:5;
		} s;
	} ControlState2;

}BattleParam2;

typedef struct DiagStruct
{
	uint8	Target1Ok;
	uint8	Target2Ok;

	uint32	Target1Err;
	uint32	Target2Err;

	uint32 XeCh1;
	uint32 YeCh1;
	uint32 XeCh2;
	uint32 YeCh2;

	uint32 XCh1;
	uint32 YCh1;
	uint32 XCh2;
	uint32 YCh2;
}DiagStruct;

void BattleFunc(int,int);
void SetBattleSizaAndPosition(uint16,uint16,uint16,uint16,uint16);
void CreateDistanseDelayBattle(uint32,uint8,uint8);
void CreateDistanseDelayDiag(uint32);
void SendTechVideoChanal2(int);


#endif /* BATTLEJOB_H_*/
