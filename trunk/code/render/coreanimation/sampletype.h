#pragma once
//------------------------------------------------------------------------------
/**
    @class CoreAnimation::SampleType
  
    Describes how an animation curve should be sampled.
    
    (C) 2008 Radon Labs GmbH
*/
#include "core/types.h"
#if !__NEBULA3_JOB__
#include "util/string.h"
#endif

//------------------------------------------------------------------------------
namespace CoreAnimation
{
class SampleType
{
public:
    /// animation sample types
    enum Code
    {
        Step,           //> do not interpolate, just step
        Linear,         //> simple linear interpolation
        Hermite,        //> herminte spline interpolation
        CatmullRom,     //> catmull-rom interpolation

        NumSampleTypes,
        InvalidSampleType,
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

