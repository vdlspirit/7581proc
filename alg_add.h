//typedef unsigned int UInt32;
//typedef unsigned short UInt16;

#ifndef ALGADD_H_
#define ALGADD_H_

#define SZCDRX 192
#define SZCDRY 192
#define SZCadr196 192

#define MIN(a,b)            ((a)<(b)?(a):(b))
#define MAX(a,b)            ((a)>(b)?(a):(b))
#define MINMN(a,b,c)            ((a)<(b)?(a):(b))
#define MAXTHR(a,b)            ((a)>(b)?(0):(1))
#define MINTHR(a,b)            ((a)<(b)?(0):(1))

void hr3mn64(UInt16 (* InpArr)[SZCadr196],UInt16 (* outArr)[SZCadr196]) ;
void vr3mn64(UInt16 (* InpArr)[SZCadr196],UInt16 (* outArr)[SZCadr196]) ;
void hr5mx64(UInt16 (* InpArr)[SZCadr196],UInt16 (* wrkArr)[SZCadr196], UInt16 (* outArr)[SZCadr196]);
void vr5mx64(UInt16 (* InpArr)[SZCadr196],UInt16 (* wrkArr)[SZCadr196], UInt16 (* outArr)[SZCadr196]);
void hr5mx3mn64(UInt16 (* InpArr)[SZCadr196],UInt16 (* wrkArr)[SZCadr196], UInt16 (* outArr)[SZCadr196]);
void vr5mx3mn64(UInt16 (* InpArr)[SZCadr196],UInt16 (* wrkArr)[SZCadr196], UInt16 (* outArr)[SZCadr196]);

#endif
