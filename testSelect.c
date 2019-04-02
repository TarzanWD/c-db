#include <stdio.h>
#include <stdlib.h>
#include "compiled_query.h"
#include "table.h"
#include "column_types.h"
#include "column.h"
#include "compiled_column.h"

extern int select(CompiledQuery *compiledQueryMock, Table * tableMock, FILE * dataFileMock);

int testSelect()
{
    printf("Unit test on selecting from data file. \n");

    FILE * testFile;
    testFile = fopen("./test/test_insert.dat", "rb");

    // Mock data
    CompiledQuery * compiledQueryMock;
    compiledQueryMock = malloc(sizeof(CompiledQuery));

    compiledQueryMock->type = SELECT;
    compiledQueryMock->columnCount = 3;

    CompiledColumn column1;
    column1.name = "first";
    compiledQueryMock->queryColumns[0] = &column1;

    CompiledColumn column2;
    column2.name = "second";
    compiledQueryMock->queryColumns[1] = &column2;

    CompiledColumn column3;
    column3.name = "third";
    compiledQueryMock->queryColumns[2] = &column3;

    Table * table;
    table = malloc(sizeof(Table));
    table->info.columnCount = 3;

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
    printf("Running select command. \n");
    int i = 0;
    for (i = 0; i < 3; i++)
    {
        select(compiledQueryMock, table, testFile);
    }
    printf("Command ran. \n");

    fclose(testFile);
    free(table);
    free(compiledQueryMock);

    return 0;
}
