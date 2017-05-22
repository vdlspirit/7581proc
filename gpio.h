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
 * File	Name:	evmc66x_gpio.h
 *
 * Description:	This file is the header	file for GPIO module
 *
 *
 ******************************************************************************/
#ifndef	GPIO_H_	
#define	GPIO_H_	

/************************
 * Defines and Macros
 ************************/ 

#define	GPIO_0					(0)
#define	GPIO_1					(1)
#define	GPIO_2					(2)
#define	GPIO_3					(3)
#define	GPIO_4					(4)
#define	GPIO_5					(5)
#define	GPIO_6					(6)
#define	GPIO_7					(7)
#define	GPIO_8					(8)
#define	GPIO_9					(9)
#define	GPIO_10					(10)
#define	GPIO_11					(11)
#define	GPIO_12					(12)
#define	GPIO_13					(13)
#define	GPIO_14					(14)
#define	GPIO_15					(15)

#define	GPIO_DATAMASK			(0x0000FFFF)

#define	GPIO_MAX_NUMBER			(15)

#define	GPIO_LOW				(0)
#define	GPIO_HIGH				(1)
#define	INVALID_GPIO_NUMBER		(2)
#define	INVALID_GPIO_DIRECTION	(3)
#define	INVALID_GPIO_STATE		(4)


typedef	enum _GpioDirection
{
	GPIO_OUT = 0,
	GPIO_IN	
}GpioDirection;	

#define BASE_GPIO_ADDR		0x2320000
#define GPIO_DIR 		*(volatile unsigned int*)(BASE_GPIO_ADDR+0x10)
#define GPIO_OUT_DATA 	*(volatile unsigned int*)(BASE_GPIO_ADDR+0x14)
#define GPIO_IN_DATA 	*(volatile unsigned int*)(BASE_GPIO_ADDR+0x20)

#define BASE_MDIO_ADDR		0x2090000
#define MDIO_CONTROL	*(volatile unsigned int*)(BASE_MDIO_ADDR+0x304)
#define MDIO_LIVE 		*(volatile unsigned int*)(BASE_MDIO_ADDR+0x308)
#define MDIO_LINK 		*(volatile unsigned int*)(BASE_MDIO_ADDR+0x30c)

//MDIO declaration

#define MDIO_CMOS1_INIT			0xffE0
#define MDIO_CMOS2_INIT			0xff98
#define MDIO_CMOS_INIT			0xff80

#define MDIO_XILINXCMOS_INIT	0xffd0

#define LOADKADR1MEM	0x81000000
#define LOADKADR2MEM	0x81200000

typedef struct GPIO
{
	union GpioUnion{
			Uint32 sw;
			struct {
					Uint32 EMIFCE3	 	:1;	//0-Flash M28	1-FRAM
					Uint32 Ether0		:1; //Включение 0 Ethernet
					Uint32 Ether1		:1; //Включение 1 Ethernet
					Uint32 SPICS0		:2;	//Выбор платы ПВП1
					Uint32 SPICS1		:2;	//Выбор платы ПВП2
					Uint32 SrioRes		:1;	//Включение SRIO
					Uint32 Strobe		:1;	//Имитация строба
					Uint32 A20			:1;	//Бит адреса A20
					Uint32 A21			:1;	//Бит адреса A21
					Uint32 reserv		:21;
				} s;
			} GpioData;
} GPIOPACKET;

/************************
 * Function declarations
 ************************/
void gpioInit(void);
void gpioWriteDataBus(unsigned short uchValue);
UInt16 gpioReadDataBus(void);

#endif // GPIO_H_


