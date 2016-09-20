#ifndef _TELEGRAMS_BITFIELDS_H_
#define _TELEGRAMS_BITFIELDS_H_

#include <Windows.h>
#include <vector>


namespace TGM
{
	enum Header_Type : BYTE {
		Type_Command, 
		Type_Reaction 
	};

	enum SERCOS_ParamVar : BYTE {
		SERCOS_Param_S,
		SERCOS_Param_P
	};

	enum SERCOS_Datablock : BYTE {
		SERCOS_Datablock_channel_not_active,
		SERCOS_Datablock_ident_number,
		SERCOS_Datablock_name,
		SERCOS_Datablock_attribute,
		SERCOS_Datablock_unit,
		SERCOS_Datablock_minval,
		SERCOS_Datablock_maxval,
		SERCOS_Datablock_operatingdata
	};

	enum SERCOS_Commandrequest : BYTE {
		SERCOS_Commandrequest_not_set = 0x0,
		SERCOS_Commandrequest_cancel = 0x1,
		SERCOS_Commandrequest_set = 0x3
	};

	enum SERCOS_Commandstatus : BYTE {
		SERCOS_Commandstatus_not_set = 0x0,
		SERCOS_Commandstatus_ok = 0x3,
		SERCOS_Commandstatus_canceled = 0x5,
		SERCOS_Commandstatus_busy = 0x7,
		SERCOS_Commandstatus_error = 0xF
	};

	enum SERCOS_TX : BYTE {
		SERCOS_TX_in_progress,
		SERCOS_TX_final
	};

	enum SERCOS_DATALEN : UINT32 {
		SERCOS_DATALEN_res1			= 0b000,
		SERCOS_DATALEN_param_2byte	= 0b001,
		SERCOS_DATALEN_param_4byte	= 0b010,
		SERCOS_DATALEN_param_8byte	= 0b011,
		SERCOS_DATALEN_listel_1byte = 0b100,
		SERCOS_DATALEN_listel_2byte = 0b101,
		SERCOS_DATALEN_listel_4byte = 0b110,
		SERCOS_DATALEN_listel_8byte = 0b111,
	};

	namespace Bitfields
	{
		/// <summary>	Control byte consisting of several bit fields. Size: 8 bit. </summary>
		typedef struct _header_cntrl_t
		{
			union
			{
				struct bits_t
				{
					///=================================================================================================
					/// <summary>
					/// Bit 0-2: Number of sub-addresses in the address block: num_sub_addresses=[0..7].
					/// </summary>
					///=================================================================================================
					BYTE num_sub_addresses : 3;

					/// <summary>	Bit 3: Running telegram number. 0: not support, 1: additional byte. </summary>
					BYTE num_running_tgm : 1;

					/// <summary>	Bit 4: Telegram type. 0: Command telegram, 1: Reaction telegram. </summary>
					Header_Type type : 1;

					///=================================================================================================
					/// <summary>
					/// Bit 5-7: Status data for the reaction telegram.
					/// * 000: no error, request was processed  
					/// * 001: transmission request being processed  
					/// * 010: transmission cannot presently be processed  
					/// * 100: warning  
					/// * 110: error.
					/// </summary>
					///=================================================================================================
					BYTE status_react_tgm : 3;

					bits_t(Header_Type _type = Type_Command) :
						num_sub_addresses(0),
						num_running_tgm(0),
						type(_type),
						status_react_tgm(0)
					{}
				} bits;

				BYTE value;
			};

			_header_cntrl_t(Header_Type _type = Type_Command) :
				bits(Type_Command)
			{}

		} Header_Cntrl;


		///=================================================================================================
		/// <summary>
		/// The  control  byte  specifies  how  a  data  block  element  of  a  parameter  is accessed.
		/// The control byte is read out of the command telegram and copied into the response  telegram.
		/// </summary>
		///================================================================================================= 
		typedef struct sercos_parcontrol_t
		{
			union
			{
				struct bits_t
				{
					BYTE res1 : 1;
					BYTE res2 : 1;

					///=================================================================================================
					/// <summary>
					/// The transmission of a consecutive telegram is controlled with this bit (lists are written in
					/// several steps):
					/// * 0: transmission in prog.
					/// * 1: final transmission.
					/// </summary>
					///=================================================================================================
					SERCOS_TX tx_status : 1;

					///=================================================================================================
					/// <summary>
					/// The type of the SERCOS parameter: 
					/// * b000: channel not active   
					/// * b001: ident number (write access)
					/// * b010: name   
					/// * b011: attribute (read access)
					/// * b100: unit (read access)
					/// * b101: min.input value (read access)
					/// * b110: max.input value (read access)
					/// * b111: operating data (write access)
					/// </summary>
					///=================================================================================================
					SERCOS_Datablock attribute : 3;

					BYTE res6 : 1;
					BYTE res7 : 1;

					bits_t(SERCOS_Datablock _attribute = SERCOS_Datablock_operatingdata) :
						res1(0), res2(0), tx_status(SERCOS_TX_final), attribute(_attribute), res6(0), res7(0)
					{}

				} bits;

				BYTE value;
			};

			sercos_parcontrol_t(SERCOS_Datablock _attribute = SERCOS_Datablock_operatingdata) :
				bits(_attribute)
			{}

			sercos_parcontrol_t(BYTE _value) :
				value(_value)
			{}

		} Sercos_ParControl;


		/// <summary>	Identification of the parameter. Size: 16 bit. </summary>
		typedef struct sercos_param_ident_t
		{
			union
			{
				struct bits_t
				{
					/// <summary>	Bit 0-11: The parameter number [0..4095], e.g. P-0-*1177*, includes 1177 as param_no. </summary>
					USHORT param_no : 12;

					/// <summary>	Bit 12-15: The parameter block [0..7], e.g. P-*0*-1177, includes 0 as param_set. </summary>
					USHORT param_set : 3;

					///=================================================================================================
					/// <summary>
					/// Bit 16: Parameter variant:
					/// * 0: S-Parameter (drive)  
					/// * 1: P-Parameter (drive).
					/// </summary>
					///=================================================================================================
					USHORT param_variant : 1;

					/// <summary>	Default constructor. </summary>
					bits_t(SERCOS_ParamVar _param_variant = TGM::SERCOS_Param_S, USHORT _param_num = 0) :
						param_no(_param_num),
						param_set(0),
						param_variant(_param_variant)
					{}

				} bits;

				USHORT value;
			};

			sercos_param_ident_t(SERCOS_ParamVar _param_variant = TGM::SERCOS_Param_S, USHORT _param_num = 0) :
				bits(_param_variant, _param_num)
			{}
			
		} Sercos_Param_Ident;


		typedef struct sercos_attribute_t
		{
			union
			{
				struct bits_t
				{
					/// <summary>	Bit 0-15: Conversion factor: The conversion factor is an unsigned integer used to convert numeric data to display format.The conversion factor shall be set to a value of 1, if a conversion is not required(e.g. for binary numbers, character strings or floating - point numbers). </summary>
					UINT32 conv_factor : 16;

					/// <summary>	Bit 16-18: The data length is required so that the Master is able to complete Service Channel data transfers correctly. </summary>
					SERCOS_DATALEN data_len : 3;

					/// <summary>	Bit 19: Indicates whether this data calls a procedure in a drive: 0 Operation data or parameter 1 Procedure command. </summary>
					UINT32 func_of_data : 1;

					/// <summary>	Bit 20-22: Format Used to convert the operation data, and min/max input values to the correct display format. </summary>
					UINT32 data_disp : 3;

					/// <summary>	Bit 23. </summary>
					UINT32 res5 : 1;

					/// <summary>	Bit 24-27: Decimal point: Places after the decimal point indicates the position of the decimal point of appropriate operation data.Decimal point is used to define fixed point decimal numbers.For all other display formats the decimal point shall be = 0. </summary>
					UINT32 scale_factor : 4;

					/// <summary>	Bit 28. </summary>
					UINT32 is_writeonly_phase2 : 1;

					/// <summary>	Bit 29. </summary>
					UINT32 is_writeonly_phase3 : 1;

					/// <summary>	Bit 30. </summary>
					UINT32 is_writeonly_phase4 : 1;

					/// <summary>	Bit 31. </summary>
					UINT32 res10 : 1;

					/// <summary>	Default constructor. </summary>
					bits_t() :
						conv_factor(0), 
						data_len(SERCOS_DATALEN_param_2byte), 
						func_of_data(0), 
						data_disp(0),
						res5(0),
						scale_factor(0),
						is_writeonly_phase2(0),
						is_writeonly_phase3(0),
						is_writeonly_phase4(0),
						res10(0)
					{}

				} bits;

				UINT32 value;
			};

			sercos_attribute_t(UINT32 _value = 0) :
				value(_value)
			{}

		} Sercos_Attribute;
	}
}


#endif // !_TELEGRAMS_BITFIELDS_H_