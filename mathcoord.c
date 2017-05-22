/*
 * mathcoord.c
 *
 *  Created on: 02.07.2015
 *      Author: Admin
 */


#include <stdio.h>
#include <math.h>


#include <ti/ndk/inc/netmain.h>
#include "ti/platform/resource_mgr.h"


#include "type.h"
#include "mathcoord.h"
#include "PaketOEDTAEth.h"
#include "BattleJob.h"

extern TATOOED ta1;						//��������� ������ �� ���� 1
extern OEDTOTA oed1;					//��������� ������ ��� ���� 1
extern TATOOED ta2;						//��������� ������ �� ���� 2
extern OEDTOTA oed2;					//��������� ������ ��� ���� 2
extern BattleParam battle;				//������ ��������� ���������� ����������
extern BattleParam2 battle2;				//������ ��������� ���������� ���������� ���� 2
extern BattleMathT1K1 battlemathT1k1;
extern BattleMathT1K2 battlemathT1k2;
extern BattleMathT2K1 battlemathT2k1;
extern BattleMathT2K2 battlemathT2k2;

double ConvertPixelToAng(uint16 pixel)
{
	double AngSec=0;
	return AngSec;

}
int16	ConvertAngToPixel(double Ang,double focus)
{
	return round(((tan (((Ang/3600)*PI)/180)*focus)/PIXEL)*1000);
}

void NormalizePixelT1()			//������������� ���������� � ��������� ������� ��� 4 ��� 8
{								//� ���������� �� ������������

	if(battle.ControlState.s.SumChan1T1)	//���� ��������� �� �������� ������ 8 �� ���������� �
	{
		battle.Target1CoordX1=round(((double)battle.Target1CoordX1 + (double)battle.VizirChan1_X-((double)battle.Target1SizeX1/2))/8);
		battle.Target1CoordX1=battle.Target1CoordX1<<3;
		battle.Target1CoordY1=battle.Target1CoordY1+(double)battle.VizirChan1_Y-battle.Target1SizeY1/2;
	}
	else									//����� ������� ������ 4
	{
		battle.Target1CoordX1=round(((double)battle.Target1CoordX1 + (double)battle.VizirChan1_X-((double)battle.Target1SizeX1/2))/4);
		battle.Target1CoordX1=battle.Target1CoordX1<<2;
		battle.Target1CoordY1=battle.Target1CoordY1+(double)battle.VizirChan1_Y-battle.Target1SizeY1/2;
	}

	if(battle.ControlState.s.SumChan2T1)	//���� ��������� �� �������� ������ 8 �� ���������� �
	{
		battle.Target1CoordX2=round(((double)battle.Target1CoordX2 + (double)battle.VizirChan2_X-((double)battle.Target1SizeX2/2))/8);
		battle.Target1CoordX2=battle.Target1CoordX2<<3;
		battle.Target1CoordY2=battle.Target1CoordY2+(double)battle.VizirChan2_Y-battle.Target1SizeY2/2;
	}
	else									//����� ������� ������ 4
	{
		battle.Target1CoordX2=round(((double)battle.Target1CoordX2 + (double)battle.VizirChan2_X-((double)battle.Target1SizeX2/2))/4);
		battle.Target1CoordX2=battle.Target1CoordX2<<2;
		battle.Target1CoordY2=battle.Target1CoordY2+(double)battle.VizirChan2_Y-battle.Target1SizeY2/2;
	}

	//���������� ����� �� ��������� �������. 2046�1088
	if((battle.Target1CoordX1+battle.Target1SizeX1)>=2048)	//��������� �� � ������� ����� 1
	{
		battle.Target1CoordX1-=(battle.Target1CoordX1+battle.Target1SizeX1)-2048;
	}
	else if(battle.Target1CoordX1<0)	battle.Target1CoordX1=0;
	if((battle.Target1CoordX2+battle.Target1SizeX2)>=2048)	//��������� �� � ������� ����� 2
	{
		battle.Target1CoordX2-=(battle.Target1CoordX2+battle.Target1SizeX2)-2048;
	}
	else if(battle.Target1CoordX2<0)	battle.Target1CoordX2=0;
	if((battle.Target1CoordY1+battle.Target1SizeY1)>=1088)	//��������� �� y ������� ����� 1
	{
		battle.Target1CoordY1-=(battle.Target1CoordY1+battle.Target1SizeY1)-1088;
	}
	else if(battle.Target1CoordY1<0) battle.Target1CoordY1=0;
	if((battle.Target1CoordY2+battle.Target1SizeY2)>=1088)	//��������� �� y ������� ����� 2
	{
		battle.Target1CoordY2-=(battle.Target1CoordY2+battle.Target1SizeY2)-1088;
	}
	else if(battle.Target1CoordY2<0) battle.Target1CoordY2=0;

}
void NormalizePixelT2()			//������������� ���������� � ��������� ������� ��� 4 ��� 8
{								//� ���������� �� ������������

	if(battle2.ControlState2.s.SumChan1T2)	//���� ��������� �� �������� ������ 8 �� ���������� �
	{
		battle2.Target2CoordX1=round(((double)battle2.Target2CoordX1 + (double)battle2.VizirChan1_X-((double)battle2.Target2SizeX1/2))/8);
		battle2.Target2CoordX1=battle2.Target2CoordX1<<3;
		battle2.Target2CoordY1=battle2.Target2CoordY1+(double)battle2.VizirChan1_Y-battle2.Target2SizeY1/2;
	}
	else									//����� ������� ������ 4
	{
		battle2.Target2CoordX1=round(((double)battle2.Target2CoordX1 + (double)battle2.VizirChan1_X-((double)battle2.Target2SizeX1/2))/4);
		battle2.Target2CoordX1=battle2.Target2CoordX1<<2;
		battle2.Target2CoordY1=battle2.Target2CoordY1+(double)battle2.VizirChan1_Y-battle2.Target2SizeY1/2;
	}

	if(battle2.ControlState2.s.SumChan2T2)	//���� ��������� �� �������� ������ 8 �� ���������� �
	{
		battle2.Target2CoordX2=round(((double)battle2.Target2CoordX2 + (double)battle2.VizirChan2_X-((double)battle2.Target2SizeX2/2))/8);
		battle2.Target2CoordX2=battle2.Target2CoordX2<<3;
		battle2.Target2CoordY2=battle2.Target2CoordY2+(double)battle2.VizirChan2_Y-battle2.Target2SizeY2/2;
	}
	else									//����� ������� ������ 4
	{
		battle2.Target2CoordX2=round(((double)battle2.Target2CoordX2 + (double)battle2.VizirChan2_X-((double)battle2.Target2SizeX2/2))/4);
		battle2.Target2CoordX2=battle2.Target2CoordX2<<2;
		battle2.Target2CoordY2=battle2.Target2CoordY2+(double)battle2.VizirChan2_Y-battle2.Target2SizeY2/2;
	}

	//���������� ����� �� ��������� �������. 2046�1088
	if((battle2.Target2CoordX1+battle2.Target2SizeX1)>=2048)	//��������� �� � ������� ����� 1
	{
		battle2.Target2CoordX1-=(battle2.Target2CoordX1+battle2.Target2SizeX1)-2048;
	}
	else if(battle2.Target2CoordX1<0)	battle2.Target2CoordX1=0;
	if((battle2.Target2CoordX2+battle2.Target2SizeX2)>=2048)	//��������� �� � ������� ����� 2
	{
		battle2.Target2CoordX2-=(battle2.Target2CoordX2+battle2.Target2SizeX2)-2048;
	}
	else if(battle2.Target2CoordX2<0)	battle2.Target2CoordX2=0;
	if((battle2.Target2CoordY1+battle2.Target2SizeY1)>=1088)	//��������� �� y ������� ����� 1
	{
		battle2.Target2CoordY1-=(battle2.Target2CoordY1+battle2.Target2SizeY1)-1088;
	}
	else if(battle2.Target2CoordY1<0) battle2.Target2CoordY1=0;
	if((battle2.Target2CoordY2+battle2.Target2SizeY2)>=1088)	//��������� �� y ������� ����� 2
	{
		battle2.Target2CoordY2-=(battle2.Target2CoordY2+battle2.Target2SizeY2)-1088;
	}
	else if(battle2.Target2CoordY2<0) battle2.Target2CoordY2=0;
}

void SetFieldAnalysis(uint16 Target)
{

	if(Target==0)			//�������� �� ���� 1
	{
		if((oed1.SignalOED1.s.IndLO1==0)||(ta1.CommandTA1.s.SoprLO1==0))
		{
			battle.Target1CoordY1 = ConvertAngToPixel(ta1.qk1,battle.FokusK1);			//������� ��������� � ������ 1 � 2 ��������������
			battle.Target1CoordX1 = ConvertAngToPixel(ta1.fk1,battle.FokusK1);			//��������� ������� ��������� �� 90 �������� ��� ������ ������ �� �������
			battle.Target1CoordX1*=-1;
		}
		else
		{
			battle.Target1CoordY1 = ConvertAngToPixel(battlemathT1k1.Target1CoordAngX1,battle.FokusK1);			//������� ��������� � ������ 1 � 2 ��������������
			battle.Target1CoordX1 = ConvertAngToPixel(battlemathT1k1.Target1CoordAngY1,battle.FokusK1);			//��������� ������� ��������� �� 90 �������� ��� ������ ������ �� �������
			battle.Target1CoordX1*=-1;
		}

		if((oed1.SignalOED1.s.IndLO2==0)||(ta1.CommandTA1.s.SoprLO2==0))
		{
			battle.Target1CoordY2 = ConvertAngToPixel(ta1.qk2,battle.FokusK2);
			battle.Target1CoordX2 = ConvertAngToPixel(ta1.fk2,battle.FokusK2);
			battle.Target1CoordX2*=-1;
		}
		else
		{
			battle.Target1CoordY2 = ConvertAngToPixel((battlemathT1k2.Target1CoordAngX2>>2),battle.FokusK2);
			battle.Target1CoordX2 = ConvertAngToPixel((battlemathT1k2.Target1CoordAngY2>>2),battle.FokusK2);
			battle.Target1CoordX2*=-1;
		}

		NormalizePixelT1();
	}
	else
	{
		if((oed2.SignalOED1.s.IndLO1==0)||(ta2.CommandTA1.s.SoprLO1==0))
		{
			battle2.Target2CoordY1 = ConvertAngToPixel(ta2.qk1,battle2.FokusK1);			//������� ��������� � ������ 1 � 2 ��������������
			battle2.Target2CoordX1 = ConvertAngToPixel(ta2.fk1,battle2.FokusK1);			//��������� ������� ��������� �� 90 �������� ��� ������ ������ �� �������
			battle2.Target2CoordX1*=-1;
		}
		else
		{
			battle2.Target2CoordY1 = ConvertAngToPixel(battlemathT2k1.Target2CoordAngX1,battle2.FokusK1);			//������� ��������� � ������ 1 � 2 ��������������
			battle2.Target2CoordX1 = ConvertAngToPixel(battlemathT2k1.Target2CoordAngY1,battle2.FokusK1);			//��������� ������� ��������� �� 90 �������� ��� ������ ������ �� �������
			battle2.Target2CoordX1*=-1;
		}

		if((oed2.SignalOED1.s.IndLO2==0)||(ta2.CommandTA1.s.SoprLO2==0))
		{
			battle2.Target2CoordY2 = ConvertAngToPixel(ta2.qk2,battle2.FokusK2);
			battle2.Target2CoordX2 = ConvertAngToPixel(ta2.fk2,battle2.FokusK2);
			battle2.Target2CoordX2*=-1;
		}
		else
		{
			battle2.Target2CoordY2 = ConvertAngToPixel((battlemathT2k2.Target2CoordAngX2>>2),battle2.FokusK2);
			battle2.Target2CoordX2 = ConvertAngToPixel((battlemathT2k2.Target2CoordAngY2>>2),battle2.FokusK2);
			battle2.Target2CoordX2*=-1;
		}

		NormalizePixelT2();
	}
}

