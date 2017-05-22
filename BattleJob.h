#include "type.h"

#ifndef BATTLEJOB_H_
#define BATTLEJOB_H_

#define MCSRAMCASH			0x0C000000
#define MCSRAMMYVIDEO		0x0C100000

typedef struct BattleStructMathT1K1
{
	uint16	Status;					//��������� ���� �� ������� ����
	//���������� ������ ����� ������� ���� 1 ������ 1 � 2 �� �����������
	int16 Target1CoordAngX1;
	int16 Target1CoordAngY1;

	//���������� ���������� ������ ����� ������� ���� 1 ������ 1 � 2 �� �����������
	int16 PTarget1CoordAngX1;
	int16 PTarget1CoordAngY1;

	//���������� ������ ��������� � �������� ���� 1 ������ 1 � 2 �� ���������� 1/10 �������
	int16 Target1CoordX1;
	int16 Target1CoordY1;

	//��������� ������������ �� OED �� ��
	uint8 IndLO1;

	int16 qd11;			// ������ ������ 1 �� ���������� q
	int16 fd11;			// ������ ������ 1 �� ���������� fi
	int16 qk11;			// ����������� ������ 1 �� ���������� q
	int16 fk11;			// ����������� ������ 1 �� ���������� fi

	uint8 Sizeq11;		// ������� ������ ��������� � ������ 1 �� ����������  q
	uint8 Sizef11;		// ������� ������ ��������� � ������ 1 �� ����������  fi

	uint16 US11;		// ������� ������� � ������ 1
	uint16 A11;			// ������ ���� ������� � ������ 1

	uint16 Ksp11;		// ��������� ������/����� � ������ 1 ������ 1

	uint16 reserv1;
	uint16 reserv2;
	uint16 reserv3;
} BattleMathT1K1;

typedef struct BattleStructMathT1K2
{
	uint16	Status;					//��������� ���� �� ������� ����
	//���������� ������ ����� ������� ���� 1 ������ 1 � 2 �� �����������
	int16 Target1CoordAngX2;
	int16 Target1CoordAngY2;

	//���������� ���������� ������ ����� ������� ���� 1 ������ 1 � 2 �� �����������
	int16 PTarget1CoordAngX2;
	int16 PTarget1CoordAngY2;


	//���������� ������ ��������� � �������� ���� 1 ������ 1 � 2 �� ���������� 1/10 �������
	int16 Target1CoordX2;
	int16 Target1CoordY2;

	//��������� ������������ �� OED �� ��
	uint8 IndLO2;
	uint8 UZK2;

	int16 qd21;			// ������ ������ 2 �� ���������� q
	int16 fd21;			// ������ ������ 2 �� ���������� fi
	int16 qk21;			// ����������� ������ 2 �� ���������� q
	int16 fk21;			// ����������� ������ 2 �� ���������� fi

	uint8 Sizeq12;		// ������� ������ ��������� � ������ 2 �� ����������  q
	uint8 Sizef12;		// ������� ������ ��������� � ������ 2 �� ����������  fi

	uint16 US21;		// ������� ������� � ������ 2

	uint16 A21;			// ������ ���� ������� � ������ 2

	uint16 Ksp21;		// ��������� ������/����� � ������ 2 ������ 1

	uint16 reserv4;
	uint16 reserv5;
	uint16 reserv6;

} BattleMathT1K2;

typedef struct BattleStructMathT2K1
{
	uint16	Status;					//��������� ���� �� ������� ����

	//���������� ������ ����� ������� ���� 1 ������ 1 � 2 �� �����������
	int16 Target2CoordAngX1;
	int16 Target2CoordAngY1;

	//���������� ���������� ������ ����� ������� ���� 2 ������ 1 � 2 �� �����������
	int16 PTarget2CoordAngX1;
	int16 PTarget2CoordAngY1;


	//���������� ������ ��������� � �������� ���� 1 ������ 1 � 2 �� ���������� 1/10 �������
	int16 Target2CoordX1;
	int16 Target2CoordY1;

	//��������� ������������ �� OED �� ��

	uint16 IndLO1;

	int16 qd12;			// ������ ������ 1 �� ������ 2 �� ���������� q
	int16 fd12;			// ������ ������ 1 �� ������ 2 �� ���������� fi
	int16 qk12;			// ����������� ������ 1 �� ������ 2 �� ���������� q
	int16 fk12;			// ����������� ������ 1 �� ������ 2 �� ���������� fi


	uint8 Sizeq21;		// ������� ������ ��������� �� ������ 2 � ������ 1 �� ����������  q
	uint8 Sizef21;		// ������� ������ ��������� �� ������ 2 � ������ 1 �� ����������  fi

	uint16 US12;		// ������� ������� � ������ 1 �� ������ 2
	uint16 A12;			// ������ ���� ������� � ������ 1 �� ������ 2
	uint16 Ksp12;		// ��������� ������/����� � ������ 1 ������ 2

	uint16 reserv1;
	uint16 reserv2;
	uint16 reserv3;
} BattleMathT2K1;

typedef struct BattleStructMathT2K2
{
	uint16	Status;					//��������� ���� �� ������� ����

	//���������� ������ ����� ������� ���� 1 ������ 1 � 2 �� �����������
	int16 Target2CoordAngX2;
	int16 Target2CoordAngY2;

	//���������� ���������� ������ ����� ������� ���� 2 ������ 1 � 2 �� �����������
	int16 PTarget2CoordAngX2;
	int16 PTarget2CoordAngY2;

	//���������� ������ ��������� � �������� ���� 1 ������ 1 � 2 �� ���������� 1/10 �������
	int16 Target2CoordX2;
	int16 Target2CoordY2;

	//��������� ������������ �� OED �� ��

	uint16 IndLO2;
	uint16 UZK2;

	int16 qd22;			// ������ ������ 2 �� ������ 2 �� ���������� q
	int16 fd22;			// ������ ������ 2 �� ������ 2 �� ���������� fi
	int16 qk22;			// ����������� ������ 2 �� ������ 2 �� ���������� q
	int16 fk22;			// ����������� ������ 2 �� ������ 2 �� ���������� fi

	uint8 Sizeq22;		// ������� ������ ��������� �� ������ 2 � ������ 2 �� ����������  q
	uint8 Sizef22;		// ������� ������ ��������� �� ������ 2 � ������ 2 �� ����������  fi

	uint16 US22;		// ������� ������� � ������ 2 �� ������ 2
	uint16 A22;			// ������ ���� ������� � ������ 2 �� ������ 2
	uint16 Ksp22;		// ��������� ������/����� � ������ 2 ������ 2

	uint16 reserv4;
	uint16 reserv5;
	uint16 reserv6;

} BattleMathT2K2;

typedef struct BattleStruct
{

	enum {sSTOP=0,sDIAG1,sDIAG2,sDIAG3,sSTANDBY,sBATTLE,sTEXREJ,sIMITBATTLE} STAGE;							//������ ������
	enum {t1STOP=0,t1GrabChan1,t1ConvoyChan1,t1GrabChan2ConvoyChan1,t1ConvoyChan1_2,t1ConvoyChan2} BMODETARGET1;	//������ ������ � ������ ������ ��� ���� 1
	enum {NotKadrCh1=0,Target1DarkCh1,Target1LightCh1}PropertyKadrCh1;
	enum {NotKadrCh2=0,Target1DarkCh2,Target1LightCh2}PropertyKadrCh2;

	//�������� �������� ���� ������ 1 � 2
	uint16 VizirChan1_X;
	uint16 VizirChan1_Y;
	uint16 VizirChan2_X;
	uint16 VizirChan2_Y;

	//�������� �������
	double FokusK1;
	double FokusK2;

	//������� ����� ������� ����1 ����� 1 � 2
	uint16 Target1SizeX1;
	uint16 Target1SizeY1;
	uint16 Target1SizeX2;
	uint16 Target1SizeY2;

	//���������� ������ �������� ���� ����� ������� ���� 1 ������ 1 � 2
	int16 Target1CoordX1;
	int16 Target1CoordY1;
	int16 Target1CoordX2;
	int16 Target1CoordY2;

	union ControlBattle	//�������� ��������� ������ �������
	{
		uint8 sw;
		struct
		{
			uint8 SumChan1T1	:1;	//�������� ������������ � ������ 1 ��� ���� 1
			uint8 SumChan2T1	:1;	//�������� ������������ � ������ 2 ��� ���� 1
			uint8 KOLT1 		:1;	//������� ���������� � ������ �������� ���� 1
			uint8 res1 			:5;
		} s;
	} ControlState;

}BattleParam;

typedef struct BattleStruct2
{
	enum {sSTOP2=0,sSTANDBY2,sBATTLE2,sTEXREJ2,sIMITBATTLE2} STAGE2;
	enum {t2STOP=0,t2GrabChan1,t2ConvoyChan1,t2GrabChan2ConvoyChan1,t2ConvoyChan1_2,t2ConvoyChan2} BMODETARGET2;	//������ ������ � ������ ������ ��� ���� 2
	enum {NotKadrT2Ch1=0,Target2DarkCh1,Target2LightCh1}PropertyKadr2Ch1;
	enum {NotKadrT2Ch2=0,Target2DarkCh2,Target2LightCh2}PropertyKadr2Ch2;

	//�������� �������� ���� ������ 1 � 2
	uint16 VizirChan1_X;
	uint16 VizirChan1_Y;
	uint16 VizirChan2_X;
	uint16 VizirChan2_Y;

	//�������� �������
	double FokusK1;
	double FokusK2;

	//������� ����� ������� ����2 ����� 1 � 2
	uint16 Target2SizeX1;
	uint16 Target2SizeY1;
	uint16 Target2SizeX2;
	uint16 Target2SizeY2;

	//���������� ������ �������� ���� ����� ������� ���� 2 ������ 1 � 2
	int16 Target2CoordX1;
	int16 Target2CoordY1;
	int16 Target2CoordX2;
	int16 Target2CoordY2;

	union ControlBattle2	//�������� ��������� ������ �������
	{
		uint8 sw;
		struct
		{
			uint8 SumChan1T2	:1;	//�������� ������������ � ������ 1 ��� ���� 2
			uint8 SumChan2T2	:1;	//�������� ������������ � ������ 2 ��� ���� 2
			uint8 KOLT2	 		:1;	//������� ���������� � ������ �������� ���� 2
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
