#pragma once

#ifndef __MEM_H__
#define __MEM_H__

#include "TypeDef.h"


OS_Error Mem_Open(char* data_path, S_Core* Core);

OS_Error Opening_Core_Output_Files(S_Core* Core,char* RegOut_path, char* CoreTrace_path, char* DSram_path, char* TSram_path, char* Status_path);

OS_Error Main_Memory_Open(char* data_path, S_Multi_Core_Env* p_Cores_Env);

OS_Error bus_trace_Open(char* data_path, S_Multi_Core_Env* p_Cores_Env);

void MemOut_File_Open(char* data_path, S_Multi_Core_Env* p_Cores_Env);

#endif