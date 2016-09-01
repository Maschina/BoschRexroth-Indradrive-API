#ifndef LABVIEW_TYPES_H
#define LABVIEW_TYPES_H

#ifdef USE_QT_ENV
#include "qglobal.h"
#endif

#pragma pack(push,1)
#include "extcode.h"
#pragma pack(pop)



typedef int LVErrCode;

typedef unsigned int FTDIHandle;


#pragma pack(push,1)
typedef struct {
	LVBoolean	status;
    int32		code;
    LStrHandle  msg;
} LVErrorCluster_t;
#pragma pack(pop)

#pragma pack(push,1)
typedef struct {
	uInt32			delay;
	uInt8			control;
	uInt8			size;
	uInt8			id;
	uInt8			data[9];
    LVBoolean		timeout;
    LVBoolean		datalost;
    LVBoolean		wrongformat;
    LVBoolean		recvnoise;
} LVLINFrame_t;
#pragma pack(pop)



#endif // LABVIEW_TYPES_H

