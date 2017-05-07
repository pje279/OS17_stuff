#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#include "../include/arrays.h"

// LOOK INTO MEMCPY, MEMCMP, FREAD, and FWRITE

bool array_copy(const void *src, void *dst, const size_t elem_size, const size_t elem_count) {
  
	//Check that the parameters are valid
	if (src == NULL || dst == NULL || elem_size <= 0 || elem_count <= 0) 
	{
		return false;
	} //End if (src == NULL || dst == NULL || elem_size <= 0 || elem_count <= 0) 

	//Copy the contents
	if (memcpy(dst, src, elem_size*elem_count))
	{
		return true;
	} //End if (memcpy(dst, src, elem_size*elem_count))
	else
	{	
		return false;
	} //End else
}

bool array_is_equal(const void *data_one, void *data_two, const size_t elem_size, const size_t elem_count) {

	//Check that the paramaters are valid
	if (data_one == NULL || data_two == NULL || elem_size <= 0 || elem_count <= 0)
	{
		return false;
	} //End if (data_one == NULL || data_two == NULL || elem_size <= 0 || elem_count <= 0)

	//Compare the arrays
	if (memcmp(data_one, data_two, elem_size*elem_count) == 0)
	{
		return true;
	} //End if (memcmp(data_one, data_two, elem_size*elem_count) == 0)
	else
	{
		return false;
	} //End else
}

ssize_t array_locate(const void *data, const void *target, const size_t elem_size, const size_t elem_count) {

	//Check that the paramaters are valid
	if (data == NULL || target == NULL || elem_size <= 0 || elem_count <= 0)
	{
		return -1;
	} //End if (data == NULL || target == NULL || elem_size <= 0 || elem_count <= 0)

	//Search for the target
	size_t i = 0;
	for (i=0; (int)i < elem_count; i++)
	{
		if ((int)(((int *)data)[i]) == (int)(*(size_t *)target))
		{
			return i;
		} //End if ((int)(((int *)data)[i]) == (int)(*(size_t *)target))
	} //End for (i=0; i < elem_count; i++)

	return -1;
}

bool array_serialize(const void *src_data, const char *dst_file, const size_t elem_size, const size_t elem_count) {

	//Check that the parameters are valid
	if (src_data == NULL || dst_file == NULL || elem_size <= 0 || elem_count <= 0)
	{
		return false;
	} //End if (src_data == NULL || dst_file == NULL || elem_size <= 0 || elem_count <= 0)

	//Check for escape characters in the file name
	char bad_chars[] = "\a\b\f\n\r\t\v\\\'\"\?";
	int i = 0;
	for (i=0; i < strlen(bad_chars); i++)
	{
		if (strchr(dst_file, bad_chars[i]) != NULL)
		{
			return false;
		} //End if (strchr(dst_file, bad_chars[i]) != NULL
	} //End for (i=0; i < strlen(bad_chars); i++)

	//Open and write to the file
	FILE *fp;
	fp = fopen((const char *)dst_file, "w");
	if (fp == NULL)
	{
		return false;
	} //End if (fp == NULL)

	fwrite(src_data, elem_size, elem_count, fp);
	fclose(fp);

	return true;
}

bool array_deserialize(const char *src_file, void *dst_data, const size_t elem_size, const size_t elem_count) {

	//Check that the parameters are valid
	if (src_file == NULL || dst_data == NULL || elem_size <= 0 || elem_count <= 0)
	{
		return false;
	} //End if (src_file == NULL || dst_data == NULL || elem_size <= 0 || elem_count <= 0)

	//Check for escape characters in the file name
	char bad_chars[] = "\a\b\f\n\r\t\v\\\'\"\?";
	
	int i = 0;
	for (i=0; i < strlen(bad_chars); i++)
	{
		if (strchr(src_file, bad_chars[i]) != NULL)
		{
			return false;
		} //End if (strchr(src_file, bad_chars[i]) != NULL)
	} //End for (i=0; i < strlen(bad_chars); i++)

	//Open the file and read its contents to the array dst_data
	FILE *fp;
	fp = fopen((const char *)src_file, "r");
	if (fp == NULL)
	{
		return false;
	} //End if (fp == NULL)

	fread(dst_data, elem_size, elem_count, fp);
	fclose(fp);

	return true;
}
