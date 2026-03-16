#include "records.h"
#include "sort.h"
#include "search.h"

#include <stdio.h>
#include <string.h>

static void run_case(const char *filepath)
{
    Records records;
    printf("\nФайл: %s\n", filepath);
    RecordsStatus status = records_read(&records, filepath);

    if (status != REC_OK)
    {
        printf("Ошибка чтения данных\n");
        return;
    }

    printf("До сортировки:\n");
    records_print(&records);

    insertion_sort(&records);

    printf("\nПосле сортировки:\n");
    records_print(&records);

    char key[256];
    while (1)
    {
        printf("\nВведите ключ для поиска (Enter или exit - завершить): ");

        if (fgets(key, sizeof(key), stdin) == NULL)
        {
            printf("Ошибка ввода\n");
            break;
        }

        size_t length = strlen(key);
        if (length > 0 && key[length - 1] == '\n')
        {
            key[length - 1] = '\0';
        }

        if (key[0] == '\0' || strcmp(key, "exit") == 0)
        {
            break;
        }

        int index = binary_search(&records, key);

        if (index == -1)
        {
            printf("Ключ не найден\n");
        }
        else
        {
            printf("Найдено: key = %s, data = %s\n",
                   records.keys[index],
                   records.data[index]);
        }
    }

    records_free(&records);
}
int main(void)
{
    run_case("data_samples/random_samples.txt");
    run_case("data_samples/sorted_samples.txt");
    run_case("data_samples/reversed_samples.txt");
    return 0;
}