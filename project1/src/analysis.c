#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../include/dyn_array.h"
#include "../include/processing_scheduling.h"

#define FCFS "FCFS"
#define P "P"
#define RR "RR"

int main(int argc, char **argv) {
    if (argc < 3) {
        printf("%s <pcb file> <schedule algorithm> [quantum]\n", argv[0]);
        return EXIT_FAILURE;
    }

    //Declare variables
    dyn_array_t *da = load_process_control_blocks(argv[1]);
    if (da == NULL)
    {
        return EXIT_FAILURE;
    } //End if (da == NULL) 

    ScheduleResult_t *sr = (ScheduleResult_t *) malloc(sizeof(ScheduleResult_t));
    if (sr == NULL)
    {
        dyn_array_destroy(da);
        return EXIT_FAILURE;
    } //End if (sr == NULL)

    if (!strcmp(argv[2], FCFS))
    {
        if (!first_come_first_serve(da, sr))
        {
            printf("\nFCFS didn't run properly\n\n");
            dyn_array_destroy(da);
            free(sr);
            return EXIT_FAILURE;
        } //End if (!first_come_first_serve(da, sr))
    } //End if (!strcmp(argv[2], FCFS))
    else if (!strcmp(argv[2], P))
    {
        if (!priority(da, sr))
        {
            printf("\nFCFS didn't run properly\n\n");
            dyn_array_destroy(da);
            free(sr);
            return EXIT_FAILURE;
        } //End if (!priority(da, sr))
    } //End else if (!strcmp(argv[2], P))
    else if (!strcmp(argv[2], RR))
    {
        //Round robin
        dyn_array_destroy(da);
        free(sr);

        return EXIT_FAILURE;
    } //End else if (!strcmp(argv[2], RR))
    else
    {
        //Bad input for schedule algorithm
        dyn_array_destroy(da);
        free(sr);

        printf("\nBad schedul algorithm imput.\n\n");

        return EXIT_FAILURE;
    } //End else

    //Write the statistics to raeadme.md
    FILE *fp = fopen("../readme.md", "a");
    if (fp == NULL)
    {
        printf("\nERROR: readme.md file didn't open.\n\n");
        dyn_array_destroy(da);
        free(sr);
        return EXIT_FAILURE;
    } //End if (fp == NULL)

    fprintf(fp, "\n%s on file: %s Time Quantum: %s\nAverage Latency Time: %f\nAverage Wall Clock Time: %f\nTotal Run Time: %lu\n", argv[2], argv[1], argv[3], sr->average_latency_time, sr->average_wall_clock_time, sr->total_run_time);

    fclose(fp);
    dyn_array_destroy(da);
    free(sr);

    return EXIT_SUCCESS;
}
