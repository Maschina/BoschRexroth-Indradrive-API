/// @file
/// Contains enums, structs and unions to make Telegram creation, transmission and reception as flexible as possible.
/// For example, by defining different Telegram structs (such as Telegram Command header , Telegram Reception payload, etc) and using unions for each of these types, Telegrams can be easily created, debugged and later provided to a transmission caller in raw byte format.

#ifndef _TELEGRAMS_BITFIELDS_H_
#define _TELEGRAMS_BITFIELDS_H_

#include <Windows.h>
#include <vector>


/// Grouping structs/enums/unions for a SIS Telegram.
namespace TGM
{
	/// Values that represent Telegram header types.
	enum HeaderType : BYTE {
		/// Telegram for command
		TypeCommand, 
		/// Telegram for reception
		TypeReaction 
	};

	/// Values that represent SERCOS Parameter variants.
	enum SercosParamVar : BYTE {
		/// SERCOS S Parameter (e.g. S-0-xxxx)
		SercosParamS,
		/// SERCOS P Parameter (e.g. P-0-xxxx)
		SercosParamP
	};

	/// Values that represent SERCOS Parameter Bytes block to be processed. Using this in the Telegram's control byte
	/// will inform or request what is/should stored in the payload.
	enum SercosDatablock : BYTE {
		/// Channel not active (read-only)
		Datablock_ChannelNotActive,
		/// Getting the SERCOS parameter identification number (read-only)
		Datablock_IdentNumber,
		/// Getting the SERCOS parameter name (read-only)
		Datablock_Name,
		/// Getting the SERCOS parameter Datablock (read-only).
		/// Response be represented by SercosParamAttribute.
		Datablock_Attribute,
		/// Getting the SERCOS parameter unit information
		Datablock_Unit,
		/// Getting the SERCOS parameter possible min Value
		Datablock_Minval,
		/// Getting the SERCOS parameter possible max Value
		Datablock_Maxval,
		/// Getting the SERCOS operation Bytes (actual content of the parameter)
		Datablock_OperationData
	};

	/// Values that represent SERCOS command requests value. Mainly used for write_parameter() in SISProtocol class
	/// to initiate or cancel processing a command (e.g. entering parametrization level).
	///
	/// @sa	SISProtocol
	/// @sa	write_parameter()
	enum SercosCommandrequest : BYTE {
		Commandrequest_NotSet	= 0x0,
		Commandrequest_Cancel	= 0x1,
		Commandrequest_Set		= 0x3
	};

	/// Values that represent SERCOS command status. Mainly used for get_parameter_status() in SISProtocol class to
	/// retrieve feedback of the command processing (e.g. entering parametrization level finished?).
	///
	/// @sa	SISProtocol
	/// @sa	get_parameter_status()
	enum SercosCommandstatus : BYTE {
		Commandstatus_NotSet	= 0x0,
		Commandstatus_OK		= 0x3,
		Commandstatus_Canceled	= 0x5,
		Commandstatus_Busy		= 0x7,
		Commandstatus_Error		= 0xF
	};

	/// Values that represent information in the SIS Telegram's Control Byte about the type of the Command Telegram
	/// or Reception Telegram.
	///
	/// @sa	SercosParamControl
	enum SercosTxProgress : BYTE {
		/// An enum constant representing that Telegram will be followed by another Telegram.
		TxProgress_InProgress,
		/// An enum constant representing that this is a single Telegram (not followed by another Telegram).
		TxProgress_Final
	};

	/// Values that represent the information stored in a Parameter attributes (can be retrieved by attribute datablock).
	/// @sa SercosParamAttribute
	enum SercosDatalen : UINT32 {
		Datalen_Res1		= 0b000,
		Datalen_2ByteParam	= 0b001,
		Datalen_4ByteParam	= 0b010,
		Datalen_8ByteParam	= 0b011,
		Datalen_1ByteList	= 0b100,
		Datalen_2ByteList	= 0b101,
		Datalen_4ByteList	= 0b110,
		Datalen_8ByteList	= 0b111,
	};


/// Grouping unions that merge together both raw and structured information.
	namespace Bitfields
	{
		/// Control byte consisting of several bit fields. Size: 8 bit.
		typedef struct HeaderControl
		{
			union
			{
				struct Bits
				{
					/// Bit 0-2 of Control Byte: Number of sub-addresses in the address block: NumSubAddresses=[0..7].
					BYTE NumSubAddresses : 3;

					/// Bit 3 of Control Byte: Running telegram number. Byte represents:
					/// * 0: not support  
					/// * 1: additional byte
					BYTE NumRunningTgm : 1;

					/// Bit 4 of Control Byte: Telegram Type, represented by HeaderType.
					HeaderType Type : 1;

					/// Bit 5-7 of Control Byte: Status Bytes for the reaction telegram. Byte represents:
					/// * 000: no error, request was processed  
					/// * 001: transmission request being processed  
					/// * 010: transmission cannot presently be processed  
					/// * 100: warning  
					/// * 110: error.
					BYTE StatusReactionTgm : 3;

					/// Constructor.
					///
					/// @param	type	(Optional) Header type, represented by TGM::HeaderType.
					///
					/// @sa	HeaderType
					Bits(HeaderType type = TypeCommand) :
						NumSubAddresses(0),
						NumRunningTgm(0),
						Type(type),
						StatusReactionTgm(0)
					{}
				} Bits;

				/// Representation of the raw value.
				BYTE Value;
			};

			/// Constructor.
			///
			/// @param	type	(Optional) Header type, represented by HeaderType.
			///
			/// @sa	HeaderType
			HeaderControl(HeaderType type = TypeCommand) : Bits(TypeCommand) {}
		} HeaderControl;


		/// The control byte specifies how a Bytes block element of a parameter is accessed. The control byte is read out
		/// of the command telegram and copied into the response telegram.
		typedef struct SercosParamControl
		{
			union
			{
				struct Bits
				{
					BYTE res1 : 1;
					BYTE res2 : 1;

					/// The transmission of a consecutive telegram is controlled with this bit (lists are written in several steps):
					/// * 0: transmission in progress
					/// * 1: final transmission.
					SercosTxProgress TxProgress : 1;

					/// SERCOS parameter datablock, represented by SercosDatablock.
					SercosDatablock Datablock : 3;

					BYTE res6 : 1;
					BYTE res7 : 1;

					/// Constructor.
					///
					/// @param	datablock	(Optional) SERCOS Datablock, represented by SercosDatablock.
					///
					/// @sa	SercosDatablock
					Bits(SercosDatablock datablock = Datablock_OperationData) :
						res1(0), res2(0), TxProgress(TxProgress_Final), Datablock(datablock), res6(0), res7(0)
					{}
				} Bits;

				/// Representation of the raw value.
				BYTE Value;
			};

			/// Constructor.
			///
			/// @param	datablock	(Optional) SERCOS Datablock, represented by SercosDatablock..
			SercosParamControl(SercosDatablock datablock = Datablock_OperationData) : Bits(datablock) {}

			/// Constructor.
			///
			/// @param	value	Raw byte data of the Control Byte.
			SercosParamControl(BYTE value) : Value(value) {}
		} SercosParamControl;


		/// Identification of the parameter. Size: 16 bit.
		typedef struct SercosParamIdent
		{
			union
			{
				struct Bits
				{
					/// Bit 0-11: The parameter number [0..4095], e.g. P-0-*1177*, includes 1177 as ParamNumber.
					USHORT ParamNumber : 12;

					/// Bit 12-15: The parameter block [0..7], e.g. P-*0*-1177, includes 0 as ParamSet.
					USHORT ParamSet : 3;

					/// Bit 16: Parameter variant:
					/// * 0: S-Parameter (drive)  
					/// * 1: P-Parameter (drive).
					USHORT ParamVariant : 1;

					/// Default constructor.
					///
					/// @param	param_variant	(Optional) The parameter variant, represented by SercosParamVar.
					/// @param	param_num	  	(Optional) The parameter number.
					Bits(SercosParamVar param_variant = TGM::SercosParamS, USHORT param_num = 0) :
						ParamNumber(param_num),
						ParamSet(0),
						ParamVariant(param_variant)
					{}
				} Bits;

				USHORT Value;
			};

			/// Constructor.
			///
			/// @param	param_variant	(Optional) The parameter variant, represented by SercosParamVar.
			/// @param	param_num	  	(Optional) The parameter number.
			SercosParamIdent(SercosParamVar param_variant = TGM::SercosParamS, USHORT param_num = 0) :
				Bits(param_variant, param_num)
			{}
		} SercosParamIdent;


		/// Attribute for a SERCOS parameter that is callable via SercosDatablock.
		///
		/// @sa	SercosDatablock
		typedef struct SercosParamAttribute
		{
			union
			{
				struct Bits
				{
					/// Bit 0-15 of Reception Telegram's payload: Conversion factor: The conversion factor is an unsigned integer used to convert numeric Bytes to
					/// display format. The conversion factor shall be set to a Value of 1, if a conversion is not required (e.g. for
					/// binary numbers, character strings or floating - point numbers).
					UINT32 ConversionFactor : 16;

					/// Bit 16-18 of Reception Telegram's payload: The Bytes length is required so that the Master is able to complete Service Channel Bytes transfers
					/// correctly.
					SercosDatalen DataLen : 3;

					/// Bit 19 of Reception Telegram's payload: Indicates whether this Bytes calls a procedure in a drive:
					/// * 0 Operation Bytes or parameter   
					/// * 1 Procedure command.
					UINT32 DataFunction : 1;

					/// Bit 20-22 of Reception Telegram's payload: Format used to convert the operation Bytes, and min/max input values to the correct display format.
					UINT32 DataDisplay : 3;

					/// Bit 23 of Reception Telegram's payload.
					UINT32 res5 : 1;

					/// Bit 24-27 of Reception Telegram's payload: Decimal point: Places after the decimal point indicates the position of the decimal point of
					/// appropriate operation Bytes. Decimal point is used to define fixed point decimal numbers. For all other display
					/// formats the decimal point shall be = 0.
					UINT32 ScaleFactor : 4;

					/// Bit 28 of Reception Telegram's payload.
					UINT32 is_writeonly_phase2 : 1;

					/// Bit 29 of Reception Telegram's payload.
					UINT32 is_writeonly_phase3 : 1;

					/// Bit 30 of Reception Telegram's payload.
					UINT32 is_writeonly_phase4 : 1;

					/// Bit 31 of Reception Telegram's payload.
					UINT32 res10 : 1;

					/// Default constructor.
					Bits() :
						ConversionFactor(0), 
						DataLen(Datalen_2ByteParam), 
						DataFunction(0), 
						DataDisplay(0),
						res5(0),
						ScaleFactor(0),
						is_writeonly_phase2(0),
						is_writeonly_phase3(0),
						is_writeonly_phase4(0),
						res10(0)
					{}
				} Bits;

				/// Raw data value.
				UINT32 Value;
			};

			/// Constructor.
			///
			/// @param	_value	(Optional) Raw data value of the Reception Telegram's payload.
			SercosParamAttribute(UINT32 _value = 0) : Value(_value) {}
		} SercosParamAttribute;
	}
}


#endif // !_TELEGRAMS_BITFIELDS_H_