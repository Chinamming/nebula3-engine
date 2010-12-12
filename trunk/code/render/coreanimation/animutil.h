#pragma once
//------------------------------------------------------------------------------
/**
    @class CoreAnimation::AnimUtil
    
    A class which contains utility methods for animation sampling and mixing.
    
    (C) 2008 Radon Labs GmbH
*/
#include "core/types.h"
#include "coreanimation/animclip.h"
#include "coreanimation/sampletype.h"
#include "coreanimation/animresource.h"
#include "coreanimation/animsamplebuffer.h"
#include "jobs/job.h"

//------------------------------------------------------------------------------
namespace CoreAnimation
{
class AnimUtil
{
public:
    /// OBSOLETE: sample an animation clip at some point in time into an AnimSampleBuffer
    static void Sample(const Ptr<AnimResource>& animResource, IndexT clipIndex, SampleType::Code sampleType, Timing::Tick time, float timeFactor, const Ptr<AnimSampleBuffer>& result);
    /// setup a job object which performs sampling
    static Ptr<Jobs::Job> CreateSampleJob(const Ptr<AnimResource>& animResource, IndexT clipIndex, SampleType::Code sampleType, Timing::Tick time, float timeFactor, const Ptr<AnimSampleBuffer>& result);    
    /// setup a job which performs both sampling and mixing
    static Ptr<Jobs::Job> CreateSampleAndMixJob(const Ptr<AnimResource>& animResource, IndexT clipIndex, SampleType::Code sampleType, Timing::Tick time, float timeFactor, float mixWeight, const Ptr<AnimSampleBuffer>& mixIn, const Ptr<AnimSampleBuffer>& result);    
    /// clamp key index into valid range
    static IndexT ClampKeyIndex(IndexT keyIndex, const AnimClip& clip);
    /// compute inbetween ticks for a given sample time
    static Timing::Tick InbetweenTicks(Timing::Tick sampleTime, const AnimClip& clip);
};

} // namespace CoreAnimation
//------------------------------------------------------------------------------