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

typedef struct FlashDataFramStruct	//��������� ����������� ������� �� fram ������
{
	uint32 	MarkerStart;
	uint32 	NumDev;					//����� �������
	uint32	PosNumRing;				//����� ��������� ������� � ������
	uint32  GlobalNumPusk;			//���������� ��������� ����� ������������ �����
	uint32	MarkerStartStrPusk;		//������ ������ ������� ���������� � ������

	uint32	NumPusk1;				//����� ����� ������������ � ������ ����
	uint32	AddrPusk1;				//���������� ����� ����� �� flash
	uint32 	SizePusk1;				//������ ����� � ������

	uint32	NumPusk2;				//����� ����� ������������ � ������ ����
	uint32	AddrPusk2;				//���������� ����� ����� �� flash
	uint32 	SizePusk2;				//������ ����� � ������

	uint32	NumPusk3;				//����� ����� ������������ � ������ ����
	uint32	AddrPusk3;				//���������� ����� ����� �� flash
	uint32 	SizePusk3;				//������ ����� � ������

	uint32	NumPusk4;				//����� ����� ������������ � ������ ����
	uint32	AddrPusk4;				//���������� ����� ����� �� flash
	uint32 	SizePusk4;				//������ ����� � ������

	uint32	NumPusk5;				//����� ����� ������������ � ������ ����
	uint32	AddrPusk5;				//���������� ����� ����� �� flash
	uint32 	SizePusk5;				//������ ����� � ������

	uint32	NumPusk6;				//����� ����� ������������ � ������ ����
	uint32	AddrPusk6;				//���������� ����� ����� �� flash
	uint32 	SizePusk6;				//������ ����� � ������

	uint32	NumPusk7;				//����� ����� ������������ � ������ ����
	uint32	AddrPusk7;				//���������� ����� ����� �� flash
	uint32 	SizePusk7;				//������ ����� � ������

	uint32	NumPusk8;				//����� ����� ������������ � ������ ����
	uint32	AddrPusk8;				//���������� ����� ����� �� flash
	uint32 	SizePusk8;				//������ ����� � ������

	uint32	NumPusk9;				//����� ����� ������������ � ������ ����
	uint32	AddrPusk9;				//���������� ����� ����� �� flash
	uint32 	SizePusk9;				//������ ����� � ������

	uint32	NumPusk10;				//����� ����� ������������ � ������ ����
	uint32	AddrPusk10;				//���������� ����� ����� �� flash
	uint32 	SizePusk10;				//������ ����� � ������

	uint32	NumPusk11;				//����� ����� ������������ � ������ ����
	uint32	AddrPusk11;				//���������� ����� ����� �� flash
	uint32 	SizePusk11;				//������ ����� � ������

	uint32	NumPusk12;				//����� ����� ������������ � ������ ����
	uint32	AddrPusk12;				//���������� ����� ����� �� flash
	uint32 	SizePusk12;				//������ ����� � ������

	uint32	NumPusk13;				//����� ����� ������������ � ������ ����
	uint32	AddrPusk13;				//���������� ����� ����� �� flash
	uint32 	SizePusk13;				//������ ����� � ������

	uint32	NumPusk14;				//����� ����� ������������ � ������ ����
	uint32	AddrPusk14;				//���������� ����� ����� �� flash
	uint32 	SizePusk14;				//������ ����� � ������

	uint32 XOR;						//����������� ����� ������
}FramPuskStruct;

typedef struct FlashDataCommand				//��������� ���������� ����� ��� ������ ������ ������
{
	uint32 MarkerStart;	//������ ������ ��������� ���������� ������

	union Command	//��������� ����� � ������ 2
	{
		uint32 sw;
		struct
		{
			uint32 ReadFlash	:1;		//������� �� ������ ����� �� Flash ������
			uint32 ReadInfo		:1;		//������ ���������� � ������
			uint32 Format 		:1;		//������� �� �������������� FLASH ������
			uint32 Delete		:1;		//������� �� ������� Flash �� ������
			uint32 ReadPaketDDR	:1;		//��������� �� Ethernet ��������� ����� ������
			uint32 Res			:26;
			uint32 NumTarget	:1;		//����� ���� � ������� ��������
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
	uint32 MarkerStop;	//������ ����� ��������� ���������� ������
	uint32 Xor;
} FlashDataCommand;



typedef struct DataFileTitle				//��������� ��������� ����� ����������
{
	uint32 MarkerStart;	//������ ������ ��������� ���������
	uint16 NumDev;							//����� �������
	uint16 NumCycle;						//����� �������� ����� ������ ������
	uint16 NumPaket;						//���������� ������� � �����
	uint16 Res1;							//��������� ����
	uint32 SizeStructTitle;					//������ ��������� ��������� � ������
	uint32* Adrr;							//����� ������� ���������� �� �������� �������
} DataFileTitle;

typedef struct DataFileFrame
{
	uint32 MarkerStart;	//������ ������ ��������� ������
	uint32 OffsetNextFrame;					//�������� ���������� ������ �� ������ ��������� ���������

	uint16 NumPaketOEDTAT1;					//����� ������ ���-TA ��� ���� 1
	uint16 NumPaketTAOEDT1;					//����� ������ TA-��� ��� ���� 1
	uint16 NumPaketOEDTAT2;					//����� ������ ���-TA ��� ���� 2
	uint16 NumPaketTAOEDT2;					//����� ������ TA-��� ��� ���� 2
	uint16 NumCycle;						//����� �������� ����� ������ ������
	uint16 SizePaketOEDTA;					//������ ������ ���-TA
	uint16 SizePaketTAOED;					//������ ������ TA-OED
	uint16 CoordX1;							//���������� ���� ������� ������ 1 �� q
	uint16 CoordY1;							//���������� ���� ������� ������ 1 �� fi
	uint16 CoordX2;							//���������� ���� ������� ������ 2 �� q
	uint16 CoordY2;							//���������� ���� ������� ������ 2 �� fi
	uint16 Reserv1;

	union KadrCh1	//��������� ����� � ������ 1
	{
		uint32 sw;
		struct
		{
			uint32 SizeY1		:10;	//������ ����� �� q
			uint32 SizeX1		:10;	//������ ����� �� fi
			uint32 SUMM 		:12;	//������� ������� ������������
		} s;
	} KadrCh1;

	union KadrCh2	//��������� ����� � ������ 2
	{
		uint32 sw;
		struct
		{
			uint32 SizeY2		:10;	//������ ����� �� q
			uint32 SizeX2		:10;	//������ ����� �� fi
			uint32 SUMM 		:12;		//������� ������� ������������
		} s;
	} KadrCh2;

}DataFileFrame;

uint8 ReadIdDFlash(void);
void formatflash(uint32 metka);
void ReadFramParamData(FramPuskStruct * fps,uint16 Target);
void WriteFramParamData(FramPuskStruct * fps,uint32 SizePusk,uint16 Target);
uint32 XorChekSUMCalc32(uint32 *Buff, uint32 Size);
uint16 MyMemCPY(void *_s1, const void *_s2, uint32 size);
void FillDataTitle1(DataFileTitle*,uint16 Paket,uint32 SizeFrame, uint16 Target);	//������� ���������� ��������� ��������� ����� ����������
uint32 FillDataFrame1(DataFileFrame*,uint16 Frame,uint16 Param,uint16 Target);	//������� ���������� ����� � �������
#endif /* FLASHDATA_H_ */
