#pragma once
//------------------------------------------------------------------------------
/**
    @class Audio2::FmodEventState

    information about the state of an fmod-event

    (C) 2009 Radon Labs GmbH
*/

//------------------------------------------------------------------------------
namespace Audio2
{
    enum FmodEventState
    {
        EventStateUnknown = 0,
        EventPlaying = 1 << 0,
        EventPaused  = 1 << 1,
        EventVoicesStolen = 1 << 2
    };
} // namespace Audio2
//------------------------------------------------------------------------------
