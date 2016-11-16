#ifndef _SISPROTOCOL_H_
#define _SISPROTOCOL_H_

#include <Windows.h>
#include <string>
#include <mutex>

#include "debug.h"
#include "helpers.h"
#include "RS232.h"
#include "Telegrams.h"



#define RS232_BUFFER			254
#define RS232_READ_LOOPS_MAX	100
#define RS232_READ_TIMEOUT		1000


/// Defines address master.
#define SIS_ADDR_MASTER			0x00
/// Defines sis address slave. '128' is used for peer-to-peer communication.
#define SIS_ADDR_SLAVE			0x01
/// Address unit. For Indradrive, this value can be found at P-0-4022.
#define SIS_ADDR_UNIT			0x01


/// Defines the maximum iterations of checking the successful executing of a Indradrive command
#define MAX_COMMANDCHECK_ITERATIONS 300


/// Class to hold functions an members for the SIS protocol support.
class SISProtocol
{
public:
	/// Generic exception handling for SIS Protocol.
	class ExceptionGeneric;
	/// Specific exception handling of SIS Protocol for failed transceiving.
	class ExceptionTransceiveFailed;
	/// Generic exception handling of SIS Protocol
	class ExceptionSISError;

	/// Values that represent identifiers to be used for SIS services.
	typedef enum SIS_SERVICES
	{
		SIS_SERVICE_INIT_COMM = 0x03,
		SIS_SERVICE_SEQUENTIALOP = 0x04,

		SIS_SERVICE_SERCOS_PARAM_READ = 0x10,
		SIS_SERVICE_SERCOS_LIST_READ = 0x11,
		SIS_SERVICE_SERCOS_READ_PHASE = 0x12,
		SIS_SERVICE_SERCOS_SWITCH_PHASE = 0x1D,
		SIS_SERVICE_SERCOS_LIST_WRITE = 0x1E,
		SIS_SERVICE_SERCOS_PARAM_WRITE = 0x1F
	} SIS_SERVICES;

	/// Baudrate mask that can be utilized for the TypeCommand Telegram Subservice 0x07.
	/// @todo This enum is not yet implemented into the code.
	typedef enum BAUDRATE
	{
		/// An enum constant representing the option for 9600 baud
		Baud_9600	= 0b00000000,
		/// An enum constant representing the option for 19200 baud
		Baud_19200	= 0b00000001,
		/// An enum constant representing the option for 38400 baud
		Baud_38400	= 0b00000010,
		/// An enum constant representing the option for 57600 baud
		Baud_57600	= 0b00000100,
		/// An enum constant representing the option for 115200 baud
		Baud_115200 = 0b00001000
	} BAUDRATE;

	/// Default constructor.
	SISProtocol();
	/// Destructor.
	virtual ~SISProtocol();

	

	void open(const wchar_t * _port = L"COM1");
	void close();

	void set_baudrate(BAUDRATE baudrate);

	void read_parameter(TGM::SercosParamVar _paramvar, USHORT _paramnum, UINT32& _rcvddata);
	void read_parameter(TGM::SercosParamVar _paramvar, USHORT _paramnum, UINT64& _rcvddata);
	void read_parameter(TGM::SercosParamVar _paramvar, USHORT _paramnum, DOUBLE& _rcvddata);
	void read_parameter(TGM::SercosParamVar _paramvar, USHORT _paramnum, char _rcvddata[TGM_SIZEMAX_PAYLOAD]);

	void read_listelm(TGM::SercosParamVar _paramvar, USHORT _paramnum, USHORT _elm_pos, UINT32& _rcvdelm);
	void read_listelm(TGM::SercosParamVar _paramvar, USHORT _paramnum, USHORT _elm_pos, UINT64& _rcvdelm);
	void read_listelm(TGM::SercosParamVar _paramvar, USHORT _paramnum, USHORT _elm_pos, DOUBLE& _rcvdelm);
	
	void write_parameter(TGM::SercosParamVar _paramvar, USHORT _paramnum, const UINT32 _data);
	void write_parameter(TGM::SercosParamVar _paramvar, USHORT _paramnum, const UINT64 _data);
	void write_parameter(TGM::SercosParamVar _paramvar, USHORT _paramnum, const DOUBLE _data);
	
	void write_listelm(TGM::SercosParamVar _paramvar, USHORT _paramnum, USHORT _elm_pos, const UINT32 _rcvdelm);
	void write_listelm(TGM::SercosParamVar _paramvar, USHORT _paramnum, USHORT _elm_pos, const UINT64 _rcvdelm);
	void write_listelm(TGM::SercosParamVar _paramvar, USHORT _paramnum, USHORT _elm_pos, const DOUBLE _rcvdelm);

	void execute_command(TGM::SercosParamVar _paramvar, USHORT _paramnum);


private:

	inline void get_parameter_attributes(TGM::SercosParamVar _paramvar, const USHORT &_paramnum, UINT8& _scalefactor, size_t& _datalen);
	inline void get_parameter_status(const TGM::SercosParamVar _paramvar, const USHORT &_paramnum, TGM::SercosCommandstatus& _datastatus);

	/// Transceive parameter.
	///
	/// @tparam	TCHeader 	Type of the Telegram TypeCommand header.
	/// @tparam	TCPayload	Type of the Telegram TypeCommand payload.
	/// @tparam	TRHeader 	Type of the Telegram Reception header.
	/// @tparam	TRPayload	Type of the Telegram Reception payload.
	/// @param	_paramvar 	SERCOS Parameter variant (S, or P), defined by TGM::SercosParamVar.
	/// @param	_paramnum 	SERCOS Parameter number.
	/// @param	_service  	Service ID, defined by SIS_SERVICES.
	/// @param	_data	  	(Optional) The Bytes.
	/// @param	_attribute	(Optional) The Datablock.
	///
	/// @return	A TGM::Map&lt;TRHeader,TRPayload&gt;
	///
	/// @sa SIS_SERVICES
	/// @sa TGM::SercosParamVar
	template <class TCHeader, class TCPayload, class TRHeader, class TRPayload>
	TGM::Map<TRHeader, TRPayload> transceive_param(TGM::SercosParamVar _paramvar, const USHORT &_paramnum, BYTE _service, TGM::Data const * const _data = new TGM::Data(), TGM::SercosDatablock _attribute = TGM::Datablock_OperationData);

	template <class TCHeader, class TCPayload, class TRHeader, class TRPayload>
	TGM::Map<TRHeader, TRPayload> transceive_list(TGM::SercosParamVar _paramvar, const USHORT &_paramnum, BYTE _service, USHORT & _element_size, USHORT & _list_offset, TGM::Data const * const _data = new TGM::Data(), TGM::SercosDatablock _attribute = TGM::Datablock_OperationData);

	template <class THeader, class TPayload>
	inline bool check_boundaries(TGM::Map<THeader, TPayload>& _tgm);

	static std::string hexprint_bytestream(const BYTE * _bytestream, const size_t _len);

	inline INT64 get_sized_data(TGM::Data& rx_data, const size_t &datalen);
	inline void set_sized_data(TGM::Data& tx_data, const size_t &datalen, UINT64& _rcvdelm);
	inline void set_parameter_listsize(TGM::SercosParamVar param_variant, USHORT& param_number, const size_t& datalen, const USHORT& segment_position, bool retain_following_segments = false);

private:

	template <class TCHeader, class TCPayload, class TRHeader, class TRPayload>
	void transceiving(TGM::Map<TCHeader, TCPayload>& tx_tgm, TGM::Map<TRHeader, TRPayload>& rx_tgm);

	static void throw_rs232_error_events(CSerial::EError _err);

private:
	CSerial m_serial;

	std::mutex mutex_sis;
};

/// Generic exceptions for SIS protocol.
///
/// @sa	std::exception
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

/// Specific exception handling of SIS Protocol transceiving failed.
///
/// @sa	SISProtocol::ExceptionGeneric
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

/// Specific exception handling of SIS Protocol error codes.
///
/// @sa	SISProtocol::ExceptionGeneric
class SISProtocol::ExceptionSISError : public SISProtocol::ExceptionGeneric
{
public:
	ExceptionSISError(
		int _status,
		int _code,
		const std::string _bytestream,
		bool _warning = false) :

		ExceptionGeneric(_status, std::string(), _warning),
		m_errorcode(_code),
		m_bytestream(_bytestream)
	{}
	~ExceptionSISError() throw() {}

	virtual const char* what() const throw ()
	{
#ifdef NDEBUG
		return str2char(sformat("(Return code: %d) SIS Protocol Error code returned has been received: 0x%04X.\nOriginal Telegram bytestream: %s", m_status, m_errorcode, m_bytestream.c_str()));
#else
		const char* ex = str2char(sformat("(Return code: %d) SIS Protocol Error code returned has been received: 0x%04X.\nOriginal Telegram bytestream: %s", m_status, m_errorcode, m_bytestream.c_str()));
		OutputDebugStringA((LPCSTR)ex);
		return ex;
#endif
	}

	int get_errorcode() { return m_errorcode; }

protected:
	int m_errorcode;
	std::string m_bytestream;
};

#endif /* _SISPROTOCOL_H_ */