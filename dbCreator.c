#include <stdio.h>
#include <stdlib.h>

#include "column.h"
#include "column_types.h"
#include "table.h"
#include "table_info.h"
#include "file_paths.h"
#include "file_type.h"
#include "compiled_query.h"

extern getHeaderFile();
extern getDataFile();

int generateHeadFile(Table * table)
{
    FILE * newHeaderFile = getHeaderFile(table->info.name, "wb");

    if (newHeaderFile == NULL)
    {
        printf("Could not create new header file");

        return -1;
    }

    printf("Header file created successfully.");

    fwrite(&table->info, sizeof(TableInfo), 1, newHeaderFile);
    fclose(newHeaderFile);

    return 0;
}

int generateDataFile(Table * table)
{
    FILE * newDataFile;
    newDataFile = getDataFile(table->info.name, "wb");

    if (newDataFile == NULL) {
        printf("Could not create new data file");

        return -1;
    }

    return 0;
}

Column * buildNewColumn(char * name, ColumnTypes type)
{
    Column * newColumn;
    newColumn = malloc(sizeof(Column));
    strcpy(newColumn->name, name);
    newColumn->type = type;

    return newColumn;
}

Table * readHeadTable (FILE * headerFile)
{
    Table * newTable;
    newTable = malloc(sizeof(Table));
    fread(&newTable->info, sizeof(TableInfo), 1, headerFile);
    int i = 0;
    while (fread(&newTable->columns[i], sizeof(Column), 1, headerFile) > 0)
    {
        i += 1;
    }

    return newTable;
}

int addColumn(char * columnName, ColumnTypes columnType, char * tableName)
{
    FILE * headerFile = getHeaderFile(tableName, "rb");
    if (headerFile == NULL)
    {
        printf("Couldn't get header file. Aborting.");
        return -1;
    }

    Table * table = readHeadTable(headerFile);

    table->columns[table->info.columnCount] = buildNewColumn(columnName, columnType);
    table->info.columnCount += 1;

    headerFile = getHeaderFile(tableName, "wb");
    fwrite(&table->info, sizeof(TableInfo), 1, headerFile);
    int i;
    for (i = 0; i < table->info.columnCount; i++)
    {
        fwrite(table->columns[i], sizeof(Column), 1, headerFile);
    }

    fclose(headerFile);

    return 0;
}

int deleteColumn(char * columnName, char * tableName)
{
    FILE * headerFile = getHeaderFile(tableName, "rb");
    if (headerFile == NULL)
    {
        printf("Couldn't get header file. Aborting.");
        return -1;
    }

    Table * table = readHeadTable(headerFile);
    Column newColumns[table->info.columnCount];
    int i = 0;
    for (i = 0; i < table->info.columnCount; i++)
    {
        if (strcmp(&table->columns[i]->name, columnName))
        {

        }
    }

    // TODO: Remove from data file!
}

int initTable(char * tableName)
{
    Table table;
    table.info.rowCount = 0;
    table.info.columnCount = 0;
    strcpy(table.info.name, tableName);

    int headerFileCreated = generateHeadFile(&table);
    int dataFileCreated = generateDataFile(&table);

    if (headerFileCreated == 0 && dataFileCreated == 0)
    {
        return 0;
    }
    return -1;
}

int removeHeaderFile(char * tableName)
{
    char * tablePath = getHeaderFilePath(tableName);
    int status = remove(tablePath);
    return status;
}

int removeDataFile(char * tableName)
{
    char * tablePath = getDataFilePath(tableName);
    int status = remove(tableName);
    return status;
}

int deleteTable(char * tableName)
{
    int headerFileRemoved = removeHeaderFile(tableName);
    int dataFileRemoved = removeDataFile(tableName);
    if (headerFileRemoved != 0 || dataFileRemoved != 0)
    {
        printf("Table wasn't deleted, error occurred");
        return -1;
    }

    return 0;
}

int insert(CompiledQuery * compiledQuery, Table * table, FILE * dataFile)
{
    int i = 0;
    for (i = 0; i < TABLE_MAX_COLUMNS_LENGTH; i++)
    {
        int j = 0;
        int isPaired = 0;
        for (j = 0; j < TABLE_MAX_COLUMNS_LENGTH; j++)
        {
            if (isPaired == 0 && strcmp(table->columns[j]->name, compiledQuery->queryColumns[i]->name))
            {
                switch(table->columns[j]->type) {
                    case VARCHAR: {
                        // Default VARCHAR 70
                        // TODO: Support various length of columns
                        char val[70];
                        strcpy(val, &compiledQuery->queryValues[i]);
                        fwrite(val, sizeof(char[70]), 1, dataFile);
                    } break;

                    case INT: {
                        // Default INT 8
                        // TODO: Handle wrong type of input
                        char val[8];
                        strcpy(val, &compiledQuery->queryValues[i]);
                        fwrite(atoi(val[8]), sizeof(int), 1, dataFile);
                    } break;

                    default: {
                        printf("Insert aborted, unknown type.");
                        return -1;
                    }
                }

                isPaired = 1;
            }
        }

        if (isPaired == 0) {
            switch(table->columns[j]->type) {
                case VARCHAR: {
                    fwrite(0, sizeof(char[70]), 1, dataFile);
                } break;

                case INT: {
                    fwrite(0, sizeof(int), 1, dataFile);
                } break;

                default: {
                    printf("Insert aborted, unknown type.");
                    return -1;
                }
            }
        }
    }

    return 1;
}
