#ifndef LABVIEW_ERRORS_H
#define LABVIEW_ERRORS_H

#include <cstring>
#include "labview_types.h"
#include "labview_helpers.h"


#define LVErr_Base (0x08EF)

#define LVErr_NoError 0

/** Error messages **/
#define LVErr_Block_OpenByCOM		1
#define LVErr_Block_Close			2
#define LVErr_Block_Test			3
#define LVErr_Block_SeqInit			6
#define LVErr_Block_SeqWrite		7
#define LVErr_Block_VelCInit		8
#define LVErr_Block_VelCWrite		9
#define LVErr_Invalid_Pointer		12


inline LVErrCode get_error_code(LStrHandle LC_errmsg, std::string errstr, LVErrCode block_code, LVErrCode issue_code = 1)
{
	populate_lstrhandle(LC_errmsg, errstr);

	return (LVErr_Base << 8) | (block_code << 4) | issue_code;
}

#endif // LABVIEW_ERRORS_H

