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

				sis.close();
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

		TEST_METHOD(SIS_SINGLEPARAMETER_READ)
		{
			SISProtocol sis;

			try
			{
				sis.open("COM1");
				
				std::vector<BYTE> rcvddata;
				sis.read_parameter(TGM::SERCOS_Param_S, 36, rcvddata);

				sis.close();
			}
			catch (SISProtocol::ExceptionTransceiveFailed &ex)
			{
				Logger::WriteMessage(ex.what());
				Assert::Fail(char2wchar(ex.what()));
			}
			catch (SISProtocol::ExceptionGeneric &ex)
			{
				Logger::WriteMessage(ex.what());
				Assert::Fail(char2wchar(ex.what()));
			}
			catch (CSerial::ExceptionReceptionFailed &ex)
			{
				Logger::WriteMessage(ex.what());
				Assert::Fail(char2wchar(ex.what()));
			}
			catch (CSerial::ExceptionGeneric &ex)
			{
				Logger::WriteMessage(ex.what());
				Assert::Fail(char2wchar(ex.what()));
			}
		}

		TEST_METHOD(SIS_SINGLEPARAMETER_WRITE)
		{
			SISProtocol sis;

			try
			{
				sis.open("COM1");

				BYTE value = 77;

				std::vector<BYTE> data;
				data.push_back(value);
				data.push_back(0);
				data.push_back(0);
				data.push_back(0);
				sis.write_parameter(TGM::SERCOS_Param_S, 36, data);

				std::vector<BYTE> rcvddata;
				sis.read_parameter(TGM::SERCOS_Param_S, 36, rcvddata);

				sis.close();

				Assert::AreEqual<BYTE>(value, rcvddata.at(0));
			}
			catch (SISProtocol::ExceptionTransceiveFailed &ex)
			{
				Logger::WriteMessage(ex.what());
				Assert::Fail(char2wchar(ex.what()));
			}
			catch (SISProtocol::ExceptionGeneric &ex)
			{
				Logger::WriteMessage(ex.what());
				Assert::Fail(char2wchar(ex.what()));
			}
			catch (CSerial::ExceptionReceptionFailed &ex)
			{
				Logger::WriteMessage(ex.what());
				Assert::Fail(char2wchar(ex.what()));
			}
			catch (CSerial::ExceptionGeneric &ex)
			{
				Logger::WriteMessage(ex.what());
				Assert::Fail(char2wchar(ex.what()));
			}
		}

		TEST_METHOD(SIS_LISTPARAMETER_READ)
		{
			SISProtocol sis;

			try
			{
				sis.open("COM1");

				std::vector<BYTE> rcvddata;
				sis.read_listelm(TGM::SERCOS_Param_P, 4007, 1*4, 4, rcvddata);

				sis.close();
			}
			catch (SISProtocol::ExceptionTransceiveFailed &ex)
			{
				Logger::WriteMessage(ex.what());
				Assert::Fail(char2wchar(ex.what()));
			}
			catch (SISProtocol::ExceptionGeneric &ex)
			{
				Logger::WriteMessage(ex.what());
				Assert::Fail(char2wchar(ex.what()));
			}
			catch (CSerial::ExceptionReceptionFailed &ex)
			{
				Logger::WriteMessage(ex.what());
				Assert::Fail(char2wchar(ex.what()));
			}
			catch (CSerial::ExceptionGeneric &ex)
			{
				Logger::WriteMessage(ex.what());
				Assert::Fail(char2wchar(ex.what()));
			}
		}
	};
}