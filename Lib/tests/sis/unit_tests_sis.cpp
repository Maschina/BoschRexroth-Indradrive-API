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


		TEST_METHOD(SIS_SetBaudrate)
		{
			SISProtocol sis;

			try
			{
				sis.open("COM1");
				sis.set_baudrate(SISProtocol::Baud_19200);
			}
			catch (SISProtocol::ExceptionTransceiveFailed &ex)
			{
				Assert::Fail(char2wchar(ex.what()));
			}
			catch (SISProtocol::ExceptionGeneric &ex)
			{
				Assert::Fail(char2wchar(ex.what()));
			}
			catch (CSerial::ExceptionReceptionFailed &ex)
			{
				Assert::Fail(char2wchar(ex.what()));
			}
			catch (CSerial::ExceptionGeneric &ex)
			{
				Assert::Fail(char2wchar(ex.what()));
			}			
		}

	};
}