
#include "Bus.h"





void removeData(S_Multi_Core_Env* p_Cores_Env)
{
    int ii;
    Queue_Bus Q_temp;

    for (ii = 0; ii < p_Cores_Env->Queue_Bus.Num_Of_Queue; ii++)
    {
        Q_temp.MSI_Bus[ii] = p_Cores_Env->Queue_Bus.MSI_Bus[ii + 1];
    }

    p_Cores_Env->Queue_Bus.Num_Of_Queue = p_Cores_Env->Queue_Bus.Num_Of_Queue - 1; // update num of buses
    p_Cores_Env->Queue_Bus.Next_Free_Slot = p_Cores_Env->Queue_Bus.Next_Free_Slot - 1; // update the next free slot
    for (ii = 0; ii < p_Cores_Env->Queue_Bus.Num_Of_Queue; ii++)
    {
        p_Cores_Env->Queue_Bus.MSI_Bus[ii] = Q_temp.MSI_Bus[ii];
    }

}

S_MSI_Bus Push(S_Multi_Core_Env* p_Cores_Env)
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

int size(S_Multi_Core_Env* p_Cores_Env)
{
    return p_Cores_Env->Queue_Bus.Num_Of_Queue;
}

void insert(S_Multi_Core_Env* p_Cores_Env, S_MSI_Bus Bus)
{

    if (!isFull(p_Cores_Env)) {

        p_Cores_Env->Queue_Bus.MSI_Bus[p_Cores_Env->Queue_Bus.Next_Free_Slot] = Bus;
        p_Cores_Env->Queue_Bus.Next_Free_Slot++;
        p_Cores_Env->Queue_Bus.Num_Of_Queue++;
    }
}

