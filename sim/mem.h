#pragma once

#ifndef __MEM_H__
#define __MEM_H__

#include "TypeDef.h"

/// Mem_Open
/// 
/// Opening imem input for each core
/// Alocating mem for DSRAM and TSRAM
/// <param name= data_path 
/// <param name= Core 
///	
/// return = return if error accord in opening input or aloocating mem
OS_Error Mem_Open(char* data_path, S_Core* Core);

/// <Opening_Core_Output_Files>
/// 
/// opening uotput files for each core
/// <param name="Core"></param>
/// <param name="RegOut_path"></param>
/// <param name="CoreTrace_path"></param>
/// <param name="DSram_path"></param>
/// <param name="TSram_path"></param>
/// <param name="Status_path"></param>
/// 
/// return = return error if error accord in opening output file
OS_Error Opening_Core_Output_Files(S_Core* Core,char* RegOut_path, char* CoreTrace_path, char* DSram_path, char* TSram_path, char* Status_path);

/// <Main_Memory_Open>
/// 
/// 
/// Opening main memory
/// <param name="data_path"></param>
/// <param name="p_Cores_Env"></param>
/// 
/// return = return error if error accord in opening the main memory
OS_Error Main_Memory_Open(char* data_path, S_Multi_Core_Env* p_Cores_Env);

/// <bus_trace_Open>
/// 
/// Opening bus ouput file
/// <param name="data_path"></param>
/// <param name="p_Cores_Env"></param>
/// 
/// return = return error if error accord in opening the bus file
OS_Error bus_trace_Open(char* data_path, S_Multi_Core_Env* p_Cores_Env);

/// <MemOut_File_Open>
/// 
/// Opening Memory out ouput file
/// <param name="data_path"></param>
/// <param name="p_Cores_Env"></param>

void MemOut_File_Open(char* data_path, S_Multi_Core_Env* p_Cores_Env);

#endif