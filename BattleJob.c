/* SRIO Driver Include File. */
#include <ti/drv/srio/srio_drv.h>
#include <ti/drv/srio/srio_osal.h>
#include <ti/sysbios/family/c66/Cache.h>
#include <ti/sysbios/knl/Task.h>
#include <ti/sysbios/knl/Event.h>
#include <ti/sysbios/BIOS.h>

#include <ti/ndk/inc/netmain.h>

#include "BattleJob.h"
#include "PaketOEDTAEth.h"
#include "gpio.h"
#include "srio.h"
#include "cmosis.h"
#include "flash_programm.h"
#include "flashdata.h"

#pragma DATA_ALIGN(ta2,256)
#pragma DATA_ALIGN(oed2,256)
#pragma DATA_ALIGN(DStruct,128)
#pragma DATA_SECTION(ta2, ".DataBuff")
#pragma DATA_SECTION(DStruct, ".DataBuff")
#pragma DATA_SECTION(oed2, ".DataBuff")

extern volatile unsigned int cregister TSCL;

extern BattleMathT1K1 battlemathT1k1;
extern BattleMathT1K2 battlemathT1k2;
extern BattleMathT2K1 battlemathT2k1;
extern BattleMathT2K2 battlemathT2k2;
extern BattleParam battle;			//������ ��������� ���������� ����������
extern BattleParam2 battle2;				//������ ��������� ���������� ���������� ���� 2
extern Srio_SockHandle srioSocket;	//����������� ����� srio
extern TATOOED ta1;						//��������� ������ �� ���� 1
extern OEDTOTA oed1;					//��������� ������ ��� ���� 1
TATOOED ta2;							//��������� ������ �� ���� 2
OEDTOTA oed2;							//��������� ������ ��� ���� 2
extern unsigned int BattleTimerSinx1;
extern unsigned int BattleTimerSinx2;

DiagStruct DStruct;

extern CMOS cmos1config;	//�������� ���������� 1 ��������
extern CMOS cmos2config;	//�������� ���������� 2 ��������

extern VideoTechParam videotechparam;
extern VideoTechPacket videotechpacket;

extern VisualKommandPaket vkp;				//��������� ������ �� �������������
extern VisualStatusPaket vsp;				//��������� ��������� �����

extern uint8 BuffOED1[SizeBuffOED];			//������ �� �������� ������ ���1
extern uint8 BuffTA1[SizeBuffTA];				//������ �� ����� ������ ��1
extern uint8 BuffOED2[SizeBuffOED];			//������ �� �������� ������ ���2
extern uint8 BuffTA2[SizeBuffTA];				//������ �� ����� ������ ��2
extern uint32 BuffVideoParamPacket[8];			//������ ���������� ����� ������

extern Uint8 GlobalVideoBuff[0x800000+sizeof(VisualKadrPaket)];

extern unsigned int sinx1count;
extern unsigned int sinx2count;
extern unsigned int finishk1count;
extern unsigned int finishk2count;

extern unsigned short 	 sinx1;
extern unsigned short	 sinx2;
extern unsigned short	 blinksinx1;
extern unsigned short	 blinksinx2;

//��������� ������� ������� ��������
extern Event_Handle   eventclkhtarget1;
extern Event_Handle   eventclkhtarget2;

extern double PIXELSECCHAN1;
extern double PIXELSECCHAN2;

extern SRIOPACKET srioUpac;
extern volatile UInt32*     isInitialized;

extern void StartStopEth(int ss);
extern void gpioStrobBlink();
extern unsigned int GiveMePeriod(unsigned short chennal);
extern unsigned int GiveMeDeltaSinx();
extern void NewVideoTechPaket();
extern double ConvertPixelToAng(uint16 pixel);
extern uint16 ConvertAngToPixel(double Ang);
extern void SetFieldAnalysis(uint16);

extern uint32 GlobalBattleCount;


void BattleFunc(int param1,int param2)
{
	if((param1==1)&&(param2==1))		//���������� ���������������
	{
		if(srioSocket==NULL)			//����� srio ��� �� ��������
			return;
		if(battle.STAGE==sDIAG1)
		{
			DStruct.Target1Err=0;
			DStruct.Target1Ok=0;
			DStruct.Target2Err=0;
			DStruct.Target2Ok=0;

			CreateDistanseDelayDiag(0);	//����������� ���������� �� 0 ������

			battle.Target1SizeX1=256;	//���� 1
			battle.Target1SizeY1=256;
			battle.Target1SizeX2=256;
			battle.Target1SizeY2=256;

			battle2.Target2SizeX1=256;	//���� 2
			battle2.Target2SizeY1=256;
			battle2.Target2SizeX2=256;
			battle2.Target2SizeY2=256;

			battle.ControlState.s.SumChan1T1=1;	//�������� ������������
			battle.ControlState.s.SumChan2T1=1;	//�������� ������������
			battle2.ControlState2.s.SumChan1T2=1;	//�������� ������������
			battle2.ControlState2.s.SumChan2T2=1;	//�������� ������������

			Cache_wb((void *) &battle, sizeof(BattleParam), Cache_Type_L1D, TRUE);
			Cache_wb((void *) &battle2, sizeof(BattleParam2), Cache_Type_L1D, TRUE);
			//�������� ������� ��������� ���� ������� ��������� �� �������� ���
			//�������� 256x256 �������� ���������. ������������ ����� ��������� �����������
			spi_cmos_reset_enable();		//reset ������
			SetBattleSizaAndPosition((battle.VizirChan1_X-(battle.Target1SizeX1/2)),
					(battle.VizirChan1_Y-(battle.Target1SizeY1/2)),(battle.VizirChan2_X-(battle.Target1SizeX2/2)),
					(battle.VizirChan2_Y-(battle.Target1SizeY2/2)),0);
			//Cache_wb((void *) &isInitialized[1], 128, Cache_Type_L1D, TRUE);

			battle.STAGE = sDIAG2;
		}
		else if(battle.STAGE==sDIAG2)
			{
				battle.STAGE = sDIAG3;
				spi_cmos_reset_enable();		//reset ������
			}
			else if(battle.STAGE==sDIAG3)
			{
				Cache_inv((void *) &oed1, sizeof(OEDTOTA), Cache_Type_L1D, TRUE);
				Cache_inv((void *) &oed2, sizeof(OEDTOTA), Cache_Type_L1D, TRUE);
				Cache_inv((void *) &DStruct, sizeof(DiagStruct), Cache_Type_L1D, TRUE);

				oed1.SignalOED1.s.Healthy = 1;		//��������
				oed2.SignalOED1.s.Healthy = 1;		//��������
				DStruct.Target1Ok=1;
				DStruct.Target2Ok=1;

				Cache_wb((void *) &oed1, sizeof(OEDTOTA), Cache_Type_L1D, TRUE);
				Cache_wb((void *) &oed2, sizeof(OEDTOTA), Cache_Type_L1D, TRUE);
				Cache_inv((void *) &oed1, sizeof(OEDTOTA), Cache_Type_L1D, TRUE);
				Cache_inv((void *) &oed2, sizeof(OEDTOTA), Cache_Type_L1D, TRUE);

				battle.STAGE=sSTANDBY;

				battle.Target1SizeX1=288;	//���� 1
				battle.Target1SizeY1=288;
				battle.Target1SizeX2=368;
				battle.Target1SizeY2=368;

				battle2.Target2SizeX1=288;	//���� 2
				battle2.Target2SizeY1=288;
				battle2.Target2SizeX2=368;
				battle2.Target2SizeY2=368;

				battle.ControlState.s.SumChan1T1=1;	//�������� ������������
				battle.ControlState.s.SumChan2T1=1;	//�������� ������������
				battle2.ControlState2.s.SumChan1T2=1;	//�������� ������������
				battle2.ControlState2.s.SumChan2T2=1;	//�������� ������������

				Cache_wb((void *) &battle, sizeof(BattleParam), Cache_Type_L1D, TRUE);
				Cache_wb((void *) &battle2, sizeof(BattleParam2), Cache_Type_L1D, TRUE);

				spi_cmos_reset_enable();		//reset ������
				//����������� ������ ����� ��� ��������� ���� ��� ������ 1 � 2
				SetBattleSizaAndPosition((battle.VizirChan1_X-(battle.Target1SizeX1/2)),
						(battle.VizirChan1_Y-(battle.Target1SizeY1/2)),(battle.VizirChan2_X-(battle.Target1SizeX2/2)),
						(battle.VizirChan2_Y-(battle.Target1SizeY2/2)),0);
	     		//��������� ������ �� SRIO � ���������� ��������
	     		isInitialized[0] = 3;
	     		Cache_wb((void *) &isInitialized[0], 4, Cache_Type_L1D, TRUE);
			}
		Cache_wb((void *) &battle, sizeof(BattleParam), Cache_Type_L1D, TRUE);
		Cache_wb((void *) &battle2, sizeof(BattleParam2), Cache_Type_L1D, TRUE);
	}
}

void SetBattleSizaAndPosition(uint16 CoordX1Chn1, uint16 CoordY1Chn1,
		uint16 CoordX2Chn2, uint16 CoordY2Chn2, uint16 Target)	//��������� ��������� � �������� �����
{
	srioUpac.VideoRamK1=LOADKADR1MEM;
	srioUpac.VideoRamK2=LOADKADR2MEM;

	if(Target==0)						//���� 1
	{
		vsp.Data1.s.CoordX1=vkp.DataWord3.s.CoordX1=CoordX1Chn1;
		vsp.Data1.s.CoordY1=vkp.DataWord3.s.CoordY1=CoordY1Chn1;

		vsp.Data2.s.CoordX2=vkp.DataWord4.s.CoordX2=CoordX2Chn2;
		vsp.Data2.s.CoordY2=vkp.DataWord4.s.CoordY2=CoordY2Chn2;

		vsp.Data3.s.SizeX1=vkp.DataWord1.s.SizeX1=battle.Target1SizeX1;
		vsp.Data3.s.SizeY1=vkp.DataWord1.s.SizeY1=battle.Target1SizeY1;
		vsp.Data4.s.SizeX2=vkp.DataWord2.s.SizeX2=battle.Target1SizeX2;
		vsp.Data4.s.SizeY2=vkp.DataWord2.s.SizeY2=battle.Target1SizeY2;


		vkp.KommandWord.s.SM=0; //�� ���������� ������������ �� Xilinx �������

		cmosxilinx_write_kadrpos();	//����������� ��������� 1024 ���� ������ �� �������

		if(battle.ControlState.s.SumChan1T1)	//������������ ������� ��� ������������ �� 2 ������ 1
		{
			//������������� ���������� ������ ��� ���. �����������
			isInitialized[18]=(vkp.DataWord1.s.SizeX1/2)|((vkp.DataWord1.s.SizeY1/2)<<16);

			srioUpac.SrioDataKamera1.s.ShiftInRow=vkp.DataWord3.s.CoordX1/8;
			srioUpac.SrioDataKamera1.s.SizeRow=(vkp.DataWord1.s.SizeX1/8)-1;
			srioUpac.SrioDataKamera1.s.SumX2=battle.ControlState.s.SumChan1T1;		//����������� �� XilinxVPU
			srioUpac.SrioDataKamera1.s.KamIm=1;
		}
		else
		{
			//������������� ���������� ������ ��� ���. �����������
			isInitialized[18]=vkp.DataWord1.s.SizeX1|(vkp.DataWord1.s.SizeY1<<16);

			srioUpac.SrioDataKamera1.s.ShiftInRow=vkp.DataWord3.s.CoordX1/4;
			srioUpac.SrioDataKamera1.s.SizeRow=(vkp.DataWord1.s.SizeX1/4)-1;
			srioUpac.SrioDataKamera1.s.SumX2=battle.ControlState.s.SumChan1T1;		//����������� �� XilinxVPU
			srioUpac.SrioDataKamera1.s.KamIm=1;
		}

		if(battle.ControlState.s.SumChan2T1)	//������������ ���������� ��� ������������ �� 2 ������ 2
		{
			//������������� ���������� ������ ��� ���. �����������
			isInitialized[19]=(vkp.DataWord2.s.SizeX2/2)|((vkp.DataWord2.s.SizeY2/2)<<16);

			srioUpac.SrioDataKamera2.s.ShiftInRow=vkp.DataWord4.s.CoordX2/8;
			srioUpac.SrioDataKamera2.s.SizeRow=(vkp.DataWord2.s.SizeX2/8)-1;
			srioUpac.SrioDataKamera2.s.SumX2=battle.ControlState.s.SumChan2T1;		//����������� �� XilinxVPU
			srioUpac.SrioDataKamera2.s.KamIm=1;
		}
		else
		{
			//������������� ���������� ������ ��� ���. �����������
			isInitialized[19]=vkp.DataWord2.s.SizeX2|(vkp.DataWord2.s.SizeY2<<16);

			srioUpac.SrioDataKamera2.s.ShiftInRow=vkp.DataWord4.s.CoordX2/4;
			srioUpac.SrioDataKamera2.s.SizeRow=(vkp.DataWord2.s.SizeX2/4)-1;
			srioUpac.SrioDataKamera2.s.SumX2=battle.ControlState.s.SumChan2T1;		//����������� �� XilinxVPU
			srioUpac.SrioDataKamera2.s.KamIm=1;
		}

		if(battle.BMODETARGET1==t1GrabChan1)
		{
			//������������ ���������� ��� ��������� �������
			cmos1config.ADC_GAIN=GAIN;
			cmos1config.Reg_73=FOTReg73;
			cmos1config.OFFSET1=OffsetChan1&0xff;
			cmos1config.OFFSET2=(OffsetChan1>>8)&0xff;

			cmos1config.Exp_time1=cmos1config.Exp_time1=1;	//����������� ����� ����������
			cmos1config.Exp_time2=cmos1config.Exp_time2=0;
			cmos1config.Exp_time3=cmos1config.Exp_time3=0;

			cmos2config.ADC_GAIN=GAIN;
			cmos2config.Reg_73=FOTReg73;
			cmos2config.OFFSET1=OffsetChan2&0xff;
			cmos2config.OFFSET2=(OffsetChan2>>8)&0xff;

			cmos2config.Exp_time1=cmos1config.Exp_time1=1;	//����������� ����� ����������
			cmos2config.Exp_time2=cmos1config.Exp_time2=0;
			cmos2config.Exp_time3=cmos1config.Exp_time3=0;

			cmos_write_reg();			//����������� �������������� ��������
		}

		cmos1config.StartLinesLO = (Uint8)(vkp.DataWord3.s.CoordY1&0x00ff);		//����� ������ � ������� �������� ����������
		cmos1config.StartLinesHI= (Uint8)((vkp.DataWord3.s.CoordY1>>8)&0xff);
		cmos1config.NumberLinesLO = (Uint8)(vkp.DataWord1.s.SizeY1&0x00ff);		//����������� ���������� �����
		cmos1config.NumberLinesHI = (Uint8)((vkp.DataWord1.s.SizeY1>>8)&0xff);

		cmos2config.StartLinesLO = (Uint8)(vkp.DataWord4.s.CoordY2&0x00ff);		//����� ������ � ������� �������� ����������
		cmos2config.StartLinesHI= (Uint8)((vkp.DataWord4.s.CoordY2>>8)&0xff);
		cmos2config.NumberLinesLO = (Uint8)(vkp.DataWord2.s.SizeY2&0x00ff);		//����������� ���������� �����
		cmos2config.NumberLinesHI = (Uint8)((vkp.DataWord2.s.SizeY2>>8)&0xff);

		cmos_write_sizekadr();		//����������� ������ ��������
	}
	else							//���� 2
	{
		vsp.Data1.s.CoordX1=vkp.DataWord3.s.CoordX1=CoordX1Chn1;
		vsp.Data1.s.CoordY1=vkp.DataWord3.s.CoordY1=CoordY1Chn1;

		vsp.Data2.s.CoordX2=vkp.DataWord4.s.CoordX2=CoordX2Chn2;
		vsp.Data2.s.CoordY2=vkp.DataWord4.s.CoordY2=CoordY2Chn2;

		vsp.Data3.s.SizeX1=vkp.DataWord1.s.SizeX1=battle2.Target2SizeX1;
		vsp.Data3.s.SizeY1=vkp.DataWord1.s.SizeY1=battle2.Target2SizeY1;
		vsp.Data4.s.SizeX2=vkp.DataWord2.s.SizeX2=battle2.Target2SizeX2;
		vsp.Data4.s.SizeY2=vkp.DataWord2.s.SizeY2=battle2.Target2SizeY2;


		vkp.KommandWord.s.SM=0; //�� ���������� ������������ �� Xilinx �������

		cmosxilinx_write_kadrpos();	//����������� ��������� 1024 ���� ������ �� �������

		if(battle2.ControlState2.s.SumChan1T2)	//������������ ������� ��� ������������ �� 2 ������ 1
		{
			//������������� ���������� ������ ��� ���. �����������
			isInitialized[16]=(vkp.DataWord1.s.SizeX1/2)|((vkp.DataWord1.s.SizeY1/2)<<16);

			srioUpac.SrioDataKamera1.s.ShiftInRow=vkp.DataWord3.s.CoordX1/8;
			srioUpac.SrioDataKamera1.s.SizeRow=(vkp.DataWord1.s.SizeX1/8)-1;
			srioUpac.SrioDataKamera1.s.SumX2=battle2.ControlState2.s.SumChan1T2;		//����������� �� XilinxVPU
			srioUpac.SrioDataKamera1.s.KamIm=1;
		}
		else
		{
			//������������� ���������� ������ ��� ���. �����������
			isInitialized[16]=vkp.DataWord1.s.SizeX1|(vkp.DataWord1.s.SizeY1<<16);

			srioUpac.SrioDataKamera1.s.ShiftInRow=vkp.DataWord3.s.CoordX1/4;
			srioUpac.SrioDataKamera1.s.SizeRow=(vkp.DataWord1.s.SizeX1/4)-1;
			srioUpac.SrioDataKamera1.s.SumX2=battle2.ControlState2.s.SumChan1T2;		//����������� �� XilinxVPU
			srioUpac.SrioDataKamera1.s.KamIm=1;
		}

		if(battle2.ControlState2.s.SumChan2T2)	//������������ ���������� ��� ������������ �� 2 ������ 2
		{
			//������������� ���������� ������ ��� ���. �����������
			isInitialized[17]=(vkp.DataWord2.s.SizeX2/2)|((vkp.DataWord2.s.SizeY2/2)<<16);

			srioUpac.SrioDataKamera2.s.ShiftInRow=vkp.DataWord4.s.CoordX2/8;
			srioUpac.SrioDataKamera2.s.SizeRow=(vkp.DataWord2.s.SizeX2/8)-1;
			srioUpac.SrioDataKamera2.s.SumX2=battle2.ControlState2.s.SumChan2T2;		//����������� �� XilinxVPU
			srioUpac.SrioDataKamera2.s.KamIm=1;
		}
		else
		{
			//������������� ���������� ������ ��� ���. �����������
			isInitialized[17]=vkp.DataWord2.s.SizeX2|(vkp.DataWord2.s.SizeY2<<16);

			srioUpac.SrioDataKamera2.s.ShiftInRow=vkp.DataWord4.s.CoordX2/4;
			srioUpac.SrioDataKamera2.s.SizeRow=(vkp.DataWord2.s.SizeX2/4)-1;
			srioUpac.SrioDataKamera2.s.SumX2=battle2.ControlState2.s.SumChan2T2;		//����������� �� XilinxVPU
			srioUpac.SrioDataKamera2.s.KamIm=1;
		}

		if(battle2.BMODETARGET2==t2GrabChan1)
		{
			//������������ ���������� ��� ��������� �������
			cmos1config.ADC_GAIN=GAIN;
			cmos1config.Reg_73=FOTReg73;
			cmos1config.OFFSET1=OffsetChan1&0xff;
			cmos1config.OFFSET2=(OffsetChan1>>8)&0xff;

			cmos1config.Exp_time1=cmos1config.Exp_time1=1;	//����������� ����� ����������
			cmos1config.Exp_time2=cmos1config.Exp_time2=0;
			cmos1config.Exp_time3=cmos1config.Exp_time3=0;

			cmos2config.ADC_GAIN=GAIN;
			cmos2config.Reg_73=FOTReg73;
			cmos2config.OFFSET1=OffsetChan2&0xff;
			cmos2config.OFFSET2=(OffsetChan2>>8)&0xff;

			cmos2config.Exp_time1=cmos1config.Exp_time1=1;	//����������� ����� ����������
			cmos2config.Exp_time2=cmos1config.Exp_time2=0;
			cmos2config.Exp_time3=cmos1config.Exp_time3=0;

			cmos_write_reg();			//����������� �������������� ��������
		}

		cmos1config.StartLinesLO = (Uint8)(vkp.DataWord3.s.CoordY1&0x00ff);		//����� ������ � ������� �������� ����������
		cmos1config.StartLinesHI= (Uint8)((vkp.DataWord3.s.CoordY1>>8)&0xff);
		cmos1config.NumberLinesLO = (Uint8)(vkp.DataWord1.s.SizeY1&0x00ff);		//����������� ���������� �����
		cmos1config.NumberLinesHI = (Uint8)((vkp.DataWord1.s.SizeY1>>8)&0xff);

		cmos2config.StartLinesLO = (Uint8)(vkp.DataWord4.s.CoordY2&0x00ff);		//����� ������ � ������� �������� ����������
		cmos2config.StartLinesHI= (Uint8)((vkp.DataWord4.s.CoordY2>>8)&0xff);
		cmos2config.NumberLinesLO = (Uint8)(vkp.DataWord2.s.SizeY2&0x00ff);		//����������� ���������� �����
		cmos2config.NumberLinesHI = (Uint8)((vkp.DataWord2.s.SizeY2>>8)&0xff);

		cmos_write_sizekadr();		//����������� ������ ��������
	}

	isInitialized[1]=srioUpac.VideoRamK1;
	isInitialized[2]=srioUpac.SrioDataKamera1.sw;
	isInitialized[3]=srioUpac.VideoRamK2;
	isInitialized[4]=srioUpac.SrioDataKamera2.sw;
//	isInitialized[0] = 3;
	Cache_wb((void *) &isInitialized[0], 80, Cache_Type_L1D, TRUE);

}

void CreateDistanseDelayBattle(uint32 Distance, uint8 Target, uint8 KolUP)
{
	//����������� ���������� ����������
	double LocalDistanseNS=0;
	double RealDelay=0;
	Uint16	NDelayXilinxLO=0;
	Uint16	NDelayXilinxDark=0;
	Uint16	NDelayXilinxKolStart=0;	//�������� ������� �����������

	LocalDistanseNS=(double)(DISTANSEDELAY+(double)((double)(2000000*(double)Distance)/(double)300000000))*1000;
	LocalDistanseNS-=3263.5;	//������� ����������� �������� �������
	NDelayXilinxKolStart=(Uint16)(LocalDistanseNS/312.5);
	LocalDistanseNS-=4800;	//������������� ���������� �� ������ ��������
	NDelayXilinxLO=(Uint16)(LocalDistanseNS/312.5);

	RealDelay=(LocalDistanseNS+3263.5)/100;

	LocalDistanseNS+=18400000;	//����� ������ ��������� ����� �� 18 �����������
	NDelayXilinxDark= (Uint16)(LocalDistanseNS/312.5);

	if(Target==0)	//1 ����
	{
		if(battle.BMODETARGET1>=t1ConvoyChan1_2)
			NDelayXilinxDark+=1900;

		isInitialized[24]=((uint32)NDelayXilinxLO)|((uint32)(NDelayXilinxDark)<<16);

		oed1.STK2=oed1.STK1=(uint16)RealDelay;
		Cache_wb((void *) &oed1, sizeof(OEDTOTA), Cache_Type_L1D, TRUE);

		if(KolUP)				//��� ������������� ��������� ������ ������ � ������� �����������
		{
			isInitialized[27]=NDelayXilinxKolStart;
			isInitialized[28]=0;
		}
		else
		{
			isInitialized[27]=isInitialized[27]&0xffff0000;
			isInitialized[28]=0;
		}
	}
	else
	{
		if(battle2.BMODETARGET2>=t2ConvoyChan1_2)
			NDelayXilinxDark+=1900;

		isInitialized[25]=((uint32)NDelayXilinxLO)|((uint32)(NDelayXilinxDark)<<16);

		oed2.STK2=oed2.STK1=(uint16)RealDelay;
		Cache_wb((void *) &oed2, sizeof(OEDTOTA), Cache_Type_L1D, TRUE);

		if(KolUP)				//��� ������������� ��������� ������ ������ � ������� �����������
		{
			isInitialized[27]=NDelayXilinxKolStart<<16;
			isInitialized[28]=0;
		}
		else
		{
			isInitialized[27]=isInitialized[27]&0x0000ffff;
			isInitialized[28]=0;
		}
	}

}

void CreateDistanseDelayDiag(uint32 Distance)
{
	//����������� ���������� ����������

	isInitialized[24]=0;
	isInitialized[25]=0;
	isInitialized[26]=0;
	isInitialized[27]=0;
	isInitialized[28]=12;	//�������� � 3,5 ��� �� �������

}

extern int AbsBlock;		//���������� ���� ������ ����� ���������
extern uint16 countCh1;		//�������� ��������
extern uint16 countCh2;
extern uint16 countCh1T2;	//�������� ��������
extern uint16 countCh2T2;

FramPuskStruct FPStarget1;
FramPuskStruct FPStarget2;
uint32 Data1StopSave=0;
uint32 Data2StopSave=0;
///////////////////////////////////////////////////////////////////////////
uint32 SaveAllTarget=0;			//������� ������������� ��������� ��� ����
uint32 SizeByteSaveTarget=0;

extern uint8 Target1MassSinx1sec[50];
extern uint8 Target2MassSinx1sec[50];

//����� �� ���� 1
int dtask_udp_target1( SOCKET s, UINT32 unused )
{
	volatile unsigned long long TimeJobStart=0;
	volatile unsigned long long TimeJobStop=0;
	volatile unsigned long long TimeJobFinish=0;

	DataFileTitle datatitle;
	DataFileFrame dataframe;

	volatile uint32 SizeFrameByte=0;

    struct sockaddr_in sin1;
    struct sockaddr_in sin2;
    struct timeval     to;
    int                i=0,j=0,tmp=0,size=0,timerclosesocket=0,battlecount=0;
  //  UInt event;
    (void)unused;


    //������ ��������� �������� �����
    // Configure our socket timeout to be 3 seconds
    to.tv_sec  = 0;
    to.tv_usec = 1;
    setsockopt( s, SOL_SOCKET, SO_RCVTIMEO, &to, sizeof( to ) );
    to.tv_sec  = 0;
    to.tv_usec = 1;
    setsockopt( s, SOL_SOCKET, SO_SNDTIMEO, &to, sizeof( to ) );

    size=7168;
    setsockopt( s, SOL_SOCKET, SO_SNDBUF, &size, sizeof(size) );

    //��� ��������� ����� ���������� �����
    //////////////////////////////////////////////////////////////////
    tmp = sizeof( sin1 );
    i = (int)recvfrom(s, &BuffTA1, sizeof(BuffTA1),0,(PSA)&sin1, &tmp);

    if(AbsBlock!=0)				//��������� �� ���������� �����
    	return 1;

    if(i == SizeBuffTA)
    {
    	assembleBuffOED((uint16*)BuffOED1,1,SizeBuffOED);
    	sendto( s, BuffOED1, sizeof(BuffOED1), 0, (PSA)&sin1, sizeof(sin1) );	//���������� ����� ������
    	sendto( s, BuffOED1, sizeof(BuffOED1), 0, (PSA)&sin1, sizeof(sin1) );
    }
    //������� ������
    for(j=0;j<3;j++)
    	i = (int)recvfrom(s, &BuffTA1, sizeof(BuffTA1),MSG_DONTWAIT,(PSA)&sin2, &tmp);

	timerclosesocket=0;	//������� ������� �� �������� ������
	disassembleBuffTA((uint16*)BuffTA1,1,SizeBuffTA);

	spi_cmos_reset_enable();		//reset ������

    for(;;)
    {
    	//������� ������� �� ������� 11,5 �� ������� ��� �������� ������ �� ����
 //   	Event_pend(eventclkhtarget1, Event_Id_21,Event_Id_NONE, BIOS_WAIT_FOREVER);
    	Event_pend(eventclkhtarget1, Event_Id_21,Event_Id_NONE, 42);

    	TimeJobStart=0;
    	TimeJobStop=0;
    	TimeJobStart|=TSCL;


    	Cache_inv((void *) &battle, sizeof(BattleParam), Cache_Type_L1D, TRUE);
    	Cache_inv((void *) &battlemathT1k1, sizeof(BattleMathT1K1), Cache_Type_L1D, TRUE);
    	Cache_inv((void *) &battlemathT1k2, sizeof(BattleMathT1K2), Cache_Type_L1D, TRUE);

    	if((ta1.CommandTA2.s.TechMode<=14))
    	{
    		AbsBlock=0;		//����� �������������� ��� ������ �� ��� ������

    		if(TimeJobFinish<0xffff)
    		 	oed1.reserv3= (uint16)TimeJobFinish;
    		else
    		 	oed1.reserv3=0xffff;
    		TimeJobFinish=0;

    		Cache_wb((void *) &oed1, sizeof(OEDTOTA), Cache_Type_L1D, TRUE);

    		assembleBuffOED((uint16*)BuffOED1,1,SizeBuffOED);

    		sendto( s, BuffOED1, sizeof(BuffOED1), 0, (PSA)&sin1, sizeof(sin1));
    		CycleDelay(60000);
    		sendto( s, BuffOED1, sizeof(BuffOED1), 0, (PSA)&sin1, sizeof(sin1));

		}
    	Cache_wb((void *) &battlemathT1k1, sizeof(BattleMathT1K1), Cache_Type_L1D, TRUE);
    	Cache_wb((void *) &battlemathT1k2, sizeof(BattleMathT1K2), Cache_Type_L1D, TRUE);

    	if((battle.STAGE==sBATTLE)&&(battle.BMODETARGET1==t1STOP)&&(battle2.STAGE2==sSTANDBY2))
    		spi_cmos_reset_enable();		//reset ������


    	//���������������� ������ � DDR ������
    	if((ta1.CommandTA1.s.Pusk)&&(ta1.CommandTA1.s.SavePusk))	//������� ���� �����������
    	{
    		uint32 tmpdata=0;
    		FillDataTitle1(&datatitle,battlecount,SizeFrameByte,0);
    		tmpdata=FillDataFrame1(&dataframe,battlecount,0,0);
    	//	SizeFrameByte=FillDataFrame1(&dataframe,battlecount,0);

    		if((tmpdata!=0)&&(tmpdata!=0xffffffff))
    		{
    			SizeFrameByte=tmpdata;
    		}
    		else
    		{
    			Data1StopSave=1;
    		}

   			if((ta1.CommandTA1.s.ResetK1==0)&&(Data1StopSave==0))
    			Task_sleep(7);			//3,5 ��
   				//Task_sleep(4);			//3,5 ��
    			else
    			Task_sleep(7);			//3,5 ��
    			//Task_sleep(4);			//3,5 ��
    	}
    	else
    	{
    		Task_sleep(8);			//4 ��
    	//	Task_sleep(4);			//4 ��
    	}


    	//������� �������� ������� 17 �� ������� ����� �������� ������ ��������� � ��� ������������� ������
//    	Event_pend(eventclkhtarget1, Event_Id_22,Event_Id_NONE, BIOS_WAIT_FOREVER);
    //	event=Event_pend(eventclkhtarget1,Event_Id_22,Event_Id_NONE, 8);
  //  	Task_sleep(7);			//3,5 ��

    	 tmp = sizeof( sin2 );
    	 i = (int)recvfrom(s, &BuffTA1, sizeof(BuffTA1),MSG_DONTWAIT,(PSA)&sin2, &tmp); //��������� ���� �� �����

 //   	 if(i == SizeBuffTA)	//������ ������ �����
 //   	 {
    	   	if(i == SizeBuffTA)	//������ ������ �����
    	   	{
    	   		timerclosesocket=0;	//������� ������� �� �������� ������
    	   		disassembleBuffTA((uint16*)BuffTA1,1,SizeBuffTA); 	//��������� ��������� �����
    	   	}

	    	Cache_inv((void *) &isInitialized[0], 128, Cache_Type_L1D, TRUE);
//	    	Cache_wb((void *) &battle, sizeof(BattleParam), Cache_Type_L1D, TRUE);


    	    if(ta1.CommandTA1.s.Pusk)	//������ ������� ����
    	    {

    	    	isInitialized[26]=1;	//������� ����� �� �������������

    	 	    if((ta1.CommandTA2.s.TechMode==12)||(ta1.CommandTA2.s.TechMode==14))	//��������������� ������ ����� ������ �� ����������� �����������
    	 	    	battle.ControlState.s.KOLT1=1;
    	 	    else
    	 	    	battle.ControlState.s.KOLT1=0;

    	 	   CreateDistanseDelayBattle(ta1.Distance, 0,battle.ControlState.s.KOLT1); 		//������������ ��������� �� ��������� ���������� �� ������� ����

    	    	if((battle.STAGE==sBATTLE)&&(battle.BMODETARGET1!=t1STOP))
    	    	{
						SetFieldAnalysis(0);		//������������� ���� ������� �� ���������� ������������ ���� 1
						SetBattleSizaAndPosition(battle.Target1CoordX1,battle.Target1CoordY1,battle.Target1CoordX2,battle.Target1CoordY2,0);
    	    	}
    	    	else
    	    	{
    	    		battle.STAGE=sBATTLE;					//��������� � ������ �����
    	    		Data1StopSave=0;

    	    		if(ta1.CommandTA1.s.Sxod)
    	    		{
    	    	    	isInitialized[76]+=1;	//���������� ���������� ������ � ������� ���������

    	    	    	Cache_wb((void *) &isInitialized[76], 4, Cache_Type_L1D, TRUE);

    	    	    	//����������� �� ������ ���������� � ����������� ������
    	    	    		ReadFramParamData(&FPStarget1,0);
    	    	    	///////////////////////////////////////////////////////

    	    			battle.BMODETARGET1=t1GrabChan1;		//��������� � ����� ������ � ������ 1
    	    			if((battle.STAGE==sBATTLE)&&(battle2.STAGE2!=sBATTLE2))
    	    	   		{
    	    	    			GlobalBattleCount++;
    	    	    			uint32* base=(uint32*)FRAM_PARAM_OED;
    	    	    			UInt16 test;
		    	    			test=gpioReadDataBus();

		 						gpioWriteDataBus(test|0x01);		//������������� ��� ������ � FRAM

		    	    			*(base+2)=GlobalBattleCount;

		    	    			CycleDelay(400);

		    	    			gpioWriteDataBus(test);
		    	    			FPStarget1.GlobalNumPusk=GlobalBattleCount;
	    	    		}
        	    		else
        	    		{
        	    			FPStarget1.GlobalNumPusk=FPStarget2.GlobalNumPusk;
        	    		}
    	    		}

    	    		battle.Target1SizeX1=288;				//���� 1
    	    		battle.Target1SizeY1=288;
    	    		battle.Target1SizeX2=368;
    	    		battle.Target1SizeY2=368;

    				battle.ControlState.s.SumChan1T1=1;	//�������� ������������
    				battle.ControlState.s.SumChan2T1=1;	//�������� ������������

    				countCh1=countCh2=0;
    				BattleTimerSinx1=0;

    				SetBattleSizaAndPosition((battle.VizirChan1_X-(battle.Target1SizeX1/2)),
    						(battle.VizirChan1_Y-(battle.Target1SizeY1/2)),(battle.VizirChan2_X-(battle.Target1SizeX2/2)),
    						(battle.VizirChan2_Y-(battle.Target1SizeY2/2)),0); //������������� ��������� ���� �������
    	    	}

    	    	isInitialized[0] = 3;	//���������� ��������� � Xilinx
    	    	Cache_wb((void *) &isInitialized[0], 128, Cache_Type_L1D, TRUE);
    	    	battlecount++;
    	    }
    	    else
    	    {
    	    	battlecount=0;
    	    	SetFieldAnalysis(0);		//������������� ���� ������� �� ���������� ������������ ���� 1

    	    	isInitialized[24]=0;
    	    	isInitialized[27]=isInitialized[27]&0xffff0000;
    	    	isInitialized[28]=0;

    	    	battle.BMODETARGET1=t1STOP;

    	    	countCh1=countCh2=0;

    	    	if(battle.STAGE>=sBATTLE)
    	    	{
    	    		if((ta1.CommandTA1.s.SavePusk)&&(battle2.STAGE2==sSTANDBY2))
    	    		{

    	    		  oed1.SignalOED1.s.SavePusk=ta1.CommandTA1.s.SavePusk;
    	    		  Cache_wb((void *) &oed1, sizeof(OEDTOTA), Cache_Type_L1D, TRUE);

    	    		  if(SaveAllTarget==1)
    	    		  {
						  oed2.SignalOED1.s.SavePusk=ta2.CommandTA1.s.SavePusk;
						  Cache_wb((void *) &oed2, sizeof(OEDTOTA), Cache_Type_L1D, TRUE);
    	    		  }

    	    		  assembleBuffOED((uint16*)BuffOED1,1,SizeBuffOED);

    	    		  Task_sleep(40);			//20 ��
    	    		  sendto( s, BuffOED1, sizeof(BuffOED1), 0, (PSA)&sin1, sizeof(sin1));
    	    		  CycleDelay(60000);
    	    		  sendto( s, BuffOED1, sizeof(BuffOED1), 0, (PSA)&sin1, sizeof(sin1));
    	    		  Task_sleep(40);			//20 ��

    	    		  //��������� ����
    	    		  SavePuskInDataFlash(FPStarget1.PosNumRing,SizeFrameByte,0);	//��������� ��� � ���� 1!!!!!!!!!
    	    		  //��������� �� fram ���������� � ����������� ������
  	    			  WriteFramParamData(&FPStarget1,SizeFrameByte,0);

  	    			  if(SaveAllTarget==1)			//������ ���� ���� ��������� � ����������
  	    				  	  	  	  	  	  	  	 //�����
  	    			  {
      	    		    //��������� ����
      	    		    SavePuskInDataFlash(FPStarget2.PosNumRing,SizeByteSaveTarget,1);	//��������� ��� � ���� 1!!!!!!!!!
      	    		    //��������� �� fram ���������� � ����������� ������
      	    		  	 WriteFramParamData(&FPStarget2,SizeByteSaveTarget,1);
      	    		   ///////////////////////////////////////////////////////

      	    		  	SaveAllTarget=0;
      	    		  	SizeByteSaveTarget=0;
      	    		  	oed2.SignalOED1.s.SavePusk=0;
      	    		  	Cache_wb((void *) &oed2, sizeof(OEDTOTA), Cache_Type_L1D, TRUE);
  	    			  }

    	    			///////////////////////////////////////////////////////

  	    			 oed1.SignalOED1.s.SavePusk=0;
  	    			 Cache_wb((void *) &oed1, sizeof(OEDTOTA), Cache_Type_L1D, TRUE);
    	    		}
    	    		else
    	    		{
    	    			if((ta1.CommandTA1.s.SavePusk)&&(battle2.STAGE2!=sSTANDBY2))
    	    			{
    	    				SaveAllTarget=1;					//��������� ������� ��� ������ �� ���� 2
    	    													//���������� ��������� ���� � ���� 1
    	    				SizeByteSaveTarget=SizeFrameByte;	//���������� ������ ������� ���������� ���������
    	    			}
    	    		}
    	    		battle.STAGE=sSTANDBY;		//��������� � �������� ����� �� ���� 1
    	    	}

				battle.ControlState.s.SumChan1T1=0;	//�������� ������������
				battle.ControlState.s.SumChan2T1=0;	//�������� ������������
    	    	if(battle2.BMODETARGET2==t2STOP)
    	    		{
    	    			isInitialized[26]=0;
    	    		}
    	    }

    	    Cache_wb((void *) &battle, sizeof(BattleParam), Cache_Type_L1D, TRUE);


       		i = (int)recvfrom(s, &BuffTA1, sizeof(BuffTA1),MSG_DONTWAIT,(PSA)&sin2, &tmp);
       		i = (int)recvfrom(s, &BuffTA1, sizeof(BuffTA1),MSG_DONTWAIT,(PSA)&sin2, &tmp);

 //  	  }

     	if(i!=-1)
     	{
     	    for(j=0;j<20;j++)
     	    	i = (int)recvfrom(s, &BuffTA1, sizeof(BuffTA1),MSG_DONTWAIT,(PSA)&sin2, &tmp);
     	}

    	if((battle2.STAGE2==sSTANDBY2)&&(battle.STAGE==sSTANDBY))
       	if(ta1.CommandTA2.s.TechMode==15)	//������� �� ������� � ��� ����� ��� ������ ��� �����
        									//��� ��������� ����������� ��� ��������� ������
        {
       		to.tv_sec  = 3;
       		to.tv_usec = 1;
       		setsockopt( s, SOL_SOCKET, SO_RCVTIMEO, &to, sizeof( to ) );
       		to.tv_sec  = 3;
       		to.tv_usec = 1;
       		setsockopt( s, SOL_SOCKET, SO_SNDTIMEO, &to, sizeof( to ) );

       		NewVideoTechPaket();

       		for(;;)						//�������� � ���� �� ���������� ��� ������
       		{
			  AbsBlock=1;				//��� ����� ������������� � ��� ������
			  sin1.sin_port=htons(40102);
			  videotechpacket.NPS=0;

			  SendTechVideoChanal2(0);	//��������� ���������� �����

			  videotechparam.Word2.s.TimeExp=ta1.CommandTA2.s.TimeExp;	//������������ ����� ����������

			  AssembleBuffersTechVideo((uint8*)&GlobalVideoBuff,SizeKadrTechRejX);
			  sendto( s, BuffVideoParamPacket, sizeof(BuffVideoParamPacket), 0, (PSA)&sin1, sizeof(sin1) );
			  CycleDelay(50000);
			  sendto( s, &GlobalVideoBuff, ((SizeKadrTechRejX*2)+sizeof(VideoTechPacket)), 0, (PSA)&sin1, sizeof(sin1) );
			  CycleDelay(50000);

			  for(j=1;j<(SizeKadrTechRejY/2);j++)
			  {
					videotechpacket.NPS+=2;
					AssembleBuffersTechVideo((uint8*)&GlobalVideoBuff[(SizeKadrTechRejX*2)*j],SizeKadrTechRejX);
					sendto(s, &GlobalVideoBuff[(SizeKadrTechRejX*2)*j], ((SizeKadrTechRejX*2)+sizeof(VideoTechPacket)), 0, (PSA) &sin1,sizeof(sin1));
					CycleDelay(50000);
			  }
			  videotechpacket.NPK++;	//����� ������������� �����
			  videotechparam.NPK=videotechpacket.NPK;

			  tmp = sizeof( sin1 );
			  i = (int)recvfrom(s, &BuffTA1, sizeof(BuffTA1),0,(PSA)&sin1, &tmp);

			  if(i == SizeBuffTA)	//������ ������ �����
			  {
				  disassembleBuffTA((uint16*)BuffTA1,1,SizeBuffTA);
			  }
			  else
			  {
				ta1.CommandTA2.s.TechMode=0;	//������� �� ��� ������

		       	to.tv_sec  = 0;
		       	to.tv_usec = 1;
		       	setsockopt( s, SOL_SOCKET, SO_RCVTIMEO, &to, sizeof( to ) );
		       	to.tv_sec  = 0;
		       	to.tv_usec = 1;
		       	setsockopt( s, SOL_SOCKET, SO_SNDTIMEO, &to, sizeof( to ) );

		       	AbsBlock=0;				//��� ����� �������������� � ��� ������

		       	break;
			  }

       		}
        }

       	else
       	{
    		//��������� ������ �� SRIO � ���������� ��������
    		isInitialized[0] = 3;
    		Cache_wb((void *) &isInitialized[0], 4, Cache_Type_L1D, TRUE);
       	}


		timerclosesocket++;

		if(timerclosesocket>=20)	//�������� ������ � ������� 3 ������
		{
			break;
		}

    	TimeJobStop=TSCL;
    	if(TimeJobStop>TimeJobStart)
    	TimeJobFinish=(TimeJobStop-TimeJobStart)/1000;

    }
    // Since the socket is still open, return "1"
    // (we need to leave UDP sockets open)

    AbsBlock=0;
    oed1.NumPaket=0;	//��������� ���������� � �������� ����� ������
    Cache_wb((void *) &oed1, sizeof(OEDTOTA), Cache_Type_L1D, TRUE);
    Cache_inv((void *) &oed1, sizeof(OEDTOTA), Cache_Type_L1D, TRUE);
    Cache_inv((void *) &isInitialized[0], 128, Cache_Type_L1D, TRUE);
    return(1);
}
//����� �� ���� 2
int dtask_udp_target2( SOCKET s, UINT32 unused )
{
	volatile unsigned long long TimeJobStart2=0;
	volatile unsigned long long TimeJobStop2=0;
	volatile unsigned long long TimeJobFinish2=0;

	DataFileTitle datatitle2;
	DataFileFrame dataframe2;

	volatile uint32 SizeFrameByte2=0;

    struct sockaddr_in sin1;
    struct sockaddr_in sin2;
    struct timeval     to;
    int                i=0,j=0,tmp=0,size=0,timerclosesocket=0,battlecount2=0;
 //   UInt event;
    (void)unused;

    //������ ��������� �������� �����
    // Configure our socket timeout to be 3 seconds
    to.tv_sec  = 0;
    to.tv_usec = 1;
    setsockopt( s, SOL_SOCKET, SO_RCVTIMEO, &to, sizeof( to ) );
    to.tv_sec  = 0;
    to.tv_usec = 1;
    setsockopt( s, SOL_SOCKET, SO_SNDTIMEO, &to, sizeof( to ) );

    size=7168;
    setsockopt( s, SOL_SOCKET, SO_SNDBUF, &size, sizeof(size) );

    //��� ��������� ����� ���������� �����
    //////////////////////////////////////////////////////////////////
    tmp = sizeof( sin1 );
    i = (int)recvfrom(s, &BuffTA2, sizeof(BuffTA2),0,(PSA)&sin1, &tmp);

    if(AbsBlock!=0)				//��������� �� ���������� �����
    	return 1;

    if(i == SizeBuffTA)
    {
    	assembleBuffOED((uint16*)BuffOED2,2,SizeBuffOED);
    	sendto( s, BuffOED2, sizeof(BuffOED2), 0, (PSA)&sin1, sizeof(sin1) );	//���������� ����� ������
    	sendto( s, BuffOED2, sizeof(BuffOED2), 0, (PSA)&sin1, sizeof(sin1) );
    }
    //������� ������
    for(j=0;j<3;j++)
    	i = (int)recvfrom(s, &BuffTA2, sizeof(BuffTA2),MSG_DONTWAIT,(PSA)&sin2, &tmp);

	timerclosesocket=0;	//������� ������� �� �������� ������
	disassembleBuffTA((uint16*)BuffTA2,2,SizeBuffTA);
	spi_cmos_reset_enable();		//reset ������

    for(;;)
    {
    //	Event_pend(eventclkhtarget2, Event_Id_23,Event_Id_NONE, BIOS_WAIT_FOREVER);
    	Event_pend(eventclkhtarget2, Event_Id_23,Event_Id_NONE, 42);

    	TimeJobStart2=0;
    	TimeJobStop2=0;
    	TimeJobStart2|=TSCL;

    	Cache_inv((void *) &battle2, sizeof(BattleParam2), Cache_Type_L1D, TRUE);
    	Cache_inv((void *) &battlemathT2k1, sizeof(BattleMathT2K1), Cache_Type_L1D, TRUE);
    	Cache_inv((void *) &battlemathT2k2, sizeof(BattleMathT2K2), Cache_Type_L1D, TRUE);

    	if((ta2.CommandTA2.s.TechMode<=14))
    	 {
    	 	AbsBlock=0;		//����� �������������� ��� ������ �� ��� ������

    	 	if(TimeJobFinish2<0xffff)
    	 	oed2.reserv3= (uint16)TimeJobFinish2;
    	 	else
    	 	oed2.reserv3=0xffff;
    	 	TimeJobFinish2=0;

    	 	Cache_wb((void *) &oed2, sizeof(OEDTOTA), Cache_Type_L1D, TRUE);

    	    assembleBuffOED((uint16*)BuffOED2,2,SizeBuffOED);

    	    sendto( s, BuffOED2, sizeof(BuffOED2), 0, (PSA)&sin1, sizeof(sin1));
    	    CycleDelay(60000);
    	    sendto( s, BuffOED2, sizeof(BuffOED2), 0, (PSA)&sin1, sizeof(sin1));

    	}
    	Cache_wb((void *) &battlemathT2k1, sizeof(BattleMathT2K1), Cache_Type_L1D, TRUE);
    	Cache_wb((void *) &battlemathT2k2, sizeof(BattleMathT2K2), Cache_Type_L1D, TRUE);

    	if((battle2.STAGE2==sBATTLE2)&&(battle2.BMODETARGET2==t2STOP)&&(battle.STAGE==sSTANDBY))
    		spi_cmos_reset_enable();		//reset ������

    	//���������������� ������ � DDR ������
    	if((ta2.CommandTA1.s.Pusk)&&(ta2.CommandTA1.s.SavePusk))	//������� ���� �����������
    	{
    		uint32 tmpdata=0;
    		FillDataTitle1(&datatitle2,battlecount2,SizeFrameByte2,1);
    		tmpdata=FillDataFrame1(&dataframe2,battlecount2,0,1);

    		if((tmpdata!=0)&&(tmpdata!=0xffffffff))
    		{
    			SizeFrameByte2=tmpdata;
    		}
    		else
    		{
    			Data2StopSave=1;
    		}

   			if((ta2.CommandTA1.s.ResetK1==0)&&(Data2StopSave==0))
    			Task_sleep(7);			//3,5 ��
   			//	Task_sleep(4);			//4 ��
    			else
    			Task_sleep(7);			//4 ��
    		//	Task_sleep(4);			//4 ��
    	}
    	else
    	{
    		Task_sleep(8);			//4 ��
    //		Task_sleep(4);			//4 ��
    	}


    	//������� �������� ������� 17 �� ������� ����� �������� ������ ��������� � ��� ������������� ������
//    	Event_pend(eventclkhtarget2, Event_Id_24,Event_Id_NONE, BIOS_WAIT_FOREVER);
    	//event=Event_pend(eventclkhtarget2, Event_Id_24,Event_Id_NONE, 8);
//    	Task_sleep(7);			//3,5 ��

    	tmp = sizeof( sin2 );
    	i = (int)recvfrom(s, &BuffTA2, sizeof(BuffTA2),MSG_DONTWAIT,(PSA)&sin2, &tmp); //��������� ���� �� �����

//    	if(i == SizeBuffTA)	//������ ������ �����
//    	{
        	if(i == SizeBuffTA)	//������ ������ �����
        	{
        		timerclosesocket=0;	//������� ������� �� �������� ������
        		disassembleBuffTA((uint16*)BuffTA2,2,SizeBuffTA); //��������� ��������� �����
        	}

	    	Cache_inv((void *) &isInitialized[0], 128, Cache_Type_L1D, TRUE);

    	    if(ta2.CommandTA1.s.Pusk)	//������ ������� ����
    	    {
    	       	isInitialized[26]=1;	//������� ����� �� �������������

    	       	if((ta2.CommandTA2.s.TechMode==12)||((ta2.CommandTA2.s.TechMode==14)))	//��������������� ������ ����� ������ �� ����������� �����������
    	       	   	battle2.ControlState2.s.KOLT2=1;
    	       	else
    	       	   	battle2.ControlState2.s.KOLT2=0;

    	        CreateDistanseDelayBattle(ta2.Distance, 1,battle2.ControlState2.s.KOLT2); 		//������������ ��������� �� ��������� ���������� �� ������� ����

    	       	if((battle2.STAGE2==sBATTLE2)&&(battle2.BMODETARGET2!=t2STOP))
    	       	{
					SetFieldAnalysis(1);		//������������� ���� ������� �� ���������� ������������ ���� 2
					SetBattleSizaAndPosition(battle2.Target2CoordX1,battle2.Target2CoordY1,battle2.Target2CoordX2,battle2.Target2CoordY2,1);
    	       	}
    	       	else
    	       	{
    	       		battle2.STAGE2=sBATTLE2;					//��������� � ������ �����
    	       		Data2StopSave=0;

    	    		if(ta2.CommandTA1.s.Sxod)
    	    		{
    	    	    	isInitialized[77]+=1;	//���������� ���������� ������ � ������� ���������
    	    	    	Cache_wb((void *) &isInitialized[77], 4, Cache_Type_L1D, TRUE);

    	    	    	//����������� �� ������ ���������� � ����������� ������
    	    	    	 ReadFramParamData(&FPStarget2,1);

    	    	 		battle2.BMODETARGET2=t2GrabChan1;		//��������� � ����� ������ � ������ 1
        	    		if((battle.STAGE!=sBATTLE)&&(battle2.STAGE2==sBATTLE2))
        	    		{
        	    			GlobalBattleCount++;
        	    			uint32* base=(uint32*)FRAM_PARAM_OED;
        	    			UInt16 test;

        	    			test=gpioReadDataBus();

     						gpioWriteDataBus(test|0x01);		//������������� ��� ������ � FRAM

        	    			*(base+2)=GlobalBattleCount;

        	    			CycleDelay(400);

        	    			gpioWriteDataBus(test);

        	    			FPStarget2.GlobalNumPusk=GlobalBattleCount;	//�������� ������ ������ �� ������ ���� � ������������
        	    		}
        	    		else
        	    		{
        	    			FPStarget2.GlobalNumPusk=FPStarget1.GlobalNumPusk;
        	    		}
    	    		}

    	    		battle2.Target2SizeX1=288;				//���� 2
    	    		battle2.Target2SizeY1=288;
    	    		battle2.Target2SizeX2=368;
    	    		battle2.Target2SizeY2=368;

    				battle2.ControlState2.s.SumChan1T2=1;	//�������� ������������
    				battle2.ControlState2.s.SumChan2T2=1;	//�������� ������������

    				countCh1T2=countCh2T2=0;
    				BattleTimerSinx2=0;

    				SetBattleSizaAndPosition((battle2.VizirChan1_X-(battle2.Target2SizeX1/2)),
    						(battle2.VizirChan1_Y-(battle2.Target2SizeY1/2)),(battle2.VizirChan2_X-(battle2.Target2SizeX2/2)),
    						(battle2.VizirChan2_Y-(battle2.Target2SizeY2/2)),1); //������������� ��������� ���� �������
    	    	}

    	       	isInitialized[0] = 3;	//���������� ��������� � Xilinx
    	       	Cache_wb((void *) &isInitialized[0], 128, Cache_Type_L1D, TRUE);
    	       	battlecount2++;
    	     }
    	     else
    	     {
    	    	battlecount2=0;
				SetFieldAnalysis(1);		//������������� ���� ������� �� ���������� ������������ ���� 2

    	       	isInitialized[25]=0;
    	       	isInitialized[27]=isInitialized[27]&0x0000ffff;
    	       	isInitialized[28]=0;

    	       	battle2.BMODETARGET2=t2STOP;

    	    	countCh1T2=countCh2T2=0;

    	    	if(battle2.STAGE2>=sBATTLE2)
    	    	{
    	    		if((ta2.CommandTA1.s.SavePusk)&&(battle.STAGE==sSTANDBY))
    	    		{

 		    	    	oed2.SignalOED1.s.SavePusk=ta2.CommandTA1.s.SavePusk;
    	    		    Cache_wb((void *) &oed2, sizeof(OEDTOTA), Cache_Type_L1D, TRUE);

    	    		    if(SaveAllTarget==1)
    	    		    {
  						  oed1.SignalOED1.s.SavePusk=ta1.CommandTA1.s.SavePusk;
  						  Cache_wb((void *) &oed1, sizeof(OEDTOTA), Cache_Type_L1D, TRUE);
    	    		    }

    	    		    assembleBuffOED((uint16*)BuffOED2,2,SizeBuffOED);

    	    		    Task_sleep(40);			//20 ��
    	    		    sendto( s, BuffOED2, sizeof(BuffOED2), 0, (PSA)&sin1, sizeof(sin1));
    	    		    CycleDelay(60000);
    	    		    sendto( s, BuffOED2, sizeof(BuffOED2), 0, (PSA)&sin1, sizeof(sin1));
    	    		    Task_sleep(40);			//20 ��

    	    		    //��������� ����
    	    		    SavePuskInDataFlash(FPStarget2.PosNumRing,SizeFrameByte2,1);	//��������� ��� � ���� 1!!!!!!!!!
    	    		    //��������� �� fram ���������� � ����������� ������
    	    		  	 WriteFramParamData(&FPStarget2,SizeFrameByte2,1);
    	    		   ///////////////////////////////////////////////////////

     	    			  if(SaveAllTarget==1)			//������ ���� ���� ��������� � ����������
     	    				  	  	  	  	  	  	  	 //�����
     	    			  {
         	    		    //��������� ����
         	    		    SavePuskInDataFlash(FPStarget1.PosNumRing,SizeByteSaveTarget,0);	//��������� ��� � ���� 1!!!!!!!!!
         	    		    //��������� �� fram ���������� � ����������� ������
         	    		  	 WriteFramParamData(&FPStarget1,SizeByteSaveTarget,0);
         	    		   ///////////////////////////////////////////////////////

         	    		  	SaveAllTarget=0;
         	    		  	SizeByteSaveTarget=0;
         	    		  	oed1.SignalOED1.s.SavePusk=0;
         	    		  	Cache_wb((void *) &oed1, sizeof(OEDTOTA), Cache_Type_L1D, TRUE);
     	    			  }

    	    		  	 oed2.SignalOED1.s.SavePusk=0;
    	    		  	 Cache_wb((void *) &oed2, sizeof(OEDTOTA), Cache_Type_L1D, TRUE);
    	    		 }
    	    		else
    	    		{
    	    			if((ta2.CommandTA1.s.SavePusk)&&(battle.STAGE!=sSTANDBY))
    	    			{
    	    				SaveAllTarget=1;					//��������� ������� ��� ������ �� ���� 2
    	    													//���������� ��������� ���� � ���� 1
    	    				SizeByteSaveTarget=SizeFrameByte2;	//���������� ������ ������� ���������� ���������
    	    			}
    	    		}
    	    		battle2.STAGE2=sSTANDBY2;		//��������� � �������� ����� �� ���� 1
    	    	}

				battle2.ControlState2.s.SumChan1T2=0;	//�������� ������������
				battle2.ControlState2.s.SumChan2T2=0;	//�������� ������������

    	    	if(battle.BMODETARGET1==t1STOP)
    	    		{
    	    			isInitialized[26]=0;
    	    		}

    	      }

    	    Cache_wb((void *) &battle2, sizeof(BattleParam2), Cache_Type_L1D, TRUE);

    		i = (int)recvfrom(s, &BuffTA2, sizeof(BuffTA2),MSG_DONTWAIT,(PSA)&sin2, &tmp);
    		i = (int)recvfrom(s, &BuffTA2, sizeof(BuffTA2),MSG_DONTWAIT,(PSA)&sin2, &tmp);
  //  	 }

    	if(i!=-1)
    	{
    	    for(j=0;j<20;j++)
    	    	i = (int)recvfrom(s, &BuffTA2, sizeof(BuffTA2),MSG_DONTWAIT,(PSA)&sin2, &tmp);
    	}

    	if((battle2.STAGE2==sSTANDBY2)&&(battle.STAGE==sSTANDBY))
        if(ta2.CommandTA2.s.TechMode==15)	//������� �� ������� � ��� ����� ��� ������ ��� �����
         									//��� ��������� ����������� ��� ��������� ������
         {
        		to.tv_sec  = 3;
        		to.tv_usec = 1;
        		setsockopt( s, SOL_SOCKET, SO_RCVTIMEO, &to, sizeof( to ) );
        		to.tv_sec  = 3;
        		to.tv_usec = 1;
        		setsockopt( s, SOL_SOCKET, SO_SNDTIMEO, &to, sizeof( to ) );

        		NewVideoTechPaket();

        	for(;;)						//�������� � ���� �� ���������� ��� ������
        	{
 			  AbsBlock=1;				//��� ����� ������������� � ��� ������
 			  sin1.sin_port=htons(40102);
 			  videotechpacket.NPS=0;

 			  SendTechVideoChanal2(1);	//��������� ���������� �����

 			  videotechparam.Word2.s.TimeExp=ta2.CommandTA2.s.TimeExp;	//������������ ����� ����������

 			  AssembleBuffersTechVideo((uint8*)&GlobalVideoBuff,SizeKadrTechRejX);
 			  sendto( s, BuffVideoParamPacket, sizeof(BuffVideoParamPacket), 0, (PSA)&sin1, sizeof(sin1) );
 			  CycleDelay(50000);
 			  sendto( s, &GlobalVideoBuff, ((SizeKadrTechRejX*2)+sizeof(VideoTechPacket)), 0, (PSA)&sin1, sizeof(sin1) );
 			  CycleDelay(50000);

 			  for(j=1;j<(SizeKadrTechRejY/2);j++)
 			  {
 					videotechpacket.NPS+=2;
 					AssembleBuffersTechVideo((uint8*)&GlobalVideoBuff[(SizeKadrTechRejX*2)*j],SizeKadrTechRejX);
 					sendto(s, &GlobalVideoBuff[(SizeKadrTechRejX*2)*j], ((SizeKadrTechRejX*2)+sizeof(VideoTechPacket)), 0, (PSA) &sin1,sizeof(sin1));
 					CycleDelay(50000);
 			  }
 			  videotechpacket.NPK++;	//����� ������������� �����
 			  videotechparam.NPK=videotechpacket.NPK;

 			  tmp = sizeof( sin1 );
 			  i = (int)recvfrom(s, &BuffTA2, sizeof(BuffTA2),0,(PSA)&sin1, &tmp);

 			  if(i == SizeBuffTA)	//������ ������ �����
 			  {
 				  disassembleBuffTA((uint16*)BuffTA2,2,SizeBuffTA);
 			  }
 			  else
 			  {
 				ta2.CommandTA2.s.TechMode=0;	//������� �� ��� ������

 		       	to.tv_sec  = 0;
 		       	to.tv_usec = 1;
 		       	setsockopt( s, SOL_SOCKET, SO_RCVTIMEO, &to, sizeof( to ) );
 		       	to.tv_sec  = 0;
 		       	to.tv_usec = 1;
 		       	setsockopt( s, SOL_SOCKET, SO_SNDTIMEO, &to, sizeof( to ) );

 		       	AbsBlock=0;				//��� ����� �������������� � ��� ������

 		       	break;
 			  }

        	}
         }

        else
        {
     		//��������� ������ �� SRIO � ���������� ��������
     		isInitialized[0] = 3;
     		Cache_wb((void *) &isInitialized[0], 4, Cache_Type_L1D, TRUE);
        }


 		timerclosesocket++;

 		if(timerclosesocket>=20)	//�������� ������ � ������� 3 ������
 		{
 			break;
 		}

    	TimeJobStop2|=TSCL;
    	if(TimeJobStop2>TimeJobStart2)
    	TimeJobFinish2=(TimeJobStop2-TimeJobStart2)/1000;
    }
    // Since the socket is still open, return "1"
    // (we need to leave UDP sockets open)

    AbsBlock=0;
    oed2.NumPaket=0;	//��������� ���������� � �������� ����� ������
    Cache_wb((void *) &oed2, sizeof(OEDTOTA), Cache_Type_L1D, TRUE);
    Cache_inv((void *) &oed2, sizeof(OEDTOTA), Cache_Type_L1D, TRUE);
    Cache_inv((void *) &isInitialized[0], 128, Cache_Type_L1D, TRUE);
    return(1);
}


void SendTechVideoChanal2(int target)
{
	srioUpac.VideoRamK1=LOADKADR1MEM;
    srioUpac.VideoRamK2=LOADKADR2MEM;

	StartStopEth(1);

		//����� ������ ����� 640x480
		//����� �� ������� ������� 8 ������� ���
		vkp.KommandWord.sw=0;
		vkp.ControlWord.sw=0;
		vkp.DataWord1.s.SizeX1=64;		//����������� ������ ����� ������ 1
		vkp.DataWord1.s.SizeY1=64;		//����������� ������ ����� ������ 1
		vkp.DataWord1.s.StartExp1=0;
		vkp.DataWord2.s.SizeX2=SizeKadrTechRejX;		//������ ����� ������ 2
		vkp.DataWord2.s.SizeY2=SizeKadrTechRejY;		//������ ����� ������ 2
		vkp.DataWord2.s.StartExp2=0;
		vkp.DataWord3.s.CoordX1=0;
		vkp.DataWord3.s.CoordY1=0;

		if(target==0)
		{
			vkp.DataWord3.s.Exp1=ta1.CommandTA2.s.TimeExp;
			vkp.DataWord4.s.Exp2=ta1.CommandTA2.s.TimeExp;
		}
		else
		{
			vkp.DataWord3.s.Exp1=ta2.CommandTA2.s.TimeExp;
			vkp.DataWord4.s.Exp2=ta2.CommandTA2.s.TimeExp;
		}
		vkp.DataWord4.s.CoordX2=VizirChan2X-(vkp.DataWord2.s.SizeX2/2);
		vkp.DataWord4.s.CoordY2=VizirChan2Y-(vkp.DataWord2.s.SizeY2/2);
		vkp.DataWord5.sw=0;
		vkp.CMOS1.s.ADC_GAIN=vkp.CMOS2.s.ADC_GAIN	=	GAIN;
		vkp.CMOS1.s.Reg73=vkp.CMOS2.s.Reg73		=		FOTReg73;
		vkp.CMOS1.s.Offset						= 		OffsetChan1;
	    vkp.CMOS2.s.Offset						= 		OffsetChan2;
	    vkp.CMOS2.sw=vkp.CMOS1.sw;

	    spi_cmos_reset_enable();	//������� ��������� ������ � ��������� �� � reset ���� ����������
	    CreateVideoChanal2(vkp.DataWord2.s.SizeX2,vkp.DataWord2.s.SizeY2);

	    //��������� ��� ������ �� �������������
	    isInitialized[24]=isInitialized[25]=isInitialized[26]=0;
	    isInitialized[27]=isInitialized[28]=isInitialized[29]=0;

	    cmosxilinx_write_kadrpos();	//����������� ��������� 1024 ���� ������ �� �������

	    //��� ���������� srio
	    srioUpac.SrioDataKamera1.s.ShiftInRow=vkp.DataWord3.s.CoordX1/4;
	    srioUpac.SrioDataKamera1.s.SizeRow=(vkp.DataWord1.s.SizeX1/4)-1;
	    srioUpac.SrioDataKamera1.s.SumX2=0;		//��� ������������ �� XilinxVPU
	    srioUpac.SrioDataKamera1.s.KamIm=1;
	    srioUpac.SrioDataKamera2.s.ShiftInRow=vkp.DataWord4.s.CoordX2/4;
	    srioUpac.SrioDataKamera2.s.SizeRow=(vkp.DataWord2.s.SizeX2/4)-1;
	    srioUpac.SrioDataKamera2.s.SumX2=0;		//��� ������������ �� XilinxVPU
	    srioUpac.SrioDataKamera2.s.KamIm=1;
		isInitialized[1]=srioUpac.VideoRamK1;
		isInitialized[2]=srioUpac.SrioDataKamera1.sw;
		isInitialized[3]=srioUpac.VideoRamK2;
		isInitialized[4]=srioUpac.SrioDataKamera2.sw;
		isInitialized[0] = 3;

		Cache_wb((void *) &isInitialized[0], 128, Cache_Type_L1D, TRUE);
		Task_sleep(2);
		Cache_inv((void *) &isInitialized[0], 128, Cache_Type_L1D, TRUE);

	    //������������ ���������� ��� ��������� �������
	    cmos1config.ADC_GAIN=vkp.CMOS1.s.ADC_GAIN;
	    cmos1config.Reg_73=vkp.CMOS1.s.Reg73;
	    cmos1config.OFFSET1=vkp.CMOS1.s.Offset&0xff;
	    cmos1config.OFFSET2=(vkp.CMOS1.s.Offset>>8)&0xff;

	    int32 Exp_time_Chan1=0;
	    Exp_time_Chan1=(((vkp.DataWord3.s.Exp1*10)/2.6875)-(0.43*cmos1config.Reg_73));
	    Exp_time_Chan1++;
	    cmos1config.Exp_time1=Exp_time_Chan1&0xff;
	    cmos1config.Exp_time2=(Exp_time_Chan1>>8)&0xff;
	    cmos1config.Exp_time3=(Exp_time_Chan1>>16)&0xff;
	    cmos1config.StartLinesLO = (Uint8)(vkp.DataWord3.s.CoordY1&0x00ff);		//����� ������ � ������� �������� ����������
	    cmos1config.StartLinesHI= (Uint8)((vkp.DataWord3.s.CoordY1>>8)&0xff);

	    cmos1config.NumberLinesLO = (Uint8)(vkp.DataWord1.s.SizeY1&0x00ff);		//����������� ���������� �����
	    cmos1config.NumberLinesHI = (Uint8)((vkp.DataWord1.s.SizeY1>>8)&0xff);

	    cmos2config.StartLinesLO = (Uint8)(vkp.DataWord4.s.CoordY2&0x00ff);		//����� ������ � ������� �������� ����������
	    cmos2config.StartLinesHI= (Uint8)((vkp.DataWord4.s.CoordY2>>8)&0xff);
	    cmos2config.NumberLinesLO = (Uint8)(vkp.DataWord2.s.SizeY2&0x00ff);		//����������� ���������� �����
	    cmos2config.NumberLinesHI = (Uint8)((vkp.DataWord2.s.SizeY2>>8)&0xff);

	    cmos2config.ADC_GAIN=vkp.CMOS2.s.ADC_GAIN;
	    cmos2config.Reg_73=vkp.CMOS2.s.Reg73;
	    cmos2config.OFFSET1=vkp.CMOS2.s.Offset&0xff;
	    cmos2config.OFFSET2=(vkp.CMOS2.s.Offset>>8)&0xff;


	    cmos2config.Exp_time1=cmos1config.Exp_time1;
	    cmos2config.Exp_time2=cmos1config.Exp_time2;
	    cmos2config.Exp_time3=cmos1config.Exp_time3;

	    cmos_write_reg();			//����������� �������������� ��������
	    cmos_write_sizekadr();		//����������� ������ ��������

	    gpioStrobBlink();	//������� �� ���������� ������ � ������
	    Task_sleep((5+(vkp.DataWord3.s.Exp1)/100));

	  StartStopEth(0);

}
