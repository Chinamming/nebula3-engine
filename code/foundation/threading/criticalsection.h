#pragma once
//------------------------------------------------------------------------------
/**
    @class Threading::CriticalSection

    Critical section objects are used to protect a portion of code from parallel
    execution. Define a static critical section object and use its Enter() 
    and Leave() methods to protect critical sections of your code.
    
    (C) 2006 Radon Labs GmbH
*/
#include "core/config.h"
#if (__WIN32__ || __XBOX360__)
#include "threading/win360/win360criticalsection.h"
namespace Threading
{
class CriticalSection : public Win360::Win360CriticalSection
{ };
}
#elif __WII__
#include "threading/wii/wiicriticalsection.h"
namespace Threading
{
class CriticalSection : public Wii::WiiCriticalSection
{ };
}
#elif __PS3__
#include "threading/ps3/ps3criticalsection.h"
namespace Threading
{
class CriticalSection : public PS3::PS3CriticalSection
{ };
}
#elif __OSX__
#include "threading/osx/osxcriticalsection.h"
namespace Threading
{
class CriticalSection : public OSX::OSXCriticalSection
{ };
}
#else
#error "Threading::CriticalSection not implemented on this platform!"
#endif
//------------------------------------------------------------------------------
    