/// @file
/// Definition of API functions that are exported to the API DLL

#ifndef _WRAPPER_H_
#define _WRAPPER_H_

/// Doxygen's mainpage documentation
#include "mainpage.dox"

#include <Windows.h>

#include "SISProtocol.h"
#include "RS232.h"
#include "errors.h"
#include "debug.h"


/// Macro to indicate that a static function shall be exported for the target DLL
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

	/// Positioning mode lagless, encoder 1.
	#define DRIVEMODE_SEQUENCER		0b111011
	/// Velocity Control.
	#define DRIVEMODE_SPEEDCONTROL	0b10

	/// Structure is used for loading the payload of the Reception Telegram from the Indradrive SERCOS parameter P-0-
	/// 0115.
	/// 
	/// The structure is designed to be loaded with an integer, but automatically structured into its components. Thus,
	/// it is possible extract the exact information that are requested (e.g. Operate State of Indradrive M device).
	/// 
	/// The following code demonstrates a possible usage of this struct:
	/// @code{.cpp}
	/// uint64_t curopstate;
	/// SISProtocol_ref->read_parameter(TGM::SercosParamP, 115, curopstate);
	/// 
	/// OPSTATE opstate(static_cast<uint16_t>(curopstate));
	/// int foo = opstate.Value;
	/// @endcode.
	///
	/// @sa	SISProtocol
	/// @sa	SISProtocol::read_parameter
	typedef struct OPSTATE
	{
		union
		{
			struct Bits
			{
				/// Bit 0-1 of parameter's payload:
				/// * 0b00: Control section / power section not ready for operation(e.g., drive error or phase 2)
				/// * 0b01 : Control section ready for operation "bb"
				/// * 0b10 : Control section and power section ready for op. "Ab"
				/// * 0b11 : Drive with torque "AF".
				uint8_t OperateState : 2;

				/// Bit 2 of parameter's payload: Drive Halt acknowledgment
				/// * 0: Drive Halt not active
				/// * 1: Drive Halt is active and axis is in standstill
				uint8_t DriveHalted : 1;

				/// Bit 3 of parameter's payload: Drive error
				/// * 0: No error  
				/// * 1: Drive error
				uint8_t DriveError : 1;

				/// Constructor.
				///
				/// @param	P_0_0115	(Optional) Payload data of SERCOS P-0-0115 parameter feedback. Default: 0.
				Bits(uint16_t P_0_0115 = 0) :
					/// Bit 14-15 @ P-0-0115.
					OperateState((P_0_0115 >> 14) & 0b11),
					/// Bit 4 @ P-0-0115.
					DriveHalted((P_0_0115 >> 4) & 0b1),
					/// Bit 13 @ P-0-0115.
					DriveError((P_0_0115 >> 13) & 0b1)
				{}
			} Bits;

			/// Raw and unstructured data value
			uint8_t Value;
		};

		/// Constructor.
		///
		/// @param	P_0_0115	(Optional) Payload data of SERCOS P-0-0115 parameter feedback. Default: 0.
		OPSTATE(uint16_t P_0_0115 = 0) : Bits(P_0_0115) {}
	} OPSTATE;


	/// Structure is used for loading the payload of the Reception Telegram from the Indradrive SERCOS parameter S-0-
	/// 0044
	/// 
	/// The structure is designed to be loaded with an integer, but automatically structured into its components. Thus,
	/// it is possible extract the exact information that are requested (e.g. Operate State of Indradrive M device).
	typedef struct SPEEDUNITS
	{
		union
		{
			struct Bits
			{
				/// Bit 0-2 of parameter's payload: Type of scaling
				/// * 0b001: Translational scaling
				/// * 0b010: Rotatory scaling.
				uint16_t type_of_scaling : 3;

				/// Bit 3 of parameter's payload: Auto mode
				/// * 0: Preferred scaling  
				/// * 1: Scaling by parameters
				uint16_t automode : 1;

				/// Bit 4 of parameter's payload: Units for translational/rotatory scaling
				/// * 0: Millimeter/Revolutions  
				/// * 1: Inch/reserved
				uint16_t scale_units : 1;

				/// Bit 5 of parameter's payload: Time units
				/// * 0: Minute  
				/// * 1: Second
				uint16_t time_units : 1;

				/// Bit 6 of parameter's payload: Data relation
				/// * 0: At motor shaft  
				/// * 1: At load
				uint16_t data_rel : 1;

				/// Bit 7-15 of parameter's payload: reserved
				uint16_t res7 : 9;

				/// Constructor.
				///
				/// @param	S_0_0044	(Optional) Reception Telegram's payload data
				Bits(uint16_t S_0_0044 = 0) :
					// Bit 0-2 @ S-0-0044
					type_of_scaling((S_0_0044) & 0b111),
					// Bit 3 @ S-0-0044
					automode((S_0_0044 >> 3) & 0b1),
					// Bit 4 @ S-0-0044
					scale_units((S_0_0044 >> 4) & 0b1),
					// Bit 5 @ S-0-0044
					time_units((S_0_0044 >> 5) & 0b1),
					// Bit 6 @ S-0-0044
					data_rel((S_0_0044 >> 6) & 0b1),
					// Bit 7-15 @ S-0-0044
					res7((S_0_0044 >> 7) & 0b111111111)
				{}
			} Bits;

			/// Raw and unstructured data value
			uint16_t Value;
		};

		/// Constructor.
		///
		/// @param	S_0_0044	(Optional) Reception Telegram's payload data
		SPEEDUNITS(uint16_t S_0_0044 = 0) : Bits(S_0_0044) {}
	} SPEEDUNITS;

	
	/// Faking the actual SISProtocol class to a struct so that the C compiler can handle compilation of this file.
	/// The SISProtocol files itself should be automically compiled using the C++ compilation process. This is
	/// automatically handled using extern "C".
	typedef struct SISProtocol SISProtocol;


#pragma region API Fundamentals

	/// Creates API reference.
	/// 
	/// The API references is a fundamental prerequisite.
	///
	/// @remarks	This function is exported to the Indradrive API DLL.
	/// 			
	/// @remarks	Refer to @ref sec_Examples "Examples" for detailed code examples.
	///
	/// @remarks	How to call with C\#:
	/// 			@code{.cs}
	/// 			[DllImport(dllpath, CharSet = CharSet.Unicode, CallingConvention = CallingConvention.Cdecl)]
	/// 			private static extern int init();
	/// 			@endcode.
	///
	/// @remarks	How to call with Python:
	/// 			@code{.py}
	/// 			indraref = indralib.init()
	/// 			@endcode.
	///
	/// @return	API reference. Pointer can be casted and treated as UINT32 (see examples).
	DLLEXPORT SISProtocol* DLLCALLCONV init();

	/// Opens the communication port to the Indradrive device.
	/// 
	/// @attention Baudrate selection is not support. Default of 19200 Bits/s is used.
	///
	/// @remarks	This function is exported to the Indradrive API DLL.
	///
	/// @remarks	Refer to @ref sec_Examples "Examples" for detailed code examples.
	///
	/// @remarks	How to call with C\#:
	/// 			@code{.cs}
	/// 			[DllImport(dllpath, CharSet = CharSet.Unicode, CallingConvention = CallingConvention.Cdecl)]
	/// 			private static extern int open(int ID_ref, Byte[] ID_comport, UInt32 ID_combaudrate, ref ErrHandle ID_err);
	/// 			@endcode.
	///
	/// @remarks	How to call with Python:
	/// 			@code{.py}
	/// 			result = indralib.open(indraref, b"COM1", 19200, ctypes.byref(indra_error))
	/// 			@endcode.
	///
	/// @param [in]		ID_ref		  	API reference. Pointer can be casted in from UINT32.
	/// @param [in]		ID_comport	  	(Optional) Communication port. Default: L"COM1".
	/// @param [in]		ID_combaudrate	(Optional) Communication baudrate in [Bits/s]. Default: 19200 Bits/s.
	/// @param [out]	ID_err		  	(Optional) Error handle.
	///
	/// @return	Error handle return code (ErrHandle()).
	DLLEXPORT int32_t DLLCALLCONV open(SISProtocol* ID_ref, const wchar_t* ID_comport = L"COM1", uint32_t ID_combaudrate = 19200, ErrHandle ID_err = ErrHandle());

	/// Closes the communication port at the Indradrive device.
	///
	/// @remarks	This function is exported to the Indradrive API DLL.
	///
	/// @remarks	Refer to @ref sec_Examples "Examples" for detailed code examples.
	///
	/// @remarks	How to call with C\#:
	/// 			@code{.cs}
	/// 			[DllImport(dllpath, CharSet = CharSet.Unicode, CallingConvention = CallingConvention.Cdecl)]
	/// 			private static extern int close(int ID_ref, ref ErrHandle ID_err);
	/// 			@endcode.
	///
	/// @remarks	How to call with Python:
	/// 			@code{.py}
	/// 			result = indralib.close(indraref, ctypes.byref(indra_error))
	/// 			@endcode.
	///
	/// @param [in]		ID_ref	API reference. Pointer can be casted in from UINT32.
	/// @param [out]	ID_err	(Optional) Error handle.
	///
	/// @return	Error handle return code (ErrHandle()).
	DLLEXPORT int32_t DLLCALLCONV close(SISProtocol* ID_ref, ErrHandle ID_err = ErrHandle());

#pragma endregion API Fundamentals


#pragma region API Sequencer

	/// Activates the drive mode "Sequencer".
	/// 
	/// @attention	Reiterate calls of this functions will harm the Indradrive EEPROM (due to limited write cycles). Use
	/// get_drivemode() to check if this function call is really needed.
	///
	/// @remarks	This function is exported to the Indradrive API DLL.
	///
	/// @remarks	Calling sequencer_* functions without calling sequencer_activate() first means that the drive will
	/// 			not operate in this mode.
	///
	/// @remarks	Refer to @ref sec_Examples "Examples" for detailed code examples.
	///
	/// @remarks	How to call with C\#:
	/// 			@code{.cs}
	/// 			[DllImport(dllpath, CharSet = CharSet.Unicode, CallingConvention = CallingConvention.Cdecl)]
	/// 			private static extern int sequencer_activate(int ID_ref, ref ErrHandle ID_err);
	/// 			@endcode.
	///
	/// @param [in]		ID_ref	API reference. Pointer can be casted in from UINT32.
	/// @param [out]	ID_err	(Optional) Error handle.
	///
	/// @return	Error handle return code (ErrHandle()).
	DLLEXPORT int32_t DLLCALLCONV sequencer_activate(SISProtocol* ID_ref, ErrHandle ID_err = ErrHandle());

	/// Initializes limits and sets the right scaling/unit factors for operation of "Sequencer" drive mode.
	///
	/// @remarks	This function is exported to the Indradrive API DLL.
	///
	/// @remarks	Refer to @ref sec_Examples "Examples" for detailed code examples.
	///
	/// @remarks	How to call with C\#:
	/// 			@code{.cs}
	/// 			[DllImport(dllpath, CharSet = CharSet.Unicode, CallingConvention = CallingConvention.Cdecl)]
	/// 			private static extern int sequencer_init(int ID_ref, Double ID_max_accel, Double ID_max_jerk, ref ErrHandle ID_err);
	/// 			@endcode.
	///
	/// @param [in]		ID_ref			API reference. Pointer can be casted in from UINT32.
	/// @param [in]		ID_max_accel	(Optional) Maximum allowed acceleration in [rad/s^2]. Default: 10000 rad/s^2.
	/// @param [in]		ID_max_jerk 	(Optional) Maximum allowed jerk in [rad/s^3]. Default: 1000 rad/s^3.
	/// @param [out]	ID_err			(Optional) Error handle.
	///
	/// @return	Error handle return code (ErrHandle()).
	DLLEXPORT int32_t DLLCALLCONV sequencer_init(SISProtocol* ID_ref, double_t ID_max_accel = 10000, double_t ID_max_jerk = 1000, ErrHandle ID_err = ErrHandle());

	/// Writes the whole run sequence into the device.
	/// 
	/// The run sequence is defined by several kinematic parameters, such as speed, acceleration, or jerk. A proper
	/// calculation of the kinetics before writing is assumed.
	/// 
	/// @bug List length will not be extended automatically. In case of list length is set too short, programming
	/// 			of all values might fail. This may cause an improper operation of the "Sequencer" drive mode.
	///
	/// @remarks	This function is exported to the Indradrive API DLL.
	///
	/// @remarks	Refer to @ref sec_Examples "Examples" for detailed code examples.
	///
	/// @remarks	How to call with C\#:
	/// 			@code{.cs}
	/// 			[DllImport(dllpath, CharSet = CharSet.Unicode, CallingConvention = CallingConvention.Cdecl)]
	/// 			private static extern int sequencer_write(int ID_ref, Double[] ID_speeds, Double[] ID_accels, Double[] ID_jerks, UInt32[] ID_delays, UInt16 ID_set_length, ref ErrHandle ID_err);
	/// 			@endcode.
	///
	/// @param [in]		ID_ref		 	API reference. Pointer can be casted in from UINT32.
	/// @param [in]		ID_speeds	 	Sequencer speed list in [1/min]. Rotation directions are defined by the sign of
	/// 								each element:
	/// 								* Positive sign: Clockwise direction  
	/// 								* Negative sign: Counter-clockwise direction.
	/// @param [in]		ID_accels	 	Sequencer acceleration list in [rad/s^2].
	/// @param [in]		ID_jerks	 	Sequencer jerk list in [rad/s^3].
	/// @param [in]		ID_delays	 	Delay list representing delay between each kinematic step in [cs].
	/// @param [in]		ID_set_length	Length of the sequence (=number of elements of ID_speeds, ID_accels, etc).
	/// @param [out]	ID_err		 	(Optional) Error handle.
	///
	/// @return	Error handle return code (ErrHandle()).
	DLLEXPORT int32_t DLLCALLCONV sequencer_write(SISProtocol* ID_ref, double_t ID_speeds[], double_t ID_accels[], double_t ID_jerks[], uint32_t ID_delays[], const uint16_t ID_set_length, ErrHandle ID_err = ErrHandle());

	/// Software-Trigger to start operation of the "Sequencer" drive mode.
	///
	/// @remarks	This function is exported to the Indradrive API DLL.
	///
	/// @remarks	Refer to @ref sec_Examples "Examples" for detailed code examples.
	///
	/// @remarks	How to call with C\#:
	/// 			@code{.cs}
	/// 			[DllImport(dllpath, CharSet = CharSet.Unicode, CallingConvention = CallingConvention.Cdecl)]
	/// 			private static extern int sequencer_softtrigger(int ID_ref, ref ErrHandle ID_err);
	/// 			@endcode.
	///
	/// @param [in]		ID_ref	API reference. Pointer can be casted in from UINT32.
	/// @param [out]	ID_err	(Optional) Error handle.
	///
	/// @return	Error handle return code (ErrHandle()).
	DLLEXPORT int32_t DLLCALLCONV sequencer_softtrigger(SISProtocol* ID_ref, ErrHandle ID_err = ErrHandle());

	/// Hardware-Trigger to start operation of the "Sequencer" drive mode.
	/// By special PLC software (if configured), the hardware trigger is realized through a 24V rising edge input line.
	///
	/// @remarks	This function is exported to the Indradrive API DLL.
	/// 
	/// @remarks	Refer to @ref sec_Examples "Examples" for detailed code examples.
	///
	/// @remarks	How to call with C\#:
	/// 			@code{.cs}
	/// 			[DllImport(dllpath, CharSet = CharSet.Unicode, CallingConvention = CallingConvention.Cdecl)]
	/// 			private static extern int sequencer_hardtrigger(int ID_ref, ref ErrHandle ID_err);
	/// 			@endcode.
	///
	/// @param [in]		ID_ref	API reference. Pointer can be casted in from UINT32.
	/// @param [out]	ID_err	(Optional) Error handle.
	///
	/// @return	Error handle return code (ErrHandle()).
	DLLEXPORT int32_t DLLCALLCONV sequencer_hardtrigger(SISProtocol* ID_ref, ErrHandle ID_err = ErrHandle());

	/// Get the status of the "Sequencer" drive mode.
	/// The information is derived from the PLC that reports the actual status within an internal register.
	///
	/// @remarks	This function is exported to the Indradrive API DLL.
	/// 
	/// @remarks	Refer to @ref sec_Examples "Examples" for detailed code examples.
	///
	/// @remarks	How to call with C\#:
	/// 			@code{.cs}
	/// 			[DllImport(dllpath, CharSet = CharSet.Unicode, CallingConvention = CallingConvention.Cdecl)]
	/// 			private static extern int sequencer_getstatus(int ID_ref, ref UInt16 status, ref ErrHandle ID_err);
	/// 			@endcode.
	///
	/// @remarks	How to call with Python:
	/// 			@code{.py}
	/// 			plcstatus = ctypes.c_uint16(0)
	/// 			result = indralib.sequencer_getstatus(indraref, ctypes.byref(plcstatus), ctypes.byref(indra_error))
	/// 			@endcode.
	///
	/// @param [in]		ID_ref	API reference. Pointer can be casted in from UINT32.
	/// @param [out]	ID_status	Pointer that provides the respective information:
	/// 							* Bit 0 - bSeqFinished, if true then end of the sequence has been reached.
	/// 							* Bit 1 - bCamTick, if true then cam is currently shifting to the next position.
	/// 							* Bit 2 - bDriveStopped, if true then drive has been stopped by PLC.
	/// 							* Bit 3 - bDriveStarted, if true then drive has been started by PLC.
	/// 							* Bit 4 - ERROR_T_PARAM, if true then error has been occurred while reading input parameters.
	/// 							* Bit 5 - RESULT_READ_OK, if true then reading input parameters have been processed correctly.
	/// 							* Bit 6 - RESULT_SEQUENCER_OK, if true then sequencer is/was running well.
	/// 							* Bit 7 - ERROR_T_CAM, if true then sequencer processing had some issues.
	/// @param [out]	ID_err	(Optional) Error handle.
	///
	/// @return	Error handle return code (ErrHandle()).
	DLLEXPORT int32_t DLLCALLCONV sequencer_getstatus(SISProtocol* ID_ref, uint16_t * ID_status, ErrHandle ID_err = ErrHandle());

#pragma endregion API Sequencer


#pragma region API Speed Control

	/// Activates the drive mode "Speed Control".
	/// 
	/// @attention Reiterate calls of this functions will harm the Indradrive EEPROM (due to limited write cycles). Use
	/// get_drivemode() to check if this function call is really needed.
	///
	/// @remarks	This function is exported to the Indradrive API DLL.
	///
	/// @remarks	Refer to @ref sec_Examples "Examples" for detailed code examples.
	///
	/// @remarks	How to call with C\#:
	/// 			@code{.cs}
	/// 			[DllImport(dllpath, CharSet = CharSet.Unicode, CallingConvention = CallingConvention.Cdecl)]
	/// 			private static extern int speedcontrol_activate(int ID_ref, ref ErrHandle ID_err);
	/// 			@endcode.
	///
	/// @remarks	How to call with Python:
	/// 			@code{.py}
	/// 			result = indralib.speedcontrol_activate(indraref, ctypes.byref(indra_error))
	/// 			@endcode.
	///
	/// @param [in]		ID_ref	API reference. Pointer can be casted in from UINT32.
	/// @param [out]	ID_err	(Optional) Error handle.
	///
	/// @return	Error handle return code (ErrHandle()).
	DLLEXPORT int32_t DLLCALLCONV speedcontrol_activate(SISProtocol* ID_ref, ErrHandle ID_err = ErrHandle());

	/// Initializes limits and sets the right scaling/unit factors for operation of "Speed Control" drive mode.
	///
	/// @remarks	This function is exported to the Indradrive API DLL.
	///
	/// @remarks	Refer to @ref sec_Examples "Examples" for detailed code examples.
	///
	/// @remarks	How to call with C\#:
	/// 			@code{.cs}
	/// 			[DllImport(dllpath, CharSet = CharSet.Unicode, CallingConvention = CallingConvention.Cdecl)]
	/// 			private static extern int speedcontrol_init(int ID_ref, Double ID_max_accel, Double ID_max_jerk, ref ErrHandle ID_err);
	/// 			@endcode.
	///
	/// @remarks	How to call with Python:
	/// 			@code{.py}
	/// 			result = indralib.speedcontrol_init(indraref, ctypes.c_double(10000), ctypes.c_double(1000), ctypes.byref(indra_error))
	/// 			@endcode.
	///
	/// @param [in]		ID_ref			API reference. Pointer can be casted in from UINT32.
	/// @param [out]	ID_max_accel	(Optional) Maximum allowed acceleration in [rad/s^2]. Default: 10000 rad/s^2.
	/// @param [out]	ID_max_jerk 	(Optional) Maximum allowed jerk in [rad/s^3]. Default: 1000 rad/s^3.
	/// @param [out]	ID_err			(Optional) Error handle.
	///
	/// @return	Error handle return code (ErrHandle()).
	DLLEXPORT int32_t DLLCALLCONV speedcontrol_init(SISProtocol* ID_ref, double_t ID_max_accel = 10000, double_t ID_max_jerk = 1000, ErrHandle ID_err = ErrHandle());

	/// Writes the current kinematic (speed and acceleration) into the device.
	///
	/// @remarks	This function is exported to the Indradrive API DLL.
	///
	/// @remarks	Refer to @ref sec_Examples "Examples" for detailed code examples.
	///
	/// @remarks	How to call with C\#:
	/// 			@code{.cs}
	/// 			[DllImport(dllpath, CharSet = CharSet.Unicode, CallingConvention = CallingConvention.Cdecl)]
	/// 			private static extern int speedcontrol_write(int ID_ref, Double ID_speed, Double ID_accel, ref ErrHandle ID_err);
	/// 			@endcode.
	///
	/// @remarks	How to call with Python:
	/// 			@code{.py}
	/// 			result = indralib.speedcontrol_write(indraref, ctypes.c_double(speed), ctypes.c_double(10), ctypes.byref(indra_error))
	/// 			@endcode.
	///
	/// @param [in]		ID_ref  	API reference. Pointer can be casted in from UINT32.
	/// @param [out]	ID_speed	Target speed in [1/min]. Sign represents the rotation direction:
	/// 							* Positive sign: Clockwise direction  
	/// 							* Negative sign: Counter-clockwise direction.
	/// @param [out]	ID_accel	Target acceleration in [rad/s^2].
	/// @param [out]	ID_err  	(Optional) Error handle.
	///
	/// @return	Error handle return code (ErrHandle()).
	DLLEXPORT int32_t DLLCALLCONV speedcontrol_write(SISProtocol* ID_ref, double_t ID_speed, double_t ID_accel, ErrHandle ID_err = ErrHandle());

#pragma endregion API Speed Control


#pragma region API Configuration

	/// Sets the proper unit and language environment.
	/// 
	/// Proper unit and language environment is:
	/// * for unit setup: Preferred scaling / Rotary scaling / Unit [rpm] / Velocity data scaling
	/// * for language environment: English language.  
	/// 
	/// @attention Not setting the proper unit and language environment may cause unexpected behavior when programming
	/// and setting kinematics.
	///
	/// @remarks	This function is exported to the Indradrive API DLL.
	///
	/// @remarks	Refer to @ref sec_Examples "Examples" for detailed code examples.
	///
	/// @remarks	How to call with Python:
	/// 			@code{.py}
	/// 			result = indralib.set_stdenvironment(indraref, ctypes.byref(indra_error))
	/// 			@endcode.
	///
	/// @param [in,out]	ID_ref	API reference. Pointer can be casted in from UINT32.
	/// @param 		   	ID_err	(Optional) Error handle.
	///
	/// @return	Error handle return code (ErrHandle()).
	DLLEXPORT int32_t DLLCALLCONV set_stdenvironment(SISProtocol* ID_ref, ErrHandle ID_err = ErrHandle());

#pragma endregion API Configuration


#pragma region API Status

	/// Retrieve information about the drive mode: Speed Control or Sequencer.
	/// 
	/// The drive mode feedback is provided by <c>ID_drvmode</c> parameter. The following table depicts the coding:
	/// 
	/// If | Then
	/// -- | -----------
	/// <c>*ID_drvmode == 0</c> | Drive Mode not supported
	/// <c>*ID_drvmode == 1</c> | "Sequencer" drive mode active
	/// <c>*ID_drvmode == 2</c> | "Speed Control" drive mode active.
	///
	/// @remarks	This function is exported to the Indradrive API DLL.
	///
	/// @remarks	The drive mode can be changed by speedcontrol_activate() or sequencer_activate().
	///
	/// @remarks	Refer to @ref sec_Examples "Examples" for detailed code examples.
	///
	/// @remarks	How to call with C\#:
	/// 			@code{.cs}
	/// 			[DllImport(dllpath, CharSet = CharSet.Unicode, CallingConvention = CallingConvention.Cdecl)]
	/// 			private static extern int get_drivemode(int ID_ref, ref UInt32 mode, ref ErrHandle ID_err);
	/// 			@endcode.
	///
	/// @remarks	How to call with Python:
	/// 			@code{.py}
	/// 			drvmode = ctypes.c_uint32(0)
	/// 			result = indralib.get_drivemode(indraref, ctypes.byref(drvmode), ctypes.byref(indra_error))
	/// 			@endcode.
	///
	/// @param [in]		ID_ref	  	API reference. Pointer can be casted in from UINT32.
	/// @param [out]	ID_drvmode	Pointer that provides the respective information:
	/// 							* 0 - Drive Mode not supported,
	/// 							* 1 - "Sequencer" drive mode active,
	/// 							* 2 - "Speed Control" drive mode active.
	/// @param [out]	ID_err	  	(Optional) Error handle.
	///
	/// @return	Error handle return code (ErrHandle()).
	DLLEXPORT int32_t DLLCALLCONV get_drivemode(SISProtocol* ID_ref, uint32_t * ID_drvmode, ErrHandle ID_err = ErrHandle());

	/// Retrieve information about the operation states: bb, Ab, or AF.
	/// 
	/// The operation state feedback is provided by <c>ID_opstate</c> parameter. The following table depicts the coding:
	/// 
	/// If | Then
	/// -- | -----------
	/// <c>(*ID_opstate &amp; 0b11) == 0b00</c> | Control section / power section not ready for operation(e.g., drive error or phase 2)
	/// <c>(*ID_opstate &amp; 0b11) == 0b01</c> | Control section ready for operation "bb"
	/// <c>(*ID_opstate &amp; 0b11) == 0b10</c> | Control section and power section ready for op. "Ab"
	/// <c>(*ID_opstate &amp; 0b11) == 0b11</c> | Drive with torque "AF".
	/// <c>((*ID_opstate &amp; 0b100) &gt;&gt; 2) == 1</c> | Drive Halt is active and axis is in standstill
	/// <c>((*ID_opstate &amp; 0b1000) &gt;&gt; 3) == 1</c> | Drive error.
	///
	/// @remarks	This function is exported to the Indradrive API DLL.
	///
	/// @remarks	Refer to @ref sec_Examples "Examples" for detailed code examples.
	///
	/// @remarks	How to call with C\#:
	/// 			@code{.cs}
	/// 			[DllImport(&lt;path_to_DLL&gt;, CharSet = CharSet.Unicode, CallingConvention = CallingConvention.Cdecl)]
	/// 			private static extern int get_opstate(int ID_ref, ref Byte state, ref ErrHandle ID_err);
	/// 			@endcode.
	///
	/// @remarks	How to call with Python:
	/// 			@code{.py}
	/// 			opstate = ctypes.c_uint8(0)
	/// 			result = indralib.get_opstate(indraref, ctypes.byref(opstate), ctypes.byref(indra_error))
	/// 			@endcode.
	///
	/// @param [in]		ID_ref	  	API reference. Pointer can be casted in from UINT32.
	/// @param [out]	ID_opstate	Pointer that provides the respective information:
	/// 							* Bit 0-1: Operation state
	/// 								* <c>0b00</c>: Control section / power section not ready for operation(e.g., drive
	/// 								error or phase 2)
	/// 								* <c>0b01</c>: Control section ready for operation "bb"
	/// 								* <c>0b10</c>: Control section and power section ready for op. "Ab"
	/// 								* <c>0b11</c>: Drive with torque "AF".
	/// 							* Bit 2: Drive Halt acknowledgment
	/// 								* <c>0</c>: Drive not halted  
	/// 								* <c>1</c>: Drive Halt is active and axis is in standstill  
	/// 							* Bit 3: Drive error  
	/// 								* <c>0</c>: No error  
	/// 								* <c>1</c>: Drive error present.
	/// @param [out]	ID_err	  	(Optional) Error handle.
	///
	/// @return	Error handle return code (ErrHandle()).
	DLLEXPORT int32_t DLLCALLCONV get_opstate(SISProtocol* ID_ref, uint8_t * ID_opstate, ErrHandle ID_err = ErrHandle());

	/// Gets the actual rotation speed.
	///
	/// @remarks	This function is exported to the Indradrive API DLL.
	///
	/// @remarks	Refer to @ref sec_Examples "Examples" for detailed code examples.
	///
	/// @remarks	How to call with C\#:
	/// 			@code{.cs}
	/// 			[DllImport(dllpath, CharSet = CharSet.Unicode, CallingConvention = CallingConvention.Cdecl)]
	/// 			private static extern int get_speed(int ID_ref, ref Double speed, ref ErrHandle ID_err);
	/// 			@endcode.
	///
	/// @param [in]		ID_ref  	API reference. Pointer can be casted in from UINT32.
	/// @param [out]	ID_speed	Pointer that provides the speed information as double Value in [1/min]. Sign
	/// 							represents the rotation direction:
	/// 							* Positive sign: Clockwise direction  
	/// 							* Negative sign: Counter-clockwise direction.
	/// @param [out]	ID_err  	(Optional) Error handle.
	///
	/// @return	Error handle return code (ErrHandle()).
	DLLEXPORT int32_t DLLCALLCONV get_speed(SISProtocol * ID_ref, double_t * ID_speed, ErrHandle ID_err = ErrHandle());

	/// Gets diagnostic message string of the current Indradrive status.
	/// 
	/// @attention The API presumes a properly allocated char array for <c>ID_diagnostic_msg</c> parameter.
	///
	/// @remarks	This function is exported to the Indradrive API DLL.
	///
	/// @remarks	Refer to @ref sec_Examples "Examples" for detailed code examples.
	///
	/// @remarks	How to call with C\#:
	/// 			@code{.cs}
	/// 			[DllImport(dllpath, CharSet = CharSet.Unicode, CallingConvention = CallingConvention.Cdecl)]
	/// 			private static extern int get_diagnostic_msg(int ID_ref, Byte[] ID_diagnostic_msg, ref ErrHandle ID_err);
	/// 			@endcode.
	///
	/// @remarks	How to call with Python:
	/// 			@code{.py}
	/// 			diagmsg = ctypes.create_string_buffer(256)
	/// 			result = indralib.get_diagnostic_msg(indraref, diagmsg, ctypes.byref(indra_error))
	/// 			@endcode.
	///
	/// @param [in]		ID_ref			 	API reference. Pointer can be casted in from UINT32.
	/// @param [out]	ID_diagnostic_msg	Pointer that provides the diagnostic message string.
	/// @param [out]	ID_err			 	(Optional) Error handle.
	///
	/// @return	Error handle return code (ErrHandle()).
	DLLEXPORT int32_t DLLCALLCONV get_diagnostic_msg(SISProtocol* ID_ref, char * ID_diagnostic_msg, ErrHandle ID_err = ErrHandle());

	/// Gets diagnostic number of the current Indradrive status.
	///
	/// @remarks	This function is exported to the Indradrive API DLL.
	///
	/// @remarks	The coding of the diagnostic number is described in the document "Rexroth IndraDrive Firmware for
	/// 			Drive Controller MPH-04, MPB-04, MPD-04 / Function Description (Chapter 10.3). For example, operation
	/// 			state "AF" is described as 0xA0101.
	///
	/// @remarks	Refer to @ref sec_Examples "Examples" for detailed code examples.
	///
	/// @remarks	How to call with C\#:
	/// 			@code{.cs}
	/// 			[DllImport(dllpath, CharSet = CharSet.Unicode, CallingConvention = CallingConvention.Cdecl)]
	/// 			private static extern int get_diagnostic_num(int ID_ref, ref UInt32 ID_diagnostic_num, ref ErrHandle ID_err);
	/// 			@endcode.
	///
	/// @param [in]		ID_ref			 	API reference. Pointer can be casted in from UINT32.
	/// @param [out]	ID_diagnostic_num	Pointer that provides the diagnostic number.
	/// @param [out]	ID_err			 	(Optional) Error handle.
	///
	/// @return	Error handle return code (ErrHandle()).
	DLLEXPORT int32_t DLLCALLCONV get_diagnostic_num(SISProtocol* ID_ref, uint32_t * ID_diagnostic_num, ErrHandle ID_err = ErrHandle());

	/// Clears a latched error in the Indradrive device.
	/// 
	/// In case of error that has been occurred on the Indradrive, the error information is latched until cleared with
	/// this call.
	///
	/// @remarks	This function is exported to the Indradrive API DLL.
	///
	/// @remarks	Use get_diagnostic_message() and/or get_diagnostic_num() for retrieving the error information.
	///
	/// @remarks	Refer to @ref sec_Examples "Examples" for detailed code examples.
	///
	/// @remarks	How to call with C\#:
	/// 			@code{.cs}
	/// 			[DllImport(dllpath, CharSet = CharSet.Unicode, CallingConvention = CallingConvention.Cdecl)]
	/// 			private static extern int clear_error(int ID_ref, ref ErrHandle ID_err);
	/// 			@endcode.
	///
	/// @param [in]		ID_ref	API reference. Pointer can be casted in from UINT32.
	/// @param [out]	ID_err	(Optional) Error handle.
	///
	/// @return	Error handle return code (ErrHandle()).
	DLLEXPORT int32_t DLLCALLCONV clear_error(SISProtocol* ID_ref, ErrHandle ID_err = ErrHandle());

#pragma endregion API Status
	
	/* \cond Do not document this */
	
#pragma region Internal helper functions

	/// Called by speedcontrol_activate() and sequencer_activate() to set the desired operation mode.
	///
	/// @param [in]	ID_ref	API reference. Pointer can be casted in from UINT32.
	/// @param [in]	opmode	Desired operation mode. Use constants DRIVEMODE_SEQUENCER as well as DRIVEMODE_SPEEDCONTROL
	/// 					for setting.
	inline void change_opmode(SISProtocol * ID_ref, const uint64_t opmode);

	/// Gets the units.
	///
	/// @param [in]	ID_ref	API reference. Pointer can be casted in from UINT32.
	///
	/// @return	Unit information.
	inline SPEEDUNITS get_units(SISProtocol * ID_ref);

	/// Change units according to the requirements. Used by set_stdenvironment().
	///
	/// @param [in]	ID_ref	API reference. Pointer can be casted in from UINT32.
	inline void change_units(SISProtocol * ID_ref);

	/// Change language of the diagnostic messages.
	///
	/// @param [in]	ID_ref   	API reference. Pointer can be casted in from UINT32.
	/// @param [in]	lang_code	(Optional) The language code:
	/// 						* 0: German
	/// 						* 1: English
	/// 						* 2: French
	/// 						* 3: Spanish
	/// 						* 4: Italian.
	inline void change_language(SISProtocol * ID_ref, const uint8_t lang_code = 1);
	
#pragma endregion Internal helper functions

	/* \endcond Do not document this */

#ifdef __cplusplus
}
#endif

#endif /* _WRAPPER_H_ */