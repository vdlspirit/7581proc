#include "type.h"

#ifndef FLASH_PROGRAMM_H_
#define FLASH_PROGRAMM_H_

#define BASE_CE0_ADDR		0x70000000
#define BASE_CE1_ADDR		0x74000000
#define BASE_CE2_ADDR		0x78000000
#define BASE_CE3_ADDR		0x7c000000

#define CONTENTS_OF(addr) \
        (*((volatile uint16 *)(addr)))

#define REG_WRITE(addr,val) \
        (CONTENTS_OF(addr) = (val))

#define REG_READ(addr) \
        (CONTENTS_OF(addr))

//��������� ������
#define FLASH_PROG		BASE_CE3_ADDR
#define FRAM_PROG		BASE_CE3_ADDR


#define FLASH_TEST		*(volatile unsigned short*)(BASE_CE3_ADDR+0)
#define FLASH_555H		*(volatile unsigned short*)(BASE_CE3_ADDR+0xAAA)
#define FLASH_2AAH		*(volatile unsigned short*)(BASE_CE3_ADDR+0x554)

#define FRAM_PARAM_OED			FRAM_PROG
#define FRAM_PARAM_DIAGN		(FRAM_PARAM_OED+SIZEPAKETUDPFLASHREAD)
#define FRAM_PARAM_OUT			(FRAM_PARAM_DIAGN+SIZEPAKETUDPFLASHREAD)
#define FRAM_PARAM_Struct_NAND1	(FRAM_PARAM_OUT+SIZEPAKETUDPFLASHREAD)
#define FRAM_PARAM_Struct_NAND2	(FRAM_PARAM_Struct_NAND1+SIZEPAKETUDPFLASHREAD)

#define ADDRRAMJOBFLASHPROGRAMM	0x82000000	//����� ������ ������� ����� ��������������
											//� �������� �������� � ��������� ������

#define ADDRRAMJOBFLASHPROGRAMMT2	0x91A00000	//����� ������ ������� ����� ��������������
											//� �������� �������� � ��������� ������ ��� ���� 2

#define MAXSIZEPROGRAMCPU	1050000	//������������ ������ ��������� ��� ������������ ����������
#define	MAXSIZEPROGRAMXVPU	5242880 //������������ ������ ��������� ��� Xilinx VPU
#define	MAXSIZEPROGRAMXPVP1	2097152 //������������ ������ ��������� ��� Xilinx PVP1
#define	MAXSIZEPROGRAMXPVP2	2097152 //������������ ������ ��������� ��� Xilinx PVP2
#define	MAXSIZEPROGRAMFRAM	1400    //������������ ������ flash FRAM

#define SIZEPAKETUDPFLASHREAD	1400	//������ ������ UDP ��� ���������������� FLASH
#define SIZEPAKETUDPFLASHWRITE	1400	//������ ������ UDP ��� ���������������� FLASH

#define TESTCHANNEL		0x0101		//�������� ������ �������� ������
#define	REQUESTINFO		0x0301		//������ ������ �� ����������
#define	REQUESTDIAG		0x0501		//������ ������ � �����������

#define	NUMPROGRAMM		0x0102		//������ ������ � ���������� ��������
#define READDESCRIPTOR	0x0202		//������ �����������
#define READIDENTIFIER	0x0302		//������ ��������������

#define READDATA		0x0402		//������ ������
#define STARTPROGRAMM	0x0502		//������ �������� ��������
#define WRITEDATA		0x0602		//������ ������
#define FINISHPROGRAMM	0x0702		//���������� �������� ��������
#define ERRASEPROGRAMM	0x0802		//���������� �������� ��������

typedef struct PaketFlashPC
{
	uint8	Title;					//������� ������ ������
	uint8 	Address;				//����� ���������� ����������
	uint16  SizePaket;				//������ ������ � ������� � ����������
	uint16 	CheckSumTitle;			//����������� ����� ���������

	uint8 	DataPak[1406];			//������ ����� ������ � ����������� ������ ������
} PcFlashPaket;

typedef struct PaketFlashOED
{
	uint8	Title;					//������� ������ ������
	uint8 	Address;				//����� ���������� ����������
	uint16  SizePaket;				//������ ������ � ������� � ����������
	uint16 	CheckSumTitle;			//����������� ����� ���������

	uint8	REQUEST;				//�����

	uint8 	DataPak[1406];			//������ ����� ������ � ����������� ������ ������
} OedFlashPaket;

typedef struct Descriptor
{
	char	NameProg[12];			//�������� �������� �� ����� 30 ��������
	uint32 	SizeBlockWrite;			//������ ����� ������ ��� ������
	uint32 	SizeBlockRead;			//������ ����� ������ ��� ������
	uint32  MaxSizeProg;			//������������ ������ ��������

	union WordD1{
					uint32 sw;
					struct {
						uint32 TimeOutWrite 	:16;	//������� ��� ������ ������
						uint32 TimeOutProg 		:16;	//������� ��� ����������������
					} s;
				} WordD1;
	union WordD2{
					uint32 sw;
					struct {
						uint32 Restart		 	:8;		//������������ ����������
						uint32 VERPO	 		:24;	//������� ������ ��������
					} s;
				} WordD2;
	uint8	VERPOH;
} Des;

uint16 ReadStatus(uint32 Bank);
void RWConfig(void);
uint16 ErrasePuskFlash(uint16 NumPusk, uint32 Adrr, uint32 Size, uint16 Target);
uint16 WritePuskFlash(uint16 NumPusk, uint32 Adrr, uint32 Size,uint16 Target);
uint16 ReadPuskFlash(uint16 NumPusk, uint32 Adrr, uint32 Size,uint16 Target);
uint16 SavePuskInDataFlash(uint16 NumPusk,uint32 Size,uint16 Target);
uint16 ReadPuskOfDataFlash(uint16 NumPusk, uint32 Size,uint16 Target);
uint32 GetAddrPusk(uint16 NumPusk,uint16 Target);

void MemCPYWithDelay(uint16* BuffOut,uint16* BuffIn,uint16 size);
UInt8 flash_prog(UInt16* DataMass, UInt32 sizebyte,UInt32 flashadd);
int Get_Descriptors(uint8* Buff,uint8 NumDes);
uint16 Get_XorCheksum(uint8* Buff,uint16 SizeByte);
uint16 ReadDataFlash(UInt16* DataMass,uint8 NumDes,uint8 Flag);
uint8 WriteDatatoDDR(UInt8* DataMass,UInt8* DataMassDDR,uint8 NumDes);
void ProgrammFlash(void *DataMass,uint8 NumDes,uint8 Flag);
void CreateTestDataProgramm(uint32 Size);

#endif /* FLASH_PROGRAMM_H_ */
