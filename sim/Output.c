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


void OutPut_Fill_Reg_File(S_Core* p_s_core)
{
	for (int ii = 2; ii < 16; ii++)
	{
		fprintf(p_s_core->p_regout_File_core, "%08X\n", p_s_core->Reg_Array[ii]); // writing Fetch PC
	}

	fclose(p_s_core->p_regout_File_core);

}

