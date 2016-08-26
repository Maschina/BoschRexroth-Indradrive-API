#include "stdafx.h"
#include "CppUnitTest.h"

#include "SISProtocol.h"
#include "Telegrams.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace SISProtocolTest
{		
	TEST_CLASS(utSISProtocol)
	{
	public:


		TEST_METHOD(SIS_SercosRead)
		{
			SISProtocol sis;

			sis.set_baudrate(SISProtocol::Baud_115200);
			
			
		}

	};
}