// ConsoleApplication1.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <Windows.h>

typedef enum sercos_control_type : BYTE {
	type_channel_not_active,
	type_ident_number,
	type_name,
	type_attribute,
	type_unit,
	type_minval,
	type_maxval,
	type_operatingdata
} SERCOS_CTRL_Type;

typedef enum sercos_control_tx_status : BYTE {
	tx_in_progress,
	tx_final
} SERCOS_CTRL_TxStatus;

typedef struct sercos_control_t
{
	union
	{
		struct bits_t
		{
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
			SERCOS_CTRL_TxStatus tx_status : 1;

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
			SERCOS_CTRL_Type type : 3;

			UCHAR res6 : 1;
			UCHAR res7 : 1;

			bits_t(SERCOS_CTRL_Type _type = type_operatingdata) :
				res1(0), res2(0), tx_status(tx_final), type(_type), res6(0), res7(0)
			{}

		} bits;

		BYTE byte;
	};

	sercos_control_t(SERCOS_CTRL_Type _type = type_operatingdata) :
		bits(_type)
	{}

} Sercos_Control;

int main()
{
	Sercos_Control control(type_attribute);

	control.bits.res1 = 0;
	control.bits.res2 = 0;
	control.bits.tx_status = tx_final;
	control.bits.type = type_operatingdata;
	control.bits.res6 = 0;
	control.bits.res7 = 1;


    return 0;
}

