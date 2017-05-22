
#include <ti/sysbios/BIOS.h>
#include <ti/ndk/inc/netmain.h>
#include "PaketOEDTAEth.h"

#ifndef MYCORE_H_
#define MYCORE_H_
	extern VisualStatusPaket vsp;
	extern VisualKommandPaket vkp;
	 void MyStartCore2();
	 void MyStartCore3();
	 void MyStartCore4();
	 void MyStartCore5();
	 void intIsr(void *eventId );
	 extern volatile UInt8 isSimpleThr;
	 extern  volatile double ndgtm;
	 extern volatile int nObjFin;
	 extern UInt32 errtm;
	 extern UInt32 TSCLsav;
	 extern UInt16 * pbgrnd;

	 UInt32  stopwatch();
#endif
