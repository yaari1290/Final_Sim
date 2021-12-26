#define _CRT_SECURE_NO_WARMIMGS


#include "mem.h"
#include "TypeDef.h"
#include "Pipline.h"
#include "Core.h"


enum files {
	PATH_IMEME0,
	PATH_IMEME1,
	PATH_IMEME2,
	PATH_IMEME3,
	PATH_MEMIN,
	PATH_MEMOUT,
	PATH_REGOUT0,
	PATH_REGOUT1,
	PATH_REGOUT2,
	PATH_REGOUT3,
	PATH_CORE0TRACE,
	PATH_CORE1TRACE,
	PATH_CORE2TRACE,
	PATH_CORE3TRACE,
	PATH_BUSTRACE,
	PATH_DSRAM0,
	PATH_DSRAM1,
	PATH_DSRAM2,
	PATH_DSRAM3,
	PATH_TSRAM0,
	PATH_TSRAM1,
	PATH_TSRAM2,
	PATH_TSRAM3,
	PATH_STATS0,
	PATH_STATS1,
	PATH_STATS2,
	PATH_STATS3,
	PATH_MAX
};

const char* default_path[27] = {
	[PATH_IMEME0] = "imem0.txt",
	[PATH_IMEME1] = "imem1.txt",
	[PATH_IMEME2] = "imem2.txt",
	[PATH_IMEME3] = "imem3.txt",
	[PATH_MEMIN] = "memin.txt",
	[PATH_MEMOUT] = "memout.txt",
	[PATH_REGOUT0] = "regout0.txt",
	[PATH_REGOUT1] = "regout1.txt",
	[PATH_REGOUT2] = "regout2.txt",
	[PATH_REGOUT3] = "regout3.txt",
	[PATH_CORE0TRACE] = "core0trace.txt",
	[PATH_CORE1TRACE] = "core1trace.txt",
	[PATH_CORE2TRACE] = "core2trace.txt",
	[PATH_CORE3TRACE] = "core3trace.txt",
	[PATH_BUSTRACE] = "bustrace.txt",
	[PATH_DSRAM0] = "dsram0.txt",
	[PATH_DSRAM1] = "dsram1.txt",
	[PATH_DSRAM2] = "dsram2.txt",
	[PATH_DSRAM3] = "dsram3.txt",
	[PATH_TSRAM0] = "tsram0.txt",
	[PATH_TSRAM1] = "tsram1.txt",
	[PATH_TSRAM2] = "tsram2.txt",
	[PATH_TSRAM3] = "tsram3.txt",
	[PATH_STATS0] = "stats0.txt",
	[PATH_STATS1] = "stats1.txt",
	[PATH_STATS2] = "stats2.txt",
	[PATH_STATS3] = "stats3.txt",
};


/// initial the core struct for it files and data
OS_Error initial_Core_Env(int argc, char* argv[], S_Multi_Core_Env *p_Cores_Env)
{
	OS_Error ErrorStatus = E_NO_ERROR;

	if (argc == NUM_OF_COMMAND_LINES)
	{

	}

	else // argc == 0
	{
		p_Cores_Env->data_path = (char**)&default_path;

	}


	for (int Core_Index = 0; Core_Index < 1; Core_Index++) // need to change the number of cores to 4
	{
		ErrorStatus = Mem_Open(p_Cores_Env->data_path[Core_Index], &p_Cores_Env->p_s_core[Core_Index]);

		Core_Init(&p_Cores_Env->p_s_core[Core_Index], Core_Index);
		
		if (ErrorStatus != E_NO_ERROR)
		{
			return ErrorStatus;
		}

		//Creating Output Files
		ErrorStatus = Opening_Core_Output_Files(&p_Cores_Env->p_s_core[Core_Index],
												p_Cores_Env->data_path[Core_Index + PATH_REGOUT0],
												p_Cores_Env->data_path[Core_Index + PATH_CORE0TRACE],
												p_Cores_Env->data_path[Core_Index + PATH_DSRAM0],
												p_Cores_Env->data_path[Core_Index + PATH_TSRAM0],
												p_Cores_Env->data_path[Core_Index + PATH_STATS0]);


	}

	ErrorStatus = Main_Memory_Open(p_Cores_Env->data_path[PATH_MEMIN], p_Cores_Env);


	return ErrorStatus;


}

int main(int argc, char* argv[])
{
	printf("hello world Start project\n");

	S_Multi_Core_Env Cores_env = { 0 };
	OS_Error Error_Status = E_NO_ERROR;
	/// checking num of inputs

	if ((argc != NUM_OF_COMMAND_LINES) && (argc != 1))
	{
		printf("error! wrong number of arguments!\n");
		exit(1);

	}


	Error_Status = initial_Core_Env(argc, argv, &Cores_env );

	Error_Status = Cores_ex(&Cores_env);


}