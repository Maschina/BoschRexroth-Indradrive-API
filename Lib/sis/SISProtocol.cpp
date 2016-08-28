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
	CSerial::EBaudrate cbaudrate = CSerial::EBaud19200;
	CSerial::EDataBits cdata = CSerial::EData8;
	CSerial::EParity cparity = CSerial::EParEven;
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
	try
	{
		m_serial.Close();
	}
	catch (CSerial::ExceptionGeneric &ex)
	{
		throw;
	}
}

void SISProtocol::get_sisversion()
{
	/// Build Telegrams	
	// Mapping for SEND Telegram
	TGM::Map<TGM::Header, TGM::Commands::Subservice_Head, TGM::Commands::Subservice_Data>
		tx_tgm(
			// Init header
			TGM::Header(SIS_ADDR_MASTER, SIS_ADDR_SLAVE, SIS_SERVICE_INIT_COMM, TGM::Bitfields::Cntrl(TGM::TGM_Type_Command)),
			// Init payload header
			TGM::Commands::Subservice_Head(SIS_ADDR_UNIT, 0x02 /* Subservice code for SIS Version read-out */),
			// Init payload data
			TGM::Commands::Subservice_Data()
			);

	// Calculate Checksum
	size_t payload_len = tx_tgm.structs.payload.head.get_size() + tx_tgm.structs.payload.data.get_size();
	tx_tgm.structs.header.calc_checksum(&tx_tgm.raw.payload, payload_len);

	// Mapping for RECEPTION Telegram
	TGM::Map<TGM::Header, TGM::Reactions::Subservice_Head, TGM::Reactions::Subservice_Data> rx_tgm;

	///  Transceive
	// Send and receive
	transceive<	TGM::Header, TGM::Commands::Subservice_Head, TGM::Commands::Subservice_Data,
		TGM::Header, TGM::Reactions::Subservice_Head, TGM::Reactions::Subservice_Data >
		(tx_tgm, rx_tgm);
}

void SISProtocol::set_baudrate(init_set_mask_baudrate _baudrate)
{
	/// Build Telegrams
	// Mapping for SEND Telegram
	TGM::Map<TGM::Header, TGM::Commands::Subservice_Head, TGM::Commands::Subservice_Data>
		tx_tgm(
			// Init header
			TGM::Header(SIS_ADDR_MASTER, SIS_ADDR_SLAVE, SIS_SERVICE_INIT_COMM, TGM::Bitfields::Cntrl(TGM::TGM_Type_Command)),
			// Init payload header
			TGM::Commands::Subservice_Head(SIS_ADDR_UNIT, 0x07 /* Subservice code for baudrate setting */),
			// Init payload data
			TGM::Commands::Subservice_Data({ (BYTE)_baudrate })
		);
	
	// Calculate Checksum
	size_t payload_len = tx_tgm.structs.payload.head.get_size() + tx_tgm.structs.payload.data.get_size();
	tx_tgm.structs.header.calc_checksum(&tx_tgm.raw.payload, payload_len);

	// Mapping for RECEPTION Telegram
	TGM::Map<TGM::Header, TGM::Reactions::Subservice_Head, TGM::Reactions::Subservice_Data> rx_tgm;

	///  Transceive
	// Send and receive
	transceive<	TGM::Header, TGM::Commands::Subservice_Head, TGM::Commands::Subservice_Data,
				TGM::Header, TGM::Reactions::Subservice_Head, TGM::Reactions::Subservice_Data >
				(tx_tgm, rx_tgm);

	int i = 0;
	i++;
}

void SISProtocol::write_parameter(TGM::Param_Variant _paramvar, const char * _data, size_t _data_len)
{
}

template <class TCHeader, class TCPLHead, class TCPLDat, class TRHeader, class TRPLHead, class TRPLDat>
void SISProtocol::transceive(TGM::Map<TCHeader, TCPLHead, TCPLDat>& tx_tgm, TGM::Map<TRHeader, TRPLHead, TRPLDat>& rx_tgm)
{
	char tx_buffer[RS232_BUFFER];
	char rx_buffer[RS232_BUFFER];

	size_t payload_len = tx_tgm.structs.payload.head.get_size() + tx_tgm.structs.payload.data.get_size();
	size_t header_len = tx_tgm.structs.header.get_size();
	size_t tx_len = header_len + payload_len;
	size_t tx_header_len = tx_tgm.structs.header.get_size();
	size_t tx_payload_len = 0;

	// Copy from map to buffer
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
		m_serial.WaitEvent(0, RS232_READ_TIMEOUT);

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
			// Read header data
			m_serial.Read(rx_buffer, RS232_BUFFER, &rcvd_cur, 0, RS232_READ_TIMEOUT);
			
			// Copy buffer back to map
			split_data(rx_buffer+rcvd_rcnt, rx_tgm.raw.header.bytes, tx_header_len, rx_tgm.raw.payload.bytes, tx_payload_len);
			rcvd_rcnt += rcvd_cur;

			// It is assumed that if the number of received bytes is bigger than 4,
			// which is the position of the payload length, the length can be read out.
			if (rcvd_rcnt > 4)
				tx_payload_len = rx_tgm.structs.header.DatL;

			// Complete Telegram received
			if (tx_header_len + tx_payload_len <= rcvd_rcnt)
			{
				bContd = false;

				if (rx_tgm.structs.payload.head.status)
					throw SISProtocol::ExceptionTransceiveFailed("SISProtocol::transceive", __FILE__, __LINE__, rx_tgm.structs.payload.head.status, sformat("Telegram with status message received. Error byte: %d.", rx_tgm.structs.payload.data.error), true);
			}
		}
	} while (bContd);
}


void SISProtocol::concat_data(char * _dest, const char * _header, size_t _header_len, const char * _payload, size_t _payload_len)
{
	memcpy_s(_dest, _header_len, _header, _header_len);
	memcpy_s(_dest+_header_len, _payload_len, _payload, _payload_len);
}

void SISProtocol::split_data(const char * _src, char * _header, size_t _header_len, char * _payload, size_t _payload_len)
{
	memcpy_s(_header, _header_len, _src, _header_len);
	memcpy_s(_payload, _payload_len, _src+_header_len, _payload_len);
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


