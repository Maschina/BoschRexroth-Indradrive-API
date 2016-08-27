#ifndef _SISPROTOCOL_H_
#define _SISPROTOCOL_H_

#include <Windows.h>

#include "RS232.h"
#include "Telegrams.h"
#include "helpers.h"


#define RS232_BUFFER			254
#define RS232_READ_LOOPS_MAX	100
#define RS232_READ_TIMEOUT		1000

/// <summary>	Defines address master. </summary>
#define SIS_ADDR_MASTER			0x00
/// <summary>	Defines sis address slave. '128' is used for peer-to-peer communication. </summary>
#define SIS_ADDR_SLAVE			0x01
/// <summary>	Address unit. For Indradrive, this value can be found at P-0-4022. </summary>
#define SIS_ADDR_UNIT			0x01

#define SIS_SERVICE_INIT_COMM			0x03

#define SIS_SERVICE_SERCOS_PARAM_READ	0x10
#define SIS_SERVICE_SERCOS_LIST_READ	0x11
#define SIS_SERVICE_SERCOS_READ_PHASE	0x12
#define SIS_SERVICE_SERCOS_SWITCH_PHASE	0x1D
#define SIS_SERVICE_SERCOS_LIST_WRITE	0x1E
#define SIS_SERVICE_SERCOS_PARAM_WRITE	0x1F


class SISProtocol
{
public:
	/// <summary>	Generic exception handling for SIS Protocol. </summary>
	class ExceptionGeneric;
	/// <summary>	Specific exception handling of SIS Protocol for failed transceiving. </summary>
	class ExceptionTransceiveFailed;

	/// <summary>	Baudrate mask that can be utilized for the Command Telegram Subservice 0x07 </summary>
	enum init_set_mask_baudrate
	{
		Baud_9600	= 0b00000000,
		Baud_19200	= 0b00000001,
		Baud_38400	= 0b00000010,
		Baud_57600	= 0b00000100,
		Baud_115200 = 0b00001000
	};

	/// <summary>	Default constructor. </summary>
	SISProtocol();
	/// <summary>	Destructor. </summary>
	virtual ~SISProtocol();


	void open(const char * _port = "COM1");
	void close();

	void get_sisversion();
	void set_baudrate(init_set_mask_baudrate _baudrate);
	void write_parameter(TGM::Param_Variant _paramvar, const char* _data, size_t _data_len);

private:
	CSerial m_serial;

	template <class TCHeader, class TCPLHead, class TCPLDat, class TRHeader, class TRPLHead, class TRPLDat>
	void transceive(TGM::Map<TCHeader, TCPLHead, TCPLDat>& tx_tgm, TGM::Map<TRHeader, TRPLHead, TRPLDat>& rx_tgm);

	void concat_data(char * _dest, const char * _header, size_t _header_len, const char * _payload, size_t _payload_len);
	void split_data(const char * _src, char * _header, size_t _header_len, char * _payload, size_t _payload_len);

	static void throw_rs232_error_events(CSerial::EError _err);
};


/// <summary>	Generic exceptions for SIS protocol. </summary>
class SISProtocol::ExceptionGeneric : public std::exception
{
public:
	bool warning;

	ExceptionGeneric(
		const std::string _src_func,
		const char* _src_file,
		const unsigned long _src_line,
		int _status,
		const std::string _trace_log,
		bool _warning = false) :

		m_src_func(_src_func),
		m_src_file(_src_file),
		m_src_line(_src_line),
		m_status(_status),
		m_trace_log(_trace_log),
		warning(_warning)
	{}

	virtual const char* what() const throw ()
	{
#ifdef NDEBUG
		return str2char(sformat("SIS Protocol exception @ %s: STATUS=%d, TRACE='%s'", m_src_func.c_str(), m_status, m_trace_log.c_str()));
#else
		return str2char(sformat("[%s @ line %d] SIS Protocol exception @ %s: STATUS=%d, TRACE='%s'", m_src_file, m_src_line, m_src_func.c_str(), m_status, m_trace_log.c_str()));
#endif
	}

	int get_status() { return m_status; }

protected:
	std::string m_src_func;
	const char* m_src_file;
	const unsigned long m_src_line;

	///=================================================================================================
	/// <summary>
	/// For Win32 API commands, most likely representation of the System Error Codes:
	/// # https://msdn.microsoft.com/de-de/library/windows/desktop/ms681381(v=vs.85).aspx.
	/// </summary>
	///=================================================================================================
	int m_status;
	std::string m_trace_log;
};


/// <summary>	Specific exception handling of SIS Protocol transceiving failed. </summary>
class SISProtocol::ExceptionTransceiveFailed : public SISProtocol::ExceptionGeneric
{
public:
	ExceptionTransceiveFailed(
		const std::string _src_func,
		const char* _src_file,
		const unsigned long _src_line,
		int _status,
		const std::string _trace_log,
		bool _warning = false) :

		ExceptionGeneric(_src_func, _src_file, _src_line, _status, _trace_log, _warning)
	{}
	~ExceptionTransceiveFailed() throw() {}

	virtual const char* what() const throw ()
	{
#ifdef NDEBUG
		return str2char(sformat("SIS Protocol reception failed: STATUS=%d, TRACE='%s'", m_status, m_trace_log.c_str()));
#else
		return str2char(sformat("[%s @ line %d] SIS Protocol reception failed: STATUS=%d, TRACE='%s'", m_src_file, m_src_line, m_status, m_trace_log.c_str()));
#endif
	}
};

#endif /* _SISPROTOCOL_H_ */