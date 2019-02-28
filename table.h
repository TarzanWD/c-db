#include "column.h"
#include "table_info.h"

#define TABLE_MAX_COLUMNS_LENGTH 100

#ifndef TABLE_INCLUDED_
#define TABLE_INCLUDED_

typedef struct {
    Column * columns[TABLE_MAX_COLUMNS_LENGTH];
    TableInfo info;
} Table;

#endif // TABLE_INCLUDED_
