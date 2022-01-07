#pragma once

#ifndef __TYPEDEF_H__
#define __TYPEDEF_H__

#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <stdbool.h>


typedef enum Operation {
	E_ADD = 0,
	E_SUB = 1,
	E_AND = 2,
	E_OR = 3,
	E_XOR = 4,
	E_MUL = 5,
	E_SLL = 6,
	E_SRA = 7,
	E_SRL = 8,
	E_BEQ = 9,
	E_BNE = 10,
	E_BLT = 11,
	E_BGT = 12,
	E_BLE = 13,
	E_BGE = 14,
	E_JAL = 15,
	E_LW = 16,
	E_SW = 17,
	E_HALT = 20
}Operation;

typedef enum E_Core_Stage {
	E_FETCH			= 0,
	E_DECODE		= 1,
	E_EXECUTE		= 2,
	E_MEM			= 3,
	E_WRITE_BACK	= 4,
	E_STALL			= 5
}E_Core_Stage;

typedef enum E_Owner {
	E_CORE0			= 0,
	E_CORE1			= 1,
	E_CORE2			= 2,
	E_CORE3			= 3,
	E_MAIN_MEM		= 4,
	E_NO_CORE		= -1,
	E_MAX_VAL		= 0xF

}E_Owner;

typedef enum E_Memory_Command {
	E_NO_COMMAND	=0,
	E_BUS_RD		=1,
	E_BUS_RDX		=2,
	E_FLUSH			=3
}E_Memory_Command;

typedef enum Current_MSI_STATE {
	INVALID = 0,
	SHARED = 1,
	EXCLUSIVE = 2,
	MODIFIED = 3
}Current_MSI_STATE;

#define NUM_OF_COMMAND_LINES 27
#define MAX_NUM_OF_CORES 4

// A structure to represent a queue
struct Queue {
	int front, rear, size;
	unsigned capacity;
	int* array;
};

//struct for IF stage in Pipline
typedef struct S_Pipline_Fetch {
	uint32_t Fetch_IR;
	uint32_t PC;
	uint32_t NPC;
}S_Pipline_Fetch;

typedef struct S_Pipline_Decode {
	uint32_t Decode_Valid_Reg;
	uint32_t Decode_IR;
	uint32_t IMM;
	uint32_t Rtv : 4;
	uint32_t Rsv : 4;
	uint32_t Rd : 4;
	uint32_t Opcode : 8;
	uint32_t NPC;
}S_Pipline_Decode;

typedef struct S_Pipline_Execute {
	uint32_t IMM;
	uint32_t Rtv;
	uint32_t Rsv;
	uint32_t Rd;
	uint32_t R_Rd_MEM;
	uint32_t Opcode : 8;
	uint32_t Execute_IR;
	uint32_t Dest_Reg;
	uint32_t NPC;
	uint32_t ALU;
}S_Pipline_Execute;


typedef struct S_Pipline_Mem {
	uint32_t ALU;
	uint32_t Dest_Reg;
	uint32_t R_Dest_Reg;
	uint32_t Rtv;
	uint32_t Rsv;
	uint32_t Mem_IR;
	uint32_t NPC;
	uint32_t Execute_Opcode : 8;
	uint32_t MD;
	bool	 MD_MUX;

}S_Pipline_Mem;


typedef struct S_pipline_WriteBack {
	uint32_t NPC;
	uint32_t ALU;
	uint32_t MD;
	uint32_t Dest;
	uint32_t WtiteBack_IR;
	bool	 WB_MUX;
	uint32_t Execute_Opcode : 8;
	uint32_t Rsv;
	uint32_t Rtv;
}S_Pipline_WriteBack;

typedef struct S_MSI_Bus {
	E_Owner bus_origid;
	E_Memory_Command bus_cmd;
	uint32_t bus_Addr :20; 
	uint32_t bus_data;
}S_MSI_Bus;

typedef struct Queue_Bus {
	S_MSI_Bus MSI_Bus[4];
	uint32_t Num_Of_Queue;
	uint16_t Next_Free_Slot;
	uint32_t Num_Of_trans;
	uint32_t shared_core;
	uint32_t need_to_read_bus;
	uint32_t checked_and_shared;
	uint32_t checked_and_modified;
	uint32_t checked_and_exclusive;
	uint32_t modified_to_MM;
	uint32_t wait_16_cycles;
	uint32_t need_new_request_overwrite;
	uint32_t core_origid;
	uint32_t offset_origid;
	uint32_t cmd_origid;
	uint32_t data_origid;
	uint32_t addr_origid;
}Queue_Bus;

// struct of the core 
typedef struct S_Core {
	uint32_t* p_mem_core;
	uint32_t* p_DSram_Core;
	uint16_t* p_TSram_Core;
	FILE* p_memout_File_core;
	FILE* p_regout_File_core;
	FILE* p_CoreTrace_File;
	FILE* p_dsram_File_core;
	FILE* p_status_File_core;
	FILE* p_TSram_File_core;
	S_Pipline_Fetch		S_Pipline_Core_Fetch_Valid;
	S_Pipline_Fetch		S_Pipline_Core_Fetch_Next;
	S_Pipline_Decode	S_Pipline_Core_Decode_Valid;
	S_Pipline_Decode	S_Pipline_Core_Decode_Next;
	S_Pipline_Execute	S_Pipline_Core_Execute_Valid;
	S_Pipline_Execute	S_Pipline_Core_Execute_Next;
	S_Pipline_Mem		S_Pipline_Core_Mem_Next;
	S_Pipline_Mem		S_Pipline_Core_Mem_Valid;
	S_Pipline_WriteBack S_Pipline_Core_WB_Valid;
	S_Pipline_WriteBack S_Pipline_Core_WB_Next;
	uint8_t Core_Stage_Flag;
	uint8_t Core_Stage_Flag_D;
	uint8_t Core_Stage_Flag_Q;
	uint16_t Core_PC;
	uint16_t Core_PC_Q;
	int imem_Index;
	uint32_t Reg_Array[16];
	uint32_t Reg_Array_Q[16];
	bool Hazard_Flag[16];
	bool Hazard_Stall;
	bool bus_Stall;
	S_MSI_Bus Bus_Request;
	bool flag_Bus_Request;
	int instructions;
	int read_hit;
	int write_hit;
	int read_miss;
	int write_miss;
	int decode_stall;
	int mem_stall;

	
}S_Core, * P_S_Core;

typedef struct S_Multi_Core_Env {

	uint32_t PC;
	uint32_t Clock;
	char** data_path;
	FILE* p_bus_trace;
	FILE* p_memout;
	uint32_t* p_MainMemory;
	S_Core p_s_core[MAX_NUM_OF_CORES];
	Queue_Bus Queue_Bus; //maybe need to delete
	uint16_t Finish_Cores;

}S_Multi_Core_Env, * P_S_Multi_Core_Env;

typedef enum OS_Error {
	E_NO_ERROR = 0,
	E_ERROR_INIT = 1,
	E_ERROR_OPENING_FILE =2,
	E_ERROR_EXECUTE_STAGE = 3
}OS_Error;


#define CORE_STAGE_FLAG_FETCH_OFFSET	1
#define CORE_STAGE_FLAG_DECODE_OFFSET	2
#define CORE_STAGE_FLAG_EXECUTE_OFFSET	4
#define CORE_STAGE_FLAG_MEM_OFFSET		8
#define CORE_STAGE_FLAG_WB_OFFSET		16

#define CORE0_WORKING_FLAG				1
#define CORE1_WORKING_FLAG				2
#define CORE2_WORKING_FLAG				4
#define CORE3_WORKING_FLAG				8

//// printf for debug 
#define DEBUG_FETCH_PRINT		1
#define DEBUG_DECODE_PRINT		1
#define DEBUG_EXECUTE_PRINT		1
#define DEBUG_MEM_PRINT			1
#define DEBUG_WRITEBACK_PRINT	1
#define DEBUG_REGESTERS_PRINT	0
#define DEBUG_HAZARD_PRINT		0



#endif
