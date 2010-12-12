#pragma once
//------------------------------------------------------------------------------
/**
    @class FX::AnimEventTimingHandler
    
    This is the godsend animevent handler, to handle timefactor events
    
    (C) 2009 Radon Labs GmbH
*/
#include "animation/animeventhandlerbase.h"
#include "fx/animeventregistry.h"
#include "timing/timer.h"

//------------------------------------------------------------------------------
namespace FX
{
class AnimEventTimingHandler : public Animation::AnimEventHandlerBase
{
    __DeclareClass(AnimEventTimingHandler);

public:
    /// constructor
    AnimEventTimingHandler();
    /// destructor
    virtual ~AnimEventTimingHandler();  

    /// called once on startup 
    virtual void Open();
    /// called once before shutdown
    virtual void Close();
    /// handle a event
    virtual bool HandleEvent(const Animation::AnimEventInfo& event);

    /// check if we have to reset time factor
    virtual void OnFrame(Timing::Time time);

private:  
    AnimEventRegistry::TimingEvent curTiming;
    bool curTimingActive;
    float curFactor;

    Timing::Timer timer;                              // we need an own timer, to prevent influencing our own time
};
} // namespace Animation
//------------------------------------------------------------------------------
