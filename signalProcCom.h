#ifndef SIGNALPROCCOM_H_
#define SIGNALPROCCOM_H_

#define SZCDRX144 144
#define SZCDRY144 144
#define SZCadr144 144
//typedef unsigned short UInt16;
//typedef unsigned char UInt8;
//typedef unsigned int UInt32;

#include "signalProc.h"
#include "BattleJob.h"


int mflt3x3_144(void * dst,void * src,int size) ;
int mflt2x2_144(void * dst,void * src,int size) ;
int clear(void * dst,int size) ;


UInt32 statSMR144( UInt16 *mbuff, double *pfSMR,  double *pfMean);
UInt32 statHistSMR144( UInt16 *mbuff,UInt16 *histbuf, double *pfSMR,  double *pfMean) ;
int signalProcessLC1( UInt16 *srcbuf,unsigned int *dstbuf,int size);
UInt32  hifr_filter144(UInt16 * mout , UInt16 * mybuff, UInt16 size) ;
UInt32 localRectStat144(UInt16 *mbuff,double fkSMR,	UInt16 sh, UInt16 sw) ;
//int mxrnkFilter( (UInt16 (*)[192])  mybuff,(UInt16 (*)[192]) mHBuf,(UInt16 (*)[192]) mVBuf);
//int mxrnkFilter( UInt16 (* inbuff)[192] ,UInt16 (* mHBuf)[192]  ,UInt16 (* mVBuf)[192] );
//m192rnkFilter( UInt16 * inbuff, UInt16 * mHBuf, UInt16 * mVBuf,UInt16 * mobj,UInt16 * mobjMx,	UInt16 * mout) ;
//int m192rnkFilter(UInt16 * inbuff,UInt16 * mHBuf, UInt16 *mVBuf,UInt16 * mobj, UInt16 * mobjMx, UInt8 * mout);
int m144rnkFilter( UInt16 * inbuff, UInt16 * mHBuf, UInt16 * mVBuf,UInt16 * mobj,UInt16 * mobjMx,	UInt8 * mout, double dsmr) ;
int m144BigFilter( UInt16 * inbuff, UInt16 * mHBuf, UInt16 * mVBuf,UInt16 * mobj,UInt16 * mobjMx,	UInt8 * mout, double dsmrpar, UInt16 thrhld, UInt16 mean) ;

//void vr5mx3mn144( (UInt16 (*)[144])  inbuff,(UInt16 (*)[144]) mVBuf,(UInt16 (*)[144]) mVBuf);
//void hr5mx3mn144( (UInt16 (*)[144])  inbuff,(UInt16 (*)[144]) mHBuf,(UInt16 (*)[144]) mHBuf);

void hr5mx3mn144(UInt16 (* InpArr)[SZCadr144],UInt16 (* wrkArr)[SZCadr144], UInt16 (* outArr)[SZCadr144]) ;
void vr5mx3mn144(UInt16 (* InpArr)[SZCadr144],UInt16 (* wrkArr)[SZCadr144], UInt16 (* outArr)[SZCadr144]) ;



UInt32 locCombSMR144(UInt16 sh, UInt16 sm, UInt16 *mbuff,double fkSMR) ;
//UInt32 combSMR(UInt16 sh, UInt16 sm, UInt16 *mbuff,double fkSMR,double fgSMR) ;
UInt32 combSMR144(UInt16 sh, UInt16 sm, UInt16 *mbuff,double fkSMR) ;
//UInt32  combSMR2(UInt16 sh, UInt16 sm, UInt16 *mbuff,double fkSMR);
UInt32  stopwatch();
//#include "platform.h"
//#include "csl_chip.h"
//#include "csl_chipAux.h"
//int test_func(int val);

extern volatile unsigned int gtm;



#define SZCDRX184 184
#define SZCDRY184 184
#define SZCadr184 184
//typedef unsigned short UInt16;
//typedef unsigned char UInt8;
//typedef unsigned int UInt32;
extern BattleParam battle;			//боева€ структура управлени€ программой
extern BattleParam2 battle2;				//боева€ структура управлени€ программой цель 2

#include "signalProc.h"


int mflt3x3_184(void * dst,void * src,int size) ;
int mflt2x2_184(void * dst,void * src,int size) ;
int clear(void * dst,int size) ;


UInt32 statSMR184( UInt16 *mbuff, double *pfSMR,  double *pfMean);
UInt32 statHistSMR184( UInt16 *mbuff,UInt16 *histbuf, double *pfSMR,  double *pfMean) ;
int signalProcessLC1( UInt16 *srcbuf,unsigned int *dstbuf,int size);
int signalProcessLC2( UInt16 *srcbuf,unsigned int *dstbuf,int size);
UInt32  hifr_filter184(UInt16 * mout , UInt16 * mybuff, UInt16 size) ;
UInt32 localRectStat184(UInt16 *mbuff,double fkSMR,	UInt16 sh, UInt16 sw) ;
UInt32 localBigRectStat(UInt16 *mbuff,double fkSMR,UInt16 mean,	UInt16 sh, UInt16 sw) ;
UInt32 localBigRectStat144(UInt16 *mbuff,double fkSMR,UInt16 mean,	UInt16 sh, UInt16 sw) ;
UInt32 localBigRectStat184(UInt16 *mbuff,double fkSMR,UInt16 mean,	UInt16 sh, UInt16 sw) ;

//int mxrnkFilter( (UInt16 (*)[192])  mybuff,(UInt16 (*)[192]) mHBuf,(UInt16 (*)[192]) mVBuf);
//int mxrnkFilter( UInt16 (* inbuff)[192] ,UInt16 (* mHBuf)[192]  ,UInt16 (* mVBuf)[192] );
//m192rnkFilter( UInt16 * inbuff, UInt16 * mHBuf, UInt16 * mVBuf,UInt16 * mobj,UInt16 * mobjMx,	UInt16 * mout) ;
//int m192rnkFilter(UInt16 * inbuff,UInt16 * mHBuf, UInt16 *mVBuf,UInt16 * mobj, UInt16 * mobjMx, UInt8 * mout);
int m184rnkFilter( UInt16 * inbuff, UInt16 * mHBuf, UInt16 * mVBuf,UInt16 * mobj,UInt16 * mobjMx,	UInt8 * mout, double dsmr) ;
int m184BigFilter( UInt16 * inbuff, UInt16 * mHBuf, UInt16 * mVBuf,UInt16 * mobj,UInt16 * mobjMx,	UInt8 * mout, double dsmrpar, UInt16 thrhld, UInt16 mean) ;

//void vr5mx3mn184( (UInt16 (*)[184])  inbuff,(UInt16 (*)[184]) mVBuf,(UInt16 (*)[184]) mVBuf);
//void hr5mx3mn184( (UInt16 (*)[184])  inbuff,(UInt16 (*)[184]) mHBuf,(UInt16 (*)[184]) mHBuf);

void hr5mx3mn184(UInt16 (* InpArr)[SZCadr184],UInt16 (* wrkArr)[SZCadr184], UInt16 (* outArr)[SZCadr184]) ;
void vr5mx3mn184(UInt16 (* InpArr)[SZCadr184],UInt16 (* wrkArr)[SZCadr184], UInt16 (* outArr)[SZCadr184]) ;



UInt32 locCombSMR184(UInt16 sh, UInt16 sm, UInt16 *mbuff,double fkSMR) ;
//UInt32 combSMR(UInt16 sh, UInt16 sm, UInt16 *mbuff,double fkSMR,double fgSMR) ;
UInt32 combSMR184(UInt16 sh, UInt16 sm, UInt16 *mbuff,double fkSMR) ;

void zoom2_192(unsigned short *psrc,unsigned short *pdst) ;
void zoom2_144(unsigned short *psrc,unsigned short *pdst) ;
//extern volatile double dgtm;

#endif
