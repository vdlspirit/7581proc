#ifndef	SRIO_H_
#define	SRIO_H_

typedef struct SRIO
{
	Uint32 VideoRamK1;						//������ ��� �������� ����������� �� SRIO
	union SrioUnion1{
			Uint32 sw;
			struct {
					Uint32 ShiftInRow 	:8;	//�������� �� ������ ������ (1 -4 �������)
					Uint32 SizeRow		:8; //������ ����������� ������ N+1 (1-4 �������)
					Uint32 SumX2		:1; //������������ �� 2
					Uint32 KamIm		:1;	//����� ������ ��� ��������� (1 - ������)
					Uint32 reserv		:14;
				} s;
			} SrioDataKamera1;

	Uint32 VideoRamK2;						//������ ��� �������� ����������� �� SRIO

	union SrioUnion2{
			Uint32 sw;
			struct {
					Uint32 ShiftInRow 	:8;	//�������� �� ������ ������ (1 -4 �������)
					Uint32 SizeRow		:8; //������ ����������� ������ N+1 (1-4 �������)
					Uint32 SumX2		:1; //������������ �� 2
					Uint32 KamIm		:1;	//����� ������ ��� ��������� (1 - ������)
					Uint32 reserv		:14;
				} s;
			} SrioDataKamera2;

} SRIOPACKET;

#endif
