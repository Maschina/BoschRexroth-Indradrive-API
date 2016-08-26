#ifndef _WRAPPER_H_
#define _WRAPPER_H_

/*  building a DLL  */
#define DLLIMPORT __declspec (dllexport)

#include "SISProtocol.h"


#ifdef __cplusplus
extern "C" {  /*  using a C++ compiler  */
#endif

	typedef struct SISProtocol SISProtocol;

	DLLIMPORT SISProtocol* init();
	DLLIMPORT __int32 open(SISProtocol* ref);

#ifdef __cplusplus
}
#endif

#endif /* _WRAPPER_H_ */