#ifndef _WRAPPER_H_
#define _WRAPPER_H_

#include <Windows.h>

#include "SISProtocol.h"
#include "RS232.h"
#include "errors.h"


/*  building a DLL  */
#define DLLEXPORT __declspec(dllexport)
#define DLLCALLCONV __cdecl

#ifndef _DLL
#error Project output has to be a DLL file
#endif

#if __cplusplus <= 199711L
#if _MSC_VER < 1900
#error This library needs at least Microsoft Visual Studio 2015 or a C++11 compliant compiler
#endif 

#ifndef _MSC_VER
#error This library needs at least a C++11 compliant compiler
#endif
#endif

#ifndef __cplusplus
#error C++ compiler required
#else
extern "C" {  /*  using a C++ compiler  */
#endif

	// Positioning mode lagless, encoder 1
	#define DRIVEMODE_SEQUENCER		0b111011
	// Velocity Control
	#define DRIVEMODE_SPEEDCONTROL	0b10


	typedef struct _opstate_t
	{
		union
		{
			struct bits_t
			{
				/* Bit 0-1:
				 * 00: Control section / power section not ready for operation (e.g., drive error or phase 2)
				 * 01: Control section ready for operation "bb"
				 * 10: Control section and power section ready for op. "Ab"
				 * 11: Drive with torque "AF"
				 */
				uint8_t ready_for_operation : 2;

				/// Bit 2: Drive Halt acknowledgment - 1: Drive Halt is active and axis is in standstill
				uint8_t drive_halted : 1;

				/// Bit 3: Drive error - 0: No error, 1: Drive error
				uint8_t drive_error : 1;

				bits_t(uint16_t _P_0_0115 = 0) :
					// Bit 14-15 @ P-0-0115
					ready_for_operation((_P_0_0115 >> 14) & 0b11),
					// Bit 4 @ P-0-0115
					drive_halted((_P_0_0115 >> 4) & 0b1),
					// Bit 13 @ P-0-0115
					drive_error((_P_0_0115 >> 13) & 0b1)
				{}
			} bits;

			uint8_t value;
		};

		_opstate_t(uint16_t _P_0_0115 = 0) : bits(_P_0_0115) {}
	} OPSTATE;


	typedef struct _speedunits_t
	{
		union
		{
			struct bits_t
			{
				/// Bit 0-2: Type of scaling
				/// * 001: Translational scaling
				/// * 010: Rotatory scaling
				uint16_t type_of_scaling : 3;

				/// Bit 3: Auto mode - 0: Preferred scaling, 1: Scaling by parameters
				uint16_t automode : 1;

				/// Bit 4: Units for translational/rotatory scaling - 0: Millimeter/Revolutions, 1: Inch/reserved
				uint16_t scale_units : 1;

				/// Bit 5: Time units - 0: Minute, 1: Second
				uint16_t time_units : 1;

				/// Bit 6: Data relation - 0: At motor shaft, 1: At load
				uint16_t data_rel : 1;

				/// Bit 7-15: reserved
				uint16_t res7 : 9;

				bits_t(uint16_t _S_0_0044 = 0) :
					// Bit 0-2 @ S-0-0044
					type_of_scaling((_S_0_0044) & 0b111),
					// Bit 3 @ S-0-0044
					automode((_S_0_0044 >> 3) & 0b1),
					// Bit 4 @ S-0-0044
					scale_units((_S_0_0044 >> 4) & 0b1),
					// Bit 5 @ S-0-0044
					time_units((_S_0_0044 >> 5) & 0b1),
					// Bit 6 @ S-0-0044
					data_rel((_S_0_0044 >> 6) & 0b1),
					// Bit 7-15 @ S-0-0044
					res7((_S_0_0044 >> 7) & 0b111111111)
				{}
			} bits;

			uint16_t value;
		};

		_speedunits_t(uint16_t _S_0_0044 = 0) : bits(_S_0_0044) {}
	} SPEEDUNITS;


	typedef struct SISProtocol SISProtocol;


	/// API: Fundumentals

	DLLEXPORT SISProtocol* DLLCALLCONV init();
	DLLEXPORT int32_t DLLCALLCONV open(SISProtocol* ID_ref, const wchar_t* ID_comport = L"COM1", uint32_t ID_combaudrate = 19200, ErrHandle ID_err = ErrHandle());
	DLLEXPORT int32_t DLLCALLCONV close(SISProtocol* ID_ref, ErrHandle ID_err = ErrHandle());


	/// API: Sequencer

	DLLEXPORT int32_t DLLCALLCONV sequencer_activate(SISProtocol* ID_ref, ErrHandle ID_err = ErrHandle());
	DLLEXPORT int32_t DLLCALLCONV sequencer_init(SISProtocol* ID_ref, uint32_t ID_max_accel, uint32_t ID_max_jerk, ErrHandle ID_err = ErrHandle());
	DLLEXPORT int32_t DLLCALLCONV sequencer_write(SISProtocol* ID_ref, int32_t ID_speeds[], double_t ID_accels[], double_t ID_jerks[], uint32_t ID_delays[], const uint16_t ID_set_length, uint8_t ID_direction, ErrHandle ID_err = ErrHandle());
	DLLEXPORT int32_t DLLCALLCONV sequencer_softtrigger(SISProtocol* ID_ref, ErrHandle ID_err = ErrHandle());


	/// API: SpeedControl

	DLLEXPORT int32_t DLLCALLCONV speedcontrol_activate(SISProtocol* ID_ref, ErrHandle ID_err = ErrHandle());
	DLLEXPORT int32_t DLLCALLCONV speedcontrol_init(SISProtocol* ID_ref, uint32_t ID_max_accel = 10000, uint32_t ID_max_jerk = 1000, ErrHandle ID_err = ErrHandle());
	DLLEXPORT int32_t DLLCALLCONV speedcontrol_write(SISProtocol* ID_ref, int32_t ID_speed, double_t ID_accel, ErrHandle ID_err = ErrHandle());


	/// API: Configuration

	DLLEXPORT int32_t DLLCALLCONV set_stdenvironment(SISProtocol* ID_ref, ErrHandle ID_err = ErrHandle());

	
	/// API: Status

	DLLEXPORT int32_t DLLCALLCONV get_drivemode(SISProtocol* ID_ref, uint32_t * ID_drvmode, ErrHandle ID_err = ErrHandle());
	DLLEXPORT int32_t DLLCALLCONV get_opstate(SISProtocol* ID_ref, uint8_t * ID_opstate, ErrHandle ID_err = ErrHandle());
	DLLEXPORT int32_t DLLCALLCONV get_speed(SISProtocol * ID_ref, double * ID_speed, ErrHandle ID_err = ErrHandle());
	DLLEXPORT int32_t DLLCALLCONV get_diagnostic_msg(SISProtocol* ID_ref, char * ID_diagnostic_msg, ErrHandle ID_err = ErrHandle());
	DLLEXPORT int32_t DLLCALLCONV get_diagnostic_num(SISProtocol* ID_ref, UINT32 * ID_diagnostic_num, ErrHandle ID_err = ErrHandle());


	/// Internal helper functions

	inline void change_opmode(SISProtocol * ID_ref, const uint64_t opmode);
	inline SPEEDUNITS get_units(SISProtocol * ID_ref);
	inline void change_units(SISProtocol * ID_ref);
	inline void change_language(SISProtocol * ID_ref, const uint8_t lang_code = 1);

#ifdef __cplusplus
}
#endif

#endif /* _WRAPPER_H_ */