/******************************************************************************
 * Copyright (c) 2011 Texas Instruments Incorporated - http://www.ti.com
 * 
 *  Redistribution and use in source and binary forms, with or without 
 *  modification, are permitted provided that the following conditions 
 *  are met:
 *
 *    Redistributions of source code must retain the above copyright 
 *    notice, this list of conditions and the following disclaimer.
 *
 *    Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the 
 *    documentation and/or other materials provided with the   
 *    distribution.
 *
 *    Neither the name of Texas Instruments Incorporated nor the names of
 *    its contributors may be used to endorse or promote products derived
 *    from this software without specific prior written permission.
 *
 *  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS 
 *  "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT 
 *  LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
 *  A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT 
 *  OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, 
 *  SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT 
 *  LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 *  DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 *  THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT 
 *  (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE 
 *  OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 * 
 *****************************************************************************/

/******************************************************************************
 *
 * File Name:   evmc66x_spi.c
 *
 * Description: This file implements SPI driver for the NOR flash
 *  
 *****************************************************************************/
 
/************************
 * Include Files
 ************************/

#include "platform_internal.h"
#include "spi.h"
#include "cmosis.h"




static void 
spi_delay
( 
    uint32_t delay 
)
{
    volatile uint32_t i;

    for ( i = 0 ; i < delay ; i++ ){ };
}


/******************************************************************************
 * 
 * Function:    spi_claim
 *
 * Description: This function claims the SPI bus in the SPI controller 
 *
 * Parameters:  Uint32 cs       - Chip Select number for the slave SPI device
 *              Uint32 freq     - SPI clock frequency
 *              uint32_t NBit	- Количество бит в слове
 *
 * Return Value: error status
 * 
 ******************************************************************************/
SPI_STATUS 
spi_claim_oed
(
    uint32_t      cs,
    uint32_t      freq,
    uint8_t      NBit
)
{
    uint32_t scalar;
    spi_release_oed();
    spi_delay (2000);
    PLIBSPILOCK()

    /* Enable the SPI hardware */
    SPI_SPIGCR0 = CSL_SPI_SPIGCR0_RESET_IN_RESET;
    spi_delay (2000);
    SPI_SPIGCR0 = CSL_SPI_SPIGCR0_RESET_OUT_OF_RESET;

    /* Set master mode, powered up and not activated */
    SPI_SPIGCR1 =   (CSL_SPI_SPIGCR1_MASTER_MASTER << CSL_SPI_SPIGCR1_MASTER_SHIFT)   |
                    (CSL_SPI_SPIGCR1_CLKMOD_INTERNAL << CSL_SPI_SPIGCR1_CLKMOD_SHIFT);
    
    
    /* CS0, CS1, CLK, Slave in and Slave out are functional pins */
    if (cs == 0)
		{
			SPI_SPIPC0 =    (CSL_SPI_SPIPC0_SCS0FUN0_SPI << CSL_SPI_SPIPC0_SCS0FUN0_SHIFT) |
							(CSL_SPI_SPIPC0_CLKFUN_SPI << CSL_SPI_SPIPC0_CLKFUN_SHIFT)     |
							(CSL_SPI_SPIPC0_SIMOFUN_SPI << CSL_SPI_SPIPC0_SIMOFUN_SHIFT)   |
							(CSL_SPI_SPIPC0_SOMIFUN_SPI << CSL_SPI_SPIPC0_SOMIFUN_SHIFT);
			SPI_SPIDAT1 =   (CSL_SPI_SPIDAT1_CSHOLD_ENABLE << CSL_SPI_SPIDAT1_CSHOLD_SHIFT) |
			                         (0x02 << CSL_SPI_SPIDAT1_CSNR_SHIFT);
		}
    else if (cs == 1)
		{
			SPI_SPIPC0 =    ((CSL_SPI_SPIPC0_SCS0FUN1_SPI << CSL_SPI_SPIPC0_SCS0FUN1_SHIFT) |
							(CSL_SPI_SPIPC0_CLKFUN_SPI << CSL_SPI_SPIPC0_CLKFUN_SHIFT)     |
							(CSL_SPI_SPIPC0_SIMOFUN_SPI << CSL_SPI_SPIPC0_SIMOFUN_SHIFT)   |
							(CSL_SPI_SPIPC0_SOMIFUN_SPI << CSL_SPI_SPIPC0_SOMIFUN_SHIFT)) & 0xFFFF;
			SPI_SPIDAT1 =   (CSL_SPI_SPIDAT1_CSHOLD_ENABLE << CSL_SPI_SPIDAT1_CSHOLD_SHIFT) |
			                         (0x01 << CSL_SPI_SPIDAT1_CSNR_SHIFT);
		}
    else if (cs == 3)
		{
    		SPI_SPIPC0 =    (CSL_SPI_SPIPC0_SCS0FUN0_SPI << CSL_SPI_SPIPC0_SCS0FUN0_SHIFT) |
    						(CSL_SPI_SPIPC0_SCS0FUN1_SPI << CSL_SPI_SPIPC0_SCS0FUN1_SHIFT)|
    						(CSL_SPI_SPIPC0_CLKFUN_SPI << CSL_SPI_SPIPC0_CLKFUN_SHIFT)     |
    						(CSL_SPI_SPIPC0_SIMOFUN_SPI << CSL_SPI_SPIPC0_SIMOFUN_SHIFT)   |
    						(CSL_SPI_SPIPC0_SOMIFUN_SPI << CSL_SPI_SPIPC0_SOMIFUN_SHIFT);
    		SPI_SPIDAT1 =   (CSL_SPI_SPIDAT1_CSHOLD_ENABLE << CSL_SPI_SPIDAT1_CSHOLD_SHIFT) |
    		                         (0x03 << CSL_SPI_SPIDAT1_CSNR_SHIFT);
		}
    
    /* setup format */
    scalar = ((MY_SPI_MODULE_CLK / freq) - 1 ) & 0xFF;

  //  if ( cs == 0) {
        SPI_SPIFMT0 =   (NBit << CSL_SPI_SPIFMT_CHARLEN_SHIFT)               |
                        (scalar << CSL_SPI_SPIFMT_PRESCALE_SHIFT)                      |
                        (CSL_SPI_SPIFMT_PHASE_DELAY << CSL_SPI_SPIFMT_PHASE_SHIFT)     |
                        (CSL_SPI_SPIFMT_POLARITY_LOW << CSL_SPI_SPIFMT_POLARITY_SHIFT) |
                        (CSL_SPI_SPIFMT_SHIFTDIR_MSB << CSL_SPI_SPIFMT_SHIFTDIR_SHIFT);
   // }else if ( cs == 1) {
     /*   SPI_SPIFMT0 =   (NBit << CSL_SPI_SPIFMT_CHARLEN_SHIFT)               |
                        (scalar << CSL_SPI_SPIFMT_PRESCALE_SHIFT)                      |
                        (CSL_SPI_SPIFMT_PHASE_NO_DELAY << CSL_SPI_SPIFMT_PHASE_SHIFT)     |
                        (CSL_SPI_SPIFMT_POLARITY_LOW << CSL_SPI_SPIFMT_POLARITY_SHIFT) |
                        (CSL_SPI_SPIFMT_SHIFTDIR_MSB << CSL_SPI_SPIFMT_SHIFTDIR_SHIFT);*/
 //   }*/
    

    /* including a minor delay. No science here. Should be good even with
    * no delay
    */
 //   if (cs == 0) {
        SPI_SPIDELAY =  (8 << CSL_SPI_SPIDELAY_C2TDELAY_SHIFT) |
                        (8 << CSL_SPI_SPIDELAY_T2CDELAY_SHIFT);
        /* default chip select register */
        SPI_SPIDEF  = CSL_SPI_SPIDEF_RESETVAL;        
  /*  } else if (cs == 1) {
        SPI_SPIDELAY =  (6 << CSL_SPI_SPIDELAY_C2TDELAY_SHIFT) |
                        (3 << CSL_SPI_SPIDELAY_T2CDELAY_SHIFT);
    }*/
    
    /* no interrupts */
    SPI_SPIINT0 = CSL_SPI_SPIINT0_RESETVAL;
    SPI_SPILVL  = CSL_SPI_SPILVL_RESETVAL;

    /* enable SPI */
 /*   SPI_SPIGCR1 |= ( CSL_SPI_SPIGCR1_ENABLE_ENABLE << CSL_SPI_SPIGCR1_ENABLE_SHIFT)|
    		(CSL_SPI_SPIGCR1_LOOPBACK_MASK);*/
    SPI_SPIGCR1 |= ( CSL_SPI_SPIGCR1_ENABLE_ENABLE << CSL_SPI_SPIGCR1_ENABLE_SHIFT);

    return SPI_EOK;
}

/******************************************************************************
 * 
 * Function:    spi_release  
 *
 * Description: This function releases the bus in SPI controller 
 *
 * Parameters:  None
 *
 * Return Value: None
 * 
 ******************************************************************************/
void 
spi_release_oed
(
    void
)
{
    /* Disable the SPI hardware */
    SPI_SPIGCR1 = CSL_SPI_SPIGCR1_RESETVAL;

    PLIBSPIRELEASE()
    SPI_SPIDAT1=0;
}

void wait_txfull()
{
	 uint16_t temp=0;
	 temp=0;
	 do
	 {
	  		temp++;
	 }
	 while( (SPI_SPIBUF & CSL_SPI_SPIBUF_TXFULL_MASK)&&(temp<500));
}

void wait_rxfull()
{
	 uint16_t temp=0;
	 temp=0;
	 do
	 {
		 temp++;
	 }
	 while ( (SPI_SPIBUF & CSL_SPI_SPIBUF_RXEMPTY_MASK)&&(temp<500));
}

void write_enable(uint8_t cs)
{
	if(cs==0)
		SPI_SPIPC0=SPI_SPIPC0|0x01;
	else
		SPI_SPIPC0=SPI_SPIPC0|0x02;

	SPI_SPIDAT0=0x06;
	wait_rxfull();
	SPI_SPIPC0 =SPI_SPIPC0&0xffffff00;
	spi_delay (50);
}

uint8_t read_status(uint8_t cs)
{
	if(cs==0)
		SPI_SPIPC0=SPI_SPIPC0|0x01;
	else
		SPI_SPIPC0=SPI_SPIPC0|0x02;

	SPI_SPIDAT0 = 0x05;//чтение регистра флагов
	wait_rxfull();
	SPI_SPIDAT0 = 0;
	wait_rxfull();
	SPI_SPIPC0 =SPI_SPIPC0&0xffffff00;
	spi_delay (50);
	return (SPI_SPIBUF&0xff);
}
/*****************************************************************************
 * Function:spi_cmos_read_flash
 ****************************************************************************/
void spi_cmos_read_flash(uint32_t size, uint8_t cs,uint16_t *buff, uint32_t addr)
{
	int i=0;
	spi_claim_oed(cs,SPI_MIN_FREQ,16);
	spi_cmos_flash_read_command(0x03,&addr);	//Команда на чтение

	for(i=0;i<(size/2);i++)
	{
		spi_cmos_flash_read_data(5,&buff[i]);//ЧИТАЕМ 2 БАЙТА
	}

	spi_release_oed();
}
/*****************************************************************************
 * Function:spi_cmos_write_flash
 ****************************************************************************/
void spi_cmos_write_flash(uint32_t size, uint8_t cs,uint8_t *buff, uint32_t addr)
{
	uint8_t command=0x02;			//прошивка страницы данных

    uint16_t data0=0,data1=0,data2=0,data3=0,temp=0,status=0;
    uint32_t i=0,j=0,numsector=0;


	spi_claim_oed(cs,SPI_MIN_FREQ,8);	//настройка spi на передачу 8 бит по заданному каналу с заданной частотой

    numsector=(size/256)+1;
    for(i=0;i<numsector;i++)
    {

    	write_enable(cs);				//передаем команду на разрешение записи

    	//read status register
    	status=read_status(cs);			//считываем статус

        SPI_SPIBUF;
    	if(cs==0)
    		SPI_SPIPC0=SPI_SPIPC0|0x01;
    	else
    		SPI_SPIPC0=SPI_SPIPC0|0x02;

        data0=command;
        data1=(addr&0xff0000)>>16;
        data2=(addr&0x00ff00)>>8;
        data3=addr&0xff;

        SPI_SPIDAT0 =data0;

    	wait_txfull();
    	wait_rxfull();
        SPI_SPIDAT0 =data1;

    	wait_txfull();
    	wait_rxfull();
        SPI_SPIDAT0 =data2;

    	wait_txfull();
    	wait_rxfull();
        SPI_SPIDAT0 =data3;

    	for(j=0;j<256;j++)
    	{
    		wait_txfull();
    		wait_rxfull();
    		SPI_SPIDAT0 =buff[j+i*256];
    	}

    	wait_txfull();
    	wait_rxfull();
    	SPI_SPIPC0 =SPI_SPIPC0&0xffffff00;
    	spi_delay (50);

        temp=0;
        do
        {
        	status=read_status(cs);
        	spi_delay (100);
        	temp++;
        }
        while((status!=0)&&(temp<100000));
                addr+=0x100;
    }

    spi_release_oed();		//дезактивируем spi
    spi_delay (10000);
}

/*****************************************************************************
 * Function:spi_cmos_erase_all_flash
 ****************************************************************************/
void spi_cmos_erase_all_flash(uint32_t size, uint8_t cs)
{
	uint8_t command=0xd8;			//очистка сектора 64кб

    uint16_t data0=0,data1=0,data2=0,data3=0,temp=0,status=0;
    uint32_t i=0,numsector=0,addr=0;


	spi_claim_oed(cs,SPI_MIN_FREQ,8);	//настройка spi на передачу 8 бит по заданному каналу с заданной частотой

    numsector=(size/0xffff)+1;
    for(i=0;i<numsector;i++)
    {

    		write_enable(cs);				//передаем команду на разрешение записи

    	//read status register
    	status=read_status(cs);			//считываем статус

        SPI_SPIBUF;
    	if(cs==0)
    		SPI_SPIPC0=SPI_SPIPC0|0x01;
    	else
    		SPI_SPIPC0=SPI_SPIPC0|0x02;

        data0=command;
        data1=(addr&0xff0000)>>16;
        data2=(addr&0x00ff00)>>8;
        data3=addr&0xff;

        SPI_SPIDAT0 =data0;

    	wait_txfull();
    	wait_rxfull();
        SPI_SPIDAT0 =data1;

    	wait_txfull();
    	wait_rxfull();
        SPI_SPIDAT0 =data2;

    	wait_txfull();
    	wait_rxfull();
        SPI_SPIDAT0 =data3;

    	wait_txfull();
    	wait_rxfull();
    	SPI_SPIPC0 =SPI_SPIPC0&0xffffff00;
    	spi_delay (50);

        temp=0;
        do
        {
        	status=read_status(cs);
        	spi_delay (100);
        	temp++;
        }
        while((status!=0)&&(temp<100000));
                addr+=0x10000;
    }

    spi_release_oed();		//дезактивируем spi
}
/*****************************************************************************
 * Function: spi_cmos_flash_read_data
 ******************************************************************************/
void spi_cmos_flash_read_data(uint8_t command,uint16_t* data_in)
{
	 uint16_t data0=0,RW=0;

	    /* Clear out any pending read data */
	    SPI_SPIBUF;
	    data0=(RW<<15)|(command<<8);

    	wait_txfull();
    	wait_rxfull();
        /* Set the TX data to SPIDAT1 */
        SPI_SPIDAT0 = data0;

    	wait_txfull();
   // 	wait_rxfull();

    	data0=SPI_SPIBUF&0xffff;
    	data0=(data0>>8)+((data0<<8)&0xff00);
       /* Read one byte data */
    //  *data_in = SPI_SPIBUF&0xffff;
    	*data_in=data0;

}
/******************************************************************************
 *Function: spi_cmos_flash_read_command
 ******************************************************************************/

void spi_cmos_flash_read_command(uint8_t command,uint32_t* addr)
{
    uint16_t data0=0,data1=0,RW=0;

    /* Clear out any pending read data */
    SPI_SPIBUF;
    data0=(RW<<15)|(command<<8)|(((*addr)&0xff0000)>>16);
    data1=((*addr)&0xffff);

        /* Wait untill TX buffer is not full */

    	wait_txfull();
        /* Set the TX data to SPIDAT1 */
        SPI_SPIDAT0 = data0;

        wait_txfull();
        SPI_SPIDAT0 = data1;

        wait_rxfull();

        //dummy cycle
        SPI_SPIDAT0 = 0;
         wait_rxfull();
         /* Read data */
        SPI_SPIBUF;
}

/******************************************************************************
 * 
 * Function:    spi_rwreg_oed
 *
 * Description: This function sends and receives N-bit data serially
 *
 ******************************************************************************/
SPI_STATUS spi_rwreg_oed(uint8_t reg,uint16_t* data_out,uint16_t* data_in,uint8_t RW)
{
    uint16_t data=0;
    

    /* Clear out any pending read data */
    SPI_SPIBUF;
    data=(RW<<15)|(reg<<8)|((*data_out)&0xff);

        /* Wait untill TX buffer is not full */

    		wait_txfull();
        
        /* Set the TX data to SPIDAT1 */
            SPI_SPIDAT0 = data;

            wait_rxfull();


         /* Read one byte data */
        *data_in = SPI_SPIBUF&0xffff;

    return SPI_EOK;
    
}



#define COLREGCMOS	33
void spi_init_cmos_oed(int cs)
{
	uint16_t initbuff[COLREGCMOS][2],i=0;
    
		initbuff[0][0]=EXPEXT;	//накопление по регистрам и без чередования
		initbuff[0][1]=4;

		initbuff[1][0]=LVDSREC;	//не известен
		initbuff[1][1]=0;

		initbuff[2][0]=CALIB;	//не известен
		initbuff[2][1]=0;

		initbuff[3][0]=ICOL;	//не известен
		initbuff[3][1]=4;

		initbuff[4][0]=ICOLPRECH;	//не известен
		initbuff[4][1]=1;

		initbuff[5][0]=IAMP;	//не известен
		initbuff[5][1]=12;

		initbuff[6][0]=VTFL1;	//не известен
		initbuff[6][1]=64;

		initbuff[7][0]=VRESLOW;	//не известен
		initbuff[7][1]=64;

		initbuff[8][0]=VPRECHARGE;	//не известен
		initbuff[8][1]=101;

		initbuff[9][0]=VREF;	//не известен
		initbuff[9][1]=106;

		initbuff[10][0]=VRAMP1;	//регулировка неравномерности подъиграть
		initbuff[10][1]=110;

		initbuff[11][0]=VRAMP2;	//регулировка неравномерности подъиграть
		initbuff[11][1]=110;

		initbuff[12][0]=PGA;	//аналоговое усиление ставим равное 1
		initbuff[12][1]=0;

		initbuff[13][0]=PGA2;
		initbuff[13][1]=0;

		initbuff[14][0]=ADCGAIN;	//цифровое усиление то же ставим равное 1
		initbuff[14][1]=40;

		initbuff[15][0]=DUMMY;
		initbuff[15][1]=1;

		initbuff[16][0]=VBLACKSUN;	////не известен
		initbuff[16][1]=98;

		initbuff[17][0]=CHENEN1;	//включение всех LVDS каналов
		initbuff[17][1]=0xff;

		initbuff[18][0]=CHENEN2;
		initbuff[18][1]=0xff;

		initbuff[19][0]=CHENEN3;
		initbuff[19][1]=0x3;

		//снимаем 400 строк по центру матрицы по умолчанию

		initbuff[20][0]=NUMBERLINESLO;
		initbuff[20][1]=0x90;

		initbuff[21][0]=NUMBERLINESHI;
		initbuff[21][1]=0x1;

		initbuff[22][0]=STARTLINELO;
		initbuff[22][1]=0xb3;

		initbuff[23][0]=STARTLINEHI;
		initbuff[23][1]=0x1;

		initbuff[24][0]=EXPTIME1;
		initbuff[24][1]=1;				//минимальное время накопления

		initbuff[25][0]=EXPTIME2;
		initbuff[25][1]=0x00;

		initbuff[26][0]=EXPTIME3;
		initbuff[26][1]=0;

		initbuff[27][0]=TDIG1;		//регистры
		initbuff[27][1]=0;

		initbuff[28][0]=TDIG2;
		initbuff[28][1]=0;

		initbuff[29][0]=IMAGEFLIPP;	//зеркальный поворот изображения
		initbuff[29][1]=0;

		//Настройка уровня черного
		initbuff[30][0]=OFFSETLO;	//Смещение черного по обоим каналам 16330
		initbuff[30][1]=0xCA;		//младшая часть

		initbuff[31][0]=OFFSETHI;	//зеркальный поворот изображения
		initbuff[31][1]=0x3f;

		initbuff[32][0]=REG73;	//Значение 73 регистра
		initbuff[32][1]=10;




		spi_claim_oed(cs,SPI_MIN_FREQ,16);

		for(i=0;i<COLREGCMOS;i++)
		{
			spi_rwreg_oed(initbuff[i][0],&initbuff[i][1],&initbuff[i][1],1);	//записываем все регистры
		}
///////////////////////чтение
	/*	for(i=0;i<COLREGCMOS;i++)
		{
			spi_rwreg_oed(initbuff[i][0],&initbuff[i][1],&initbuff[i][1],0);	//читаем все регистры
		}*/

		spi_release_oed();


}

#define COLREGCMOSXILINX	5
void spi_init_xilinx_cmos_oed(uint16_t command,uint16_t   cs)
{
	uint16_t initbuff[COLREGCMOSXILINX][2],i=0;

		initbuff[0][0]=0;
		initbuff[0][1]=command;

		initbuff[1][0]=1;
		initbuff[1][1]=0;

		initbuff[2][0]=2;
		initbuff[2][1]=0;

		initbuff[3][0]=3;
		initbuff[3][1]=0;

		initbuff[4][0]=4;
		initbuff[4][1]=0;


		spi_claim_oed(cs,SPI_MIN_FREQ,16);

		for(i=0;i<1;i++)
		{
			spi_rwreg_oed(initbuff[i][0],&initbuff[i][1],&initbuff[i][1],1);	//записываем все регистры
		}


		spi_delay (500);

		spi_release_oed();


}


