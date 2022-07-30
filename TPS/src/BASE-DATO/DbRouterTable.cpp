#include "DbRouterTable.h"

DbRouterTable::~DbRouterTable(void)
{
}

void DbRouterTable::getTableName( char* name, int size )
{
    strncpy(name,((struct _dbRouter*)DbTable::mPtrStruct)->tableName, size);
}

void DbRouterTable::getColumnName( char* name, int size )
{
    strncpy(name,((struct _dbRouter*)DbTable::mPtrStruct)->columName, size);
}

char DbRouterTable::getDataType()
{
    return ((struct _dbRouter*)DbTable::mPtrStruct)->dataType;
}

int DbRouterTable::getColumnOffset()
{
    return ((struct _dbRouter*)DbTable::mPtrStruct)->columnOffset;
}

int DbRouterTable::getDataSize()
{
    return ((struct _dbRouter*)DbTable::mPtrStruct)->dataSize;
}

int DbRouterTable::getTableNumber()
{
    return ((struct _dbRouter*)DbTable::mPtrStruct)->tableNumber;
}

bool DbRouterTable::getPKey()
{
    return ((struct _dbRouter*)DbTable::mPtrStruct)->pkey == 1;
}

void DbRouterTable::getColumnNameSuc( char* name, int size )
{
    strncpy(name,((struct _dbRouter*)DbTable::mPtrStruct)->columNameSuc, size);
}
