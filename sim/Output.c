#include "output.h"




void Outout_Fill_Trace(S_Core* p_s_core , uint32_t Clock)
{


	fprintf(p_s_core->p_CoreTrace_File, "%d ", Clock); //writing total PC

	if ((p_s_core->Core_Stage_Flag & 0x01) == CORE_STAGE_FLAG_FETCH_OFFSET)
	{
		fprintf(p_s_core->p_CoreTrace_File, "%03X ", p_s_core->S_Pipline_Core_Fetch_Next.NPC); // writing Fetch PC
	}
	else
		fprintf(p_s_core->p_CoreTrace_File, "--- ");

	if ((p_s_core->Core_Stage_Flag & 0x02) == CORE_STAGE_FLAG_DECODE_OFFSET)
	{
		fprintf(p_s_core->p_CoreTrace_File, "%03X ", p_s_core->S_Pipline_Core_Decode_Valid.NPC); // WRITING DECODE PC
	}
	else
		fprintf(p_s_core->p_CoreTrace_File, "--- ");

	if ((p_s_core->Core_Stage_Flag & 0x04) == CORE_STAGE_FLAG_EXECUTE_OFFSET)
	{
		fprintf(p_s_core->p_CoreTrace_File, "%03X ", p_s_core->S_Pipline_Core_Execute_Valid.NPC); // WRITING EXEC PC
	}
	else
		fprintf(p_s_core->p_CoreTrace_File, "--- ");

	if ((p_s_core->Core_Stage_Flag & 0x08) == CORE_STAGE_FLAG_MEM_OFFSET)
	{
		fprintf(p_s_core->p_CoreTrace_File, "%03X ", p_s_core->S_Pipline_Core_Mem_Valid.NPC);//WRITING MEM PC
	}
	else
		fprintf(p_s_core->p_CoreTrace_File, "--- ");


	if ((p_s_core->Core_Stage_Flag & 0x10) == CORE_STAGE_FLAG_WB_OFFSET)
	{
	fprintf(p_s_core->p_CoreTrace_File, "%03X ", p_s_core->S_Pipline_Core_WB_Valid.NPC); //WRITING WB PC
	}
	else
		fprintf(p_s_core->p_CoreTrace_File, "--- ");

	for (int ii = 2; ii < 16; ii++)
	{
		fprintf(p_s_core->p_CoreTrace_File, "%08X ", p_s_core->Reg_Array[ii]);
	}
	fprintf(p_s_core->p_CoreTrace_File, "\n");


}

void Output_Bus_Trace(S_Multi_Core_Env* p_Cores_Env, uint32_t Clock)
{
	if (p_Cores_Env->Queue_Bus.MSI_Bus[0].bus_cmd)
	{
		fprintf(p_Cores_Env->p_bus_trace, "%d ", Clock); //writing total PC
		fprintf(p_Cores_Env->p_bus_trace, "%01X ", p_Cores_Env->Queue_Bus.MSI_Bus[0].bus_origid); //writing bus_origid
		fprintf(p_Cores_Env->p_bus_trace, "%01X ", p_Cores_Env->Queue_Bus.MSI_Bus[0].bus_cmd); //writing bus_cmd
		if((p_Cores_Env->Queue_Bus.MSI_Bus[0].bus_cmd !=3) && (p_Cores_Env->Queue_Bus.MSI_Bus[0].bus_cmd != 0))
			fprintf(p_Cores_Env->p_bus_trace, "%05X ", p_Cores_Env->Queue_Bus.addr_origid); //writing bus_Addr
		else
			fprintf(p_Cores_Env->p_bus_trace, "%05X ", p_Cores_Env->Queue_Bus.MSI_Bus[0].bus_Addr); //writing bus_Addr
		fprintf(p_Cores_Env->p_bus_trace, "%08X ", p_Cores_Env->Queue_Bus.MSI_Bus[0].bus_data); //writing bus_data
		fprintf(p_Cores_Env->p_bus_trace, "%01X ", p_Cores_Env->Queue_Bus.shared_core); //writing bus_data
		fprintf(p_Cores_Env->p_bus_trace, "\n");
	}
}


void OutPut_Fill_Reg_File(S_Core* p_s_core)
{
	for (int ii = 2; ii < 16; ii++)
	{
		fprintf(p_s_core->p_regout_File_core, "%08X\n", p_s_core->Reg_Array[ii]); // writing Fetch PC
	}

	fclose(p_s_core->p_regout_File_core);

}

void OutPut_stats_Files(uint32_t Clock ,S_Core* p_s_core)
{
	fprintf(p_s_core->p_status_File_core, "cycles %d\n", Clock+1);
	fprintf(p_s_core->p_status_File_core, "instructions %d\n", p_s_core->imem_Index);
	fprintf(p_s_core->p_status_File_core, "read_hit %d\n", p_s_core->read_hit);
	fprintf(p_s_core->p_status_File_core, "write_hit %d\n", p_s_core->write_hit);
	fprintf(p_s_core->p_status_File_core, "read_miss %d\n", p_s_core->read_miss);
	fprintf(p_s_core->p_status_File_core, "write_miss %d\n", p_s_core->write_miss);
	fprintf(p_s_core->p_status_File_core, "decode_stall %d\n", p_s_core->decode_stall);
	fprintf(p_s_core->p_status_File_core, "mem_stall %d\n", p_s_core->mem_stall);

	fclose(p_s_core->p_status_File_core);
}

void OutPut_MemOut_Files(S_Multi_Core_Env* Cores_env)
{
	for (int ii = 0; ii < 768; ii++)
	{
		
		Cores_env->p_MainMemory++;
		fprintf(Cores_env->p_memout, "%08X\n", *(Cores_env->p_MainMemory-1));
	}
	fclose(Cores_env->p_memout);

}

void OutPut_Fill_Dsram_File(S_Core* p_s_core)
{
	for (int ii = 0; ii < 256; ii++)
	{
		p_s_core->p_DSram_Core++;
		fprintf(p_s_core->p_dsram_File_core, "%08X\n", *(p_s_core->p_DSram_Core - 1));
	}
	fclose(p_s_core->p_dsram_File_core);
}

void OutPut_Fill_Tsram_File(S_Core* p_s_core)
{
	for (int ii = 0; ii < 64; ii++)
	{
		p_s_core->p_TSram_Core++;
		fprintf(p_s_core->p_TSram_File_core, "%08X\n", *(p_s_core->p_TSram_Core - 1));
	}
	fclose(p_s_core->p_TSram_File_core);
}

