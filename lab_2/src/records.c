#include "records.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

RecordsStatus records_read(Records *records, const char *filename)
/* Читает таблицу из файла в раздельные массивы ключей и данных */
{
    if (records == NULL || filename == NULL)
    {
        return REC_NULL_POINTER;
    }

    FILE *file = fopen(filename, "r");

    if (file == NULL)
    {
        return REC_FILE_OPEN_ERROR;
    }

    int count;
    if (fscanf(file, "%d", &count) != 1 || count <= 0)
    {
        fclose(file);
        return REC_INVALID_COUNT;
    }

    int symbol;
    // Пропускаем остаток строки после count
    do
    {
        symbol = fgetc(file); // Один символ из файла
    } while (symbol != '\n' && symbol != EOF);

    records->count = count;
    records->keys = NULL;
    records->data = NULL;

    records->keys = malloc(sizeof(records->keys[0]) * count);
    if (records->keys == NULL)
    {
        fclose(file);
        return REC_MEMORY_ERROR;
    }

    records->data = malloc(sizeof(records->data[0]) * count);
    if (records->data == NULL)
    {
        records_free(records);
        fclose(file);
        return REC_MEMORY_ERROR;
    }

    // Зануляем элементы, чтобы records_free можно было вызывать даже после ошибки
    for (int i = 0; i < count; i++)
    {
        records->keys[i] = NULL;
        records->data[i] = NULL;
    }

    char *buffer = NULL;    // ИСПРАВИЛ
    size_t buffer_size = 0; // ИСПРАВИЛ
    ssize_t line_length;    // ИСПРАВИЛ

    for (int i = 0; i < count; i++)
    {
        // Считываем ключ
        line_length = getline(&buffer, &buffer_size, file);
        if (line_length == -1)
        {
            free(buffer);
            records_free(records);
            fclose(file);
            return REC_READ_ERROR;
        }

        if (line_length > 0 && buffer[line_length - 1] == '\n')
        {
            buffer[line_length - 1] = '\0';
        }

        size_t length = strlen(buffer);
        records->keys[i] = malloc(sizeof(char) * (length + 1));
        if (records->keys[i] == NULL)
        {
            free(buffer);
            records_free(records);
            fclose(file);
            return REC_MEMORY_ERROR;
        }

        strcpy(records->keys[i], buffer);

        // Считываем связанные данные
        line_length = getline(&buffer, &buffer_size, file);
        if (line_length == -1)
        {
            free(buffer);
            records_free(records);
            fclose(file);
            return REC_READ_ERROR;
        }

        if (line_length > 0 && buffer[line_length - 1] == '\n')
        {
            buffer[line_length - 1] = '\0';
        }

        length = strlen(buffer);
        records->data[i] = malloc(sizeof(char) * (length + 1));
        if (records->data[i] == NULL)
        {
            free(buffer);
            records_free(records);
            fclose(file);
            return REC_MEMORY_ERROR;
        }

        strcpy(records->data[i], buffer);
    }

    free(buffer);
    fclose(file);
    return REC_OK;
}

void records_print(const Records *records)
/* Печатает все записи таблицы */
{
    if (records == NULL)
    {
        return;
    }

    for (int i = 0; i < records->count; i++)
    {
        printf("%d. key: %s, data: %s\n", i + 1, records->keys[i], records->data[i]);
    }
}

void records_free(Records *records)
/* Освобождает память, выделенную под таблицу */
{
    if (records == NULL)
    {
        return;
    }

    for (int i = 0; i < records->count; i++)
    {
        if (records->keys != NULL)
        {
            free(records->keys[i]);
        }
        if (records->data != NULL)
        {
            free(records->data[i]);
        }
    }

    free(records->keys);
    free(records->data);

    records->keys = NULL;
    records->data = NULL;
    records->count = 0;
}
