#ifndef LABVIEW_HELPERS_H
#define LABVIEW_HELPERS_H

#include <stdlib.h>
#include <string>

#include "extcode.h"


MgErr populate_lstrhandle(LStrHandle lvhandle, std::string str)
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

#endif // LABVIEW_HELPERS_H