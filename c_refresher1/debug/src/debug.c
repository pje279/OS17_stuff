#include "../include/debug.h"

#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <stdio.h>

// protected function, that only this .c can use
int comparator_func(const void *a, const void *b) {
    return *(uint16_t *)a -  *(uint16_t *)b;
}

bool terrible_sort(uint16_t *data_array, const size_t value_count) {
    //Check that the parameters are valid
    if (data_array == NULL || value_count <= 0)
    {
        return false;
    } //End if (data_array == NULL || value_count <= 0) 

    uint16_t *sorting_array = (uint16_t *)malloc(value_count * sizeof(uint16_t));
    for (uint16_t i = 0; i < value_count; ++i) {
        sorting_array[i] = data_array[i];
    } //End for (uint16_t i = 0; i < value_count; ++i) {

    qsort(sorting_array, value_count, /*sizeof(sorting_array) / */ sizeof(uint16_t), comparator_func);

    bool sorted = true;
    for (uint16_t i = 0; i < value_count-1; ++i) {
        sorted &= sorting_array[i] <= sorting_array[i + 1];
    } //End for (uint16_t i = 0; i < value_count-1; ++i) {

    if (sorted) 
    {
        memcpy(data_array, sorting_array, value_count * sizeof(uint16_t));
    } //End if (sorted)

    free(sorting_array);

    return sorted;
}

