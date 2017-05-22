#include "type.h"

#ifndef PAKETOEDTAETH_H_
#define PAKETOEDTAETH_H_

#define NUMBEROED		1		//����� �������� �������
#define DISTANSEDELAY	26.5	//����������� �������� ������� �� ��� 0 ���������

#define TRUE   1
#define FALSE  0

//������ ������� 1 � 2
#define FOKUSK1	50.8
#define FOKUSK2	331.7
/////////////////////////////////////////////////////
#define VerPO  4	//������ ������������ �����������
/////////////////////////////////////////////////////
/////////////////////////////////////////////////////
//�������� �������� ���� ������ 1 � 2
#define VizirChan1X			1024
#define VizirChan1Y			544

#define VizirChan2X			1024
#define VizirChan2Y			544
//���� ������� ������� � ��������
#define PIXEL				5.5
#define PI					3.14159265

/////////////////////////////////////////////////////
//////////////////////////���������� ��������� ������������� ������//////////////////////
#define GAIN		40
#define FOTReg73	10
#define OffsetChan1	16332
#define OffsetChan2	16330


#define REG32(addr) (*(volatile unsigned int*)(addr))
#define REG16(addr) (*(volatile unsigned short*)(addr))
#define REG8(addr)  (*(volatile unsigned char*)(addr))

#define SizeBuffOED 		70					//������ ������ ��� � ������ ��� CRC
#define SizeBuffTA 			36					//������ ������ �� � ������ ��� CRC
#define SizeKadrTechRejX	480					//���������� �������� � ������
#define SizeKadrTechRejY	480					//���������� �����


//������� ����������
typedef struct OEDTOTA {
	union SignalOED1{
			uint16 sw;
			struct {
				uint16 Ready 		:1;	//�����
				uint16 Healthy 		:1;	//��������
				uint16 IndLO1 		:1; //��������� �� � ������ 1
				uint16 IndLO2 		:1;	//��������� �� � ������ 2
				uint16 Light 		:1; //��������
				uint16 TNorm 		:1; //����������� � �����
				uint16 StrobK 		:1; //������� ������
				uint16 UZK2 		:1; //������������ ������ 2 �� ����� ���� ������
				uint16 res01 		:1;
				uint16 res02 		:1;
				uint16 res03 		:1;
				uint16 res04 		:1;
				uint16 res05 		:1;
				uint16 res06 		:1;
				uint16 NumTarget	:1; //����� �������
				uint16 SavePusk 	:1; //���� ����� ��������
			} s;
		} SignalOED1;

	uint16 reserv1;		//���������������� ��� ��������� ��������� ���. ������

	int16 qd1;			// ������ ������ 1 �� ���������� q
	int16 fd1;			// ������ ������ 1 �� ���������� fi
	int16 qk1;			// ����������� ������ 1 �� ���������� q
	int16 fk1;			// ����������� ������ 1 �� ���������� fi

	int16 qd2;			// ������ ������ 2 �� ���������� q
	int16 fd2;			// ������ ������ 2 �� ���������� fi
	int16 qk2;			// ����������� ������ 2 �� ���������� q
	int16 fk2;			// ����������� ������ 2 �� ���������� fi

	uint8 Sizeq11;		// ������� ������ ��������� � ������ 1 �� ����������  q
	uint8 Sizef11;		// ������� ������ ��������� � ������ 1 �� ����������  fi
	uint8 Sizeq12;		// ������� ������ ��������� � ������ 2 �� ����������  q
	uint8 Sizef12;		// ������� ������ ��������� � ������ 2 �� ����������  fi

	uint16 US1;			// ������� ������� � ������ 1
	uint16 US2;			// ������� ������� � ������ 2

	uint16 A1;			// ������ ���� ������� � ������ 1
	uint16 A2;			// ������ ���� ������� � ������ 2

	uint16 Ntime;		//����� ��������� � �����
	uint16 Ncycle;		//���������� ������������ ������

	uint16 FokusK1;		//����� ������ 1
	uint16 FokusK2;		//����� ������ 2

	uint16 STK1;		//�������� ������ ������� ���������� � ������ 1 ������������ ������� STROBK1
	uint16 STK2;		//�������� ������ ������� ���������� � ������ 2 ������������ ������� STROBK1

	uint16 Ksp1;		// ��������� ������/����� � ������ 1
	uint16 Ksp2;		// ��������� ������/����� � ������ 2

	uint16 OEDVerPO;	// ������ ������������ �����������

	uint16 reserv2;
	uint16 reserv3;
	uint16 reserv4;
	uint16 reserv5;
	uint16 reserv6;

	uint16 NumPaket;	//����� ������
	int16 Temper;		//������� ����������� ���

	uint16 Tstrob;		//������ ������
	uint16 DeltaTstrob;	//�������� ����� ��������

	uint16 reserv7;

} OEDTOTA;

typedef struct TATOOED {

	//��������� ����� ���������� ���
	union CommandTA1{
		uint16 sw;
		struct {
			uint16 Pusk 		:1;	//���� ������
			uint16 Sxod 		:1; //���� ������
			uint16 SoprLO1	 	:1;	//���������� ������������� � ������ 1
			uint16 SoprLO2 		:1; //���������� ������������� � ������ 2
			uint16 ResetK1 		:1; //����� ������ 1
			uint16 Ground 		:1; //������ �� �������� ����
			uint16 res01 		:1;
			uint16 res02 		:1;
			uint16 res03 		:1;
			uint16 res04 		:1;
			uint16 res05 		:1;
			uint16 res06 		:1;
			uint16 res07 		:1;
			uint16 res08 		:1;
			uint16 NumTarget	:1; //����� �������
			uint16 SavePusk 	:1; //���������� ���������� �����
		} s;
	} CommandTA1;

	//��������� ����� ��� ��������� ������
	union CommandTA2{
		uint16 sw;
		struct {
			uint16 TechMode	:4;	//��� ���. ������
			uint16 TimeExp	:5; //��� ������� ���������� � ��� ������
			uint16 res1 	:1;
			uint16 res2 	:1;
			uint16 res3 	:1;
			uint16 res4 	:1;
			uint16 res5 	:1;
			uint16 res6 	:1;
			uint16 KolUp 	:1;//�������� ���������� � ������ ������
		} s;
	} CommandTA2;

	int32 qk1;					// ��������� � ������ 1 �� ���������� q
	int32 fk1;					// ��������� � ������ 1 �� ���������� fi
	int16 qk2;					// ��������� � ������ 2 �� ���������� q
	int16 fk2;					// ��������� � ������ 2 �� ���������� fi
	int32 qPod;					// �������� ��� �� ���������� q
	int32 fPod;					// �������� ��� �� ���������� fi
	uint16 Distance;			// ����������� ���������

	int16 qGDU;					// ��� �� ���������� q
	int16 fGDU;					// ��� �� ���������� fi

	uint16 res1;
	uint16 res2;
	uint16 res3;
	uint16 res4;
	uint16 res5;

	uint16 NumPaket;			// ����� ������
	uint16 res6;

} TATOOED;

typedef struct VideoTechParam
{
	union Word1{
				uint32 sw;
				struct {
					uint32 KBP 			:5;	//���������� ��� �� ������� �����������
					uint32 res1 		:27;
				} s;
			} Word1;

	union Word2{
				uint32 sw;
				struct {
					uint32 TechRej 		:4;	//������������� ���������������� ������
					uint32 TimeExp 		:5;	//����� ����������
					uint32 res1 		:23;
				} s;
			} Word2;

	uint32 NPK;			//����� ������������� �����
	uint16 RKB;			//������ ����� �� ���������
	uint16 RKG;			//������ ����� �� ���������
	int16 kq2;			//���������� ������� ������ ���� � �������� �� �������
	int16 kf2;			//���������� ������� ������ ���� � �������� �� ���� �����
	uint16 dq2;			//���� ������� � ����� �� �������
	uint16 df2;			//���� ������� � ����� �� ���� �����
	uint16 res1;
	uint16 res2;
	uint16 res3;
	uint16 res4;

} VideoTechParam;

typedef struct VideoTechPacket
{
	uint16 res1;
	uint16 IMD;			//������������� ������� �����������
	uint32 NPK;			//����� ������������� �����
	uint16 KSS;			//���������� ����� � ������� ���������
	uint16 NPS;			//����� ������ ������ � ������� ������
	uint32 res2;
} VideoTechPacket;

//�������� ������ � ��������������////////////////////////////////////////////////
typedef struct VisualKommandPaket
{
		union Kommand{
					uint32 sw;
					struct {
						uint32 VP 			:1;	//������� �� ������ ������ �����
						uint32 SM 			:1;	//������� �� ������������ x2 �����������
						uint32 ImB 			:1;	//������� �� �������� ������� ������ �� ����������� ��� �����.
						uint32 ImBS1 		:1;	//������� �� �������� ������� ������ �� ����1.
						uint32 ImBS2 		:1;	//������� �� �������� ������� ������ �� ����2.
						uint32 res1 		:26;
						uint32 RS	 		:1;	//������� �� ������� 2 ����
					} s;
				} KommandWord;

		union Control{
					uint32 sw;
					struct {
						uint32 S1	 		:1;	//��������� ������������� ������������ ����1
						uint32 S2	 		:1;	//��������� ������������� ������������ ����2
						uint32 KN	 		:1;	//��������� ����������� � ����������� ������
						uint32 KI	 		:1;	//��������� ����������� � ���������� ������
						uint32 B	 		:1;	//��������� �������� 16-������� �����������
						uint32 KO	 		:4;	//���������� �������� ��������
						uint32 Imp			:1;	//������ �� �������� ����� � ��������� �� �������� �� ��������������
						uint32 res1 		:21;
						uint32 RS	 		:1;	//������� �� ������� 2 ����
					} s;
				} ControlWord;

		union DataWord1{
					uint32 sw;
					struct {
							uint32 SizeX1 		:12;//������ X ���� ������� � ������ 1
							uint32 SizeY1 		:12;//������ Y ���� ������� � ������ 1
							uint32 StartExp1	:8;	//�������� ������ ������� ���������� � ���
						} s;
					} DataWord1;

		union DataWord2{
					uint32 sw;
					struct {
							uint32 SizeX2 		:12;//������ X ���� ������� � ������ 2
							uint32 SizeY2 		:12;//������ Y ���� ������� � ������ 2
							uint32 StartExp2	:8;	//�������� ������ ������� ���������� � ���
						} s;
					} DataWord2;

		union DataWord3{
					uint32 sw;
					struct {
							uint32 CoordX1 		:11;//���������� X ������ ���� ������� � ������ 1
							uint32 CoordY1 		:11;//���������� Y ������ ���� ������� � ������ 1
							uint32 Exp1			:10;//������ ������� ���������� � ���
						} s;
					} DataWord3;

		union DataWord4{
					uint32 sw;
					struct {
							uint32 CoordX2 		:11;//���������� X ������ ���� ������� � ������ 2
							uint32 CoordY2 		:11;//���������� Y ������ ���� ������� � ������ 2
							uint32 Exp2			:10;//������ ������� ���������� � ���
						} s;
					} DataWord4;

		union CMOS1{
					uint32 sw;
					struct {
							uint32 Reg73 		:8;//�������� �������� 73
							uint32 ADC_GAIN		:8;//����������� ��������
							uint32 Offset		:14;//�������� �� ������ �������
							uint32 res	 		:2;
							} s;
						} CMOS1;
		uint32 res1;
		uint32 res2;

		union CMOS2{
					uint32 sw;
					struct {
							uint32 Reg73 		:8;//�������� �������� 73
							uint32 ADC_GAIN		:8;//����������� ��������
							uint32 Offset		:14;//�������� �� ������ �������
							uint32 res	 		:2;
							} s;
						} CMOS2;

		uint32 res3;
		uint32 res4;

		union DataWord5{
					uint32 sw;
					struct {
							uint32 NumPacket	:16;//����� ������
							uint32 Distance		:16;//����������� ���������
						} s;
					} DataWord5;

		union MathKommand{
					uint32 sw;
					struct {
							uint32 Kommand		:16;//������� ���������� �����������
							uint32 K_s_sh		:16;//����������� ������/���
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
//���� 1, ����� 1, ���� ������� - 1
uint16 TimeJob111;				//����� ��������� ����� �� ���� 1 � ������ 1 ���� 1
uint16 NPointPorg111;			//���������� ����� ���� ������ �� ���� 1 � ������ 1 ���� 1
uint16 SKO111;					//CKO �� ���� 1 � ������ 1 ���� 1
uint16 NOBJ111;					//���������� �������� �� ���� 1 � ������ 1 ���� 1
int16 CordX111;					//���������� ���� �� X ���� 1 � ������ 1 ���� 1
int16 CordY111;					//���������� ���� �� Y ���� 1 � ������ 1 ���� 1
uint16 SUFon111;				//������� ������� ���� ���� 1 � ������ 1 ���� 1
uint16 LocUFon111;				//��������� ������� ���� ���� 1 � ������ 1 ���� 1
uint16 LocSKO111;				//CKO ��������� ����������� �� ���� 1 � ������ 1 ���� 1
uint16 res111_1;
uint16 res111_2;
uint16 res111_3;
uint16 res111_4;
uint16 res111_5;
} VisualStatusPaket11;


typedef struct VisualStatusPaketProcPar
{
//���� 1, ����� 1, ���� ������� - 1
uint16 TimeJob;				//����� ��������� ����� �� ���� 1 � ������ 1 ���� 1
uint16 NPointPorg;			//���������� ����� ���� ������ �� ���� 1 � ������ 1 ���� 1
uint16 SKO;					//CKO �� ���� 1 � ������ 1 ���� 1
uint16 NOBJ;					//���������� �������� �� ���� 1 � ������ 1 ���� 1
int16 CordX;					//���������� ���� �� X ���� 1 � ������ 1 ���� 1
int16 CordY;					//���������� ���� �� Y ���� 1 � ������ 1 ���� 1
uint16 SUFon;				//������� ������� ���� ���� 1 � ������ 1 ���� 1
uint16 LocUFon;				//��������� ������� ���� ���� 1 � ������ 1 ���� 1
uint16 LocSKO;				//CKO ��������� ����������� �� ���� 1 � ������ 1 ���� 1
uint16 res_1;
uint16 res_2;
uint16 res_3;
uint16 res_4;
uint16 res_5;
} VisualStatusPaketProcPar;

typedef struct VisualStatusPaket112
{
//���� 1, ����� 1, ���� ���������� - 2
uint16 TimeJob112;				//����� ��������� ����� �� ���� 1 � ������ 1 ���� 2
uint16 NPointPorg112;			//���������� ����� ���� ������ �� ���� 1 � ������ 1 ���� 2
uint16 SKO112;					//CKO �� ���� 1 � ������ 1 ���� 2
uint16 NOBJ112;					//���������� �������� �� ���� 1 � ������ 1 ���� 2
int16 CordX112;					//���������� ���� �� X ���� 1 � ������ 1 ���� 2
int16 CordY112;					//���������� ���� �� Y ���� 1 � ������ 1 ���� 2
uint16 SUFon112;				//������� ������� ���� ���� 1 � ������ 1 ���� 2
uint16 LocUFon112;				//��������� ������� ���� ���� 1 � ������ 1 ���� 2
uint16 LocSKO112;				//CKO ��������� ����������� �� ���� 1 � ������ 1 ���� 2
uint16 res112_1;
uint16 res112_2;
uint16 res112_3;
uint16 res112_4;
uint16 res112_5;
} VisualStatusPaket112;


typedef struct VisualStatusPaket
{
	uint32 NumKadr;		//����� �������� �����

	union ControlWord1{
			uint32 sw;
			struct {
					uint32 B	 		:1;//�������� �����������
					uint32 STROB1 		:1;//������� ������������� �����1
					uint32 STROB2 		:1;//������� ������������� �����2
					uint32 WithLO		:1;//������� ��������� � �����
					uint32 Res			:28;
				} s;
			} ControlWord1;

	union Data1{
			uint32 sw;
			struct {
					uint32 CoordX1 		:11;//���������� X ������ ���� ������� � ������ 1
					uint32 CoordY1 		:11;//���������� Y ������ ���� ������� � ������ 1
					uint32 Exp1			:10;//����� ���������� � ���
				} s;
			} Data1;

	union Data2{
			uint32 sw;
			struct {
					uint32 CoordX2 		:11;//���������� X ������ ���� ������� � ������ 2
					uint32 CoordY2 		:11;//���������� Y ������ ���� ������� � ������ 2
					uint32 Exp2	 		:10;//����� ���������� � ���
				} s;
			} Data2;

	union Data3{
			uint32 sw;
			struct {
					uint32 SizeX1 		:12;//������ X ���� ������� � ������ 1
					uint32 SizeY1 		:12;//������ Y ���� ������� � ������ 1
					int32 Temper		:8;//����������� �������
				} s;
			} Data3;

	union Data4{
			uint32 sw;
			struct {
					uint32 SizeX2 		:12;//������ X ���� ������� � ������ 2
					uint32 SizeY2 		:12;//������ Y ���� ������� � ������ 2
					uint32 res			:8;
				} s;
			} Data4;

	union Data5{
			uint32 sw;
			struct {
					uint32 Fokus1		:16;//����� ������ 1
					uint32 Fokus2		:16;//����� ������ 2
				} s;
			} Data5;

	union StatVPUXilinx{
			uint32 sw;
			struct {
					uint32 StatusK1		:16;//������ srio ������ 1
					uint32 StatusK2		:16;//������ srio ������ 2
				} s;
			} StatVPUXilinx;

	uint32 TimeShtampGlobalX;				//����� �������� �������� Xilinx VPU

	uint32 Tvpu;							//������������� ������� vpu

	union Data6{
			uint32 sw;
			struct {
					uint32 Distance		:16;//���������� ���������
					uint32 NumPribor	:16;//����� �������
				} s;
			} Data6;

	union StatPVPK1_1{
			uint32 sw;
			struct {
					uint8 Status0		:8;//��������� �������
					uint8 Status1		:8;//��������� �������
					uint8 Status2		:8;//��������� �������
					uint8 Status3		:8;//��������� �������
				} s;
			} StatPVPK1_1;

	union StatPVPK1_2{
			uint32 sw;
			struct {
					uint8 Status4		:8;//��������� �������
					uint8 Status5		:8;//��������� �������
					uint8 Status6		:8;//��������� �������
					uint8 Status7		:8;//��������� �������
				} s;
			} StatPVPK1_2;

	union StatPVPK2_1{
			uint32 sw;
			struct {
					uint8 Status0		:8;//��������� �������
					uint8 Status1		:8;//��������� �������
					uint8 Status2		:8;//��������� �������
					uint8 Status3		:8;//��������� �������
				} s;
			} StatPVPK2_1;

	union StatPVPK2_2{
			uint32 sw;
			struct {
					uint8 Status4		:8;//��������� �������
					uint8 Status5		:8;//��������� �������
					uint8 Status6		:8;//��������� �������
					uint8 Status7		:8;//��������� �������
				} s;
			} StatPVPK2_2;

	uint32 Tvpu2;							//������������� ������� vpu 2

	union STROBPERIOD{
			uint32 sw;
			struct {
					uint32 STR1PERIOD	:16;//������ ���������� �������� STROB1
					uint32 STR2PERIOD	:16;//������ ���������� �������� STROB2
				} s;
			} STRPERIOD;
	union STROBDELAY{
			uint32 sw;
			struct {
					uint32 DelayStr1Str2	:16;//�������� ����� STROB1 � STROB2
					uint32 SigImitBattleJob	:16;//������� ��������� ��������� ������� ������
				} s;
			} STRDELAY;

	//���� 1, ����� 1, ���� ������� - 1
	uint16 TimeJob111;				//����� ��������� ����� �� ���� 1 � ������ 1 ���� 1
	uint16 NPointPorg111;			//���������� ����� ���� ������ �� ���� 1 � ������ 1 ���� 1
	uint16 SKO111;					//CKO �� ���� 1 � ������ 1 ���� 1
	uint16 NOBJ111;					//���������� �������� �� ���� 1 � ������ 1 ���� 1
	int16 CordX111;					//���������� ���� �� X ���� 1 � ������ 1 ���� 1
	int16 CordY111;					//���������� ���� �� Y ���� 1 � ������ 1 ���� 1
	uint16 SUFon111;				//������� ������� ���� ���� 1 � ������ 1 ���� 1
	uint16 LocUFon111;				//��������� ������� ���� ���� 1 � ������ 1 ���� 1
	uint16 LocSKO111;				//CKO ��������� ����������� �� ���� 1 � ������ 1 ���� 1
	uint16 res111_1;
	uint16 res111_2;
	uint16 res111_3;
	uint16 res111_4;
	uint16 res111_5;

	//���� 1, ����� 1, ���� ���������� - 2
	uint16 TimeJob112;				//����� ��������� ����� �� ���� 1 � ������ 1 ���� 2
	uint16 NPointPorg112;			//���������� ����� ���� ������ �� ���� 1 � ������ 1 ���� 2
	uint16 SKO112;					//CKO �� ���� 1 � ������ 1 ���� 2
	uint16 NOBJ112;					//���������� �������� �� ���� 1 � ������ 1 ���� 2
	int16 CordX112;					//���������� ���� �� X ���� 1 � ������ 1 ���� 2
	int16 CordY112;					//���������� ���� �� Y ���� 1 � ������ 1 ���� 2
	uint16 SUFon112;				//������� ������� ���� ���� 1 � ������ 1 ���� 2
	uint16 LocUFon112;				//��������� ������� ���� ���� 1 � ������ 1 ���� 2
	uint16 LocSKO112;				//CKO ��������� ����������� �� ���� 1 � ������ 1 ���� 2
	uint16 res112_1;
	uint16 res112_2;
	uint16 res112_3;
	uint16 res112_4;
	uint16 res112_5;

	//���� 1, ����� 2, ���� ������� - 1
	uint16 TimeJob121;				//����� ��������� ����� �� ���� 1 � ������ 2 ���� 1
	uint16 NPointPorg121;			//���������� ����� ���� ������ �� ���� 1 � ������ 2 ���� 1
	uint16 SKO121;					//CKO �� ���� 1 � ������ 2 ���� 1
	uint16 NOBJ121;					//���������� �������� �� ���� 1 � ������ 2 ���� 1
	int16 CordX121;					//���������� ���� �� X ���� 1 � ������ 2 ���� 1
	int16 CordY121;					//���������� ���� �� Y ���� 1 � ������ 2 ���� 1
	uint16 SUFon121;				//������� ������� ���� ���� 1 � ������ 2 ���� 1
	uint16 LocUFon121;				//��������� ������� ���� ���� 1 � ������ 2 ���� 1
	uint16 LocSKO121;				//CKO ��������� ����������� �� ���� 1 � ������ 2 ���� 1
	uint16 res121_1;
	uint16 res121_2;
	uint16 res121_3;
	uint16 res121_4;
	uint16 res121_5;

	//���� 1, ����� 2, ���� ������� - 2
	uint16 TimeJob122;				//����� ��������� ����� �� ���� 1 � ������ 2 ���� 2
	uint16 NPointPorg122;			//���������� ����� ���� ������ �� ���� 1 � ������ 2 ���� 2
	uint16 SKO122;					//CKO �� ���� 1 � ������ 2 ���� 2
	uint16 NOBJ122;					//���������� �������� �� ���� 1 � ������ 2 ���� 2
	int16 CordX122;					//���������� ���� �� X ���� 1 � ������ 2 ���� 2
	int16 CordY122;					//���������� ���� �� Y ���� 1 � ������ 2 ���� 2
	uint16 SUFon122;				//������� ������� ���� ���� 1 � ������ 2 ���� 2
	uint16 LocUFon122;				//��������� ������� ���� ���� 1 � ������ 2 ���� 2
	uint16 LocSKO122;				//CKO ��������� ����������� �� ���� 1 � ������ 2 ���� 2
	uint16 res122_1;
	uint16 res122_2;
	uint16 res122_3;
	uint16 res122_4;
	uint16 res122_5;

	//���� 2, ����� 1, ���� ������� - 1
	uint16 TimeJob211;				//����� ��������� ����� �� ���� 2 � ������ 1 ���� 1
	uint16 NPointPorg211;			//���������� ����� ���� ������ �� ���� 2 � ������ 1 ���� 1
	uint16 SKO211;					//CKO �� ���� 2 � ������ 1 ���� 1
	uint16 NOBJ211;					//���������� �������� �� ���� 2 � ������ 1 ���� 1
	int16 CordX211;					//���������� ���� �� X ���� 2 � ������ 1 ���� 1
	int16 CordY211;					//���������� ���� �� Y ���� 2 � ������ 1 ���� 1
	uint16 SUFon211;				//������� ������� ���� ���� 2 � ������ 1 ���� 1
	uint16 LocUFon211;				//��������� ������� ���� ���� 2 � ������ 1 ���� 1
	uint16 LocSKO211;				//CKO ��������� ����������� �� ���� 2 � ������ 1 ���� 1
	uint16 res211_1;
	uint16 res211_2;
	uint16 res211_3;
	uint16 res211_4;
	uint16 res211_5;

	//���� 2, ����� 1, ���� ���������� - 2
	uint16 TimeJob212;				//����� ��������� ����� �� ���� 2 � ������ 1 ���� 2
	uint16 NPointPorg212;			//���������� ����� ���� ������ �� ���� 2 � ������ 1 ���� 2
	uint16 SKO212;					//CKO �� ���� 2 � ������ 1 ���� 2
	uint16 NOBJ212;					//���������� �������� �� ���� 2 � ������ 1 ���� 2
	int16 CordX212;					//���������� ���� �� X ���� 2 � ������ 1 ���� 2
	int16 CordY212;					//���������� ���� �� Y ���� 2 � ������ 1 ���� 2
	uint16 SUFon212;				//������� ������� ���� ���� 2 � ������ 1 ���� 2
	uint16 LocUFon212;				//��������� ������� ���� ���� 2 � ������ 1 ���� 2
	uint16 LocSKO212;				//CKO ��������� ����������� �� ���� 2 � ������ 1 ���� 2
	uint16 res212_1;
	uint16 res212_2;
	uint16 res212_3;
	uint16 res212_4;
	uint16 res212_5;

	//���� 2, ����� 2, ���� ������� - 1
	uint16 TimeJob221;				//����� ��������� ����� �� ���� 2 � ������ 2 ���� 1
	uint16 NPointPorg221;			//���������� ����� ���� ������ �� ���� 2 � ������ 2 ���� 1
	uint16 SKO221;					//CKO �� ���� 2 � ������ 2 ���� 1
	uint16 NOBJ221;					//���������� �������� �� ���� 2 � ������ 2 ���� 1
	int16 CordX221;					//���������� ���� �� X ���� 2 � ������ 2 ���� 1
	int16 CordY221;					//���������� ���� �� Y ���� 2 � ������ 2 ���� 1
	uint16 SUFon221;				//������� ������� ���� ���� 2 � ������ 2 ���� 1
	uint16 LocUFon221;				//��������� ������� ���� ���� 2 � ������ 2 ���� 1
	uint16 LocSKO221;				//CKO ��������� ����������� �� ���� 2 � ������ 2 ���� 1
	uint16 res221_1;
	uint16 res221_2;
	uint16 res221_3;
	uint16 res221_4;
	uint16 res221_5;

	//���� 2, ����� 2, ���� ������� - 2
	uint16 TimeJob222;				//����� ��������� ����� �� ���� 2 � ������ 2 ���� 2
	uint16 NPointPorg222;			//���������� ����� ���� ������ �� ���� 2 � ������ 2 ���� 2
	uint16 SKO222;					//CKO �� ���� 2 � ������ 2 ���� 2
	uint16 NOBJ222;					//���������� �������� �� ���� 2 � ������ 2 ���� 2
	int16 CordX222;					//���������� ���� �� X ���� 2 � ������ 2 ���� 2
	int16 CordY222;					//���������� ���� �� Y ���� 2 � ������ 2 ���� 2
	uint16 SUFon222;				//������� ������� ���� ���� 2 � ������ 2 ���� 2
	uint16 LocUFon222;				//��������� ������� ���� ���� 2 � ������ 2 ���� 2
	uint16 LocSKO222;				//CKO ��������� ����������� �� ���� 2 � ������ 2 ���� 2
	uint16 res222_1;
	uint16 res222_2;
	uint16 res222_3;
	uint16 res222_4;
	uint16 res222_5;

} VisualStatusPaket;

typedef struct VisualKadrPaket
{
	uint32 NumKadr;				//����� �������� �����
	uint32 OffsetInPacket;		//�������� � ����� �����

	union Data{
			uint32 sw;
			struct {
					uint32 SizePacket 	:11;//������ ������
					uint32 NumPacket	:21;//����� ������
				} s;
			} Data;
} VisualKadrPaket;
////////////////////////////////////////////////////////////////////////////////
//timeout
void CycleDelay (Int32 count);

//��������� �������
void InitPaketBattleJob();
void InitPaketTech();
void assembleBuffOED(uint16* Buff,int target,int size);
int disassembleBuffTA(uint16* Buff,int target,int size);
void AssembleBuffersTechVideo(uint8* Buff,int size);
void spi_cmos_reset_enable();

//����� ��������� UDP
int dtask_udp_visual(SOCKET s, UINT32 unused);
int dtask_udp_target1(SOCKET s, UINT32 unused);
int dtask_udp_target2(SOCKET s, UINT32 unused);
int dtask_udp_flash( SOCKET s, UINT32 unused );
int dtask_d_flash( SOCKET s, UINT32 unused );



uint16 ChecksumCalck(uint16* Buff,int size);	//������� ����������� �����

void CreateVideo(int sizeX1,int sizeY1,int sizeX2,int sizeY2,int type);

void CreateVideoChanal2(int sizeX2,int sizeY2);	//�������� ����� 2 ������ ��� ��� ������


#endif /* PAKETOEDTAETH_H_ */

