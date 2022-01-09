#pragma once
#ifndef _CORE_H_
#define _CORE_H_

#include "TypeDef.h"
#include "Pipline.h"
#include "Branch.h"
#include "mem.h"
#include "output.h"
#include "Bus.h"

/// Core init
/// 
/// </summary>
/// 
/// initialize each Core parameters
/// 
/// <param name= Core
/// <param name= Core_Index 
void Core_Init(S_Core* Core, int Core_Index);

/// Cores_ex
/// 
/// execute all Cores Pipline by order
/// 
/// <param name= Cores_env
/// return = Error in Cores Pipline
OS_Error Cores_ex(S_Multi_Core_Env* Cores_env);




#endif // !_CORE_H_
