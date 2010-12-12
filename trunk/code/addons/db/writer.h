#ifndef DB_WRITER_H
#define DB_WRITER_H
//------------------------------------------------------------------------------
/**
    @class Db::Writer
    
    A wrapper class to bulk-write data to the database in a simple way.
    
    (C) 2006 Radon Labs GmbH
*/
#include "core/refcounted.h"
#include "attr/attrid.h"
#include "addons/db/column.h"
#include "addons/db/valuetable.h"
#include "addons/db/database.h"

//------------------------------------------------------------------------------
namespace Db
{
class Writer : public Core::RefCounted
{
    __DeclareClass(Writer);
public:
    /// constructor
    Writer();
    /// destructor
    virtual ~Writer();
    
    /// set database object
    void SetDatabase(const Ptr<Database>& db);
    /// set the database table name
    void SetTableName(const Util::String& n);
    /// add a column definition
    void AddColumn(const Db::Column& col);
    /// set true if the table should be deleted before writing new data
    void SetFlushTable(bool flushTable);

    /// open the writer
    bool Open();
    /// special case: open from existing value table
    bool OpenFromValueTable(const Ptr<ValueTable>& values);
    /// return true if open
    bool IsOpen() const;
    /// close the writer - write to DB
    void Close();

    /// begin writing a new row
    void BeginRow();
    /// set bool attribute in current row
    void SetBool(Attr::BoolAttrId id, bool b);
    /// set int attribute in current row
    void SetInt(Attr::IntAttrId id, int i);
    /// set float attribute in current row
    void SetFloat(Attr::FloatAttrId id, float f);
    /// set string attribute in current row
    void SetString(Attr::StringAttrId id, const Util::String& s);
    /// set float4 attribute in current row
    void SetFloat4(Attr::Float4AttrId id, const Math::float4& v);
    /// set matrix44 attribute in current row
    void SetMatrix44(Attr::Matrix44AttrId id, const Math::matrix44& m);
    /// set guid attribute in current row
    void SetGuid(Attr::GuidAttrId id, const Util::Guid& guid);
    /// set blob attribute in current row
    void SetBlob(Attr::BlobAttrId id, const Util::Blob& blob);
    /// end current row
    void EndRow();

private:
    /// check if attribute exists in current row
    IndexT FindAttrIndex(Attr::AttrId id) const;

    Ptr<Database> database;
    bool isOpen;
    bool inBeginRow;
    Util::String tableName;
    Util::Array<Column> columns;
    Util::Dictionary<Attr::AttrId,IndexT> columnMap;
    Ptr<ValueTable> valueTable;     
    bool flushTable;
    IndexT rowIndex;
};

//------------------------------------------------------------------------------
/**
    Set pointer to database.
*/
inline void
Writer::SetDatabase(const Ptr<Database>& db)
{
    this->database = db;
}

//------------------------------------------------------------------------------
/**
    Set the name of database table the writer will work on.
*/
inline void
Writer::SetTableName(const Util::String& t)
{
    this->tableName = t;
}

//------------------------------------------------------------------------------
/**
    Add a column to the db writer.
*/
inline void
Writer::AddColumn(const Db::Column& col)
{    
    n_assert(!this->columnMap.Contains(col.GetAttrId()));
    this->columns.Append(col);
    this->columnMap.Add(col.GetAttrId(), this->columns.Size() - 1);
}

//------------------------------------------------------------------------------
/**
*/
inline bool
Writer::IsOpen() const
{
    return this->isOpen;
}

//------------------------------------------------------------------------------
/**
*/
inline void
Writer::SetBool(Attr::BoolAttrId id, bool b)
{
    n_assert(this->inBeginRow);
    this->valueTable->SetBool(id, this->rowIndex, b);
}

//------------------------------------------------------------------------------
/**
*/
inline void
Writer::SetInt(Attr::IntAttrId id, int i)
{
    n_assert(this->inBeginRow);
    this->valueTable->SetInt(id, this->rowIndex, i);
}

//------------------------------------------------------------------------------
/**
*/
inline void
Writer::SetFloat(Attr::FloatAttrId id, float f)
{
    n_assert(this->inBeginRow);
    this->valueTable->SetFloat(id, this->rowIndex, f);
}

//------------------------------------------------------------------------------
/**
*/
inline void
Writer::SetString(Attr::StringAttrId id, const Util::String& s)
{
    n_assert(this->inBeginRow);
    this->valueTable->SetString(id, this->rowIndex, s);
}

//------------------------------------------------------------------------------
/**
*/
inline void
Writer::SetFloat4(Attr::Float4AttrId id, const Math::float4& v)
{
    n_assert(this->inBeginRow);
    this->valueTable->SetFloat4(id, this->rowIndex, v);
}

//------------------------------------------------------------------------------
/**
*/
inline void
Writer::SetMatrix44(Attr::Matrix44AttrId id, const Math::matrix44& m)
{
    n_assert(this->inBeginRow);
    this->valueTable->SetMatrix44(id, this->rowIndex, m);
}

//------------------------------------------------------------------------------
/**
*/
inline void
Writer::SetGuid(Attr::GuidAttrId id, const Util::Guid& guid)
{
    n_assert(this->inBeginRow);
    this->valueTable->SetGuid(id, this->rowIndex, guid);
}

//------------------------------------------------------------------------------
/**
*/
inline void
Writer::SetBlob(Attr::BlobAttrId id, const Util::Blob& blob)
{
    n_assert(this->inBeginRow);
    this->valueTable->SetBlob(id, this->rowIndex, blob);
}

//------------------------------------------------------------------------------
/**
*/
inline void
Writer::SetFlushTable(bool flush)
{
    this->flushTable = flush;
}

}; // namespace Db
//------------------------------------------------------------------------------
#endif