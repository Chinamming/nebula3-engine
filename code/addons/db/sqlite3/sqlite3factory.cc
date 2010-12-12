//------------------------------------------------------------------------------
//  sqlite3factory.cc
//  (C) 2006 Radon Labs GmbH
//------------------------------------------------------------------------------
#include "stdneb.h"
#include "addons/db/sqlite3/sqlite3factory.h"
#include "addons/db/sqlite3/sqlite3database.h"
#include "addons/db/sqlite3/sqlite3table.h"
#include "addons/db/sqlite3/sqlite3dataset.h"
#include "addons/db/sqlite3/sqlite3command.h"
#include "addons/db/sqlite3/sqlite3filterset.h"

namespace Db
{
__ImplementClass(Db::Sqlite3Factory, 'S3FC', Db::DbFactory);
__ImplementSingleton(Db::Sqlite3Factory);

//------------------------------------------------------------------------------
/**
*/
Sqlite3Factory::Sqlite3Factory()
{
    __ConstructSingleton;
}

//------------------------------------------------------------------------------
/**
*/
Sqlite3Factory::~Sqlite3Factory()
{
    __DestructSingleton;
}

//------------------------------------------------------------------------------
/**
*/
Ptr<Database>
Sqlite3Factory::CreateDatabase() const
{
    return Sqlite3Database::Create();
}

//------------------------------------------------------------------------------
/**
*/
Ptr<Command>
Sqlite3Factory::CreateCommand() const
{
    return Sqlite3Command::Create();
}

//------------------------------------------------------------------------------
/**
*/
Ptr<Table>
Sqlite3Factory::CreateTable() const
{
    return Sqlite3Table::Create();
}

//------------------------------------------------------------------------------
/**
*/
inline
Ptr<Dataset>
Sqlite3Factory::CreateDataset() const
{
    return Sqlite3Dataset::Create();
}

//------------------------------------------------------------------------------
/**
*/
inline
Ptr<FilterSet>
Sqlite3Factory::CreateFilterSet() const
{
    return Sqlite3FilterSet::Create();
}

}