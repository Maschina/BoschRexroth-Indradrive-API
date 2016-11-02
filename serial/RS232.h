//	Serial.h - Definition of the CSerial class
//
//	Copyright (C) 1999-2003 Ramon de Klein (Ramon.de.Klein@ict.nl)
//
// This library is free software; you can redistribute it and/or
// modify it under the terms of the GNU Lesser General Public
// License as published by the Free Software Foundation; either
// version 2.1 of the License, or (at your option) any later version.
// 
// This library is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
// Lesser General Public License for more details.
// 
// You should have received a copy of the GNU Lesser General Public
// License along with this library; if not, write to the Free Software
// Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA


#ifndef _RS232_H
#define _RS232_H


#define STRICT
#include <tchar.h>
#include <windows.h>
#include "WinError.h"
#include <exception>
#include <string>

#include "ftd2xx.h"
#include "helpers.h"
#include "debug.h"



//////////////////////////////////////////////////////////////////////
// The SERIAL_DEFAULT_OVERLAPPED defines if the default open mode uses overlapped I/O. When
// overlapped I/O is available (normal Win32 platforms) it uses overlapped I/O. Windows CE doesn't
// allow the use of overlapped I/O, so it is disabled there by default. 
//////////////////////////////////////////////////////////////////////
#ifndef SERIAL_DEFAULT_OVERLAPPED
#ifndef SERIAL_NO_OVERLAPPED
#define SERIAL_DEFAULT_OVERLAPPED	true
#else
#define SERIAL_DEFAULT_OVERLAPPED	false
#endif
#endif

///=================================================================================================
/// <summary>
/// CSerial - Win32 wrapper for serial communications
/// 
/// Serial communication often causes a lot of problems. This class tries to supply an easy to
/// use interface to deal with serial devices.
/// 
/// The class is actually pretty ease to use. You only need to open the COM-port, where you need
/// to specify the basic serial communication parameters. You can also choose to setup
/// handshaking and read timeout behaviour.
/// 
/// The following serial classes are available:
/// 
/// CSerial      - Serial communication support. CSerialEx    - Serial communication with
/// listener thread for events CSerialSync  - Serial communication with synchronized event
/// handler CSerialWnd   - Asynchronous serial support, which uses the Win32
///                message queue for event notification.
/// CSerialMFC   - Preferred class to use in MFC-based GUI windows.
/// 
/// 
/// Pros:
/// -----
///    - Easy to use (hides a lot of nasty Win32 stuff)
///    - Fully ANSI and Unicode aware
/// 
/// Cons:
/// -----
///  - Little less flexibility then native Win32 API, however you can
///    use this API at the same time for features which are missing from this class.
///  - Incompatible with Windows 95 or Windows NT v3.51 (or earlier),
///    because CancelIo isn't support on these platforms. Define the
///      SERIAL_NO_CANCELIO macro for support of these platforms as well. When this macro is
///      defined, then only time-out values of 0 or INFINITE are valid.
/// 
/// 
/// Copyright (C) 1999-2003 Ramon de Klein
///                         (Ramon.de.Klein@ict.nl)
/// </summary>
///=================================================================================================
class CSerial
{
// Class enumerations
public:
	/// <summary>	Communication event. </summary>
	typedef enum
	{
		EEventUnknown  	   = -1,			// Unknown event
		EEventNone  	   = 0,				// Event trigged without cause
		EEventBreak 	   = EV_BREAK,		// A break was detected on input
		EEventCTS   	   = EV_CTS,		// The CTS signal changed state
		EEventDSR   	   = EV_DSR,		// The DSR signal changed state
		EEventError 	   = EV_ERR,		// A line-status error occurred
		EEventRing  	   = EV_RING,		// A ring indicator was detected
		EEventRLSD  	   = EV_RLSD,		// The RLSD signal changed state
		EEventRecv  	   = EV_RXCHAR,		// Data is received on input
		EEventRcvEv 	   = EV_RXFLAG,		// Event character was received on input
		EEventSend		   = EV_TXEMPTY,	// Last character on output was sent
		EEventPrinterError = EV_PERR,		// Printer error occurred
		EEventRx80Full	   = EV_RX80FULL,	// Receive buffer is 80 percent full
		EEventProviderEvt1 = EV_EVENT1,		// Provider specific event 1
		EEventProviderEvt2 = EV_EVENT2,		// Provider specific event 2
	} 
	EEvent;

	/// <summary>	Baudrate. </summary>
	typedef enum
	{
		EBaudUnknown = -1,			// Unknown
		EBaud110     = CBR_110,		// 110 bits/sec
		EBaud300     = CBR_300,		// 300 bits/sec
		EBaud600     = CBR_600,		// 600 bits/sec
		EBaud1200    = CBR_1200,	// 1200 bits/sec
		EBaud2400    = CBR_2400,	// 2400 bits/sec
		EBaud4800    = CBR_4800,	// 4800 bits/sec
		EBaud9600    = CBR_9600,	// 9600 bits/sec
		EBaud14400   = CBR_14400,	// 14400 bits/sec
		EBaud19200   = CBR_19200,	// 19200 bits/sec (default)
		EBaud38400   = CBR_38400,	// 38400 bits/sec
		EBaud56000   = CBR_56000,	// 56000 bits/sec
		EBaud57600   = CBR_57600,	// 57600 bits/sec
		EBaud115200  = CBR_115200,	// 115200 bits/sec
		EBaud128000  = CBR_128000,	// 128000 bits/sec
		EBaud256000  = CBR_256000,	// 256000 bits/sec
	}
	EBaudrate;

	/// <summary>	Data bits (5-8) </summary>
	typedef enum
	{
		EDataUnknown = -1,			// Unknown
		EData5       =  5,			// 5 bits per byte
		EData6       =  6,			// 6 bits per byte
		EData7       =  7,			// 7 bits per byte
		EData8       =  8			// 8 bits per byte (default)
	}
	EDataBits;

	/// <summary>	Parity scheme. </summary>
	typedef enum
	{
		EParUnknown = -1,			// Unknown
		EParNone    = NOPARITY,		// No parity (default)
		EParOdd     = ODDPARITY,	// Odd parity
		EParEven    = EVENPARITY,	// Even parity
		EParMark    = MARKPARITY,	// Mark parity
		EParSpace   = SPACEPARITY	// Space parity
	}
	EParity;

	/// <summary>	Stop bits. </summary>
	typedef enum
	{
		EStopUnknown = -1,			// Unknown
		EStop1       = ONESTOPBIT,	// 1 stopbit (default)
		EStop1_5     = ONE5STOPBITS,// 1.5 stopbit
		EStop2       = TWOSTOPBITS	// 2 stopbits
	} 
	EStopBits;

	/// <summary>	Handshaking. </summary>
	typedef enum
	{
		EHandshakeUnknown		= -1,	// Unknown
		EHandshakeOff			=  0,	// No handshaking
		EHandshakeHardware		=  1,	// Hardware handshaking (RTS/CTS)
		EHandshakeSoftware		=  2	// Software handshaking (XON/XOFF)
	} 
	EHandshake;

	/// <summary>	Timeout settings. </summary>
	typedef enum
	{
		EReadTimeoutUnknown		= -1,	// Unknown
		EReadTimeoutNonblocking	=  0,	// Always return immediately
		EReadTimeoutBlocking	=  1	// Block until everything is retrieved
	}
	EReadTimeout;

	/// <summary>	Communication errors. </summary>
	typedef enum
	{
		EErrorUnknown = 0,			// Unknown
		EErrorBreak   = CE_BREAK,	// Break condition detected
		EErrorFrame   = CE_FRAME,	// Framing error
		EErrorIOE     = CE_IOE,		// I/O device error
		EErrorMode    = CE_MODE,	// Unsupported mode
		EErrorOverrun = CE_OVERRUN,	// Character buffer overrun, next byte is lost
		EErrorRxOver  = CE_RXOVER,	// Input buffer overflow, byte lost
		EErrorParity  = CE_RXPARITY,// Input parity error
		EErrorTxFull  = CE_TXFULL	// Output buffer full
	}
	EError;

	/// <summary>	Port availability. </summary>
	typedef enum
	{
		EPortUnknownError = -1,		// Unknown error occurred
		EPortAvailable    =  0,		// Port is available
		EPortNotAvailable =  1,		// Port is not present
		EPortInUse        =  2		// Port is in use

	} 
	EPort;

	/// <summary>	Generic exception handling for Serial communication. </summary>
	class ExceptionGeneric;
	/// <summary>	Specific exception handling of Serial communication for failed reception. </summary>
	class ExceptionReceptionFailed;

public:
	/// <summary>	Default constructor. </summary>
	CSerial();
	/// <summary>	Destructor. </summary>
	virtual ~CSerial();


public:

	///=================================================================================================
	/// <summary>	Check if particular COM-port is available (static method). </summary>
	///
	/// <param name="lpszDevice">	(Optional) COM port, stated as char*. Keep in mind to state ports higher than COM9 as
	/// 							the following: "\\\\.\\COMx". </param>
	///
	/// <returns>	Port availability. </returns>
	///=================================================================================================
	static void CheckPort (LPCTSTR lpszDevice = _T("COM1"));

	///=================================================================================================
	/// <summary>
	/// Open the serial communications for a particular COM port. You need to use the full devicename
	/// (i.e. "COM1") to open the port. It's possible to specify the size of the input/output queues.
	/// </summary>
	///
	/// <returns>	A LONG. </returns>
	///
	/// <param name="lpszDevice"> 	(Optional) COM port, stated as char*. Keep in mind to state ports
	/// 							higher than COM9 as the following: "\\\\.\\COMx". </param>
	/// <param name="dwInQueue">  	(Optional) queue of INS. Make sure that this value is neither
	/// 							below 16 or above 254. </param>
	/// <param name="dwOutQueue"> 	(Optional) queue of outs. Make sure that this value is neither
	/// 							below 16 ore above 254. </param>
	/// <param name="fOverlapped">	(Optional) true if overlapped. </param>
	///=================================================================================================
	virtual void Open (LPCTSTR lpszDevice = _T("COM1"), DWORD dwInQueue = 0, DWORD dwOutQueue = 0, bool fOverlapped = SERIAL_DEFAULT_OVERLAPPED);

	///=================================================================================================
	/// <summary>	Close the serial port. </summary>
	///
	/// <returns>	A LONG. </returns>
	///=================================================================================================
	virtual LONG Close (void);

	///=================================================================================================
	/// <summary>
	/// Setup the communication settings such as baudrate, databits, parity and stopbits. The default
	/// settings are applied when the device has been opened. Call this function if these settings do
	/// not apply for your application. If you prefer to use integers instead of the enumerated types
	/// then just cast the integer to the required type. So the following two initializations are
	/// equivalent:
	/// 
	///   Setup(EBaud9600,EData8,EParNone,EStop1)
	/// 
	/// or
	/// 
	///   Setup(EBaudrate(9600),EDataBits(8),EParity(NOPARITY),EStopBits(ONESTOPBIT))
	/// 
	/// In the latter case, the types are not validated. So make sure that you specify the
	/// appropriate values.
	/// </summary>
	///
	/// <param name="eBaudrate">	(Optional) the baudrate. </param>
	/// <param name="eDataBits">	(Optional) the data bits. </param>
	/// <param name="eParity">  	(Optional) the parity. </param>
	/// <param name="eStopBits">	(Optional) the stop bits. </param>
	///
	/// <returns>	A LONG. </returns>
	///=================================================================================================
	virtual LONG Setup (EBaudrate eBaudrate = EBaud9600,
						EDataBits eDataBits = EData8,
						EParity   eParity   = EParNone,
						EStopBits eStopBits = EStop1);

	///=================================================================================================
	/// <summary>
	/// Set/clear the event character. When this byte is being received on the serial port then the EEventRcvEv event is
	/// signaled, when the mask has been set appropriately. If the fAdjustMask flag has been set, then the event mask is
	/// automatically adjusted.
	/// </summary>
	///
	/// <param name="bEventChar"> 	The event character. </param>
	/// <param name="fAdjustMask">	(Optional) true if event mask has to be automatically adjusted. </param>
	///
	/// <returns>	A LONG. </returns>
	///=================================================================================================
	virtual LONG SetEventChar (BYTE bEventChar, bool fAdjustMask = true);

	///=================================================================================================
	/// <summary>
	/// Set the event mask, which indicates what events should be monitored. The WaitEvent method can
	/// only monitor events that have been enabled. The default setting only monitors the error
	/// events and data events. An application may choose to monitor CTS. DSR, RLSD, etc as well.
	/// </summary>
	///
	/// <param name="dwMask">	(Optional) the mask. </param>
	///
	/// <returns>	A LONG. </returns>
	///=================================================================================================
	virtual LONG SetMask (DWORD dwMask = EEventBreak|EEventError|EEventRecv);

	///=================================================================================================
	/// <summary>
	/// The WaitEvent method waits for one of the events that are enabled (see SetMask).
	/// </summary>
	///
	/// <param name="lpOverlapped">	(Optional) the overlapped. </param>
	/// <param name="dwTimeout">   	(Optional) the timeout. </param>
	///
	/// <returns>	A LONG. </returns>
	///=================================================================================================
	virtual LONG WaitEvent (LPOVERLAPPED lpOverlapped = 0, DWORD dwTimeout = INFINITE);

	///=================================================================================================
	/// <summary>
	/// Setup the handshaking protocol. There are three forms of handshaking:
	/// 
	/// 1) No handshaking, so data is always send even if the receiver
	///    cannot handle the data anymore. This can lead to data loss, when the sender is able to
	///    transmit data faster then the receiver can handle.
	/// 2) Hardware handshaking, where the RTS/CTS lines are used to
	///    indicate if data can be sent. This mode requires that both ports and the cable support
	///    hardware handshaking. Hardware handshaking is the most reliable and efficient form of
	///    handshaking available, but is hardware dependant.
	/// 3) Software handshaking, where the XON/XOFF characters are used
	///    to throttle the data. A major drawback of this method is that these characters cannot be
	///    used for data anymore.
	/// </summary>
	///
	/// <param name="eHandshake">	The handshake. </param>
	///
	/// <returns>	A LONG. </returns>
	///=================================================================================================
	virtual LONG SetupHandshaking (EHandshake eHandshake);

	///=================================================================================================
	/// <summary>
	/// Read operations can be blocking or non-blocking. You can use this method to setup wether to
	/// use blocking or non-blocking reads. Non-blocking reads is the default, which is required for
	/// most applications.
	/// 
	/// 1) Blocking reads, which will cause the 'Read' method to block
	///    until the requested number of bytes have been read. This is useful if you know how many
	///    data you will receive.
	/// 2) Non-blocking reads, which will read as many bytes into your
	///    buffer and returns almost immediately. This is often the preferred setting.
	/// </summary>
	///
	/// <param name="eReadTimeout">	The read timeout. </param>
	///
	/// <returns>	A LONG. </returns>
	///=================================================================================================
	virtual LONG SetupReadTimeouts (EReadTimeout eReadTimeout);

	virtual EBaudrate  GetBaudrate    (void);
	virtual EDataBits  GetDataBits    (void);
	virtual EParity    GetParity      (void);
	virtual EStopBits  GetStopBits    (void);
	virtual EHandshake GetHandshaking (void);
	virtual DWORD      GetEventMask   (void);
	virtual BYTE       GetEventChar   (void);

	///=================================================================================================
	/// <summary>
	/// Write data to the serial port. Note that we are only able to send ANSI strings, because it
	/// probably doesn't make sense to transmit Unicode strings to an application.
	/// </summary>
	///
	/// <param name="pData">	   	The data. </param>
	/// <param name="iLen">		   	Zero-based index of the length. </param>
	/// <param name="pdwWritten">  	[in,out] (Optional) If non-null, the pdw written. </param>
	/// <param name="lpOverlapped">	(Optional) the overlapped. </param>
	/// <param name="dwTimeout">   	(Optional) the timeout. </param>
	///
	/// <returns>	A LONG. </returns>
	///=================================================================================================
	virtual LONG Write (const void* pData, size_t iLen, DWORD* pdwWritten = 0, LPOVERLAPPED lpOverlapped = 0, DWORD dwTimeout = INFINITE);

	///=================================================================================================
	/// <summary>	Writes. </summary>
	///
	/// <param name="pString">	   	The string. </param>
	/// <param name="pdwWritten">  	[in,out] (Optional) If non-null, the pdw written. </param>
	/// <param name="lpOverlapped">	(Optional) the overlapped. </param>
	/// <param name="dwTimeout">   	(Optional) the timeout. </param>
	///
	/// <returns>	A LONG. </returns>
	///=================================================================================================
	virtual LONG Write (LPCSTR pString, DWORD* pdwWritten = 0, LPOVERLAPPED lpOverlapped = 0, DWORD dwTimeout = INFINITE);

	///=================================================================================================
	/// <summary>
	/// Read data from the serial port. Refer to the description of the 'SetupReadTimeouts' for an
	/// explanation about (non) blocking reads and how to use this.
	/// </summary>
	///
	/// <param name="pData">	   	[in,out] If non-null, the data. </param>
	/// <param name="iLen">		   	Zero-based index of the length. </param>
	/// <param name="pdwRead">	   	[in,out] (Optional) If non-null, the pdw read. </param>
	/// <param name="lpOverlapped">	(Optional) the overlapped. </param>
	/// <param name="dwTimeout">   	(Optional) the timeout in milliseconds. </param>
	///
	/// <returns>	A LONG. </returns>
	///=================================================================================================
	virtual LONG Read (void* pData, size_t iLen, DWORD* pdwRead = 0, LPOVERLAPPED lpOverlapped = 0, DWORD dwTimeout = INFINITE);

	///=================================================================================================
	/// <summary>	Send a break. </summary>
	///
	/// <returns>	A LONG. </returns>
	///=================================================================================================
	LONG Break (void);

	///=================================================================================================
	/// <summary>	Determine what caused the event to trigger. </summary>
	///
	/// <returns>	The event type. </returns>
	///=================================================================================================
	EEvent GetEventType (void);

	///=================================================================================================
	/// <summary>	Obtain the error. </summary>
	///
	/// <returns>	The error. </returns>
	///=================================================================================================
	EError GetError (void);

	///=================================================================================================
	/// <summary>	Obtain the COMM and event handle. </summary>
	///
	/// <returns>	The communications handle. </returns>
	///=================================================================================================
	HANDLE GetCommHandle (void)		{ return m_hFile; }

	///=================================================================================================
	/// <summary>	Check if com-port is opened. </summary>
	///
	/// <returns>	true if open, false if not. </returns>
	///=================================================================================================
	bool IsOpen (void) const		{ return (m_hFile != 0); }

	///=================================================================================================
	/// <summary>	Obtain last error status. </summary>
	///
	/// <returns>	The last error. </returns>
	///=================================================================================================
	LONG GetLastError (void) const	{ return m_lLastError; }

	// Obtain CTS/DSR/RING/RLSD settings
	bool GetCTS (void);
	bool GetDSR (void);
	bool GetRing (void);
	bool GetRLSD (void);

	///=================================================================================================
	/// <summary>	Purge all buffers. </summary>
	///
	/// <returns>	A LONG. </returns>
	///=================================================================================================
	LONG Purge (void);

protected:
	/// <summary>	Internal helper class which wraps DCB structure. </summary>
	class CDCB : public DCB
	{
	public:
		CDCB() { DCBlength = sizeof(DCB); }
	};

// Attributes
protected:
	/// <summary>	Last serial error. </summary>
	LONG	m_lLastError;
	/// <summary>	File handle. </summary>
	HANDLE	m_hFile;
	/// <summary>	Event type. </summary>
	EEvent	m_eEvent;
	/// <summary>	Event mask. </summary>
	DWORD	m_dwEventMask;

#ifndef SERIAL_NO_OVERLAPPED
	/// <summary>	Event handle for internal overlapped operations. </summary>
	HANDLE	m_hevtOverlapped;
#endif

protected:

	///=================================================================================================
	/// <summary>	Check the requirements. </summary>
	///
	/// <param name="lpOverlapped">	The overlapped. </param>
	/// <param name="dwTimeout">   	The timeout. </param>
	///=================================================================================================
	void CheckRequirements (LPOVERLAPPED lpOverlapped, DWORD dwTimeout) const;

	///=================================================================================================
	/// <summary>	CancelIo wrapper (for Win95 compatibility) </summary>
	///
	/// <returns>	true if it succeeds, false if it fails. </returns>
	///=================================================================================================
	BOOL CancelCommIo (void);
};


/// <summary>	Generic exceptions for Serial communication. </summary>
class CSerial::ExceptionGeneric : public std::exception
{
public:
	bool warning;

	ExceptionGeneric(
		int _status,
		const std::string _message,
		bool _warning = false) :

		m_status(_status),
		m_message(_message),
		warning(_warning)
	{}

	virtual const char* what() const throw ()
	{
#ifdef NDEBUG
		return str2char(sformat("CSerial exception caused %s: STATUS=%d, MESSAGE='%s'", Stack::GetTraceString().c_str(), m_status, m_message.c_str()));
#else
		std::string foo = stde::GetWinErrorString(m_status);
		const char* ex = str2char(sformat("CSerial exception caused: %s ### STATUS=0x%04x (%s) ### MESSAGE='%s'", Stack::GetTraceString().c_str(), m_status, foo.c_str(), m_message.c_str()));
		OutputDebugStringA((LPCSTR)ex);
		return ex;
#endif
	}

	int get_status() { return m_status; }

	///=================================================================================================
	/// <summary>
	/// For Win32 API commands, most likely representation of the System Error Codes:
	/// # https://msdn.microsoft.com/de-de/library/windows/desktop/ms681381(v=vs.85).aspx.
	/// </summary>
	///=================================================================================================
	int m_status;

	std::string m_message;
};


/// <summary>	Specific exception handling of Serial communication for failed reception. </summary>
class CSerial::ExceptionReceptionFailed : public CSerial::ExceptionGeneric
{
public:
	ExceptionReceptionFailed(
		int _status,
		const std::string _message,
		bool _warning = false) :

		ExceptionGeneric(_status, _message, _warning)
	{}
	~ExceptionReceptionFailed() throw() {}

	virtual const char* what() const throw ()
	{
#ifdef NDEBUG
		return str2char(sformat("CSerial reception fail caused: STATUS=0x%04x ### MESSAGE='%s'", m_status, m_message.c_str()));
#else
		std::string errstr = stde::GetWinErrorString(m_status);
		const char* ex = str2char(sformat("CSerial reception fail caused: STATUS=0x%04x (%s) ### MESSAGE='%s'", m_status, errstr.c_str(), m_message.c_str()));
		OutputDebugStringA((LPCSTR)ex);
		return ex;
#endif
	}
};

#endif	// _RS232_H
