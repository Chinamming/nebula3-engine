//------------------------------------------------------------------------------
//  reader.cc
//  (C) 2006 Radon Labs GmbH
//------------------------------------------------------------------------------
#include "stdneb.h"
#include "addons/db/reader.h"
#include "addons/db/dbserver.h"

namespace Db
{
__ImplementClass(Db::Reader, 'DBRE', Core::RefCounted);

//------------------------------------------------------------------------------
/**
*/
Reader::Reader() :
    isOpen(false),
    curRowIndex(InvalidIndex)
{
    // empty
}

//------------------------------------------------------------------------------
/**
*/
Reader::~Reader()
{
    if (this->isOpen)
    {
        this->Close();
    }
}

//------------------------------------------------------------------------------
/**
    Open the reader. This will create a dataset which performs a query
    on the database.
*/
bool
Reader::Open()
{
    n_assert(!this->isOpen);
    n_assert(this->database.isvalid());
    
    // get database from db server
    if (!this->database->HasTable(this->tableName))
    {
        n_error("Db::Reader(): database '%s' has no table '%s'!", 
            this->database->GetURI().AsString().AsCharPtr(),
            this->tableName.AsCharPtr());
        return false;
    }
    
    // create and configure a dataset object
    Ptr<Table> table = this->database->GetTableByName(this->tableName);
    this->dataset = table->CreateDataset();
    this->dataset->AddAllTableColumns();
    IndexT i;
    IndexT num = this->filterAttrs.Size();
    for (i = 0; i < this->filterAttrs.Size(); i++)
    {
        this->dataset->Filter()->AddEqualCheck(this->filterAttrs[i]);
        if (i < (num - 1))
        {
            this->dataset->Filter()->AddAnd();
        }
    }

    // perform query on database
    this->dataset->PerformQuery();
    this->valueTable = this->dataset->Values();

    this->isOpen = true;
    return true;
}

//------------------------------------------------------------------------------
/**
    Close the reader. This deletes the query object created in the
    constructor.
*/
void
Reader::Close()
{
    n_assert(this->isOpen);
    this->valueTable = 0;
    this->dataset = 0;
    this->database = 0;
    this->isOpen = false;
}

//------------------------------------------------------------------------------
/**
    Return true if a specific attribute exists in the current row.
*/
bool
Reader::HasAttr(Attr::AttrId attrId) const
{
    n_assert(this->isOpen);
    return this->valueTable->HasColumn(attrId);
}

//------------------------------------------------------------------------------
/**
*/
bool
Reader::GetBool(Attr::BoolAttrId attrId) const
{
    n_assert(this->isOpen);
    return this->valueTable->GetBool(attrId, this->curRowIndex);
}

//------------------------------------------------------------------------------
/**
*/
int
Reader::GetInt(Attr::IntAttrId attrId) const
{
    n_assert(this->isOpen);
    return this->valueTable->GetInt(attrId, this->curRowIndex);
}

//------------------------------------------------------------------------------
/**
*/
float
Reader::GetFloat(Attr::FloatAttrId attrId) const
{
    n_assert(this->isOpen);
    return this->valueTable->GetFloat(attrId, this->curRowIndex);
}

//------------------------------------------------------------------------------
/**
*/
const Util::String&
Reader::GetString(Attr::StringAttrId attrId) const
{
    n_assert(this->isOpen);
    return this->valueTable->GetString(attrId, this->curRowIndex);
}

//------------------------------------------------------------------------------
/**
*/
const Math::float4
Reader::GetFloat4(Attr::Float4AttrId attrId) const
{
    n_assert(this->isOpen);
    return this->valueTable->GetFloat4(attrId, this->curRowIndex);
}

//------------------------------------------------------------------------------
/**
*/
const Math::matrix44
Reader::GetMatrix44(Attr::Matrix44AttrId attrId) const
{
    n_assert(this->isOpen);
    return this->valueTable->GetMatrix44(attrId, this->curRowIndex);
}

//------------------------------------------------------------------------------
/**
*/
const Util::Guid&
Reader::GetGuid(Attr::GuidAttrId attrId) const
{
    n_assert(this->isOpen);
    return this->valueTable->GetGuid(attrId, this->curRowIndex);
}

//------------------------------------------------------------------------------
/**
*/
const Util::Blob&
Reader::GetBlob(Attr::BlobAttrId attrId) const
{
    n_assert(this->isOpen);
    return this->valueTable->GetBlob(attrId, this->curRowIndex);
}

}; // namespace Db
