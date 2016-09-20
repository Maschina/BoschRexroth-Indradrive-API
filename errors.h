#ifndef LABVIEW_ERRORS_H
#define LABVIEW_ERRORS_H

#include <cstring>
#include <string>


typedef struct GenericErrHandle_t {

	uint32_t	code;
	char		msg[2048];

	GenericErrHandle_t(uint32_t _code = 0, const char* _msg = "") :
		code(_code)
	{}

	void set(uint32_t _code, const char* _msg)
	{
		code = _code;

		for (size_t i = 0; i < strlen(_msg); i++)
			msg[i] = _msg[i];
	}

	void set_msg(const char* _msg)
	{
		set(code, _msg);
	}

	void set_code(uint32_t _code)
	{
		set(_code, msg);
	}
} GenericErrHandle;


#ifdef USE_LABVIEW_ENV
typedef LStrHandle ErrHandle;
#else
typedef GenericErrHandle* ErrHandle;
#endif


#ifdef USE_LABVIEW_ENV
#pragma pack(push,1)
#include "extcode.h"
#pragma pack(pop)
#endif

#ifdef USE_LABVIEW_ENV
#pragma pack(push,1)
typedef struct {
	LVBoolean	status;
	int32		code;
	LStrHandle  msg;
} LVErrorCluster_t;
#pragma pack(pop)
#endif


#ifdef USE_LABVIEW_ENV
#define Err_Base (0x08EF)
#else
#define Err_Base (0x0)
#endif

/// Error messages
#define Err_NoError				0
#define Err_Block_OpenByCOM		1
#define Err_Block_Close			2
#define Err_Block_Test			3
#define Err_Block_SeqInit		6
#define Err_Block_SeqWrite		7
#define Err_Block_VelCInit		8
#define Err_Block_VelCWrite		9
#define Err_Invalid_Pointer		12


#ifdef USE_LABVIEW_ENV
MgErr write_string(ErrHandle lvhandle, std::string str)
{
	//Initializes the buffer
	MgErr err = NumericArrayResize(uB, 1, (UHandle*)&lvhandle, str.length());
	if (err) return err;

	//Informs the LabVIEW string handle about the size of the size
	(**lvhandle).cnt = str.length();

	//Fills the string buffer with str
	strcpy((char*)(**lvhandle).str, str.c_str());

	return noErr;
}
#endif


inline int32_t set_error(ErrHandle errhndl, std::string errstr, int32_t block_code, int32_t issue_code = 1)
{
	int32_t retcode = (Err_Base << 8) | (block_code << 4) | issue_code;

#ifdef USE_LABVIEW_ENV
	write_string(errhndl, errstr);
#else
	errhndl->set(retcode, errstr.c_str());
#endif

	return retcode;
}


#endif // LABVIEW_ERRORS_H

