#include "Core.h"




void Core_Init(S_Core* Core , int Core_Index)
{
	int ii;

	Core->Core_PC = 0;
	Core->Core_PC_Q = 0;
	Core->imem_Index = 0;
	Core->Core_Stage_Flag = CORE_STAGE_FLAG_FETCH_OFFSET;
	Core->Core_Stage_Flag_Q = CORE_STAGE_FLAG_FETCH_OFFSET;
	Core->Hazard_Stall = false;


	for (ii = 0; ii < 16; ii++)
	{
		Core->Hazard_Flag[ii] = false; // initial the Hazard Falg to zero
	}



}

void Next_Cycle_Data(S_Core* p_Core)
{

	for (int ii = 0; ii < 16; ii++)
	{
		p_Core->Reg_Array[ii] = p_Core->Reg_Array_Q[ii];
	}

	if ((p_Core->Core_Stage_Flag & 0x08) == CORE_STAGE_FLAG_MEM_OFFSET)
		if (!p_Core->bus_Stall)
		{
			get_WB_Data(p_Core);
		}
	if ((p_Core->Core_Stage_Flag & 0x04) == CORE_STAGE_FLAG_EXECUTE_OFFSET)
			Get_MEM_Data(p_Core);

	if ((p_Core->Core_Stage_Flag & 0x02) == CORE_STAGE_FLAG_DECODE_OFFSET)
	{
		if (p_Core->Hazard_Stall == false)
		{
			Get_Execute_Data(p_Core);
		}
		/*else
		{
			p_Core->Core_Stage_Flag_Q = p_Core->Core_Stage_Flag_Q | CORE_STAGE_FLAG_EXECUTE_OFFSET;
		}
		*/
	}
	if ((p_Core->Core_Stage_Flag & 0x01) == CORE_STAGE_FLAG_FETCH_OFFSET)
		if (p_Core->Hazard_Stall == false) 
		{
			Get_Decode_Data(p_Core);
		}

	p_Core->Core_Stage_Flag = p_Core->Core_Stage_Flag_Q;
	if ((p_Core->Hazard_Stall == false) && (p_Core->bus_Stall == false))
		p_Core->Core_PC = p_Core->Core_PC_Q;

}

OS_Error Cores_ex(S_Multi_Core_Env* Cores_env)
{

	int Core_Index;
	OS_Error Error_Status = E_NO_ERROR;
	while (1)
	{
		for (Core_Index = 0; Core_Index < 1; Core_Index++) // need to change for 4 cores for now it is only for core 0
		{
			if ((Cores_env->p_s_core[Core_Index].Hazard_Stall == false) &&(Cores_env->p_s_core[Core_Index].bus_Stall == false))
				Cores_env->p_s_core[Core_Index].Core_PC_Q++; // Adding Core Q PC by 1


			if ((Cores_env->p_s_core[Core_Index].Core_Stage_Flag & 0x01) == CORE_STAGE_FLAG_FETCH_OFFSET)
			{
				Error_Status = Core_Stage_ex(&Cores_env->p_s_core[Core_Index], Core_Index, E_FETCH);

			}
			
			if ((Cores_env->p_s_core[Core_Index].Core_Stage_Flag & 0x02) == CORE_STAGE_FLAG_DECODE_OFFSET)
			{
				Error_Status = Core_Stage_ex(&Cores_env->p_s_core[Core_Index], Core_Index, E_DECODE);
			}
			

			if ((Cores_env->p_s_core[Core_Index].Core_Stage_Flag & 0x04) == CORE_STAGE_FLAG_EXECUTE_OFFSET)
			{
				Error_Status = Core_Stage_ex(&Cores_env->p_s_core[Core_Index], Core_Index, E_EXECUTE);
			}
			

			if ((Cores_env->p_s_core[Core_Index].Core_Stage_Flag & 0x08) == CORE_STAGE_FLAG_MEM_OFFSET)
			{
				if(!Cores_env->p_s_core[Core_Index].bus_Stall)
					Error_Status = Core_Stage_ex(&Cores_env->p_s_core[Core_Index], Core_Index, E_MEM);
				
				if (Cores_env->p_s_core[Core_Index].flag_Bus_Request == true)
				{
					if (!Cores_env->p_s_core[Core_Index].bus_Stall)
					{
						new_request(Cores_env, Cores_env->p_s_core[Core_Index].Bus_Request);
					}
					Current_MSI_STATE current_state = cuurent_MSI_state(&Cores_env->p_s_core[Core_Index], Cores_env->p_s_core[Core_Index].Bus_Request.bus_origid, Cores_env->p_s_core[Core_Index].Bus_Request.bus_Addr);
					execute_MSI_request(Cores_env, current_state);
				}
			}

			if ((Cores_env->p_s_core[Core_Index].Core_Stage_Flag & 0x10) == CORE_STAGE_FLAG_WB_OFFSET)
				Error_Status = Core_Stage_ex(&Cores_env->p_s_core[Core_Index], Core_Index, E_WRITE_BACK);




			Outout_Fill_Trace(&Cores_env->p_s_core[Core_Index], Cores_env->Clock);

			Next_Cycle_Data(&Cores_env->p_s_core[Core_Index]);
			


			if (Cores_env->p_s_core[Core_Index].Core_Stage_Flag == 0)
			{
				fclose(Cores_env->p_s_core[Core_Index].p_CoreTrace_File);
				OutPut_Fill_Reg_File(&Cores_env->p_s_core[Core_Index]);
				Cores_env->Finish_Cores = Cores_env->Finish_Cores - 1;
				exit(1); //need to delete this and make flag to know that the core is finish
			}

			printf("\n Clock =%d\n", Cores_env->Clock);

#if DEBUG_REGESTERS_PRINT	
			for (int jj = 2; jj < 16; jj++)
			{
				printf("\n R%d = %d", jj, Cores_env->p_s_core[Core_Index].Reg_Array[jj]);
			}
#endif
#if DEBUG_HAZARD_PRINT
			printf("\n Hazard Array\n");
			for (int ii = 0; ii < 16; ii++)
			{
				printf("R%d = %d\n", ii, Cores_env->p_s_core[Core_Index].Hazard_Flag[ii]);
			}
#endif

		}// finish loop for all cores



		Cores_env->Clock++;
		if (Cores_env->Clock == 7)
		{
			printf("debug\n");
		}

	}
}
