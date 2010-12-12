//------------------------------------------------------------------------------
//  plane.cc
//  (C) 2007 Radon Labs GmbH
//------------------------------------------------------------------------------
#include "stdneb.h"
#include "math/plane.h"
#include "math/matrix44.h"

namespace Math
{

//------------------------------------------------------------------------------
/**
*/
plane
plane::transform(__PlaneArg p, const matrix44& m)
{
    return XMPlaneTransform(p.vec, m.mx);
}

} // namespace Math