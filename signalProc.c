#include "ti/platform/resource_mgr.h"
#include "signalProc.h"
#include "alg_add.h"
#include "math.h"

void zoom2_192(unsigned short *psrc,unsigned short *pdst) ; // only test Remove

#pragma DATA_SECTION(fr34j, ".l2Buff")
volatile int fr34j=0;

//#pragma DATA_SECTION(gtm, ".l2Buff")
volatile unsigned int gtm;

//#pragma DATA_SECTION(ndgtm, ".l2Buff")
volatile double ndgtm=0;

int mflt2x2(void * dst,void * src,int size);
int mflt3x3(void * dst,void * src,int size);

int diffcpy(UInt16* dstbuf,UInt16* sigbuf,UInt16* backbuf,int size,UInt16 zero) ;
int multpix(UInt16* dstbuf,UInt16* sigbuf,UInt16* backbuf,int size,UInt16 zero) ;
int clearfbuff(double *pfbuf,int size) ;
int clear(void * dst,int size) ;

//int signalProcessing(int size, unsigned int *srcbuf)
/*
render(UInt16 *mHBuf,UInt16 *mVBuf2,UInt16 *mVBuf,UInt16 *mobj,UInt16 *mClstSz,int nCluster,int nObjFin) ;
render(UInt16 *mHBuf,UInt16 *mVBuf2,UInt16 *mVBuf,UInt16 *mobj,UInt16 *mClstSz,int nCluster,int nObjFin) {
	int i,j,n,nob;
	if(alginf.command & (0x1 <<6)) {
//		 render(mHbuf,mVbuf2,mobj,mClstSz,nClust,nObjFin);
		if(alginf.command & (0x1 <<4)) {
			for(nob=0;nob<nObjFin;nob++) {
				i=mobj[nob]>>8;
				j=mobj[nob]&0xff;
				mHBuf[i*192+j]=mVBuf[nob];
	//		mClstSz[mVBuf[nob]]++; // may be list ????
				}
			}
		if(alginf.command & (0x1 <<5)) {
		 for(n=1;n<=nCluster;n++) {
			nob=mClstSz[n];
			while(nob<0xffff) {
				i=mobj[nob]>>8;
				j=mobj[nob]&0xff;
				mHBuf[i*192+j]=n; // Debug ????
				nob=mVBuf2[nob];
				}
			}
		  }
	 	 }

}
*/
double dsmr;

int signalProcess( UInt16 *dstbuf,unsigned int *srcbuf,int size)
{
//	volatile unsigned int *InBuff;
	//volatile unsigned int *OutBuff;
	int retval,i;
	double dmean;
//	double dmean, dsmr;
	stopwatch();
//	int size;
/*
	if(alginf.command & (0x1 <<10)) {
		for(i=0;i<20;i++)
			mymcpy((void *)dstbuf,(void*)0x80030000,size);
//			mymcpy((void *)dstbuf,(void*)0xc112000,size);

	}
*/
	if(alginf.command & (0x1 <<15)) {
			 mflt3x3((void *)dstbuf,(void*)srcbuf,192);
//			 clearfbuff((void *)0xc124000,192);
//			 alginf.nobj2=3;
		}
	else {
		if(alginf.command & (0x1 <<14)) {
				 mflt2x2((void *)dstbuf,(void*)srcbuf,192);
//				 alginf.nobj2=2;
				 if(alginf.command & (0x1 <<11))
						mymcpy((UInt16 *) 0x812400,(void *)dstbuf,size);
				 else
						hifr_filter((UInt16 *) 0x812400,(void*)dstbuf,size);
	//				statSMR( (UInt16 *) 0x812400, &dsmr,  &dmean);
				 alginf.niter=statHistSMR( (UInt16 *) 0x812400,  (UInt16 *) 0x824400, &dsmr,  &dmean);


			}
		else {
			if(alginf.command & (0x1 <<13))  {
				mymcpy((void *)dstbuf,(void*)srcbuf,size);
				if(alginf.command & (0x1 <<11))
					mymcpy((UInt16 *) 0x812400,(void *)dstbuf,size);
				else
					hifr_filter((UInt16 *) 0x812400,(void*)dstbuf,size);
//				statSMR( (UInt16 *) 0x812400, &dsmr,  &dmean);
				alginf.niter=statHistSMR( (UInt16 *) 0x812400,  (UInt16 *) 0x824400, &dsmr,  &dmean);
//				alginf.nobj2=4;
				}
			else {
				if(alginf.command & (0x1 <<12))  {
					//diffcpy((void *)dstbuf,(void*)srcbuf,(void*) 0xc112000,192,16);
					multpix((void *)dstbuf,(void*)srcbuf,(void*) 0xc112000,192,16);
					//alginf.nobj2=5;
				}
				else {
					mymcpy((void *)dstbuf,(void*)srcbuf,size);
//					alginf.nobj2=1;
					}
				}
			}
		}
	if((alginf.command & (0x1 <<13))==0)
		statSMR( dstbuf, &dsmr,  &dmean);
	alginf.uimean=(UInt16)(dmean*10);
	alginf.uismr=(UInt16)(dsmr*100);

	//m192BigFilter( UInt16 * inbuff, UInt16 * mHBuf, UInt16 * mVBuf,UInt16 * mobj,UInt16 * mobjMx,	UInt8 * mout, double dsmrpar, UInt16 thrhld, UInt16 mean);
	if(alginf.command & (0x1 <<10)) {
		if(alginf.command & (0x1 <<13) && !(alginf.command & (0x1 <<11))) {
			mymcpy((void *)dstbuf,(UInt16 *) 0x812400,size);
			}

		retval= m192BigFilter( (UInt16 *) 0x800400, (UInt16 *) 0x812400,  (UInt16 *) 0x824400,(UInt16 *) 0x836400, (UInt16 *) 0x83f400, (UInt8 *) 0x873600,dsmr,10*dsmr,dmean) ;
		}
//	 mflt2x2((void *)dstbuf,(void*)srcbuf,192);
//	 mflt3x3((void *)dstbuf,(void*)srcbuf,192);
//	int i;

	for(i=0;i<2000;i++) {
				asm("  nop");
	//			asm
			}
//	m192rnkFilter( UInt16 * inbuff, UInt16 * mHBuf, UInt16 * mVBuf,UInt16 * mobj,UInt16 * mobjMx,	UInt16 * mout) ;
//	m192rnkFilter( (UInt16 *) 0x800400, (UInt16 *) 0x812400,  (UInt16 *) 0x824400,(UInt16 *) 0x836400, (UInt16 *) 0x83f400, (UInt8 *) 0x873600) ;
	if((alginf.command & (0x1 <<10))==0) {
		retval=m192rnkFilter( (UInt16 *) 0x800400, (UInt16 *) 0x812400,  (UInt16 *) 0x824400,(UInt16 *) 0x836400, (UInt16 *) 0x83f400, (UInt8 *) 0x873600,dsmr) ;
		}

//	zoom2_192((unsigned short *)0xc100000,(unsigned short *)0xc1a0000); //only test Remove!
//	zoom2_192((unsigned short *)0xc112000,(unsigned short *)0xc1a5000); //only test Remove!

	gtm=stopwatch();
	ndgtm=0.1*gtm+ndgtm*0.9;
	return retval;

}

int clearfbuff(double *pfbuf,int size) {
	int i,j;
	for(i=0;i<size;i++) {
		for(j=0;j<size;j++) {
			pfbuf[i*192+j]=0.0;
		}
	}
	return 1;
}


int multpix(UInt16* dstbuf,UInt16* sigbuf,UInt16* backbuf,int size,UInt16 zero) {
	UInt16 *pibf=(UInt16 *)0xc124000;
	int j;
	int shift=alginf.command&0x0fff;
	if((++pibf[0])>=192  )
		pibf[0]=0;
	for(j=0;j<size;j++) {
		dstbuf[pibf[0]*192+j]=sigbuf[96*192+96+shift];
		}
	return 1;
}

int diffcpy(UInt16* dstbuf,UInt16* sigbuf,UInt16* backbuf,int size,UInt16 zero) {
int i,j;
double *pfbuf=(double *)0xc124000;
for(i=0;i<size;i++) {
	for(j=0;j<size;j++) {
		pfbuf[i*192+j]=pfbuf[i*192+j]*0.999 +sigbuf[i*192+j]*0.001;
		dstbuf[i*192+j]=round(pfbuf[i*192+j]);
//		dstbf[i*192+j]=(srcbf[i*192+j+1]+srcbf[(i+1)*192+j]+srcbf[(i+1)*192+j+1] +2)>>2;
//		dstbuf[i*192+j]=round((double)(dstbuf[i*192+j]*127+sigbuf[i*192+j])/128.0);
//		dstbuf[i*192+j]=round((double)(dstbuf[i*192+j]*1+sigbuf[i*192+j]*127)/128.0);
		}
}
return 1;
/*
	for(i=0;i<size;i++) {
		for(j=0;j<size;j++) {
	//		dstbf[i*192+j]=(srcbf[i*192+j+1]+srcbf[(i+1)*192+j]+srcbf[(i+1)*192+j+1] +2)>>2;
			dstbuf[i*192+j]=zero+sigbuf[i*192+j]- backbuf[i*192+j];
			}
	}
*/
}

int mflt2x2simple(void * dst,void * src,int size) {
//long long *ldst=(long long *)dst;
//long long *lsrc=(long long *)src;

unsigned short *dstbf=dst;
unsigned short *srcbf=src;
int i;
int j;
for(i=0;i<size-1;i++) {
	for(j=0;j<size-1;j++) {
//		dstbf[i*192+j]=(srcbf[i*192+j+1]+srcbf[(i+1)*192+j]+srcbf[(i+1)*192+j+1] +2)>>2;
		dstbf[i*192+j]=(srcbf[i*192+j]+ srcbf[i*192+j+1]+srcbf[(i+1)*192+j]+srcbf[(i+1)*192+j+1]);
		}

//	ldst[i]=lsrc[i];
	}
return 1;
}

int mflt2x2(void * dst,void * src,int size) {
//long long *ldst=(long long *)dst;
//long long *lsrc=(long long *)src;
unsigned short *dstbf=dst;
unsigned short *srcbf=src;
int i;
int j;
//alginf.nobj2=2;
for(i=0;i<191;i++) {
	for(j=0;j<191;j++) {
//		dstbf[i*192+j]=(srcbf[i*192+j+1]+srcbf[(i+1)*192+j]+srcbf[(i+1)*192+j+1] +2)>>2;
		dstbf[i*192+j]=(srcbf[i*192+j]+ srcbf[i*192+j+1]+srcbf[(i+1)*192+j]+srcbf[(i+1)*192+j+1])>>0;
//		dstbf[i*192+j]=(srcbf[i*192+j]);
		}

//	ldst[i]=lsrc[i];
	}

for(i=0,j=190;i<191;i++) {
	dstbf[(i+0)*192+j+1]=dstbf[i*192+j];
	}
for(j=0,i=190;j<192;j++) {
	dstbf[(i+1)*192+j+1]=dstbf[i*192+j];
	}


return 1;
}


int mflt3x3(void * dst,void * src,int size) {
//long long *ldst=(long long *)dst;
//long long *lsrc=(long long *)src;
unsigned short *dstbf=dst;
unsigned short *srcbf=src;
int i;
int j;
for(i=1;i<size-1;i++) {
	for(j=1;j<size-1;j++) {
//		dstbf[i*192+j]=(srcbf[i*192+j+1]+srcbf[(i+1)*192+j]+srcbf[(i+1)*192+j+1] +2)>>2;
		dstbf[i*192+j]=(srcbf[i*192+j]+ srcbf[i*192+j+1]+srcbf[(i+1)*192+j]+srcbf[(i+1)*192+j+1]+
						srcbf[i*192+j-1]+srcbf[(i+1)*192-j]+srcbf[(i-1)*192+j-1] + +srcbf[(i-1)*192+j+1] +srcbf[(i+1)*192+j-1])>>0;
		}

//	ldst[i]=lsrc[i];
	}

for(i=0,j=190;i<191;i++) {
	dstbf[(i+0)*192+j+1]=dstbf[i*192+j];
	}
for(j=0,i=190;j<192;j++) {
	dstbf[(i+1)*192+j+1]=dstbf[i*192+j];
	}

for(i=0,j=0;i<191;i++) {
	dstbf[(i+0)*192+j]=dstbf[i*192+j+1];
	}
for(j=0,i=0;j<192;j++) {
	dstbf[(i+0)*192+j+1]=dstbf[(i+1)*192+j];
	}

return 1;
}


int clear(void * dst,int size) {
long long *ldst=(long long *)dst;
int i;
for(i=0;i<size;i++) {

	ldst[i]=0;
	}
return 1;
}


int mymcpy(void * dst,void * src,int size) {
long long *ldst=(long long *)dst;
long long *lsrc=(long long *)src;
int i;
for(i=0;i<size;i++) {
	//for(j=0;j<00;j++) {
//		asm("	nop	");
//		asm("	nop	");
//		}


	ldst[i]=lsrc[i];
	}
return 1;
}

/*************************************************************************
 * stopwatch
 *************************************************************************/
UInt32 errtm=0;
UInt32 TSCLsav=0;
UInt32  stopwatch()
{
UInt32 retval=TSCL-TSCLsav;
    /* Get the current TSC  */
	TSCLsav=TSCL;
    return retval;

}
/*************************************************************************/

//int m192xrnkFilter( (UInt16 (*)[192])  mybuff,(UInt16 (*)[192]) mHBuf,(UInt16 (*)[192]) mVBuf) {
//int m192rnkFilter( UInt16 (* inbuff)[192], UInt16 (* mHBuf)[192] , UInt16 (* mVBuf)[192], \
//					UInt16 (* mout)[192],UInt16 (* mobjMx)[192]  ) {
volatile int nObjFin=0; // change local !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
volatile UInt8 isSimpleThr=0;
volatile double dk=3.0;
volatile double dk2=1.5;
volatile UInt16 err=0;
volatile UInt16 cnt=0;
volatile UInt16 mv1,mv2,mv3;

volatile double dmn2;
volatile double dsmr2;
volatile double dsmr3;


UInt32 statHistSMR( UInt16 *mbuff,UInt16 *histbuf, double *pfSMR,  double *pfMean) {
	double fMean,fSMR;
	double long dli;
	double di,dsmr;
//	double di,dsmr,dmean;
	int i,j,imax,imean,ncnt;
	unsigned int sqsm=0,sq=0,n;
	dli=0; sq=0;
	clear(histbuf,0x2400);
	for(i=0;i<192;i++) {
			sqsm=0;
			for(j=0;j<192;j++) {
				sq+=mbuff[i*192+j];
				sqsm+=mbuff[i*192+j]*mbuff[i*192+j];
				histbuf[mbuff[i*192+j]]++;
				}
			dli+=sqsm;
			}
	fMean=di=(double)sq/(192.0*192.0);
	//dsmr=sqrt((sqsm-dmn*dmn*(196.0*196.0))/(196.0*196.0));
	fSMR=sqrt(((double)dli-di*di*(192.0*192.0))/(192.0*192.0));
	ncnt=0;
	do {
//		dmean=fMean;
		dsmr=fSMR;
		imean=round(fMean);
		j=round(fSMR*5.0);
		if(imean>j)
			i=imean-j;
		else
			i=0;
		imax=imean+j;
		if(imax>=4*1024)
			imax=4*1024-1;
		dli=0; sq=0;
		for(n=0;i<imax;i++) {
			n+=histbuf[i];
			sq+=histbuf[i]*i;
			dli+=(double long )histbuf[i] *(double long )(i*i);
			}
		fMean=di=(double)sq/(double)n;
		fSMR=sqrt(((double)dli-di*di*(double)n)/(double)n);
		ncnt++;
	}
	while(fSMR<dsmr*0.9 && ncnt<5);

	*pfMean=fMean;
	*pfSMR=fSMR;
//	return (unsigned int)((fMean)*10.0);
	return ncnt;
}

UInt32 statSMR( UInt16 *mbuff, double *pfSMR,  double *pfMean) {
	double long dli;
	double di;
	int i,j;
	unsigned int sqsm=0,sq=0;
	dli=0; sq=0;
	for(i=0;i<192;i++) {
			sqsm=0;
			for(j=0;j<192;j++) {
				sq+=mbuff[i*192+j];
				sqsm+=mbuff[i*192+j]*mbuff[i*192+j];
				}
			dli+=sqsm;
			}
	*pfMean=di=(double)sq/(192.0*192.0);
	//dsmr=sqrt((sqsm-dmn*dmn*(196.0*196.0))/(196.0*196.0));
	*pfSMR=sqrt(((double)dli-di*di*(192.0*192.0))/(192.0*192.0));
	return (unsigned int)((*pfMean)*10.0);
}



UInt32 combSMR(UInt16 sh, UInt16 sm, UInt16 *mbuff,double fkSMR) {
	double long dli;
//	double dmn2;
//	double dsmr2;
	UInt16 is,js;
	 UInt32 sqsm=0,i,j,sq;
//	 double dsq;

//	is=sh/196;
//	js=sh%196;

	is=sh;
	js=sm;

	sq=0; dli=0;
//	return round(fkSMR*2.0);

	for(i=is-4,j=js-4;i<=is+4;i++) {
			sqsm=0;
//			for(j=0;j<20;j++) {
				sq+=mbuff[i*192+j];
				sqsm+=mbuff[i*192+j]*mbuff[i*192+j];
//			}
			dli+=sqsm;
	}

	for(i=is-4,j=js+4;i<=is+4;i++) {
			sqsm=0;
				sq+=mbuff[i*192+j];
				sqsm+=mbuff[i*192+j]*mbuff[i*192+j];
			dli+=sqsm;
	}

	for(j=js-3,i=is-4;j<=js+3;j++) {
			sqsm=0;
				sq+=mbuff[i*192+j];
				sqsm+=mbuff[i*192+j]*mbuff[i*192+j];
			dli+=sqsm;
	}

	for(j=js-3,i=is+4;j<=js+3;j++) {
			sqsm=0;
				sq+=mbuff[i*192+j];
				sqsm+=mbuff[i*192+j]*mbuff[i*192+j];
			dli+=sqsm;
	}


	dmn2=(double)sq/(32.0);
	//dsmr=sqrt((sqsm-dmn*dmn*(196.0*196.0))/(196.0*196.0));
	dsmr2=sqrt((dli-dmn2*dmn2*(32.0))/(32.0));

/*
	dsq=0; dli=0;

	for(i=is-4,j=js-4;i<=is+4;i++) {
			sqsm=0;
//			for(j=0;j<20;j++) {
				dsq+=(mbuff[i*192+j]-dmn2)*(mbuff[i*192+j]-dmn2);
				//sqsm+=mbuff[i*192+j]*mbuff[i*192+j];
//			}
			dli+=sqsm;
	}

	for(i=is-4,j=js+4;i<=is+4;i++) {
		dsq+=(mbuff[i*192+j]-dmn2)*(mbuff[i*192+j]-dmn2);
		dli+=sqsm;
	}

	for(j=js-3,i=is-4;j<=js+3;j++) {
		dsq+=(mbuff[i*192+j]-dmn2)*(mbuff[i*192+j]-dmn2);
			dli+=sqsm;
	}

	for(j=js-3,i=is+4;j<=js+3;j++) {
		dsq+=(mbuff[i*192+j]-dmn2)*(mbuff[i*192+j]-dmn2);
			dli+=sqsm;
	}

	dsmr3=sqrt(dsq/(32.0));

*/


//	return round(2.0*dsmr2);
//	return round(fkSMR*MIN(dsmr2,dsmr));

	return round(fkSMR*dsmr2);
//	return round(fkSMR*2.0);


}



//UInt32 combSMR(UInt16 sh, UInt16 sm, UInt16 *mbuff,double fkSMR,double fgSMR) {
UInt32  locCombSMR(UInt16 sh, UInt16 sm, UInt16 *mbuff,double fkSMR) {

	double long dli;
//	double dmn2;
//	double dsmr2;
	UInt16 is,js;
	 UInt32 sqsm=0,i,j,sq;
//	 double dsq;

//	is=sh/196;
//	js=sh%196;

	is=sh;
	js=sm;

	sq=0; dli=0;
//	return round(fkSMR*2.0);

	for(i=is-4,j=js-4;i<=is+4;i++) {
			sqsm=0;
//			for(j=0;j<20;j++) {

				sq+=mbuff[i*192+j];
				sqsm+=mbuff[i*192+j]*mbuff[i*192+j];

				sq+=mbuff[i*192+j-2];
				sqsm+=mbuff[i*192+j-2]*mbuff[i*192+j-2];

				sq+=mbuff[i*192+j-4];
				sqsm+=mbuff[i*192+j-4]*mbuff[i*192+j-4];

				sq+=mbuff[i*192+j-6];
				sqsm+=mbuff[i*192+j-6]*mbuff[i*192+j-6];
//			}
			dli+=sqsm;
	}

	for(i=is-4,j=js+4;i<=is+4;i++) {
			sqsm=0;
				sq+=mbuff[i*192+j];
				sqsm+=mbuff[i*192+j]*mbuff[i*192+j];

				sq+=mbuff[i*192+j+2];
				sqsm+=mbuff[i*192+j+2]*mbuff[i*192+j+2];

				sq+=mbuff[i*192+j+4];
				sqsm+=mbuff[i*192+j+4]*mbuff[i*192+j+4];

				sq+=mbuff[i*192+j+6];
				sqsm+=mbuff[i*192+j+6]*mbuff[i*192+j+6];

				dli+=sqsm;
	}

	for(j=js-3,i=is-4;j<=js+3;j++) {
			sqsm=0;
				sq+=mbuff[i*192+j];
				sqsm+=mbuff[i*192+j]*mbuff[i*192+j];

				sq+=mbuff[(i-2)*192+j];
				sqsm+=mbuff[(i-2)*192+j]*mbuff[(i-2)*192+j];

				sq+=mbuff[(i-4)*192+j];
				sqsm+=mbuff[(i-4)*192+j]*mbuff[(i-4)*192+j];

				sq+=mbuff[(i-6)*192+j];
				sqsm+=mbuff[(i-6)*192+j]*mbuff[(i-6)*192+j];
			dli+=sqsm;
	}

	for(j=js-3,i=is+4;j<=js+3;j++) {
			sqsm=0;
				sq+=mbuff[i*192+j];
				sqsm+=mbuff[i*192+j]*mbuff[i*192+j];

				sq+=mbuff[(i+2)*192+j];
				sqsm+=mbuff[(i+2)*192+j]*mbuff[(i+2)*192+j];

				sq+=mbuff[(i+4)*192+j];
				sqsm+=mbuff[(i+4)*192+j]*mbuff[(i+4)*192+j];

				sq+=mbuff[(i+6)*192+j];
				sqsm+=mbuff[(i+6)*192+j]*mbuff[(i+6)*192+j];
			dli+=sqsm;
	}


//	dmn2=(double)sq/(32.0);
//	dsmr2=sqrt((dli-dmn2*dmn2*(32.0))/(32.0));
	dmn2=(double)sq/(128.0);
	dsmr2=sqrt((dli-dmn2*dmn2*(128.0))/(128.0));

/*
	dsq=0; dli=0;

	for(i=is-4,j=js-4;i<=is+4;i++) {
			sqsm=0;
//			for(j=0;j<20;j++) {
				dsq+=(mbuff[i*192+j]-dmn2)*(mbuff[i*192+j]-dmn2);
				//sqsm+=mbuff[i*192+j]*mbuff[i*192+j];
//			}
			dli+=sqsm;
	}

	for(i=is-4,j=js+4;i<=is+4;i++) {
		dsq+=(mbuff[i*192+j]-dmn2)*(mbuff[i*192+j]-dmn2);
		dli+=sqsm;
	}

	for(j=js-3,i=is-4;j<=js+3;j++) {
		dsq+=(mbuff[i*192+j]-dmn2)*(mbuff[i*192+j]-dmn2);
			dli+=sqsm;
	}

	for(j=js-3,i=is+4;j<=js+3;j++) {
		dsq+=(mbuff[i*192+j]-dmn2)*(mbuff[i*192+j]-dmn2);
			dli+=sqsm;
	}

	dsmr3=sqrt(dsq/(32.0));

*/


//	return round(2.0*dsmr2);
//	return round(fkSMR*MIN(dsmr2,dsmr));
	return round(fkSMR*dsmr); // Temporary

//	return round(fkSMR*dsmr2);
//	return round(fkSMR*2.0);

}




UInt32  hifr_filter(UInt16 * mout , UInt16 * mybuff, UInt16 size) {
 int i,j;
	for(i=0;i<4;i++) {
		for(j=0;j<192;j++) {
			mout[i*192+j] =512;
		}
	}

	for(i=188;i<192;i++) {
		for(j=0;j<192;j++) {
			mout[i*192+j] =512;
		}
	}

	for(i=0;i<192;i++) {
		for(j=0;j<4;j++) {
			mout[i*192+j] =512;
		}
	}

	for(i=0;i<192;i++) {
		for(j=188;j<192;j++) {
			mout[i*192+j] =512;
		}
	}

	//dsum=0;
	for(i=4;i<188;i++) {
		for(j=4;j<188;j++) {
			mout[i*192+j]=512+mybuff[i*192+j]-	(( mybuff[(i-4)*192+(j-0)]+ mybuff[(i-4)*192+(j-4)]+ mybuff[(i-4)*192+(j+4)]+
			 mybuff[(i-0)*192+(j-4)]+ mybuff[(i-0)*192+(j+4)]+ mybuff[(i+4)*192+(j-0)]+ mybuff[(i+4)*192+(j-4)]+ mybuff[(i+4)*192+(j+4)])>>3);
//			mout[i*192+j]=512+mybuff[i*192+j]-	(( mybuff[(i-1)*192+(j-0)]+ mybuff[(i-1)*192+(j-1)]+ mybuff[(i-1)*192+(j+1)]+
//			 mybuff[(i-0)*192+(j-1)]+ mybuff[(i-0)*192+(j+1)]+ mybuff[(i+1)*192+(j-0)]+ mybuff[(i+1)*192+(j-1)]+ mybuff[(i+1)*192+(j+1)])>>3);
			}
		}
	return 1;
}


UInt32 localRectStat_(UInt16 *mbuff,double fkSMR,	UInt16 sh, UInt16 sw) {
	//double long dli;
//	double dmn2;
//	double dsmr2;
	UInt16 is,js;
	 UInt32 i,j,sq;
	 double dsq;
	 UInt16 maxLevel=0;
//	is=sh/196;
//	js=sh%196;
//	 return round(fkSMR*((double)2));

	is=sh;
	js=sw;

//	sq=0; dli=0;
	sq=0;
	for(i=is-4;i<=is+4;i++) {
		for(j=js-4;j<=js+4;j++) {
//			sqsm=0;
				maxLevel=MAX(maxLevel,mbuff[i*192+j]);
				sq+=mbuff[i*192+j];
//				sqsm+=mbuff[i*192+j]*mbuff[i*192+j];
//			dli+=sqsm;
		}
	}



	dsq=(double)sq/(81.0);



	return round(fkSMR*((double)maxLevel-dsq));


}




UInt32 localRectStat(UInt16 *mbuff,double fkSMR,	UInt16 sh, UInt16 sw) {
	//double long dli;
//	double dmn2;
//	double dsmr2;
	UInt16 is,js,im,jm,ie,je;
	 UInt32 i,j,sq,sqs,n;

	 double dsq;
	 UInt16 maxLevel=0;
//	is=sh/196;
//	js=sh%196;
//	 return round(fkSMR*((double)2));

	is=sh;
	js=sw;
	im=jm=0;
//	sq=0; dli=0;
	sq=0;
	for(i=is-4;i<=is+4;i++) {
		for(j=js-4;j<=js+4;j++) {
//			sqsm=0;
//				maxLevel=MAX(maxLevel,mbuff[i*192+j]);
				if(mbuff[i*192+j]>maxLevel) {
					maxLevel=mbuff[i*192+j];
					im=i; jm=j;
					}
				sq+=mbuff[i*192+j];
//				sqsm+=mbuff[i*192+j]*mbuff[i*192+j];
//			dli+=sqsm;
		}
	}

	dsq=(double)sq/(81.0);
	i=im-3; 	if( i<is-4) i=is-4;
	j=jm-3; 	if( j<js-4) j=js-4;

	ie=im+3; 	if( ie>is+4) i=is+4;
	je=jm+3; 	if( je>js+4) j=js+4;

	sqs=0;

	for(n=0;i<=ie;i++) {
		for(;j<=je;j++) {
			sqs+=mbuff[i*192+j];
			n++;
			}
		}

	dsq=(double)(sq-sqs)/(double)(81-n);
	return round(fkSMR*((double)maxLevel-dsq));


}

extern volatile int gn;

UInt32 localRectStatXL(UInt16 *mbuff,double fkSMR,	UInt16 sh, UInt16 sw) {
	//double long dli;
//	double dmn2;
//	double dsmr2;
	UInt16 is,js,im,jm,ie,je;
	 UInt32 i,j,sq,sqs,n;

	 double dsq;
	 UInt16 maxLevel=0;
//	is=sh/196;
//	js=sh%196;
//	 return round(fkSMR*((double)2));
	 if(sh<8 || sh>=192-8 || sw<8 || sw>=192-8 )
		 return 20.0;

	is=sh;
	js=sw;
	im=jm=0;
//	sq=0; dli=0;
	sq=0;
	for(i=is-8;i<=is+8;i++) {
		for(j=js-8;j<=js+8;j++) {
//			sqsm=0;
//				maxLevel=MAX(maxLevel,mbuff[i*192+j]);
				if(mbuff[i*192+j]>maxLevel) {
					maxLevel=mbuff[i*192+j];
					im=i; jm=j;
					}
				sq+=mbuff[i*192+j];
//				sqsm+=mbuff[i*192+j]*mbuff[i*192+j];
//			dli+=sqsm;
		}
	}

//	dsq=(double)sq/(81.0);
	dsq=(double)sq/(289.0);
	i=im-3; 	if( i<is-4) i=is-4;
	j=jm-3; 	if( j<js-4) j=js-4;

	ie=im+3; 	if( ie>is+4) i=is+4;
	je=jm+3; 	if( je>js+4) j=js+4;

	sqs=0;
	n=0;
	for(;i<=ie;i++) {
		for(;j<=je;j++) {
			sqs+=mbuff[i*192+j];
			n++;
			}
		}

	dsq=(double)(sq-sqs)/(double)(289-n);
//	dsq=(double)(sq-sqs)/(double)(81-n);
	gn=n;
	return round(fkSMR*((double)maxLevel-dsq));


}


UInt32 localBigRectStat(UInt16 *mbuff,double fkSMR,UInt16 mean,	UInt16 sh, UInt16 sw) {
	//double long dli;
//	double dmn2;
//	double dsmr2;
	UInt16 is,js;
	 UInt32 i,j;
	 double dsq;
	 UInt16 maxLevel=0;
//	is=sh/196;
//	js=sh%196;
//	 return round(fkSMR*((double)2));

	is=sh;
	js=sw;

//	sq=0;
	for(i=is-4;i<=is+4;i++) {
		for(j=js-4;j<=js+4;j++) {
//			sqsm=0;
				maxLevel=MAX(maxLevel,mbuff[i*192+j]);
				//sq+=mbuff[i*192+j];
//				sqsm+=mbuff[i*192+j]*mbuff[i*192+j];
//			dli+=sqsm;
		}
	}



//	dsq=(double)sq/(81.0);
	dsq=(double)mean;


	return round(fkSMR*((double)maxLevel-dsq));


}

void zoom2_192(unsigned short *psrc,unsigned short *pdst) {
	int i,j;
	for(j=0;j<192/2;j++) {
		for(i=0;i<192/2;i++) {
			pdst[j*96+i] =psrc[2*j*192+2*i]+psrc[2*j*192+2*i+1]+psrc[(2*j+1)*192+2*i]+psrc[(2*j+1)*192+2*i+1];
			}
		}
}


