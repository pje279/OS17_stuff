#include "../include/block_store.h"
#include "../include/bitmap.h"
#include <errno.h>
#include <string.h>

// Helpful constants...
#define BITMAP_SIZE_BYTES 256        // 2^8 blocks.
#define BLOCK_STORE_NUM_BLOCKS 256   // 2^8 blocks.
#define BLOCK_STORE_AVAIL_BLOCKS 255 // Last block consumed by the FBM
#define BLOCK_STORE_NUM_BYTES 65536  // 2^8 blocks of 2^8 bytes.
#define BLOCK_SIZE_BITS 2048         // 2^8 BYTES per block
#define BLOCK_SIZE_BYTES 256         // 2^8 BYTES per block

/***************Structs***************/
struct block_store {
    uint8_t **blocks;
    bitmap_t *fbm;
};

/***************Function Definitions***************/
block_store_t *block_store_create()
{
    block_store_t *bs = (block_store_t *)malloc(sizeof(block_store_t));

    if (bs == NULL)
    {
        return NULL;
    } //End if (bs == NULL

    bs->blocks = (uint8_t **)malloc(sizeof(uint8_t *) * BLOCK_STORE_AVAIL_BLOCKS);

    if (bs->blocks == NULL)
    {
        free(bs);
        return NULL;
    } //End if (bs->blocks == NULL)

    bs->fbm = bitmap_create(BLOCK_STORE_AVAIL_BLOCKS);

    if (bs->fbm == NULL)
    {
        free(bs->blocks);
        free(bs);
        return NULL;
    } //End if (bs->fbm == NULL)

    bitmap_format(bs->fbm, 0);

    return bs;
}

/************************************************************/

void block_store_destroy(block_store_t *const bs)
{
    //Check that the parameters are valid
    if (bs != NULL)
    {
        if (bs->blocks != NULL)
        {
            //Check if any blocks have been allocated, and thus need to be freed
            if (bitmap_total_set(bs->fbm) > 0)
            {
                size_t i = 0;
                for (; i < BLOCK_STORE_AVAIL_BLOCKS; i++)
                {
                    if (bs->blocks[i] != NULL)
                    {
                        if (bitmap_test(bs->fbm, i))
                        {
                            block_store_release(bs, i);
                        } //End if (bitmap_test(bs->fbm, i))
                    } //End if (bs->blocks[i] != NULL)
                } //End for (; i < BLOCK_STORE_AVAIL_BLOCKS; i++)
            } //End if (bitmap_total_set(bs->fbm) > 0)
        } //End if (bs->blocks != NULL)

        free(bs->blocks);
        bitmap_destroy(bs->fbm);
        free(bs);
    } //End if (bs != NULL)
}

/************************************************************/

size_t block_store_allocate(block_store_t *const bs)
{
    //Check that the parameters are valid
    if (bs == NULL)
    {
        return SIZE_MAX;
    } //End if (bs == NULL)

    size_t id = SIZE_MAX;

    id = bitmap_ffz(bs->fbm);

    if (id == SIZE_MAX || id > 255)
    {
        return SIZE_MAX;
    } //End if (id == SIZE_MAX)

    bs->blocks[id] = (uint8_t *)malloc(sizeof(uint8_t) * BLOCK_SIZE_BYTES);

    if (bs->blocks[id] == NULL)
    {
        return SIZE_MAX;
    } //End if (bs->blocks[id] == NULL)

    bitmap_set(bs->fbm, id);

    return id;
}

/************************************************************/

bool block_store_request(block_store_t *const bs, const size_t block_id)
{
    //Check that the parameters are valid
    if (bs == NULL || block_id > 255)
    {
        return false;
    } //End if (bs == NULL || block_id > 255)

    if (bitmap_test(bs->fbm, block_id))
    {
        return false;
    } //End if (bitmap_test(bs->fbm, block_id))

    bs->blocks[block_id] = (uint8_t *)malloc(sizeof(uint8_t) * BLOCK_SIZE_BYTES);

    if (bs->blocks[block_id] == NULL)
    {
        return false;
    } //End if (bs->blocks[block_id] == NULL)

    bitmap_set(bs->fbm, block_id);

    return true;
}

/************************************************************/

void block_store_release(block_store_t *const bs, const size_t block_id)
{
    //Check that the parameters are valid
    if (bs != NULL && block_id < 256)
    {
        if (bs->blocks[block_id] != NULL && bitmap_test(bs->fbm, block_id))
        {
            free(bs->blocks[block_id]);
            bs->blocks[block_id] = NULL;
        } //End if (bs->blocks[block_id] != NULL && bitmap_test(bs->fbm, block_id))

        if (bitmap_test(bs->fbm, block_id))
        {
            bitmap_flip(bs->fbm, block_id);
        } //End if (bitmap_test(bs->fbm, block_id))
    } //End if (bs != NULL && block_id < 256)
}

/************************************************************/

size_t block_store_get_used_blocks(const block_store_t *const bs)
{
    //Check that the parameters are valid
    if (bs == NULL)
    {
        return SIZE_MAX;
    } //End if (bs == NULL)

    return bitmap_total_set(bs->fbm);
}

/************************************************************/

size_t block_store_get_free_blocks(const block_store_t *const bs)
{
    //Check that the parameters are valid
    if (bs == NULL)
    {
        return SIZE_MAX;
    } //End if (bs == NULL)

    return 255 - bitmap_total_set(bs->fbm);
}

/************************************************************/

size_t block_store_get_total_blocks()
{
    return BLOCK_STORE_AVAIL_BLOCKS;
}

/************************************************************/

size_t block_store_read(const block_store_t *const bs, const size_t block_id, void *buffer)
{
    //Check that the parameters are valid
    if (bs == NULL || block_id > 255 || buffer == NULL || bs->blocks[block_id] == NULL)
    {
        return 0;
    } //End if (bs == NULL || block_id > 255 || buffer == NULL || bs->blocks[block_id] == NULL)

    memcpy(buffer, bs->blocks[block_id], BLOCK_SIZE_BYTES);

    if (errno != 0)
    {
        return 0;
    } //End if (errno != 0)

    return BLOCK_SIZE_BYTES;
}

/************************************************************/

size_t block_store_write(block_store_t *const bs, const size_t block_id, const void *buffer)
{
    //Check that the parameters are valid
    if (bs == NULL || block_id > 255 || buffer == NULL || bs->blocks[block_id] == NULL)
    {
        return 0;
    } //End if (bs == NULL || block_id > 255 || buffer == NULL || bs->blocks[block_id] == NULL)

    memcpy(bs->blocks[block_id], buffer, BLOCK_SIZE_BYTES);

    if (errno != 0)
    {
        return 0;
    } //End if (errno != 0)

    return BLOCK_SIZE_BYTES;
}

/************************************************************/

block_store_t *block_store_deserialize(const char *const filename)
{
    //Check that the parameters are valid
    if (filename == NULL)
    {
        return NULL;
    } //End if (filename == NULL)

    return NULL;
}

/************************************************************/

size_t block_store_serialize(const block_store_t *const bs, const char *const filename)
{
    //Check that the parameters are valid
    if (bs == NULL || filename == NULL)
    {
        return 0;
    } //End if (bs == NULL || filename == NULL)

    return 0;
}
