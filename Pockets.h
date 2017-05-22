
#ifndef _POCKETS_H_
#define _POCKETS_H_

//"Pockets.h"
#include "atypes.h"

#define TABLCOUNT 2300
#define SIZEOFTABL (((TABLCOUNT)*4)+12)
#define SIZEOFTABL32 ((SIZEOFTABL)>>2)
#define TOTAL_FLASH_SIZE_DIAGN 1048576	// ������ ������ �� ����������� 1 ��
#define POCKETPARAMSIZE 20	// ������ ���������� truncate - ��������� �� ��������� ��������

extern Uint32 nPocket;
typedef struct POCKETBEGIN
{
	Uint8 typeOfStruct;			// ��� ���������
	Uint8 rez1;					// ������
	Uint16 N;				// ���������� �������
	Uint32 SizeStruct;	// ������ ���� ������
	Uint32 next[TABLCOUNT];				// ��������
	Uint32 KC;				// ����������� �����
} POCKETBEGIN ,*PPOCKETBEGIN;

typedef struct POINTINT 
{  
	short int   x;				// ���������� X
	short int   y;				// ���������� Y
} POINTINT;

typedef struct OEDTOCVS 
{
	short int CL1;		// �������
	short int CL2;		// ������ ������ 1 �� ���������� q
	short int CL3;		// ������ ������ 1 �� ���������� fi
	short int CL4;		// ����������� ������ 1 �� ���������� q
	short int CL5;		// ����������� ������ 1 �� ���������� fi
	short int CL6;		// ������ ������ 2 �� ���������� q
	short int CL7;		// ������ ������ 2 �� ���������� fi
	short int CL8;		// ����������� ������ 2 �� ���������� q
	short int CL9;		// ����������� ������ 2 �� ���������� fi
	short int CL10;		// ��������� �� �������
	Uint8 CL11a;					// ������� ������ ��������� � ������ 1 �� ����������  q
	Uint8 CL11b;					// ������� ������ ��������� � ������ 1 �� ����������  fi
	Uint8 CL12a;					// ������� ������ ��������� � ������ 2 �� ����������  q
	Uint8 CL12b;					// ������� ������ ��������� � ������ 2 �� ����������  fi
	short int CL13;		// ������� ������� � ������ 1
	short int CL14;		// ������� ������� � ������ 2
	short int CL15;		// ������� ������� � ������������ ������
	short int CL16;		// ������ ���� ������� � ������ 1
	short int CL17;		// ������ ���� ������� � ������ 2
	short int CL18;		// ��������� ������/����� � ������ 1
	short int CL19;		// ��������� ������/����� � ������ 2
	short int CL20;		// ��������� ������/����� � ������������ ������
	short int CL21;		// ����� ��
	short int CL22;		// ������� ��������� ������1
	short int CL23;		// ����� ���������� � ������ 1
	short int CL24;		// ������� ����������� ���
	
}OEDTOCVS ;

typedef struct CVSTOOED 
{
	unsigned short int CL1;			// �������
	short int CL2;					// ��������� � ������ 1 �� ���������� q
	short int CL3;					// ��������� � ������ 1 �� ���������� fi
	short int CL4;					// ��������� � ������ 2 �� ���������� q
	short int CL5;					// ��������� � ������ 2 �� ���������� fi
	short int CL6;					// �������� ��� �� ���������� q
	short int CL7;					// �������� ��� �� ���������� fi
	unsigned short int CL8;			// ����������� ���������
	signed short int CL9;			// ��� �� ���������� q
	signed short int CL10;			// ��� �� ���������� fi
	unsigned short int CL11;		// ��������������� �������
	unsigned short int CL12;		// ��������������� �������
	short int CL13;					// �������� ��� �� ���������� q
	short int CL14;					// �������� ��� �� ���������� fi
}CVSTOOED;

typedef struct TEXTDATA 
{
	OEDTOCVS oedToCvs;				// ���������� �� ��� � ���
	CVSTOOED  cvsToOed;				// ���������� �� ���� ��� 
}TEXTDATA;

// ��������� ��� ���������
typedef struct POCKETINFOTRUNCATED
{
//	Uint8 o2cSize;
//	Uint8 c2oSize;
	POINTINT XY[2];				// ������� ������������
	Uint8 dx1;					// �� ��������� ������
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
	Uint32 BeginningOfData;		// ������� ������ ������
	Uint32 NextPocket;			// �������� �� ��������� �����
//	Uint32 Counter;				// ����� ������
	Uint16 Counter;				// ����� ������
	Uint16 rez2;
	POCKETINFOTRUNCATED PocketTruncatedInfo;
/*	POINTINT XY[2];				// ������� ������������
	TEXTDATA textData;			// ��������� ����������
	Uint8* AdditionData;		// �������������� ���������� ����������
*/	
//	Uint8* graphicInfo;			// ����������� ������
	Uint32 EndOfData;				// ������� ����� ������
	Uint32 KC;				// ����������� �����
} POCKETINFO, *PPOCKETINFO;

#endif /* _POCKETS_H_ */
