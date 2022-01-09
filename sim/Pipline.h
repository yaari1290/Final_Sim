#pragma once
#ifndef _PIPLINE_H_
#define _PIPLINE_H_

#include "TypeDef.h"
#include "Branch.h"
#include "Bus.h"


/// <Core_Fetch_Stage>
/// 
/// start fetch stage in pipline:
/// 1. Reading from imem input by core PC
/// 2. putting the IR from imem input in the next data for decode
/// <param name="p_Core"></param>
/// return Error
OS_Error Core_Fetch_Stage(S_Core* p_Core);

/// <Get_Decode_Data>
/// 
/// get the data from fetch stage to next decode stage
/// <param name="p_Core"></param>
void Get_Decode_Data(S_Core* p_Core);

/// <Core_Decode_Stage>
/// 
/// Start Decode stage:
/// 1. decode the function from imem[core_pc]
/// 2. Checking for hazards
/// 3. Check if the opcode is branch finction , if it is start brach function
/// <param name="p_Core"></param>
/// <returns></returns>
OS_Error Core_Decode_Stage(S_Core* p_Core);

/// <Get_Execute_Data>
/// 
/// Get from Decode stage the data for execute stage for the next cycle
/// 
/// <param name="p_Core"></param>
void Get_Execute_Data(S_Core* p_Core);

/// <Core_Execute_Stage>
/// 
/// Start Execute stage:
/// Checking what Operation need to be done
/// <param name="p_Core"></param>
/// <returns></returns>
OS_Error Core_Execute_Stage(S_Core* p_Core);

/// <Get_MEM_Data>
/// 
/// Getting from Execute stage the data for next cycle Mem stage
/// <param name="p_Core"></param>
void Get_MEM_Data(S_Core* p_Core);

/// <Core_Mem_Stage>
/// 
/// start MEM stage:
/// 1. Checking if LW or SW operation need to be done , if need to do -> Bus request
/// 2. if no LW or SW operation need to be done "pass" the data for next WB stage
/// <param name="p_Core"></param>
/// <param name="Core_Index"></param>
/// <returns></returns>
OS_Error Core_Mem_Stage(S_Core* p_Core, int Core_Index);

/// <get_WB_Data>
/// 
/// <Getting from MEM stage the data for next cycle WB stage
/// <param name="p_Core"></param>
void get_WB_Data(S_Core* p_Core);

/// <Core_WB_Stage>
/// 
/// Start Write Back stage:
/// 1. Checking if there was LW Function -> if there was ->data to reg
/// 2. clearing Hazards
/// <param name="p_Core"></param>
/// <returns></returns>
OS_Error Core_WB_Stage(S_Core* p_Core);

/// <Core_Stage_ex>
/// 
/// "switch" function for each stage in pipline
/// <param name="p_Core"></param>
/// <param name="Core_Index"></param>
/// <param name="Stage"></param>
/// <returns></returns>
OS_Error Core_Stage_ex(S_Core* p_Core, int Core_Index, E_Core_Stage Stage);
#endif // !_PIPLINE_H_
