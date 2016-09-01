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
<member name="T:TGM.Bitfields._header_cntrl_t" decl="false" source="c:\ae\alternator_testbench\software\api-hardware\indradrive\lib\api\sis\telegrams_bitfields.h" line="49">
<summary>	Control byte consisting of several bit fields. Size: 8 bit. </summary>
</member>
<member name="F:TGM.Bitfields._header_cntrl_t.&lt;unnamed-tag&gt;.bits_t.num_sub_addresses" decl="false" source="c:\ae\alternator_testbench\software\api-hardware\indradrive\lib\api\sis\telegrams_bitfields.h" line="56">
=================================================================================================
<summary>
Bit 0-2: Number of sub-addresses in the address block: num_sub_addresses=[0..7].
</summary>
=================================================================================================
</member>
<member name="F:TGM.Bitfields._header_cntrl_t.&lt;unnamed-tag&gt;.bits_t.num_running_tgm" decl="false" source="c:\ae\alternator_testbench\software\api-hardware\indradrive\lib\api\sis\telegrams_bitfields.h" line="63">
<summary>	Bit 3: Running telegram number. 0: not support, 1: additional byte. </summary>
</member>
<member name="F:TGM.Bitfields._header_cntrl_t.&lt;unnamed-tag&gt;.bits_t.type" decl="false" source="c:\ae\alternator_testbench\software\api-hardware\indradrive\lib\api\sis\telegrams_bitfields.h" line="66">
<summary>	Bit 4: Telegram type. 0: Command telegram, 1: Reaction telegram. </summary>
</member>
<member name="F:TGM.Bitfields._header_cntrl_t.&lt;unnamed-tag&gt;.bits_t.status_react_tgm" decl="false" source="c:\ae\alternator_testbench\software\api-hardware\indradrive\lib\api\sis\telegrams_bitfields.h" line="69">
=================================================================================================
<summary>
Bit 5-7: Status data for the reaction telegram.
* 000: no error, request was processed  
* 001: transmission request being processed  
* 010: transmission cannot presently be processed  
* 100: warning  
* 110: error.
</summary>
=================================================================================================
</member>
<member name="T:TGM.Bitfields.sercos_control_t" decl="false" source="c:\ae\alternator_testbench\software\api-hardware\indradrive\lib\api\sis\telegrams_bitfields.h" line="99">
=================================================================================================
<summary>
The  control  byte  specifies  how  a  data  block  element  of  a  parameter  is accessed.
The control byte is read out of the command telegram and copied into the response  telegram.
</summary>
================================================================================================= 
</member>
<member name="F:TGM.Bitfields.sercos_control_t.&lt;unnamed-tag&gt;.bits_t.tx_status" decl="false" source="c:\ae\alternator_testbench\software\api-hardware\indradrive\lib\api\sis\telegrams_bitfields.h" line="114">
=================================================================================================
<summary>
The transmission of a consecutive telegram is controlled with this bit (lists are written in
several steps):
* 0: transmission in prog.
* 1: final transmission.
</summary>
=================================================================================================
</member>
<member name="F:TGM.Bitfields.sercos_control_t.&lt;unnamed-tag&gt;.bits_t.attribute" decl="false" source="c:\ae\alternator_testbench\software\api-hardware\indradrive\lib\api\sis\telegrams_bitfields.h" line="124">
=================================================================================================
<summary>
The type of the SERCOS parameter: 
* b000: channel not active   
* b001: ident number (write access)
* b010: name   
* b011: attribute (read access)
* b100: unit (read access)
* b101: min.input value (read access)
* b110: max.input value (read access)
* b111: operating data (write access)
</summary>
=================================================================================================
</member>
<member name="T:TGM.Bitfields.sercos_param_ident_t" decl="false" source="c:\ae\alternator_testbench\software\api-hardware\indradrive\lib\api\sis\telegrams_bitfields.h" line="162">
<summary>	Identification of the parameter. Size: 16 bit. </summary>
</member>
<member name="F:TGM.Bitfields.sercos_param_ident_t.&lt;unnamed-tag&gt;.bits_t.param_no" decl="false" source="c:\ae\alternator_testbench\software\api-hardware\indradrive\lib\api\sis\telegrams_bitfields.h" line="169">
<summary>	Bit 0-11: The parameter number [0..4095], e.g. P-0-*1177*, includes 1177 as param_no. </summary>
</member>
<member name="F:TGM.Bitfields.sercos_param_ident_t.&lt;unnamed-tag&gt;.bits_t.param_set" decl="false" source="c:\ae\alternator_testbench\software\api-hardware\indradrive\lib\api\sis\telegrams_bitfields.h" line="172">
<summary>	Bit 12-15: The parameter block [0..7], e.g. P-*0*-1177, includes 0 as param_set. </summary>
</member>
<member name="F:TGM.Bitfields.sercos_param_ident_t.&lt;unnamed-tag&gt;.bits_t.param_variant" decl="false" source="c:\ae\alternator_testbench\software\api-hardware\indradrive\lib\api\sis\telegrams_bitfields.h" line="175">
=================================================================================================
<summary>
Bit 16: Parameter variant:
* 0: S-Parameter (drive)  
* 1: P-Parameter (drive).
</summary>
=================================================================================================
</member>
<member name="M:TGM.Bitfields.sercos_param_ident_t.&lt;unnamed-tag&gt;.bits_t.#ctor(&lt;unknown type&gt;,System.UInt16)" decl="false" source="c:\ae\alternator_testbench\software\api-hardware\indradrive\lib\api\sis\telegrams_bitfields.h" line="184">
<summary>	Default constructor. </summary>
</member>
<member name="F:TGM.Bitfields.sercos_attribute_t.&lt;unnamed-tag&gt;.bits_t.conv_factor" decl="false" source="c:\ae\alternator_testbench\software\api-hardware\indradrive\lib\api\sis\telegrams_bitfields.h" line="209">
<summary>	Bit 0-15: Conversion factor: The conversion factor is an unsigned integer used to convert numeric data to display format.The conversion factor shall be set to a value of 1, if a conversion is not required(e.g. for binary numbers, character strings or floating - point numbers). </summary>
</member>
<member name="F:TGM.Bitfields.sercos_attribute_t.&lt;unnamed-tag&gt;.bits_t.data_len" decl="false" source="c:\ae\alternator_testbench\software\api-hardware\indradrive\lib\api\sis\telegrams_bitfields.h" line="212">
<summary>	Bit 16-18: The data length is required so that the Master is able to complete Service Channel data transfers correctly. </summary>
</member>
<member name="F:TGM.Bitfields.sercos_attribute_t.&lt;unnamed-tag&gt;.bits_t.func_of_data" decl="false" source="c:\ae\alternator_testbench\software\api-hardware\indradrive\lib\api\sis\telegrams_bitfields.h" line="215">
<summary>	Bit 19: Indicates whether this data calls a procedure in a drive: 0 Operation data or parameter 1 Procedure command. </summary>
</member>
<member name="F:TGM.Bitfields.sercos_attribute_t.&lt;unnamed-tag&gt;.bits_t.data_disp" decl="false" source="c:\ae\alternator_testbench\software\api-hardware\indradrive\lib\api\sis\telegrams_bitfields.h" line="218">
<summary>	Bit 20-22: Format Used to convert the operation data, and min/max input values to the correct display format. </summary>
</member>
<member name="F:TGM.Bitfields.sercos_attribute_t.&lt;unnamed-tag&gt;.bits_t.res5" decl="false" source="c:\ae\alternator_testbench\software\api-hardware\indradrive\lib\api\sis\telegrams_bitfields.h" line="221">
<summary>	Bit 23. </summary>
</member>
<member name="F:TGM.Bitfields.sercos_attribute_t.&lt;unnamed-tag&gt;.bits_t.scale_factor" decl="false" source="c:\ae\alternator_testbench\software\api-hardware\indradrive\lib\api\sis\telegrams_bitfields.h" line="224">
<summary>	Bit 24-27: Decimal point: Places after the decimal point indicates the position of the decimal point of appropriate operation data.Decimal point is used to define fixed point decimal numbers.For all other display formats the decimal point shall be = 0. </summary>
</member>
<member name="F:TGM.Bitfields.sercos_attribute_t.&lt;unnamed-tag&gt;.bits_t.is_writeonly_phase2" decl="false" source="c:\ae\alternator_testbench\software\api-hardware\indradrive\lib\api\sis\telegrams_bitfields.h" line="227">
<summary>	Bit 28. </summary>
</member>
<member name="F:TGM.Bitfields.sercos_attribute_t.&lt;unnamed-tag&gt;.bits_t.is_writeonly_phase3" decl="false" source="c:\ae\alternator_testbench\software\api-hardware\indradrive\lib\api\sis\telegrams_bitfields.h" line="230">
<summary>	Bit 29. </summary>
</member>
<member name="F:TGM.Bitfields.sercos_attribute_t.&lt;unnamed-tag&gt;.bits_t.is_writeonly_phase4" decl="false" source="c:\ae\alternator_testbench\software\api-hardware\indradrive\lib\api\sis\telegrams_bitfields.h" line="233">
<summary>	Bit 30. </summary>
</member>
<member name="F:TGM.Bitfields.sercos_attribute_t.&lt;unnamed-tag&gt;.bits_t.res10" decl="false" source="c:\ae\alternator_testbench\software\api-hardware\indradrive\lib\api\sis\telegrams_bitfields.h" line="236">
<summary>	Bit 31. </summary>
</member>
<member name="M:TGM.Bitfields.sercos_attribute_t.&lt;unnamed-tag&gt;.bits_t.#ctor" decl="false" source="c:\ae\alternator_testbench\software\api-hardware\indradrive\lib\api\sis\telegrams_bitfields.h" line="239">
<summary>	Default constructor. </summary>
</member>
<member name="T:TGM._data_t" decl="false" source="c:\ae\alternator_testbench\software\api-hardware\indradrive\lib\api\sis\telegrams.h" line="21">
=================================================================================================
<summary>
Struct to hold payload data in a command payload. Payload data is third part of a regular Telegram: Header +
Payload data + Payload header.
</summary>
=================================================================================================
</member>
<member name="M:TGM._data_t.#ctor(std.vector&lt;System.Byte,std.allocator&lt;System.Byte&gt;&gt;)" decl="false" source="c:\ae\alternator_testbench\software\api-hardware\indradrive\lib\api\sis\telegrams.h" line="32">
<summary>	Default constructor. </summary>
</member>
<member name="T:TGM._container_t" decl="false" source="c:\ae\alternator_testbench\software\api-hardware\indradrive\lib\api\sis\telegrams.h" line="155">
<summary>	Container for Telegram in raw data. </summary>
</member>
<member name="F:TGM._container_t.bytes" decl="false" source="c:\ae\alternator_testbench\software\api-hardware\indradrive\lib\api\sis\telegrams.h" line="158">
<summary>	The raw data. Size: 254 bytes </summary>
</member>
<member name="M:TGM._container_t.#ctor" decl="false" source="c:\ae\alternator_testbench\software\api-hardware\indradrive\lib\api\sis\telegrams.h" line="161">
<summary>	Default constructor. </summary>
</member>
<member name="M:TGM._container_t.clear" decl="false" source="c:\ae\alternator_testbench\software\api-hardware\indradrive\lib\api\sis\telegrams.h" line="164">
<summary>	Clears this object to its blank/initial state. </summary>
</member>
<member name="T:TGM._header_t" decl="false" source="c:\ae\alternator_testbench\software\api-hardware\indradrive\lib\api\sis\telegrams.h" line="208">
=================================================================================================
<summary>
The Telegram Header contains all information required for conducting orderly telegram traffic.
</summary>
=================================================================================================
</member>
<member name="F:TGM._header_t.StZ" decl="false" source="c:\ae\alternator_testbench\software\api-hardware\indradrive\lib\api\sis\telegrams.h" line="216">
<summary>	Start symbol: STX (0x02). </summary>
</member>
<member name="F:TGM._header_t.CS" decl="false" source="c:\ae\alternator_testbench\software\api-hardware\indradrive\lib\api\sis\telegrams.h" line="219">
=================================================================================================
<summary>
The checksum byte. It is generated by adding all sub-sequential telegram symbols as well as
the start symbol StZ and concluding negation. In other words, the sum of all telegram symbols
always equals 0 if the transmission was successful.
</summary>
=================================================================================================
</member>
<member name="F:TGM._header_t.DatL" decl="false" source="c:\ae\alternator_testbench\software\api-hardware\indradrive\lib\api\sis\telegrams.h" line="228">
=================================================================================================
<summary>
The length of the sub-sequential user data and the variable part are in the frame protocol. Up
to 247 bytes (255 - 7{subaddresses} - 1{running telegram number}) user data can be
transmitted in one telegram.
</summary>
=================================================================================================
</member>
<member name="F:TGM._header_t.DatLW" decl="false" source="c:\ae\alternator_testbench\software\api-hardware\indradrive\lib\api\sis\telegrams.h" line="237">
=================================================================================================
<summary>
Repetition of DatL takes place here. The telegram length is generated from the DatLW and the
fixed part of the frame protocol (byte 1-8), i.e. telegram length = DatLW + 8.
</summary>
=================================================================================================
</member>
<member name="F:TGM._header_t.Cntrl" decl="false" source="c:\ae\alternator_testbench\software\api-hardware\indradrive\lib\api\sis\telegrams.h" line="245">
=================================================================================================
<summary>
Control byte consisting of several bit fields. Use TGM::Bitfields::Cntrl and toByte() for configuration.
</summary>
=================================================================================================
</member>
<member name="F:TGM._header_t.Service" decl="false" source="c:\ae\alternator_testbench\software\api-hardware\indradrive\lib\api\sis\telegrams.h" line="252">
=================================================================================================
<summary>
This specifies the service that the sender requests from the recipient or that the recipient
has executed.
* 0x00 ... 0x0F     General services:
* 0x00				User identification
* 0x01				Data transmission aborted
* 0x02				Flash operation
* 0x03				Initializing SIS communication
* 0x04				Executing a list of SIS services
* 0x0F				Token passing
* 0x10 ... 0x7F     temporarily reserved
* 0x80 ... 0x8F     Special services for ECODRIVE
* 0x90 ... 0x9F     Special services for SYNAX
* 0xA0 ... 0xAF     Special services for MT - CNC or .MTC200
* 0xB0 ... 0xBF     Special services for ISP200
* 0xC0 ... 0xCF     Special services for CLC - GPS
* 0xD0 ... 0xDF     Special services for HMI - System
* 0xE0 ... 0xEF     Special services for DISC
* 0xF0 ... 0xFF     temporarily reserved.
</summary>
=================================================================================================
</member>
<member name="F:TGM._header_t.AdrS" decl="false" source="c:\ae\alternator_testbench\software\api-hardware\indradrive\lib\api\sis\telegrams.h" line="276">
=================================================================================================
<summary>
Address of sender:
* AdrS = [0..126]: specifies a single station
* AdrS = 127:  Special address for a SIS master in case of service or
emergencies (this address may not be used during active communication).
</summary>
=================================================================================================
</member>
<member name="F:TGM._header_t.AdrE" decl="false" source="c:\ae\alternator_testbench\software\api-hardware\indradrive\lib\api\sis\telegrams.h" line="286">
=================================================================================================
<summary>
Address of Recipient:
* AdrE = [0..126]: specifies a single station,
* AdrE = 128: Special address for point - to - point communication(the
recipient's response is not dependent on its actual station number with this special address).
* AdrE = [129..199]: reserved,
* AdrE = [200..253]: addresses logical groups,
* AdrE = 254: specifies a broadcast to all stations on a hierarchical
level(this address can only be listed once, as the last address in the list),
* AdrE = 255: specifies a global broadcast.
Telegrams with AdrE = [200..255] are not answered with a response telegram.
</summary>
=================================================================================================
</member>
<member name="M:TGM._header_t.#ctor(System.Byte,System.Byte,System.Byte,TGM.Bitfields._header_cntrl_t)" decl="false" source="c:\ae\alternator_testbench\software\api-hardware\indradrive\lib\api\sis\telegrams.h" line="302">
<summary>	Default constructor. </summary>
</member>
<member name="M:TGM._header_t.get_sum(System.Boolean)" decl="false" source="c:\ae\alternator_testbench\software\api-hardware\indradrive\lib\api\sis\telegrams.h" line="314">
=================================================================================================
<summary>	Gets the sum without carry of all header bytes for checksum calculation. </summary>

<param name="exclude_cs">	(Optional) true to exclude checksum from calculation. </param>

<returns>	The sum. </returns>
=================================================================================================
</member>
<member name="M:TGM._header_t.set_DatL(System.UInt32)" decl="false" source="c:\ae\alternator_testbench\software\api-hardware\indradrive\lib\api\sis\telegrams.h" line="332">
=================================================================================================
<summary>
Sets length of Telegram, stored in DatL and DatLW (copy). By default, the length of the telegram is defined by
the payload length (head + data).
</summary>

<param name="_payload_len">	Length of the payload. </param>
=================================================================================================
</member>
<member name="M:TGM._header_t.calc_checksum(TGM._container_t*)" decl="false" source="c:\ae\alternator_testbench\software\api-hardware\indradrive\lib\api\sis\telegrams.h" line="344">
=================================================================================================
<summary>
Calculates the Telegram checksum, stored in CS. The calculated checksum will automatically
assigned to CS. This function will use DatL parameter for the appropriate length
determination.
</summary>

<param name="_payload">	   	[in] Bytestream of payload (head + data) with the raw data. </param>
<param name="_payload_len">	Length of the payload. </param>
=================================================================================================
</member>
<member name="T:TGM._header_ext_t" decl="false" source="c:\ae\alternator_testbench\software\api-hardware\indradrive\lib\api\sis\telegrams.h" line="370">
<summary>	Extended Telegram Header to be used for Routing and Sequential Telegrams. </summary>
</member>
<member name="F:TGM._header_ext_t.AdrES1" decl="false" source="c:\ae\alternator_testbench\software\api-hardware\indradrive\lib\api\sis\telegrams.h" line="374">
=================================================================================================
<summary>
Expanded part of the telegram header. Subaddress 1 of recipient. Bit 0-2 of Cntrl byte:  000.
</summary>
=================================================================================================
</member>
<member name="F:TGM._header_ext_t.AdrES2" decl="false" source="c:\ae\alternator_testbench\software\api-hardware\indradrive\lib\api\sis\telegrams.h" line="381">
=================================================================================================
<summary>
Expanded part of the telegram header. Subaddress 2 of recipient. Bit 0-2 of Cntrl byte:  001.
</summary>
=================================================================================================
</member>
<member name="F:TGM._header_ext_t.AdrES3" decl="false" source="c:\ae\alternator_testbench\software\api-hardware\indradrive\lib\api\sis\telegrams.h" line="388">
=================================================================================================
<summary>
Expanded part of the telegram header. Subaddress 3 of recipient. Bit 0-2 of Cntrl byte:  010.
</summary>
=================================================================================================
</member>
<member name="F:TGM._header_ext_t.AdrES4" decl="false" source="c:\ae\alternator_testbench\software\api-hardware\indradrive\lib\api\sis\telegrams.h" line="395">
=================================================================================================
<summary>
Expanded part of the telegram header. Subaddress 4 of recipient. Bit 0-2 of Cntrl byte:  011.
</summary>
=================================================================================================
</member>
<member name="F:TGM._header_ext_t.AdrES5" decl="false" source="c:\ae\alternator_testbench\software\api-hardware\indradrive\lib\api\sis\telegrams.h" line="402">
=================================================================================================
<summary>
Expanded part of the telegram header. Subaddress 5 of recipient. Bit 0-2 of Cntrl byte:  100.
</summary>
=================================================================================================
</member>
<member name="F:TGM._header_ext_t.AdrES6" decl="false" source="c:\ae\alternator_testbench\software\api-hardware\indradrive\lib\api\sis\telegrams.h" line="409">
=================================================================================================
<summary>
Expanded part of the telegram header. Subaddress 6 of recipient. Bit 0-2 of Cntrl byte:  101.
</summary>
=================================================================================================
</member>
<member name="F:TGM._header_ext_t.AdrES7" decl="false" source="c:\ae\alternator_testbench\software\api-hardware\indradrive\lib\api\sis\telegrams.h" line="416">
=================================================================================================
<summary>
Expanded part of the telegram header. Subaddress 7 of recipient. Bit 0-2 of Cntrl byte:  110.
</summary>
=================================================================================================
</member>
<member name="F:TGM._header_ext_t.PaketN" decl="false" source="c:\ae\alternator_testbench\software\api-hardware\indradrive\lib\api\sis\telegrams.h" line="423">
=================================================================================================
<summary>
Expanded part of the telegram header. Sequential telegram number (packet number) , if bit 3
in Cntrl byte is set.
</summary>
=================================================================================================
</member>
<member name="T:TGM.Commands._subservice_payload_t" decl="false" source="c:\ae\alternator_testbench\software\api-hardware\indradrive\lib\api\sis\telegrams.h" line="437">
=================================================================================================
<summary>
Representation of the PAYLOAD for a Subservice command. A Command Telegram is for regular subservices,
such communication init, or device identification. User for master communication (active communicator).
</summary>
=================================================================================================
</member>
<member name="M:TGM.Commands._subservice_payload_t.#ctor(System.Byte,System.Byte,TGM._data_t)" decl="false" source="c:\ae\alternator_testbench\software\api-hardware\indradrive\lib\api\sis\telegrams.h" line="451">
=================================================================================================
<summary>	Constructor. </summary>

<param name="_addr">	  	The recipient address. </param>
<param name="_subservice">	The subservice number. </param>
=================================================================================================
</member>
<member name="M:TGM.Commands._subservice_payload_t.clear" decl="false" source="c:\ae\alternator_testbench\software\api-hardware\indradrive\lib\api\sis\telegrams.h" line="466">
<summary>	Clears this object to its blank/initial state. </summary>
</member>
<member name="T:TGM.Commands._sercos_param_t" decl="false" source="c:\ae\alternator_testbench\software\api-hardware\indradrive\lib\api\sis\telegrams.h" line="477">
=================================================================================================
<summary>
Sercos Command Telegram used for reading/writing single parameter from/to slave.
</summary>
=================================================================================================
</member>
<member name="F:TGM.Commands._sercos_param_t.control" decl="false" source="c:\ae\alternator_testbench\software\api-hardware\indradrive\lib\api\sis\telegrams.h" line="485">
<summary>	Sercos control. Size: 8 bit. Set coding by TGM::Bitfields::Sercos_Control and toByte(). </summary>
</member>
<member name="F:TGM.Commands._sercos_param_t.unit_addr" decl="false" source="c:\ae\alternator_testbench\software\api-hardware\indradrive\lib\api\sis\telegrams.h" line="488">
=================================================================================================
<summary>
The unit address of a drive is read in the command telegram and copied into the response
telegram. For direct SIS communication with drives supporting SIS interface, unit address is
the same as the SIS address of the receiver. Otherwise, the  SIS  address  is  related  to
the  motion control and the unit address to the drive.
</summary>
=================================================================================================
</member>
<member name="F:TGM.Commands._sercos_param_t.param_num" decl="false" source="c:\ae\alternator_testbench\software\api-hardware\indradrive\lib\api\sis\telegrams.h" line="500">
=================================================================================================
<summary>
Identifier for the parameter. Size: 16 bit. Set coding by TGM::Bitfields::Sercos_Param_Ident and toByte().
</summary>
=================================================================================================
</member>
<member name="F:TGM.Commands._sercos_param_t.data" decl="false" source="c:\ae\alternator_testbench\software\api-hardware\indradrive\lib\api\sis\telegrams.h" line="507">
<summary>	Payload data. </summary>
</member>
<member name="T:TGM.Commands._sercos_list_t" decl="false" source="c:\ae\alternator_testbench\software\api-hardware\indradrive\lib\api\sis\telegrams.h" line="538">
=================================================================================================
<summary>
Sercos Command Telegram used for reading/writing single elements in lists from/to slave.
</summary>
=================================================================================================
</member>
<member name="F:TGM.Commands._sercos_list_t.control" decl="false" source="c:\ae\alternator_testbench\software\api-hardware\indradrive\lib\api\sis\telegrams.h" line="546">
<summary>	Sercos control. Size: 8 bit. Set coding by TGM::Bitfields::Sercos_Control and toByte(). </summary>
</member>
<member name="F:TGM.Commands._sercos_list_t.unit_addr" decl="false" source="c:\ae\alternator_testbench\software\api-hardware\indradrive\lib\api\sis\telegrams.h" line="549">
=================================================================================================
<summary>
The unit address of a drive is read in the command telegram and copied into the response
telegram. For direct SIS communication with drives supporting SIS interface, unit address is
the same as the SIS address of the receiver. Otherwise, the  SIS  address  is  related  to
the  motion control and the unit address to the drive.
</summary>
=================================================================================================
</member>
<member name="F:TGM.Commands._sercos_list_t.param_num" decl="false" source="c:\ae\alternator_testbench\software\api-hardware\indradrive\lib\api\sis\telegrams.h" line="561">
=================================================================================================
<summary>
Identifier for the parameter. Size: 16 bit. Set coding by TGM::Bitfields::Sercos_Param_Ident and toByte().
</summary>
=================================================================================================
</member>
<member name="F:TGM.Commands._sercos_list_t.list_offset" decl="false" source="c:\ae\alternator_testbench\software\api-hardware\indradrive\lib\api\sis\telegrams.h" line="568">
=================================================================================================
<summary>
Defines the offset in bytes of the segment that has to be read. For example: The 11th element of a list
consisting of 4-byte elements should be handeled --&gt; list_offset=0x0028.
</summary>
=================================================================================================
</member>
<member name="F:TGM.Commands._sercos_list_t.element_size" decl="false" source="c:\ae\alternator_testbench\software\api-hardware\indradrive\lib\api\sis\telegrams.h" line="576">
<summary>	Size of the element to be handeled. For example: The 11th element of a list
consisting of 4-byte elements should be handeled --&gt; element_size=0x0004. </summary>
</member>
<member name="F:TGM.Commands._sercos_list_t.data" decl="false" source="c:\ae\alternator_testbench\software\api-hardware\indradrive\lib\api\sis\telegrams.h" line="580">
<summary>	Payload data. </summary>
</member>
<member name="T:TGM.Reactions._subservice_payload_t" decl="false" source="c:\ae\alternator_testbench\software\api-hardware\indradrive\lib\api\sis\telegrams.h" line="618">
=================================================================================================
<summary>
Representation of the payload for a Subservice reaction. A Reaction Telegram is for regular subservices,
such communication init, or device identification. This telegram is responded after successful execution of
previous Command Telegram.
</summary>
=================================================================================================
</member>
<member name="T:TGM.Reactions._sercos_param_payload_t" decl="false" source="c:\ae\alternator_testbench\software\api-hardware\indradrive\lib\api\sis\telegrams.h" line="659">
=================================================================================================
<summary>
Representation of the payload for a Sercos Parameter reaction. A Reaction Telegram is for regular subservices,
such communication init, or device identification. This telegram is responded after successful execution of
previous Command Telegram.
</summary>
=================================================================================================
</member>
<member name="F:TGM.Reactions._sercos_param_payload_t.control" decl="false" source="c:\ae\alternator_testbench\software\api-hardware\indradrive\lib\api\sis\telegrams.h" line="670">
<summary>	Sercos control. Size: 8 bit. Set coding by TGM::Bitfields::Sercos_Control and toByte(). </summary>
</member>
<member name="F:TGM.Reactions._sercos_param_payload_t.unit_addr" decl="false" source="c:\ae\alternator_testbench\software\api-hardware\indradrive\lib\api\sis\telegrams.h" line="673">
=================================================================================================
<summary>
The unit address of a drive is read in the command telegram and copied into the response
telegram. For direct SIS communication with drives supporting SIS interface, unit address is
the same as the SIS address of the receiver. Otherwise, the  SIS  address  is  related  to
the  motion control and the unit address to the drive.
</summary>
=================================================================================================
</member>
<member name="T:TGM.Reactions._sercos_list_t" decl="false" source="c:\ae\alternator_testbench\software\api-hardware\indradrive\lib\api\sis\telegrams.h" line="710">
=================================================================================================
<summary>
Sercos Command Telegram used for reading/writing single elements in lists from/to slave.
</summary>
=================================================================================================
</member>
<member name="F:TGM.Reactions._sercos_list_t.control" decl="false" source="c:\ae\alternator_testbench\software\api-hardware\indradrive\lib\api\sis\telegrams.h" line="720">
<summary>	Sercos control. Size: 8 bit. Set coding by TGM::Bitfields::Sercos_Control and toByte(). </summary>
</member>
<member name="F:TGM.Reactions._sercos_list_t.unit_addr" decl="false" source="c:\ae\alternator_testbench\software\api-hardware\indradrive\lib\api\sis\telegrams.h" line="723">
=================================================================================================
<summary>
The unit address of a drive is read in the command telegram and copied into the response
telegram. For direct SIS communication with drives supporting SIS interface, unit address is
the same as the SIS address of the receiver. Otherwise, the  SIS  address  is  related  to
the  motion control and the unit address to the drive.
</summary>
=================================================================================================
</member>
<member name="T:SISProtocol" decl="false" source="c:\ae\alternator_testbench\software\api-hardware\indradrive\lib\api\sis\sisprotocol.h" line="18">
<summary>	Defines address master. </summary>
<summary>	Defines sis address slave. '128' is used for peer-to-peer communication. </summary>
<summary>	Address unit. For Indradrive, this value can be found at P-0-4022. </summary>
</member>
<member name="T:SISProtocol.ExceptionGeneric" decl="true" source="c:\ae\alternator_testbench\software\api-hardware\indradrive\lib\api\sis\sisprotocol.h" line="39">
<summary>	Generic exception handling for SIS Protocol. </summary>
</member>
<member name="T:SISProtocol.ExceptionTransceiveFailed" decl="true" source="c:\ae\alternator_testbench\software\api-hardware\indradrive\lib\api\sis\sisprotocol.h" line="41">
<summary>	Specific exception handling of SIS Protocol for failed transceiving. </summary>
</member>
<member name="T:SISProtocol.init_set_mask_baudrate" decl="false" source="c:\ae\alternator_testbench\software\api-hardware\indradrive\lib\api\sis\sisprotocol.h" line="44">
<summary>	Baudrate mask that can be utilized for the Command Telegram Subservice 0x07 </summary>
</member>
<member name="M:SISProtocol.#ctor" decl="true" source="c:\ae\alternator_testbench\software\api-hardware\indradrive\lib\api\sis\sisprotocol.h" line="54">
<summary>	Default constructor. </summary>
</member>
<member name="M:SISProtocol.Dispose" decl="true" source="c:\ae\alternator_testbench\software\api-hardware\indradrive\lib\api\sis\sisprotocol.h" line="56">
<summary>	Destructor. </summary>
</member>
<member name="M:SISProtocol.open(System.SByte!System.Runtime.CompilerServices.IsSignUnspecifiedByte!System.Runtime.CompilerServices.IsConst*)" decl="true" source="c:\ae\alternator_testbench\software\api-hardware\indradrive\lib\api\sis\sisprotocol.h" line="59">
API FUNCTIONS
</member>
<member name="M:SISProtocol.get_attributes(&lt;unknown type&gt;,System.UInt16!System.Runtime.CompilerServices.IsConst*!System.Runtime.CompilerServices.IsImplicitlyDereferenced,System.Byte*!System.Runtime.CompilerServices.IsImplicitlyDereferenced,System.UInt32*!System.Runtime.CompilerServices.IsImplicitlyDereferenced)" decl="true" source="c:\ae\alternator_testbench\software\api-hardware\indradrive\lib\api\sis\sisprotocol.h" line="79">
TELEGRAM SUPPORTING FUNCTIONS
</member>
<member name="T:SISProtocol.ExceptionGeneric" decl="false" source="c:\ae\alternator_testbench\software\api-hardware\indradrive\lib\api\sis\sisprotocol.h" line="107">
<summary>	Generic exceptions for SIS protocol. </summary>
</member>
<member name="T:SISProtocol.ExceptionTransceiveFailed" decl="false" source="c:\ae\alternator_testbench\software\api-hardware\indradrive\lib\api\sis\sisprotocol.h" line="143">
<summary>	Specific exception handling of SIS Protocol transceiving failed. </summary>
</member>
</members>
</doc>