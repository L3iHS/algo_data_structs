#ifndef RECORDS_H
#define RECORDS_H

typedef struct Records
{
    int count;
    char **keys;
    char **data;
} Records;

typedef enum
{
    REC_OK,              // Успешно
    REC_FILE_OPEN_ERROR, // Не удалось открыть файл
    REC_INVALID_COUNT,   // В первой строке файла некорректное число записей
    REC_MEMORY_ERROR,    // Не удалось выделить память
    REC_READ_ERROR,      // Не удалось прочитать данные из файла
    REC_NULL_POINTER     // В функцию передали NULL
} RecordsStatus;

RecordsStatus records_read(Records *records, const char *filename);
void records_print(const Records *records);
void records_free(Records *records);

#endif