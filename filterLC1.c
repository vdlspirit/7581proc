#include "signalProc.h"
#include "signalProcCom.h"
#include "alg_add.h"
#include "math.h"
//#pragma DATA_SECTION(r3j, ".l2Buff")
//volatile int r3j=0;

extern volatile int fr34j;
//#pragma DATA_SECTION(gtm, ".l2Buff")
//extern unsigned int gtm;

//#pragma DATA_SECTION(dgtm, ".l2Buff")
extern volatile  double ndgtm;

int mflt2x2_144(void * dst,void * src,int size);
int mflt3x3_144(void * dst,void * src,int size);

int diffcpy144(UInt16* dstbuf,UInt16* sigbuf,UInt16* backbuf,int size,UInt16 zero) ;
int multpix(UInt16* dstbuf,UInt16* sigbuf,UInt16* backbuf,int size,UInt16 zero) ;
int clearfbuff144(double *pfbuf,int size) ;
int clear(void * dst,int size) ;






extern volatile int nObjFin; // change local !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
extern volatile UInt8 isSimpleThr;
extern volatile double dk;
extern volatile double dk2;
extern volatile UInt16 err;
extern volatile UInt16 cnt;
//extern volatile UInt16 mv1,mv2,mv3;

extern volatile CLUSTERINF clstmax;

int m144rnkFilter( UInt16 * inbuff, UInt16 * mHBuf, UInt16 * mVBuf,UInt16 * mobj,UInt16 * mobjMx,	UInt8 * mout, double dsmrpar) {

//	void hr3mn64(UInt16 (* InpArr)[SZCadr196],UInt16 (* outArr)[SZCadr196])
	int i,j,n,c00,c01,c02,nob;
	double long dlb,dlim,dljm;
	int nObj=0;
	int nObjFin=0;
	int adds=0;
	int addthr=1;
	int  nMax=100000;
	int nObjMax;
	nObjFin=0;
	UInt16 nsecCluster,nCluster;
//	int nMaxClst;
//	UInt8 isSimpleThr=0;
	int thr;
	UInt16 * mVBuf2= mVBuf+0x4800;
	UInt16 * mClstSz= (UInt16 *)mout;
	CLUSTERINF clst;
//	fr34j+=1.0; // Delete debug
	//fr34j+=1;
	vr5mx3mn144( (UInt16 (*)[144])  inbuff,(UInt16 (*)[144]) mVBuf,(UInt16 (*)[144]) mVBuf);
	hr5mx3mn144( (UInt16 (*)[144])  inbuff,(UInt16 (*)[144]) mHBuf,(UInt16 (*)[144]) mHBuf);

//	return 1;
//	mymcpy((void *)dstbuf,(void*)srcbuf,size);
	//0xc112000 //0xc124000
	//memcpy((void *)(0x800400),(void*)0xc124000,73728);
	for(i=0;i<144;i++) {
			for(j=0;j<144;j++) {
//				mout[i*144+j]=0;
				mout[i*144+j]=0xff;
			}
		}

	for(i=4;i<144-4;i++) {
			for(j=4;j<144-4;j++) {
				c01=MAX(mVBuf[(i-3)*144+(j-4)],mVBuf[(i-3)*144+(j+4)]);
				c02=MAX(mHBuf[(i-4)*144+(j-3)],mHBuf[(i+4)*144+(j-3)]);
//				c01=MAX(mybuff[(i-0)*196+(j-4)],mybuff[(i+0)*196+(j+4)]);
//				c02=MAX(mybuff[(i-4)*196+(j-0)],mybuff[(i+4)*196+(j-0)]);
				c01=MAX(c01,c02);
				c00=MAX(mVBuf[(i+3)*144+(j-4)],mVBuf[(i+3)*144+(j+4)]);
				c02=MAX(mHBuf[(i-4)*144+(j+3)],mHBuf[(i+4)*144+(j+3)]);
				c00=MAX(c00,c02);
				c00=MAX(c00,c01);

				if(inbuff[i*144+j]> c00 +adds) {
							mobj[nObj]=(i<<8)+j;
//							myout[i*196+j]=1;
//							myobjMax[nObj]=c01; // 07.08.2014
							mobjMx[nObj]=c00;
							nObj++;
						}

			}



		}
	nsecCluster=1;
	clear(mHBuf,0x2400);
	clear(mVBuf,0x2400);
	nObjFin=nObj;
	nCluster=0;
	nObjMax=MIN(nObj,nMax);
	nObjFin=0;
		for(nob=0;nob<nObjMax;nob++) {

			i=mobj[nob]>>8;
			j=mobj[nob]&0xff;
			if(isSimpleThr)
				thr=adds+addthr;
			else
				thr=locCombSMR144(i,j,inbuff,dk);
//				thr=combSMR(i,j,inbuff,dk,dsmr);
//				thr=combSMR(i,j,inbuff,dk);
//				thr=combSMR2(i,j,inbuff,dk);
			c00=mobjMx[nob];

			if(isSimpleThr<2) {

				if(inbuff[i*144+j]>= c00+thr) {
//				mout[i*144+j]=1;
//					c02=localRectStat144((void*)0xc136000,dk2,	i, j);
					c02=localRectStat144(pbgrnd,dk2,	i, j);


//					c02=localRectStat((void*)0xc100000,dk2,	i, j);
					if(inbuff[i*144+j]>c00+c02) {
//						mout[i*144+j]=thr;

						c01=nsecCluster;
						if(mHBuf[(i-1)*144+j]>0)
						{
							n=mHBuf[(i-1)*144+j];
							while(mVBuf[mVBuf[n]] != mVBuf[n]) {
								cnt++;
								if(mVBuf[mVBuf[n]]>20)
									err++;
								mVBuf[n]=mVBuf[mVBuf[n]];
								}
							c01=mVBuf[n];
						}
						else {
							if(mHBuf[i*144+j-1]>0 || mHBuf[(i-1)*144+j-1]>0) { // idea - change 0 <-> 0xffff !!!!!!!!!!!!!!
								if(mHBuf[i*144+j-1]>0)
									n=mHBuf[i*144+j-1];
								else
									n=mHBuf[(i-1)*144+j-1];
								while(mVBuf[mVBuf[n]] != mVBuf[n]) {
									cnt++;
									if(mVBuf[mVBuf[n]]>20)
										err++;
									mVBuf[n]=mVBuf[mVBuf[n]];
									}
								c01=mVBuf[n];

								if(mHBuf[(i-1)*144+j+1]>0) {
									n=mHBuf[(i-1)*144+j+1];
									while(mVBuf[mVBuf[n]] != mVBuf[n]) {
										cnt++;
										if(mVBuf[mVBuf[n]]>20)
											err++;
										mVBuf[n]=mVBuf[mVBuf[n]];
										}

									if(c01>mVBuf[n]) {
										mVBuf[c01]=mVBuf[n];
										c01=mVBuf[n];
										}
									else {
										mVBuf[mVBuf[n]]=c01; // Need Debug !!!!!!!!!!!!!!
										mVBuf[n]=c01;
									}

//									c01=MIN(mVBuf[n],c01);
//									mVBuf[mHBuf[(i-1)*144+j+1]]=mVBuf[mHBuf[i*144+j-1]]=c01;
									}
//								else {
//									c01=MAX(mHBuf[(i-1)*144+j-1],mHBuf[(i-0)*144+j-1]);  ???????
//									}
								}
							else {
								if(mHBuf[(i-1)*144+j+1]>0) {
									n=mHBuf[(i-1)*144+j+1];
									while(mVBuf[mVBuf[n]] != mVBuf[n]) {
										cnt++;
										if(mVBuf[mVBuf[n]]>20)
											err++;
										mVBuf[n]=mVBuf[mVBuf[n]];
										}
									c01=mVBuf[n];
									}
								else {
									mVBuf[nsecCluster]=nsecCluster;
									nsecCluster++;
									}
							}

						}


						mHBuf[i*144+j]=c01; // clear mHBuf ????
						mobj[nObjFin]=mobj[nob];
						mobjMx[nObjFin]=c00;
						nObjFin++;



						if(nObjFin>2500)
							err++;
						}
				}
			}
			else {
				if(inbuff[i*144+j]> mobjMx[n]+adds) {
//				mout[i*144+j]=1;
//				c02=localRectStat144((void*)0xc136000,10.0,	i, j);
				c02=localRectStat144(pbgrnd,dk2,	i, j);

//				c02=localRectStat((void*)0xc100000,10.0,	i, j);
				if(inbuff[i*144+j]>c00+c02) {
//					smClst=mout[i*144+j-1]+mout[(i-1)*144+j-1]+mout[(i-1)*144+j+0]+mout[(i-1)*144+j+1];
//					if(smClst>0)
					mout[i*144+j]=thr;
					mobj[nObjFin]=(i<<8)+j;
					mobjMx[nObjFin]=c00;

					c01=nsecCluster;
					if(mHBuf[i*144+j-1]>0) { // idea - change 0 <-> 0xffff !!!!!!!!!!!!!!
						if(mHBuf[(i-1)*144+j+1]>0) {
							c01=MIN(mHBuf[i*144+j-1],mHBuf[(i-1)*144+j+1]);
							mVBuf[mHBuf[(i-1)*144+j+1]]=mVBuf[mHBuf[i*144+j-1]]=c01;
							}
						else
							c01=mHBuf[(i-0)*144+j-1];
						}
					else {
						if(mHBuf[(i-1)*144+j+1]>0) {
							c01=mHBuf[(i-1)*144+j+1];
							}
						else {
							mVBuf[nsecCluster]=nsecCluster;
							nsecCluster++;
							}

					}

					if(c01>nObjFin) {
						err++;
						mHBuf[i*144+j]=c00; // clear mHBuf ????
						}
					if(c01>1000) {
						err++;
						mHBuf[i*144+j]=c02; // clear mHBuf ????
						}

					mHBuf[i*144+j]=c01; // clear mHBuf ????
					mobj[nObjFin]=(i<<8)+j;
					mobjMx[nObjFin]=c00;
					nObjFin++;

					}
				}

			}


		}
	nCluster=0;
	cnt=0;

	if(mVBuf[1]>20)
		err++;

	if(mVBuf[2]>20)
			err++;

	if(mVBuf[3]>20)
			err++;

	if(nObjFin>1000) {
		err++;
		}





	for(n=1;n<nsecCluster;n++) {
		while(mVBuf[mVBuf[n]] != mVBuf[n]) {
			cnt++;
			mVBuf[n]=mVBuf[mVBuf[n]];
			}

		}

	for(n=1;n<nsecCluster;n++) {
		if(n==mVBuf[n]) {
			nCluster++;
			mVBuf[n]=nCluster;
			}
		else {
			mVBuf[n]=mVBuf[mVBuf[n]];
			}

		}

	for(nob=0;nob<nObjFin;nob++) {
		i=mobj[nob]>>8;
		j=mobj[nob]&0xff;
		mHBuf[i*144+j]=mVBuf[mHBuf[i*144+j]];
		}

	for(nob=0;nob<nObjFin;nob++) {
		i=mobj[nob]>>8;
		j=mobj[nob]&0xff;
		mVBuf[nob]=mHBuf[i*144+j];
//		mClstSz[mVBuf[nob]]++; // may be list ????
		}

	for(nob=0;nob<nObjFin;nob++) {
		n=mVBuf[nob];
		mVBuf2[nob]=mClstSz[n];
		mClstSz[n]=nob;
		}


	clear(mHBuf,0x2400);
	 clstmax.npt=0;
	 clstmax.suml=clstmax.maxl=clstmax.lt =clstmax.rb =0;

if(alginf.command & (0x1 <<6)) {

	if(alginf.command & (0x1 <<4)) {
	 clstmax.npt=0;
	 clstmax.suml=clstmax.maxl=0;
/*
	 unsigned short lt;
	 unsigned short rb;
	 unsigned short maxl;
	 unsigned int   suml;
	 unsigned short snr;
	 unsigned short bgrnd;
	 unsigned short npt;
 */
	 for(n=1;n<=nCluster;n++) {
		nob=mClstSz[n];
		clst.npt=clst.suml=clst.maxl=0;
		clst.lt=0xffff; clst.rb=0; clst.bgrnd=0xffff;
		while(nob<0xffff) {
			i=mobj[nob]>>8;
			j=mobj[nob]&0xff;
			clst.npt++;

			if((clst.lt>>8)>i)
				clst.lt= (clst.lt&0x00ff) | (i<<8);
			if((clst.lt & 0xff)>j)
				clst.lt= (clst.lt&0xff00) | j;

			if((clst.rb>>8)<i)
				clst.rb= (clst.rb&0x00ff) | (i<<8);
			if((clst.rb & 0xff)<j)
				clst.rb= (clst.rb&0xff00) | j;

			if(clst.bgrnd>mobjMx[nob])
				clst.bgrnd=mobjMx[nob];
			if(clst.maxl<inbuff[i*144+j])
				  clst.maxl=inbuff[i*144+j];
			clst.suml+=inbuff[i*144+j];
			mHBuf[i*144+j]=n; // Debug ????
			nob=mVBuf2[nob];
			}
		clst.maxl-=clst.bgrnd;
		clst.suml-=clst.bgrnd*clst.npt;
		if(clst.suml>clstmax.suml) {
//			nMaxClst=n;
			clstmax.npt=clst.npt;
			clstmax.suml=clst.suml;
			clstmax.maxl=clst.maxl;
			clstmax.lt=clst.lt;
			clstmax.rb=clst.rb;
			clstmax.bgrnd=clst.bgrnd;
			}
		}
	 }

	if(alginf.command & (0x1 <<5)) {
		for(nob=0;nob<nObjFin;nob++) {
			i=mobj[nob]>>8;
			j=mobj[nob]&0xff;
			mHBuf[i*144+j]=mVBuf[nob];
//		mClstSz[mVBuf[nob]]++; // may be list ????
			}
	}
 }

/*
 if(alginf.command & (0x1 <<6)) {
//	 render(UInt16 *mHBuf,UInt16 *mVBuf2,UInt16 *mVBuf,UInt16 *mobj,UInt16 *mClstSz,int nCluster,int nObjFin) ;
//	 render(mHBuf,mVBuf2,mVBuf,mobj,mClstSz,nCluster,nObjFin);

	if(alginf.command & (0x1 <<4)) {
		for(nob=0;nob<nObjFin;nob++) {
			i=mobj[nob]>>8;
			j=mobj[nob]&0xff;
			mHBuf[i*144+j]=mVBuf[nob];
//		mClstSz[mVBuf[nob]]++; // may be list ????

			}
		}
	if(alginf.command & (0x1 <<5)) {
	 for(n=1;n<=nCluster;n++) {
		nob=mClstSz[n];
		while(nob<0xffff) {
			i=mobj[nob]>>8;
			j=mobj[nob]&0xff;
			mHBuf[i*144+j]=n; // Debug ????
			nob=mVBuf2[nob];
			}
		}
	  }

 	 }
 	 */
/*


	for(n=1;n<nsecCluster;n++) {
		while(mVBuf[mVBuf[n]] != mVBuf[n]) {
			cnt++;
			if(mVBuf[mVBuf[n]]>20)
				err++;
			mVBuf[n]=mVBuf[mVBuf[n]];
			}

		if(n==mVBuf[n]) {
			nCluster++;
			mVBuf2[n]=nCluster;
			}
		else {
			mVBuf2[n]=mVBuf2[mVBuf[n]];
			}


		//nCluster=MAX(nCluster,mVBuf[n]);
		}

*/

/*
	for(nob=0;nob<nObjFin;nob++) {
		i=mobj[nob]>>8;
		j=mobj[nob]&0xff;
		mVBuf[nob]=mVBuf2[mHBuf[i*144+j]];
//		mClstSz[mVBuf[nob]]++; // may be list ????
		}



	for(nob=0;nob<nObjFin;nob++) {
		i=mobj[nob]>>8;
		j=mobj[nob]&0xff;
		mHBuf[i*144+j]=mVBuf[nob]; // Debug ????
		}
*/

/*
	for(nob=0;nob<nObjFin;nob++) {
		i=mobj[nob]>>8;
		j=mobj[nob]&0xff;
		n=mVBuf2[mHBuf[i*144+j]];
		mVBuf[nob]=mClstSz[n];
		mClstSz[n]=nob;
		}



	for(n=1;n<=nCluster;n++) {
		nob=mClstSz[n];
		while(nob<0xffff) {
			i=mobj[nob]>>8;
			j=mobj[nob]&0xff;
			mHBuf[i*144+j]=n; // Debug ????
			nob=mVBuf[nob];
			}
		}

*/


/*
	for(i=0;i<144;i++) {
		for(j=0;j<144;j++) {
			mHBuf[i*144+j]=mVBuf2[mHBuf[i*144+j]]; // Debug ????
			}
		}
*/
	if(nCluster>1)
		err++;
/*
		nObjMax=nObjFin;
		nObjFin=0;
		for(n=0;n<nObjMax;n++) {

				i=mobj[n]>>8;
				j=mobj[n]&0xff;
		}
*/


	alginf.nobj=nObjFin;
//	alginf.nobj2=nsecCluster-1;
	alginf.nobj2=nCluster;

	alginf.npt=nObjMax;
	alginf.sz=clstmax.npt;
	if(clstmax.npt>0) {
		i=(clstmax.lt>>8)-1; c00=(clstmax.rb>>8)+1;
		dlb=dlim=dljm=0; 	 c01=(clstmax.rb&0xff)+1;
		for(;i<=c00;i++) {
			j=(clstmax.lt&0xff)-1;
			for(;j<=c01;j++) {
			if(inbuff[i*144+j]>clstmax.bgrnd)
				c02=inbuff[i*144+j]-clstmax.bgrnd;
				dlim+=c02*i;
				dljm+=c02*j;
				dlb+=c02;
				}
			}
		if(dlb>0) {
			alginf.x =round(((double)dljm /(double)dlb)*10.0);
			alginf.y =round(((double)dlim /(double)dlb)*10.0);
			}
		else {
			alginf.x=((clstmax.lt&0xff) + (clstmax.rb&0xff))/2;
			alginf.y=((clstmax.lt>>8) + (clstmax.rb>>8))/2;
			}
		alginf.x2=((clstmax.lt&0xff) + (clstmax.rb&0xff))/2;
		alginf.y2=((clstmax.lt>>8) + (clstmax.rb>>8))/2;
		alginf.bgrnd=clstmax.bgrnd*10;

//		alginf.level=clstmax.maxl - clstmax.bgrnd;
		alginf.level=clstmax.maxl;

		alginf.size=(((clstmax.rb>>8) - (clstmax.lt>>8)+1)<<8) | ((clstmax.rb&0xff)-(clstmax.lt&0xff));
		alginf.rms=locCombSMR144((alginf.y+5)/10,(alginf.x+5)/10,inbuff,100.0);
		alginf.snr=round(1000.0*(double)alginf.level/(double)alginf.rms);

		}
	else {
//		alginf.x=alginf.y=alginf.x2=alginf.y2=alginf.bgrnd=alginf.rms=0;
		}


	alginf.tm=round(ndgtm/10000.0);
	dk=((double)alginf.ksmr)/10.0;
	dk2=((double)alginf.kbgrnd)/10.0;

//	return nObjFin;
	return nCluster;
}

volatile int gcnterr=0;
volatile unsigned short maxcord=900;
int m144BigFilter( UInt16 * inbuff, UInt16 * mHBuf, UInt16 * mVBuf,UInt16 * mobj,UInt16 * mobjMx,	UInt8 * mout, double dsmrpar, UInt16 thrhld, UInt16 mean) {

//	void hr3mn64(UInt16 (* InpArr)[SZCadr196],UInt16 (* outArr)[SZCadr196])
	int i,j,n,c00,c01,c02,nob;
	double long dlb,dlim,dljm;
	int nObj=0;
	int nObjFin=0;
	int adds=0;
//	int addthr=1;
	int  nMax=100000;
	int nObjMax;
	nObjFin=0;
	UInt16 nsecCluster,nCluster;
//	int nMaxClst;

//	int smClst;
//	UInt8 isSimpleThr=0;
	int thr;
	UInt16 * mVBuf2= mVBuf+0x4800;
	UInt16 * mClstSz= (UInt16 *)mout;
	CLUSTERINF clst;
//	fr34j+=1.0; // Delete debug
	//fr34j+=1;
//	vr5mx3mn64( (UInt16 (*)[144])  inbuff,(UInt16 (*)[144]) mVBuf,(UInt16 (*)[144]) mVBuf);
//	hr5mx3mn64( (UInt16 (*)[144])  inbuff,(UInt16 (*)[144]) mHBuf,(UInt16 (*)[144]) mHBuf);

//	return 1;
//	mymcpy((void *)dstbuf,(void*)srcbuf,size);
	//0xc112000 //0xc124000
	//memcpy((void *)(0x800400),(void*)0xc124000,73728);
	for(i=0;i<144;i++) {
			for(j=0;j<144;j++) {
//				mout[i*144+j]=0;
				mout[i*144+j]=0xff;
			}
		}

	for(i=4;i<144-4;i++) {
			for(j=4;j<144-4;j++) {
				c00=thr;
//				if(inbuff[i*144+j]> c00 +adds) {
				if(inbuff[i*144+j]> mean +thrhld ) {
							mobj[nObj]=(i<<8)+j;
//							myout[i*196+j]=1;
//							myobjMax[nObj]=c01; // 07.08.2014
							mobjMx[nObj]=mean;
							nObj++;
						}

			}



		}
	nsecCluster=1;
	clear(mHBuf,0x2400);
	clear(mVBuf,0x2400);
	nObjFin=nObj;
	nCluster=0;
	nObjMax=MIN(nObj,nMax);
	nObjFin=0;
		for(nob=0;nob<nObjMax;nob++) {

			i=mobj[nob]>>8;
			j=mobj[nob]&0xff;
			if(isSimpleThr)
				thr=thrhld;
			else
				thr=thrhld;
//				thr=locCombSMR144(i,j,inbuff,dk);
//				thr=combSMR(i,j,inbuff,dk,dsmr);
//				thr=combSMR(i,j,inbuff,dk);
//				thr=combSMR2(i,j,inbuff,dk);
			c00=mobjMx[nob];

			if(isSimpleThr<2) {

				if(inbuff[i*144+j]>= c00+thr) {
//				mout[i*144+j]=1;
					//c02=1;
					//c02=localRectStat144((void*)0xc136000,dk2,	i, j);
					//c02=localRectStat144(pbgrnd,dk2,	i, j);
					c02=localBigRectStat144(pbgrnd,dk2,mean,i, j);

//					c02=localRectStat((void*)0xc100000,dk2,	i, j);
					if(inbuff[i*144+j]>c00+c02) {
//						mout[i*144+j]=thr;

						c01=nsecCluster;
						if(mHBuf[(i-1)*144+j]>0)
						{
							n=mHBuf[(i-1)*144+j];
							while(mVBuf[mVBuf[n]] != mVBuf[n]) {
								cnt++;
								if(mVBuf[mVBuf[n]]>20)
									err++;
								mVBuf[n]=mVBuf[mVBuf[n]];
								}
							c01=mVBuf[n];
						}
						else {
							if(mHBuf[i*144+j-1]>0 || mHBuf[(i-1)*144+j-1]>0) { // idea - change 0 <-> 0xffff !!!!!!!!!!!!!!
								if(mHBuf[i*144+j-1]>0)
									n=mHBuf[i*144+j-1];
								else
									n=mHBuf[(i-1)*144+j-1];
								while(mVBuf[mVBuf[n]] != mVBuf[n]) {
									cnt++;
									if(mVBuf[mVBuf[n]]>20)
										err++;
									mVBuf[n]=mVBuf[mVBuf[n]];
									}
								c01=mVBuf[n];

								if(mHBuf[(i-1)*144+j+1]>0) {
									n=mHBuf[(i-1)*144+j+1];
									while(mVBuf[mVBuf[n]] != mVBuf[n]) {
										cnt++;
										if(mVBuf[mVBuf[n]]>20)
											err++;
										mVBuf[n]=mVBuf[mVBuf[n]];
										}

									if(c01>mVBuf[n]) {
										mVBuf[c01]=mVBuf[n];
										c01=mVBuf[n];
										}
									else {
										mVBuf[mVBuf[n]]=c01; // Need Debug !!!!!!!!!!!!!!
										mVBuf[n]=c01;
									}

//									c01=MIN(mVBuf[n],c01);
//									mVBuf[mHBuf[(i-1)*144+j+1]]=mVBuf[mHBuf[i*144+j-1]]=c01;
									}
//								else {
//									c01=MAX(mHBuf[(i-1)*144+j-1],mHBuf[(i-0)*144+j-1]);  ???????
//									}
								}
							else {
								if(mHBuf[(i-1)*144+j+1]>0) {
									n=mHBuf[(i-1)*144+j+1];
									while(mVBuf[mVBuf[n]] != mVBuf[n]) {
										cnt++;
										if(mVBuf[mVBuf[n]]>20)
											err++;
										mVBuf[n]=mVBuf[mVBuf[n]];
										}
									c01=mVBuf[n];
									}
								else {
									mVBuf[nsecCluster]=nsecCluster;
									nsecCluster++;
									}
							}

						}


						mHBuf[i*144+j]=c01; // clear mHBuf ????
						mobj[nObjFin]=mobj[nob];
						mobjMx[nObjFin]=c00;
						nObjFin++;



						if(nObjFin>2500)
							err++;
						}
				}
			}
			else {
				if(inbuff[i*144+j]> mobjMx[n]+adds) {
//				mout[i*144+j]=1;
//				c02=localRectStat((void*)0xc136000,10.0,	i, j);
				c02=localRectStat144(pbgrnd,10.0,	i, j);

//				c02=localRectStat((void*)0xc100000,10.0,	i, j);
				if(inbuff[i*144+j]>c00+c02) {
//					smClst=mout[i*144+j-1]+mout[(i-1)*144+j-1]+mout[(i-1)*144+j+0]+mout[(i-1)*144+j+1];
//					if(smClst>0)
					mout[i*144+j]=thr;
					mobj[nObjFin]=(i<<8)+j;
					mobjMx[nObjFin]=c00;

					c01=nsecCluster;
					if(mHBuf[i*144+j-1]>0) { // idea - change 0 <-> 0xffff !!!!!!!!!!!!!!
						if(mHBuf[(i-1)*144+j+1]>0) {
							c01=MIN(mHBuf[i*144+j-1],mHBuf[(i-1)*144+j+1]);
							mVBuf[mHBuf[(i-1)*144+j+1]]=mVBuf[mHBuf[i*144+j-1]]=c01;
							}
						else
							c01=mHBuf[(i-0)*144+j-1];
						}
					else {
						if(mHBuf[(i-1)*144+j+1]>0) {
							c01=mHBuf[(i-1)*144+j+1];
							}
						else {
							mVBuf[nsecCluster]=nsecCluster;
							nsecCluster++;
							}

					}

					if(c01>nObjFin) {
						err++;
						mHBuf[i*144+j]=c00; // clear mHBuf ????
						}
					if(c01>1000) {
						err++;
						mHBuf[i*144+j]=c02; // clear mHBuf ????
						}

					mHBuf[i*144+j]=c01; // clear mHBuf ????
					mobj[nObjFin]=(i<<8)+j;
					mobjMx[nObjFin]=c00;
					nObjFin++;

					}
				}

			}


		}
	nCluster=0;
	cnt=0;

	if(mVBuf[1]>20)
		err++;

	if(mVBuf[2]>20)
			err++;

	if(mVBuf[3]>20)
			err++;

	if(nObjFin>1000) {
		err++;
		}





	for(n=1;n<nsecCluster;n++) {
		while(mVBuf[mVBuf[n]] != mVBuf[n]) {
			cnt++;
			mVBuf[n]=mVBuf[mVBuf[n]];
			}

		}

	for(n=1;n<nsecCluster;n++) {
		if(n==mVBuf[n]) {
			nCluster++;
			mVBuf[n]=nCluster;
			}
		else {
			mVBuf[n]=mVBuf[mVBuf[n]];
			}

		}

	for(nob=0;nob<nObjFin;nob++) {
		i=mobj[nob]>>8;
		j=mobj[nob]&0xff;
		mHBuf[i*144+j]=mVBuf[mHBuf[i*144+j]];
		}

	for(nob=0;nob<nObjFin;nob++) {
		i=mobj[nob]>>8;
		j=mobj[nob]&0xff;
		mVBuf[nob]=mHBuf[i*144+j];
//		mClstSz[mVBuf[nob]]++; // may be list ????
		}

	for(nob=0;nob<nObjFin;nob++) {
		n=mVBuf[nob];
		mVBuf2[nob]=mClstSz[n];
		mClstSz[n]=nob;
		}


	clear(mHBuf,0x2400);
	 clstmax.npt=0;
	 clstmax.suml=clstmax.maxl=clstmax.lt =clstmax.rb =0;

if(alginf.command & (0x1 <<6)) {

	if(alginf.command & (0x1 <<4)) {
	 clstmax.npt=0;
	 clstmax.suml=clstmax.maxl=0;
/*
	 unsigned short lt;
	 unsigned short rb;
	 unsigned short maxl;
	 unsigned int   suml;
	 unsigned short snr;
	 unsigned short bgrnd;
	 unsigned short npt;
 */
	 for(n=1;n<=nCluster;n++) {
		nob=mClstSz[n];
		clst.npt=clst.suml=clst.maxl=0;
		clst.lt=0xffff; clst.rb=0; clst.bgrnd=0xffff;
		while(nob<0xffff) {
			i=mobj[nob]>>8;
			j=mobj[nob]&0xff;
			clst.npt++;

			if((clst.lt>>8)>i)
				clst.lt= (clst.lt&0x00ff) | (i<<8);
			if((clst.lt & 0xff)>j)
				clst.lt= (clst.lt&0xff00) | j;

			if((clst.rb>>8)<i)
				clst.rb= (clst.rb&0x00ff) | (i<<8);
			if((clst.rb & 0xff)<j)
				clst.rb= (clst.rb&0xff00) | j;

			if(clst.bgrnd>mobjMx[nob])
				clst.bgrnd=mobjMx[nob];
			if(clst.maxl<inbuff[i*144+j])
				  clst.maxl=inbuff[i*144+j];
			clst.suml+=inbuff[i*144+j];
			mHBuf[i*144+j]=n; // Debug ????
			nob=mVBuf2[nob];
			}
		clst.maxl-=clst.bgrnd;
		clst.suml-=clst.bgrnd*clst.npt;
		if(clst.suml>clstmax.suml) {
//			nMaxClst=n;
			clstmax.npt=clst.npt;
			clstmax.suml=clst.suml;
			clstmax.maxl=clst.maxl;
			clstmax.lt=clst.lt;
			clstmax.rb=clst.rb;
			clstmax.bgrnd=clst.bgrnd;
			}
		}
	 }

	if(alginf.command & (0x1 <<5)) {
		for(nob=0;nob<nObjFin;nob++) {
			i=mobj[nob]>>8;
			j=mobj[nob]&0xff;
			mHBuf[i*144+j]=mVBuf[nob];
//		mClstSz[mVBuf[nob]]++; // may be list ????
			}
	}
 }

/*

 	 }
 	 */


	if(nCluster>1)
		err++;
/*
		nObjMax=nObjFin;
		nObjFin=0;
		for(n=0;n<nObjMax;n++) {

				i=mobj[n]>>8;
				j=mobj[n]&0xff;
		}
*/


	alginf.nobj=nObjFin;
//	alginf.nobj2=nsecCluster-1;
	alginf.nobj2=nCluster;

	alginf.npt=nObjMax;
	alginf.sz=clstmax.npt;
	if(clstmax.npt>0) {
		i=(clstmax.lt>>8)-1; c00=(clstmax.rb>>8)+1;
		dlb=dlim=dljm=0; 	 c01=(clstmax.rb&0xff)+1;
		for(;i<=c00;i++) {
			j=(clstmax.lt&0xff)-1;
			for(;j<=c01;j++) {
			if(inbuff[i*144+j]>clstmax.bgrnd)
				c02=inbuff[i*144+j]-clstmax.bgrnd;
				dlim+=c02*i;
				dljm+=c02*j;
				dlb+=c02;
				}
			}
		if(dlb>0) {
			alginf.x =round(((double)dljm /(double)dlb)*10.0);
			alginf.y =round(((double)dlim /(double)dlb)*10.0);
			}
		else {
			alginf.x=((clstmax.lt&0xff) + (clstmax.rb&0xff))/2;
			alginf.y=((clstmax.lt>>8) + (clstmax.rb>>8))/2;
			}
		alginf.x2=((clstmax.lt&0xff) + (clstmax.rb&0xff))/2;
		alginf.y2=((clstmax.lt>>8) + (clstmax.rb>>8))/2;
		alginf.bgrnd=clstmax.bgrnd*10;

//		alginf.level=clstmax.maxl - clstmax.bgrnd;
		alginf.level=clstmax.maxl;

		alginf.size=(((clstmax.rb>>8) - (clstmax.lt>>8)+1)<<8) | ((clstmax.rb&0xff)-(clstmax.lt&0xff));
		alginf.rms=locCombSMR144((alginf.y+5)/10,(alginf.x+5)/10,inbuff,100.0); //Must Big area !!!!!!!!!!!!!!!!!!
		alginf.snr=round(1000.0*(double)alginf.level/(double)alginf.rms);
		}
	else {
//		alginf.x=alginf.y=alginf.x2=alginf.y2=alginf.bgrnd=alginf.rms=0;
		}


	alginf.tm=round(ndgtm/10000.0);
	dk=((double)alginf.ksmr)/10.0;
	dk2=((double)alginf.kbgrnd)/10.0;
	if(alginf.x>maxcord)
		gcnterr++;
//	return nObjFin;
	return nCluster;
}
