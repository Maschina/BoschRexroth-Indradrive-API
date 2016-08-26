#ifndef _TELEGRAMS_BITFIELDS_H_
#define _TELEGRAMS_BITFIELDS_H_

#include <Windows.h>
#include <vector>


namespace TGM
{
	enum Type { TGM_Type_Command, TGM_Type_Reaction };
	enum Param_Variant { TGM_Param_S, TGM_Param_P };

	namespace Bitfields
	{
		/// <summary>	Control byte consisting of several bit fields. Size: 8 bit. </summary>
		typedef struct _cntrl_t
		{
			///=================================================================================================
			/// <summary>
			/// Bit 0-2: Number of sub-addresses in the address block: num_sub_addresses=[0..7].
			/// </summary>
			///=================================================================================================
			UCHAR num_sub_addresses : 3;

			/// <summary>	Bit 3: Running telegram number. 0: not support, 1: additional byte. </summary>
			UCHAR num_running_tgm : 1;

			/// <summary>	Bit 4: Telegram type. 0: Command telegram, 1: Reaction telegram. </summary>
			Type type : 1;

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
			UCHAR status_react_tgm : 3;

			_cntrl_t(Type _type = TGM_Type_Command) :
				num_sub_addresses(0),
				num_running_tgm(0),
				type(_type),
				status_react_tgm(0)
			{}

			BYTE toByte() { return *(BYTE*)this; }

		} Cntrl;

		///=================================================================================================
		/// <summary>
		/// The  control  byte  specifies  how  a  data  block  element  of  a  parameter  is accessed.
		/// The control byte is read out of the command telegram and copied into the response  telegram.
		/// </summary>
		///================================================================================================= 
		typedef struct sercos_control_t
		{
			enum sercos_control_tx_status {
				tx_in_progress,
				tx_final
			};

			enum sercos_control_type {
				type_channel_not_active,
				type_ident_number,
				type_name,
				type_attribute,
				type_unit,
				type_minval,
				type_maxval,
				type_operatingdata
			};


			UCHAR res1 : 1;
			UCHAR res2 : 1;

			///=================================================================================================
			/// <summary>
			/// The transmission of a consecutive telegram is controlled with this bit (lists are written in
			/// several steps):
			/// * 0: transmission in prog.
			/// * 1: final transmission.
			/// </summary>
			///=================================================================================================
			sercos_control_tx_status tx_status : 1;

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
			sercos_control_type type : 3;

			UCHAR res6 : 1;
			UCHAR res7 : 1;

			/// <summary>	Default constructor. </summary>
			sercos_control_t(sercos_control_type _type = type_operatingdata) :
				res1(0),
				res2(0),
				tx_status(tx_final),
				type(_type),
				res6(0),
				res7(0)
			{}

			BYTE toByte() { return *(BYTE*)this; }

		} Sercos_Control;

		/// <summary>	Identification of the parameter. Size: 16 bit. </summary>
		typedef struct sercos_param_ident_t
		{
			/// <summary>	Bit 0-11: The parameter number [0..4095]. </summary>
			USHORT param_num : 12;

			/// <summary>	Bit 12-15: The parameter block [0..7]. </summary>
			UCHAR param_block : 3;

			///=================================================================================================
			/// <summary>
			/// Bit 16: Parameter variant:
			/// * 0: S-Parameter (drive)  
			/// * 1: P-Parameter (drive).
			/// </summary>
			///=================================================================================================
			UCHAR param_variant : 1;

			/// <summary>	Default constructor. </summary>
			sercos_param_ident_t(Param_Variant _param_variant = TGM_Param_S, USHORT _param_num = 0) :
				param_num(_param_num),
				param_block(0),
				param_variant(_param_variant)
			{}

			USHORT toByte() { return *(USHORT*)this; }

		} Sercos_Param_Ident;
	}
}


#endif // !_TELEGRAMS_BITFIELDS_H_