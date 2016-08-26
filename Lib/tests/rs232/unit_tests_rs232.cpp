#include "stdafx.h"
#include "CppUnitTest.h"

#include "RS232.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;


namespace RS232Test
{		
	TEST_CLASS(utRS232)
	{
	public:

		LPCTSTR cport = _T("COM8");
		CSerial::EBaudrate cbaudrate = CSerial::EBaud115200;
		CSerial::EDataBits cdata = CSerial::EData8;
		CSerial::EParity cparity = CSerial::EParEven;
		CSerial::EStopBits cstopbits = CSerial::EStop1;
		CSerial::EHandshake chandshake = CSerial::EHandshakeSoftware;
		
		TEST_METHOD(RS232_ConnectionTest)
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


		TEST_METHOD(RS232_SendRecvTest_NoOverlapped)
		{
			CSerial serial;

			try
			{
				//////////////////////////////////////////////////////////////////////////
				/// Open and setup
				serial.Open(cport, 16, 16, false /* no overlapped */);

				serial.Setup(cbaudrate, cdata, cparity, cstopbits);
				serial.SetupHandshaking(chandshake);
				
				serial.SetMask(CSerial::EEventBreak |
					CSerial::EEventCTS |
					CSerial::EEventDSR |
					CSerial::EEventError |
					CSerial::EEventRing |
					CSerial::EEventRLSD |
					CSerial::EEventRecv |
					CSerial::EEventRcvEv );

				serial.SetEventChar(0x11 /* XOFF */);

				serial.SetupReadTimeouts(CSerial::EReadTimeoutNonblocking);

				/// Clear buffers
				serial.Purge();

				//////////////////////////////////////////////////////////////////////////
				/// Write
				Logger::WriteMessage("Send command ... e;s");
				serial.Write("e;s\r\n");

				//////////////////////////////////////////////////////////////////////////
				/// Read
				Logger::WriteMessage("Receive status ... e;s;?");

				bool fContinue = true;
				do
				{
					/// Wait for an event
					serial.WaitEvent();

					/// Save event
					const CSerial::EEvent eEvent = serial.GetEventType();

					/// Handle break event
					if (eEvent & CSerial::EEventBreak)
					{
						Logger::WriteMessage("\n### BREAK received ###\n");
					}

					/// Handle CTS event
					if (eEvent & CSerial::EEventCTS)
					{
						Logger::WriteMessage(str2char(sformat("\n### Clear to send %s ###\n", serial.GetCTS() ? "on" : "off")));
					}

					/// Handle DSR event
					if (eEvent & CSerial::EEventDSR)
					{
						Logger::WriteMessage(str2char(sformat("\n### Data set ready %s ###\n", serial.GetDSR() ? "on" : "off")));
					}

					/// Handle error event
					if (eEvent & CSerial::EEventError)
					{
						Logger::WriteMessage("\n### ERROR: ");
						switch (serial.GetError())
						{
							case CSerial::EErrorBreak:		Logger::WriteMessage("Break condition");			break;
							case CSerial::EErrorFrame:		Logger::WriteMessage("Framing error");			break;
							case CSerial::EErrorIOE:		Logger::WriteMessage("IO device error");			break;
							case CSerial::EErrorMode:		Logger::WriteMessage("Unsupported mode");			break;
							case CSerial::EErrorOverrun:	Logger::WriteMessage("Buffer overrun");			break;
							case CSerial::EErrorRxOver:		Logger::WriteMessage("Input buffer overflow");	break;
							case CSerial::EErrorParity:		Logger::WriteMessage("Input parity error");		break;
							case CSerial::EErrorTxFull:		Logger::WriteMessage("Output buffer full");		break;
							default:						Logger::WriteMessage("Unknown");					break;
						}
						Logger::WriteMessage("###\n");
					}

					/// Handle ring event
					if (eEvent & CSerial::EEventRing)
					{
						Logger::WriteMessage("\n### RING ###\n");
					}

					/// Handle RLSD/CD event
					if (eEvent & CSerial::EEventRLSD)
					{
						Logger::WriteMessage(str2char(sformat("\n### RLSD/CD %s ###\n", serial.GetRLSD() ? "on" : "off")));
					}

					/// Handle data receive event
					if (eEvent & CSerial::EEventRecv)
					{
						// Read data, until there is nothing left
						DWORD dwBytesRead = 0;
						char szBuffer[101];
						do
						{
							// Read data from the COM-port
							serial.Read(szBuffer, sizeof(szBuffer) - 1, &dwBytesRead);

							if (dwBytesRead > 0)
							{
								// Finalize the data, so it is a valid string
								szBuffer[dwBytesRead] = '\0';

								// Display the data
								Logger::WriteMessage(str2char(sformat("%s", szBuffer)));

								// Check if EOF
								//if (strchr(szBuffer, '\n')) fContinue = false;
							}
						} while (dwBytesRead == sizeof(szBuffer) - 1);
					}

					/// Handle event character
					if (eEvent & CSerial::EEventRcvEv)
					{
						Logger::WriteMessage("Event character received!");
						fContinue = false;
					}

				} while (fContinue);

				Logger::WriteMessage("Reception finished ...");

				//////////////////////////////////////////////////////////////////////////
				/// Close
				serial.Close();
			}
			catch (CSerial::ExceptionGeneric &ex)
			{
				Assert::Fail(char2wchar(ex.what()));
			}
		}
	};
}