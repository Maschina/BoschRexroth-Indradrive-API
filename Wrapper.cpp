#include "Wrapper.h"


DLLEXPORT SISProtocol * DLLCALLCONV init()
{
	SISProtocol * protocol = new SISProtocol();
	return protocol;
}


DLLEXPORT int32_t DLLCALLCONV open(SISProtocol* ID_ref, const wchar_t* ID_comport, uint32_t ID_combaudrate, ErrHandle ID_err)
{
	if (!dynamic_cast<SISProtocol*>(ID_ref)) 
		// Return error for wrong reference
		return set_error(
			ID_err, sformat("Reference pointing to invalid location '%p'.", ID_ref),
			Err_Invalid_Pointer);

	try
	{
		ID_ref->open(ID_comport);
		return Err_NoError;
	}
	catch (SISProtocol::ExceptionGeneric &ex)
	{
		return set_error(ID_err, char2str(ex.what()), Err_Block_OpenByCOM);
	}
	catch (CSerial::ExceptionGeneric &ex)
	{
		return set_error(ID_err, char2str(ex.what()), Err_Block_OpenByCOM);
	}
}


DLLEXPORT int32_t DLLCALLCONV close(SISProtocol* ID_ref, ErrHandle ID_err)
{
	if (!dynamic_cast<SISProtocol*>(ID_ref))
		// Return error for wrong reference
		return set_error(
			ID_err, sformat("Reference pointing to invalid location '%p'.", ID_ref),
			Err_Invalid_Pointer);

	try
	{
		ID_ref->close();

		delete ID_ref;
		ID_ref = NULL;
		return Err_NoError;
	}
	catch (SISProtocol::ExceptionGeneric &ex)
	{
		return set_error(ID_err, char2str(ex.what()), Err_Block_Close);
	}
	catch (CSerial::ExceptionGeneric &ex)
	{
		return set_error(ID_err, char2str(ex.what()), Err_Block_Close);
	}
}


DLLEXPORT int32_t DLLCALLCONV sequencer_activate(SISProtocol * ID_ref, ErrHandle ID_err)
{
	if (!dynamic_cast<SISProtocol*>(ID_ref))
		// Return error for wrong reference
		return set_error(
			ID_err, sformat("Reference pointing to invalid location '%p'.", ID_ref),
			Err_Invalid_Pointer);

	try
	{
		// Change mode
		change_opmode(ID_ref, DRIVEMODE_SEQUENCER);
		
		return Err_NoError;
	}
	catch (SISProtocol::ExceptionGeneric &ex)
	{
		return set_error(ID_err, char2str(ex.what()), Err_Block_SeqInit);
	}
	catch (CSerial::ExceptionGeneric &ex)
	{
		return set_error(ID_err, char2str(ex.what()), Err_Block_SeqInit);
	}
}


DLLEXPORT int32_t DLLCALLCONV sequencer_init(SISProtocol * ID_ref, double_t ID_max_accel, double_t ID_max_jerk, ErrHandle ID_err)
{
	if (!dynamic_cast<SISProtocol*>(ID_ref))
		// Return error for wrong reference
		return set_error(
			ID_err, sformat("Reference pointing to invalid location '%p'.", ID_ref),
			Err_Invalid_Pointer);

	try
	{
		// Set required units (preferred scaling, rotary scaling, [rpm])
		change_units(ID_ref);

		// Max Acceleration (S-0-0138)
		ID_ref->write_parameter(TGM::SERCOS_Param_S, 138, ID_max_accel);

		// Max Jerk (S-0-0349)
		ID_ref->write_parameter(TGM::SERCOS_Param_S, 349, ID_max_jerk);

		// SPS Global Register G1 (P-0-1371) - Reset Read Trigger
		ID_ref->write_parameter(TGM::SERCOS_Param_P, 1371, static_cast<uint32_t>(0));

		// SPS Global Register G2 (P-0-1372) - Reset Sequencer Trigger
		ID_ref->write_parameter(TGM::SERCOS_Param_P, 1372, static_cast<uint32_t>(0));

		return Err_NoError;
	}
	catch (SISProtocol::ExceptionGeneric &ex)
	{
		return set_error(ID_err, char2str(ex.what()), Err_Block_SeqInit);
	}
	catch (CSerial::ExceptionGeneric &ex)
	{
		return set_error(ID_err, char2str(ex.what()), Err_Block_SeqInit);
	}
}

DLLEXPORT int32_t DLLCALLCONV sequencer_write(SISProtocol * ID_ref, double_t ID_speeds[], double_t ID_accels[], double_t ID_jerks[], uint32_t ID_delays[], const uint16_t ID_set_length, uint8_t ID_direction, ErrHandle ID_err)
{
	if (!dynamic_cast<SISProtocol*>(ID_ref))
		// Return error for wrong reference
		return set_error(
			ID_err, sformat("Reference pointing to invalid location '%p'.", ID_ref),
			Err_Invalid_Pointer);

	try
	{
			
		for (uint16_t i = 1; i <= ID_set_length; i++)
		{
			// Speed in min^-1 (P-0-4007)
			ID_ref->write_listelm(TGM::SERCOS_Param_P, 4007, i, abs(ID_speeds[i]));

			// Acceleration in rad/s^2 (P-0-4008)
			ID_ref->write_listelm(TGM::SERCOS_Param_P, 4008, i, ID_accels[i]);

			// Deceleration in rad/s^2 (P-0-4063)
			ID_ref->write_listelm(TGM::SERCOS_Param_P, 4063, i, ID_accels[i]);

			// Jerk in rad/s^3 (P-0-4009)
			ID_ref->write_listelm(TGM::SERCOS_Param_P, 4009, i, ID_jerks[i]);

			// Mode (P-0-4019)
			ID_ref->write_listelm(TGM::SERCOS_Param_P, 4019, i, static_cast<uint32_t>(0b10000000 & ((stde::sgn<double_t>(ID_speeds[i]) == 1 ? 0b01 : 0b10) << 2)));

			// Pos (P-0-4006)
			ID_ref->write_listelm(TGM::SERCOS_Param_P, 4006, i, static_cast<uint64_t>(0));

			// Wait (P-0-4018)
			ID_ref->write_listelm(TGM::SERCOS_Param_P, 4018, i, static_cast<uint64_t>(0));

			// Delay (P-0-4063)
			ID_ref->write_listelm(TGM::SERCOS_Param_P, 4063, i, static_cast<uint64_t>(0));

			// Timers in cs (P-0-1389)
			ID_ref->write_listelm(TGM::SERCOS_Param_P, 1389, i, ID_delays[i]);
		}		

		// Time triggers for cam (P-0-1370)
		ID_ref->write_parameter(TGM::SERCOS_Param_P, 1370, static_cast<uint32_t>(ID_set_length));

		return Err_NoError;
	}
	catch (SISProtocol::ExceptionGeneric &ex)
	{
		return set_error(ID_err, char2str(ex.what()), Err_Block_SeqWrite);
	}	
	catch (CSerial::ExceptionGeneric &ex)
	{
		return set_error(ID_err, char2str(ex.what()), Err_Block_SeqWrite);
	}
}

DLLEXPORT int32_t DLLCALLCONV sequencer_softtrigger(SISProtocol * ID_ref, ErrHandle ID_err)
{
	if (!dynamic_cast<SISProtocol*>(ID_ref))
		// Return error for wrong reference
		return set_error(
			ID_err, sformat("Reference pointing to invalid location '%p'.", ID_ref),
			Err_Invalid_Pointer);

	try
	{
		uint32_t qb0stat;

		/// FEED DATA

		// SPS Global Register G1 (P-0-1371) - Reset Read Trigger
		ID_ref->write_parameter(TGM::SERCOS_Param_P, 1371, static_cast<uint64_t>(0));

		// SPS Global Register G1 (P-0-1371) - Set Read Trigger
		ID_ref->write_parameter(TGM::SERCOS_Param_P, 1371, static_cast<uint64_t>(1));
					
		// Check status (P-0-1410)
		ID_ref->read_parameter(TGM::SERCOS_Param_P, 1410, qb0stat); /// TODO: Check RESULT_READ_OK bit (0b100000)

		/// TRIGGER

		// SPS Global Register G2 (P-0-1372) - Reset Sequencer Trigger
		ID_ref->write_parameter(TGM::SERCOS_Param_P, 1372, static_cast<uint64_t>(0));

		// SPS Global Register G2 (P-0-1372) - Set Sequencer Trigger
		ID_ref->write_parameter(TGM::SERCOS_Param_P, 1372, static_cast<uint64_t>(1));

		// Check status (P-0-1410)
		ID_ref->read_parameter(TGM::SERCOS_Param_P, 1410, qb0stat); /// TODO: Check Drive started bit (0b1000)

		return Err_NoError;
	}
	catch (SISProtocol::ExceptionGeneric &ex)
	{
		return set_error(ID_err, char2str(ex.what()), Err_Block_SeqWrite);
	}
	catch (CSerial::ExceptionGeneric &ex)
	{
		return set_error(ID_err, char2str(ex.what()), Err_Block_SeqWrite);
	}
}

DLLEXPORT int32_t DLLCALLCONV speedcontrol_activate(SISProtocol * ID_ref, ErrHandle ID_err)
{
	if (!dynamic_cast<SISProtocol*>(ID_ref))
		// Return error for wrong reference
		return set_error(
			ID_err, sformat("Reference pointing to invalid location '%p'.", ID_ref),
			Err_Invalid_Pointer);

	try
	{
		// Change mode
		change_opmode(ID_ref, DRIVEMODE_SPEEDCONTROL);

		return Err_NoError;
	}
	catch (SISProtocol::ExceptionGeneric &ex)
	{
		return set_error(ID_err, char2str(ex.what()), Err_Block_VelCInit);
	}
	catch (CSerial::ExceptionGeneric &ex)
	{
		return set_error(ID_err, char2str(ex.what()), Err_Block_VelCInit);
	}
}

DLLEXPORT int32_t DLLCALLCONV speedcontrol_init(SISProtocol * ID_ref, double_t ID_max_accel, double_t ID_max_jerk, ErrHandle ID_err)
{
	if (!dynamic_cast<SISProtocol*>(ID_ref))
		// Return error for wrong reference
		return set_error(
			ID_err, sformat("Reference pointing to invalid location '%p'.", ID_ref),
			Err_Invalid_Pointer);

	try
	{
		// Set required units (preferred scaling, rotary scaling, [rpm])
		change_units(ID_ref);

		// Max Acceleration (S-0-0138)
		ID_ref->write_parameter(TGM::SERCOS_Param_S, 138, ID_max_accel);

		// Max Jerk (S-0-0349)
		ID_ref->write_parameter(TGM::SERCOS_Param_S, 349, ID_max_jerk);

		return Err_NoError;
	}
	catch (SISProtocol::ExceptionGeneric &ex)
	{
		return set_error(ID_err, char2str(ex.what()), Err_Block_VelCInit);
	}
	catch (CSerial::ExceptionGeneric &ex)
	{
		return set_error(ID_err, char2str(ex.what()), Err_Block_VelCInit);
	}
}

DLLEXPORT int32_t DLLCALLCONV speedcontrol_write(SISProtocol * ID_ref, double_t ID_speed, double_t ID_accel, ErrHandle ID_err)
{
	if (!dynamic_cast<SISProtocol*>(ID_ref))
		// Return error for wrong reference
		return set_error(
			ID_err, sformat("Reference pointing to invalid location '%p'.", ID_ref),
			Err_Invalid_Pointer);

	try
	{
		// Rotation direction
		uint32_t rotmode = static_cast<uint32_t>((stde::sgn<double_t>(ID_speed) == 1 ? 1 : 0) << 10);
		// Control Mode (P-0-1200)
		ID_ref->write_parameter(TGM::SERCOS_Param_P, 1200, rotmode);

		// Acceleration in rad/s^2 (P-0-1203)
		ID_ref->write_parameter(TGM::SERCOS_Param_P, 1203, ID_accel);

		// Speed in rpm (S-0-0036)
		ID_ref->write_parameter(TGM::SERCOS_Param_S, 36, abs(ID_speed));

		return Err_NoError;
	}
	catch (SISProtocol::ExceptionGeneric &ex)
	{
		return set_error(ID_err, char2str(ex.what()), Err_Block_VelCWrite);
	}
	catch (CSerial::ExceptionGeneric &ex)
	{
		return set_error(ID_err, char2str(ex.what()), Err_Block_VelCWrite);
	}
}

DLLEXPORT int32_t DLLCALLCONV set_stdenvironment(SISProtocol * ID_ref, ErrHandle ID_err)
{
	if (!dynamic_cast<SISProtocol*>(ID_ref))
		// Return error for wrong reference
		return set_error(
			ID_err, sformat("Reference pointing to invalid location '%p'.", ID_ref),
			Err_Invalid_Pointer);

	try
	{
		change_units(ID_ref);
		change_language(ID_ref);

		return Err_NoError;
	}
	catch (SISProtocol::ExceptionGeneric &ex)
	{
		return set_error(ID_err, char2str(ex.what()), Err_Block_GetStatus);
	}
	catch (CSerial::ExceptionGeneric &ex)
	{
		return set_error(ID_err, char2str(ex.what()), Err_Block_GetStatus);
	}
}

DLLEXPORT int32_t DLLCALLCONV get_drivemode(SISProtocol * ID_ref, uint32_t * ID_drvmode, ErrHandle ID_err)
{
	if (!dynamic_cast<SISProtocol*>(ID_ref))
		// Return error for wrong reference
		return set_error(
			ID_err, sformat("Reference pointing to invalid location '%p'.", ID_ref),
			Err_Invalid_Pointer);

	try
	{
		uint64_t curdrvmode;
		// Primary Operation Mode (S-0-0032)
		ID_ref->read_parameter(TGM::SERCOS_Param_S, 32, curdrvmode);

		switch (curdrvmode)
		{
		case DRIVEMODE_SEQUENCER: // Drive Mode: Sequencer
			*ID_drvmode = 1;
			break;
		case DRIVEMODE_SPEEDCONTROL: // Drive Mode: Speed Control
			*ID_drvmode = 2;
			break;
		default: // Drive Mode not supported
			*ID_drvmode = 0;
			break;
		}

		return Err_NoError;
	}
	catch (SISProtocol::ExceptionGeneric &ex)
	{
		return set_error(ID_err, char2str(ex.what()), Err_Block_GetStatus);
	}
	catch (CSerial::ExceptionGeneric &ex)
	{
		return set_error(ID_err, char2str(ex.what()), Err_Block_GetStatus);
	}
}

DLLEXPORT int32_t DLLCALLCONV get_opstate(SISProtocol * ID_ref, uint8_t * ID_opstate, ErrHandle ID_err)
{
	if (!dynamic_cast<SISProtocol*>(ID_ref))
		// Return error for wrong reference
		return set_error(
			ID_err, sformat("Reference pointing to invalid location '%p'.", ID_ref),
			Err_Invalid_Pointer);

	try
	{
		uint64_t curopstate;
		// Device control: Status word (P-0-0115)
		ID_ref->read_parameter(TGM::SERCOS_Param_P, 115, curopstate);

		OPSTATE opstate(static_cast<uint16_t>(curopstate));
		*ID_opstate = opstate.value;

		return Err_NoError;
	}
	catch (SISProtocol::ExceptionGeneric &ex)
	{
		return set_error(ID_err, char2str(ex.what()), Err_Block_GetStatus);
	}
	catch (CSerial::ExceptionGeneric &ex)
	{
		return set_error(ID_err, char2str(ex.what()), Err_Block_GetStatus);
	}
}

DLLEXPORT int32_t DLLCALLCONV get_speed(SISProtocol * ID_ref, double_t * ID_speed, ErrHandle ID_err)
{
	if (!dynamic_cast<SISProtocol*>(ID_ref))
		// Return error for wrong reference
		return set_error(
			ID_err, sformat("Reference pointing to invalid location '%p'.", ID_ref),
			Err_Invalid_Pointer);

	try
	{
		double_t speed;
		// Velocity feedback value (S-0-0040)
		ID_ref->read_parameter(TGM::SERCOS_Param_S, 40, speed);

		*ID_speed = speed;

		return Err_NoError;
	}
	catch (SISProtocol::ExceptionGeneric &ex)
	{
		return set_error(ID_err, char2str(ex.what()), Err_Block_GetStatus);
	}
	catch (CSerial::ExceptionGeneric &ex)
	{
		return set_error(ID_err, char2str(ex.what()), Err_Block_GetStatus);
	}
}

DLLEXPORT int32_t DLLCALLCONV get_diagnostic_msg(SISProtocol * ID_ref, char * ID_diagnostic_msg, ErrHandle ID_err)
{
	if (!dynamic_cast<SISProtocol*>(ID_ref))
		// Return error for wrong reference
		return set_error(
			ID_err, sformat("Reference pointing to invalid location '%p'.", ID_ref),
			Err_Invalid_Pointer);

	try
	{
		char msg[TGM_SIZEMAX_PAYLOAD];
		// Diagnostic message (S-0-0095)
		ID_ref->read_parameter(TGM::SERCOS_Param_S, 95, msg);

		strncpy(ID_diagnostic_msg, msg+4, TGM_SIZEMAX_PAYLOAD-4);

		return Err_NoError;
	}
	catch (SISProtocol::ExceptionGeneric &ex)
	{
		return set_error(ID_err, char2str(ex.what()), Err_Block_GetStatus);
	}
	catch (CSerial::ExceptionGeneric &ex)
	{
		return set_error(ID_err, char2str(ex.what()), Err_Block_GetStatus);
	}
}

DLLEXPORT int32_t DLLCALLCONV get_diagnostic_num(SISProtocol * ID_ref, uint32_t * ID_diagnostic_num, ErrHandle ID_err)
{
	if (!dynamic_cast<SISProtocol*>(ID_ref))
		// Return error for wrong reference
		return set_error(
			ID_err, sformat("Reference pointing to invalid location '%p'.", ID_ref),
			Err_Invalid_Pointer);

	try
	{
		UINT32 num;
		// Diagnostic number (S-0-0390)
		ID_ref->read_parameter(TGM::SERCOS_Param_S, 390, num);

		*ID_diagnostic_num = num;

		return Err_NoError;
	}
	catch (SISProtocol::ExceptionGeneric &ex)
	{
		return set_error(ID_err, char2str(ex.what()), Err_Block_GetStatus);
	}
	catch (CSerial::ExceptionGeneric &ex)
	{
		return set_error(ID_err, char2str(ex.what()), Err_Block_GetStatus);
	}
}


void change_opmode(SISProtocol * ID_ref, const uint64_t opmode)
{
	uint64_t curopmode;
	// Primary Operation Mode (S-0-0032)
	ID_ref->read_parameter(TGM::SERCOS_Param_S, 32, curopmode);

	// Operation change will trigger flash operations that may cause limited life time
	// Thus, operation change should be mainly triggered if required only
	if (curopmode != opmode)
	{
		// Enter parameterization level 1 (S-0-0420) // Command C0400
		ID_ref->execute_command(TGM::SERCOS_Param_S, 420);

		// Primary Operation Mode (S-0-0032)
		ID_ref->write_parameter(TGM::SERCOS_Param_S, 32, opmode);

		// Leave parameterization level 1 (S-0-0422) // Command C0200
		ID_ref->execute_command(TGM::SERCOS_Param_S, 422);
	}
}

inline SPEEDUNITS get_units(SISProtocol * ID_ref)
{
	uint64_t curunits;
	// Scaling of speed units (S-0-0044)
	ID_ref->read_parameter(TGM::SERCOS_Param_S, 44, curunits);

	return SPEEDUNITS(static_cast<uint16_t>(curunits));
}

void change_units(SISProtocol * ID_ref)
{
	SPEEDUNITS units = get_units(ID_ref);
	if (units.bits.type_of_scaling == 0b010 && !units.bits.automode && !units.bits.scale_units && !units.bits.time_units && !units.bits.data_rel) return;

	// Set required units (preferred scaling, rotary scaling, [rpm])
	uint64_t scalingtype = 0b0000000000000010;
	// Velocity data scaling type (S-0-0044)
	ID_ref->write_parameter(TGM::SERCOS_Param_S, 44, scalingtype);
}

inline void change_language(SISProtocol * ID_ref, const uint8_t lang_code)
{
	// Language selection (S-0-0265):
	// * 0: German
	// * 1: English
	// * 2: French
	// * 3: Spanish
	// * 4: Italian
	ID_ref->write_parameter(TGM::SERCOS_Param_S, 265, (UINT32)lang_code);
}
