#include "records.h"
#include <stdio.h>

int main()
{
    Records records;
    RecordsStatus status = records_read(&records, "data/sample.txt");

    if (status != REC_OK)
    {
        printf("Ошибка чтения данных\n");
        return 1;
    }

    records_print(&records);
    records_free(&records);
    return 0;
}