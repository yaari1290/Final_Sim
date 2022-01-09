#pragma once
#ifndef _OUTPUT_H_
#define _OUTPUT_H_

#include "TypeDef.h"
#include "Core.h"
#include "Bus.h"


/// <Outout_Fill_Trace>
/// 
/// Fill Output trace
/// <param name="p_s_core"></param>
/// <param name="Clock"></param>
void Outout_Fill_Trace(S_Core* p_s_core , uint32_t Clock);

/// <OutPut_Fill_Reg_File>
/// 
/// fill output reg file
/// <param name="p_s_core"></param>
void OutPut_Fill_Reg_File(S_Core* p_s_core);

/// <Output_Bus_Trace>
/// 
/// fill output bus trace file
/// <param name="p_Cores_Env"></param>
/// <param name="Clock"></param>
void Output_Bus_Trace(S_Multi_Core_Env* p_Cores_Env, uint32_t Clock);

/// <OutPut_stats_Files>
/// 
/// fill output stats file
/// <param name="Clock"></param>
/// <param name="p_s_core"></param>
void OutPut_stats_Files(uint32_t Clock, S_Core* p_s_core);

/// <OutPut_MemOut_Files>
/// 
/// fill Memory out file
/// <param name="Cores_env"></param>
void OutPut_MemOut_Files(S_Multi_Core_Env* Cores_env);

/// <OutPut_Fill_Dsram_File>
/// 
/// fill DSRAM output file
/// <param name="p_s_core"></param>
void OutPut_Fill_Dsram_File(S_Core* p_s_core);

/// <OutPut_Fill_Tsram_File>
/// 
/// fill TSRAM output file
/// <param name="p_s_core"></param>
void OutPut_Fill_Tsram_File(S_Core* p_s_core);


#endif