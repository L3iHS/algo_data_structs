#include "records.h"
#include "sort.h"

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

    printf("\nПосле sort:\n");
    records_print(&records);

    records_free(&records);
    return 0;
}