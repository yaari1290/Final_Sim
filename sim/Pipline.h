#pragma once
#ifndef _PIPLINE_H_
#define _PIPLINE_H_

#include "TypeDef.h"
#include "Branch.h"
#include "Bus.h"



OS_Error Core_Fetch_Stage(S_Core* p_Core);

void Get_Decode_Data(S_Core* p_Core);

OS_Error Core_Decode_Stage(S_Core* p_Core);

void Get_Execute_Data(S_Core* p_Core);


OS_Error Core_Execute_Stage(S_Core* p_Core);

void Get_MEM_Data(S_Core* p_Core);

OS_Error Core_Mem_Stage(S_Core* p_Core, int Core_Index);

void get_WB_Data(S_Core* p_Core);

OS_Error Core_WB_Stage(S_Core* p_Core);

OS_Error Core_Stage_ex(S_Core* p_Core, int Core_Index, E_Core_Stage Stage);
#endif // !_PIPLINE_H_
