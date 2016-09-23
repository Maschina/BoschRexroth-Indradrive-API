#include "SISProtocol.h"



SISProtocol::SISProtocol()
{
}


SISProtocol::~SISProtocol()
{
}


void SISProtocol::open(const wchar_t * _port)
{
	STACK;

	LPCTSTR cport = (LPCTSTR)_port;
	CSerial::EBaudrate cbaudrate = CSerial::EBaud19200;
	CSerial::EDataBits cdata = CSerial::EData8;
	CSerial::EParity cparity = CSerial::EParNone;
	CSerial::EStopBits cstopbits = CSerial::EStop1;
	CSerial::EHandshake chandshake = CSerial::EHandshakeOff;

	try
	{
		CSerial::CheckPort(cport);

		m_serial.Open(cport, RS232_BUFFER, RS232_BUFFER, true /* overlapped */);
		m_serial.Setup(cbaudrate, cdata, cparity, cstopbits);
		m_serial.SetupHandshaking(chandshake);

		m_serial.SetMask(CSerial::EEventBreak |
			CSerial::EEventError |
			CSerial::EEventRecv);

		m_serial.SetupReadTimeouts(CSerial::EReadTimeoutNonblocking);
	}
	catch (CSerial::ExceptionGeneric &ex)
	{
		throw;
	}
}

void SISProtocol::close()
{
	STACK;

	try
	{
		m_serial.Close();
	}
	catch (CSerial::ExceptionGeneric &ex)
	{
		throw;
	}
}


void SISProtocol::set_baudrate(init_set_mask_baudrate _baudrate)
{
	STACK;

	/// Build Telegrams
	// Mapping for SEND Telegram
	TGM::Map<TGM::Header, TGM::Commands::Subservice>
		tx_tgm(
			// Init header
			TGM::Header(SIS_ADDR_MASTER, SIS_ADDR_SLAVE, SIS_SERVICE_INIT_COMM, TGM::Bitfields::Header_Cntrl(TGM::Type_Command)),
			// Init payload
			TGM::Commands::Subservice(SIS_ADDR_UNIT, 0x07, TGM::Data({ (BYTE)_baudrate }))
		);
	
	// Mapping for RECEPTION Telegram
	TGM::Map<TGM::Header, TGM::Reactions::Subservice> rx_tgm;

	// Set payload size
	tx_tgm.mapping.header.set_DatL(tx_tgm.mapping.payload.get_size());

	/// Transceive
	transceiving(tx_tgm, rx_tgm);
}


void SISProtocol::read_parameter(TGM::SERCOS_ParamVar _paramvar, USHORT _paramnum, UINT32 & _rcvddata)
{
	STACK;

	/// Fetching attributes for length and scale
	size_t datalen = 1;
	UINT8 scalefactor = 0;
	get_parameter_attributes(_paramvar, _paramnum, scalefactor, datalen);

	/// Communication with Telegrams
	BYTE service = SIS_SERVICE_SERCOS_PARAM_READ;

	auto rx_tgm = transceive_param
		<TGM::Header, TGM::Commands::Sercos_Param, TGM::Header, TGM::Reactions::Sercos_Param>
		(_paramvar, _paramnum, service);

	/// Convert responsed data
	INT64 response = get_sized_data(rx_tgm.mapping.payload.data, datalen);
	_rcvddata = static_cast<UINT32>(response);
}

void SISProtocol::read_parameter(TGM::SERCOS_ParamVar _paramvar, USHORT _paramnum, UINT64& _rcvddata)
{
	STACK;

	/// Fetching attributes for length and scale
	size_t datalen = 1;
	UINT8 scalefactor = 0;
	get_parameter_attributes(_paramvar, _paramnum, scalefactor, datalen);

	/// Communication with Telegrams
	BYTE service = SIS_SERVICE_SERCOS_PARAM_READ;

	auto rx_tgm = transceive_param
		<TGM::Header, TGM::Commands::Sercos_Param, TGM::Header, TGM::Reactions::Sercos_Param>
		(_paramvar, _paramnum, service);

	/// Convert responsed data
	INT64 response = get_sized_data(rx_tgm.mapping.payload.data, datalen);
	_rcvddata = static_cast<UINT64>(response);
}

void SISProtocol::read_parameter(TGM::SERCOS_ParamVar _paramvar, USHORT _paramnum, DOUBLE & _rcvddata)
{
	STACK;

	/// Fetching attributes for length and scale
	size_t datalen = 1;
	UINT8 scalefactor = 0;
	get_parameter_attributes(_paramvar, _paramnum, scalefactor, datalen);

	/// Communication with Telegrams
	BYTE service = SIS_SERVICE_SERCOS_PARAM_READ;

	auto rx_tgm = transceive_param
					<TGM::Header, TGM::Commands::Sercos_Param, TGM::Header, TGM::Reactions::Sercos_Param>
					(_paramvar, _paramnum, service);

	/// Convert responsed data
	INT64 response = get_sized_data(rx_tgm.mapping.payload.data, datalen);
	_rcvddata = (double)response / std::pow(10, scalefactor);
}

void SISProtocol::read_parameter(TGM::SERCOS_ParamVar _paramvar, USHORT _paramnum, char _rcvddata[TGM_SIZEMAX_PAYLOAD])
{
	/// Communication with Telegrams
	BYTE service = SIS_SERVICE_SERCOS_PARAM_READ;

	auto rx_tgm = transceive_param
					<TGM::Header, TGM::Commands::Sercos_Param, TGM::Header, TGM::Reactions::Sercos_Param>
					(_paramvar, _paramnum, service);

	/// Convert responsed data
	strcpy_s(_rcvddata, rx_tgm.mapping.payload.data.size, (char*)rx_tgm.mapping.payload.data.data);
}

void SISProtocol::read_listelm(TGM::SERCOS_ParamVar _paramvar, USHORT _paramnum, USHORT _elm_pos, UINT32 & _rcvdelm)
{
	STACK;

	/// Fetching attributes for length and scale
	size_t datalen = 1;
	UINT8 scalefactor = 0;
	get_parameter_attributes(_paramvar, _paramnum, scalefactor, datalen);

	USHORT element_size = (USHORT)datalen;
	USHORT list_offset = _elm_pos * element_size;

	/// Communication with Telegrams
	BYTE service = SIS_SERVICE_SERCOS_LIST_WRITE;

	auto rx_tgm = transceive_list
		<TGM::Header, TGM::Commands::Sercos_List, TGM::Header, TGM::Reactions::Sercos_List>
		(_paramvar, _paramnum, service, element_size, list_offset);

	/// Response data
	INT64 response = get_sized_data(rx_tgm.mapping.payload.data, datalen);
	_rcvdelm = static_cast<UINT32>(response);
}

void SISProtocol::read_listelm(TGM::SERCOS_ParamVar _paramvar, USHORT _paramnum, USHORT _elm_pos, UINT64& _rcvdelm)
{
	STACK;

	/// Fetching attributes for length and scale
	size_t datalen = 1;
	UINT8 scalefactor = 0;
	get_parameter_attributes(_paramvar, _paramnum, scalefactor, datalen);

	USHORT element_size = (USHORT)datalen;
	USHORT list_offset = _elm_pos * element_size;

	/// Communication with Telegrams
	BYTE service = SIS_SERVICE_SERCOS_LIST_WRITE;

	auto rx_tgm = transceive_list
		<TGM::Header, TGM::Commands::Sercos_List, TGM::Header, TGM::Reactions::Sercos_List>
		(_paramvar, _paramnum, service, element_size, list_offset);

	/// Response data
	INT64 response = get_sized_data(rx_tgm.mapping.payload.data, datalen);
	_rcvdelm = static_cast<UINT64>(response);
}

void SISProtocol::read_listelm(TGM::SERCOS_ParamVar _paramvar, USHORT _paramnum, USHORT _elm_pos, DOUBLE & _rcvdelm)
{
	STACK;

	/// Fetching attributes for length and scale
	size_t datalen = 1;
	UINT8 scalefactor = 0;
	get_parameter_attributes(_paramvar, _paramnum, scalefactor, datalen);

	USHORT element_size = (USHORT)datalen;
	USHORT list_offset = _elm_pos * element_size;

	/// Communication with Telegrams
	BYTE service = SIS_SERVICE_SERCOS_LIST_WRITE;

	auto rx_tgm = transceive_list
					<TGM::Header, TGM::Commands::Sercos_List, TGM::Header, TGM::Reactions::Sercos_List>
					(_paramvar, _paramnum, service, element_size, list_offset);

	/// Response data
	INT64 response = get_sized_data(rx_tgm.mapping.payload.data, datalen);
	_rcvdelm = (double)response / std::pow(10, scalefactor);
}


INT64 SISProtocol::get_sized_data(TGM::Data& rx_data, const size_t &datalen)
{
	STACK;

	if (datalen == 1)
	{
		UINT8 data = rx_data.toUINT8();
		UINT64 mask = ((data >> 7) & 1) ? 0xFFFFFFFFFFFFFF00 : 0;
		return (INT64)(data | mask);
	}
	else if (datalen == 2)
	{
		UINT16 data = rx_data.toUINT16();
		UINT64 mask = ((data >> 15) & 1) ? 0xFFFFFFFFFFFF0000 : 0;
		return (INT64)(data | mask);
	}
	else if (datalen == 8) 
	{
		return (INT64)rx_data.toUINT64();
	}		
	else
	{
		UINT32 data = rx_data.toUINT32();
		UINT64 mask = ((data >> 31) & 1) ? 0xFFFFFFFF00000000 : 0;
		return (INT64)(data | mask);
	}
}



void SISProtocol::write_parameter(TGM::SERCOS_ParamVar _paramvar, USHORT _paramnum, const UINT32 _data)
{
	STACK;

	write_parameter(_paramvar, _paramnum, static_cast<DOUBLE>(_data));
}

void SISProtocol::write_parameter(TGM::SERCOS_ParamVar _paramvar, USHORT _paramnum, const UINT64 _data)
{
	STACK;

	write_parameter(_paramvar, _paramnum, static_cast<DOUBLE>(_data));
}

void SISProtocol::write_parameter(TGM::SERCOS_ParamVar _paramvar, USHORT _paramnum, const DOUBLE _data)
{
	STACK;

	/// Fetching attributes for length and scale
	size_t datalen = 1;
	UINT8 scalefactor = 0;
	get_parameter_attributes(_paramvar, _paramnum, scalefactor, datalen);

	/// Preprocess data
	UINT64 inval = static_cast<UINT64>(_data * std::pow(10, scalefactor));

	TGM::Data data;
	set_sized_data(data, datalen, inval);

	/// Communication with Telegrams
	BYTE service = SIS_SERVICE_SERCOS_PARAM_WRITE;

	transceive_param
		<TGM::Header, TGM::Commands::Sercos_Param, TGM::Header, TGM::Reactions::Sercos_Param>
		(_paramvar, _paramnum, service, &data);
}


void SISProtocol::write_listelm(TGM::SERCOS_ParamVar _paramvar, USHORT _paramnum, USHORT _elm_pos, const UINT32 _rcvdelm)
{
	STACK;

	DOUBLE buf = static_cast<DOUBLE>(_rcvdelm);
	write_listelm(_paramvar, _paramnum, _elm_pos, buf);
}

void SISProtocol::write_listelm(TGM::SERCOS_ParamVar _paramvar, USHORT _paramnum, USHORT _elm_pos, const UINT64 _rcvdelm)
{
	STACK;

	DOUBLE buf = static_cast<DOUBLE>(_rcvdelm);
	write_listelm(_paramvar, _paramnum, _elm_pos, buf);
}

void SISProtocol::write_listelm(TGM::SERCOS_ParamVar _paramvar, USHORT _paramnum, USHORT _elm_pos, const DOUBLE _rcvdelm)
{
	STACK;

	/// Fetching attributes for length and scale
	size_t datalen = 1;
	UINT8 scalefactor = 0;
	get_parameter_attributes(_paramvar, _paramnum, scalefactor, datalen);

	UINT64 inval = static_cast<UINT64>(_rcvdelm * std::pow(10, scalefactor));

	TGM::Data data;
	set_sized_data(data, datalen, inval);

	USHORT element_size = (USHORT)datalen;
	USHORT list_offset = _elm_pos * element_size;

	/// Communication with Telegrams
	BYTE service = SIS_SERVICE_SERCOS_LIST_WRITE;

	transceive_list
		<TGM::Header, TGM::Commands::Sercos_List, TGM::Header, TGM::Reactions::Sercos_List>
		(_paramvar, _paramnum, service, element_size, list_offset, &data);
}


void SISProtocol::execute_command(TGM::SERCOS_ParamVar _paramvar, USHORT _paramnum)
{
	TGM::SERCOS_Commandrequest cmd;
	TGM::SERCOS_Commandstatus status = TGM::SERCOS_Commandstatus_busy;
	int iterations;

	/// Start command
	cmd = TGM::SERCOS_Commandrequest_set;
	try
	{
		write_parameter(_paramvar, _paramnum, static_cast<UINT64>(cmd));
	}
	catch (SISProtocol::ExceptionSISError &ex)
	{
		if (ex.get_errorcode() == 0x700C)
			throw SISProtocol::ExceptionGeneric(-1, "Command cannot be executed, because it is write-protected. Release the drive torque (disable drive), or restart the Indradrive system.");
		else
			throw;
	}
	
	iterations = 0;
	do
	{
		get_parameter_status(_paramvar, _paramnum, status);

		if (iterations > 300) throw ExceptionGeneric(-1, "Command execution caused a continuous busy loop. Please restart the Indradrive system.");
	} while (status == TGM::SERCOS_Commandstatus_busy);

	if (status != TGM::SERCOS_Commandstatus_ok)
		throw ExceptionGeneric(static_cast<int>(status), sformat("Command execution failed with status code %d. Command executation canceled or not possible due to released operation state of the drive.", status));

	
	/// Delete command
	cmd = TGM::SERCOS_Commandrequest_not_set;
	write_parameter(_paramvar, _paramnum, static_cast<UINT64>(cmd));
	
	status = TGM::SERCOS_Commandstatus_busy;
	iterations = 0;
	do
	{
		get_parameter_status(_paramvar, _paramnum, status);

		if (iterations > 300) throw ExceptionGeneric(-1, "Command execution caused a continuous busy loop. Please restart the Indradrive system.");
	} while (status == TGM::SERCOS_Commandstatus_busy);

	if (status != TGM::SERCOS_Commandstatus_not_set)
		throw ExceptionGeneric(static_cast<int>(status), sformat("Command execution failed with status code %d. Command executation canceled or not possible due to released operation state of the drive.", status));
}


void SISProtocol::get_parameter_status(const TGM::SERCOS_ParamVar _paramvar, const USHORT & _paramnum, TGM::SERCOS_Commandstatus& _datastatus)
{
	STACK;

	/// Communication with Telegrams
	BYTE service = SIS_SERVICE_SERCOS_PARAM_WRITE;

	auto rx_tgm = transceive_param
					<TGM::Header, TGM::Commands::Sercos_Param, TGM::Header, TGM::Reactions::Sercos_Param>
					(_paramvar, _paramnum, service, new TGM::Data(), TGM::SERCOS_Datablock_ident_number);

	/// Read back attribute
	_datastatus = static_cast<TGM::SERCOS_Commandstatus>(rx_tgm.mapping.payload.data.toUINT8());
}


void SISProtocol::set_sized_data(TGM::Data& tx_data, const size_t &datalen, UINT64 & _rcvdelm)
{
	STACK; 

	if (datalen == 1) tx_data = TGM::Data((UINT8)_rcvdelm);
	else if (datalen == 2) tx_data = TGM::Data((UINT16)_rcvdelm);
	else if (datalen == 4) tx_data = TGM::Data((UINT32)_rcvdelm);
	else if (datalen == 8) tx_data = TGM::Data((UINT64)_rcvdelm);
	else tx_data = TGM::Data((UINT8&)_rcvdelm);
}


template <class TCHeader, class TCPayload, class TRHeader, class TRPayload>
TGM::Map<TRHeader, TRPayload> SISProtocol::transceive_param(TGM::SERCOS_ParamVar _paramvar, const USHORT &_paramnum, BYTE _service, TGM::Data const * const _data, TGM::SERCOS_Datablock _attribute)
{
	/// Build Telegrams
	TGM::Bitfields::Sercos_ParControl	sercos_control(_attribute);
	TGM::Bitfields::Sercos_Param_Ident	param_num(_paramvar, _paramnum);

	// Mapping for SEND Telegram
	TGM::Map<TCHeader, TCPayload>
		tx_tgm(
			// Init header
			TCHeader(SIS_ADDR_MASTER, SIS_ADDR_SLAVE, _service, TGM::Bitfields::Header_Cntrl(TGM::Type_Command)),
			// Init payload
			TCPayload(sercos_control, SIS_ADDR_SLAVE, param_num, *_data)
		);

	// Set payload size
	tx_tgm.mapping.header.set_DatL(tx_tgm.mapping.payload.get_size());
	
	// Calculate Checksum
	tx_tgm.mapping.header.calc_checksum(&tx_tgm.raw);

	if (!check_boundaries(tx_tgm))
		throw SISProtocol::ExceptionGeneric(-1, "Boundaries are out of spec. Telegram is not ready to be sent.");
	
	// Mapping for RECEPTION Telegram
	TGM::Map<TRHeader, TRPayload> rx_tgm;
	
	///  Transceive
	// Send and receive
	transceiving<	TCHeader, TCPayload,
		TRHeader, TRPayload >
		(tx_tgm, rx_tgm);

	return rx_tgm;
}

template<class TCHeader, class TCPayload, class TRHeader, class TRPayload>
TGM::Map<TRHeader, TRPayload> SISProtocol::transceive_list(TGM::SERCOS_ParamVar _paramvar, const USHORT & _paramnum, BYTE _service, USHORT & _element_size, USHORT & _list_offset, TGM::Data const * const _data, TGM::SERCOS_Datablock _attribute)
{																																	 
	/// Build Telegrams
	TGM::Bitfields::Sercos_ParControl	sercos_control(_attribute);
	TGM::Bitfields::Sercos_Param_Ident	param_num(_paramvar, _paramnum);

	// Mapping for SEND Telegram
	TGM::Map<TCHeader, TCPayload>
		tx_tgm(
			// Init header
			TCHeader(SIS_ADDR_MASTER, SIS_ADDR_SLAVE, _service, TGM::Bitfields::Header_Cntrl(TGM::Type_Command)),
			// Init payload
			TCPayload(sercos_control, SIS_ADDR_SLAVE, param_num, _list_offset, _element_size, *_data)
		);

	// Set payload size
	tx_tgm.mapping.header.set_DatL(tx_tgm.mapping.payload.get_size());

	// Calculate Checksum
	tx_tgm.mapping.header.calc_checksum(&tx_tgm.raw);

	if (!check_boundaries(tx_tgm))
		throw SISProtocol::ExceptionGeneric(-1, "Boundaries are out of spec. Telegram is not ready to be sent.");
	
	// Mapping for RECEPTION Telegram
	TGM::Map<TRHeader, TRPayload> rx_tgm;
	
	///  Transceive
	// Send and receive
	transceiving<	TCHeader, TCPayload,
		TRHeader, TRPayload >
		(tx_tgm, rx_tgm);

	return rx_tgm;
}


void SISProtocol::get_parameter_attributes(TGM::SERCOS_ParamVar _paramvar, const USHORT &_paramnum, UINT8& _scalefactor, size_t& _datalen)
{
	STACK;

	/// Communication with Telegrams
	BYTE service = SIS_SERVICE_SERCOS_PARAM_READ;

	auto rx_tgm = transceive_param
					<TGM::Header, TGM::Commands::Sercos_Param, TGM::Header, TGM::Reactions::Sercos_Param>
					(_paramvar, _paramnum, service, new TGM::Data(), TGM::SERCOS_Datablock_attribute);

	/// Read back attribute
	UINT32 attr = rx_tgm.mapping.payload.data.toUINT32();
	TGM::Bitfields::Sercos_Attribute sercos_attribute(attr);

	_datalen = 1;
	if (sercos_attribute.bits.data_len == TGM::SERCOS_DATALEN_listel_2byte) _datalen = 2;
	if (sercos_attribute.bits.data_len == TGM::SERCOS_DATALEN_listel_4byte) _datalen = 4;
	if (sercos_attribute.bits.data_len == TGM::SERCOS_DATALEN_listel_8byte) _datalen = 8;
	if (sercos_attribute.bits.data_len == TGM::SERCOS_DATALEN_param_2byte) _datalen = 2;
	if (sercos_attribute.bits.data_len == TGM::SERCOS_DATALEN_param_4byte) _datalen = 4;
	if (sercos_attribute.bits.data_len == TGM::SERCOS_DATALEN_param_8byte) _datalen = 8;

	_scalefactor = 0xFF & sercos_attribute.bits.scale_factor;
}


template <class TCHeader, class TCPayload, class TRHeader, class TRPayload>
void SISProtocol::transceiving(TGM::Map<TCHeader, TCPayload>& tx_tgm, TGM::Map<TRHeader, TRPayload>& rx_tgm)
{
	STACK;

	// Transceiver lengths
	size_t tx_payload_len = tx_tgm.mapping.payload.get_size();
	size_t tx_header_len = tx_tgm.mapping.header.get_size();

	// Receiver lengths
	size_t rx_header_len = tx_tgm.mapping.header.get_size();
	size_t rx_payload_len = 0;
	
	// Clear buffers
	m_serial.Purge();	

	/// Write
	m_serial.Write(tx_tgm.raw.bytes, tx_header_len + tx_payload_len);
	
	/// Read
	bool bContd = true;
	DWORD rcvd_cur = 0;
	DWORD rcvd_rcnt = 0;

	do
	{
		// Wait for an event
		m_serial.WaitEvent(0, RS232_READ_TIMEOUT);

		// Save event
		const CSerial::EEvent event = m_serial.GetEventType();

		// Handle Break event
		if (event & CSerial::EEventBreak)
			throw SISProtocol::ExceptionTransceiveFailed(CSerial::EEventBreak, "Break event occurred. Transceive has been aborted.", true);

		// Handle error event
		if (event & CSerial::EEventError)
			throw_rs232_error_events(m_serial.GetError());

		// Handle data receive event
		if (event & CSerial::EEventRecv)
		{
			// Read header data
			m_serial.Read(rx_tgm.raw.bytes + rcvd_rcnt, RS232_BUFFER - rcvd_rcnt, &rcvd_cur, 0, RS232_READ_TIMEOUT);

			// Loop back if nothing received
			if (rcvd_cur == 0) continue;

			// Hold back number of already received bytes
			rcvd_rcnt += rcvd_cur;

			// It is assumed that if the number of received bytes is bigger than 4,
			// which is the position of the payload length, the length can be read out.
			if (rcvd_rcnt > 4)
			{
				rx_payload_len = rx_tgm.mapping.header.DatL;
				rx_tgm.mapping.payload.data.set_size(rx_payload_len - rx_tgm.mapping.payload.get_head_size());
			}

			// Length of payload is zero --> No payload received
			if (rx_payload_len == 0)
			{
				std::string tx_hexstream = hexprint_bytestream(tx_tgm.raw.bytes, tx_header_len + tx_payload_len);
				std::string rx_hexstream = hexprint_bytestream(rx_tgm.raw.bytes, rx_header_len);
				throw SISProtocol::ExceptionTransceiveFailed(-1, sformat("Reception Telegram received without payload, but just the header.\nRecption Header bytestream: %s.\nCommand Telegram bytestream was: %s.", rx_hexstream.c_str(), tx_hexstream.c_str()), true);
			}
				

			// Complete Telegram received
			if (rx_header_len + rx_payload_len <= rcvd_rcnt)
			{
				if (rx_tgm.mapping.payload.status)
				{
					std::string tx_hexstream = hexprint_bytestream(tx_tgm.raw.bytes, tx_header_len + tx_payload_len);
					//std::string rx_hexstream = hexprint_bytestream(rx_tgm.raw.bytes, rx_header_len + rx_payload_len);
					throw SISProtocol::ExceptionSISError(rx_tgm.mapping.payload.status, rx_tgm.mapping.payload.error, tx_hexstream);
				}
					
				bContd = false;
			}
		}
		
	} while (bContd);
}


template<class THeader, class TPayload>
bool SISProtocol::check_boundaries(TGM::Map<THeader, TPayload>& _tgm)
{
	STACK;

	size_t tgm_size = _tgm.mapping.header.get_size() + _tgm.mapping.payload.get_size();
	if (tgm_size <= RS232_BUFFER) return true;

	return false;
}


std::string SISProtocol::hexprint_bytestream(const BYTE * _bytestream, const size_t _len)
{
	STACK;

	std::string buf;
	
	for (size_t i = 0; i < _len; i++)
		buf.append(sformat("%02X ", (BYTE)_bytestream[i]));

	return buf;
}

void SISProtocol::throw_rs232_error_events(CSerial::EError _err)
{
	STACK;

	switch (_err)
	{
	case CSerial::EErrorBreak:
		throw SISProtocol::ExceptionTransceiveFailed(CSerial::EErrorBreak, "Break condition occurred. Transceive has been aborted.", true);

	case CSerial::EErrorFrame:
		throw SISProtocol::ExceptionTransceiveFailed(CSerial::EErrorFrame, "Framing error occurred. Transceive has been aborted.", true);

	case CSerial::EErrorIOE:
		throw SISProtocol::ExceptionTransceiveFailed(CSerial::EErrorIOE, "IO device error occurred. Transceive has been aborted.", true);

	case CSerial::EErrorMode:
		throw SISProtocol::ExceptionTransceiveFailed(CSerial::EErrorMode, "Unsupported mode detected. Transceive has been aborted.", true);

	case CSerial::EErrorOverrun:
		throw SISProtocol::ExceptionTransceiveFailed(CSerial::EErrorOverrun, "Buffer overrun detected. Transceive has been aborted.", true);

	case CSerial::EErrorRxOver:
		throw SISProtocol::ExceptionTransceiveFailed(CSerial::EErrorRxOver, "Input buffer overflow detected. Transceive has been aborted.", true);

	case CSerial::EErrorParity:
		throw SISProtocol::ExceptionTransceiveFailed(CSerial::EErrorParity, "Input parity occurred. Transceive has been aborted.", true);

	case CSerial::EErrorTxFull:
		throw SISProtocol::ExceptionTransceiveFailed(CSerial::EErrorTxFull, "Output buffer full. Transceive has been aborted.", true);

	default:
		throw SISProtocol::ExceptionTransceiveFailed(CSerial::EErrorBreak, "Unknown error occurred. Transceive has been aborted.", true);
	}
}


