
#include "../include/structures.h"
#include <stdio.h>

int compare_structs(sample_t* a, sample_t* b)
{
    //Check that the parameters are valid
    if (a == NULL || b == NULL)
    {
        return 0;
    } //End if (a == NULL || b == NULL)

    if (a->a != b->a || a->b != b->b || a->c != b->c)
    {
        return 0;
    } //End if (a->a != b->a || a->b != b->b || a->c != b->c)

    return 1;
}

void print_alignments()
{
    printf("Alignment of int is %zu bytes\n",__alignof__(int));
    printf("Alignment of double is %zu bytes\n",__alignof__(double));
    printf("Alignment of float is %zu bytes\n",__alignof__(float));
    printf("Alignment of char is %zu bytes\n",__alignof__(char));
    printf("Alignment of long long is %zu bytes\n",__alignof__(long long));
    printf("Alignment of short is %zu bytes\n",__alignof__(short));
    printf("Alignment of structs are %zu bytes\n",__alignof__(fruit_t));
}

int sort_fruit(const fruit_t* a,int* apples,int* oranges, const size_t size)
{
    //Check that the parameters are valid
    if (a == NULL || apples == NULL || oranges == NULL || size <= 0)
    {
        return -1;
    } //End if (a == NULL || apples == NULL || oranges == NULL || size <= 0)

    //Iterate through the array, checking the type of each fruit
    size_t i = 0;
    for (i=0; i < size; i++)
    {
        if (a[i].type == 'a')
        {
            *apples += 1;
        } //End if (a[i].type == 'a')
        else if (a[i].type == 'o')
        {
            *oranges += 1;
        } //End else if (a[i].type == 'o')
    } //End for (i=0; i < size; i++)

    return size;
}

int initialize_array(fruit_t* a, int apples, int oranges)
{
    //Check that the parameters are valid
    if (a == NULL || apples < 0 || oranges < 0 || (apples == 0 && oranges == 0))
    {
        return -1;
    } //End if (a == NULL || apples < 0 || oranges < 0 || (apples == 0 && oranges == 0))

    //Inialize the apples, then the oranges
    int i = 0;
    for (i=0; i < apples; i++)
    {
        a[i].type = 'a';
    } //End for (i=0; i < apples; i++)
    for (i=apples; i < apples + oranges; i++)
    {
        a[i].type = 'o';
    } //End for (i=apples; i < apples + oranges; i++)

    return 0;
}

int initialize_orange(orange_t* a)
{
    //Check that the parameters are valid
    if (a == NULL)
    {
        return -1;
    } //End if (a == NULL)

    a->type = 'o';
    a->weight = 0;
    a->peeled = 0;

    return 0;
}

int initialize_apple(apple_t* a)
{
    //Check that the parameters are valid
    if (a == NULL)
    {
        return -1;
    } //End if (a == NULL)

    a->type = 'a';
    a->weight = 0;
    a->worms = 0;
    return 0;
}
