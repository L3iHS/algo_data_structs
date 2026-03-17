#include "sort.h"

#include <string.h>

void insertion_sort(Records *records)
/* Сортирует таблицу методом простой вставки */
{
    for (int i = 1; i < records->count; i++)
    {
        char *current_key = records->keys[i];
        char *current_data = records->data[i];

        int j = i - 1;
        while (j >= 0 && strcmp(records->keys[j], current_key) > 0)
        {
            records->keys[j + 1] = records->keys[j];
            records->data[j + 1] = records->data[j];
            j--;
        }

        records->keys[j + 1] = current_key;
        records->data[j + 1] = current_data;
    }
}