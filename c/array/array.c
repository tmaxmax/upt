#include <stdio.h>
#include "array.h"

void print_array(int *arr, size_t len)
{
    for (size_t i = 0; i < len; i++)
    {
        printf("%d ", arr[i]);
    }

    printf("\n");
}

array_t(int, 10) create_nums()
{
    typeof(create_nums()) nums = {{1, 2, 3, 4, 5, 6, 7, 8, 9, 10}};
    return nums;
}

int main(void)
{
    array_t(int, 10) nums;
    printf("size: %lu\n", array_size(nums));
    printf("len: %lu\n", array_len(nums));
    print_array(array_decay(nums));

    return 0;
}