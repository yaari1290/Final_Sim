#pragma once
#ifndef _BRANCH_H_
#define _BRANCH_H_

#include "TypeDef.h"

/// Beq_Execute
/// 
/// Branch BEQ Operation
/// <param name = p_Core
void Beq_Execute(S_Core* p_Core);

/// Bne_Execute
/// 
/// Branch BNE Operation
/// <param name = p_Core
void Bne_Execute(S_Core* p_Core);

/// Blt_Execute
/// 
/// Branch BLT Operation
/// <param name = p_Core
void Blt_Execute(S_Core* p_Core);

/// Bgt_Execute
/// 
/// Branch BGT Operation
/// <param name = p_Core
void Bgt_Execute(S_Core* p_Core);

/// Ble_Execute
/// 
/// Branch BLE Operation
/// <param name = p_Core
void Ble_Execute(S_Core* p_Core);

/// Bge_Execute
/// 
/// Branch BGE Operation
/// <param name = p_Core
void Bge_Execute(S_Core* p_Core);

/// Execute_JAL_Op
/// 
/// Branch JAL Operation
/// <param name = p_Core
void Execute_JAL_Op(S_Core* p_Core);


#endif