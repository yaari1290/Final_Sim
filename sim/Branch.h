#pragma once
#ifndef _BRANCH_H_
#define _BRANCH_H_

#include "TypeDef.h"

void Beq_Execute(S_Core* p_Core);
void Bne_Execute(S_Core* p_Core);
void Blt_Execute(S_Core* p_Core);
void Bgt_Execute(S_Core* p_Core);
void Ble_Execute(S_Core* p_Core);
void Bge_Execute(S_Core* p_Core);
void Execute_JAL_Op(S_Core* p_Core);


#endif