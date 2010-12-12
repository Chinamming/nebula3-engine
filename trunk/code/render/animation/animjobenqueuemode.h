#pragma once
//------------------------------------------------------------------------------
/**
    @class Animation::AnimJobEnqueueMode
    
    Describes the behaviour when enqueueing a new animation job into
    an anim sequencer.
    
    (C) 2010 Radon Labs GmbH
*/
#include "core/types.h"

//------------------------------------------------------------------------------
namespace Animation
{
class AnimJobEnqueueMode
{
public:
    /// enqueue modes
    enum Code
    {
        Append,                     // append the new job after other enqueued jobs on same track
        Intercept,                  // stop all clips on same track and insert new job at current time
        IgnoreIfSameClipActive,     // ignore the new job if another job with the same clip name is running
        IgnoreIfSameExclTagActive,  // ignore the new job is another job with the same exclusive tag is running
    };

    /// convert to string
    static const char* ToString(Code c);
    /// convert from string
    static Code FromString(const char* s);
};

//------------------------------------------------------------------------------
/**
*/
inline AnimJobEnqueueMode::Code
AnimJobEnqueueMode::FromString(const char* s)
{
    n_assert(0 != s);
    if (0 == Util::String::StrCmp(s, "Append")) return Append;
    else if (0 == Util::String::StrCmp(s, "Intercept")) return Intercept;
    else if (0 == Util::String::StrCmp(s, "IgnoreIfSameClipActive")) return IgnoreIfSameClipActive;
    else if (0 == Util::String::StrCmp(s, "IgnoreIfSameExclTagActive")) return IgnoreIfSameExclTagActive;
    else
    {
        n_error("AnimJobEnqueueMode: Invalid mode string '%s'!\n", s);
    }
}

//------------------------------------------------------------------------------
/**
*/
inline const char*
AnimJobEnqueueMode::ToString(Code c)
{
    switch (c)
    {
        case Append: return "Append";
        case Intercept: return "Intercept";
        case IgnoreIfSameClipActive: return "IgnoreIfSameClipActive";
        case IgnoreIfSameExclTagActive: return "IgnoreIfSameExclTagActive";
        default:
            n_error("AnimJobEnqueueMode: Invalid mode '%d'!\n", c);
            break;
    }
}

} // namespace Animation
//------------------------------------------------------------------------------

