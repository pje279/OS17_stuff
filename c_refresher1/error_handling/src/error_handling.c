#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>

#include "../include/error_handling.h"

int create_blank_records(Record_t **records, const size_t num_records) {

    //Check that the parameters are valid
    if ((int)num_records < 0)
    {
        return -2;
    } //End if ((int)num_records < 0)
    else if (*records != NULL || num_records <= 0)
    {
        return -1;
    } //End if (records != NULL)

    *records = (Record_t*) malloc(sizeof(Record_t) * num_records);
    if (*records == NULL)
    {
        return -2;
    } //End if (*records == NULL)

    memset(*records,0,sizeof(Record_t) * num_records);
    return 0;	
}

int read_records(const char *input_filename, Record_t *records, const size_t num_records) {

    //Check that the parameters are valid
    if (input_filename == NULL || records == NULL || num_records <= 0)
    {
        return -1;
    } //End if (inpute_filename == NULL || records == NULL || num_records <= 0) 

    int fd = open(input_filename, O_RDONLY);
    if (fd < 0)
    {
        return -2;
    } //End if (fd < 0)

    //Check that num_records is not larger than the file
    size_t pos = lseek(fd, 0, SEEK_CUR);
    lseek(fd, 0, SEEK_END);
    size_t length = lseek(fd, 0, SEEK_CUR);
    lseek(fd, pos, SEEK_SET);
    if (num_records > length / sizeof(Record_t))
    {
        return -3;
    } //End if (num_records > length / sizeof(Record_T))

    ssize_t data_read = 0;
    for (size_t i = 0; i < num_records; ++i) {
        if (data_read < 0)
        {
            return -3;
        } //End if (data_read < 0)
        data_read = read(fd,&records[i], sizeof(Record_t));
    }
    return 0;
}

int create_record(Record_t **new_record, const char* name, int age) {

    //Check that the parameters are valid
    if (new_record == NULL || *new_record != NULL || name == NULL || strlen(name) > 50 || age < 1 || age > 200 || name[0] == '\n')
    {
        return -1;
    } //End 

    *new_record = (Record_t*) malloc(sizeof(Record_t));
    if (*new_record == NULL)
    {
        return -2;
    } //End 

    memcpy((*new_record)->name,name,sizeof(char) * strlen(name));
    (*new_record)->name[MAX_NAME_LEN - 1] = 0;	
    (*new_record)->age = age;
    return 0;

}
