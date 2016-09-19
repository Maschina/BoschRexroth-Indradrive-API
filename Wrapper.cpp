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
			ID_ref->open(ID_comport);
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


DLLIMPORT int32_t sequencer_init(SISProtocol * ID_ref, uint32_t ID_max_accel, uint32_t ID_max_jerk, LStrHandle ID_errmsg)
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
			ID_ref->write_parameter(TGM::SERCOS_Param_S, 138, ID_max_accel);

			/// Max Jerk (S-0-0349)
			ID_ref->write_parameter(TGM::SERCOS_Param_S, 349, ID_max_jerk);

			/// SPS Global Register G1 (P-0-1371) - Reset Read Trigger
			UINT64 g1 = 0;
			ID_ref->write_parameter(TGM::SERCOS_Param_P, 1371, g1);

			/// SPS Global Register G2 (P-0-1372) - Reset Sequencer Trigger
			UINT64 g2 = 0;
			ID_ref->write_parameter(TGM::SERCOS_Param_P, 1372, g2);

			return LVErr_NoError;
		}
		catch (SISProtocol::ExceptionGeneric &ex)
		{
			return get_error_code(ID_errmsg, char2str(ex.what()), LVErr_Block_OpenByCOM);
		}
	}
}

DLLIMPORT int32_t sequencer_write(SISProtocol * ID_ref, int32_t ID_speeds[], double_t ID_accels[], double_t ID_jerks[], uint32_t ID_delays[], const uint16_t ID_set_length, LStrHandle ID_errmsg)
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
			/// Speed Array (P-0-4007)
			for (uint16_t i = 1; i <= ID_set_length; i++)
				ID_ref->write_listelm(TGM::SERCOS_Param_P, 4007, i, (UINT32&)ID_speeds[i]);
		}
		catch (SISProtocol::ExceptionGeneric &ex)
		{
			return get_error_code(ID_errmsg, char2str(ex.what()), LVErr_Block_OpenByCOM);
		}
	}
	
}
