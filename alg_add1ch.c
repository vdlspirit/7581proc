#define SZCDRX144 144
#define SZCDRY144 144
#define SZCadr144 144
typedef unsigned char UInt8;
typedef unsigned short UInt16;
///-----------------------------------------------------------------------------------------------------

void hr3mn144(UInt16 (* InpArr)[SZCadr144],UInt16 (* outArr)[SZCadr144]) {
  unsigned int i,j,imax,imax2;
  UInt16 mpair1,mpair2;
  for (j=0; j<SZCDRY144; j++) {
	  mpair1=InpArr[j][1];
	  if(mpair1>InpArr[j][2])
			mpair1=InpArr[j][2];
	  for (i=1; i<SZCDRX144-4; i+=4)   {
		  imax=InpArr[j][i-1];
		  if(imax>mpair1)
				imax=mpair1;
		  mpair2=imax2=InpArr[j][i+2];
		  if(imax2>mpair1)
				imax2=mpair1;
		  if(mpair2>InpArr[j][i+3])
				mpair2=InpArr[j][i+3];
		  outArr[j][i]=imax;
		  outArr[j][i+1]=imax2;

		  imax=InpArr[j][i+1];
		  if(imax>mpair2)
				imax=mpair2;
		  mpair1=imax2=InpArr[j][i+4];
		  if(imax2>mpair2)
				imax2=mpair2;
		  if(mpair1>InpArr[j][i+5])
				mpair1=InpArr[j][i+5];
		  outArr[j][i+2]=imax;
		  outArr[j][i+3]=imax2;
		  }
	  imax=InpArr[j][i-1];
	  if(imax>mpair1)
			imax=mpair1;
	  imax2=InpArr[j][i+2];
	  if(imax2>mpair1)
			imax2=mpair1;
	  outArr[j][i]=imax;
	  outArr[j][i+1]=outArr[j][i+1]=imax2;

	  }
}

void vr3mn144(UInt16 (* InpArr)[SZCadr144],UInt16 (* outArr)[SZCadr144]) {
  unsigned int i,j,imax,imax2;
  UInt16 mpair1,mpair2;
  for (j=0; j<SZCDRY144; j++) {
	  mpair1=InpArr[1][j];
	  if(mpair1>InpArr[2][j])
			mpair1=InpArr[2][j];
	  for (i=1; i<SZCDRX144-4; i+=4)   {
		  imax=InpArr[i-1][j];
		  if(imax>mpair1)
				imax=mpair1;
		  mpair2=imax2=InpArr[i+2][j];
		  if(imax2>mpair1)
				imax2=mpair1;
		  if(mpair2>InpArr[i+3][j])
				mpair2=InpArr[i+3][j];
		  outArr[i][j]=imax;
		  outArr[i+1][j]=imax2;

		  imax=InpArr[i+1][j];
		  if(imax>mpair2)
				imax=mpair2;
		  mpair1=imax2=InpArr[i+4][j];
		  if(imax2>mpair2)
				imax2=mpair2;
		  if(mpair1>InpArr[i+5][j])
				mpair1=InpArr[i+5][j];
		  outArr[i+2][j]=imax;
		  outArr[i+3][j]=imax2;
		  }
	  imax=InpArr[i-1][j];
	  if(imax>mpair1)
			imax=mpair1;
	  imax2=InpArr[i+2][j];
	  if(imax2>mpair1)
			imax2=mpair1;
	  outArr[i][j]=imax;
	  outArr[i+1][j]=outArr[i+1][j]=imax2;
	  }
}


void hr5mx144(UInt16 (* InpArr)[SZCadr144],UInt16 (* wrkArr)[SZCadr144], UInt16 (* outArr)[SZCadr144])
{
//    unsigned int i,j,tmp=0,imax,imax2;
    unsigned int i,j,imax,imax2;
	UInt16 mpair1,mpair2,mpair3;
  //  REG_WRITE(TST_STROBE,1);	// test time

  for (j=0; j<SZCDRY144; j++) {
	  mpair1=InpArr[j][1];
	  mpair2=InpArr[j][3];
	  if(mpair1<InpArr[j][2])
			mpair1=InpArr[j][2];
	  if(mpair2<InpArr[j][4])
			mpair2=InpArr[j][4];
	  for (i=2; i<SZCDRX144-7; i+=6)   {
		  imax=InpArr[j][i-2];
		  if(imax<mpair1)
				imax=mpair1;
		  mpair3=imax2=InpArr[j][i+3];
		  if(imax2<mpair2)
				imax2=mpair2;
		  if(mpair3<InpArr[j][i+4])
				mpair3=InpArr[j][i+4];
		  if(imax<mpair2)
				imax=mpair2;
		  if(imax2<mpair1)
				imax2=mpair1;
		  outArr[j][i]=imax;
		  outArr[j][i+1]=imax2;

		  imax=InpArr[j][i];
		  if(imax<mpair2)
				imax=mpair2;
		  mpair1=imax2=InpArr[j][i+5];
		  if(imax2<mpair3)
				imax2=mpair3;
		  if(mpair1<InpArr[j][i+6])
				mpair1=InpArr[j][i+6];
		  if(imax<mpair3)
				imax=mpair3;
		  if(imax2<mpair2)
				imax2=mpair2;
		  outArr[j][i+2]=imax;
		  outArr[j][i+3]=imax2;

		  imax=InpArr[j][i+2];
		  if(imax<mpair3)
				imax=mpair3;
		  mpair2=imax2=InpArr[j][i+7];
		  if(imax2<mpair1)
				imax2=mpair1;
		  if(mpair2<InpArr[j][i+8])
				mpair2=InpArr[j][i+8];
		  if(imax<mpair1)
				imax=mpair1;
		  if(imax2<mpair3)
				imax2=mpair3;
		  outArr[j][i+4]=imax;
		  outArr[j][i+5]=imax2;
//		  mpair2=mpair3;
		  }
	  }
}


void vr5mx144(UInt16 (* InpArr)[SZCadr144],UInt16 (* wrkArr)[SZCadr144], UInt16 (* outArr)[SZCadr144])
{
//  unsigned int i,j,tmp=0,imax,imax2;
  unsigned int i,j,imax,imax2;
	UInt16 mpair1,mpair2,mpair3;
  //  REG_WRITE(TST_STROBE,1);	// test time

  for (j=0; j<SZCDRY144; j++) {
	  mpair1=InpArr[1][j];
	  mpair2=InpArr[3][j];
	  if(mpair1<InpArr[2][j])
			mpair1=InpArr[2][j];
	  if(mpair2<InpArr[4][j])
			mpair2=InpArr[4][j];
	  for (i=2; i<SZCDRX144-7; i+=6)   {
		  imax=InpArr[i-2][j];
		  if(imax<mpair1)
				imax=mpair1;
		  mpair3=imax2=InpArr[i+3][j];
		  if(imax2<mpair2)
				imax2=mpair2;
		  if(mpair3<InpArr[i+4][j])
				mpair3=InpArr[i+4][j];
		  if(imax<mpair2)
				imax=mpair2;
		  if(imax2<mpair1)
				imax2=mpair1;
		  outArr[i][j]=imax;
		  outArr[i+1][j]=imax2;

		  imax=InpArr[i][j];
		  if(imax<mpair2)
				imax=mpair2;
		  mpair1=imax2=InpArr[i+5][j];
		  if(imax2<mpair3)
				imax2=mpair3;
		  if(mpair1<InpArr[i+6][j])
				mpair1=InpArr[i+6][j];
		  if(imax<mpair3)
				imax=mpair3;
		  if(imax2<mpair2)
				imax2=mpair2;
		  outArr[i+2][j]=imax;
		  outArr[i+3][j]=imax2;

		  imax=InpArr[i+2][j];
		  if(imax<mpair3)
				imax=mpair3;
		  mpair2=imax2=InpArr[i+7][j];
		  if(imax2<mpair1)
				imax2=mpair1;
		  if(mpair2<InpArr[i+8][j])
				mpair2=InpArr[i+8][j];
		  if(imax<mpair1)
				imax=mpair1;
		  if(imax2<mpair3)
				imax2=mpair3;
		  outArr[i+4][j]=imax;
		  outArr[i+5][j]=imax2;
//		  mpair2=mpair3;
		  }
	  }
}

void hr5mx3mn144(UInt16 (* InpArr)[SZCadr144],UInt16 (* wrkArr)[SZCadr144], UInt16 (* outArr)[SZCadr144]) {
	hr5mx144(InpArr,outArr,wrkArr);
//	hor3min64(wrkArr,outArr);
	vr3mn144(wrkArr,outArr);
}

void vr5mx3mn144(UInt16 (* InpArr)[SZCadr144],UInt16 (* wrkArr)[SZCadr144], UInt16 (* outArr)[SZCadr144]) {
	vr5mx144(InpArr,outArr,wrkArr);
//	ver3min64(wrkArr,outArr);
	hr3mn144(wrkArr,outArr);
}

