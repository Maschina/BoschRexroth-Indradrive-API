#include "stdafx.h"
#include "CppUnitTest.h"

#include "RS232.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;


namespace RS232Test
{		
	TEST_CLASS(utRS232)
	{
	public:

		LPCTSTR cport = _T("COM1");
		CSerial::EBaudrate cbaudrate = CSerial::EBaud115200;
		CSerial::EDataBits cdata = CSerial::EData8;
		CSerial::EParity cparity = CSerial::EParEven;
		CSerial::EStopBits cstopbits = CSerial::EStop1;
		CSerial::EHandshake chandshake = CSerial::EHandshakeSoftware;
		
		TEST_METHOD(RS232_CONNECT)
		{
			CSerial serial;

			try
			{
				//Assert::AreEqual<int>(CSerial::EPortAvailable, CSerial::CheckPort(cport));

				serial.Open(cport, 0, 0, false /* no overlapped */);

				serial.Setup(cbaudrate, cdata, cparity, cstopbits);
				serial.SetupHandshaking(chandshake);

				serial.Close();
			}
			catch (CSerial::ExceptionGeneric &ex)
			{
				Assert::Fail(char2wchar(ex.what()));
			}
		}
	};
}