#include "SISProtocol.h"



SISProtocol::SISProtocol()
{
}


SISProtocol::~SISProtocol()
{
}


void SISProtocol::open(const char * _port)
{
	LPCTSTR cport = (LPCTSTR)char2wchar(_port);
	CSerial::EBaudrate cbaudrate = CSerial::EBaud9600;
	CSerial::EDataBits cdata = CSerial::EData8;
	CSerial::EParity cparity = CSerial::EParEven;
	CSerial::EStopBits cstopbits = CSerial::EStop1;
	CSerial::EHandshake chandshake = CSerial::EHandshakeOff;

	try
	{
		CSerial::CheckPort(cport);

		m_serial.Open(cport, RS232_BUFFER, RS232_BUFFER, false /* no overlapped */);
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
	/// Build Command Telegram	
	// Mapping
	TGM::Maps::Map<TGM::Header, TGM::Commands::Subservice_PL_Header, TGM::Commands::Subservice_PL_Dat>
		tx_tgm(
			// Init header
			TGM::Header(SIS_SERVICE_INIT_COMM, TGM::Bitfields::Cntrl(TGM::TGM_Type_Command)),
			// Init payload header
			TGM::Commands::Subservice_PL_Header(SIS_ADDR_SLAVE, 0x07 /* Subservice code for baudrate setting */),
			// Init payload data
			TGM::Commands::Subservice_PL_Dat({ (BYTE)_baudrate })
		);
	
	// Calculate Checksum
	size_t payload_len = tx_tgm.structs.payload.head.get_size() + tx_tgm.structs.payload.data.get_size();
	tx_tgm.structs.header.calc_checksum(&tx_tgm.raw.payload, payload_len);

	/// Transceive Telegram
	TGM::Maps::Map<TGM::Header, TGM::Reactions::Subservice_PL_Header, TGM::Reactions::Subservice_PL_Dat> rx_tgm;

	// Send and receive
	transceive<	TGM::Header, TGM::Commands::Subservice_PL_Header, TGM::Commands::Subservice_PL_Dat,
				TGM::Header, TGM::Reactions::Subservice_PL_Header, TGM::Reactions::Subservice_PL_Dat >
				(tx_tgm, rx_tgm);
}

void SISProtocol::write_parameter(TGM::Param_Variant _paramvar, const char * _data, size_t _data_len)
{
}

template <class TCHeader, class TCPLHead, class TCPLDat, class TRHeader, class TRPLHead, class TRPLDat>
void SISProtocol::transceive(TGM::Maps::Map<TCHeader, TCPLHead, TCPLDat>& tx_tgm, TGM::Maps::Map<TRHeader, TRPLHead, TRPLDat>& rx_tgm)
{
	char tx_buffer[RS232_BUFFER];
	char rx_buffer[RS232_BUFFER];

	size_t payload_len = tx_tgm.structs.payload.head.get_size() + tx_tgm.structs.payload.data.get_size();
	size_t header_len = tx_tgm.structs.header.get_size();
	size_t tx_len = header_len + payload_len;

	concat_data(tx_buffer, tx_tgm.raw.header.bytes, header_len, tx_tgm.raw.payload.bytes, payload_len);

	// Clear buffers
	m_serial.Purge();

	/// Write
	m_serial.Write(tx_buffer, tx_len);

	/// Read
	bool bContd = true;
	DWORD rcvd_cur = 0;
	DWORD rcvd_rcnt = 0;

	do
	{
		// Wait for an event
		m_serial.WaitEvent();

		// Save event
		const CSerial::EEvent event = m_serial.GetEventType();

		// Handle Break event
		if (event & CSerial::EEventBreak)
			throw SISProtocol::ExceptionTransceiveFailed("SISProtocol::transceive", __FILE__, __LINE__, CSerial::EEventBreak, "Break event occurred. Transceive has been aborted.", true);

		// Handle error event
		if (event & CSerial::EEventError)
			throw_rs232_error_events(m_serial.GetError());

		// Handle data receive event
		if (event & CSerial::EEventRecv)
		{
			// Read data, until there is nothing left
			unsigned int idos = 0;
			do
			{
				// Read data from the COM-port
				m_serial.Read(rx_buffer, rx_len_min, &rcvd_cur);

				rcvd_rcnt += rcvd_cur;

				if (rcvd_rcnt >= rx_len_min) bContd = true;

				if (++idos > RS232_READ_LOOPS_MAX)
					throw SISProtocol::ExceptionTransceiveFailed("SISProtocol::transceive", __FILE__, __LINE__, -1, sformat("Received payload is not equal to expected number of payload: %d vs %d", rcvd_rcnt, rx_len_min));

			} while (rcvd_rcnt < rx_len_min);
		}
	} while (bContd);
}


void SISProtocol::concat_data(char * _dest, const char * _header, size_t _header_len, const char * _payload, size_t _payload_len)
{
	memcpy_s(_dest, _header_len, _header, _header_len);
	memcpy_s(_dest+_header_len, _payload_len, _payload, _payload_len);
}

void SISProtocol::throw_rs232_error_events(CSerial::EError _err)
{
	switch (_err)
	{
	case CSerial::EErrorBreak:
		throw SISProtocol::ExceptionTransceiveFailed("SISProtocol::transceive", __FILE__, __LINE__, CSerial::EErrorBreak, "Break condition occurred. Transceive has been aborted.", true);

	case CSerial::EErrorFrame:
		throw SISProtocol::ExceptionTransceiveFailed("SISProtocol::transceive", __FILE__, __LINE__, CSerial::EErrorFrame, "Framing error occurred. Transceive has been aborted.", true);

	case CSerial::EErrorIOE:
		throw SISProtocol::ExceptionTransceiveFailed("SISProtocol::transceive", __FILE__, __LINE__, CSerial::EErrorIOE, "IO device error occurred. Transceive has been aborted.", true);

	case CSerial::EErrorMode:
		throw SISProtocol::ExceptionTransceiveFailed("SISProtocol::transceive", __FILE__, __LINE__, CSerial::EErrorMode, "Unsupported mode detected. Transceive has been aborted.", true);

	case CSerial::EErrorOverrun:
		throw SISProtocol::ExceptionTransceiveFailed("SISProtocol::transceive", __FILE__, __LINE__, CSerial::EErrorOverrun, "Buffer overrun detected. Transceive has been aborted.", true);

	case CSerial::EErrorRxOver:
		throw SISProtocol::ExceptionTransceiveFailed("SISProtocol::transceive", __FILE__, __LINE__, CSerial::EErrorRxOver, "Input buffer overflow detected. Transceive has been aborted.", true);

	case CSerial::EErrorParity:
		throw SISProtocol::ExceptionTransceiveFailed("SISProtocol::transceive", __FILE__, __LINE__, CSerial::EErrorParity, "Input parity occurred. Transceive has been aborted.", true);

	case CSerial::EErrorTxFull:
		throw SISProtocol::ExceptionTransceiveFailed("SISProtocol::transceive", __FILE__, __LINE__, CSerial::EErrorTxFull, "Output buffer full. Transceive has been aborted.", true);

	default:
		throw SISProtocol::ExceptionTransceiveFailed("SISProtocol::transceive", __FILE__, __LINE__, CSerial::EErrorBreak, "Unknown error occurred. Transceive has been aborted.", true);
	}
}


