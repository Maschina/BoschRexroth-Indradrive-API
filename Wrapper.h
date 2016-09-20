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

	typedef struct SISProtocol SISProtocol;

	/// API: Fundumentals

	DLLEXPORT SISProtocol* DLLCALLCONV init();
	DLLEXPORT int32_t DLLCALLCONV open(SISProtocol* ID_ref, const char* ID_comport = "COM1", uint32_t ID_combaudrate = 19200, ErrHandle ID_err = ErrHandle());
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

#ifdef __cplusplus
}
#endif

#endif /* _WRAPPER_H_ */