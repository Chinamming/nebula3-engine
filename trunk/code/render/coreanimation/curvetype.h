#pragma once
//------------------------------------------------------------------------------
/**
    @class CoreAnimation::CurveType
  
    Describes the general data type of the keys stored in an animation curve.
    
    (C) 2008 Radon Labs GmbH
*/
#include "core/types.h"
#if !__NEBULA3_JOB__
#include "util/string.h"
#endif

//------------------------------------------------------------------------------
namespace CoreAnimation
{
class CurveType
{
public:
    /// animation curve types
    enum Code
    {
        Translation,    //> keys in curve describe a translation
        Scale,          //> keys in curve describe a scale
        Rotation,       //> keys in curve describe a rotation quaternion
        Color,          //> keys in curve describe a color
        Velocity,       //> keys describe a linear velocition
        Float4,         //> generic 4D key

        NumCurveTypes,
        InvalidCurveType,
    };

    #if !__NEBULA3_JOB__
    /// convert from string
    static Code FromString(const Util::String& str);
    /// convert to string
    static Util::String ToString(Code c);
    #endif
};

} // namespace CoreAnimation
//------------------------------------------------------------------------------

