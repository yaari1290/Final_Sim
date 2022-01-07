#pragma once
#ifndef _OUTPUT_H_
#define _OUTPUT_H_

#include "TypeDef.h"
#include "Core.h"
#include "Bus.h"


//fill Trace Data for core
void Outout_Fill_Trace(S_Core* p_s_core , uint32_t Clock);
void OutPut_Fill_Reg_File(S_Core* p_s_core);
void Output_Bus_Trace(S_Multi_Core_Env* p_Cores_Env, uint32_t Clock);
void OutPut_stats_Files(uint32_t Clock, S_Core* p_s_core);
void OutPut_MemOut_Files(S_Multi_Core_Env* Cores_env);
void OutPut_Fill_Dsram_File(S_Core* p_s_core);
void OutPut_Fill_Tsram_File(S_Core* p_s_core);


#endif