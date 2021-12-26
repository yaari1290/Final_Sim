#define _CRT_SECURE_NO_WARMIMGS

#include "mem.h"




OS_Error Convert_imem(uint32_t *p_mem_core , FILE* fp)
{
	int ii =0;
	OS_Error Error_Status = E_NO_ERROR;

	printf("\n===================================\n");
	printf("\nConvert Imem!\n");
	while(!feof(fp))
	{
		int j =fscanf(fp, "%x\n", p_mem_core++);
		printf("%x\n", *(p_mem_core-1));

	}

	return Error_Status;

}


OS_Error Mem_Open(char* data_path, S_Core *Core)
{

	OS_Error ErrorStatus = E_NO_ERROR;


	FILE* p_File = fopen(data_path, "r");
	if (!p_File)
	{
		printf("Error Opening File\n");
		ErrorStatus = E_ERROR_OPENING_FILE;
		return ErrorStatus;
	}

	Core->p_mem_core = calloc(1024,sizeof(uint32_t));

	if (!Core->p_mem_core)
	{
		printf("Error Aloccating memory\n");
	}
	
	ErrorStatus = Convert_imem(Core->p_mem_core, p_File);

	Core->p_DSram_Core = calloc(256, sizeof(uint32_t));
	Core->p_TSram_Core = calloc(64, sizeof(uint16_t));



	return ErrorStatus;


}

OS_Error Opening_Core_Output_Files(S_Core* Core,char* RegOut_path, char* CoreTrace_path, char* DSram_path, char* TSram_path, char* Status_path)
{

	Core->p_regout_File_core = fopen(RegOut_path, "w");
	Core->p_CoreTrace_File = fopen(CoreTrace_path, "w");
	Core->p_dsram_File_core = fopen(DSram_path, "w");
	Core->p_TSram_File_core = fopen(TSram_path, "w");
	Core->p_status_File_core = fopen(Status_path,"w");
}

OS_Error Main_Memory_Open(char* data_path, S_Multi_Core_Env* p_Cores_Env)
{
	OS_Error ErrorStatus;

	FILE* p_File = fopen(data_path, "r");
	if (!p_File)
	{
		printf("Error Opening Main Memory File\n");
		ErrorStatus = E_ERROR_OPENING_FILE;
		return ErrorStatus;
	}
	
	p_Cores_Env->p_MainMemory = calloc(2 ^ 20, sizeof(20));

	while (!feof(p_File))
	{
		int j = fscanf(p_File, "%x\n", &p_Cores_Env->p_MainMemory);

	}

}