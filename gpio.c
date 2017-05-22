
/* Platform utilities include */
#include "ti/platform/platform.h"

/* Resource manager for QMSS, PA, CPPI */
#include "ti/platform/resource_mgr.h"
#include "gpio.h"

#include <ti/csl/csl_psc.h>
#include <ti/csl/csl_pscAux.h>
#include <ti/csl/csl_psc.h>
#include <ti/csl/csl_gpio.h>
#include <ti/csl/csl_gpioAux.h>

#define GPIOBANKNUM     (0)
extern GPIOPACKET gpio;

void gpioInit()
{
	int32_t pinNum;
    CSL_GpioHandle  hGpio;

    // Open the CSL GPIO Module 0
    hGpio = CSL_GPIO_open (0);
    GPIO_DIR=0xffffffff;

	// Disable the GPIO global interrupts
	CSL_GPIO_bankInterruptDisable(hGpio, GPIOBANKNUM);

	GPIO_OUT_DATA=0xfffe;
	// Clear all falling edge trigger and rising edge trigger
	for (pinNum = GPIO_0; pinNum <= GPIO_15; pinNum++) {
        CSL_GPIO_clearFallingEdgeDetect(hGpio, pinNum);
        CSL_GPIO_clearRisingEdgeDetect (hGpio, pinNum);
    }

	//Устанавливаем отработку прерываний по переднему и заднему фронту для GPIO12-GPIO15

	CSL_GPIO_setFallingEdgeDetect(hGpio, GPIO_12);	//срабатываем по заднему фронту
	CSL_GPIO_setFallingEdgeDetect(hGpio, GPIO_13);	//срабатываем по заднему фронту

	CSL_GPIO_setRisingEdgeDetect (hGpio, GPIO_14);	//срабатываем по переднему фронту
	CSL_GPIO_setRisingEdgeDetect (hGpio, GPIO_15);	//срабатываем по переднему фронту


	// Set  GPIO0-GPIO11 as output
	GPIO_DIR=0xfffff000;
//	GPIO_DIR=0xffff0000;
	MDIO_CONTROL=0x410000ff;

	// Enable the GPIO global interrupts
	CSL_GPIO_bankInterruptEnable(hGpio, GPIOBANKNUM);
}

void gpioWriteDataBus(unsigned short uchValue)
{
	GPIO_OUT_DATA=uchValue;
}

UInt16 gpioReadDataBus()
{
	return GPIO_IN_DATA;
}
void gpioStrobBlink()
{
	int mycount=0;
			gpio.GpioData.sw=gpioReadDataBus();

	 /*	 	gpio.GpioData.s.Strobe=1;

			//gpioWriteDataBus(0xff00);
			gpioWriteDataBus(gpio.GpioData.sw);
			int mycount=0;
			for(mycount=0;mycount<20;mycount++)
			{}*/

			gpio.GpioData.s.Strobe=0;
			gpioWriteDataBus(gpio.GpioData.sw);
			for(mycount=0;mycount<20;mycount++)
			{}

			gpio.GpioData.s.Strobe=1;
			gpioWriteDataBus(gpio.GpioData.sw);

}

