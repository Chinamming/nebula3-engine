//------------------------------------------------------------------------------
//  relation.cc
//  (C) 2006 Radon Labs GmbH
//------------------------------------------------------------------------------
#include "stdneb.h"
#include "addons/db/relation.h"

namespace Db
{
__ImplementClass(Db::Relation, 'RLTN', Core::RefCounted);

//------------------------------------------------------------------------------
/**
*/
Relation::Relation()
{
    // empty
}

//------------------------------------------------------------------------------
/**
*/
Relation::~Relation()
{
    // empty
}

} // namespace Db
