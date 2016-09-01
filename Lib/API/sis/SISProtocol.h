#ifndef _SISPROTOCOL_H_
#define _SISPROTOCOL_H_

#include <Windows.h>
#include <string>

#include "poppydebugtools.h"
#include "helpers.h"
#include "RS232.h"
#include "Telegrams.h"



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
#define SIS_SERVICE_SEQUENTIALOP		0x04

#define SIS_SERVICE_SERCOS_PARAM_READ	0x10
#define SIS_SERVICE_SERCOS_LIST_READ	0x11
#define SIS_SERVICE_SERCOS_READ_PHASE	0x12
#define SIS_SERVICE_SERCOS_SWITCH_PHASE	0x1D
#define SIS_SERVICE_SERCOS_LIST_WRITE	0x1E
#define SIS_SERVICE_SERCOS_PARAM_WRITE	0x1F


class SISProtocol
{
public:
	INT32 foo;

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

	/// API FUNCTIONS

	void open(const char * _port = "COM1");
	void close();

	void set_baudrate(init_set_mask_baudrate _baudrate);

	void read_parameter(TGM::SERCOS_ParamVar _paramvar, USHORT _paramnum, UINT32& _rcvddata);
	void read_parameter(TGM::SERCOS_ParamVar _paramvar, USHORT _paramnum, UINT64& _rcvddata);

	void read_listelm(TGM::SERCOS_ParamVar _paramvar, USHORT _paramnum, USHORT _elm_pos, UINT32 & _rcvdelm);
	void read_listelm(TGM::SERCOS_ParamVar _paramvar, USHORT _paramnum, USHORT _elm_pos, UINT64 & _rcvdelm);
	
	void write_parameter(TGM::SERCOS_ParamVar _paramvar, USHORT _paramnum, UINT32& _data);
	void write_parameter(TGM::SERCOS_ParamVar _paramvar, USHORT _paramnum, UINT64& _data);

	void write_listelm(TGM::SERCOS_ParamVar _paramvar, USHORT _paramnum, USHORT _elm_pos, UINT32& _rcvdelm);
	void write_listelm(TGM::SERCOS_ParamVar _paramvar, USHORT _paramnum, USHORT _elm_pos, UINT64& _rcvdelm);

private:
	/// TELEGRAM SUPPORTING FUNCTIONS

	inline void get_attributes(TGM::SERCOS_ParamVar _paramvar, const USHORT &_paramnum, UINT8& _scalefactor, size_t& _datalen);

	template <class TCHeader, class TCPayload, class TRHeader, class TRPayload>
	inline void prepare_and_transceive(TGM::Map<TCHeader, TCPayload>& tx_tgm, TGM::Map<TRHeader, TRPayload>& rx_tgm);

	template <class THeader, class TPayload>
	inline bool check_boundaries(TGM::Map<THeader, TPayload>& _tgm);

	static std::string hexprint_bytestream(const BYTE * _bytestream, const size_t _len);

	inline UINT64 get_sized_data(TGM::Data& rx_data, const size_t &datalen);
	inline void set_sized_data(TGM::Data& tx_data, const size_t &datalen, UINT64 & _rcvdelm);

private:
	/// COMMUNICATION FUNCTIONS

	template <class TCHeader, class TCPayload, class TRHeader, class TRPayload>
	void transceive(TGM::Map<TCHeader, TCPayload>& tx_tgm, TGM::Map<TRHeader, TRPayload>& rx_tgm);

	static void throw_rs232_error_events(CSerial::EError _err);

private:
	CSerial m_serial;
};


/// <summary>	Generic exceptions for SIS protocol. </summary>
class SISProtocol::ExceptionGeneric : public std::exception
{
public:
	bool warning;

	ExceptionGeneric(
		int _status,
		const std::string _trace_log,
		bool _warning = false) :

		m_status(_status),
		m_message(_trace_log),
		warning(_warning)
	{}

	virtual const char* what() const throw ()
	{
#ifdef NDEBUG
		return str2char(sformat("SIS Protocol exception caused: %s ### STATUS=0x%04x (%d) ### MESSAGE='%s'", Stack::GetTraceString().c_str(), m_status, m_status, m_message.c_str()));
#else
		const char* ex = str2char(sformat("SIS Protocol exception caused: %s ### STATUS=0x%04x (%d) ### MESSAGE='%s'", Stack::GetTraceString().c_str(), m_status, m_status, m_message.c_str()));
		OutputDebugStringA((LPCSTR)ex);
		return ex;
#endif
	}

	int get_status() { return m_status; }

protected:
	int m_status;

	std::string m_message;
};


/// <summary>	Specific exception handling of SIS Protocol transceiving failed. </summary>
class SISProtocol::ExceptionTransceiveFailed : public SISProtocol::ExceptionGeneric
{
public:
	ExceptionTransceiveFailed(
		int _status,
		const std::string _message,
		bool _warning = false) :

		ExceptionGeneric(_status, _message, _warning)
	{}
	~ExceptionTransceiveFailed() throw() {}

	virtual const char* what() const throw ()
	{
#ifdef NDEBUG
		return str2char(sformat("SIS Protocol reception fail caused: STATUS=0x%04x (%d) ### MESSAGE='%s'", m_status, m_status, m_message.c_str()));
#else
		const char* ex = str2char(sformat("SIS Protocol reception fail caused: STATUS=0x%04x (%d) ### MESSAGE='%s'", m_status, m_status, m_message.c_str()));
		OutputDebugStringA((LPCSTR)ex);
		return ex;
#endif
	}
};

#endif /* _SISPROTOCOL_H_ */