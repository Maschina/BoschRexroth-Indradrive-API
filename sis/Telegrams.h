/// @file
/// Contains struct definitions for different types of Telegrams

#ifndef _TELEGRAMS_H_
#define _TELEGRAMS_H_


#include <Windows.h>
#include <vector>
#include <algorithm>
#include <numeric>
#include <type_traits>

#include "Telegrams_Bitfields.h"


#define TGM_SIZE_HEADER		8
#define TGM_SIZE_HEADER_EXT	16
#define TGM_SIZEMAX_PAYLOAD	246
#define TGM_SIZEMAX			254



/// Grouping structs/enums/unions for a SIS Telegram.
namespace TGM
{
	/// Struct to hold payload Bytes in a command payload. Payload Bytes is third part of a regular Telegram: Header +
	/// Payload Bytes + Payload header.
	typedef struct Data
	{
		/// Actual payload Bytes [TGM_SIZEMAX_PAYLOAD].
		BYTE	Bytes[TGM_SIZEMAX_PAYLOAD];
		/// Size of the payload Bytes.
		size_t	Size;

		/// Default constructor.
		///
		/// @param	_PayloadData	(Optional) The data vector.
		Data(std::vector<BYTE> _data = std::vector<BYTE>())
		{
			clear();

			for (std::vector<BYTE>::iterator it = _data.begin(); it != _data.end(); ++it)
				operator<<(*it);

			Size = _data.size();
		}

		/// Constructor.
		///
		/// @param	_PayloadData	Single data byte.
		Data(UINT8 _data)
		{
			clear();

			operator<<(_data);
		}

		/// Constructor.
		///
		/// @param	_PayloadData	Single data word (2 bytes).
		Data(UINT16 _data)
		{
			clear();

			operator<<(_data & 0xFF);
			operator<<((_data & 0xFF00) >> 8);
		}

		/// Constructor.
		///
		/// @param	_PayloadData	Single data integer (4 bytes).
		Data(UINT32 _data)
		{
			clear();

			operator<<(_data & 0xFF);
			operator<<((_data & 0xFF00) >> 8);
			operator<<((_data & 0xFF0000) >> 16);
			operator<<((_data & 0xFF000000) >> 24);
		}

		/// Constructor.
		///
		/// @param	_PayloadData	Single UINT64 data (8 bytes).
		Data(UINT64 _data)
		{
			clear();

			operator<<(_data & 0xFF);
			operator<<((_data & 0xFF00) >> 8);
			operator<<((_data & 0xFF0000) >> 16);
			operator<<((_data & 0xFF000000) >> 24);
			operator<<((_data & 0xFF00000000) >> 32);
			operator<<((_data & 0xFF0000000000) >> 40);
			operator<<((_data & 0xFF000000000000) >> 48);
			operator<<((_data & 0xFF00000000000000) >> 54);
		}

		/// Ats the given index.
		///
		/// @param	_idx	The index.
		///
		/// @return	Data byte.
		BYTE at(UINT32 _idx)
		{
			return Bytes[_idx];
		}

		/// Converts this object to a vector.
		///
		/// @return	This object as a std::vector&lt;BYTE&gt;
		std::vector<BYTE> toVector()
		{
			std::vector<BYTE> out;

			for (int i = 0; i < Size; i++)
				out.push_back(Bytes[i]);

			return out;
		}

		/// Converts this object to an uint 64.
		///
		/// @return	This object as an UINT64.
		UINT64 toUINT64()
		{
			UINT64 out = 0;

			for (int i = 0; i < std::min<size_t>(Size, 8); i++)
				out |= Bytes[i] << (i * 8);

			return out;
		}

		/// Converts this object to an uint 32.
		///
		/// @return	This object as an UINT32.
		UINT32 toUINT32()
		{
			UINT32 out = 0;

			for (int i = 0; i < std::min<size_t>(Size, 4); i++)
				out |= Bytes[i] << (i * 8);

			return out;
		}

		/// Converts this object to an uint 16.
		///
		/// @return	This object as an UINT16.
		UINT16 toUINT16()
		{
			UINT16 out = 0;

			for (int i = 0; i < std::min<size_t>(Size, 2); i++)
				out |= Bytes[i] << (i * 8);

			return out;
		}

		/// Converts this object to an uint 8.
		///
		/// @return	This object as an UINT8.
		UINT8 toUINT8()
		{
			return toBYTE();
		}

		/// Converts this object to a byte.
		///
		/// @return	This object as a BYTE.
		BYTE toBYTE()
		{
			return Size > 0 ? (BYTE)Bytes[0] : (BYTE)0;
		}

		/// Clears this object to its blank/initial state.
		void clear()
		{
			memset(Bytes, 0, sizeof(Bytes));
			Size = 0;
		}

		/// Bitwise left shift operator.
		///
		/// @param	rhs	The right hand side.
		///
		/// @return	The shifted result.
		Data& operator<<(const BYTE& rhs)
		{
			Bytes[Size++] = rhs;
			return *this;
		}

		/// Gets the size.
		///
		/// @return	The size.
		size_t get_size() { return Size; }

		/// Sets a size.
		///
		/// @param	_size	The size.
		void set_size(size_t _size) { Size = _size; }

	} Data;


	/// Container for Telegram in raw Bytes.
	typedef struct Bytestream
	{
		/// The raw Bytes. Size: 254 bytes.
		BYTE Bytes[TGM_SIZEMAX];

		/// Default constructor.
		Bytestream() { clear(); }

		/// Clears this object to its blank/initial state.
		void clear()
		{
			memset(Bytes, 0, sizeof(Bytes));
		}
	} Bytestream;


	/// Templated mapping union to transfer raw TGM Bytes from/to specialized Bytes class.
	template <class THeader, class TPayload>
	union Map
	{
	public:
		/// Generic raw Bytes, comprising byte arrays
		Bytestream Raw;

		/// Specialized Bytes class, comprising structure payload head and Bytes
#pragma pack(push,1)
		struct Mapping
		{
			/// The Telegram header.
			THeader		Header;
			/// The Telegram payload.
			TPayload	Payload;

			/// Constructor.
			///
			/// @param [in]	_header 	The Telegram header.
			/// @param [in]	_payload	The Telegram payload.
			Mapping(THeader& _header, TPayload _payload) :
				Header(_header),
				Payload(_payload)
			{};
		} Mapping;
#pragma pack(pop)

		/// Default constructor.
		///
		/// @param [in]	_header 	(Optional) The Telegram header.
		/// @param [in]	_payload	(Optional) The Telegram payload.
		Map(THeader& _header = THeader(), TPayload& _payload = TPayload()) :
			Mapping(_header, _payload)
		{};
		/// Destructor.
		~Map() {};

		/// Sets the header/payload even after initialization.
		///
		/// @param [in]	_header 	The Telegram header.
		/// @param [in]	_payload	The Telegram payload.
		void set(THeader& _header, TPayload& _payload)
		{
			Mapping = Mapping(_header, _payload);
		}
	};


#pragma pack(push,1)
	/// The Telegram Header contains all information required for conducting orderly telegram traffic..
	typedef struct Header
	{
		/// Start symbol: STX (0x02).
		BYTE StZ = 0x02;

		/// The checksum byte. It is generated by adding all sub-sequential telegram symbols as well as the start symbol
		/// StZ and concluding negation. In other words, the sum of all telegram symbols always equals 0 if the transmission
		/// was successful.
		BYTE CS;

		/// The length of the sub-sequential user Bytes and the variable part are in the frame protocol. Up to 247 bytes
		/// (255 - 7{subaddresses} - 1{running telegram number}) user Bytes can be transmitted in one telegram.
		BYTE DatL;

		/// Repetition of DatL takes place here. The telegram length is generated from the DatLW and the fixed part of
		/// the frame protocol (byte 1-8), i.e. telegram length = DatLW + 8.
		BYTE DatLW;

		/// Control byte consisting of several bit fields. Use TGM::Bitfields::Cntrl and toByte() for configuration.
		BYTE Cntrl;

		/// This specifies the service that the sender requests from the recipient or that the recipient has executed.
		/// * 0x00 ... 0x0F     General services:
		/// * 0x00				User identification
		/// * 0x01				Data transmission aborted
		/// * 0x02				Flash operation
		/// * 0x03				Initializing SIS communication
		/// * 0x04				Executing a list of SIS services
		/// * 0x0F				Token passing
		/// * 0x10 ... 0x7F     temporarily reserved
		/// * 0x80 ... 0x8F     Special services for ECODRIVE
		/// * 0x90 ... 0x9F     Special services for SYNAX
		/// * 0xA0 ... 0xAF     Special services for MT - CNC or .MTC200
		/// * 0xB0 ... 0xBF     Special services for ISP200
		/// * 0xC0 ... 0xCF     Special services for CLC - GPS
		/// * 0xD0 ... 0xDF     Special services for HMI - System
		/// * 0xE0 ... 0xEF     Special services for DISC
		/// * 0xF0 ... 0xFF     temporarily reserved.
		BYTE Service;

		/// Address of sender:
		/// * AdrS = [0..126]: specifies a single station
		/// * AdrS = 127:  Special address for a SIS master in case of service or
		/// emergencies (this address may not be used during active communication).
		BYTE AdrS;

		/// Address of Recipient:
		/// * AdrE = [0..126]: specifies a single station,
		/// * AdrE = 128: Special address for point-to-point communication (the recipient's response is not dependent on its
		/// actual station number with this special address).
		/// * AdrE = [129..199]: reserved,
		/// * AdrE = [200..253]: addresses logical groups,
		/// * AdrE = 254: specifies a broadcast to all stations on a hierarchical level(this address can only be listed once,
		/// as the last address in the list),
		/// * AdrE = 255: specifies a global broadcast.
		/// Telegrams with AdrE = [200..255] are not answered with a response telegram.
		BYTE AdrE;

		/// Default constructor.
		///
		/// @param	_addr_master	(Optional) The address master id.
		/// @param	_addr_slave 	(Optional) The address slave id.
		/// @param	_service		(Optional) The service id.
		/// @param	_cntrl			(Optional) The Control Byte, represented by TGM::Bitfields::HeaderControl.
		///
		/// @sa	TGM::Bitfields::HeaderControl
		Header(BYTE _addr_master = 0, BYTE _addr_slave = 0, BYTE _service = 0, TGM::Bitfields::HeaderControl _cntrl = TGM::Bitfields::HeaderControl()) :
			StZ(0x02),
			CS(0),
			DatL(get_size()),
			DatLW(get_size()),
			Cntrl(_cntrl.Value),
			Service(_service),
			AdrS(_addr_master),
			AdrE(_addr_slave)
		{}

		/// Gets the sum without carry of all header bytes for checksum calculation.
		///
		/// @param	exclude_cs	(Optional) true to exclude checksum from calculation.
		///
		/// @return	The sum.
		BYTE get_sum(bool exclude_cs = true)
		{
			BYTE res = StZ + DatL + DatLW + Cntrl + Service + AdrS + AdrE;

			if (!exclude_cs) res += CS;

			return res;
		}

		/// Gets the size.
		///
		/// @return	The size.
		size_t get_size() { return sizeof(*this); }

		/// Sets length of Telegram, stored in DatL and DatLW (copy). By default, the length of the telegram is defined
		/// by the payload length (head + Bytes).
		///
		/// @param	_payload_len	Length of the payload.
		inline void set_DatL(size_t _payload_len) { DatL = DatLW = (BYTE)_payload_len; }

		/// Gets Telegram's length.
		///
		/// @return	The length of Telegram.
		inline size_t get_DatL() { return DatL; }

		/// Calculates the Telegram checksum, stored in CS. The calculated checksum will automatically assigned to CS.
		/// This function will use DatL parameter for the appropriate length determination.
		///
		/// @param 	   	_payload_len	Length of the payload.
		///
		/// @param [in]	_payload	Bytestream of payload (head + Bytes) with the raw Bytes.
		void calc_checksum(TGM::Bytestream * _payload)
		{
			// Sum of payload
			BYTE sum_of_payload = 0;
			for (int i = TGM_SIZE_HEADER; i < TGM_SIZE_HEADER + get_DatL(); i++)
				sum_of_payload += (BYTE)_payload->Bytes[i];
			
			// Calc difference
			BYTE diff_cs = get_sum() + sum_of_payload;

			// Calc negation and assign to checksum (Byte 1)
			CS = (BYTE)0 - diff_cs;
		}
	} Header;
#pragma pack(pop)


#pragma pack(push,1)
	/// Extended Telegram Header to be used for Routing and Sequential Telegrams.
	///
	/// @sa	Header
	typedef struct HeaderExt : Header
	{
		/// Expanded part of the telegram header. Subaddress 1 of recipient. Bit 0-2 of Cntrl byte:  000.
		BYTE AdrES1;

		/// Expanded part of the telegram header. Subaddress 2 of recipient. Bit 0-2 of Cntrl byte:  001.
		BYTE AdrES2;

		/// Expanded part of the telegram header. Subaddress 3 of recipient. Bit 0-2 of Cntrl byte:  010.
		BYTE AdrES3;

		/// Expanded part of the telegram header. Subaddress 4 of recipient. Bit 0-2 of Cntrl byte:  011.
		BYTE AdrES4;

		/// Expanded part of the telegram header. Subaddress 5 of recipient. Bit 0-2 of Cntrl byte:  100.
		BYTE AdrES5;

		/// Expanded part of the telegram header. Subaddress 6 of recipient. Bit 0-2 of Cntrl byte:  101.
		BYTE AdrES6;

		/// Expanded part of the telegram header. Subaddress 7 of recipient. Bit 0-2 of Cntrl byte:  110.
		BYTE AdrES7;

		/// Expanded part of the telegram header. Sequential telegram number (packet number) , if bit 3 in Cntrl byte is
		/// set.
		BYTE PaketN;

	} HeaderExt;
#pragma pack(pop)


/// Grouping SIS Telegram Payload struct definitions for commands.
	namespace Commands
	{
		
#pragma pack(push,1)
		/// Representation of the PAYLOAD for a Subservice command. A Command Telegram is for regular subservices, such
		/// communication init, or device identification. User for master communication (active communicator).
		typedef struct Subservice
		{
			/// The recipient address.
			BYTE	RecipientAddr;
			/// The subservice number.
			BYTE	ServiceNumber;
			/// The Payload content.
			Data	Bytes;

			/// Constructor.
			///
			/// @param	_addr	   	(Optional) The recipient address.
			/// @param	_subservice	(Optional) The subservice number.
			/// @param	_PayloadData	   	(Optional) The data.
			Subservice(
				BYTE _addr = 0, 
				BYTE _subservice = 0, 
				Data _data = Data()) : 
				RecipientAddr(_addr), 
				ServiceNumber(_subservice),
				Bytes(_data)
			{}

			/// Clears this object to its blank/initial state.
			void clear() { RecipientAddr = ServiceNumber = 0; }

			/// Gets size of Payload Header
			///
			/// @return	The Payload Header size.
			size_t get_head_size() { return 2; }

			/// Gets the Payload size including Payload Header size.
			///
			/// @return	The Payload size.
			size_t get_size() { return get_head_size() + Bytes.get_size(); }

		} Subservice;
#pragma pack(pop)


#pragma pack(push,1)
		/// Sercos Command Telegram used for reading/writing single parameter from/to slave.
		typedef struct SercosParam
		{
			/// Sercos control. Size: 8 bit. Set coding by TGM::Bitfields::SercosParamControl and toByte().
			BYTE Control;

			/// The unit address of a drive is read in the command telegram and copied into the response telegram. For direct
			/// SIS communication with drives supporting SIS interface, unit address is the same as the SIS address of the
			/// receiver. Otherwise, the  SIS  address  is  related  to the  motion control and the unit address to the drive.
			BYTE UnitAddr;

			BYTE ParamType;

			/// Identifier for the parameter. Size: 16 bit. Set coding by TGM::Bitfields::SercosParamIdent and
			/// toByte().
			USHORT ParamNum;

			/// Payload Bytes.
			Data Bytes;

			/// Constructor.
			///
			/// @param	_ControlByte		(Optional) Control Byte.
			/// @param	_unit_addr  	(Optional) Unit address, which is the same as the SIS address of the receiver.
			/// @param	_ParamIdent	(Optional) Parameter Identifier (e.g. S-0-4000).
			/// @param	_PayloadData			(Optional) Payload data.
			SercosParam(
				TGM::Bitfields::SercosParamControl _control = TGM::Bitfields::SercosParamControl(), 
				BYTE _unit_addr = 0,
				TGM::Bitfields::SercosParamIdent _param_ident = TGM::Bitfields::SercosParamIdent(), 
				TGM::Data _data = Data()) :
				Control(_control.Value),
				UnitAddr(_unit_addr),
				ParamType(0),
				ParamNum(_param_ident.Value),
				Bytes(_data)
			{}

			/// Clears this object to its blank/initial state.
			void clear() 
			{ 
				Control = 0;
				UnitAddr = 0;
				ParamType = 0;
				ParamNum = 0;
				Bytes.clear();
			}

			/// Gets size of Payload Header
			///
			/// @return	The Payload Header size.
			size_t get_head_size() { return 5; }

			/// Gets the Payload size including Payload Header size.
			///
			/// @return	The Payload size.
			size_t get_size() { return get_head_size() + Bytes.get_size(); }

		}  SercosParam;
#pragma pack(pop)


#pragma pack(push,1)
		/// Sercos Command Telegram used for reading/writing single elements in lists from/to slave.
		typedef struct SercosList
		{
			/// Sercos control. Size: 8 bit. Set coding by TGM::Bitfields::SercosParamControl and toByte().
			BYTE Control;

			/// The unit address of a drive is read in the command telegram and copied into the response telegram. For direct
			/// SIS communication with drives supporting SIS interface, unit address is the same as the SIS address of the
			/// receiver. Otherwise, the  SIS  address  is  related  to the  motion control and the unit address to the drive.
			BYTE UnitAddr;

			BYTE ParamType;

			/// Identifier for the parameter. Size: 16 bit. Set coding by TGM::Bitfields::SercosParamIdentification and
			/// toByte().
			USHORT ParamNum;

			/// Defines the offset in bytes of the segment that has to be read. For example: The 11th element of a list
			/// consisting of 4-byte elements should be handeled --> ListOffset=0x0028.
			USHORT ListOffset;

			/// Size of the element to be handeled. For example: The 11th element of a list consisting of 4-byte elements
			/// should be handeled --> SegmentSize=0x0004.
			USHORT SegmentSize;

			/// Payload Bytes.
			Data Bytes;

			/// Constructor.
			///
			/// @param	_ControlByte 	(Optional) Control Byte.
			/// @param	_unit_addr   	(Optional) Unit address, which is the same as the SIS address of the receiver.
			/// @param	_ParamIdent  	(Optional) Parameter Identifier.
			/// @param	_ListOffset  	(Optional) List offset.
			/// @param	_SegmentSize	(Optional) Size of a single segment.
			/// @param	_PayloadData		 	(Optional) Payload data.
			SercosList(
				TGM::Bitfields::SercosParamControl _ControlByte = TGM::Bitfields::SercosParamControl(),
				BYTE _unit_addr = 0,
				TGM::Bitfields::SercosParamIdent _ParamIdent = TGM::Bitfields::SercosParamIdent(),
				USHORT _ListOffset = 0,
				USHORT _SegmentSize = 0,
				TGM::Data _PayloadData = Data()) :

				Control(_ControlByte.Value),
				UnitAddr(_unit_addr),
				ParamType(0),
				ParamNum(_ParamIdent.Value),
				ListOffset(_ListOffset),
				SegmentSize(_SegmentSize),
				Bytes(_PayloadData)
			{}

			/// Clears this object to its blank/initial state.
			void clear()
			{
				Control = UnitAddr = ParamNum = ListOffset = SegmentSize = 0;
				Bytes.clear();
			}

			/// Gets size of payload header.
			///
			/// @return	Size of payload header.
			size_t get_head_size() { return 9; }

			/// Gets the Payload size including Payload Header size.
			///
			/// @return	The Payload size.
			size_t get_size() { return get_head_size() + Bytes.get_size(); }

		}  SercosList;
#pragma pack(pop)
	}



/// Grouping SIS Telegram Payload struct definitions for reception.
	namespace Reactions
	{
#pragma pack(push,1)
		/// Representation of the payload for a Subservice reaction. A Reaction Telegram is for regular subservices, such
		/// communication init, or device identification. This telegram is responded after successful execution of previous
		/// Command Telegram.
		typedef struct Subservice
		{
			/// Recipient status.
			BYTE	Status;

			/// Address of the recipient.
			BYTE	RecipientAddr;

			/// SIS service number.
			BYTE	ServiceNumber;

			/// Payload Bytes, or error byte.
			union
			{
				Data	Bytes;
				BYTE	Error;
			};

			/// Default constructor.
			Subservice() :
				Status(1),
				RecipientAddr(0),
				ServiceNumber(0),
				Error(0)
			{}

			/// Clears this object to its blank/initial state.
			void clear()
			{
				Status = 1;
				RecipientAddr = ServiceNumber = 0;
				Bytes.clear();
			}

			/// Gets payload header size.
			///
			/// @return	The payload head size.
			size_t get_head_size() { return 3; }

			/// Gets the Payload size including Payload Header size.
			///
			/// @return	The Payload size.
			size_t get_size() { return get_head_size() + Bytes.get_size(); }

		} Subservice;
#pragma pack(pop)


#pragma pack(push,1)
		/// Representation of the payload for a Sercos Parameter reaction. A Reaction Telegram is for regular subservices,
		/// such communication init, or device identification. This telegram is responded after successful execution of
		/// previous Command Telegram.
		typedef struct SercosParam
		{
			/// Recipient status.
			BYTE Status;

			/// Sercos control. Size: 8 bit. Set coding by TGM::Bitfields::SercosParamControl and toByte().
			BYTE Control;

			/// The unit address of a drive is read in the command telegram and copied into the response telegram. For direct
			/// SIS communication with drives supporting SIS interface, unit address is the same as the SIS address of the
			/// receiver. Otherwise, the  SIS  address  is  related  to the  motion control and the unit address to the drive.
			BYTE UnitAddr;

			/// Payload Bytes, or error byte.
			union
			{
				Data	Bytes;
				USHORT	Error;
			};

			/// Default constructor.
			SercosParam() :
				Status(1),
				Control(0),
				UnitAddr(0),
				Bytes(TGM::Data())
			{}

			/// Clears this object to its blank/initial state.
			void clear()
			{
				Status = 1;
				Control = UnitAddr = 0;
				Bytes.clear();
			}

			/// Gets payload header size.
			///
			/// @return	The payload header size.
			size_t get_head_size() { return 3; }

			/// Gets the Payload size including Payload Header size.
			///
			/// @return	The Payload size.
			size_t get_size() { return get_head_size() + Bytes.get_size(); }

		} SercosParam;
#pragma pack(pop)


#pragma pack(push,1)
		/// Sercos Command Telegram used for reading/writing single elements in lists from/to slave..
		typedef struct SercosList
		{
			/// Recipient status.
			BYTE Status;

			/// Sercos control. Size: 8 bit. Set coding by TGM::Bitfields::SercosParamControl and toByte().
			BYTE Control;

			/// The unit address of a drive is read in the command telegram and copied into the response telegram. For direct
			/// SIS communication with drives supporting SIS interface, unit address is the same as the SIS address of the
			/// receiver. Otherwise, the  SIS  address  is  related  to the  motion control and the unit address to the drive.
			BYTE UnitAddr;
			
			/// Payload Bytes, or error byte.
			union
			{
				Data Bytes;
				USHORT Error;
			};
			
			/// Default constructor.
			SercosList() :
				Status(1),
				Control(0),
				UnitAddr(0),
				Bytes(TGM::Data())
			{}

			/// Clears this object to its blank/initial state.
			void clear()
			{
				Status = 1;
				Control = UnitAddr = 0;
				Bytes.clear();
			}

			/// Gets payload header size.
			///
			/// @return	The payload header size.
			size_t get_head_size() { return 3; }

			/// Gets the Payload size including Payload Header size.
			///
			/// @return	The Payload size.
			size_t get_size() { return get_head_size() + Bytes.get_size(); }

		}  SercosList;
#pragma pack(pop)
	}
}


#endif /* _TELEGRAMS_H_ */