#include <assert.h>
#include <stdio.h>
#include <string.h>

#include "../src/records.h"
#include "../src/search.h"
#include "../src/sort.h"

static Records load_records(void)
/* Загружает тестовые данные из файла */
{
    Records records;
    RecordsStatus status = records_read(&records, "tests/test_data.txt");

    assert(status == REC_OK);
    return records;
}

static void swap_pairs(Records *records, int first, int second)
/* Меняет местами пару key/data по двум индексам */
{
    char *temp_key = records->keys[first];
    char *temp_data = records->data[first];

    records->keys[first] = records->keys[second];
    records->data[first] = records->data[second];

    records->keys[second] = temp_key;
    records->data[second] = temp_data;
}

static void test_records_read(void)
/* Проверяет корректное чтение записей из файла */
{
    Records records = load_records();

    assert(records.count == 5);
    assert(strcmp(records.keys[0], "key_1") == 0);
    assert(strcmp(records.data[0], "value_1") == 0);
    assert(strcmp(records.keys[1], "key_2") == 0);
    assert(strcmp(records.data[1], "value_2") == 0);
    assert(strcmp(records.keys[2], "key_3") == 0);
    assert(strcmp(records.data[2], "value_3") == 0);
    assert(strcmp(records.keys[3], "key_4") == 0);
    assert(strcmp(records.data[3], "value_4") == 0);
    assert(strcmp(records.keys[4], "key_5") == 0);
    assert(strcmp(records.data[4], "value_5") == 0);

    records_free(&records);
}

static void test_insertion_sort_order(void)
/* Проверяет порядок после сортировки вставками */
{
    Records records = load_records();

    insertion_sort(&records);

    assert(strcmp(records.keys[0], "key_1") == 0);
    assert(strcmp(records.keys[1], "key_2") == 0);
    assert(strcmp(records.keys[2], "key_3") == 0);
    assert(strcmp(records.keys[3], "key_4") == 0);
    assert(strcmp(records.keys[4], "key_5") == 0);

    records_free(&records);
}

static void test_insertion_sort_preserves_pairs(void)
/* Проверяет, что данные остались связаны со своими ключами */
{
    Records records = load_records();

    insertion_sort(&records);

    assert(strcmp(records.keys[0], "key_1") == 0);
    assert(strcmp(records.data[0], "value_1") == 0);
    assert(strcmp(records.keys[1], "key_2") == 0);
    assert(strcmp(records.data[1], "value_2") == 0);
    assert(strcmp(records.keys[2], "key_3") == 0);
    assert(strcmp(records.data[2], "value_3") == 0);
    assert(strcmp(records.keys[3], "key_4") == 0);
    assert(strcmp(records.data[3], "value_4") == 0);
    assert(strcmp(records.keys[4], "key_5") == 0);
    assert(strcmp(records.data[4], "value_5") == 0);

    records_free(&records);
}

static void test_binary_search_found(void)
/* Проверяет поиск существующего ключа */
{
    Records records = load_records();

    swap_pairs(&records, 0, 4);
    swap_pairs(&records, 1, 3);
    swap_pairs(&records, 0, 2);

    insertion_sort(&records);

    int index = binary_search(&records, "key_3");

    assert(index != -1);
    assert(strcmp(records.keys[index], "key_3") == 0);
    assert(strcmp(records.data[index], "value_3") == 0);

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

int main()
{
    test_records_read();
    test_insertion_sort_order();
    test_insertion_sort_preserves_pairs();
    test_binary_search_found();
    test_binary_search_not_found();

    printf("All tests passed\n");
    return 0;
}
