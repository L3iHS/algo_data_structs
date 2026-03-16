#include "records.h"
#include "sort.h"
#include "search.h"

#include <stdio.h>

int main(void)
{
    Records records;
    RecordsStatus status = records_read(&records, "data/sample.txt");

    if (status != REC_OK)
    {
        printf("Ошибка чтения данных\n");
        return 1;
    }

    printf("До sort:\n");
    records_print(&records);

    insertion_sort(&records);

    int index = binary_search(&records, "banana");

    if (index == -1)
    {
        printf("\nКлюч не найден\n");
    }
    else
    {
        printf("\nНайдено: key = %s, data = %s\n",
               records.keys[index],
               records.data[index]);
    }

    printf("\nПосле sort:\n");
    records_print(&records);

    records_free(&records);
    return 0;
}