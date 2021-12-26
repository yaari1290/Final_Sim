#include "Pipline.h"




bool Check_Hazard(S_Core* p_Core , uint32_t Add_Rtv, uint32_t Add_Rsv )
{
	uint16_t Hazard_Reg_Rtv = 0;
	uint16_t Hazard_Reg_Rsv = 0;

	if (Add_Rtv != 0)
	{
		if (p_Core->Hazard_Flag[Add_Rsv] == true)
			return true;
	}

	if (Add_Rsv != 0)
	{
		if (p_Core->Hazard_Flag[Add_Rtv] == true)
			return true;
	}

	return false;

}

void Signal_Hazard(S_Core* p_Core, uint32_t Add_Rdv)
{
	if((Add_Rdv != 0) && (Add_Rdv != 1))
		p_Core->Hazard_Flag[Add_Rdv] = true;
}

void Clear_Hazard(S_Core* p_Core, uint32_t Add_Rdv)
{
	if((Add_Rdv != 0) && (Add_Rdv != 1))
		p_Core->Hazard_Flag[Add_Rdv] = false;
}

OS_Error Core_Fetch_Stage(S_Core* p_Core)
{
	OS_Error Error_Status = E_NO_ERROR;


		uint32_t* imem = p_Core->p_mem_core;
		p_Core->S_Pipline_Core_Fetch_Next.Fetch_IR = imem[p_Core->Core_PC];
		p_Core->S_Pipline_Core_Fetch_Next.NPC = p_Core->Core_PC;

#if DEBUG_FETCH_PRINT
		printf("\n=========================\n");
		printf("\nValue in Fetch:\n");
		printf("NPC = %d\n", p_Core->S_Pipline_Core_Fetch_Next.NPC);
		printf("Decode next Reg  = %x\n", p_Core->S_Pipline_Core_Fetch_Next.Fetch_IR);
#endif
	


	return Error_Status;
}


void Get_Decode_Data(S_Core *p_Core)
{
	//p_Core->S_Pipline_Core_ID.Decode_Valid_Reg = p_Core->S_Pipline_Core_ID.Decode_Next_Reg;
	p_Core->S_Pipline_Core_Decode_Valid.Decode_IR = p_Core->S_Pipline_Core_Fetch_Next.Fetch_IR;
	p_Core->S_Pipline_Core_Decode_Valid.NPC = p_Core->S_Pipline_Core_Fetch_Next.NPC;

	p_Core->Core_Stage_Flag_Q = p_Core->Core_Stage_Flag_Q + CORE_STAGE_FLAG_DECODE_OFFSET;
}


OS_Error Core_Decode_Stage(S_Core* p_Core)
{

	OS_Error Error_Status = E_NO_ERROR;
	uint32_t Immediate_Mask		= 0x00000FFF;
	uint32_t Rt_Mask			= 0x0000F000;
	uint32_t Rs_Mask			= 0x000F0000;
	uint32_t Rd_Mask			= 0x00F00000;
	uint32_t Opcode_Mask		= 0xFF000000;

	p_Core->S_Pipline_Core_Decode_Next.IMM = p_Core->S_Pipline_Core_Decode_Valid.Decode_IR & Immediate_Mask;
	p_Core->Reg_Array_Q[1] = p_Core->S_Pipline_Core_Decode_Next.IMM;
	p_Core->S_Pipline_Core_Decode_Next.Rtv = (p_Core->S_Pipline_Core_Decode_Valid.Decode_IR & Rt_Mask) >> 12;
	p_Core->S_Pipline_Core_Decode_Next.Rsv = (p_Core->S_Pipline_Core_Decode_Valid.Decode_IR & Rs_Mask) >> 16;
	p_Core->S_Pipline_Core_Decode_Next.Rd = (p_Core->S_Pipline_Core_Decode_Valid.Decode_IR & Rd_Mask) >> 20;
	p_Core->S_Pipline_Core_Decode_Next.Opcode = (p_Core->S_Pipline_Core_Decode_Valid.Decode_IR & Opcode_Mask) >> 24;
	p_Core->S_Pipline_Core_Decode_Next.NPC = p_Core->S_Pipline_Core_Decode_Valid.NPC;
	p_Core->S_Pipline_Core_Decode_Next.Decode_IR = p_Core->S_Pipline_Core_Decode_Valid.Decode_IR;

	p_Core->Core_Stage_Flag_Q = p_Core->Core_Stage_Flag_Q & ~CORE_STAGE_FLAG_DECODE_OFFSET;

	Operation OperationStage = (Operation)p_Core->S_Pipline_Core_Decode_Next.Opcode;
			if (OperationStage == E_BEQ)
		{
#if DEBUG_EXECUTE_PRINT
		printf("BEQ Operation\n");
#endif
		Beq_Execute(p_Core);
		}

			else if (OperationStage == E_BNE)
		{
#if DEBUG_EXECUTE_PRINT
		printf("BNE Operation\n");
#endif
		Bne_Execute(p_Core);
		}

			else if (OperationStage == E_BLT)
		{
#if DEBUG_EXECUTE_PRINT
		printf("BLT Operation\n");
#endif
		Blt_Execute(p_Core);
		}

			else if (OperationStage == E_BGT)
		{
#if DEBUG_EXECUTE_PRINT
		printf("BGT Operation\n");
#endif
		Bgt_Execute(p_Core);
		}

			else if (OperationStage == E_BLE)
		{
#if DEBUG_EXECUTE_PRINT
		printf("BLE Operation\n");
#endif
		Ble_Execute(p_Core);
		}

			else if (OperationStage == E_BGE)
		{
#if DEBUG_EXECUTE_PRINT
		printf("BGE Operation\n");
#endif
		Bge_Execute(p_Core);
		}


#if DEBUG_DECODE_PRINT
	printf("\n======================================\n");
	printf("\n Values in Decode:\n");
	printf("NPC = %d\n", p_Core->S_Pipline_Core_Decode_Next.NPC);
	printf("IR = %x\n", p_Core->S_Pipline_Core_Decode_Next.Decode_IR);
	printf("Immediate = %d\n", p_Core->S_Pipline_Core_Decode_Next.IMM);
	printf("Rt = %d\n", p_Core->S_Pipline_Core_Decode_Next.Rtv);
	printf("Rs = %d\n", p_Core->S_Pipline_Core_Decode_Next.Rsv);
	printf("Rd = %d\n", p_Core->S_Pipline_Core_Decode_Next.Rd);
	printf("Opcode = %d\n", p_Core->S_Pipline_Core_Decode_Next.Opcode);
#endif

	return Error_Status;

}


void Get_Execute_Data(S_Core* p_Core)
{


	// need to add her hazard opperation//
	if (Check_Hazard(p_Core, p_Core->S_Pipline_Core_Decode_Next.Rtv, p_Core->S_Pipline_Core_Decode_Next.Rsv))
	{
		// hazard check for Rsv Rtv and stay in loop until is free
		p_Core->Hazard_Stall = true;
	}
	else
	{
		p_Core->S_Pipline_Core_Execute_Valid.IMM = p_Core->S_Pipline_Core_Decode_Next.IMM;
		p_Core->S_Pipline_Core_Execute_Valid.Dest_Reg = p_Core->S_Pipline_Core_Decode_Next.Rd;
		p_Core->S_Pipline_Core_Execute_Valid.Opcode = p_Core->S_Pipline_Core_Decode_Next.Opcode;
		p_Core->S_Pipline_Core_Execute_Valid.Execute_IR = p_Core->S_Pipline_Core_Decode_Next.Decode_IR;
		p_Core->S_Pipline_Core_Execute_Valid.NPC = p_Core->S_Pipline_Core_Decode_Next.NPC;

		p_Core->S_Pipline_Core_Execute_Valid.Rtv = p_Core->Reg_Array[p_Core->S_Pipline_Core_Decode_Next.Rtv];
		p_Core->S_Pipline_Core_Execute_Valid.Rsv = p_Core->Reg_Array[p_Core->S_Pipline_Core_Decode_Next.Rsv];

		Signal_Hazard(p_Core, p_Core->S_Pipline_Core_Execute_Valid.Dest_Reg); // signal hazard for the Dest Add Reg

		p_Core->Core_Stage_Flag_Q = p_Core->Core_Stage_Flag_Q + CORE_STAGE_FLAG_EXECUTE_OFFSET;
	}


}

void Execute_ADD_Op(S_Core* p_Core)
{

	p_Core->S_Pipline_Core_Execute_Valid.ALU = p_Core->S_Pipline_Core_Execute_Valid.Rtv + p_Core->S_Pipline_Core_Execute_Valid.Rsv;
}

void Execute_SUB_Op(S_Core* p_Core)
{

	p_Core->S_Pipline_Core_Execute_Valid.ALU = p_Core->S_Pipline_Core_Execute_Valid.Rsv - p_Core->S_Pipline_Core_Execute_Valid.Rtv;
}

void Execute_AND_Op(S_Core* p_Core)
{

	p_Core->S_Pipline_Core_Execute_Valid.ALU = p_Core->S_Pipline_Core_Execute_Valid.Rsv & p_Core->S_Pipline_Core_Execute_Valid.Rtv;
}

void Execute_OR_Op(S_Core* p_Core)
{

	p_Core->S_Pipline_Core_Execute_Valid.ALU = p_Core->S_Pipline_Core_Execute_Valid.Rsv | p_Core->S_Pipline_Core_Execute_Valid.Rtv;
}

void Execute_XOR_Op(S_Core* p_Core)
{

	p_Core->S_Pipline_Core_Execute_Valid.ALU = p_Core->S_Pipline_Core_Execute_Valid.Rsv ^ p_Core->S_Pipline_Core_Execute_Valid.Rtv;
}

void Execute_MUL_Op(S_Core* p_Core)
{

	p_Core->S_Pipline_Core_Execute_Valid.ALU = p_Core->S_Pipline_Core_Execute_Valid.Rsv * p_Core->S_Pipline_Core_Execute_Valid.Rtv;
}

void Execute_SLL_Op(S_Core* p_Core)
{
	p_Core->S_Pipline_Core_Execute_Valid.ALU = p_Core->S_Pipline_Core_Execute_Valid.Rsv << p_Core->S_Pipline_Core_Execute_Valid.Rtv;
}

void Execute_SRA_Op(S_Core* p_Core)
{

	p_Core->S_Pipline_Core_Execute_Valid.ALU = p_Core->S_Pipline_Core_Execute_Valid.Rsv >> p_Core->S_Pipline_Core_Execute_Valid.Rtv;
}

void Execute_SRL_Op(S_Core* p_Core)
{

	p_Core->S_Pipline_Core_Execute_Valid.ALU = p_Core->S_Pipline_Core_Execute_Valid.Rsv >> p_Core->S_Pipline_Core_Execute_Valid.Rtv;
}


void Execute_JAL_Op(S_Core* p_Core)
{
	//need to under stand what to do
}


void Execute_LW_Op(S_Core* p_Core)
{

}

void Execute_SW_Op(S_Core* p_Core)
{

}

void Execute_HALT_Op(S_Core* p_Core)
{

	p_Core->Core_Stage_Flag = p_Core->Core_Stage_Flag & ~CORE_STAGE_FLAG_FETCH_OFFSET;
	p_Core->Core_Stage_Flag = p_Core->Core_Stage_Flag & ~CORE_STAGE_FLAG_DECODE_OFFSET;
	p_Core->Core_Stage_Flag_Q = p_Core->Core_Stage_Flag_Q & ~CORE_STAGE_FLAG_FETCH_OFFSET;
	p_Core->Core_Stage_Flag_Q = p_Core->Core_Stage_Flag_Q & ~CORE_STAGE_FLAG_DECODE_OFFSET;

}




OS_Error Core_Execute_Stage(S_Core* p_Core)
{
	OS_Error Error_Status = E_NO_ERROR;
	
	p_Core->Core_Stage_Flag_Q = p_Core->Core_Stage_Flag_Q & ~CORE_STAGE_FLAG_EXECUTE_OFFSET;
	Operation OperationStage = (Operation)p_Core->S_Pipline_Core_Execute_Valid.Opcode;

#if DEBUG_EXECUTE_PRINT
	printf("\n======================\n");
	printf("Start Execute Stage!\n");
	printf("NPC = %d\n", p_Core->S_Pipline_Core_Execute_Valid.NPC);
#endif


		if(OperationStage == E_ADD)
		{
#if DEBUG_EXECUTE_PRINT
			printf("ADD Operation\n");
#endif
			Execute_ADD_Op(p_Core);
		}

		else if(OperationStage == E_SUB)
		{
#if DEBUG_EXECUTE_PRINT
			printf("SUB Operation\n");
#endif
			Execute_SUB_Op(p_Core);
		}

		else if(OperationStage == E_AND)
		{
#if DEBUG_EXECUTE_PRINT
			printf("AND Operation\n");
#endif
			return Error_Status;
		}

		else if(OperationStage == E_OR)
		{
#if DEBUG_EXECUTE_PRINT
			printf("OR Operation\n");
#endif
		}

		else if(OperationStage == E_XOR)
		{
#if DEBUG_EXECUTE_PRINT
			printf("XOR Operation\n");
#endif
		}

		else if(OperationStage == E_MUL)
		{
#if DEBUG_EXECUTE_PRINT
			printf("MUL Operation\n");
#endif
		}

		else if(OperationStage == E_SLL)
		{
#if DEBUG_EXECUTE_PRINT
			printf("SLL Operation\n");
#endif
		}

		else if(OperationStage == E_SRA)
		{
#if DEBUG_EXECUTE_PRINT
			printf("SRA Operation\n");
#endif
		}

		else if(OperationStage == E_SRL)
		{
#if DEBUG_EXECUTE_PRINT
			printf("SRL Operation\n");
#endif
		}

		else if(OperationStage == E_JAL)
		{
#if DEBUG_EXECUTE_PRINT
			printf("JAL Operation\n");
#endif
		}

		else if(OperationStage == E_LW)
		{
#if DEBUG_EXECUTE_PRINT
			printf("LW Operation\n");
#endif
		}

		else if(OperationStage == E_SW)
		{
#if DEBUG_EXECUTE_PRINT
			printf("SW Operation\n");
#endif

		}

		else if(OperationStage == E_HALT)
		{
#if DEBUG_EXECUTE_PRINT
			printf("HALT Operation\n");
#endif
			Execute_HALT_Op(p_Core);

		}

		p_Core->S_Pipline_Core_Execute_Next.ALU = p_Core->S_Pipline_Core_Execute_Valid.ALU;
		p_Core->S_Pipline_Core_Execute_Next.Dest_Reg = p_Core->S_Pipline_Core_Execute_Valid.Dest_Reg;
		p_Core->S_Pipline_Core_Execute_Next.Execute_IR = p_Core->S_Pipline_Core_Execute_Valid.Execute_IR;
		p_Core->S_Pipline_Core_Execute_Next.IMM = p_Core->S_Pipline_Core_Execute_Valid.IMM;
		p_Core->S_Pipline_Core_Execute_Next.NPC = p_Core->S_Pipline_Core_Execute_Valid.NPC;
		p_Core->S_Pipline_Core_Execute_Next.Opcode = p_Core->S_Pipline_Core_Execute_Valid.Opcode;
		p_Core->S_Pipline_Core_Execute_Next.Rd = p_Core->S_Pipline_Core_Execute_Valid.Rd;
		p_Core->S_Pipline_Core_Execute_Next.Rsv = p_Core->S_Pipline_Core_Execute_Valid.Rsv;
		p_Core->S_Pipline_Core_Execute_Next.Rtv = p_Core->S_Pipline_Core_Execute_Valid.Rtv;
	
}

void Get_MEM_Data(S_Core* p_Core)
{
	p_Core->S_Pipline_Core_Mem_Valid.Dest_Reg = p_Core->S_Pipline_Core_Execute_Next.Dest_Reg;
	p_Core->S_Pipline_Core_Mem_Valid.Mem_IR = p_Core->S_Pipline_Core_Execute_Next.Execute_IR;
	p_Core->S_Pipline_Core_Mem_Valid.ALU = p_Core->S_Pipline_Core_Execute_Next.ALU;
	p_Core->S_Pipline_Core_Mem_Valid.NPC = p_Core->S_Pipline_Core_Execute_Next.NPC;
	p_Core->S_Pipline_Core_Mem_Valid.Rtv = p_Core->S_Pipline_Core_Execute_Next.Rtv;
	p_Core->S_Pipline_Core_Mem_Valid.Execute_Opcode = p_Core->S_Pipline_Core_Execute_Next.Opcode;

	p_Core->Core_Stage_Flag_Q = p_Core->Core_Stage_Flag_Q + CORE_STAGE_FLAG_MEM_OFFSET;
}

OS_Error Core_Mem_Stage(S_Core* p_Core)
{
	OS_Error Error_Status = E_NO_ERROR;

	printf("\nStart MEM Stage!\n");
	p_Core->Core_Stage_Flag_Q = p_Core->Core_Stage_Flag_Q & ~CORE_STAGE_FLAG_MEM_OFFSET;

	Operation Operation_Status = p_Core->S_Pipline_Core_Mem_Valid.Execute_Opcode;


	if ((Operation_Status == E_LW) || (Operation_Status == E_SW) || (Operation_Status == E_HALT))
	{
		/// need to add memort function
		printf("Start MEM Function!\n");
	}

	else // Overall dont do anything 
	{
		printf("No MEM Function!\n");
		p_Core->S_Pipline_Core_Mem_Next.Dest_Reg = p_Core->S_Pipline_Core_Mem_Valid.Dest_Reg;
		p_Core->S_Pipline_Core_Mem_Next.Mem_IR =p_Core->S_Pipline_Core_Mem_Valid.Mem_IR;
		p_Core->S_Pipline_Core_Mem_Next.ALU =p_Core->S_Pipline_Core_Mem_Valid.ALU;
		p_Core->S_Pipline_Core_Mem_Next.NPC =p_Core->S_Pipline_Core_Mem_Valid.NPC;
		p_Core->S_Pipline_Core_Mem_Next.Rtv =p_Core->S_Pipline_Core_Mem_Valid.Rtv;
		p_Core->S_Pipline_Core_Mem_Next.Execute_Opcode =p_Core->S_Pipline_Core_Mem_Valid.Execute_Opcode;

		//need to do for MD if we make MEM function
		p_Core->S_Pipline_Core_Mem_Next.MD = p_Core->S_Pipline_Core_Mem_Valid.MD;
		p_Core->S_Pipline_Core_Mem_Next.MD_MUX = true;///indicate that we made MEM function and need to WB to MEM


	}

	return Error_Status;



}


void get_WB_Data(S_Core* p_Core)
{
	p_Core->S_Pipline_Core_WB_Valid.ALU = p_Core->S_Pipline_Core_Mem_Next.ALU;
	p_Core->S_Pipline_Core_WB_Valid.Dest = p_Core->S_Pipline_Core_Mem_Next.Dest_Reg;
	p_Core->S_Pipline_Core_WB_Valid.WtiteBack_IR = p_Core->S_Pipline_Core_Mem_Next.Mem_IR;
	p_Core->S_Pipline_Core_WB_Valid.MD = p_Core->S_Pipline_Core_Mem_Next.MD;
	p_Core->S_Pipline_Core_WB_Valid.NPC = p_Core->S_Pipline_Core_Mem_Valid.NPC;
	p_Core->S_Pipline_Core_WB_Valid.Execute_Opcode = p_Core->S_Pipline_Core_Mem_Next.Execute_Opcode;

	p_Core->Core_Stage_Flag_Q = p_Core->Core_Stage_Flag_Q + CORE_STAGE_FLAG_WB_OFFSET;

}

OS_Error Core_WB_Stage(S_Core* p_Core)
{
	OS_Error Error_Status = E_NO_ERROR;

	printf("\nStart WB Operation!\n");
	if ((uint32_t)p_Core->S_Pipline_Core_WB_Valid.Execute_Opcode >(uint32_t)E_SRL)
	{
		//DO NOTHING
	}

	else //no need to WB to mem only to REG
	{
		p_Core->Reg_Array_Q[p_Core->S_Pipline_Core_WB_Valid.Dest] = p_Core->S_Pipline_Core_WB_Valid.ALU; // Writing the final value to reg
		Clear_Hazard(p_Core, p_Core->S_Pipline_Core_WB_Valid.Dest);

		printf("ALU = %d\n", p_Core->S_Pipline_Core_WB_Valid.ALU);
		printf("Dest Reg = %d\n", p_Core->S_Pipline_Core_WB_Valid.Dest);

	}

	p_Core->Core_Stage_Flag_Q = p_Core->Core_Stage_Flag_Q & ~CORE_STAGE_FLAG_WB_OFFSET;


	return Error_Status;

}

OS_Error Core_Stage_ex(S_Core *p_Core , E_Core_Stage Stage)
{

	OS_Error Error_Status = E_NO_ERROR;

	switch (Stage)
	{
		case E_FETCH:
		{
			Error_Status = Core_Fetch_Stage(p_Core);
			return Error_Status;

		}

		case E_DECODE:
		{
			Error_Status = Core_Decode_Stage(p_Core);
			return Error_Status;

		}

		case E_EXECUTE:
		{
			Error_Status = Core_Execute_Stage(p_Core);
			return Error_Status;
		}

		case E_MEM:
		{
			Error_Status = Core_Mem_Stage(p_Core);
			return Error_Status;

		}

		case E_WRITE_BACK:
		{
			Error_Status = Core_WB_Stage(p_Core);

			//for debug
			printf("\nCore Reg Array!\n");
			for (int ii = 0; ii < 16; ii++)
			{
				printf("R%d = %d\n", ii, p_Core->Reg_Array[ii]);
			}
			return Error_Status;

		}

		case E_STALL:
		{

		}
		default:
			break;
	}
}


