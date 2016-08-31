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

	enum SERCOS_Attribute : BYTE {
		SERCOS_Attribute_channel_not_active,
		SERCOS_Attribute_ident_number,
		SERCOS_Attribute_name,
		SERCOS_Attribute_attribute,
		SERCOS_Attribute_unit,
		SERCOS_Attribute_minval,
		SERCOS_Attribute_maxval,
		SERCOS_Attribute_operatingdata
	};

	enum SERCOS_TX : BYTE {
		SERCOS_TX_in_progress,
		SERCOS_TX_final
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
		typedef struct sercos_control_t
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
					SERCOS_Attribute attribute : 3;

					BYTE res6 : 1;
					BYTE res7 : 1;

					bits_t(SERCOS_Attribute _attribute = SERCOS_Attribute_operatingdata) :
						res1(0), res2(0), tx_status(SERCOS_TX_final), attribute(_attribute), res6(0), res7(0)
					{}

				} bits;

				BYTE value;
			};

			sercos_control_t(SERCOS_Attribute _attribute = SERCOS_Attribute_operatingdata) :
				bits(_attribute)
			{}

			sercos_control_t(BYTE _value) :
				value(_value)
			{}

		} Sercos_Control;


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
	}
}


#endif // !_TELEGRAMS_BITFIELDS_H_