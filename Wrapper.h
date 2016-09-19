#ifndef _WRAPPER_H_
#define _WRAPPER_H_

#include <Windows.h>

#include "SISProtocol.h"
#include "RS232.h"

#include "labview_types.h"
#include "labview_errors.h"
#include "labview_helpers.h"


/*  building a DLL  */
#define DLLIMPORT __declspec (dllexport)

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

	DLLIMPORT SISProtocol* init();
	DLLIMPORT int32_t open(SISProtocol* ID_ref, char* ID_comport = "COM1", uint32_t ID_combaudrate = 19200, LStrHandle ID_errmsg = LStrHandle());
	DLLIMPORT int32_t close(SISProtocol* ID_ref, LStrHandle ID_errmsg);


	/// API: Sequencer

	DLLIMPORT int32_t sequencer_activate(SISProtocol* ID_ref, LStrHandle ID_errmsg);
	DLLIMPORT int32_t sequencer_init(SISProtocol* ID_ref, uint32_t ID_max_accel, uint32_t ID_max_jerk, LStrHandle ID_errmsg);
	DLLIMPORT int32_t sequencer_write(SISProtocol* ID_ref, int32_t ID_speeds[], double_t ID_accels[], double_t ID_jerks[], uint32_t ID_delays[], const uint16_t ID_set_length, uint8_t ID_direction, LStrHandle ID_errmsg);
	DLLIMPORT int32_t sequencer_softtrigger(SISProtocol* ID_ref, LStrHandle ID_errmsg);

#ifdef __cplusplus
}
#endif

#endif /* _WRAPPER_H_ */