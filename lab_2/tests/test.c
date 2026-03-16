#include <assert.h>
#include <stdio.h>
#include <string.h>

#include "../src/records.h"
#include "../src/search.h"
#include "../src/sort.h"

static const char *TEST_FILE = "tests/test_data.txt";

static Records load_records(void)
/* Загружает тестовые данные из файла */
{
    Records records;
    RecordsStatus status = records_read(&records, TEST_FILE);

    assert(status == REC_OK);
    return records;
}

static void test_records_read(void)
/* Проверяет корректное чтение записей из файла */
{
    Records records = load_records();

    assert(records.count == 3);
    assert(strcmp(records.keys[0], "key_1") == 0);
    assert(strcmp(records.data[0], "value_1") == 0);
    assert(strcmp(records.keys[1], "key_2") == 0);
    assert(strcmp(records.data[1], "value_2") == 0);
    assert(strcmp(records.keys[2], "key_3") == 0);
    assert(strcmp(records.data[2], "value_3") == 0);

    records_free(&records);
}

static void test_insertion_sort(void)
/* Проверяет корректность сортировки вставками */
{
    Records records = load_records();

    insertion_sort(&records);

    assert(strcmp(records.keys[0], "key_1") == 0);
    assert(strcmp(records.data[0], "value_1") == 0);
    assert(strcmp(records.keys[1], "key_2") == 0);
    assert(strcmp(records.data[1], "value_2") == 0);
    assert(strcmp(records.keys[2], "key_3") == 0);
    assert(strcmp(records.data[2], "value_3") == 0);

    records_free(&records);
}

static void test_binary_search_found(void)
/* Проверяет поиск существующего ключа */
{
    Records records = load_records();

    insertion_sort(&records);

    int index = binary_search(&records, "key_2");

    assert(index != -1);
    assert(strcmp(records.keys[index], "key_2") == 0);
    assert(strcmp(records.data[index], "value_2") == 0);

    records_free(&records);
}

static void test_binary_search_not_found(void)
/* Проверяет поиск отсутствующего ключа */
{
    Records records = load_records();

    insertion_sort(&records);

    int index = binary_search(&records, "key_10");

    assert(index == -1);

    records_free(&records);
}

int main(void)
{
    test_records_read();
    test_insertion_sort();
    test_binary_search_found();
    test_binary_search_not_found();

    printf("All tests passed\n");
    return 0;
}
