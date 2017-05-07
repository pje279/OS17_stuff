#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>

#include "../include/sys_prog.h"

// LOOK INTO OPEN, READ, WRITE, CLOSE, FSTAT/STAT, LSEEK
// GOOGLE FOR ENDIANESS HELP

bool bulk_read(const char *input_filename, void *dst, const size_t offset, const size_t dst_size) {

	//Check that the parameters are valid
	if (input_filename == NULL || dst == NULL || offset < 0 || dst_size < 0 || offset > dst_size)
	{
		return false;
	} //End if (input_filename == NULL || dst == NULL || offset < 0 || dst_size < 0 || offset > dst_size)

	//Check if the input file exists
	if (access(input_filename, F_OK) == -1)
	{
		return false;
	} //End if (!access(input_filename, F_OK) == -1)

	//Open the file
	int fd = open(input_filename, O_RDONLY);

	if (fd == -1)
	{
		return false;
	} //End if (fd == -1)

	//Move to the correct position in the file and read the file
	lseek(fd, offset, SEEK_SET);
	read(fd, dst, dst_size);
	close(fd);

	return true;
}

bool bulk_write(const void *src, const char *output_filename, const size_t offset, const size_t src_size) {

	//Check that the parameters are valid
	if (src == NULL || output_filename == NULL || offset < 0 || src_size <= 0)
	{
		return false;
	} //End if (src == NULL || output_filename == NULL || offset < 0 || src_size <= 0)

	//Open the file
	int fd = open(output_filename, O_WRONLY);
	
	if (fd == -1)
	{
		return false;
	} //End if (fd == -1

	//Move to the correct position in the file and write to it
	lseek(fd, offset, SEEK_SET);
	write(fd, src, src_size);
	close(fd);

	return true;
}


bool file_stat(const char *query_filename, struct stat *metadata) {

	//Check that the parameters are valid
	if (query_filename == NULL || metadata == NULL)
	{
		return false;
	} //End if (query_filename == NULL || metadata == NULL)

	//Check if the file exists
	if (access(query_filename, F_OK) == -1)
	{
		return false;
	} //End if (access(query_filename, F_OK) == -1)

	//Get the metadata
	stat(query_filename, metadata);

	return true;
}

bool endianess_converter(uint32_t *src_data, uint32_t *dst_data, const size_t src_count) {

	//Check that the parameters are valid
	if (src_data == NULL || dst_data == NULL || src_count <= 0)
	{
		return false;
	} //End if (src_data == NULL || dst_data == NULL || src_count <= 0)
	
	//Convert the data's endianness
	int i = 0;
	uint32_t num = 0;
	for (i=0; i < src_count; i++)
	{
		num = src_data[i];
		src_data[i] = ((num>>24)&0xff) | ((num<<8)&0xff0000) | ((num>>8)&0xff00) | ((num<<24)&0xff000000);
		dst_data[i] = src_data[i];
	} //End for (i=0; i < src_count; i++)

	return true;
}
