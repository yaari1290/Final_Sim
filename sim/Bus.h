#pragma once

#ifndef _BUS_H_
#define _BUS_H_

#include "TypeDef.h"
#include "output.h"


/// <Pop_first_in_queue>
/// 
/// Pop the first bus request
/// 
/// <param name="p_Cores_Env"></param>
/// <returns></returns>
S_MSI_Bus Pop_first_in_queue(S_Multi_Core_Env* p_Cores_Env);


/// <init_Queue_bus>
/// 
/// initialize the queue bus
/// 
/// <param name="p_Cores_Env"></param>
/// <returns></returns>
int init_Queue_bus(S_Multi_Core_Env* p_Cores_Env);

/// isEmpty
/// 
/// Checkig if queue is empty
/// 
/// <param name="p_Cores_Env"></param>
/// <returns></returns>
bool isEmpty(S_Multi_Core_Env* p_Cores_Env);

/// <isFull>
/// 
/// Checking if the queue is full
/// 
/// <param name="p_Cores_Env"></param>
/// <returns></returns>
bool isFull(S_Multi_Core_Env* p_Cores_Env);

/// removeData
/// 
/// Remove bus request from queue after finish
/// 
/// <param name="p_Cores_Env"></param>
void removeData(S_Multi_Core_Env* p_Cores_Env);

/// <size>
/// 
/// Get the number of bus request in queue
/// 
/// <param name="p_Cores_Env"></param>
/// <returns></returns>
int size(S_Multi_Core_Env* p_Cores_Env);

/// <set_S_MSI_Bus>
/// 
/// Set Bus Request 
/// 
/// <param name="Bus"></param>
/// <param name="bus_origid"></param>
/// <param name="bus_cmd"></param>
/// <param name="bus_Addr"></param>
/// <param name="bus_data"></param>
void set_S_MSI_Bus(S_MSI_Bus* Bus, E_Owner bus_origid, E_Memory_Command bus_cmd, uint32_t bus_Addr, uint32_t bus_data);


int userInQueue(S_Multi_Core_Env* p_Cores_Env, S_MSI_Bus* Bus);

/// <new_request>
/// 
/// Adding new bus request in queue
/// 
/// <param name="p_Cores_Env"></param>
/// <param name="Bus"></param>
void new_request(S_Multi_Core_Env* p_Cores_Env, S_MSI_Bus Bus);

/// <getNextfromQueue>
/// 
/// getting the next bus request from queue
/// 
/// <param name="p_Cores_Env"></param>
/// <returns></returns>
S_MSI_Bus getNextfromQueue(S_Multi_Core_Env* p_Cores_Env);

/// check_bus_shared
/// 
/// Check if the data in bus request is in shared state
/// 
/// <param name="p_Cores_Env"></param>
/// <param name="bus_Addr"></param>
/// <param name="core_index"></param>
/// <returns></returns>
int check_bus_shared(S_Multi_Core_Env* p_Cores_Env, int bus_Addr, E_Owner core_index);

/// <snooping_func>
/// 
/// Snooping function for Bus request
/// 
/// <param name="p_Cores_Env"></param>
/// <param name="shared_core"></param>
/// <param name="core"></param>
/// <param name="cmd"></param>
/// <param name="current_state"></param>
void snooping_func(S_Multi_Core_Env* p_Cores_Env, int shared_core, E_Owner core, E_Memory_Command cmd, Current_MSI_STATE current_state);

/// <execute_flush_on_bus>
/// 
/// Start FLUSH operation if needed
/// 
/// <param name="p_Cores_Env"></param>
/// <param name="owner"></param>
/// <param name="cmd"></param>
/// <param name="bus_Addr"></param>
/// <param name="shared_core"></param>
void execute_flush_on_bus(S_Multi_Core_Env* p_Cores_Env, E_Owner owner, E_Memory_Command cmd, int bus_Addr, int shared_core);

/// <execute_MSI_request>
/// 
/// Start MSI on first Bus request
/// <param name="p_Cores_Env"></param>
/// <param name="current_state"></param>
void execute_MSI_request(S_Multi_Core_Env* p_Cores_Env, Current_MSI_STATE current_state);

/// cuurent_MSI_state
/// 
/// Checking the state on the Bus request
/// <param name="p_Cores_Env"></param>
/// <param name="bus_origid"></param>
/// <param name="bus_Addr"></param>
/// <returns></returns>
Current_MSI_STATE cuurent_MSI_state(S_Multi_Core_Env* p_Cores_Env, E_Owner bus_origid, uint32_t bus_Addr);



#endif
