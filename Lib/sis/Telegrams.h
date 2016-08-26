#ifndef _TELEGRAMS_H_
#define _TELEGRAMS_H_

#include <Windows.h>
#include <vector>
#include <numeric>
#include <type_traits>

#include "Telegrams_Bitfields.h"


namespace TGM
{
	namespace Container
	{
		/// <summary>	Container for Telegram's header in raw data. </summary>
		typedef struct _container_header_t
		{
			/// <summary>	The raw data. </summary>
			char	bytes[8];
			
			/// <summary>	Default constructor. </summary>
			_container_header_t() { clear(); }

			/// <summary>	Clears this object to its blank/initial state. </summary>
			void clear()
			{
				memset(bytes, 0, sizeof(bytes));
			}
		} Header_Contr;

		/// <summary>	Container for Telegram's payload in raw data. </summary>
		typedef struct _container_payload_t
		{
			/// <summary>	The raw data. Size: 239 bytes (255 bytes - 8 bytes {Header}) </summary>
			char	bytes[247];
			
			/// <summary>	Default constructor. </summary>
			_container_payload_t() { clear(); }

			/// <summary>	Clears this object to its blank/initial state. </summary>
			void clear()
			{
				memset(bytes, 0, sizeof(bytes));
			}
		} PL_Contr;

		typedef struct _container_t
		{
			/// <summary>	The raw data. </summary>
			char	payload[255];

			/// <summary>	Default constructor. </summary>
			_container_t() { clear(); }

			/// <summary>	Clears this object to its blank/initial state. </summary>
			void clear()
			{
				memset(payload, 0, sizeof(payload));
			}
		} Contr;
	}


	namespace Maps
	{
		/// <summary>	Templated mapping union to transfer raw TGM header data from/to specialized data class. </summary>
		template <class THeader>
		union MapHeader
		{
		public:
			/// Generic raw data
			Container::Header_Contr raw;

			/// Specialized data class
			struct
			{
				THeader map;
			};

			/// <summary>	Default constructor. </summary>
			MapHeader(THeader _head = THeader()) :
				raw(Container::Header_Contr()),
				map(_head)
			{};
			/// <summary>	Destructor. </summary>
			~MapHeader() {};

			void set(THeader& _map)
			{
				map = _map;
			}

			size_t getSize() 
			{ 
				// Standard header has size of 8 bytes
				if (std::is_same<THeader, TGM::Header>::value) return (size_t)8;
				// Extended header has size of up to 16 bytes
				else return (size_t)16;
			}
		};

		/// <summary>	Templated mapping union to transfer raw TGM payload data from/to specialized data class. </summary>
		template <class TPLHead, class TPLDat>
		union MapPayload
		{
		public:
			/// Generic raw data
			Container::PL_Contr raw;

			/// Specialized data class
			struct
			{
				TPLHead map_plhead;
				TPLDat	map_pldat;
			};
			
			/// <summary>	Default constructor. </summary>
			MapPayload(TPLHead _head = TPLHead(0, 0), TPLDat _dat = TPLDat()) : 
				raw(Container::PL_Contr()), 
				map_plhead(_head),
				map_pldat(_dat)
			{};
			/// <summary>	Destructor. </summary>
			~MapPayload() {};

			void set(TPLHead& _head, TPLDat& _dat)
			{
				map_plhead = _head;
				map_pldat = _dat;
			}

			void setSize(size_t _size)
			{
				raw.size = _size;
			}
		};
		
		/// <summary>	Templated mapping union to transfer raw TGM data from/to specialized data class. </summary>
		template <class THeader, class TPLHead, class TPLDat>
		union Map
		{
		public:
			/// Generic raw data, comprising byte arrays
#pragma pack(push,1)
			struct _raw_t
			{
				Container::Header_Contr header;
				Container::PL_Contr payload;

				_raw_t(Container::Header_Contr& _header = Container::Header_Contr(), Container::PL_Contr& _data = Container::PL_Contr()) :
					header(_header),
					payload(_data)
				{}
			} raw;		
#pragma pack(pop)

			/// Specialized data class, comprising structure payload head and data
#pragma pack(push,1)
			struct _mapping_t
			{
				THeader header;

				struct _payload_t
				{
					TPLHead head;
					TPLDat data;

					_payload_t(TPLHead& _head = TPLHead(), TPLDat& _data = TPLDat()) :
						head(_head), data(_data)
					{};
				} payload;

				_mapping_t(THeader& _header, TPLHead& _head, TPLDat& _data) :
					header(_header),
					payload(_head, _data)
				{};
			} structs;
#pragma pack(pop)

			/// <summary>	Default constructor. </summary>
			Map(THeader& _header = THeader(0, 0), TPLHead& _plhead = TPLHead(), TPLDat& _pldat = TPLDat()) :
				structs(_header, _plhead, _pldat)
			{};
			/// <summary>	Destructor. </summary>
			~Map() {};

			void set(THeader& _header, TPLHead& _plhead, TPLDat& _pldat)
			{
				structs = _mapping_t(_header, _plhead, _pldat);	
			}
		};
	}



	///=================================================================================================
	/// <summary>
	/// The Telegram Header contains all information required for conducting orderly telegram traffic.
	/// </summary>
	///=================================================================================================
#pragma pack(push,1)
	typedef struct _header_t
	{
		/// <summary>	Start symbol: STX (0x02). </summary>
		BYTE StZ = 0x02;

		///=================================================================================================
		/// <summary>
		/// The checksum byte. It is generated by adding all sub-sequential telegram symbols as well as
		/// the start symbol StZ and concluding negation. In other words, the sum of all telegram symbols
		/// always equals 0 if the transmission was successful.
		/// </summary>
		///=================================================================================================
		BYTE CS;

		///=================================================================================================
		/// <summary>
		/// The length of the sub-sequential user data and the variable part are in the frame protocol. Up
		/// to 247 bytes (255 - 7{subaddresses} - 1{running telegram number}) user data can be
		/// transmitted in one telegram.
		/// </summary>
		///=================================================================================================
		BYTE DatL;

		///=================================================================================================
		/// <summary>
		/// Repetition of DatL takes place here. The telegram length is generated from the DatLW and the
		/// fixed part of the frame protocol (byte 1-8), i.e. telegram length = DatLW + 8.
		/// </summary>
		///=================================================================================================
		BYTE DatLW;

		///=================================================================================================
		/// <summary>
		/// Control byte consisting of several bit fields. Use TGM::Bitfields::Cntrl and toByte() for configuration.
		/// </summary>
		///=================================================================================================
		BYTE Cntrl;

		///=================================================================================================
		/// <summary>
		/// This specifies the service that the sender requests from the recipient or that the recipient
		/// has executed.
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
		/// </summary>
		///=================================================================================================
		BYTE Service;

		///=================================================================================================
		/// <summary>
		/// Address of sender:
		/// * AdrS = [0..126]: specifies a single station
		/// * AdrS = 127:  Special address for a SIS master in case of service or
		/// emergencies (this address may not be used during active communication).
		/// </summary>
		///=================================================================================================
		BYTE AdrS;

		///=================================================================================================
		/// <summary>
		/// Address of Recipient:
		/// * AdrE = [0..126]: specifies a single station,
		/// * AdrE = 128: Special address for point - to - point communication(the
		/// recipient's response is not dependent on its actual station number with this special address).
		/// * AdrE = [129..199]: reserved,
		/// * AdrE = [200..253]: addresses logical groups,
		/// * AdrE = 254: specifies a broadcast to all stations on a hierarchical
		/// level(this address can only be listed once, as the last address in the list),
		/// * AdrE = 255: specifies a global broadcast.
		/// Telegrams with AdrE = [200..255] are not answered with a response telegram.
		/// </summary>
		///=================================================================================================
		BYTE AdrE;

		/// <summary>	Default constructor. </summary>
		_header_t(BYTE _addr_master, BYTE _addr_slave, BYTE _service = 0, TGM::Bitfields::Cntrl _cntrl = TGM::Bitfields::Cntrl()) :
			StZ(0x02),
			CS(0),
			DatL(get_size()),
			DatLW(get_size()),
			Cntrl(_cntrl.toByte()),
			Service(_service),
			AdrS(_addr_master),
			AdrE(_addr_slave)
		{}

		///=================================================================================================
		/// <summary>	Gets the sum without carry of all header bytes for checksum calculation. </summary>
		///
		/// <param name="exclude_cs">	(Optional) true to exclude checksum from calculation. </param>
		///
		/// <returns>	The sum. </returns>
		///=================================================================================================
		BYTE get_sum(bool exclude_cs = true)
		{
			BYTE res = StZ + DatL + DatLW + Cntrl + Service + AdrS + AdrE;

			if (!exclude_cs) res += CS;

			return res;
		}

		size_t get_size() { return sizeof(*this); }

		///=================================================================================================
		/// <summary>
		/// Sets length of Telegram, stored in DatL and DatLW (copy). By default, the length of the telegram is defined by
		/// the payload length (head + data).
		/// </summary>
		///
		/// <param name="_payload_len">	Length of the payload. </param>
		///=================================================================================================
		void set_DatL(size_t _payload_len) { DatL = DatLW = (BYTE)_payload_len; }

		///=================================================================================================
		/// <summary>
		/// Calculates the Telegram checksum, stored in CS. The calculated checksum will automatically assigned to CS.
		/// </summary>
		///
		/// <param name="_payload">	   	[in] Container of payload (head + data) with the raw data. </param>
		/// <param name="_payload_len">	Length of the payload. </param>
		///=================================================================================================
		void calc_checksum(Container::PL_Contr * _payload, size_t _payload_len)
		{
			// Set length of telegram
			set_DatL(_payload_len);

			// Sum of payload
			BYTE sum_of_payload = 0;
			for (int i = 0; i < _payload_len; i++)
				sum_of_payload += (BYTE)_payload->bytes[i];
			
			// Calc difference
			BYTE diff_cs = get_sum() + sum_of_payload;

			// Calc negation and assign to checksum (Byte 1)
			CS = (BYTE)0 - diff_cs;
		}
	} Header;
#pragma pack(pop)

	/// <summary>	Extended Telegram Header to be used for Routing and Sequential Telegrams. </summary>
#pragma pack(push,1)
	typedef struct _header_ext_t : Header
	{
		///=================================================================================================
		/// <summary>
		/// Expanded part of the telegram header. Subaddress 1 of recipient. Bit 0-2 of Cntrl byte:  000.
		/// </summary>
		///=================================================================================================
		BYTE AdrES1;

		///=================================================================================================
		/// <summary>
		/// Expanded part of the telegram header. Subaddress 2 of recipient. Bit 0-2 of Cntrl byte:  001.
		/// </summary>
		///=================================================================================================
		BYTE AdrES2;

		///=================================================================================================
		/// <summary>
		/// Expanded part of the telegram header. Subaddress 3 of recipient. Bit 0-2 of Cntrl byte:  010.
		/// </summary>
		///=================================================================================================
		BYTE AdrES3;

		///=================================================================================================
		/// <summary>
		/// Expanded part of the telegram header. Subaddress 4 of recipient. Bit 0-2 of Cntrl byte:  011.
		/// </summary>
		///=================================================================================================
		BYTE AdrES4;

		///=================================================================================================
		/// <summary>
		/// Expanded part of the telegram header. Subaddress 5 of recipient. Bit 0-2 of Cntrl byte:  100.
		/// </summary>
		///=================================================================================================
		BYTE AdrES5;

		///=================================================================================================
		/// <summary>
		/// Expanded part of the telegram header. Subaddress 6 of recipient. Bit 0-2 of Cntrl byte:  101.
		/// </summary>
		///=================================================================================================
		BYTE AdrES6;

		///=================================================================================================
		/// <summary>
		/// Expanded part of the telegram header. Subaddress 7 of recipient. Bit 0-2 of Cntrl byte:  110.
		/// </summary>
		///=================================================================================================
		BYTE AdrES7;

		///=================================================================================================
		/// <summary>
		/// Expanded part of the telegram header. Sequential telegram number (packet number) , if bit 3
		/// in Cntrl byte is set.
		/// </summary>
		///=================================================================================================
		BYTE PaketN;

	} Header_Ext;
#pragma pack(pop)


	namespace Commands
	{
		///=================================================================================================
		/// <summary>
		/// Representation of the payload header for a Subservice command. A Command Telegram is for regular subservices,
		/// such communication init, or device identification. User for master communication (active communicator).
		/// </summary>
		///=================================================================================================
#pragma pack(push,1)
		typedef struct _subservice_pl_head_t
		{
			BYTE	address;
			BYTE	subservice;

			///=================================================================================================
			/// <summary>	Constructor. </summary>
			///
			/// <param name="_addr">	  	The recipient address. </param>
			/// <param name="_subservice">	The subservice number. </param>
			///=================================================================================================
			_subservice_pl_head_t(BYTE _addr = 0, BYTE _subservice = 0) : address(_addr), subservice(_subservice) {}

			/// <summary>	Clears this object to its blank/initial state. </summary>
			void clear() { address = subservice = 0; }

			size_t get_size() { return sizeof(*this); }

		} Subservice_PL_Header;
#pragma pack(pop)


		///=================================================================================================
		/// <summary>
		/// Representation of the payload data for a Subservice command. A Command Telegram is for regular subservices, such
		/// communication init, or device identification. User for master communication (active communicator).
		/// </summary>
		///=================================================================================================
#pragma pack(push,1)
		typedef struct _subservice_pl_data_t
		{
			BYTE	payload[237];
			size_t	size;

			_subservice_pl_data_t& operator<<(const BYTE& rhs)
			{
				payload[size++] = rhs;
				return *this;
			}

			/// <summary>	Default constructor. </summary>
			_subservice_pl_data_t(std::vector<BYTE> _dat = std::vector<BYTE>())
			{ 
				clear(); 

				for (std::vector<BYTE>::iterator it = _dat.begin(); it != _dat.end(); ++it)
					operator<<(*it);
			}

			/// <summary>	Clears this object to its blank/initial state. </summary>
			void clear()
			{
				memset(payload, 0, sizeof(payload));
				size = 0;
			}

			size_t get_size() { return size; }
			void set_size(size_t _size) { size = _size; }

		} Subservice_PL_Dat;
#pragma pack(pop)
	}


	namespace Reactions
	{
#pragma pack(push,1)
		///=================================================================================================
		/// <summary>
		/// Representation of the payload header for a Subservice reaction. A Reaction Telegram is for regular subservices,
		/// such communication init, or device identification. This telegram is responded after successful execution of
		/// previous Command Telegram.
		/// </summary>
		///=================================================================================================
		typedef struct _subservice_pl_head_t
		{
			BYTE	status;
			BYTE	address;
			BYTE	subservice;
		} Subservice_PL_Header;
#pragma pack(pop)

#pragma pack(push,1)
		///=================================================================================================
		/// <summary>
		/// Representation of the payload data for a Subservice reaction. A Reaction Telegram is for regular subservices,
		/// such communication init, or device identification. This telegram is responded after successful execution of
		/// previous Command Telegram.
		/// </summary>
		///=================================================================================================
		typedef struct _subservice_pl_data_t
		{
			BYTE	error;
		} Subservice_PL_Dat;
#pragma pack(pop)
	}
}







//
/////=================================================================================================
///// <summary>
///// Sercos Command Telegram. Used for master communication (active communicator) of Sercos
///// parameters.
///// 
///// The common SIS services are
///// *   0x10: Read access on a SERCOS parameter, supporting consecutive telegrams in case
///// of(long) lists.
///// *   0x11: Read access on a list segment of SERCOS parameter, supporting no consecutive
///// telegram.
///// *   0x12: Read access on the actual SERCOS phase.
///// *   0x1D: Switch of the SERCOS phase(Write access).
///// *   0x1E: Write access on a list segment of a SERCOS parameter, supporting no consecutive
///// telegram.
///// *   0x1F: Write  access  on  a  SERCOS  parameter, supporting consecutive telegrams in case
///// of(long) lists.
///// </summary>
/////=================================================================================================
//typedef struct sercos_param_command_tgm_t
//{
//	/// <summary>	Telegram header. </summary>
//	TGM_Header header;
//
//	/// <summary>	Sercos control. Size: 8 bit. Set coding by Telegrams::Sercos_Control. </summary>
//	BYTE control;
//
//	///=================================================================================================
//	/// <summary>
//	/// The unit address of a drive is read in the command telegram and copied into the response
//	/// telegram. For direct SIS communication with drives supporting SIS interface, unit address is
//	/// the same as the SIS address of the receiver. Otherwise, the  SIS  address  is  related  to
//	/// the  motion control and the unit address to the drive.
//	/// </summary>
//	///=================================================================================================
//	BYTE unit_addr;
//
//	/// <summary>	Identifier for the parameter. Size: 16 bit. Set coding by Telegrams::Sercos_Param_Ident. </summary>
//	USHORT param_ident;
//
//	/// <summary>	Payload used for user data that a specific subservice may require. </summary>
//	std::vector<BYTE> payload;
//
//
//}  Sercos_Param_Cmd_TGM;
//




#endif /* _TELEGRAMS_H_ */