#include "Wrapper.h"

DLLIMPORT SISProtocol * init()
{
	return new SISProtocol();
}

DLLIMPORT __int32 open(SISProtocol * ref)
{
	if (!dynamic_cast<SISProtocol*>(ref)) return -1;
		// Return error for wrong reference
	
	return 0;
}
