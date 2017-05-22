/*
 *  Do not modify this file; it is automatically 
 *  generated and any modifications will be overwritten.
 *
 * @(#) xdc-y44
 */

#include <xdc/std.h>

#include <ti/sysbios/heaps/HeapMem.h>
extern const ti_sysbios_heaps_HeapMem_Handle heap0;

#include <ti/sysbios/hal/Timer.h>
extern const ti_sysbios_hal_Timer_Handle timer0;

#include <ti/sysbios/family/c64p/Hwi.h>
extern const ti_sysbios_family_c64p_Hwi_Handle hwiGPIO15;

#include <ti/sysbios/family/c64p/Hwi.h>
extern const ti_sysbios_family_c64p_Hwi_Handle hwiGPIO14;

#include <ti/sysbios/family/c64p/Hwi.h>
extern const ti_sysbios_family_c64p_Hwi_Handle hwiGPIO13;

#include <ti/sysbios/family/c64p/Hwi.h>
extern const ti_sysbios_family_c64p_Hwi_Handle hwiGPIO12;

extern int xdc_runtime_Startup__EXECFXN__C;

extern int xdc_runtime_Startup__RESETFXN__C;

#ifndef ti_sysbios_knl_Task__include
#ifndef __nested__
#define __nested__
#include <ti/sysbios/knl/Task.h>
#undef __nested__
#else
#include <ti/sysbios/knl/Task.h>
#endif
#endif

extern ti_sysbios_knl_Task_Struct TSK_idle;

