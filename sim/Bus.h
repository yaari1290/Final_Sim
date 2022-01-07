#pragma once

#ifndef _BUS_H_
#define _BUS_H_

#include "TypeDef.h"
#include "output.h"

S_MSI_Bus Pop_first_in_queue(S_Multi_Core_Env* p_Cores_Env);
int init_Queue_bus(S_Multi_Core_Env* p_Cores_Env); //initilize the bus
bool isEmpty(S_Multi_Core_Env* p_Cores_Env);
void insert(S_Multi_Core_Env* p_Cores_Env, S_MSI_Bus Bus);
bool isFull(S_Multi_Core_Env* p_Cores_Env);
void removeData(S_Multi_Core_Env* p_Cores_Env);
int size(S_Multi_Core_Env* p_Cores_Env);
void set_S_MSI_Bus(S_MSI_Bus* Bus, E_Owner bus_origid, E_Memory_Command bus_cmd, uint32_t bus_Addr, uint32_t bus_data);
S_MSI_Bus Pop_first_in_queue(S_Multi_Core_Env* p_Cores_Env);
int userInQueue(S_Multi_Core_Env* p_Cores_Env, S_MSI_Bus* Bus);
void new_request(S_Multi_Core_Env* p_Cores_Env, S_MSI_Bus Bus);
S_MSI_Bus getNextfromQueue(S_Multi_Core_Env* p_Cores_Env);
int check_bus_shared(S_Multi_Core_Env* p_Cores_Env, int bus_Addr, E_Owner core_index);
void snooping_func(S_Multi_Core_Env* p_Cores_Env, int shared_core, E_Owner core, E_Memory_Command cmd, Current_MSI_STATE current_state);
void execute_flush_on_bus(S_Multi_Core_Env* p_Cores_Env, E_Owner owner, E_Memory_Command cmd, int bus_Addr, int shared_core);
void execute_MSI_request(S_Multi_Core_Env* p_Cores_Env, Current_MSI_STATE current_state);
Current_MSI_STATE cuurent_MSI_state(S_Multi_Core_Env* p_Cores_Env, E_Owner bus_origid, uint32_t bus_Addr);



#endif
