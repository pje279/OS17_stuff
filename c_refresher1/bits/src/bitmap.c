#include "../include/bitmap.h"
#include <math.h>

// data is an array of uint8_t and needs to be allocated in bitmap_create
//      and used in the remaining bitmap functions. You will use data for any bit operations and bit logic
// bit_count the number of requested bits, set in bitmap_create from n_bits
// byte_count the total number of bytes the data contains, set in bitmap_create


bitmap_t *bitmap_create(size_t n_bits) {

	//Make sure the parameters are valid
	if (n_bits <= 0)
	{
		return NULL;
	} //End if (n_bits <= 0)

	//Create a new bitmap
	bitmap_t *bitmap = (bitmap_t *)malloc(sizeof(bitmap_t *));
	if (bitmap == NULL)
	{
		return NULL;
	} //End if (bitmap == NULL)

	int num_bytes = ceil((double)n_bits/8);

	bitmap->data = (uint8_t*)malloc(sizeof(uint8_t)*num_bytes);
	if (bitmap->data == NULL)
	{
		return NULL;
	} //End if (bitmap->data == NULL)

	//Initialize the data to 0
	int i = 0;
	for (i=0; i<num_bytes; i++)
	{
		bitmap->data[i] = 0;
	} //End for (i=0; i<num_bytes; i++)

	bitmap->bit_count = n_bits;
	bitmap->byte_count = num_bytes;

	return bitmap;
}

bool bitmap_set(bitmap_t *const bitmap, const size_t bit) {

	//Make sure the parameters are valid
	if (bitmap == NULL || bit < 0 || bit >= bitmap->bit_count)
	{
		return false;
	} //End if (bitmap == NULL || bit < 0 || bit >= bitmap->bit_count)

	int i = bit/8;
	int pos = bit%8;
	unsigned int flag = 1;

	flag = flag << pos;

	//Set the bit
	bitmap->data[i] |= flag;

	return true;
}

bool bitmap_reset(bitmap_t *const bitmap, const size_t bit) {

	//Make sure the parameters are valid
	if (bitmap == NULL || bit < 0 || bit >= bitmap->bit_count)
	{
		return false;
	} //End if (bitmap == NULL || bit < 0 || bit >= bitmap->bit_count)

	int i = bit/8;
	int pos = bit%8;
	unsigned int flag = 1;

	flag = flag << pos;
	flag = ~flag;

	//Reset the bit
	bitmap->data[i] &= flag;

	return true;
}

bool bitmap_test(const bitmap_t *const bitmap, const size_t bit) {

	//Make sure the parameters are valid
	if (bitmap == NULL || bit < 0 || bit >= bitmap->bit_count)
	{
		return false;
	} //End if (bitmap == NULL || bit < 0 || bit >= bitmap->bit_count)

	int i = bit/8;
	int pos = bit%8;
	unsigned int flag = 1;

	flag = flag << pos;

	if (bitmap->data[i] & flag)
	{
		return true;
	} //End if (bitmap->data[i] & flag)
	else
	{
		return false;
	} //End else	


}

size_t bitmap_ffs(const bitmap_t *const bitmap) {

	//Make sure the parameters are valid
	if (bitmap == NULL)
	{
		return SIZE_MAX;
	} //End 

	int i = 0;
	for (i=0; i < bitmap->bit_count; i++)
	{
		if (bitmap_test(bitmap, i))
		{
			return i;
		} //End if (bitmap_test(bitmap, i))
	} //End for (i=0; i < bitmap->bit_count; i++)

	return SIZE_MAX;
}

size_t bitmap_ffz(const bitmap_t *const bitmap) {

	//Make sure the parameters are valid
	if (bitmap == NULL)
	{
		return SIZE_MAX;
	} //End 

	int i = 0;
	for (i=0; i < bitmap->bit_count; i++)
	{
		if (!bitmap_test(bitmap, i))
		{
			return i;
		} //End if (!bitmap_test(bitmap, i))
	} //End for (i=0; i < bitmap->bit_count; i++)

	return SIZE_MAX;
}

bool bitmap_destroy(bitmap_t *bitmap) {

	//Make sure the parameters are valid
	if (bitmap == NULL)
	{
		return false;
	} //End if (bitmapp == NULL)

	bitmap->data = NULL;
	free(bitmap);

	return true;
}
