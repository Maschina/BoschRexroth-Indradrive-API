/// @file
/// Implementation of Error handle that is used within each API call function

#ifndef LABVIEW_ERRORS_H
#define LABVIEW_ERRORS_H

#include <cstring>
#include <string>


#ifdef USE_LABVIEW_ENV
// Including Labviewv.lib in case of project is compiled for LabVIEW
#pragma comment(lib,"labviewv.lib")
// Including extcode.h in case of project is compiled for LabVIEW
#include "extcode.h"
#endif

/// Generic error handle that is returned from each API function.
/// 
/// In contrast to a LabVIEW error handle (LVErrorCluster_t) that includes a specific type of Error string, the
/// Generic Error Handle (GenericErrHandle) consists of generic C types (int and char*) for both error code and
/// error message.
/// 
/// GenericErrHandle is used as pointer for all Indradrive API Functions (see @ref ss_overview "API Function Overview").
///
/// @remarks	Depending on the USE_LABVIEW_ENV switch, the GenericErrHandle can be replaced by LStrHandle.
typedef struct GenericErrHandle {

	/// Error code.
	uint32_t	code;
	/// Error message [2048].
	char		msg[2048];

	/// Constructor.
	///
	/// @param	_code	(Optional) Error code. Can be later on set also via set() function.
	/// @param	_msg 	(Optional) Error message. Parameter will not be used.
	GenericErrHandle(uint32_t _code = 0, const char* _msg = "") :
		code(_code)
	{}

	/// Sets error code and error message.
	///
	/// @param	_code	Error code.
	/// @param	_msg 	Error message.
	void set(uint32_t _code, const char* _msg)
	{
		code = _code;

		for (size_t i = 0; i < strlen(_msg); i++)
			msg[i] = _msg[i];
	}

	/// Sets an error message.
	///
	/// @param	_msg	Error message.
	void set_msg(const char* _msg)
	{
		set(code, _msg);
	}

	/// Sets an error code.
	///
	/// @param	_code	Error code.
	void set_code(uint32_t _code)
	{
		set(_code, msg);
	}
} GenericErrHandle;


#ifdef USE_LABVIEW_ENV
/// Defines an alias representing handle of the error via LStrHandle for LabVIEW.
/// 
/// @remarks The alias is used since the USE_LABVIEW_ENV switch can the LStrHandle by GenericErrHandle.
typedef LStrHandle ErrHandle;
#else
/// Defines an alias representing a pointer to GenericErrHandle.
///
/// @remarks The alias is used since the USE_LABVIEW_ENV switch can the GenericErrHandle by LStrHandle.
typedef GenericErrHandle* ErrHandle;
#endif


#ifdef USE_LABVIEW_ENV
#pragma pack(push,1)
#include "extcode.h"
#pragma pack(pop)
#endif

#ifdef USE_LABVIEW_ENV
#pragma pack(push,1)
/// LabVIEW error cluster that is used for LabVIEW environment.
/// @deprecated This cluster is not used anymore since error cluster compilation is done directly in LabVIEW VI.
typedef struct {
	LVBoolean	status;
	int32		code;
	LStrHandle  msg;
} LVErrorCluster_t;
#pragma pack(pop)
#endif


#ifdef USE_LABVIEW_ENV
/// A macro that defines Error base that is used for coding the final error code in LabVIEW environment.
///
/// @sa	set_error()
#define Err_Base (0x08EF)
#else

/// A macro that defines Error base that is used for coding the final error code.
///
/// @sa	set_error()
#define Err_Base (0x0)
#endif

/// Values that represent error blocks to be used as block_code paramater for set_error() function.
///
/// @sa	set_error()
typedef enum EErrorBlocks
{
	/// An enum constant representing the Error: no error
	Err_NoError				= 0,
	/// An enum constant representing the Error on open by com 
	Err_Block_OpenByCOM		= 1,
	/// An enum constant representing the Error on close 
	Err_Block_Close			= 2,
	/// An enum constant representing the Error on test  
	Err_Block_Test			= 3,				 
	/// An enum constant representing the Error on Sequence init  
	Err_Block_SeqInit		= 6,				 
	/// An enum constant representing the Error on Sequence write  
	Err_Block_SeqWrite		= 7,				 
	/// An enum constant representing the Error on Speed Contrl init  
	Err_Block_VelCInit		= 8,				 
	/// An enum constant representing the Error on Speed Control write  
	Err_Block_VelCWrite		= 9,				 
	/// An enum constant representing the Error on get status  
	Err_Block_GetStatus		= 10,				 
	/// An enum constant representing the Error on set control  
	Err_Block_SetControl	= 11,
	/// An enum constant representing the Error of invalid API reference
	Err_Invalid_Pointer		= 12
} EErrorBlocks;

#ifdef USE_LABVIEW_ENV
/// Writes a error string to a LabVIEW error handle.
///
/// @param [out]	lvhandle	LabVIEW error handle.
/// @param [in]		str			Error message.
///
/// @return	Return code on successful execution.
static MgErr write_string(ErrHandle lvhandle, std::string str)
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

/// Sets an error handle to the errhndl parameter.
/// 
/// This static function can be utilized to set an error message as well as a error code in the following scheme to
/// an Error handle: Error code: 0 &lt;&lt; 8 | block_code &lt;&lt; 4 | issue_code, whereas "|" indicates an OR-
/// concatenation.
///
/// @param [out]	errhndl   	Error handle pointer.
/// @param [in]		errstr	  	Error message.
/// @param [in]		block_code	Error block code defined by EErrorBlocks enum.
/// @param [in]		issue_code	(Optional) The issue code.
///
/// @return	The final error code.
///
/// @sa	EErrorBlocks
inline static int32_t set_error(ErrHandle errhndl, std::string errstr, int32_t block_code, int32_t issue_code = 1)
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

