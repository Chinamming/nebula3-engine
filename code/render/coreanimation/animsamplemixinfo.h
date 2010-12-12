#pragma once
//------------------------------------------------------------------------------
/**
    @class CoreAnimation::AnimSampleMixInfo
    
    A data structure for providing sample/mixing attributes to 
    asynchronous jobs in the CoreAnimation subsystem.
    
    (C) 2009 Radon Labs GmbH
*/
#include "core/types.h"
#include "math/float4.h"
#include "coreanimation/sampletype.h"

//------------------------------------------------------------------------------
namespace CoreAnimation
{
struct NEBULA3_ALIGN16 AnimSampleMixInfo
{
    SampleType::Code sampleType;
    float sampleWeight;
    float mixWeight;
    Math::float4 velocityScale;
};

} // namespace CoreAnimation   
//------------------------------------------------------------------------------
