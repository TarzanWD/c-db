#include <stdio.h>
#include <stdlib.h>
#include "compiled_query.h"
#include "table.h"
#include "column_types.h"
#include "column.h"
#include "compiled_column.h"

extern int insert(CompiledQuery *compiledQueryMock, Table * tableMock, FILE * dataFileMock);

int testInsert()
{
    printf("Unit test on inserting into data file. \n");

    FILE * testFile;
    testFile = fopen("./test/test_insert.dat", "ab");

    // Mock data
    CompiledQuery * compiledQueryMock;
    compiledQueryMock = malloc(sizeof(CompiledQuery));

    compiledQueryMock->type = INSERT;
    compiledQueryMock->columnCount = 2;

    CompiledColumn column1;
    column1.name = "first";
    column1.value = "ABCDEF";
    compiledQueryMock->queryColumns[0] = &column1;

    CompiledColumn column2;
    column2.name = "second";
    column2.value = "123";
    compiledQueryMock->queryColumns[1] = &column2;

    CompiledColumn column3;
    compiledQueryMock->queryColumns[2] = &column3;

    Table * table;
    table = malloc(sizeof(Table));
    table->info.columnCount = 3;
    table->info.rowCount = 0;

    Column tableColumn1;
    strcpy(tableColumn1.name, "first");
    tableColumn1.type = VARCHAR;
    table->columns[0] = &tableColumn1;

    Column tableColumn2;
    strcpy(tableColumn2.name, "second");
    tableColumn2.type = INT;
    table->columns[1] = &tableColumn2;

    Column tableColumn3;
    strcpy(tableColumn3.name, "third");
    tableColumn3.type = VARCHAR;
    table->columns[2] = &tableColumn3;

    printf("Mocking done. \n");

    // Run command
    printf("Running insert command. \n");
    int i = 0;
    for (i = 0; i < 3; i++)
    {
        insert(compiledQueryMock, table, testFile);
    }
    printf("Command ran. \n");

    fclose(testFile);

    // Read back values
    testFile = fopen("./test/test_insert.dat", "rb");

    for (i = 0; i < 3; i++)
    {
        char val1[70];
        fread(val1, sizeof(char[70]), 1, testFile);

        int val2;
        fread(&val2, sizeof(int), 1, testFile);

        char val3[70];
        fread(val3, sizeof(char[70]), 1, testFile);

        // Print
        printf("first: %s, second: %d, third: %s \n", val1, val2, val3);
    }

    fclose(testFile);

    free(table);
    free(compiledQueryMock);

    return 0;
}
