#ifndef SIGNALPROC_H_
#define SIGNALPROC_H_

//typedef unsigned int UInt32;
#include "ti/platform/resource_mgr.h"

typedef struct ALGINFOALGINFO {
	 unsigned short tm;
	 unsigned short npt;
	 unsigned short nobj;
	 unsigned short nclst;
	 unsigned short ksmr;
	 unsigned short kbgrnd;
	 unsigned short command;
	 unsigned short nobj2;
	 unsigned short uismr;
	 unsigned short uimean;
	 unsigned short sz;
	 unsigned short x;
	 unsigned short y;
	 unsigned short size;
	 unsigned short level;
	 unsigned short snr;
	 unsigned short x2;
	 unsigned short y2;
	 unsigned short bgrnd;
	 unsigned short rms;
	 unsigned short niter;
} ALGINFO;


typedef struct CLUSTERINFO {
	 unsigned short lt;
	 unsigned short rb;
	 unsigned short maxl;
	 unsigned int   suml;
	 unsigned short snr;
	 unsigned short bgrnd;
	 unsigned short npt;
} CLUSTERINF;
extern double dsmr;
extern ALGINFO alginf;
//typedef unsigned short UInt16;
//typedef unsigned char UInt8;
//typedef unsigned int UInt32;

extern volatile unsigned int cregister TSCL;
extern volatile unsigned int cregister TSCH;

UInt32 statSMR( UInt16 *mbuff, double *pfSMR,  double *pfMean);
UInt32 statHistSMR( UInt16 *mbuff,UInt16 *histbuf, double *pfSMR,  double *pfMean) ;
int signalProcess( UInt16 *dstbuf,unsigned int *srcbuf,int size);
int signalProcessLC1( UInt16 *dstbuf,unsigned int *srcbuf,int size);

int mymcpy(void * dst,void * src,int size) ;
UInt32  hifr_filter(UInt16 * mout , UInt16 * mybuff, UInt16 size) ;
UInt32 localRectStat(UInt16 *mbuff,double fkSMR,	UInt16 sh, UInt16 sw) ;
UInt32 localBigRectStat(UInt16 *mbuff,double fkSMR,UInt16 mean,	UInt16 sh, UInt16 sw) ;

//int mxrnkFilter( (UInt16 (*)[192])  mybuff,(UInt16 (*)[192]) mHBuf,(UInt16 (*)[192]) mVBuf);
//int mxrnkFilter( UInt16 (* inbuff)[192] ,UInt16 (* mHBuf)[192]  ,UInt16 (* mVBuf)[192] );
//m192rnkFilter( UInt16 * inbuff, UInt16 * mHBuf, UInt16 * mVBuf,UInt16 * mobj,UInt16 * mobjMx,	UInt16 * mout) ;
//int m192rnkFilter(UInt16 * inbuff,UInt16 * mHBuf, UInt16 *mVBuf,UInt16 * mobj, UInt16 * mobjMx, UInt8 * mout);
int m192rnkFilter( UInt16 * inbuff, UInt16 * mHBuf, UInt16 * mVBuf,UInt16 * mobj,UInt16 * mobjMx,	UInt8 * mout, double dsmr) ;
int m192BigFilter( UInt16 * inbuff, UInt16 * mHBuf, UInt16 * mVBuf,UInt16 * mobj,UInt16 * mobjMx,	UInt8 * mout, double dsmrpar, UInt16 thrhld, UInt16 mean) ;
UInt32 locCombSMR(UInt16 sh, UInt16 sm, UInt16 *mbuff,double fkSMR) ;
//UInt32 combSMR(UInt16 sh, UInt16 sm, UInt16 *mbuff,double fkSMR,double fgSMR) ;
UInt32 combSMR(UInt16 sh, UInt16 sm, UInt16 *mbuff,double fkSMR) ;
//UInt32  combSMR2(UInt16 sh, UInt16 sm, UInt16 *mbuff,double fkSMR);
UInt32  stopwatch();
//#include "platform.h"
//#include "csl_chip.h"
//#include "csl_chipAux.h"
//int test_func(int val);

extern volatile unsigned int gtm;
extern UInt16 * pbgrnd;


//extern volatile double dgtm;

#endif
