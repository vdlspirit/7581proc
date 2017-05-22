/*
 * cmosis.h
 *
 *  Created on: 28.08.2014
 *      Author: Admin
 */

#ifndef CMOSIS_H_
#define CMOSIS_H_

//�������� ������� �������� �� SPI
#define SENSORTYPE		125			//ID �������

#define TEMPLO			126			//������� ����������� ������� ����
#define TEMPHI			127			//������� ����������� ������� ����

#define REG73			73			//������� ����������� �������� FOT

#define PLLCONFIG1		114			//�������� ��������� PLL
#define PLLCONFIG2		116
#define PLLLOAD			117

#define VBLACKSUN		123

#define BITMODE			111
#define ADCRES			112
#define ADCGAIN			103

#define OFFSETLO		100
#define OFFSETHI		101

#define PGA				102
#define PGA2			121
#define	TDIG1			108			//����� ������ ���������������� ������������ �������
#define	TDIG2			109

#define	DUMMY			118

#define VLOW2			89
#define VLOW3			90

#define CHENEN1			80
#define CHENEN2			81
#define CHENEN3			82

#define ICOL			84
#define ICOLPRECH		85
#define IAMP			87

#define VTFL1			88
#define VRESLOW			91
#define VPRECHARGE		94
#define VREF			95
#define VRAMP1			98
#define VRAMP2			99

#define TRAINPATTERNLO	78
#define TRAINPATTERNHI	79

#define CALIB			77

#define OUTPUTMODE		72

#define NUMBERFRAMESLO	70
#define NUMBERFRAMESHI	71

#define LVDSREC			74

#define COLOR			39
#define IMAGEFLIPP		40
#define EXPEXT			41
#define EXPTIME1		42
#define EXPTIME2		43
#define EXPTIME3		44
#define EXPSTEP			45

#define NUMBERLINESLO	1
#define NUMBERLINESHI	2

#define STARTLINELO		3
#define STARTLINEHI		4


typedef struct CMOSREG
{
			Uint8 NumberLinesLO;	//���������� ����������� ����� � �������
			Uint8 NumberLinesHI;

			Uint8 StartLinesLO;		//������ ����������� ������
			Uint8 StartLinesHI;

			Uint8 Exp_time1;		//����� ���������� 3 �����
			Uint8 Exp_time2;
			Uint8 Exp_time3;

			Uint8 Reg_73;			//������� 73 �� ��������� 10 ��� 10��� - (7,6)
			Uint8 ADC_GAIN;			//����������� �������� ����.
			Uint8 OFFSET1;			//�������� ������ �������
			Uint8 OFFSET2;			//�������� ������ �������

} CMOS;

void cmos_write_reg();			//������ ���������� � �������� CMOS
void cmos_write_sizekadr();		//������ �������� ���� � �������� CMOS � Xilinx
void cmosxilinx_write_kadrpos();

#endif /* CMOSIS_H_ */
