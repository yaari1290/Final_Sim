#include "Branch.h"

#define BIT(x)	(1U << (x))
#define SAME_SIGN(a, b)	!(((a) ^ (b)) & BIT(31))


void Beq_Execute(S_Core* p_Core)
{
	uint32_t Rs_Reg, Rt_Reg, Rd_Reg;

	Rd_Reg = p_Core->Reg_Array[p_Core->S_Pipline_Core_Decode_Next.Rd];
	Rt_Reg = p_Core->Reg_Array[p_Core->S_Pipline_Core_Decode_Next.Rtv];
	Rs_Reg = p_Core->Reg_Array[p_Core->S_Pipline_Core_Decode_Next.Rsv];

	if (Rs_Reg == Rt_Reg)
	{
		p_Core->Core_PC_Q = Rd_Reg & 0x3FF; //9:0 bits of Rd
	}
	
}

void Bne_Execute(S_Core* p_Core)
{
	uint32_t Rs_Reg, Rt_Reg, Rd_Reg;

	Rs_Reg = p_Core->S_Pipline_Core_Decode_Next.Rsv;
	Rt_Reg = p_Core->S_Pipline_Core_Decode_Next.Rtv;
	Rd_Reg = p_Core->Reg_Array[p_Core->S_Pipline_Core_Decode_Next.Rd];

	if (Rs_Reg != Rt_Reg)
	{
		p_Core->Core_PC_Q = Rd_Reg & 0x3FF; //9:0 bits of Rd
	}
}

void Blt_Execute(S_Core* p_Core)
{
	uint32_t Rs_Reg, Rt_Reg, Rd_Reg;

	Rd_Reg = p_Core->Reg_Array_Q[p_Core->S_Pipline_Core_Decode_Next.Rd];
	Rt_Reg = p_Core->Reg_Array_Q[p_Core->S_Pipline_Core_Decode_Next.Rtv];
	Rs_Reg = p_Core->Reg_Array_Q[p_Core->S_Pipline_Core_Decode_Next.Rsv];

	if (SAME_SIGN(Rs_Reg, Rt_Reg))
	{
		if (Rs_Reg < Rt_Reg)
		{
			p_Core->Core_PC_Q = Rd_Reg & 0x3FF; //9:0 bits of Rd
		}
	}
	else
		if (Rs_Reg > Rt_Reg)
		{
			p_Core->Core_PC_Q = Rd_Reg & 0x3FF; //9:0 bits of Rd
		}
	

}

void Bgt_Execute(S_Core* p_Core)
{
	uint32_t Rs_Reg, Rt_Reg, Rd_Reg;

	Rs_Reg = p_Core->S_Pipline_Core_Decode_Next.Rsv;
	Rt_Reg = p_Core->S_Pipline_Core_Decode_Next.Rtv;
	Rd_Reg = p_Core->Reg_Array[p_Core->S_Pipline_Core_Decode_Next.Rd];

	if (SAME_SIGN(Rs_Reg, Rt_Reg))
	{
		if (Rs_Reg > Rt_Reg)
		{
			p_Core->Core_PC_Q = Rd_Reg & 0x3FF; //9:0 bits of Rd
		}
	}
	else
	{
		if (Rs_Reg < Rt_Reg)
		{
			p_Core->Core_PC_Q = Rd_Reg & 0x3FF; //9:0 bits of Rd
		}
	}

}


void Ble_Execute(S_Core* p_Core)
{
	uint32_t Rs_Reg, Rt_Reg, Rd_Reg;

	Rs_Reg = p_Core->S_Pipline_Core_Decode_Next.Rsv;
	Rt_Reg = p_Core->S_Pipline_Core_Decode_Next.Rtv;
	Rd_Reg = p_Core->Reg_Array[p_Core->S_Pipline_Core_Decode_Next.Rd];


	if (SAME_SIGN(Rs_Reg, Rt_Reg))
	{
		if (Rs_Reg <= Rt_Reg)
		{
			p_Core->Core_PC_Q = Rd_Reg & 0x3FF; //9:0 bits of Rd
		}
	}
	else
	{
		if (Rs_Reg >= Rt_Reg)
		{
			p_Core->Core_PC_Q = Rd_Reg & 0x3FF; //9:0 bits of Rd
		}
	}
}

void Bge_Execute(S_Core* p_Core)
{
	uint32_t Rs_Reg, Rt_Reg, Rd_Reg;

	Rs_Reg = p_Core->S_Pipline_Core_Decode_Next.Rsv;
	Rt_Reg = p_Core->S_Pipline_Core_Decode_Next.Rtv;
	Rd_Reg = p_Core->Reg_Array[p_Core->S_Pipline_Core_Decode_Next.Rd];


	if (SAME_SIGN(Rs_Reg, Rt_Reg))
	{
		if (Rs_Reg >= Rt_Reg)
		{
			p_Core->Core_PC_Q = Rd_Reg & 0x3FF; //9:0 bits of Rd
		}
	}

	else
	{
		if (Rs_Reg <= Rt_Reg)
		{
			p_Core->Core_PC_Q = Rd_Reg & 0x3FF; //9:0 bits of Rd
		}
	}

}

void Execute_JAL_Op(S_Core* p_Core)
{
	p_Core->Core_PC_Q = p_Core->Core_PC_Q = p_Core->Reg_Array[p_Core->S_Pipline_Core_Decode_Next.Rd] & 0x3FF; //9:0 bits of Rd
	p_Core->Reg_Array[15] = p_Core->Core_PC + 1;
}


