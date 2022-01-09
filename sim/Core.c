#include "Core.h"


#define BIT(x)	(1U << (x))

void Core_Init(S_Core* Core , int Core_Index)
{
	int ii;

	Core->Core_PC = 0;
	Core->Core_PC_Q = 0;
	Core->imem_Index = 0;
	Core->Core_Stage_Flag = CORE_STAGE_FLAG_FETCH_OFFSET;
	Core->Core_Stage_Flag_Q = CORE_STAGE_FLAG_FETCH_OFFSET;
	Core->Hazard_Stall = false;
	Core->instructions = 0;
	Core->read_hit = 0;
	Core->write_hit = 0;
	Core->read_miss = 0;
	Core->write_miss = 0;
	Core->decode_stall = 0;
	Core->mem_stall = 0;


	for (ii = 0; ii < 16; ii++)
	{
		Core->Hazard_Flag[ii] = false; // initial the Hazard Falg to zero
	}



}

// Updating Next Cycle Data for each stage in pipline.
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
	{
		Get_MEM_Data(p_Core);

	}

	if(p_Core->bus_Stall)
		p_Core->Core_Stage_Flag_Q = p_Core->Core_Stage_Flag_Q | CORE_STAGE_FLAG_MEM_OFFSET;

	if ((p_Core->Core_Stage_Flag & 0x02) == CORE_STAGE_FLAG_DECODE_OFFSET)
	{
		if (p_Core->Hazard_Stall == false)
		{
			Get_Execute_Data(p_Core);
		}

		if (p_Core->bus_Stall)
		{
			if (p_Core->Hazard_Stall == false)
				p_Core->Core_Stage_Flag_Q = p_Core->Core_Stage_Flag_Q | CORE_STAGE_FLAG_EXECUTE_OFFSET;
		}

		
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
	while (1) //infinite loop until HALT
	{
		for (Core_Index = 0; Core_Index < 4; Core_Index++)//loop for all 4 Cores 
		{

			if (Cores_env->Clock == 1035)
			{
				printf("debug\n");
			}

			//Mask for each core to check if he finish is functions
			int Core_Bit_Mask;
			if (Core_Index == 0)
				Core_Bit_Mask = CORE0_WORKING_FLAG;
			else if (Core_Index == 1)
				Core_Bit_Mask = CORE1_WORKING_FLAG;
			else if (Core_Index == 2)
				Core_Bit_Mask = CORE2_WORKING_FLAG;
			else if (Core_Index == 3)
				Core_Bit_Mask = CORE3_WORKING_FLAG;

	
			if ((Cores_env->Finish_Cores & Core_Bit_Mask) == Core_Bit_Mask) //Checking if the core finish
			{
				//if there is no HAZARD and no BUS STALL continue
				if ((Cores_env->p_s_core[Core_Index].Hazard_Stall == false) && (Cores_env->p_s_core[Core_Index].bus_Stall == false))
					Cores_env->p_s_core[Core_Index].Core_PC_Q++; // Adding Core Q PC by 1
				else
				{
					if (Cores_env->p_s_core[Core_Index].Hazard_Stall == true)
						Cores_env->p_s_core[Core_Index].decode_stall += 1; // adding 1 for stall statistics 
				}

				//checking if fetch stage in pipline need to initiate
				if ((Cores_env->p_s_core[Core_Index].Core_Stage_Flag & 0x01) == CORE_STAGE_FLAG_FETCH_OFFSET)
				{
					//Start Fetch stage in pipline
					Error_Status = Core_Stage_ex(&Cores_env->p_s_core[Core_Index], Core_Index, E_FETCH);

				}
				//checking if Decode stage in pipline need to initiate
				if ((Cores_env->p_s_core[Core_Index].Core_Stage_Flag & 0x02) == CORE_STAGE_FLAG_DECODE_OFFSET)
				{
					//Start Decode stage in pipline
					Error_Status = Core_Stage_ex(&Cores_env->p_s_core[Core_Index], Core_Index, E_DECODE);
				}

				//checking if Execute stage in pipline need to initiate
				if ((Cores_env->p_s_core[Core_Index].Core_Stage_Flag & 0x04) == CORE_STAGE_FLAG_EXECUTE_OFFSET)
				{
					//Start Execute stage in pipline
					Error_Status = Core_Stage_ex(&Cores_env->p_s_core[Core_Index], Core_Index, E_EXECUTE);
				}

				//checking if fetch MEM in pipline need to initiate
				if ((Cores_env->p_s_core[Core_Index].Core_Stage_Flag & 0x08) == CORE_STAGE_FLAG_MEM_OFFSET)
				{
					if (!Cores_env->p_s_core[Core_Index].bus_Stall)
					{
						//Start Mem stage in pipline
						Error_Status = Core_Stage_ex(&Cores_env->p_s_core[Core_Index], Core_Index, E_MEM);
					}
					if (Cores_env->p_s_core[Core_Index].flag_Bus_Request == true)
					{
						if (!Cores_env->p_s_core[Core_Index].bus_Stall)
						{
							new_request(Cores_env, Cores_env->p_s_core[Core_Index].Bus_Request);
						}
						Current_MSI_STATE current_state = cuurent_MSI_state(Cores_env, Cores_env->p_s_core[Core_Index].Bus_Request.bus_origid, Cores_env->p_s_core[Core_Index].Bus_Request.bus_Addr);
						if (Cores_env->Queue_Bus.Next_Free_Slot == 1)
						{
							execute_MSI_request(Cores_env, current_state);
						}
						else if (Cores_env->Queue_Bus.core_origid == Core_Index)
						{
							execute_MSI_request(Cores_env, current_state);
						}
					}
				}
				//checking if Write Back stage in pipline need to initiate
				if ((Cores_env->p_s_core[Core_Index].Core_Stage_Flag & 0x10) == CORE_STAGE_FLAG_WB_OFFSET)
				{
					//Start Write Back stage in pipline
					Error_Status = Core_Stage_ex(&Cores_env->p_s_core[Core_Index], Core_Index, E_WRITE_BACK);
				}


				// Fill trace output for each core after one clock
				Outout_Fill_Trace(&Cores_env->p_s_core[Core_Index], Cores_env->Clock);

				// updating next cycle data
				Next_Cycle_Data(&Cores_env->p_s_core[Core_Index]);

				

				// Checking the core finished 
				// if the core finished:
				// closing all outputs
				// closing all alocated ver
				// unmask the Finish Cores flag
				if (Cores_env->p_s_core[Core_Index].Core_Stage_Flag == 0)
				{
					fclose(Cores_env->p_s_core[Core_Index].p_CoreTrace_File);
					OutPut_Fill_Reg_File(&Cores_env->p_s_core[Core_Index]);
					OutPut_stats_Files(Cores_env->Clock, &Cores_env->p_s_core[Core_Index]);
					OutPut_Fill_Dsram_File(&Cores_env->p_s_core[Core_Index]);
					OutPut_Fill_Tsram_File(&Cores_env->p_s_core[Core_Index]);
					if (Core_Index == 0)
					{
						Cores_env->Finish_Cores = Cores_env->Finish_Cores & 0xE;
					}

					else if (Core_Index == 1)
					{
						Cores_env->Finish_Cores = Cores_env->Finish_Cores & 0xD;
					}

					else if (Core_Index == 2)
					{
						Cores_env->Finish_Cores = Cores_env->Finish_Cores & 0xB;
					}

					else if (Core_Index == 3)
					{
						Cores_env->Finish_Cores = Cores_env->Finish_Cores & 0x7;
					}
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
			}
		}// finish loop for all cores

		Cores_env->Clock++;
		//checking if all cores finish 
		if (Cores_env->Finish_Cores == 0) // all Cores Finish
		{

			OutPut_MemOut_Files(Cores_env);
			fclose(Cores_env->p_bus_trace);
			exit(1);
		}

	}
}
