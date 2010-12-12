//------------------------------------------------------------------------------
//  attrid.cc
//  (C) 2006 Radon Labs GmbH
//------------------------------------------------------------------------------
#include "stdneb.h"
#include "attr/attrid.h"

namespace Attr
{
//------------------------------------------------------------------------------
/**
    Returns an array with all existing attribute ids. This is a slow
    method.
*/
Util::FixedArray<AttrId>
AttrId::GetAllAttrIds()
{
    n_assert(0 != AttributeDefinitionBase::NameRegistry);

    // get attribute registry content
    Util::Array<Util::KeyValuePair<Util::String, const Attr::AttributeDefinitionBase*> > attrDefs = AttributeDefinitionBase::NameRegistry->Content();
    Util::FixedArray<AttrId> result(attrDefs.Size());
    IndexT i;
    SizeT num = attrDefs.Size();
    for (i = 0; i < num; i++)
    {
        AttrId attrId(*(attrDefs[i].Value()));
        result[i] = attrId;
    }
    return result;
}

} // namespace Attr
