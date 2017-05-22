#include <ti/csl/csl_psc.h>
#include <ti/csl/csl_pscAux.h>

#include "resource_mgr.h"
extern Qmss_QueueHnd   gGlobalFreeQHnd;

extern int32_t res_mgr_stop_qmss (void);

void StartStopEth(int idss)
{
	uint32_t* pa_padma_txchconf0 = (uint32_t*) 0x02004400;
	uint32_t* pa_padma_rxchconf0 = (uint32_t*) 0x02004800;

	if (idss==1)	//sleep eth
	{
		*pa_padma_txchconf0|=0x20000000; /*teardown the 9 TX channels and the 24 RX channels*/
		*pa_padma_rxchconf0|=0x20000000;
	}
	else
	{
		*pa_padma_txchconf0&=0xdfffffff; /*teardown the 9 TX channels and the 24 RX channels*/
		*pa_padma_rxchconf0&=0xdfffffff;
	}

}

void GlobalResetEthQmss()
{
/*	uint32_t* PDSP0_CTRL = (uint32_t*) 0x2001000;
	uint32_t* PDSP1_CTRL = (uint32_t*) 0x2001100;
	uint32_t* PDSP2_CTRL = (uint32_t*) 0x2001200;
	uint32_t* PDSP3_CTRL = (uint32_t*) 0x2001300;
	uint32_t* PDSP4_CTRL = (uint32_t*) 0x2001400;
	uint32_t* PDSP5_CTRL = (uint32_t*) 0x2001500;
	uint32_t* PDSP6_CTRL = (uint32_t*) 0x2001600;
	uint32_t* PDSP7_CTRL = (uint32_t*) 0x2001700;
	uint32_t* CPSW_CTRL = (uint32_t*) 0x2090804;

	uint32_t* pa_padma_txchconf0 = (uint32_t*) 0x02004400;
	uint32_t* pa_padma_rxchconf0 = (uint32_t*) 0x02004800;

	CSL_PSC_PDSTATE pdstate=PSC_PDSTATE_OFF;
	pdstate= CSL_PSC_getPowerDomainState(CSL_PSC_PD_PASS);
	if (pdstate!=PSC_PDSTATE_OFF)
	//if PASS power domain is not OFF then reset
	{
		*pa_padma_txchconf0|=0x40000000; //teardown the 9 TX channels and the 24 RX channels
		*pa_padma_rxchconf0|=0x40000000;

		*CPSW_CTRL = 0;// disable port 0 of the Gigabit Ethernet Switch
		*PDSP0_CTRL &= 0xFFFFFFFD;// disable PDSP0
		*PDSP1_CTRL &= 0xFFFFFFFD;// disable PDSP1
		*PDSP2_CTRL &= 0xFFFFFFFD; // disable PDSP2
		*PDSP3_CTRL &= 0xFFFFFFFD; // disable PDSP3
		*PDSP4_CTRL &= 0xFFFFFFFD;// disable PDSP4
		*PDSP5_CTRL &= 0xFFFFFFFD;// disable PDSP5
		*PDSP6_CTRL &= 0xFFFFFFFD;// disable PDSP6
		*PDSP7_CTRL &= 0xFFFFFFFD;// disable PDSP7

		// disable the clocks for PASS modules
		CSL_PSC_setModuleNextState (CSL_PSC_LPSC_PKTPROC, PSC_MODSTATE_DISABLE);
		CSL_PSC_setModuleNextState (CSL_PSC_LPSC_CPGMAC, PSC_MODSTATE_DISABLE);
		CSL_PSC_setModuleNextState (CSL_PSC_LPSC_Crypto, PSC_MODSTATE_DISABLE);

		CSL_PSC_disablePowerDomain(CSL_PSC_PD_PASS);
		CSL_PSC_startStateTransition(CSL_PSC_PD_PASS);
		while (!CSL_PSC_isStateTransitionDone(CSL_PSC_PD_PASS));
	}


	// Set PASS Power domain to ON
	CSL_PSC_enablePowerDomain (CSL_PSC_PD_PASS);

	// Enable the clocks for PASS modules
	CSL_PSC_setModuleNextState (CSL_PSC_LPSC_PKTPROC, PSC_MODSTATE_ENABLE);
	CSL_PSC_setModuleNextState (CSL_PSC_LPSC_CPGMAC, PSC_MODSTATE_ENABLE);
	CSL_PSC_setModuleNextState (CSL_PSC_LPSC_Crypto, PSC_MODSTATE_ENABLE);

	// Start the state transition
	CSL_PSC_startStateTransition (CSL_PSC_PD_PASS);

	// Wait until the state transition process is completed.
		while (!CSL_PSC_isStateTransitionDone (CSL_PSC_PD_PASS));
*/

/*	int status=0;
	status=res_mgr_stop_qmss();*/
}
