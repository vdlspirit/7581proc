#ifndef	SRIO_H_
#define	SRIO_H_

typedef struct SRIO
{
	Uint32 VideoRamK1;						//память для загрузки видеопотока из SRIO
	union SrioUnion1{
			Uint32 sw;
			struct {
					Uint32 ShiftInRow 	:8;	//смещение от начала строки (1 -4 пикселя)
					Uint32 SizeRow		:8; //размер загружаемой строки N+1 (1-4 пикселя)
					Uint32 SumX2		:1; //Суммирование на 2
					Uint32 KamIm		:1;	//Выбор камеры или имитатора (1 - камера)
					Uint32 reserv		:14;
				} s;
			} SrioDataKamera1;

	Uint32 VideoRamK2;						//память для загрузки видеопотока из SRIO

	union SrioUnion2{
			Uint32 sw;
			struct {
					Uint32 ShiftInRow 	:8;	//смещение от начала строки (1 -4 пикселя)
					Uint32 SizeRow		:8; //размер загружаемой строки N+1 (1-4 пикселя)
					Uint32 SumX2		:1; //Суммирование на 2
					Uint32 KamIm		:1;	//Выбор камеры или имитатора (1 - камера)
					Uint32 reserv		:14;
				} s;
			} SrioDataKamera2;

} SRIOPACKET;

#endif
