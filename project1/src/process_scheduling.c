#include <fcntl.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

#include "../include/dyn_array.h"
#include "../include/processing_scheduling.h"

#define QUANTUM 4 // Used for Robin Round for process as the run time limit

/**************************************************/

// private function
void virtual_cpu(ProcessControlBlock_t *process_control_block)
{
    // decrement the burst time of the pcb
    --process_control_block->remaining_burst_time;
}

/**************************************************/

bool first_come_first_serve(dyn_array_t *ready_queue, ScheduleResult_t *result)
{
    //Check that the parameters are valid
    if (ready_queue == NULL || result == NULL || dyn_array_size(ready_queue) <= 0)
    {
        return false;
    } //End if (ready_queue == NULL || result == NULL || dyn_array_size(ready_queue) <= 0)

    //Declare variables
    uint32_t clock = 0;
    float avg_finish = 0;
    float avg_latency = 0;
    ProcessControlBlock_t *curPCB = NULL;
    dyn_array_t *da_holding = dyn_array_create(0, sizeof(ProcessControlBlock_t), NULL);

    if (da_holding == NULL)
    {
        printf("\nSupplemental dynamic array failed to create. Out of memory?\n\n");
        return NULL;
    } //End if (da_holding == NULL)

    while (dyn_array_back(ready_queue) != NULL)
    {
        if (dyn_array_size(ready_queue) <= 0 || clock > 99999)
        {
            break;
        } //End if (dyn_array_size(ready_queue) <= 0 || clock > 99999)

        curPCB = dyn_array_back(ready_queue);
        if (curPCB == NULL)
        {
            printf("\nready_queue returned NULL, but while loop check didn't end on NULL\n\n");
            break;
        } //End if (curPCB == NULL)

        curPCB->started = true;
        curPCB->wait_time = clock;
        while (curPCB->remaining_burst_time > 0)
        {
            virtual_cpu(curPCB);
            clock++;
        } //End while (curPCB->PCB->remaining_burst_time > 0)

        curPCB->finish_time = clock;
        dyn_array_pop_back(ready_queue);
        dyn_array_push_front(da_holding, curPCB);
    } //End while (ready_queue != NULL)

    //Calculate the statistics of the algorithm
    size_t i = 0;
    for (i=0; i<dyn_array_size(da_holding); i++)
    {
        curPCB = dyn_array_at(da_holding, i);
        if (da_holding == NULL)
        {

        } //End if (da_holding == NULL)
        avg_finish += curPCB->finish_time;
        avg_latency += curPCB->wait_time;
    } //End for (i=0; i<dyn_array_size(da_holding); i++)

    result->average_latency_time = (avg_latency / dyn_array_size(da_holding));
    result->average_wall_clock_time = (avg_finish / dyn_array_size(da_holding));
    result->total_run_time = clock;

    dyn_array_destroy(da_holding);

    return true;
}

/**************************************************/

int comparePCBs(const void *pcbA, const void *pcbB)
{
    if (pcbA == NULL || pcbB == NULL)
    {
        printf("\nERROR: comparePCBs - NULL parameter passed.");
        return 0;
    } //End if (pcbA == NULL || pcbB == NULL)

    if (((ProcessControlBlock_t *)pcbA)->priority < ((ProcessControlBlock_t *)pcbB)->priority)
    {
        return 1;
    } //End if ((ProcessControlBlock_t *)pcbA->priority < (ProcessControlBlock_t *)pcbB)
    else if (((ProcessControlBlock_t *)pcbA)->priority > ((ProcessControlBlock_t *)pcbB)->priority)
    {
        return -1;
    } //End else if ((ProcessControlBlock_t *)pcbA->priority > (ProcessControlBlock_t *)pcbB)
    else
    {
        return 0;
    } //End else

} //End int comparePCBs(const void *pcbA, const void *pcbB)

/**************************************************/

bool priority(dyn_array_t *ready_queue, ScheduleResult_t *result)
{
    //Check that the parameters are valid
    if (ready_queue == NULL || result == NULL || dyn_array_size(ready_queue) <= 0)
    {
        //printf("\nBad params\n\n");
        return false;
    } //End if (ready_queue == NULL || result == NULL || dyn_array_size(ready_queue) <= 0)

    if (!dyn_array_sort(ready_queue, comparePCBs))
    {
        printf("\nSort failed\n\n");
        return false;
    } //End 

    //Declare variables
    uint32_t clock = 0;
    float avg_finish = 0;
    float avg_latency = 0;
    ProcessControlBlock_t *curPCB = NULL;
    dyn_array_t *da_holding = dyn_array_create(0, sizeof(ProcessControlBlock_t), NULL);

    if (da_holding == NULL)
    {
        printf("\nSupplemental dynamic array failed to create. Out of memory?\n\n");
        return NULL;
    } //End if (da_holding == NULL)

    while (dyn_array_front(ready_queue) != NULL)
    {
        if (dyn_array_size(ready_queue) <= 0 || clock > 99999)
        {
            break;
        } //End if (dyn_array_size(ready_queue) <= 0 || clock > 99999)

        curPCB = dyn_array_back(ready_queue);
        if (curPCB == NULL)
        {
            printf("\nready_queue returned NULL, but while loop check didn't end on NULL\n\n");
            break;
        } //End if (curPCB == NULL)

        curPCB->started = true;
        curPCB->wait_time = clock;

        while (curPCB->remaining_burst_time > 0)
        {
            virtual_cpu(curPCB);
            clock++;
        } //End while (curPCB->PCB->remaining_burst_time > 0)

        curPCB->finish_time = clock;
        dyn_array_pop_back(ready_queue);
        dyn_array_push_front(da_holding, curPCB);
    } //End while (ready_queue != NULL)

    //Calculate the statistics of the algorithm
    size_t i = 0;
    for (i=0; i<dyn_array_size(da_holding); i++)
    {
        curPCB = dyn_array_at(da_holding, i);
        if (da_holding == NULL)
        {

        } //End if (da_holding == NULL)
        avg_finish += curPCB->finish_time;
        avg_latency += curPCB->wait_time;
    } //End for (i=0; i<dyn_array_size(da_holding); i++)

    result->average_latency_time = (avg_latency / dyn_array_size(da_holding));
    result->average_wall_clock_time = (avg_finish / dyn_array_size(da_holding));
    result->total_run_time = clock;


    dyn_array_destroy(da_holding);

    return true;
} //End bool priority(dyn_array_t *ready_queue, ScheduleResult_t *result)

/**************************************************/

#ifdef GRAD_TESTS
bool round_robin(dyn_array_t *ready_queue, ScheduleResult_t *result, size_t quantum)
{
    //Check that the parameters are valid

    return false;
}
#endif

/**************************************************/

dyn_array_t *load_process_control_blocks(const char *input_file)
{
    //Check that the parameters are valid
    if (input_file == NULL || access(input_file, F_OK) == -1)
    {
        return NULL;
    } //End if (input_file == NULL)

    //Open the file
    int fd = open(input_file, O_RDONLY);
    if (fd == -1)
    {
        return NULL;
    } //End if (fd == -1)

    //Check if the file is empty
    off_t pos = lseek(fd, 0, SEEK_END);

    if (pos == 0)
    {
        close(fd);
        return NULL;
    } //End if (pos == 0)

    //Reset the position
    lseek(fd, 0, SEEK_SET);

    //Get the number of processes
    uint32_t numProcs = 0;
    read(fd, &numProcs, sizeof(uint32_t));

    //Check if the number of processes is valid (ie. multiple of 2)
    if (numProcs % 2 != 0)
    {
        close(fd);
        return NULL;
    } //End if (numProcs % 2 != 0)

    //Read the PCBS and put them into the dyn_array
    uint32_t burst = 0;
    uint32_t priority = 0;
    uint32_t numProcsRead = 0;
    size_t i = 0;
    //ProcessControlBlock_t *pcb = NULL;
    ProcessControlBlock_t pcb;
    dyn_array_t *da = dyn_array_create(0, sizeof(ProcessControlBlock_t), NULL);

    if (da == NULL)
    {
        close(fd);
        return NULL;
    } //End if (da == NULL)

    for (i=0; i<numProcs; i++)
    {
        if (read(fd, &burst, sizeof(uint32_t)) <= 0 || read(fd, &priority, sizeof(uint32_t)) <= 0)
        {
            break;
        } //End  if (read(fd, &burst, sizeof(uint32_t)) <= 0 || read(fd, &priority, sizeof(uint32_t)) <= 0)

        numProcsRead++;

        pcb.remaining_burst_time = burst;
        pcb.priority = priority;
        pcb.started = false;
        pcb.wait_time = 0;
        pcb.finish_time = 0;

        if (!dyn_array_push_back(da, &pcb))
        {
            dyn_array_destroy(da);
            close(fd);
            return NULL;
        } //End if (!dyn_array_push_back(da, pcb))
    } //End for (i=0; i<numProcs; i++)

    close(fd);

    //Check if the correct amount of files were read
    if (numProcsRead != numProcs)
    {
        dyn_array_destroy(da);
        return NULL;
    } //End if (numProcsRead != numProcs)

    //Check that the dyn_array is not empty
    if (dyn_array_size(da) <= 0)
    {
        dyn_array_destroy(da);
        return NULL;
    } //End if (dyn_array_size(da) <= 0)

    return da;
} //End dyn_array_t *load_process_control_blocks(const char *input_file)
