#include <stdio.h>
#include <ti/ndk/inc/netmain.h>
#include <ti/ndk/inc/_stack.h>
#include <ti/ndk/inc/tools/servers.h>
#include <ti/sysbios/knl/Task.h>
#include <ti/sysbios/family/c66/Cache.h>

#include "client.h"
#include "PaketOEDTAEth.h"
#include "BattleJob.h"
#include "InitCache.h"
#include "gpio.h"
#include "spi.h"
#include "emif16.h"
#include "flash_programm.h"



/* BIOS6 include */
#include <ti/sysbios/BIOS.h>

/* Platform utilities include */
#include "ti/platform/platform.h"

/* Resource manager for QMSS, PA, CPPI */
#include "resource_mgr.h"

/* CSL EMAC include */
#include <ti/csl/csl_cpsw.h>
#include <ti/csl/csl_cpsgmii.h>
#include <ti/csl/csl_cpsgmiiAux.h>
#include <ti/csl/cslr_cpsgmii.h>
#include <ti/csl/csl_mdio.h>
#include <ti/csl/csl_mdioAux.h>

/* IPC includes */
#include <ti/ipc/GateMP.h>
#include <ti/ipc/Ipc.h>
#include <ti/ipc/ListMP.h>
#include <ti/ipc/SharedRegion.h>
#include <ti/ipc/MultiProc.h>

#include "setup6678.h"

#include "tiboot.h"
#include "mycore.h"
#include "cmosis.h"
#include "srio.h"


#pragma  DATA_SECTION (emif4Cfg, ".aemif4Cfg")

volatile const BOOT_EMIF4_TBL_T  emif4Cfg =  {

//volatile const BOOT_EMIF4_TBL_T  mass =  {

    BOOT_EMIF4_ENABLE_MSW_pllCtl          | \
    BOOT_EMIF4_ENABLE_MSW_sdRamTiming1    | \
    BOOT_EMIF4_ENABLE_MSW_sdRamTiming2    | \
    BOOT_EMIF4_ENABLE_MSW_sdRamTiming3    | \
    BOOT_EMIF4_ENABLE_MSW_ddrPhyCtl1      | \
    BOOT_EMIF4_ENABLE_MSW_sdRamRefreshCtl | \
	BOOT_EMIF4_ENABLE_MSW_sdRamOutImpdedCalCfg | \
	BOOT_EMIF4_ENABLE_MSW_sdRamConfig,
//	BOOT_EMIF4_ENABLE_MSW_sdRamConfig2| \
//	BOOT_EMIF4_ENABLE_MSW_lpDdrNvmTiming,


//	BOOT_EMIF4_ENABLE_ALL_MSW,
//		0xf, //!!!!!!!!!!!!!!!!!! Interest

//    BOOT_EMIF_ENABLE_SLSW_config0 | \
//	BOOT_EMIF_ENABLE_SLSW_config6 | \
//	BOOT_EMIF_ENABLE_SLSW_config7 | \
//	BOOT_EMIF_ENABLE_SLSW_config8 | \
//	BOOT_EMIF_ENABLE_SLSW_config9 | \
//	BOOT_EMIF_ENABLE_SLSW_config10 | \
//	BOOT_EMIF_ENABLE_SLSW_config18 | \
//	BOOT_EMIF_ENABLE_SLSW_config19 | \
//	BOOT_EMIF_ENABLE_SLSW_config20 | \
//	BOOT_EMIF_ENABLE_SLSW_config22 | \
//	BOOT_EMIF_ENABLE_SLSW_config12 | \
//	BOOT_EMIF_ENABLE_SLSW_config23 | \
//	BOOT_EMIF_ENABLE_SLSW_config21,      // Config select slsw
//  0,      // Config select lsw
//	(0<<16)+(20<<8)+2, //pllconfig
//	(1<<16)+(20<<8)+2, //pllconfig
   2, //1, 	//3,      // pllPrediv // 2
	//3,      // pllPrediv
   17, //20,	//40,     // pllMult //17
	//40,     // pllMult
   //2,	//2,      // pllPostDiv
	2,      // pllPostDiv
   ////0x63C452B2, // sdRamConfig
   //0x62477AB2,  // sdRamConfig
   //////0x63066A32,  // sdRamConfig
   0x63062A32,  // sdRamConfig
    0,           // sdRamConfig2, dont care
    //0x000030D4,
    ////0x0000144F,  // sdRamRefreshCtl
    0x00001450,  // sdRamRefreshCtl
    //0x0AAAE51B,
    0x1333780C,  // sdRamTiming1
    //0x2A2F7FDA,
    0x30717FE3,  // sdRamTiming2
    //0x057F82B8,
    0x559F86AF,  // sdRamTiming3

    0,           // lpDdrNvmTiming, dont care //
    0,           // powerManageCtl, dont care //
    0,           // iODFTTestLogic, dont care //
    0,           // performCountCfg, dont care //
    0,           // performCountMstRegSel, dont care //
    0,           // readIdleCtl, dont care //
    0,           // sysVbusmIntEnSet, dont care //

    0x70073214,	//	0, //0x70074c1f,  // sdRamOutImpdedCalCfg, dont care //
    0,           // tempAlterCfg, dont care //

    //0x0010010d,
    0x0010010F,  // ddrPhyCtl1 //

    0,           // ddrPhyCtl2, dont care //
    0,           // priClassSvceMap, dont care //
    0,           // mstId2ClsSvce1Map, dont care //
    0,           // mstId2ClsSvce2Map, dont care //
    0,           // eccCtl, dont care //
    0,           // eccRange1, dont care //
    0,           // eccRange2, dont care //
    0x305, //0,           // rdWrtExcThresh, dont care //

    0x87A0047F, 0, 0, 0, 0, 0, 0x33, 0x3A,
    0x2C, 0x2C, 0x21, 0, 0xAF00002, 0, 0, 0,
    0, 0, 0xB7, 0xB1, 0xA4, 0xA4, 0x98, 0x200,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0
    };

//---------------------------------------------------------------------------
#define IPCGR_REG  ((volatile unsigned long*)0x02620240)
#define REBOOT_BASE 0x1087FFFF
#define REBOOT_GAP  0x01000000
extern void _c_int00(void); //Entry point ELF ABI (from libc): initialize data segments and jump to main()

extern void StartStopEth(int ss);
extern void gpioStrobBlink();
volatile int RunStop3=0;
// When USE_OLD_SERVERS set to zero, server daemon is used
#define USE_OLD_SERVERS 0
#define _INCLUDE_NIMU_CODE

//---------------------------------------------------------------------------
// Title String
//
char *VerStr = "\nTCP/IP Stack Example Client\n";

// Our NETCTRL callback functions

HANDLE hSendVideo = 0;					//поток отправки видео
HANDLE hSrioInputVideo=0;				//поток получения видео
HANDLE hEthInputOutput=0;				//поток получения видео

static void SrioTask();
static void StackTest();

static void   NetworkOpen();
static void   NetworkClose();
static void   NetworkIPAddr( IPN IPAddr, uint IfIdx, uint fAdd );
extern Int32 enableALL_srio (void);

/* Platform Information - we will read it form the Platform Library */
platform_info	gPlatformInfo;
PLATFORM_EMAC_EXT_info mac_info;

extern TATOOED ta1;						//структура пакета ТА цель 1
extern TATOOED ta2;						//структура пакета ТА цель 2
extern OEDTOTA oed1;					//структура пакета ОЭД цель 1
extern OEDTOTA oed2;					//структура пакета ОЭД цель 2
extern VideoTechParam videotechparam;
extern VideoTechPacket videotechpacket;
extern VisualKommandPaket vkp;				//структура команд от визуализатора
extern VisualStatusPaket vsp;				//структура заголовка кадра
extern CMOS cmos1config;	//регистры управления 1 матрицей
extern CMOS cmos2config;	//регистры управления 2 матрицей
extern SRIOPACKET srioUpac;
extern BattleParam battle;					//боевая структура управления программой
extern BattleParam2 battle2;				//боевая структура управления программой цель 2
extern Uint8 GlobalVideoBuff[0x800000+sizeof(VisualKadrPaket)];

#pragma DATA_SECTION(gpio, ".MyCash")
GPIOPACKET gpio;

extern uint8 BuffOED1[SizeBuffOED];			//буффер на передачу покета ОЭД1
extern uint8 BuffTA1[SizeBuffTA];				//буффер на прием пакета ТА1
extern uint8 BuffOED2[SizeBuffOED];			//буффер на передачу покета ОЭД2
extern uint8 BuffTA2[SizeBuffTA];				//буффер на прием пакета ТА2

extern uint32 BuffVideoParamPacket[8];			//буффер параметров видео пакета

extern Void SrioRunJob();
extern void GlobalResetEthQmss();

//

volatile UInt32*     isInitialized =(UInt32*) MCSRAMCASH;
volatile UInt32*     isMathBuffVideo =(UInt32*) MCSRAMMYVIDEO;

volatile UInt32*     NoBoot =(UInt32*) (MCSRAMCASH+20);


//---------------------------------------------------------------------------
// Configuration
//

char *HostName    = "tidsp";
char *LocalIPAddr = "192.168.0.2";
char *LocalIPMask = "255.255.255.0";    // Not used when using DHCP
char *GatewayIP   = "192.168.0.100";    // Not used when using DHCP
char *DomainName  = "demo.net";         // Not used when using DHCP
char *DNSServer   = "0.0.0.0";          // Used when set to anything but zero


//#define     PACKET_SIZE     1000
//#define     PACKET_COUNT    10

/*************************************************************************
 *  @b EVM_init()
 *
 *  @n
 *
 *  Initializes the platform hardware. This routine is configured to start in
 * 	the evm.cfg configuration file. It is the first routine that BIOS
 * 	calls and is executed before Main is called. If you are debugging within
 *  CCS the default option in your target configuration file may be to execute
 *  all code up until Main as the image loads. To debug this you should disable
 *  that option.
 *
 *  @param[in]  None
 *
 *  @retval
 *      None
 ************************************************************************/


void Init_Video_System()
{
	gpioWriteDataBus(MDIO_XILINXCMOS_INIT);	//выбираем XilinxCmos

	gpio.GpioData.sw=gpioReadDataBus();
	gpio.GpioData.s.SPICS0=0x1;
	gpio.GpioData.s.SPICS1=0x1;

	gpioWriteDataBus(gpio.GpioData.sw);	//выбираем XilinxCmos
	spi_init_xilinx_cmos_oed(2,0);	//настройка Xilinx 0 матрицы
	spi_init_xilinx_cmos_oed(2,1);	//настройка Xilinx 1 матрицы


	gpio.GpioData.sw=gpioReadDataBus();
	gpio.GpioData.s.SPICS0=0x0;
	gpio.GpioData.s.SPICS1=0x0;
	gpioWriteDataBus(gpio.GpioData.sw);

	spi_init_cmos_oed(0);
	spi_init_cmos_oed(1);

/*	gpioWriteDataBus(MDIO_CMOS_INIT);	//выбираем все Cmos
	spi_init_cmos_oed();*/


	gpio.GpioData.sw=gpioReadDataBus();
	gpio.GpioData.s.SPICS0=0x1;
	gpio.GpioData.s.SPICS1=0x1;
	gpioWriteDataBus(gpio.GpioData.sw);

	spi_init_xilinx_cmos_oed(0,0);	//настройка Xilinx 0 матрицы
	spi_init_xilinx_cmos_oed(0,1);	//настройка Xilinx 1 матрицы

}
volatile unsigned int voli=1;
volatile unsigned int isStart=1;

void EVM_init()
{
	platform_init_flags  	sFlags;
	platform_init_config 	sConfig;
	/* Status of the call to initialize the platform */
	int32_t pform_status;

	/*
	 * You can choose what to initialize on the platform by setting the following
	 * flags. Things like the DDR, PLL, etc should have been set by the boot loader.
	*/
	//init gpio
	if (CSL_chipReadReg (CSL_CHIP_DNUM) == 0)
	{
		/* Disabling PA PDSP to prepare for power down */
			volatile unsigned int *passPDSP;
			passPDSP = (unsigned int*)PASS_PDSP0;
			*passPDSP &= 0xFFFFFFFD;

			passPDSP = (unsigned int*)PASS_PDSP1;
			*passPDSP &= 0xFFFFFFFD;

			passPDSP = (unsigned int*)PASS_PDSP2;
			*passPDSP &= 0xFFFFFFFD;

			passPDSP = (unsigned int*)PASS_PDSP3;
			*passPDSP &= 0xFFFFFFFD;

			passPDSP = (unsigned int*)PASS_PDSP4;
			*passPDSP &= 0xFFFFFFFD;

			passPDSP = (unsigned int*)PASS_PDSP5;
			*passPDSP &= 0xFFFFFFFD;

			setModulePower(CSL_PSC_LPSC_CPGMAC, CSL_PSC_PD_PASS, 0);
			setModulePower(CSL_PSC_LPSC_Crypto, CSL_PSC_PD_PASS, 0);
			setDomainAndModulePower(CSL_PSC_LPSC_PKTPROC, CSL_PSC_PD_PASS, 0);

		setupCPU();
		gpioInit();

		Init_Video_System();

		gpio.GpioData.sw=gpioReadDataBus();
		gpio.GpioData.s.Ether0=0;
		gpio.GpioData.s.Ether1=0;
		gpioWriteDataBus(gpio.GpioData.sw);
	}

	else {
		while(!voli);
	}


	memset( (void *) &sFlags,  0, sizeof(platform_init_flags));
	memset( (void *) &sConfig, 0, sizeof(platform_init_config));

	sFlags.pll  = 0;	/* PLLs for clocking  	*/
	sFlags.ddr  = 0;   	/* External memory 		*/
    sFlags.tcsl = 1;	/* Time stamp counter 	*/
    if (CSL_chipReadReg (CSL_CHIP_DNUM) == 0)
    sFlags.phy  = 1;	/* Ethernet 			*/
    else
    sFlags.phy  = 0;	/* Ethernet 			*/
  	sFlags.ecc  = 0;	/* Memory ECC 			*/

    sConfig.pllm = 0;	/* Use libraries default clock divisor */

	pform_status = platform_init(&sFlags, &sConfig);


	if (pform_status != Platform_EOK) {
		while (1) {
		}
	}
}

//---------------------------------------------------------------------
// Main Entry Point
//---------------------------------------------------------------------
int timercount =0;
int main()
{
//	InitCache();
	UInt16 test;

	if(NoBoot[0]==0xaa5432bb)//блок загрузки ядер
	{
		for(;;)
		{
			if(NoBoot[0]==0)
				break;
			Cache_inv((void *) &NoBoot[0], 4, Cache_Type_L1D, TRUE);
		}
	}

	RunStop3=0;		///при прошивке убрать!!!!!!!!
	Cache_wb((void *) &RunStop3, 4, Cache_Type_L1D, TRUE);

	Task_Params     taskParams;
	volatile UInt32 ui=emif4Cfg.pllMult;

	if (CSL_chipReadReg (CSL_CHIP_DNUM) == 0)
	{
		Cache_inv((void *) &RunStop3, 4, Cache_Type_L1D, TRUE);

		int i;
		 if(!RunStop3) {

	        for(i=1; i<6; ++i)  //copy core 0 L2 to core 1..7 L2 (using global adddress, not cached)
	        {
	          memcpy((void*)(0x10800000+0x01000000*i), (void*)0x00800000, 512*1024-0xD23F);
	          unsigned int magic=REBOOT_BASE+(REBOOT_GAP*i);
	          *((volatile unsigned int*)magic)=(unsigned int)_c_int00;  //set the slaves core entry point
	//          *((volatile unsigned int*)magic)=(unsigned int)0x8098CFC0;  //set the slaves core entry point
	//          *((volatile unsigned int*)magic)=(unsigned int)0xc0cf9880;  //set the slaves core entry point
	        }
		 }
//	        for(i=1; i<2; ++i)
//	        {
//	          IPCGR_REG[i]=1;  //sent IPC interrupt to slave cores
//	        }



		test=gpioReadDataBus();
		EmifInit();
		test=test&0xf978;
		gpioWriteDataBus(test);

		timercount=0;

		Task_Params_init(&taskParams);
		taskParams.stackSize=0x1400;
		taskParams.priority=5;
		hEthInputOutput = Task_create(StackTest, &taskParams, NULL);
	}
	else {
		for(voli=0;voli<100000;voli++) {
			asm("  nop");
		//	asm("  nop");
//			asm("  nop");
	//		asm("  nop");
		}

	}
	if(CSL_chipReadReg (CSL_CHIP_DNUM)==1)
	{
		Task_Params_init(&taskParams);
		taskParams.stackSize=65536;
		taskParams.priority=5;
		hSrioInputVideo = Task_create(SrioTask, &taskParams, NULL);
	}

	if(CSL_chipReadReg (CSL_CHIP_DNUM)==2)
	{
		MyStartCore2();
	}
	if(CSL_chipReadReg (CSL_CHIP_DNUM)==3)
	{
		MyStartCore3();
	}
	if(CSL_chipReadReg (CSL_CHIP_DNUM)==4)
	{
		MyStartCore4();
	}
	if(CSL_chipReadReg (CSL_CHIP_DNUM)==5)
	{
		MyStartCore5();
	}

	BIOS_start ();
}

//
// Main Thread
//
//
static void StackTest()
{
    int               rc;
    HANDLE            hCfg;
    QMSS_CFG_T      qmss_cfg;
    QMSS_CFG_T      qmss_cfg_srio;
    CPPI_CFG_T      cppi_cfg;



	/* Get information about the platform so we can use it in various places */

  //  Ipc_start();

  /*  memset( (void *) &gPlatformInfo, 0, sizeof(platform_info));
	(void) platform_get_info(&gPlatformInfo);
	platform_get_emac_info(0,&mac_info);
	platform_get_emac_info(1,&mac_info);*/

	(void) platform_uart_init();
	(void) platform_uart_set_baudrate(115200);
	(void) platform_write_configure(PLATFORM_WRITE_ALL);

    if(CSL_chipReadReg (CSL_CHIP_DNUM)==0)
    {

    	Cache_inv((void *) &RunStop3, 4, Cache_Type_L1D, TRUE);
    	// start second proc
		 if(!RunStop3) {
		//	 while(!voli);
		//	 if(isStart)
		//	 {
				 IPCGR_REG[1]=1;  //sent IPC interrupt to slave cores
				 IPCGR_REG[2]=1;  //sent IPC interrupt to slave cores
				 IPCGR_REG[3]=1;  //sent IPC interrupt to slave cores
				 IPCGR_REG[4]=1;  //sent IPC interrupt to slave cores
				 IPCGR_REG[5]=1;  //sent IPC interrupt to slave cores
		//	 }
			 RunStop3=1;
			 Cache_wb((void *) &RunStop3, 4, Cache_Type_L1D, TRUE);
		 	 }

    	while (isInitialized[0] != 0x0001abcd)
    		Cache_inv((void *) &isInitialized[0], 4, Cache_Type_L1D, TRUE);

        qmss_cfg.master_core        = 1;
        qmss_cfg_srio.master_core	= 1;
 //       Timer_start(timer0);

		qmss_cfg.max_num_desc       = MAX_NUM_DESC;
		qmss_cfg.desc_size          = MAX_DESC_SIZE;
		qmss_cfg.mem_region         = Qmss_MemRegion_MEMORY_REGION0;

		qmss_cfg_srio.max_num_desc       = NUM_HOST_DESC_SRIO;
		qmss_cfg_srio.desc_size          = SIZE_HOST_DESC_SRIO;
		qmss_cfg_srio.mem_region         = Qmss_MemRegion_MEMORY_REGION_NOT_SPECIFIED;

		if (res_mgr_init_qmss (&qmss_cfg,&qmss_cfg_srio) != 0)
		{
			platform_write ("Failed to initialize the QMSS subsystem \n");
			goto main_exit;
		}
		else
		{
			platform_write ("QMSS successfully initialized Eth \n");
		}


    /* Initialize CPPI */
        cppi_cfg.master_core        = 1;

		cppi_cfg.dma_num            = Cppi_CpDma_PASS_CPDMA;
		cppi_cfg.num_tx_queues      = NUM_PA_TX_QUEUES;
		cppi_cfg.num_rx_channels    = NUM_PA_RX_CHANNELS;
		if (res_mgr_init_cppi (&cppi_cfg) != 0)
		{
			platform_write ("Failed to initialize CPPI subsystem \n");
			goto main_exit;
		}
		else
		{
			platform_write ("CPPI successfully initialized \n");
		}


		if (res_mgr_init_pass()!= 0) {
			platform_write ("Failed to initialize the Packet Accelerator \n");
			goto main_exit;
		}
		else
		{
			platform_write ("PA successfully initialized \n");
		}

		if (enableALL_srio()> 0)
		{
			platform_write ("SRIO - Enable!\n");
		 }
		//
		// THIS MUST BE THE ABSOLUTE FIRST THING DONE IN AN APPLICATION before
		//  using the stack!!
		//
		rc = NC_SystemOpen( NC_PRIORITY_LOW, NC_OPMODE_INTERRUPT );
		if( rc )
		{
			platform_write("NC_SystemOpen Failed (%d)\n",rc);
			for(;;);
		}

		// Print out our banner
		platform_write(VerStr);

		//
		// Create and build the system configuration from scratch.
		//

		// Create a new configuration
		hCfg = CfgNew();
		if( !hCfg )
		{
			platform_write("Unable to create configuration\n");
			goto main_exit;
		}

		// We better validate the length of the supplied names
		if( strlen( DomainName ) >= CFG_DOMAIN_MAX ||
			strlen( HostName ) >= CFG_HOSTNAME_MAX )
		{
			platform_write("Names too long\n");
			goto main_exit;
		}

		// Add our global hostname to hCfg (to be claimed in all connected domains)
		CfgAddEntry( hCfg, CFGTAG_SYSINFO, CFGITEM_DHCP_HOSTNAME, 0,
					 strlen(HostName), (UINT8 *)HostName, 0 );


    	CI_IPNET NA;
        CI_ROUTE RT;
        IPN      IPTmp;

        // Setup manual IP address
        bzero( &NA, sizeof(NA) );
        NA.IPAddr  = inet_addr(LocalIPAddr);
        NA.IPMask  = inet_addr(LocalIPMask);
        strcpy( NA.Domain, DomainName );
        NA.NetType = 0;
        ///////////////////////!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!/////////
        // Add the address to interface 1
        CfgAddEntry( hCfg, CFGTAG_IPNET, 1, 0,
                           sizeof(CI_IPNET), (UINT8 *)&NA, 0 );

    /*    CfgAddEntry( hCfg, CFGTAG_IPNET, 2, 0,
                                   sizeof(CI_IPNET), (UINT8 *)&NA, 0 );*/

        // Add the default gateway. Since it is the default, the
        // destination address and mask are both zero (we go ahead
        // and show the assignment for clarity).
        bzero( &RT, sizeof(RT) );
        RT.IPDestAddr = 0;
        RT.IPDestMask = 0;
        RT.IPGateAddr = inet_addr(GatewayIP);

        // Add the route
        CfgAddEntry( hCfg, CFGTAG_ROUTE, 0, 0,
                           sizeof(CI_ROUTE), (UINT8 *)&RT, 0 );

        // Manually add the DNS server when specified
        IPTmp = inet_addr(DNSServer);
        if( IPTmp )
            CfgAddEntry( hCfg, CFGTAG_SYSINFO, CFGITEM_DHCP_DOMAINNAMESERVER,
                         0, sizeof(IPTmp), (UINT8 *)&IPTmp, 0 );


		//
		// Configure IPStack/OS Options
		//

		// We don't want to see debug messages less than WARNINGS
		rc = DBG_INFO;
		CfgAddEntry( hCfg, CFGTAG_OS, CFGITEM_OS_DBGPRINTLEVEL,
					 CFG_ADDMODE_UNIQUE, sizeof(uint), (UINT8 *)&rc, 0 );


		rc = 8192;
		// configure NDK
		CfgAddEntry(hCfg, CFGTAG_IP, CFGITEM_IP_IPREASMMAXSIZE,
				CFG_ADDMODE_UNIQUE, sizeof(uint), (UINT8*) &rc, 0);

		// UDP Receive limit
		rc = 8192;
		CfgAddEntry( hCfg, CFGTAG_IP, CFGITEM_IP_SOCKUDPRXLIMIT,
					 CFG_ADDMODE_UNIQUE, sizeof(uint), (UINT8 *)&rc, 0 );

		// MTU size
		 rc = 7168;
		 CfgAddEntry( hCfg, CFGTAG_IP, CFGITEM_IP_RTDEFAULTMTU,
						CFG_ADDMODE_UNIQUE, sizeof(uint), (UINT8 *)&rc, 0 );


		//
		// Boot the system using this configuration
		//
		// We keep booting until the function returns 0. This allows
		// us to have a "reboot" command.
		//
		do
		{
			rc = NC_NetStart( hCfg, NetworkOpen, NetworkClose, NetworkIPAddr );
		} while( rc > 0 );

	    // Close the OS
		main_exit:
	    NC_SystemClose();
    }
    for(;;)
    {

    }
}

//
// System Task Code [ Server Daemon Servers ]
//
// NetworkOpen
//
// This function is called after the configuration has booted
//
static HANDLE target1=0;
static HANDLE target2=0;
static HANDLE vHello=0;
static HANDLE PRWFlash=0;
static HANDLE DFlash=0;

static void NetworkOpen()
{
	target1 = DaemonNew( SOCK_DGRAM, 0, 40100, dtask_udp_target1,
							OS_TASKPRINORM, OS_TASKSTKHIGH, 0, 1 );
	target2 = DaemonNew( SOCK_DGRAM, 0, 40101, dtask_udp_target2,
							OS_TASKPRINORM, OS_TASKSTKHIGH, 0, 1 );
    vHello = DaemonNew( SOCK_DGRAM, 0, 40010, dtask_udp_visual,
                            OS_TASKPRINORM, OS_TASKSTKHIGH, 0, 1 );

    PRWFlash = DaemonNew( SOCK_DGRAM, 0, 50000, dtask_udp_flash,
                             OS_TASKPRINORM, OS_TASKSTKNORM, 0, 1 );
    DFlash = DaemonNew( SOCK_DGRAM, 0, 50001, dtask_d_flash,
    							OS_TASKPRILOW, OS_TASKSTKLOW, 0, 1 );
}

//
// NetworkClose
//
// This function is called when the network is shutting down,
// or when it no longer has any IP addresses assigned to it.
//
static void NetworkClose()
{
    DaemonFree( target1 );
    DaemonFree( target2 );
    DaemonFree( vHello );
    DaemonFree( PRWFlash );
    DaemonFree( DFlash );

    // Kill any active console
   // ConsoleClose();
}


//
// NetworkIPAddr
//
// This function is called whenever an IP address binding is
// added or removed from the system.
//
static void NetworkIPAddr( IPN IPAddr, uint IfIdx, uint fAdd )
{
    static uint fAddGroups = 0;
    IPN IPTmp;

    if( fAdd )
        platform_write("Network Added: ");
    else
        platform_write("Network Removed: ");

    // Print a message
    IPTmp = ntohl( IPAddr );
    platform_write("If-%d:%d.%d.%d.%d\n", IfIdx,
            (UINT8)(IPTmp>>24)&0xFF, (UINT8)(IPTmp>>16)&0xFF,
            (UINT8)(IPTmp>>8)&0xFF, (UINT8)IPTmp&0xFF );

    // This is a good time to join any multicast group we require
    if( fAdd && !fAddGroups )
    {
        fAddGroups = 1;
//              IGMPJoinHostGroup( inet_addr("224.1.2.3"), IfIdx );
    }

    /* Create a Task to send/receive Raw ethernet traffic */

    //hSendVideo = TaskCreate( SendVideoEth, "TxEthVideoTsk", OS_TASKPRINORM, 0x1400, 0, 0, 0 );
    CreateVideo(2048,1088,2048,1088,0);

	InitPaketBattleJob();
	InitPaketTech();
	assembleBuffOED((uint16*)BuffOED1,1,SizeBuffOED>>1);
	disassembleBuffTA((uint16*)BuffTA1,1,SizeBuffTA>>1);
	assembleBuffOED((uint16*)BuffOED2,2,SizeBuffOED>>1);
	disassembleBuffTA((uint16*)BuffTA2,2,SizeBuffTA>>1);

    //инициализация ядра 0 завершена
	isInitialized[0] = 1;
	isInitialized[1]=LOADKADR1MEM;
	isInitialized[2]=0x00029e00;
	isInitialized[3]=LOADKADR2MEM;
	isInitialized[4]=0x00029e00;
	Cache_wb((void *) &isInitialized[0], 32, Cache_Type_L1D, TRUE);

	battle.STAGE = sDIAG1;			//начинаем диагностику
	battle2.STAGE2 = sSTANDBY2;			//дежурный режим
	Cache_wb((void *) &battle, sizeof(BattleParam), Cache_Type_L1D, TRUE);
	Cache_wb((void *) &battle2, sizeof(BattleParam2), Cache_Type_L1D, TRUE);

}

static Void SrioTask(UArg arg0, UArg arg1)
{
	isInitialized[0]=0x0001abcd;	//на ядре 1 биос запущен
	Cache_wb((void *) &isInitialized[0], 16, Cache_Type_L1D, TRUE);


		for(;;)
		{
			if((isInitialized[0]==3)||(isInitialized[0]==1))
				break;
			Task_sleep(2);	//1 мс
			Cache_inv((void *) &isInitialized[0], 4, Cache_Type_L1D, TRUE);
		}

    	  if (Qmss_start() != QMSS_SOK)
    	   {
    		  for(;;){}
    	   }
    	   else
    	   {
    		   SrioRunJob();
    	   }


}

