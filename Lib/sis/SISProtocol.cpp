#include "SISProtocol.h"



SISProtocol::SISProtocol()
{
}


SISProtocol::~SISProtocol()
{
}


void SISProtocol::open(const char * _port)
{
	STACK;

	LPCTSTR cport = (LPCTSTR)char2wchar(_port);
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

	prepare_and_transceive(tx_tgm, rx_tgm);
}


void SISProtocol::read_parameter(const std::string & _paramvar, USHORT _paramnum, std::vector<BYTE>& _rcvddata)
{
	STACK;

	TGM::SERCOS_ParamVar var;

	if (_paramvar == "S")
		var = TGM::SERCOS_Param_S;
	else if (_paramvar == "P")
		var = TGM::SERCOS_Param_P;
	else
		throw SISProtocol::ExceptionGeneric(-1, sformat("Wrong paramvar parameter given: %s", _paramvar), true);

	read_parameter(var, _paramnum, _rcvddata);
}

void SISProtocol::read_parameter(TGM::SERCOS_ParamVar _paramvar, USHORT _paramnum, std::vector<BYTE>& _rcvddata)
{
	STACK;

	/// Build Telegrams
	TGM::Bitfields::Sercos_Control		sercos_control;
	TGM::Bitfields::Sercos_Param_Ident	param_num(_paramvar, _paramnum);

	// Mapping for SEND Telegram
	TGM::Map<TGM::Header, TGM::Commands::Sercos_Param>
		tx_tgm(
			// Init header
			TGM::Header(SIS_ADDR_MASTER, SIS_ADDR_SLAVE, SIS_SERVICE_SERCOS_PARAM_READ, TGM::Bitfields::Header_Cntrl(TGM::Type_Command)),
			// Init payload
			TGM::Commands::Sercos_Param(sercos_control, SIS_ADDR_SLAVE, param_num)
		);

	// Mapping for RECEPTION Telegram
	TGM::Map<TGM::Header, TGM::Reactions::Sercos_Param> rx_tgm;

	/// Transceive
	prepare_and_transceive(tx_tgm, rx_tgm);

	/// Read back response
	_rcvddata = rx_tgm.mapping.payload.data.toVector();
}


void SISProtocol::write_parameter(const std::string& _paramvar, USHORT _paramnum, std::vector<BYTE>& _data)
{
	STACK;

	TGM::SERCOS_ParamVar var;

	if (_paramvar == "S")
		var = TGM::SERCOS_Param_S;
	else if (_paramvar == "P")
		var = TGM::SERCOS_Param_P;
	else
		throw SISProtocol::ExceptionGeneric(-1, sformat("Wrong paramvar parameter given: %s", _paramvar), true);

	write_parameter(var, _paramnum, _data);
}

void SISProtocol::write_parameter(TGM::SERCOS_ParamVar _paramvar, USHORT _paramnum, std::vector<BYTE>& _data)
{
	STACK;

	/// Build Telegrams
	TGM::Bitfields::Sercos_Control		sercos_control;
	TGM::Bitfields::Sercos_Param_Ident	param_num(_paramvar, _paramnum);
			
	// Mapping for SEND Telegram
	TGM::Map<TGM::Header, TGM::Commands::Sercos_Param>
		tx_tgm(
			// Init header
			TGM::Header(SIS_ADDR_MASTER, SIS_ADDR_SLAVE, SIS_SERVICE_SERCOS_PARAM_WRITE, TGM::Bitfields::Header_Cntrl(TGM::Type_Command)),
			// Init payload
			TGM::Commands::Sercos_Param(sercos_control, SIS_ADDR_SLAVE, param_num, TGM::Data(_data))
		);

	// Mapping for RECEPTION Telegram
	TGM::Map<TGM::Header, TGM::Reactions::Sercos_Param> rx_tgm;

	/// Transceive
	prepare_and_transceive(tx_tgm, rx_tgm);
}


template <class TCHeader, class TCPayload, class TRHeader, class TRPayload>
inline void SISProtocol::prepare_and_transceive(TGM::Map<TCHeader, TCPayload>& tx_tgm, TGM::Map<TRHeader, TRPayload>& rx_tgm)
{
	STACK;

	// Calculate Checksum
	size_t payload_len = tx_tgm.mapping.payload.get_size();
	tx_tgm.mapping.header.calc_checksum(&tx_tgm.raw, payload_len);

	if (!check_boundaries(tx_tgm))
		throw SISProtocol::ExceptionGeneric(-1, "Boundaries are out of spec. Telegram is not ready to be sent.");

	///  Transceive
	// Send and receive
	transceive<	TCHeader, TCPayload,
				TRHeader, TRPayload >
				(tx_tgm, rx_tgm);
}


template <class TCHeader, class TCPayload, class TRHeader, class TRPayload>
void SISProtocol::transceive(TGM::Map<TCHeader, TCPayload>& tx_tgm, TGM::Map<TRHeader, TRPayload>& rx_tgm)
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
					std::string rx_hexstream = hexprint_bytestream(rx_tgm.raw.bytes, rx_header_len);
					throw SISProtocol::ExceptionTransceiveFailed(rx_tgm.mapping.payload.status, sformat("Telegram with status message received. Error byte: 0x%04x.\nCommand Telegram bytestream was: %s.\nRecption Telegram bytestream was: %s.", rx_tgm.mapping.payload.error, tx_hexstream.c_str(), rx_hexstream.c_str()), true);
				}
					
				bContd = false;
			}
		}
		
	} while (bContd);
}


template<class THeader, class TPayload>
inline bool SISProtocol::check_boundaries(TGM::Map<THeader, TPayload>& _tgm)
{
	STACK;

	size_t tgm_size = _tgm.mapping.header.get_size() + _tgm.mapping.payload.get_size();
	if (tgm_size <= RS232_BUFFER) return true;

	return false;
}


std::string SISProtocol::hexprint_bytestream(const BYTE * _bytestream, const size_t _len)
{
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


