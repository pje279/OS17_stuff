#include "../include/allocation.h"
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>

void* allocate_array(size_t member_size, size_t nmember,bool clear)
{
    //Check that the paramaters are valid
    if (member_size <= 0 || nmember <= 0)
    {
        return NULL;
    } //End if (member_size <= 0 || nmember <= 0)

    void *array = NULL;

    //Check the clear flag
    if (clear)
    {
        array = (void *)calloc(nmember, member_size);
    } //End if (clear)
    else
    {
        array = (void *)malloc(member_size * nmember);
    } //End else

    return array;
}

void* reallocate_array(void* ptr, size_t size)
{
    //Check that the parameters are valid
    if (ptr != NULL  && size <= 0)
    {
        free(ptr);
        return NULL;
    } //End if (size <= 0)

    //Reallocate the pointer
    return (void *)realloc(ptr, size);
}

void deallocate_array(void** ptr)
{
    //Check that the parameters are valid
    if (ptr == NULL || *ptr == NULL)
    {
        return;
    } //End if (ptr == NULL || *ptr == NULL)

    free(*ptr);
    *ptr = NULL;
    return;
}

char* read_line_to_buffer(char* filename)
{
    //Check that the parameters are valid
    if (filename == NULL || access(filename, F_OK) == -1)
    {
        return NULL;
    } //End if (filename == NULL || access(filename, F_OK) == -1)

    //Open the file
    FILE *fp = fopen(filename, "r");
    if (fp == NULL)
    {
        return NULL;
    } //End if (fp == NULL)

    //Declare variables
    char ch;
    int CUR_MAX = 1024;
    int length = 0;
    char *buffer = (char *)malloc(sizeof(char) * CUR_MAX);
    if (buffer == NULL)
    {
        fclose(fp);
        return NULL;
    } //End if (buffer == NULL)

    //Read the line one character at a time, reallocating when needing more space
    while (ch != '\n' && ch != EOF)
    {
        if (length == CUR_MAX)
        {
            CUR_MAX *= 2;
            buffer = realloc(buffer, CUR_MAX);
        } //End if (length == CUR_MAX) 

        //Get the next character, validate it, then add it to the buffer
        ch = fgetc(fp);
        if (ch == '\n' || ch == EOF)
        {
            break;
        } //End if (ch == '\n' || ch == EOF)

        buffer[length] = ch;
        length++;
    } //End while (ch != '\n' && ch != EOF)

    fclose(fp);

    return buffer;
}
