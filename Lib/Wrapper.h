#ifndef _WRAPPER_H_
#define _WRAPPER_H_

#include "SISProtocol.h"


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

	DLLIMPORT SISProtocol* init();
	DLLIMPORT __int32 open(SISProtocol* ref);

#ifdef __cplusplus
}
#endif

#endif /* _WRAPPER_H_ */