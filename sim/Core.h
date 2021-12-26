#pragma once
#ifndef _CORE_H_
#define _CORE_H_

#include "TypeDef.h"
#include "Pipline.h"
#include "Branch.h"
#include "mem.h"
#include "output.h"


void Core_Init(S_Core* Core, int Core_Index);

OS_Error Cores_ex(S_Multi_Core_Env* Cores_env);




#endif // !_CORE_H_
