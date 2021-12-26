#pragma once

#ifndef _BUS_H_
#define _BUS_H_

#include "TypeDef.h"

S_MSI_Bus Push(S_Multi_Core_Env* p_Cores_Env);
bool isEmpty(S_Multi_Core_Env* p_Cores_Env);
void insert(S_Multi_Core_Env* p_Cores_Env, S_MSI_Bus Bus);
bool isFull(S_Multi_Core_Env* p_Cores_Env);


#endif
