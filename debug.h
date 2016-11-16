#ifndef _DEBUG_H_
#define _DEBUG_H_

//#include "poppydebugtools.h"
#include <string>


#define STACK
#define STACK_SECTION(x)


class Stack
{
private: 
	Stack() {}
public: 
	static std::string GetTraceString() 
	{
		return std::string();
	}
};

#endif // !_DEBUG_H_

