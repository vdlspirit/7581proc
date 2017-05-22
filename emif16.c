#include "ti/platform/platform.h"
#include "ti/platform/resource_mgr.h"
#include <cslr_emif16.h>
#include "emif16.h"

#define hEmif16Cfg     ((CSL_Emif16Regs*)CSL_EMIF16_REGS)

void EmifInit()
{
	UInt32 data=0;
	UInt8	W_SETUP=0,W_STROBE=0,W_HOLD=0;
	UInt8	R_SETUP=0,R_STROBE=0,R_HOLD=0;
	UInt8	SS=0, EW=0, TA=3, ASIZE=1;
	//Настраиваем emif

	//CE3 FLASH Programm M28 or FRAM

	data=hEmif16Cfg->RCSR;

	hEmif16Cfg->AWCCR=0xf0000080;	//максимальная задержка WAIT[0] used
	hEmif16Cfg->NANDFCTL=0x07;		//переводим CE0-CE2 в режим работы с NAND
	hEmif16Cfg->IMR=0;
	hEmif16Cfg->IMCR=0;
	hEmif16Cfg->IMSR=0;
	hEmif16Cfg->IRR=0;
	hEmif16Cfg->PMCR=0xfefefefe;

	SS=0;
	//////////////////CE0 NAND1///////////////////////////
	W_SETUP=0x7,W_STROBE=0x14,W_HOLD=0x7;
	R_SETUP=0x7,R_STROBE=0x14,R_HOLD=0x7;

	data=(SS<<31)+(EW<<30)+((W_SETUP&0xf)<<26)+((W_STROBE&0x3f)<<20)+((W_HOLD&0x7)<<17)+
							((R_SETUP&0xf)<<13)+((R_STROBE&0x3f)<<7)+((R_HOLD&0x7)<<4)+
							((TA&0x3)<<2)+(ASIZE&0x3);
	hEmif16Cfg->A0CR=data;

	/////////CE1 NAND2//////////////////////
	W_SETUP=0x7,W_STROBE=0x14,W_HOLD=0x7;
	R_SETUP=0x7,R_STROBE=0x14,R_HOLD=0x7;

	data=(SS<<31)+(EW<<30)+((W_SETUP&0xf)<<26)+((W_STROBE&0x3f)<<20)+((W_HOLD&0x7)<<17)+
							((R_SETUP&0xf)<<13)+((R_STROBE&0x3f)<<7)+((R_HOLD&0x7)<<4)+
							((TA&0x3)<<2)+(ASIZE&0x3);
	hEmif16Cfg->A1CR=data;

	////////////CE2 NAND3/////////////////
	W_SETUP=0x7,W_STROBE=0x14,W_HOLD=0x7;
	R_SETUP=0x7,R_STROBE=0x14,R_HOLD=0x7;

	data=(SS<<31)+(EW<<30)+((W_SETUP&0xf)<<26)+((W_STROBE&0x3f)<<20)+((W_HOLD&0x7)<<17)+
							((R_SETUP&0xf)<<13)+((R_STROBE&0x3f)<<7)+((R_HOLD&0x7)<<4)+
							((TA&0x3)<<2)+(ASIZE&0x3);
	hEmif16Cfg->A2CR=data;

	SS=1;
	//////////////////////CE3 FLASH programm and FRAM///////////////
	W_SETUP=0xf,W_STROBE=0xf,W_HOLD=0x7;
	R_SETUP=0xf,R_STROBE=0xf,R_HOLD=0x7;

	data=(SS<<31)+(EW<<30)+((W_SETUP&0xf)<<26)+((W_STROBE&0x3f)<<20)+((W_HOLD&0x7)<<17)+
							((R_SETUP&0xf)<<13)+((R_STROBE&0x3f)<<7)+((R_HOLD&0x7)<<4)+
							((TA&0x3)<<2)+(ASIZE&0x3);
	hEmif16Cfg->A3CR=data;
}
