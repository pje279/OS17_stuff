#include <string.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>

#include "../include/sstring.h"

bool string_valid(const char *str, const size_t length) {

	//Check that the parameters are valid
	if (str == NULL || length <= 0)
	{
		return false;
	} //End if (str == NULL || length <= 0)

	int i = 0;
	for (i=0; i < (int)length; i++)
	{
		if (str[i] == '\0')
		{
			return true;
		} //End if (str[i] == '\0')
	} //End for (i=0; i < (int)length; i++)

	return false;
}

char *string_duplicate(const char *str, const size_t length) {

	//Check that the parameters are valid
	if (str == NULL || length <= 0)
	{
		return NULL;
	} //End if (str == NULL || length <= 0)

	//Create a new string to copy the first string to
	char *new_str = (char *)malloc(length);
	if (new_str == NULL)
	{
		return NULL;
	} //End if (new_str == NULL)

	//Initalize the new string
	memset(new_str, '\0', length);

	//Copy the string
	strncpy(new_str, str, length);

	return new_str;
}

bool string_equal(const char *str_a, const char *str_b, const size_t length) {

	//Check that the parameters are valid
	if (str_a == NULL || str_b == NULL || length <= 0)
	{
		return false;
	} //End if (str_a == NULL || str_b == NULL || length <= 0)

	if (strncmp(str_a, str_b, length) == 0)
	{
		return true;
	} //End if (strncmp(str_a, str_b, length) == 0)
	else
	{
		return false;
	} //End else
}

int string_length(const char *str, const size_t length) {

	//Check that the parameters are valid
	if (str == NULL || length <= 0)
	{
		return -1;
	} //End if (str == NULL || length <= 0)

	return strlen(str);
}

int string_tokenize(const char *str, const char *delims, const size_t str_length,char **tokens, const size_t max_token_length, const size_t requested_tokens) {

	//Check that the parameters are valid
	if (str == NULL || delims == NULL || str_length <= 0 || tokens == NULL || max_token_length <= 0 || requested_tokens <= 0)
	{
		return 0;
	} //End if (str == NULL || delims == NULL || str_length <= 0 || tokens == NULL || max_token_length <= 0 || requested_tokens <= 0)

	//Tokenize the string
	int num_tokens = 0;
	char *token = NULL;
	char *temp_str = string_duplicate(str, str_length);
	
	token = strtok(temp_str, delims);

	while (token != NULL)
	{
		if (num_tokens < requested_tokens && strlen(token) < max_token_length)
		{
			if (tokens[num_tokens] != NULL)
			{
				strcpy(tokens[num_tokens], token);
			} //End if (tokens[num_tokens] != NULL)
			else
			{
				free(temp_str);
				return -1;
			} //End else

			num_tokens += 1;
		} //End if (num_tokens < requested_tokens && strlen(token) < max_token_length)
		else
		{
			free(temp_str);
			return num_tokens;
		} //End else

		token = strtok(NULL, delims);
	} //End while (token != NULL)

	free(temp_str);
	return num_tokens;
}

bool string_to_int(const char *str, int *converted_value) {

	//Check that the parameters are valid
	if (str == NULL || converted_value == NULL)
	{
		return false;
	} //End if (str == NULL || converted_value == NULL)

	//int num = atoi(str)

	//printf("\n%d : %d\n", atoi(str), *converted_value);

	//if (atoi(str) != *converted_value)
	if (atoi(str) <= 0)
	{
		return false;
	} //End if (num == 0 || num != *converted_value)
	else
	{
		*converted_value = atoi(str);
		return true;
	} //End else
}
