#include <xdc/std.h>
#include <string.h>
#include <c6x.h>
#include <xdc/runtime/IHeap.h>
#include <xdc/runtime/System.h>
#include <xdc/runtime/Error.h>
#include <xdc/runtime/Memory.h>
#include <ti/sysbios/BIOS.h>
#include <ti/sysbios/knl/Task.h>
#include <ti/sysbios/heaps/HeapBuf.h>
#include <ti/sysbios/heaps/HeapMem.h>
#include <ti/sysbios/family/c64p/Hwi.h>
#include <ti/sysbios/family/c64p/EventCombiner.h>
#include <ti/sysbios/family/c66/tci66xx/CpIntc.h>
#include <ti/sysbios/family/c66/Cache.h>

/* IPC includes */
#include <ti/ipc/GateMP.h>
#include <ti/ipc/Ipc.h>
#include <ti/ipc/ListMP.h>
#include <ti/ipc/SharedRegion.h>
#include <ti/ipc/MultiProc.h>

#include <xdc/cfg/global.h>

/* SRIO Driver Include File. */
#include <ti/drv/srio/srio_drv.h>
#include <ti/drv/srio/srio_osal.h>

/* CPPI/QMSS Include Files. */
#include <ti/drv/cppi/cppi_drv.h>
#include <ti/drv/qmss/qmss_drv.h>

/* CSL Chip Functional Layer */
#include <ti/csl/csl_chip.h>

/* CSL Cache Functional Layer */
#include <ti/csl/csl_cacheAux.h>

/* PSC CSL Include Files */
#include <ti/csl/csl_psc.h>
#include <ti/csl/csl_pscAux.h>

/* CSL SRIO Functional Layer */
#include <ti/csl/csl_srio.h>
#include <ti/csl/csl_srioAux.h>

/* CSL CPINTC Include Files. */
#include<ti/csl/csl_cpIntc.h>

/* Platform utilities include */
#include "ti/platform/platform.h"

/* Resource manager for QMSS, PA, CPPI */
#include "ti/platform/resource_mgr.h"

//GPIO
#include "gpio.h"
#include "srio.h"
#include "spi.h"
#include "BattleJob.h"

/* MTU of the SRIO Driver. We are currently operating @ MTU of 256 bytes. */
#define SRIO_MAX_MTU				128

/* This is the size of the data buffer which is used for DIO Sockets. */
#define SIZE_DIO_PACKET				128

Srio_DrvHandle  hDrvManagedSrioDrv;

CSL_SrioHandle  hSrioCSL;

/* Memory used for the accumulator list. */
#pragma DATA_ALIGN (SriogHiPriAccumList, 16)
#pragma DATA_SECTION(SriogHiPriAccumList, ".MyCash")
UInt32              SriogHiPriAccumList[64];

/* These are the device identifiers used in the Example Application */
const uint32_t DEVICE_ID1_16BIT    = 0xBEEF;
const uint32_t DEVICE_ID1_8BIT     = 0xAB;
const uint32_t DEVICE_ID2_16BIT    = 0xaaaa;
const uint32_t DEVICE_ID2_8BIT     = 0xaa;

/* Source and Destination Data Buffers (payload buffers) */
UInt8* srcDataBuffer;
UInt8* dstDataBuffer;

#pragma DATA_SECTION(DstGlobalBuff, ".MyCash")
Uint32 DstGlobalBuff[32];
#pragma DATA_SECTION(MyBuffL2, ".MyCash")
Uint32 MyBuffL2[32];
#pragma DATA_SECTION(srioUpac, ".MyCash")
SRIOPACKET srioUpac;

/* Global debug variable to track number of ISRs raised */
volatile UInt32 srioDbgDioIsrCnt = 0;

/* Global variable to indicate completion of ISR processing */
volatile UInt32 srioLsuIsrServiced = 0;

/* Array containing SRIO socket handles */
Srio_SockHandle srioSocket;

/* Global variables to count good and bad transfers */
volatile UInt32 srioDioLsuGoodTransfers = 0;
volatile UInt32 srioDioLsuBadTransfers  = 0;

extern UInt32 malloc_counter;
extern UInt32 free_counter;
extern DiagStruct DStruct;
extern BattleParam battle;			//боевая структура управления программой
extern BattleParam2 battle2;				//боевая структура управления программой цель 2

//extern UInt32 coreNum;

/* Enable Time Stamp Counter */
extern void CSL_tscEnable(void);

extern int32_t SrioDevice_init (void);
extern int32_t Osal_dataBufferInitMemory(uint32_t dataBufferSize);
extern int32_t SrioDevice_init (void);
extern volatile UInt32*     isInitialized;
extern GPIOPACKET gpio;

static Int32 enable_srio (void)
{
    /* SRIO power domain is turned OFF by default. It needs to be turned on before doing any
     * SRIO device register access. This not required for the simulator. */

    /* Set SRIO Power domain to ON */
    CSL_PSC_enablePowerDomain (CSL_PSC_PD_SRIO);

    /* Enable the clocks too for SRIO */
    CSL_PSC_setModuleNextState (CSL_PSC_LPSC_SRIO, PSC_MODSTATE_ENABLE);

    /* Start the state transition */
    CSL_PSC_startStateTransition (CSL_PSC_PD_SRIO);

    /* Wait until the state transition process is completed. */
    while (!CSL_PSC_isStateTransitionDone (CSL_PSC_PD_SRIO));

    /* Return SRIO PSC status */
    if ((CSL_PSC_getPowerDomainState(CSL_PSC_PD_SRIO) == PSC_PDSTATE_ON) &&
        (CSL_PSC_getModuleState (CSL_PSC_LPSC_SRIO) == PSC_MODSTATE_ENABLE))
    {
        /* SRIO ON. Ready for use */
        return 0;
    }
    else
    {
        /* SRIO Power on failed. Return error */
        return -1;
    }
}
static UInt32 l2_global_address (Uint32 addr)
{
	UInt32 corenum;

	/* Get the core number. */
	corenum = CSL_chipReadReg(CSL_CHIP_DNUM);

	/* Compute the global address. */
	return (addr + (0x10000000 + (corenum*0x1000000)));
}

static void myDioTxCompletionIsr(UArg argument)
{
    /* Pass the control to the driver DIO Tx Completion ISR handler */
    Srio_dioTxCompletionIsr ((Srio_DrvHandle)argument, hSrioCSL);

    /* Wake up the pending task */
    srioLsuIsrServiced = 1;

    /* Debug: Increment the ISR count */
    srioDbgDioIsrCnt++;

    return;
}



Int32 enableALL_srio (void)
{
	/* Power on SRIO peripheral before using it */
	if (enable_srio () < 0)
	 {
	     System_printf ("Error: SRIO PSC Initialization Failed\n");
	  return 0;
	 }
	if (SrioDevice_init() < 0)
	     return 0;

	if (Srio_init () < 0)
	{
	     System_printf ("Error: SRIO Driver Initialization Failed\n");
	     return 0;
	}
	return 1;
}

static Int32 dioSocketsWithISR (Srio_DrvHandle hSrioDrv, uint8_t dio_ftype, uint8_t dio_ttype)
{
	static volatile Uint32	 TSCLin =0, TSCHin = 0, TSCLout=0, TSCHout=0;
    Srio_SockBindAddrInfo   bindInfo;
    Srio_SockAddrInfo       to;
    volatile int32_t                 eventId, startTime,mycount,j=0;
    uint8_t                 compCode;
    uint32_t                *sriodatabuff;


    /* Get the CSL SRIO Handle. */
    hSrioCSL = CSL_SRIO_Open (0);
    if (hSrioCSL == NULL)
    	System_printf ("Error: Unable to open the SRIO handle\n");

     /* SRIO DIO Interrupts need to be routed from the CPINTC0 to GEM Event.
     *  - We have configured DIO Interrupts to get routed to Interrupt Destination 0
     *    (Refer to the CSL_SRIO_RouteLSUInterrupts API configuration in the SRIO Initialization)
     *  - We want this System Interrupt to mapped to Host Interrupt 8 */

    /* Disable Interrupt Pacing for INTDST0 */
    CSL_SRIO_DisableInterruptPacing (hSrioCSL, 0);

    /* Route LSU0 ICR0 to INTDST0 */
    CSL_SRIO_RouteLSUInterrupts (hSrioCSL, 0, 0);

    /* Route LSU0 ICR1 to INTDST0 */
    CSL_SRIO_RouteLSUInterrupts (hSrioCSL, 1, 0);

    /* Route LSU0 ICR2 to INTDST0 */
    CSL_SRIO_RouteLSUInterrupts (hSrioCSL, 2, 0);

    /* Map the System Interrupt i.e. the Interrupt Destination 0 interrupt to the DIO ISR Handler. */
    CpIntc_dispatchPlug(CSL_INTC0_INTDST0, (CpIntc_FuncPtr)myDioTxCompletionIsr, (UArg)hSrioDrv, TRUE);

    /* The configuration is for CPINTC0. We map system interrupt 112 to Host Interrupt 8. */
    CpIntc_mapSysIntToHostInt(0, CSL_INTC0_INTDST0, 8);

    /* Enable the Host Interrupt. */
    CpIntc_enableHostInt(0, 8);

    /* Enable the System Interrupt */
    CpIntc_enableSysInt(0, CSL_INTC0_INTDST0);

    /* Get the event id associated with the host interrupt. */
    eventId = CpIntc_getEventId(8);

    /* Plug the CPINTC Dispatcher. */
    EventCombiner_dispatchPlug (eventId, CpIntc_dispatch, 8, TRUE);

      /* Open DIO SRIO Non-Blocking Socket */
      srioSocket = Srio_sockOpen (hSrioDrv, Srio_SocketType_DIO, FALSE);
      if (srioSocket == NULL)
      {
          System_printf ("Error: Unable to open the DIO socket\n");
      }

      /* DIO Binding Information: Use 16 bit identifiers and we are bound to the first source id.
       * and we are using 16 bit device identifiers. */
      bindInfo.dio.doorbellValid  = 0;
      bindInfo.dio.intrRequest    = 1;
      bindInfo.dio.supInt         = 0;
      bindInfo.dio.xambs          = 0;
      bindInfo.dio.priority       = 0;
      bindInfo.dio.outPortID      = 0;
 //     bindInfo.dio.idSize         = 1;
  //    bindInfo.dio.srcIDMap       = sockIdx;
      bindInfo.dio.idSize         = 0;	//8bit
    //  bindInfo.dio.srcIDMap       = sockIdx;
      bindInfo.dio.srcIDMap       = 0;
      bindInfo.dio.hopCount       = 0;
      bindInfo.dio.doorbellReg    = 0;
      bindInfo.dio.doorbellBit    = 0;

      /* Bind the SRIO socket: DIO sockets do not need any binding information. */
      if (Srio_sockBind_DIO (srioSocket, &bindInfo) < 0)
      {
          System_printf ("Error: Binding the SIO socket failed.\n");
      }

    /* Allocate memory for the Source and Destination Buffers */

    	srcDataBuffer=(UInt8 *)l2_global_address((UInt32)MyBuffL2);

        dstDataBuffer = (UInt8 *)l2_global_address((UInt32)DstGlobalBuff);

////////////////////////test запись чтение
	sriodatabuff=(uint32_t*)srcDataBuffer;

    for(;;)
    {

    	while (isInitialized[0] != 3)
    	 {
    		Task_sleep(1);			//0.5 мс
    		Cache_inv((void *) &isInitialized[0], 128, Cache_Type_L1D, TRUE);
    	 }
    	sriodatabuff[0]=isInitialized[1];
    	sriodatabuff[1]=isInitialized[2];
    	sriodatabuff[2]=isInitialized[3];
    	sriodatabuff[3]=isInitialized[4];
    	sriodatabuff[4]=isInitialized[24];
    	sriodatabuff[5]=isInitialized[25];

    	//добавляем также вывод пуска и исправности
    	sriodatabuff[6]=isInitialized[26];		//включение работы по стробу

    	if((DStruct.Target1Ok==1)&&(DStruct.Target2Ok==1))	//выводим сигнал исправен
    	{
    		sriodatabuff[6]|=4;
    	}

    	if((battle.STAGE==sBATTLE)||((battle2.STAGE2==sBATTLE2)))
    	{
    		sriodatabuff[6]|=2;
    	}

    	Cache_inv((void *) &battle, sizeof(BattleParam), Cache_Type_L1D, TRUE);
    	Cache_inv((void *) &battle2, sizeof(BattleParam2), Cache_Type_L1D, TRUE);
    	/////////////////////////////////////////////////////////////////////
    	sriodatabuff[7]=0;
    	sriodatabuff[8]=isInitialized[27];
    	sriodatabuff[9]=isInitialized[28];
    	sriodatabuff[10]=isInitialized[29];

		dio_ftype=Srio_Ftype_SWRITE;
		dio_ttype = 0;

	//	dio_ftype=Srio_Ftype_REQUEST;
	//	dio_ttype = Srio_Ttype_Request_NREAD;

		to.dio.rapidIOMSB    = 0x0;
		to.dio.rapidIOLSB    = 0;
		to.dio.dstID         = DEVICE_ID2_8BIT;
		to.dio.ttype         = dio_ttype;
		to.dio.ftype         = dio_ftype;

		if (Srio_sockSend_DIO (srioSocket, srcDataBuffer, 40, (Srio_SockAddrInfo*)&to) < 0)
		{
		   System_printf ("Debug(Core %d): DIO Socket Example Failed\n", CSL_chipReadReg (CSL_CHIP_DNUM));
		}
		    startTime = TSCL;
		    while((! srioLsuIsrServiced) && ((TSCL - startTime) < 5000));
			compCode = 0xFF;
			if (Srio_getSockOpt(srioSocket, Srio_Opt_DIO_READ_SOCK_COMP_CODE, &compCode, sizeof(uint8_t)) < 0)
				{
					System_printf ("Error: Unable to read the completion code in socket\n");
				 }
			 CSL_SRIO_ClearLSUPendingInterrupt (hSrioCSL, 0xFFFFFFFF, 0xFFFFFFFF);
			 srioLsuIsrServiced = 0;

		dio_ftype=Srio_Ftype_REQUEST;
		dio_ttype = Srio_Ttype_Request_NREAD;

		to.dio.rapidIOMSB    = 0x0;
		to.dio.rapidIOLSB    = 0;
		to.dio.dstID         = DEVICE_ID2_8BIT;
		to.dio.ttype         = dio_ttype;
		to.dio.ftype         = dio_ftype;

		if (Srio_sockSend_DIO (srioSocket, dstDataBuffer, 64, (Srio_SockAddrInfo*)&to) < 0)
		{
			 System_printf ("Debug(Core %d): DIO Socket Example Failed\n", CSL_chipReadReg (CSL_CHIP_DNUM));
		}

		startTime = TSCL;
		while((! srioLsuIsrServiced) && ((TSCL - startTime) < 10000));
		compCode = 0xFF;
		            if (Srio_getSockOpt(srioSocket, Srio_Opt_DIO_READ_SOCK_COMP_CODE, &compCode, sizeof(uint8_t)) < 0)
		            {
		                System_printf ("Error: Unable to read the completion code in socket\n");
		            }
		 CSL_SRIO_ClearLSUPendingInterrupt (hSrioCSL, 0xFFFFFFFF, 0xFFFFFFFF);
		 srioLsuIsrServiced = 0;

		isInitialized[0]=0;
		isInitialized[20]=dstDataBuffer[0]|dstDataBuffer[1]<<8|dstDataBuffer[8]<<16|dstDataBuffer[9]<<24;
		isInitialized[21]=dstDataBuffer[16]|dstDataBuffer[17]<<8|dstDataBuffer[18]<<16|dstDataBuffer[19]<<24;
		isInitialized[22]=dstDataBuffer[24]|dstDataBuffer[25]<<8|dstDataBuffer[26]<<16|dstDataBuffer[27]<<24;
		isInitialized[23]=dstDataBuffer[28]|dstDataBuffer[29]<<8|dstDataBuffer[30]<<16|dstDataBuffer[31]<<24;

		isInitialized[10]=dstDataBuffer[32]|dstDataBuffer[33]<<8|dstDataBuffer[34]<<16|dstDataBuffer[35]<<24;
		isInitialized[10]&=0x0fffffff;
		isInitialized[11]=dstDataBuffer[35]|dstDataBuffer[36]<<8|dstDataBuffer[37]<<16|dstDataBuffer[38]<<24;
		isInitialized[11]=isInitialized[11]>>4;
		isInitialized[12]=dstDataBuffer[40]|dstDataBuffer[41]<<8|dstDataBuffer[42]<<16|dstDataBuffer[43]<<24;
		isInitialized[12]&=0x0fffffff;
		isInitialized[13]=dstDataBuffer[43]|dstDataBuffer[44]<<8|dstDataBuffer[45]<<16|dstDataBuffer[46]<<24;
		isInitialized[13]=isInitialized[13]>>4;

		Cache_wb((void *) &isInitialized[0], 128, Cache_Type_L1D, TRUE);

   }

    /* Close the sockets */

   //     Srio_sockClose_DIO (srioSocket);

 //   return 0;
}

Void SrioRunJob()
{
    UInt8           isAllocated;
    Srio_DrvConfig  drvCfg;

    /* Initialize the SRIO Driver Configuration. */
    memset ((Void *)&drvCfg, 0, sizeof(Srio_DrvConfig));

    /* Initialize the OSAL */
    if (Osal_dataBufferInitMemory(SRIO_MAX_MTU) < 0)
    {
	    System_printf ("Error: Unable to initialize the OSAL. \n");
    }

    /********************************************************************************
     * The SRIO Driver Instance is going to be created with the following properties:
     * - Driver Managed
     * - Interrupt Support (Pass the Rx Completion Queue as NULL)
     ********************************************************************************/

    /* Setup the SRIO Driver Managed Configuration. */
    drvCfg.bAppManagedConfig = FALSE;

    /* Driver Managed: Receive Configuration */
    drvCfg.u.drvManagedCfg.bIsRxCfgValid             = 1;
  //  drvCfg.u.drvManagedCfg.rxCfg.rxMemRegion         = Qmss_MemRegion_MEMORY_REGION0;
    drvCfg.u.drvManagedCfg.rxCfg.rxMemRegion         = Qmss_MemRegion_MEMORY_REGION1;
    drvCfg.u.drvManagedCfg.rxCfg.numRxBuffers        = 4;
    drvCfg.u.drvManagedCfg.rxCfg.rxMTU               = SRIO_MAX_MTU;

    /* Accumulator Configuration. */
    {
	    int32_t coreToQueueSelector[4];

      /* This is the table which maps the core to a specific receive queue. */
	    coreToQueueSelector[0] = 704;
	    coreToQueueSelector[1] = 705;
	    coreToQueueSelector[2] = 706;
	    coreToQueueSelector[3] = 707;

	    /* Since we are programming the accumulator we want this queue to be a HIGH PRIORITY Queue */
	    drvCfg.u.drvManagedCfg.rxCfg.rxCompletionQueue = Qmss_queueOpen (Qmss_QueueType_HIGH_PRIORITY_QUEUE,
	    															     coreToQueueSelector[CSL_chipReadReg (CSL_CHIP_DNUM)], &isAllocated);
		if (drvCfg.u.drvManagedCfg.rxCfg.rxCompletionQueue < 0)
		{
			System_printf ("Error: Unable to open the SRIO Receive Completion Queue\n");
		}

		/* Accumulator Configuration is VALID. */
		drvCfg.u.drvManagedCfg.rxCfg.bIsAccumlatorCfgValid = 1;

		/* Accumulator Configuration. */
	    drvCfg.u.drvManagedCfg.rxCfg.accCfg.channel             = CSL_chipReadReg (CSL_CHIP_DNUM);
	    drvCfg.u.drvManagedCfg.rxCfg.accCfg.command             = Qmss_AccCmd_ENABLE_CHANNEL;
	    drvCfg.u.drvManagedCfg.rxCfg.accCfg.queueEnMask         = 0;
	    drvCfg.u.drvManagedCfg.rxCfg.accCfg.queMgrIndex         = coreToQueueSelector[CSL_chipReadReg (CSL_CHIP_DNUM)];
	    drvCfg.u.drvManagedCfg.rxCfg.accCfg.maxPageEntries      = 2;
	    drvCfg.u.drvManagedCfg.rxCfg.accCfg.timerLoadCount      = 0;
	    drvCfg.u.drvManagedCfg.rxCfg.accCfg.interruptPacingMode = Qmss_AccPacingMode_LAST_INTERRUPT;
	    drvCfg.u.drvManagedCfg.rxCfg.accCfg.listEntrySize       = Qmss_AccEntrySize_REG_D;
	    drvCfg.u.drvManagedCfg.rxCfg.accCfg.listCountMode       = Qmss_AccCountMode_ENTRY_COUNT;
	    drvCfg.u.drvManagedCfg.rxCfg.accCfg.multiQueueMode      = Qmss_AccQueueMode_SINGLE_QUEUE;

        /* Initialize the accumulator list memory */
        memset ((Void *)&SriogHiPriAccumList[0], 0, sizeof(SriogHiPriAccumList));
        drvCfg.u.drvManagedCfg.rxCfg.accCfg.listAddress = l2_global_address((UInt32)&SriogHiPriAccumList[0]);
    }

    /* Driver Managed: Transmit Configuration */
    drvCfg.u.drvManagedCfg.bIsTxCfgValid             = 1;
//    drvCfg.u.drvManagedCfg.txCfg.txMemRegion         = Qmss_MemRegion_MEMORY_REGION0;
    drvCfg.u.drvManagedCfg.txCfg.txMemRegion         = Qmss_MemRegion_MEMORY_REGION1;
    drvCfg.u.drvManagedCfg.txCfg.numTxBuffers        = 4;
    drvCfg.u.drvManagedCfg.txCfg.txMTU               = SRIO_MAX_MTU;

    /* Start the Driver Managed SRIO Driver. */
    hDrvManagedSrioDrv = Srio_start(&drvCfg);
    if (hDrvManagedSrioDrv == NULL)
    {
        System_printf ("Error(Core %d): SRIO Driver failed to start\n", CSL_chipReadReg (CSL_CHIP_DNUM));
    }

    /* Hook up the SRIO interrupts with the core. */
    EventCombiner_dispatchPlug (49, (EventCombiner_FuncPtr)Srio_rxCompletionIsr, (UArg)hDrvManagedSrioDrv, TRUE);
	  EventCombiner_enableEvent(49);

    /* Enable Time Stamp Counter */
    CSL_tscEnable();

    /* Run the loopback data transfers on the system initialization core. */

    	for(;;)
    	{
        /* DIO Write Operation */
			if (dioSocketsWithISR (hDrvManagedSrioDrv, Srio_Ftype_WRITE, Srio_Ttype_Write_NWRITE_R) < 0)
			{
				System_printf ("Error: Loopback DIO ISR example for Write operation failed\n");
			}
    	}
}

