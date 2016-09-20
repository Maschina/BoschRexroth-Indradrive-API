<?xml version="1.0"?><doc>
<members>
<member name="T:CSerial" decl="false" source="c:\ae\alternator_testbench\software\api-hardware\indradrive\lib\api\serial\rs232.h" line="50">
=================================================================================================
<summary>
CSerial - Win32 wrapper for serial communications

Serial communication often causes a lot of problems. This class tries to supply an easy to
use interface to deal with serial devices.

The class is actually pretty ease to use. You only need to open the COM-port, where you need
to specify the basic serial communication parameters. You can also choose to setup
handshaking and read timeout behaviour.

The following serial classes are available:

CSerial      - Serial communication support. CSerialEx    - Serial communication with
listener thread for events CSerialSync  - Serial communication with synchronized event
handler CSerialWnd   - Asynchronous serial support, which uses the Win32
               message queue for event notification.
CSerialMFC   - Preferred class to use in MFC-based GUI windows.


Pros:
-----
   - Easy to use (hides a lot of nasty Win32 stuff)
   - Fully ANSI and Unicode aware

Cons:
-----
 - Little less flexibility then native Win32 API, however you can
   use this API at the same time for features which are missing from this class.
 - Incompatible with Windows 95 or Windows NT v3.51 (or earlier),
   because CancelIo isn't support on these platforms. Define the
     SERIAL_NO_CANCELIO macro for support of these platforms as well. When this macro is
     defined, then only time-out values of 0 or INFINITE are valid.


Copyright (C) 1999-2003 Ramon de Klein
                        (Ramon.de.Klein@ict.nl)
</summary>
=================================================================================================
</member>
<member name="T:CSerial.ExceptionGeneric" decl="true" source="c:\ae\alternator_testbench\software\api-hardware\indradrive\lib\api\serial\rs232.h" line="214">
<summary>	Generic exception handling for Serial communication. </summary>
</member>
<member name="T:CSerial.ExceptionReceptionFailed" decl="true" source="c:\ae\alternator_testbench\software\api-hardware\indradrive\lib\api\serial\rs232.h" line="216">
<summary>	Specific exception handling of Serial communication for failed reception. </summary>
</member>
<member name="M:CSerial.#ctor" decl="true" source="c:\ae\alternator_testbench\software\api-hardware\indradrive\lib\api\serial\rs232.h" line="220">
<summary>	Default constructor. </summary>
</member>
<member name="M:CSerial.Dispose" decl="true" source="c:\ae\alternator_testbench\software\api-hardware\indradrive\lib\api\serial\rs232.h" line="222">
<summary>	Destructor. </summary>
</member>
<member name="M:CSerial.CheckPort(System.SByte!System.Runtime.CompilerServices.IsSignUnspecifiedByte!System.Runtime.CompilerServices.IsConst*)" decl="true" source="c:\ae\alternator_testbench\software\api-hardware\indradrive\lib\api\serial\rs232.h" line="228">
=================================================================================================
<summary>	Check if particular COM-port is available (static method). </summary>

<param name="lpszDevice">	(Optional) COM port, stated as char*. Keep in mind to state ports higher than COM9 as
							the following: "\\\\.\\COMx". </param>

<returns>	Port availability. </returns>
=================================================================================================
</member>
<member name="M:CSerial.Open(System.SByte!System.Runtime.CompilerServices.IsSignUnspecifiedByte!System.Runtime.CompilerServices.IsConst*,System.UInt32!System.Runtime.CompilerServices.IsLong,System.UInt32!System.Runtime.CompilerServices.IsLong,System.Boolean)" decl="true" source="c:\ae\alternator_testbench\software\api-hardware\indradrive\lib\api\serial\rs232.h" line="238">
=================================================================================================
<summary>
Open the serial communications for a particular COM port. You need to use the full devicename
(i.e. "COM1") to open the port. It's possible to specify the size of the input/output queues.
</summary>

<returns>	A LONG. </returns>

<param name="lpszDevice"> 	(Optional) COM port, stated as char*. Keep in mind to state ports
							higher than COM9 as the following: "\\\\.\\COMx". </param>
<param name="dwInQueue">  	(Optional) queue of INS. Make sure that this value is neither
							below 16 or above 254. </param>
<param name="dwOutQueue"> 	(Optional) queue of outs. Make sure that this value is neither
							below 16 ore above 254. </param>
<param name="fOverlapped">	(Optional) true if overlapped. </param>
=================================================================================================
</member>
<member name="M:CSerial.Close" decl="true" source="c:\ae\alternator_testbench\software\api-hardware\indradrive\lib\api\serial\rs232.h" line="256">
=================================================================================================
<summary>	Close the serial port. </summary>

<returns>	A LONG. </returns>
=================================================================================================
</member>
<member name="M:CSerial.Setup(CSerial.EBaudrate,CSerial.EDataBits,CSerial.EParity,CSerial.EStopBits)" decl="true" source="c:\ae\alternator_testbench\software\api-hardware\indradrive\lib\api\serial\rs232.h" line="263">
=================================================================================================
<summary>
Setup the communication settings such as baudrate, databits, parity and stopbits. The default
settings are applied when the device has been opened. Call this function if these settings do
not apply for your application. If you prefer to use integers instead of the enumerated types
then just cast the integer to the required type. So the following two initializations are
equivalent:

  Setup(EBaud9600,EData8,EParNone,EStop1)

or

  Setup(EBaudrate(9600),EDataBits(8),EParity(NOPARITY),EStopBits(ONESTOPBIT))

In the latter case, the types are not validated. So make sure that you specify the
appropriate values.
</summary>

<param name="eBaudrate">	(Optional) the baudrate. </param>
<param name="eDataBits">	(Optional) the data bits. </param>
<param name="eParity">  	(Optional) the parity. </param>
<param name="eStopBits">	(Optional) the stop bits. </param>

<returns>	A LONG. </returns>
=================================================================================================
</member>
<member name="M:CSerial.SetEventChar(System.Byte,System.Boolean)" decl="true" source="c:\ae\alternator_testbench\software\api-hardware\indradrive\lib\api\serial\rs232.h" line="293">
=================================================================================================
<summary>
Set/clear the event character. When this byte is being received on the serial port then the EEventRcvEv event is
signaled, when the mask has been set appropriately. If the fAdjustMask flag has been set, then the event mask is
automatically adjusted.
</summary>

<param name="bEventChar"> 	The event character. </param>
<param name="fAdjustMask">	(Optional) true if event mask has to be automatically adjusted. </param>

<returns>	A LONG. </returns>
=================================================================================================
</member>
<member name="M:CSerial.SetMask(System.UInt32!System.Runtime.CompilerServices.IsLong)" decl="true" source="c:\ae\alternator_testbench\software\api-hardware\indradrive\lib\api\serial\rs232.h" line="307">
=================================================================================================
<summary>
Set the event mask, which indicates what events should be monitored. The WaitEvent method can
only monitor events that have been enabled. The default setting only monitors the error
events and data events. An application may choose to monitor CTS. DSR, RLSD, etc as well.
</summary>

<param name="dwMask">	(Optional) the mask. </param>

<returns>	A LONG. </returns>
=================================================================================================
</member>
<member name="M:CSerial.WaitEvent(_OVERLAPPED*,System.UInt32!System.Runtime.CompilerServices.IsLong)" decl="true" source="c:\ae\alternator_testbench\software\api-hardware\indradrive\lib\api\serial\rs232.h" line="320">
=================================================================================================
<summary>
The WaitEvent method waits for one of the events that are enabled (see SetMask).
</summary>

<param name="lpOverlapped">	(Optional) the overlapped. </param>
<param name="dwTimeout">   	(Optional) the timeout. </param>

<returns>	A LONG. </returns>
=================================================================================================
</member>
<member name="M:CSerial.SetupHandshaking(CSerial.EHandshake)" decl="true" source="c:\ae\alternator_testbench\software\api-hardware\indradrive\lib\api\serial\rs232.h" line="332">
=================================================================================================
<summary>
Setup the handshaking protocol. There are three forms of handshaking:

1) No handshaking, so data is always send even if the receiver
   cannot handle the data anymore. This can lead to data loss, when the sender is able to
   transmit data faster then the receiver can handle.
2) Hardware handshaking, where the RTS/CTS lines are used to
   indicate if data can be sent. This mode requires that both ports and the cable support
   hardware handshaking. Hardware handshaking is the most reliable and efficient form of
   handshaking available, but is hardware dependant.
3) Software handshaking, where the XON/XOFF characters are used
   to throttle the data. A major drawback of this method is that these characters cannot be
   used for data anymore.
</summary>

<param name="eHandshake">	The handshake. </param>

<returns>	A LONG. </returns>
=================================================================================================
</member>
<member name="M:CSerial.SetupReadTimeouts(CSerial.EReadTimeout)" decl="true" source="c:\ae\alternator_testbench\software\api-hardware\indradrive\lib\api\serial\rs232.h" line="354">
=================================================================================================
<summary>
Read operations can be blocking or non-blocking. You can use this method to setup wether to
use blocking or non-blocking reads. Non-blocking reads is the default, which is required for
most applications.

1) Blocking reads, which will cause the 'Read' method to block
   until the requested number of bytes have been read. This is useful if you know how many
   data you will receive.
2) Non-blocking reads, which will read as many bytes into your
   buffer and returns almost immediately. This is often the preferred setting.
</summary>

<param name="eReadTimeout">	The read timeout. </param>

<returns>	A LONG. </returns>
=================================================================================================
</member>
<member name="M:CSerial.Write(System.Void!System.Runtime.CompilerServices.IsConst*,System.UInt32,System.UInt32!System.Runtime.CompilerServices.IsLong*,_OVERLAPPED*,System.UInt32!System.Runtime.CompilerServices.IsLong)" decl="true" source="c:\ae\alternator_testbench\software\api-hardware\indradrive\lib\api\serial\rs232.h" line="381">
=================================================================================================
<summary>
Write data to the serial port. Note that we are only able to send ANSI strings, because it
probably doesn't make sense to transmit Unicode strings to an application.
</summary>

<param name="pData">	   	The data. </param>
<param name="iLen">		   	Zero-based index of the length. </param>
<param name="pdwWritten">  	[in,out] (Optional) If non-null, the pdw written. </param>
<param name="lpOverlapped">	(Optional) the overlapped. </param>
<param name="dwTimeout">   	(Optional) the timeout. </param>

<returns>	A LONG. </returns>
=================================================================================================
</member>
<member name="M:CSerial.Write(System.SByte!System.Runtime.CompilerServices.IsSignUnspecifiedByte!System.Runtime.CompilerServices.IsConst*,System.UInt32!System.Runtime.CompilerServices.IsLong*,_OVERLAPPED*,System.UInt32!System.Runtime.CompilerServices.IsLong)" decl="true" source="c:\ae\alternator_testbench\software\api-hardware\indradrive\lib\api\serial\rs232.h" line="397">
=================================================================================================
<summary>	Writes. </summary>

<param name="pString">	   	The string. </param>
<param name="pdwWritten">  	[in,out] (Optional) If non-null, the pdw written. </param>
<param name="lpOverlapped">	(Optional) the overlapped. </param>
<param name="dwTimeout">   	(Optional) the timeout. </param>

<returns>	A LONG. </returns>
=================================================================================================
</member>
<member name="M:CSerial.Read(System.Void*,System.UInt32,System.UInt32!System.Runtime.CompilerServices.IsLong*,_OVERLAPPED*,System.UInt32!System.Runtime.CompilerServices.IsLong)" decl="true" source="c:\ae\alternator_testbench\software\api-hardware\indradrive\lib\api\serial\rs232.h" line="409">
=================================================================================================
<summary>
Read data from the serial port. Refer to the description of the 'SetupReadTimeouts' for an
explanation about (non) blocking reads and how to use this.
</summary>

<param name="pData">	   	[in,out] If non-null, the data. </param>
<param name="iLen">		   	Zero-based index of the length. </param>
<param name="pdwRead">	   	[in,out] (Optional) If non-null, the pdw read. </param>
<param name="lpOverlapped">	(Optional) the overlapped. </param>
<param name="dwTimeout">   	(Optional) the timeout in milliseconds. </param>

<returns>	A LONG. </returns>
=================================================================================================
</member>
<member name="M:CSerial.Break" decl="true" source="c:\ae\alternator_testbench\software\api-hardware\indradrive\lib\api\serial\rs232.h" line="425">
=================================================================================================
<summary>	Send a break. </summary>

<returns>	A LONG. </returns>
=================================================================================================
</member>
<member name="M:CSerial.GetEventType" decl="true" source="c:\ae\alternator_testbench\software\api-hardware\indradrive\lib\api\serial\rs232.h" line="432">
=================================================================================================
<summary>	Determine what caused the event to trigger. </summary>

<returns>	The event type. </returns>
=================================================================================================
</member>
<member name="M:CSerial.GetError" decl="true" source="c:\ae\alternator_testbench\software\api-hardware\indradrive\lib\api\serial\rs232.h" line="439">
=================================================================================================
<summary>	Obtain the error. </summary>

<returns>	The error. </returns>
=================================================================================================
</member>
<member name="M:CSerial.GetCommHandle" decl="false" source="c:\ae\alternator_testbench\software\api-hardware\indradrive\lib\api\serial\rs232.h" line="446">
=================================================================================================
<summary>	Obtain the COMM and event handle. </summary>

<returns>	The communications handle. </returns>
=================================================================================================
</member>
<member name="M:CSerial.IsOpen" decl="false" source="c:\ae\alternator_testbench\software\api-hardware\indradrive\lib\api\serial\rs232.h" line="453">
=================================================================================================
<summary>	Check if com-port is opened. </summary>

<returns>	true if open, false if not. </returns>
=================================================================================================
</member>
<member name="M:CSerial.GetLastError" decl="false" source="c:\ae\alternator_testbench\software\api-hardware\indradrive\lib\api\serial\rs232.h" line="460">
=================================================================================================
<summary>	Obtain last error status. </summary>

<returns>	The last error. </returns>
=================================================================================================
</member>
<member name="M:CSerial.Purge" decl="true" source="c:\ae\alternator_testbench\software\api-hardware\indradrive\lib\api\serial\rs232.h" line="473">
=================================================================================================
<summary>	Purge all buffers. </summary>

<returns>	A LONG. </returns>
=================================================================================================
</member>
<member name="T:CSerial.CDCB" decl="false" source="c:\ae\alternator_testbench\software\api-hardware\indradrive\lib\api\serial\rs232.h" line="481">
<summary>	Internal helper class which wraps DCB structure. </summary>
</member>
<member name="F:CSerial.m_lLastError" decl="false" source="c:\ae\alternator_testbench\software\api-hardware\indradrive\lib\api\serial\rs232.h" line="490">
<summary>	Last serial error. </summary>
</member>
<member name="F:CSerial.m_hFile" decl="false" source="c:\ae\alternator_testbench\software\api-hardware\indradrive\lib\api\serial\rs232.h" line="492">
<summary>	File handle. </summary>
</member>
<member name="F:CSerial.m_eEvent" decl="false" source="c:\ae\alternator_testbench\software\api-hardware\indradrive\lib\api\serial\rs232.h" line="494">
<summary>	Event type. </summary>
</member>
<member name="F:CSerial.m_dwEventMask" decl="false" source="c:\ae\alternator_testbench\software\api-hardware\indradrive\lib\api\serial\rs232.h" line="496">
<summary>	Event mask. </summary>
</member>
<member name="F:CSerial.m_hevtOverlapped" decl="false" source="c:\ae\alternator_testbench\software\api-hardware\indradrive\lib\api\serial\rs232.h" line="500">
<summary>	Event handle for internal overlapped operations. </summary>
</member>
<member name="M:CSerial.CheckRequirements(_OVERLAPPED*,System.UInt32!System.Runtime.CompilerServices.IsLong)" decl="true" source="c:\ae\alternator_testbench\software\api-hardware\indradrive\lib\api\serial\rs232.h" line="506">
=================================================================================================
<summary>	Check the requirements. </summary>

<param name="lpOverlapped">	The overlapped. </param>
<param name="dwTimeout">   	The timeout. </param>
=================================================================================================
</member>
<member name="M:CSerial.CancelCommIo" decl="true" source="c:\ae\alternator_testbench\software\api-hardware\indradrive\lib\api\serial\rs232.h" line="514">
=================================================================================================
<summary>	CancelIo wrapper (for Win95 compatibility) </summary>

<returns>	true if it succeeds, false if it fails. </returns>
=================================================================================================
</member>
<member name="T:CSerial.ExceptionGeneric" decl="false" source="c:\ae\alternator_testbench\software\api-hardware\indradrive\lib\api\serial\rs232.h" line="523">
<summary>	Generic exceptions for Serial communication. </summary>
</member>
<member name="F:CSerial.ExceptionGeneric.m_status" decl="false" source="c:\ae\alternator_testbench\software\api-hardware\indradrive\lib\api\serial\rs232.h" line="553">
=================================================================================================
<summary>
For Win32 API commands, most likely representation of the System Error Codes:
# https://msdn.microsoft.com/de-de/library/windows/desktop/ms681381(v=vs.85).aspx.
</summary>
=================================================================================================
</member>
<member name="T:CSerial.ExceptionReceptionFailed" decl="false" source="c:\ae\alternator_testbench\software\api-hardware\indradrive\lib\api\serial\rs232.h" line="565">
<summary>	Specific exception handling of Serial communication for failed reception. </summary>
</member>
</members>
</doc>