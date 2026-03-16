#include "search.h"

#include <string.h>

int binary_search(const Records *records, const char *key)
{
    int left = 0;
    int right = records->count - 1;

    while (left <= right)
    {
        int mid = left + (right - left) / 2;

        int cmp = strcmp(records->keys[mid], key);

        if (cmp == 0)
        {
            return mid;
        }
        else if (cmp > 0)
        {
            right = mid - 1;
        }
        else
        {
            left = mid + 1;
        }
    }
    return -1;
}