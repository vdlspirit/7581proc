#include <ti/sysbios/BIOS.h>

/* Platform utilities include */
#include "ti/platform/platform.h"

/* Resource manager for QMSS, PA, CPPI */
#include "ti/platform/resource_mgr.h"

#include <ti/sysbios/family/c66/Cache.h>

#include "InitCache.h"

void InitCache()
{
	Cache_Size CashS;

	Cache_getSize(&CashS);
	CashS.l2Size=ti_sysbios_family_c66_Cache_L2Size_32K;
	Cache_setSize(&CashS);
	Cache_getSize(&CashS);


}
