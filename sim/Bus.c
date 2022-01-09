
#include "Bus.h"



int init_Queue_bus(S_Multi_Core_Env* p_Cores_Env) //initilize the bus
{
    p_Cores_Env->Queue_Bus.Num_Of_Queue = 0;
    p_Cores_Env->Queue_Bus.Next_Free_Slot = 0;
    p_Cores_Env->Queue_Bus.Num_Of_trans = 0;
    p_Cores_Env->Queue_Bus.shared_core = -1;
    p_Cores_Env->Queue_Bus.need_to_read_bus = 0;
    p_Cores_Env->Queue_Bus.checked_and_shared = -1;
    p_Cores_Env->Queue_Bus.checked_and_modified = -1;
    p_Cores_Env->Queue_Bus.checked_and_exclusive = -1;
    p_Cores_Env->Queue_Bus.modified_to_MM = 0;
    p_Cores_Env->Queue_Bus.wait_16_cycles = 0;
    p_Cores_Env->Queue_Bus.need_new_request_overwrite = 0;
    p_Cores_Env->Queue_Bus.core_origid = -1;
	for (int ii = 0; ii < 4; ii++)
	{
		p_Cores_Env->Queue_Bus.MSI_Bus[ii].bus_origid = E_NO_CORE;
	}
	

    return 1;
}

void set_S_MSI_Bus(S_MSI_Bus* Bus_request, E_Owner bus_origid, E_Memory_Command bus_cmd, uint32_t bus_Addr, uint32_t bus_data)
{
	Bus_request->bus_origid = bus_origid;
	Bus_request->bus_cmd = bus_cmd;
	Bus_request->bus_Addr = bus_Addr;
    Bus_request->bus_data = bus_data;
}

void removeData(S_Multi_Core_Env* p_Cores_Env)
{
    int ii;
    Queue_Bus Q_temp;

    for (ii = 0; ii < 3; ii++)
    {
        Q_temp.MSI_Bus[ii] = p_Cores_Env->Queue_Bus.MSI_Bus[ii + 1];
    }
	Q_temp.MSI_Bus[3] = p_Cores_Env->Queue_Bus.MSI_Bus[2];

    p_Cores_Env->Queue_Bus.Num_Of_Queue = p_Cores_Env->Queue_Bus.Num_Of_Queue - 1; // update num of buses
    p_Cores_Env->Queue_Bus.Next_Free_Slot = p_Cores_Env->Queue_Bus.Next_Free_Slot - 1; // update the next free slot
    for (ii = 0; ii < 4; ii++)
    {
        p_Cores_Env->Queue_Bus.MSI_Bus[ii] = Q_temp.MSI_Bus[ii];
    }


    p_Cores_Env->Queue_Bus.Num_Of_trans = 0;
    p_Cores_Env->Queue_Bus.shared_core = -1;
    p_Cores_Env->Queue_Bus.need_to_read_bus = 0;
    p_Cores_Env->Queue_Bus.checked_and_shared = -1;
    p_Cores_Env->Queue_Bus.checked_and_modified = -1;
    p_Cores_Env->Queue_Bus.checked_and_exclusive = -1;
    p_Cores_Env->Queue_Bus.modified_to_MM = 0;
    p_Cores_Env->Queue_Bus.wait_16_cycles = 0;
    p_Cores_Env->Queue_Bus.need_new_request_overwrite = 0;
	p_Cores_Env->p_s_core[p_Cores_Env->Queue_Bus.core_origid].bus_Stall = false;
    p_Cores_Env->Queue_Bus.core_origid = -1;
}

Current_MSI_STATE cuurent_MSI_state(S_Multi_Core_Env* p_Cores_Env, E_Owner bus_origid, uint32_t bus_Addr)
{
	int index = (bus_Addr / 4) & 0x003f;
	int tag = bus_Addr / 256;
	printf("Current_state: %d", p_Cores_Env->p_s_core[bus_origid].p_TSram_Core[index]);
	return (p_Cores_Env->p_s_core[bus_origid].p_TSram_Core[index] / 4096);
}
S_MSI_Bus Pop_first_in_queue(S_Multi_Core_Env* p_Cores_Env)
{
    S_MSI_Bus Bus_temp;

    Bus_temp = p_Cores_Env->Queue_Bus.MSI_Bus[0];
    removeData(p_Cores_Env);

    return Bus_temp;
}

bool isEmpty(S_Multi_Core_Env* p_Cores_Env)
{
    
    if (p_Cores_Env->Queue_Bus.Num_Of_Queue == 0)
        return true;
    else
        return false;
}

bool isFull(S_Multi_Core_Env* p_Cores_Env) 
{

    if (p_Cores_Env->Queue_Bus.Num_Of_Queue == 4)
        return true;
    else
        return false;
}

int userInQueue(S_Multi_Core_Env* p_Cores_Env, S_MSI_Bus* Bus)
{
    for (int i = 0; i < p_Cores_Env->Queue_Bus.Num_Of_Queue; i++)
    {
        if (Bus->bus_Addr == p_Cores_Env->Queue_Bus.MSI_Bus[i].bus_Addr)
            return i;
    }
    return -1;
}

int size(S_Multi_Core_Env* p_Cores_Env)
{
    return p_Cores_Env->Queue_Bus.Num_Of_Queue;
}

void new_request(S_Multi_Core_Env* p_Cores_Env, S_MSI_Bus Bus)
{

    if (!isFull(p_Cores_Env)) {

        p_Cores_Env->Queue_Bus.MSI_Bus[p_Cores_Env->Queue_Bus.Next_Free_Slot] = Bus;
        p_Cores_Env->Queue_Bus.Next_Free_Slot++;
        p_Cores_Env->Queue_Bus.Num_Of_Queue++;
        p_Cores_Env->p_s_core[Bus.bus_origid].bus_Stall = true;
    }
}

S_MSI_Bus getNextfromQueue(S_Multi_Core_Env* p_Cores_Env)
{
    return p_Cores_Env->Queue_Bus.MSI_Bus[0];
}

int check_bus_shared(S_Multi_Core_Env* p_Cores_Env, int bus_Addr, E_Owner core_index)
{
	int offset = bus_Addr & 0x0003;
	int index = (bus_Addr / 4) & 0x003f;
	int tag = bus_Addr / 256;
	int bus_shared = 0;
	switch (core_index)
	{
	case E_CORE0:
	{

		if (((p_Cores_Env->p_s_core[1].p_TSram_Core[index] & 0x0fff) == tag) && ((p_Cores_Env->p_s_core[1].p_TSram_Core[index] / 4096) == 1))
		{
			bus_shared = 1;
			p_Cores_Env->Queue_Bus.checked_and_shared = 1;
		}
		else if (((p_Cores_Env->p_s_core[1].p_TSram_Core[index] & 0x0fff) == tag) && ((p_Cores_Env->p_s_core[1].p_TSram_Core[index] / 4096) == 2))
		{
			p_Cores_Env->Queue_Bus.checked_and_exclusive = 1;
		}
		else if (((p_Cores_Env->p_s_core[1].p_TSram_Core[index] & 0x0fff) == tag) && ((p_Cores_Env->p_s_core[1].p_TSram_Core[index] / 4096) == 3))
		{
			bus_shared = 1;
			p_Cores_Env->Queue_Bus.checked_and_modified = 1;
		}

		if (((p_Cores_Env->p_s_core[2].p_TSram_Core[index] & 0x0fff) == tag) && ((p_Cores_Env->p_s_core[2].p_TSram_Core[index] / 4096) == 1))
		{
			bus_shared = 1;
			p_Cores_Env->Queue_Bus.checked_and_shared = 1;
		}
		else if (((p_Cores_Env->p_s_core[2].p_TSram_Core[index] & 0x0fff) == tag) && ((p_Cores_Env->p_s_core[2].p_TSram_Core[index] / 4096) == 2))
		{
			p_Cores_Env->Queue_Bus.checked_and_exclusive = 1;
		}
		else if (((p_Cores_Env->p_s_core[2].p_TSram_Core[index] & 0x0fff) == tag) && ((p_Cores_Env->p_s_core[2].p_TSram_Core[index] / 4096) == 3))
		{
			bus_shared = 1;
			p_Cores_Env->Queue_Bus.checked_and_modified = 1;
		}

		if (((p_Cores_Env->p_s_core[3].p_TSram_Core[index] & 0x0fff) == tag) && ((p_Cores_Env->p_s_core[3].p_TSram_Core[index] / 4096) == 1))
		{
			bus_shared = 1;
			p_Cores_Env->Queue_Bus.checked_and_shared = 1;
		}
		else if (((p_Cores_Env->p_s_core[3].p_TSram_Core[index] & 0x0fff) == tag) && ((p_Cores_Env->p_s_core[3].p_TSram_Core[index] / 4096) == 2))
		{
			p_Cores_Env->Queue_Bus.checked_and_exclusive = 1;
		}
		else if (((p_Cores_Env->p_s_core[3].p_TSram_Core[index] & 0x0fff) == tag) && ((p_Cores_Env->p_s_core[3].p_TSram_Core[index] / 4096) == 3))
		{
			bus_shared = 1;
			p_Cores_Env->Queue_Bus.checked_and_modified = 1;
		}
		break;
	}
	case E_CORE1:
	{
		if (((p_Cores_Env->p_s_core[0].p_TSram_Core[index] & 0x0fff) == tag) && ((p_Cores_Env->p_s_core[0].p_TSram_Core[index] / 4096) == 1))
		{
			bus_shared = 1;
			p_Cores_Env->Queue_Bus.checked_and_shared = 1;
		}
		else if (((p_Cores_Env->p_s_core[0].p_TSram_Core[index] & 0x0fff) == tag) && ((p_Cores_Env->p_s_core[0].p_TSram_Core[index] / 4096) == 2))
		{
			p_Cores_Env->Queue_Bus.checked_and_exclusive = 1;
		}
		else if (((p_Cores_Env->p_s_core[0].p_TSram_Core[index] & 0x0fff) == tag) && ((p_Cores_Env->p_s_core[0].p_TSram_Core[index] / 4096) == 3))
		{
			bus_shared = 1;
			p_Cores_Env->Queue_Bus.checked_and_modified = 1;
		}

		if (((p_Cores_Env->p_s_core[2].p_TSram_Core[index] & 0x0fff) == tag) && ((p_Cores_Env->p_s_core[2].p_TSram_Core[index] / 4096) == 1))
		{
			bus_shared = 1;
			p_Cores_Env->Queue_Bus.checked_and_shared = 1;
		}
		else if (((p_Cores_Env->p_s_core[2].p_TSram_Core[index] & 0x0fff) == tag) && ((p_Cores_Env->p_s_core[2].p_TSram_Core[index] / 4096) == 2))
		{
			p_Cores_Env->Queue_Bus.checked_and_exclusive = 1;
		}
		else if (((p_Cores_Env->p_s_core[2].p_TSram_Core[index] & 0x0fff) == tag) && ((p_Cores_Env->p_s_core[2].p_TSram_Core[index] / 4096) == 3))
		{
			bus_shared = 1;
			p_Cores_Env->Queue_Bus.checked_and_modified = 1;
		}

		if (((p_Cores_Env->p_s_core[3].p_TSram_Core[index] & 0x0fff) == tag) && ((p_Cores_Env->p_s_core[3].p_TSram_Core[index] / 4096) == 1))
		{
			bus_shared = 1;
			p_Cores_Env->Queue_Bus.checked_and_shared = 1;
		}
		else if (((p_Cores_Env->p_s_core[3].p_TSram_Core[index] & 0x0fff) == tag) && ((p_Cores_Env->p_s_core[3].p_TSram_Core[index] / 4096) == 2))
		{
			p_Cores_Env->Queue_Bus.checked_and_exclusive = 1;
		}
		else if (((p_Cores_Env->p_s_core[3].p_TSram_Core[index] & 0x0fff) == tag) && ((p_Cores_Env->p_s_core[3].p_TSram_Core[index] / 4096) == 3))
		{
			bus_shared = 1;
			p_Cores_Env->Queue_Bus.checked_and_modified = 1;
		}
		break;
	}
	case E_CORE2:
	{
		if (((p_Cores_Env->p_s_core[0].p_TSram_Core[index] & 0x0fff) == tag) && ((p_Cores_Env->p_s_core[0].p_TSram_Core[index] / 4096) == 1))
		{
			p_Cores_Env->Queue_Bus.checked_and_shared = 1;
		}
		else if (((p_Cores_Env->p_s_core[0].p_TSram_Core[index] & 0x0fff) == tag) && ((p_Cores_Env->p_s_core[0].p_TSram_Core[index] / 4096) == 2))
		{
			p_Cores_Env->Queue_Bus.checked_and_exclusive = 1;
		}
		else if (((p_Cores_Env->p_s_core[0].p_TSram_Core[index] & 0x0fff) == tag) && ((p_Cores_Env->p_s_core[0].p_TSram_Core[index] / 4096) == 3))
		{
			bus_shared = 1;
			p_Cores_Env->Queue_Bus.checked_and_modified = 1;
		}

		if (((p_Cores_Env->p_s_core[1].p_TSram_Core[index] & 0x0fff) == tag) && ((p_Cores_Env->p_s_core[1].p_TSram_Core[index] / 4096) == 1))
		{
			p_Cores_Env->Queue_Bus.checked_and_shared = 1;
		}
		else if (((p_Cores_Env->p_s_core[1].p_TSram_Core[index] & 0x0fff) == tag) && ((p_Cores_Env->p_s_core[1].p_TSram_Core[index] / 4096) == 2))
		{
			p_Cores_Env->Queue_Bus.checked_and_exclusive = 1;
		}
		else if (((p_Cores_Env->p_s_core[1].p_TSram_Core[index] & 0x0fff) == tag) && ((p_Cores_Env->p_s_core[1].p_TSram_Core[index] / 4096) == 3))
		{
			bus_shared = 1;
			p_Cores_Env->Queue_Bus.checked_and_modified = 1;
		}

		if (((p_Cores_Env->p_s_core[3].p_TSram_Core[index] & 0x0fff) == tag) && ((p_Cores_Env->p_s_core[3].p_TSram_Core[index] / 4096) == 1))
		{
			p_Cores_Env->Queue_Bus.checked_and_shared = 1;
		}
		else if (((p_Cores_Env->p_s_core[3].p_TSram_Core[index] & 0x0fff) == tag) && ((p_Cores_Env->p_s_core[3].p_TSram_Core[index] / 4096) == 2))
		{
			p_Cores_Env->Queue_Bus.checked_and_exclusive = 1;
		}
		else if (((p_Cores_Env->p_s_core[3].p_TSram_Core[index] & 0x0fff) == tag) && ((p_Cores_Env->p_s_core[3].p_TSram_Core[index] / 4096) == 3))
		{
			bus_shared = 1;
			p_Cores_Env->Queue_Bus.checked_and_modified = 1;
		}
		break;
	}
	case E_CORE3:
	{
		if (((p_Cores_Env->p_s_core[0].p_TSram_Core[index] & 0x0fff) == tag) && ((p_Cores_Env->p_s_core[0].p_TSram_Core[index] / 4096) == 1))
		{
			p_Cores_Env->Queue_Bus.checked_and_shared = 1;
		}
		else if (((p_Cores_Env->p_s_core[0].p_TSram_Core[index] & 0x0fff) == tag) && ((p_Cores_Env->p_s_core[0].p_TSram_Core[index] / 4096) == 2))
		{
			p_Cores_Env->Queue_Bus.checked_and_exclusive = 1;
		}
		else if (((p_Cores_Env->p_s_core[0].p_TSram_Core[index] & 0x0fff) == tag) && ((p_Cores_Env->p_s_core[0].p_TSram_Core[index] / 4096) == 3))
		{
			bus_shared = 1;
			p_Cores_Env->Queue_Bus.checked_and_modified = 1;
		}

		if (((p_Cores_Env->p_s_core[1].p_TSram_Core[index] & 0x0fff) == tag) && ((p_Cores_Env->p_s_core[1].p_TSram_Core[index] / 4096) == 1))
		{
			p_Cores_Env->Queue_Bus.checked_and_shared = 1;
		}
		else if (((p_Cores_Env->p_s_core[1].p_TSram_Core[index] & 0x0fff) == tag) && ((p_Cores_Env->p_s_core[1].p_TSram_Core[index] / 4096) == 2))
		{
			p_Cores_Env->Queue_Bus.checked_and_exclusive = 1;
		}
		else if (((p_Cores_Env->p_s_core[1].p_TSram_Core[index] & 0x0fff) == tag) && ((p_Cores_Env->p_s_core[1].p_TSram_Core[index] / 4096) == 3))
		{
			bus_shared = 1;
			p_Cores_Env->Queue_Bus.checked_and_modified = 1;
		}

		if (((p_Cores_Env->p_s_core[2].p_TSram_Core[index] & 0x0fff) == tag) && ((p_Cores_Env->p_s_core[2].p_TSram_Core[index] / 4096) == 1))
		{
			p_Cores_Env->Queue_Bus.checked_and_shared = 1;
		}
		else if (((p_Cores_Env->p_s_core[2].p_TSram_Core[index] & 0x0fff) == tag) && ((p_Cores_Env->p_s_core[2].p_TSram_Core[index] / 4096) == 2))
		{
			p_Cores_Env->Queue_Bus.checked_and_exclusive = 1;
		}
		else if (((p_Cores_Env->p_s_core[2].p_TSram_Core[index] & 0x0fff) == tag) && ((p_Cores_Env->p_s_core[2].p_TSram_Core[index] / 4096) == 3))
		{
			bus_shared = 1;
			p_Cores_Env->Queue_Bus.checked_and_modified = 1;
		}
		break;
	}
	}
	return bus_shared;
}

void snooping_func(S_Multi_Core_Env* p_Cores_Env, int shared_core, E_Owner core, E_Memory_Command cmd, Current_MSI_STATE current_state)
{
	int index = (p_Cores_Env->Queue_Bus.MSI_Bus[0].bus_Addr / 4) & 0x003f;
	int tag = p_Cores_Env->Queue_Bus.MSI_Bus[0].bus_Addr / 256;
	if (current_state != 2)
	{
		Output_Bus_Trace(p_Cores_Env, p_Cores_Env->Clock);
	}
	int is_same_tag = 0;
	if ((p_Cores_Env->p_s_core[core].p_TSram_Core[index] & 0x0fff) == tag)
	{
		is_same_tag = 1;
	}

	for (int i_core = 0; i_core < 4; i_core++)
	{
		if (i_core == (int)core) // if the core asked for the bus
		{
			if (current_state == 0) // INVALID state
			{
				if (cmd == 1) //BusRD
				{
					if (shared_core)
					{
						p_Cores_Env->p_s_core[core].read_miss += 1;
						p_Cores_Env->p_s_core[i_core].p_TSram_Core[index] = (p_Cores_Env->p_s_core[i_core].p_TSram_Core[index] & 0xFFF) | 0x1000;
						p_Cores_Env->Queue_Bus.MSI_Bus[0].bus_cmd = E_FLUSH;
						p_Cores_Env->Queue_Bus.need_to_read_bus = 1;
						p_Cores_Env->Queue_Bus.MSI_Bus[0].bus_origid = shared_core;
					}
					else
					{
						p_Cores_Env->p_s_core[core].read_miss += 1;
						p_Cores_Env->p_s_core[i_core].p_TSram_Core[index] = (p_Cores_Env->p_s_core[i_core].p_TSram_Core[index] & 0xFFF) | 0x2000;
						p_Cores_Env->Queue_Bus.MSI_Bus[0].bus_cmd = E_FLUSH;
						p_Cores_Env->Queue_Bus.MSI_Bus[0].bus_origid = 4;
						p_Cores_Env->Queue_Bus.need_to_read_bus = 1;
						p_Cores_Env->Queue_Bus.MSI_Bus[0].bus_origid = E_MAIN_MEM;
						p_Cores_Env->Queue_Bus.wait_16_cycles = 17;
					}
				}
				if (cmd == 2) //BusRDX
				{
					p_Cores_Env->p_s_core[core].write_miss += 1;
					p_Cores_Env->p_s_core[i_core].p_TSram_Core[index] = (p_Cores_Env->p_s_core[i_core].p_TSram_Core[index] & 0xFFF) | 0x3000;
					p_Cores_Env->Queue_Bus.MSI_Bus[0].bus_cmd = E_FLUSH;
					p_Cores_Env->Queue_Bus.MSI_Bus[0].bus_origid = E_MAIN_MEM;
					p_Cores_Env->Queue_Bus.need_to_read_bus = 1;
					p_Cores_Env->Queue_Bus.wait_16_cycles = 17;
				}
				if (!is_same_tag)
				{
					p_Cores_Env->p_s_core[i_core].p_TSram_Core[index] = (p_Cores_Env->p_s_core[i_core].p_TSram_Core[index] & 0xF000) + tag;
				}
			}
			if (current_state == 1) // SHARED
			{
				if (is_same_tag)
				{
					if (cmd == 1) //BusRd
					{
						p_Cores_Env->p_s_core[core].read_hit += 1;
						p_Cores_Env->Queue_Bus.MSI_Bus[0].bus_cmd = E_NO_COMMAND;
						p_Cores_Env->Queue_Bus.need_to_read_bus = 0;
					}
					else if (cmd == 2) //BusRDX
					{
						p_Cores_Env->p_s_core[core].write_miss += 1;
						p_Cores_Env->p_s_core[i_core].p_TSram_Core[index] = (p_Cores_Env->p_s_core[i_core].p_TSram_Core[index] & 0xFFF) | 0x3000;
						p_Cores_Env->Queue_Bus.MSI_Bus[0].bus_cmd = E_NO_COMMAND;
						p_Cores_Env->Queue_Bus.need_to_read_bus = 0;
					}
				}
				else
				{
					p_Cores_Env->p_s_core[i_core].p_TSram_Core[index] = (p_Cores_Env->p_s_core[i_core].p_TSram_Core[index] & 0x0000) + tag;
					if (cmd == 1) //BusRD
					{
						if (shared_core)
						{
							p_Cores_Env->p_s_core[core].read_miss += 1;
							p_Cores_Env->p_s_core[i_core].p_TSram_Core[index] = (p_Cores_Env->p_s_core[i_core].p_TSram_Core[index] & 0xFFF) | 0x1000;
							p_Cores_Env->Queue_Bus.MSI_Bus[0].bus_cmd = E_FLUSH;
							p_Cores_Env->Queue_Bus.need_to_read_bus = 1;
							p_Cores_Env->Queue_Bus.MSI_Bus[0].bus_origid = shared_core;
						}
						else
						{
							p_Cores_Env->p_s_core[core].read_miss += 1;
							p_Cores_Env->p_s_core[i_core].p_TSram_Core[index] = (p_Cores_Env->p_s_core[i_core].p_TSram_Core[index] & 0xFFF) | 0x2000;
							p_Cores_Env->Queue_Bus.MSI_Bus[0].bus_cmd = E_FLUSH;
							p_Cores_Env->Queue_Bus.MSI_Bus[0].bus_origid = 4;
							p_Cores_Env->Queue_Bus.need_to_read_bus = 1;
							p_Cores_Env->Queue_Bus.MSI_Bus[0].bus_origid = E_MAIN_MEM;
							p_Cores_Env->Queue_Bus.wait_16_cycles = 17;
						}
					}
					if (cmd == 2) //BusRDX
					{
						p_Cores_Env->p_s_core[core].write_miss += 1;
						p_Cores_Env->p_s_core[i_core].p_TSram_Core[index] = (p_Cores_Env->p_s_core[i_core].p_TSram_Core[index] & 0xFFF) | 0x3000;
						p_Cores_Env->Queue_Bus.MSI_Bus[0].bus_cmd = E_FLUSH;
						p_Cores_Env->Queue_Bus.MSI_Bus[0].bus_origid = E_MAIN_MEM;
						p_Cores_Env->Queue_Bus.need_to_read_bus = 1;
						p_Cores_Env->Queue_Bus.wait_16_cycles = 17;
					}
				}

			}
			if (current_state == 2) // EXCLUSIVE
			{
				if (is_same_tag)
				{
					if (cmd == 1) //BusRd
					{
						p_Cores_Env->p_s_core[core].read_hit += 1;
						p_Cores_Env->Queue_Bus.MSI_Bus[0].bus_cmd = E_NO_COMMAND;
						p_Cores_Env->Queue_Bus.need_to_read_bus = 0;
					}
					else if (cmd == 2) //BusRDX
					{
						p_Cores_Env->p_s_core[core].write_hit += 1;
						p_Cores_Env->p_s_core[i_core].p_TSram_Core[index] = (p_Cores_Env->p_s_core[i_core].p_TSram_Core[index] & 0xFFF) | 0x3000;
						p_Cores_Env->Queue_Bus.MSI_Bus[0].bus_cmd = E_NO_COMMAND;
						p_Cores_Env->Queue_Bus.need_to_read_bus = 0;
					}
				}
				else
				{
					p_Cores_Env->p_s_core[i_core].p_TSram_Core[index] = (p_Cores_Env->p_s_core[i_core].p_TSram_Core[index] & 0x0000) + tag;
					if (cmd == 1) //BusRD
					{
						if (shared_core)
						{
							p_Cores_Env->p_s_core[core].read_miss += 1;
							p_Cores_Env->p_s_core[i_core].p_TSram_Core[index] = (p_Cores_Env->p_s_core[i_core].p_TSram_Core[index] & 0xFFF) | 0x1000;
							p_Cores_Env->Queue_Bus.MSI_Bus[0].bus_cmd = E_FLUSH;
							p_Cores_Env->Queue_Bus.need_to_read_bus = 1;
							p_Cores_Env->Queue_Bus.MSI_Bus[0].bus_origid = shared_core;
						}
						else
						{
							p_Cores_Env->p_s_core[core].read_miss += 1;
							p_Cores_Env->p_s_core[i_core].p_TSram_Core[index] = (p_Cores_Env->p_s_core[i_core].p_TSram_Core[index] & 0xFFF) | 0x2000;
							p_Cores_Env->Queue_Bus.MSI_Bus[0].bus_cmd = E_FLUSH;
							p_Cores_Env->Queue_Bus.MSI_Bus[0].bus_origid = 4;
							p_Cores_Env->Queue_Bus.need_to_read_bus = 1;
							p_Cores_Env->Queue_Bus.MSI_Bus[0].bus_origid = E_MAIN_MEM;
							p_Cores_Env->Queue_Bus.wait_16_cycles = 17;
						}
					}
					if (cmd == 2) //BusRDX
					{
						p_Cores_Env->p_s_core[core].write_miss += 1;
						p_Cores_Env->p_s_core[i_core].p_TSram_Core[index] = (p_Cores_Env->p_s_core[i_core].p_TSram_Core[index] & 0xFFF) | 0x3000;
						p_Cores_Env->Queue_Bus.MSI_Bus[0].bus_cmd = E_FLUSH;
						p_Cores_Env->Queue_Bus.MSI_Bus[0].bus_origid = E_MAIN_MEM;
						p_Cores_Env->Queue_Bus.need_to_read_bus = 1;
						p_Cores_Env->Queue_Bus.wait_16_cycles = 17;
					}
				}
			}
			if (current_state == 3) // MODIFIED
			{
				if (is_same_tag)
				{
					if (cmd == 1) //BusRd
					{
						p_Cores_Env->p_s_core[core].read_hit += 1;

					}
					else if (cmd == 2) //BusRDX
					{
						p_Cores_Env->p_s_core[core].write_miss += 1;
					}
					p_Cores_Env->Queue_Bus.MSI_Bus[0].bus_cmd = E_NO_COMMAND;
					p_Cores_Env->Queue_Bus.need_to_read_bus = 0;
				}
				else
				{
					p_Cores_Env->p_s_core[i_core].p_TSram_Core[index] = (p_Cores_Env->p_s_core[i_core].p_TSram_Core[index] & 0x0000) + tag;
					if (cmd == 1) //BusRD
					{
						if (shared_core)
						{
							p_Cores_Env->p_s_core[core].read_miss += 1;
							p_Cores_Env->p_s_core[i_core].p_TSram_Core[index] = (p_Cores_Env->p_s_core[i_core].p_TSram_Core[index] & 0xFFF) | 0x1000;
							p_Cores_Env->Queue_Bus.MSI_Bus[0].bus_cmd = E_FLUSH;
							p_Cores_Env->Queue_Bus.need_to_read_bus = 1;
							p_Cores_Env->Queue_Bus.MSI_Bus[0].bus_origid = shared_core;
						}
						else
						{
							p_Cores_Env->p_s_core[core].read_miss += 1;
							p_Cores_Env->p_s_core[i_core].p_TSram_Core[index] = (p_Cores_Env->p_s_core[i_core].p_TSram_Core[index] & 0xFFF) | 0x2000;
							p_Cores_Env->Queue_Bus.MSI_Bus[0].bus_cmd = E_FLUSH;
							p_Cores_Env->Queue_Bus.MSI_Bus[0].bus_origid = 4;
							p_Cores_Env->Queue_Bus.need_to_read_bus = 1;
							p_Cores_Env->Queue_Bus.MSI_Bus[0].bus_origid = E_MAIN_MEM;
							p_Cores_Env->Queue_Bus.wait_16_cycles = 17;
						}
					}
					if (cmd == 2) //BusRDX
					{
						p_Cores_Env->p_s_core[core].write_miss += 1;
						p_Cores_Env->p_s_core[i_core].p_TSram_Core[index] = (p_Cores_Env->p_s_core[i_core].p_TSram_Core[index] & 0xFFF) | 0x3000;
						p_Cores_Env->Queue_Bus.MSI_Bus[0].bus_cmd = E_FLUSH;
						p_Cores_Env->Queue_Bus.MSI_Bus[0].bus_origid = E_MAIN_MEM;
						p_Cores_Env->Queue_Bus.need_to_read_bus = 1;
						p_Cores_Env->Queue_Bus.wait_16_cycles = 17;
					}
				}

			}
		}
	}

	//Handle the not requesters cores 
	for (int i_core = 0; i_core < 5; i_core++)
	{
		if (i_core == p_Cores_Env->Queue_Bus.checked_and_shared)
		{
			if (cmd == 1) //BusRd
			{
				break;
			}
			else if (cmd == 2) //BusRDX
			{
				p_Cores_Env->p_s_core[i_core].p_TSram_Core[index] = (p_Cores_Env->p_s_core[i_core].p_TSram_Core[index] & 0xFFF);
			}
		}
		else if (i_core == p_Cores_Env->Queue_Bus.checked_and_modified)
		{
			if (cmd == 1) //BusRd
			{
				p_Cores_Env->p_s_core[i_core].p_TSram_Core[index] = (p_Cores_Env->p_s_core[i_core].p_TSram_Core[index] & 0xFFF) | 0x1000;
				p_Cores_Env->Queue_Bus.MSI_Bus[0].bus_cmd = E_FLUSH;
				p_Cores_Env->Queue_Bus.modified_to_MM = 1;
			}
			else if (cmd == 2) //BusRDX
			{
				p_Cores_Env->p_s_core[i_core].p_TSram_Core[index] = (p_Cores_Env->p_s_core[i_core].p_TSram_Core[index] & 0xFFF);
				p_Cores_Env->Queue_Bus.MSI_Bus[0].bus_cmd = E_FLUSH;
				p_Cores_Env->Queue_Bus.MSI_Bus[0].bus_origid = i_core;
			}
		}
		else if (i_core == p_Cores_Env->Queue_Bus.checked_and_exclusive)
		{
			if (cmd == 1) //BusRd
			{
				p_Cores_Env->p_s_core[i_core].p_TSram_Core[index] = (p_Cores_Env->p_s_core[i_core].p_TSram_Core[index] & 0xFFF) | 0x1000;
			}
			else if (cmd == 2) //BusRDX
			{
				p_Cores_Env->p_s_core[i_core].p_TSram_Core[index] = (p_Cores_Env->p_s_core[i_core].p_TSram_Core[index] & 0xFFF);
			}
		}
	}
}

void execute_flush_on_bus(S_Multi_Core_Env* p_Cores_Env, E_Owner owner, E_Memory_Command cmd, int bus_Addr, int shared_core)
{
	int offset = bus_Addr & 0x0003;
	int index = (bus_Addr / 4) & 0x003f;
	int tag = bus_Addr / 256;
	uint32_t* p_MainMemory = p_Cores_Env->p_MainMemory;
	if (p_Cores_Env->Queue_Bus.MSI_Bus[0].bus_cmd == E_FLUSH)
	{
		if (owner != 4)
		{
			if (p_Cores_Env->Queue_Bus.modified_to_MM)
			{
				p_Cores_Env->p_MainMemory[bus_Addr] = p_Cores_Env->p_s_core[shared_core].p_DSram_Core[index];
			}
			p_Cores_Env->Queue_Bus.MSI_Bus[0].bus_data = p_Cores_Env->p_s_core[shared_core].p_TSram_Core[index];
		}
		else if (owner == 4)
		{
			p_Cores_Env->Queue_Bus.MSI_Bus[0].bus_data = p_MainMemory[bus_Addr]; // need to implement main memory
		}
	}
	return;
}

void execute_MSI_request(S_Multi_Core_Env* p_Cores_Env, Current_MSI_STATE current_state) //nead to get from main the current state in TSRAM
{
	S_MSI_Bus current_request = getNextfromQueue(p_Cores_Env);
	int bus_Addr = current_request.bus_Addr;
	int core_index = p_Cores_Env->Queue_Bus.MSI_Bus[0].bus_origid;
	int cmd = p_Cores_Env->Queue_Bus.MSI_Bus[0].bus_cmd;
	int bus_data = p_Cores_Env->Queue_Bus.MSI_Bus[0].bus_data;
	if ((p_Cores_Env->Queue_Bus.Num_Of_trans == 0) && (p_Cores_Env->Queue_Bus.MSI_Bus[0].bus_origid != 4))
	{
		p_Cores_Env->Queue_Bus.addr_origid = bus_Addr;
		p_Cores_Env->Queue_Bus.MSI_Bus[0].bus_Addr = (p_Cores_Env->Queue_Bus.MSI_Bus[0].bus_Addr / 4) * 4;
		p_Cores_Env->Queue_Bus.cmd_origid = cmd;
		p_Cores_Env->Queue_Bus.offset_origid = bus_Addr % 4;
		p_Cores_Env->Queue_Bus.core_origid = core_index;
		p_Cores_Env->Queue_Bus.data_origid = bus_data;
		p_Cores_Env->Queue_Bus.shared_core = check_bus_shared(p_Cores_Env, bus_Addr, core_index); //update paramters for snooping
		snooping_func(p_Cores_Env, p_Cores_Env->Queue_Bus.shared_core, core_index, cmd, current_state); // execute snooping and update state
	}

	if (p_Cores_Env->Queue_Bus.MSI_Bus[0].bus_origid != 0)
	{
		p_Cores_Env->p_s_core[p_Cores_Env->Queue_Bus.core_origid].mem_stall += 1;
	}

	cmd = p_Cores_Env->Queue_Bus.MSI_Bus[0].bus_cmd;
	int owner = p_Cores_Env->Queue_Bus.MSI_Bus[0].bus_origid;
	if (p_Cores_Env->Queue_Bus.MSI_Bus[0].bus_origid != 4) // handle read not from main memory
	{
		execute_flush_on_bus(p_Cores_Env, owner, cmd, bus_Addr, p_Cores_Env->Queue_Bus.shared_core);
		int offset = bus_Addr & 0x0003;
		int index = (bus_Addr / 4) & 0x003f;
		int tag = bus_Addr / 256;
		if (p_Cores_Env->Queue_Bus.MSI_Bus[0].bus_cmd == E_FLUSH)
		{
			Output_Bus_Trace(p_Cores_Env, p_Cores_Env->Clock);
			p_Cores_Env->p_s_core[p_Cores_Env->Queue_Bus.core_origid].p_DSram_Core[(index + offset)] = p_Cores_Env->Queue_Bus.MSI_Bus[0].bus_data;
			p_Cores_Env->Queue_Bus.Num_Of_trans += 1;
			p_Cores_Env->Queue_Bus.MSI_Bus[0].bus_Addr += 1;
		}
		else if (p_Cores_Env->Queue_Bus.cmd_origid == E_BUS_RDX && p_Cores_Env->Queue_Bus.MSI_Bus[0].bus_cmd == E_NO_COMMAND) // write hit
		{
			p_Cores_Env->p_s_core[p_Cores_Env->Queue_Bus.core_origid].S_Pipline_Core_Mem_Next.MD = 0;
			p_Cores_Env->p_s_core[p_Cores_Env->Queue_Bus.core_origid].p_DSram_Core[(p_Cores_Env->Queue_Bus.MSI_Bus[0].bus_Addr + p_Cores_Env->Queue_Bus.offset_origid)] = p_Cores_Env->Queue_Bus.MSI_Bus[0].bus_data;
			removeData(p_Cores_Env);
			p_Cores_Env->p_s_core[p_Cores_Env->Queue_Bus.core_origid].bus_Stall = false;
			return;
		}
		else
		{
			p_Cores_Env->p_s_core[p_Cores_Env->Queue_Bus.core_origid].S_Pipline_Core_Mem_Next.MD = 1;
			removeData(p_Cores_Env);
			p_Cores_Env->p_s_core[p_Cores_Env->Queue_Bus.core_origid].bus_Stall = false;
			return;
		}
	}
	else if (p_Cores_Env->Queue_Bus.wait_16_cycles != 0)
		p_Cores_Env->Queue_Bus.wait_16_cycles -= 1;
	if (p_Cores_Env->Queue_Bus.wait_16_cycles == 0) // handle reed from main memory
	{
		execute_flush_on_bus(p_Cores_Env, owner, cmd, bus_Addr, p_Cores_Env->Queue_Bus.shared_core);
		int offset = bus_Addr & 0x0003;
		int index = (bus_Addr / 4) & 0x03f;
		int tag = bus_Addr / 256;
		p_Cores_Env->p_s_core[p_Cores_Env->Queue_Bus.core_origid].p_DSram_Core[(index*4 + offset)] = p_Cores_Env->Queue_Bus.MSI_Bus[0].bus_data; //update the cache
		Output_Bus_Trace(p_Cores_Env, p_Cores_Env->Clock);
		p_Cores_Env->Queue_Bus.Num_Of_trans += 1;
		p_Cores_Env->Queue_Bus.MSI_Bus[0].bus_Addr += 1;
	}
	// if finish transmit
	if (p_Cores_Env->Queue_Bus.Num_Of_trans == 4)
	{
		if (p_Cores_Env->Queue_Bus.cmd_origid == 2) // when finish transimt block from MM write to the cache the relevant value
		{
			p_Cores_Env->p_s_core[p_Cores_Env->Queue_Bus.core_origid].p_DSram_Core[(p_Cores_Env->Queue_Bus.MSI_Bus[0].bus_Addr-4) + (p_Cores_Env->Queue_Bus.offset_origid)] = p_Cores_Env->Queue_Bus.data_origid;
		}
		int index = ((bus_Addr - 1) / 4) & 0x00ff;
		int last_addr = index + p_Cores_Env->Queue_Bus.offset_origid;
		p_Cores_Env->p_s_core[p_Cores_Env->Queue_Bus.core_origid].S_Pipline_Core_Mem_Next.MD =1;
		removeData(p_Cores_Env);
		p_Cores_Env->p_s_core[p_Cores_Env->Queue_Bus.core_origid].bus_Stall = false;
	}
}

