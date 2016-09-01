#include "Wrapper.h"

DLLIMPORT SISProtocol * init()
{
	SISProtocol * protocol = new SISProtocol();
	protocol->foo = 42;
	return protocol;
}

DLLIMPORT int32_t open(SISProtocol* ID_ref, const char* ID_comport, uint32_t ID_combaudrate, LStrHandle ID_errmsg)
{
	if (!dynamic_cast<SISProtocol*>(ID_ref)) 
		// Return error for wrong reference
		return get_error_code(
			ID_errmsg, sformat("Reference pointing to invalid location '%p'.", ID_ref).c_str(),
			LVErr_Invalid_Pointer);

	else
	{
		try
		{
			char portName[5] = { "COM1" };
			ID_ref->open(portName);

			return LVErr_NoError;
		}
		catch (SISProtocol::ExceptionGeneric &ex)
		{
			return get_error_code(ID_errmsg, char2str(ex.what()), LVErr_Block_OpenByCOM);
		}
		catch (CSerial::ExceptionGeneric &ex)
		{
			return get_error_code(ID_errmsg, char2str(ex.what()), LVErr_Block_OpenByCOM);
		}
	}
}

DLLIMPORT int32_t close(SISProtocol* ID_ref, LStrHandle ID_errmsg)
{
	if (!dynamic_cast<SISProtocol*>(ID_ref))
		// Return error for wrong reference
		return get_error_code(
			ID_errmsg, sformat("Reference pointing to invalid location '%p'.", ID_ref).c_str(),
			LVErr_Invalid_Pointer);

	try
	{
		ID_ref->close();

		delete ID_ref;
		ID_ref = NULL;
		return LVErr_NoError;
	}
	catch (SISProtocol::ExceptionGeneric &ex)
	{
		return get_error_code(ID_errmsg, char2str(ex.what()), LVErr_Block_Close);
	}
}

DLLIMPORT int32_t sequencer_init(SISProtocol * ID_ref, uint32_t ID_max_accel, int32_t ID_max_jerk, LStrHandle ID_errmsg)
{
	if (!dynamic_cast<SISProtocol*>(ID_ref))
		// Return error for wrong reference
		return get_error_code(
			ID_errmsg, sformat("Reference pointing to invalid location '%p'.", ID_ref).c_str(),
			LVErr_Invalid_Pointer);

	else
	{
		try
		{
			/// Max Acceleration (S-0-0138)
			UINT64 max_accel = ID_max_accel;
			ID_ref->write_parameter(TGM::SERCOS_Param_S, 138, max_accel);

			return LVErr_NoError;
		}
		catch (SISProtocol::ExceptionGeneric &ex)
		{
			return get_error_code(ID_errmsg, char2str(ex.what()), LVErr_Block_OpenByCOM);
		}
	}
}
