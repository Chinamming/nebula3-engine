//------------------------------------------------------------------------------
//  colorparams.cc
//  (C) 2006 Radon Labs GmbH
//------------------------------------------------------------------------------
#include "stdneb.h"
#include "parambase.h"

namespace PostEffect
{
using namespace Math;

__ImplementClass(PostEffect::ParamBase, 'PEPB', Core::RefCounted);

//------------------------------------------------------------------------------
/**
*/
ParamBase::ParamBase()
{
    // empty
}

//------------------------------------------------------------------------------
/**
*/
ParamBase::~ParamBase()
{
    // empty
}

//------------------------------------------------------------------------------
/**
*/
void
ParamBase::operator=(const Ptr<ParamBase>& rhs)
{
}

//------------------------------------------------------------------------------
/**
*/
void
ParamBase::BlendTo(const Ptr<ParamBase>& p, float l)
{
    // overwrite in subclass
}

//------------------------------------------------------------------------------
/**
*/
bool
ParamBase::Equals(const Ptr<ParamBase>& v2, float threshold) const
{
    // overwrite in subclass
    // and do not compare stuff like time or blending flag -> senseless
    return true;
}
} // namespace PostEffects
