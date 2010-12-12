#pragma once
//------------------------------------------------------------------------------
/**
    @class Core::SysFunc
    
    Wrap some platform specific low-level functions.
    
    (C) 2007 Radon Labs GmbH
*/
#include "core/config.h"
#if __WIN32__
#include "core/win32/win32sysfunc.h"
namespace Core
{
class SysFunc : public Win32::SysFunc
{
    // empty
};
} // namespace Core
#elif __XBOX360__
#include "core/xbox360/xbox360sysfunc.h"
namespace Core
{
class SysFunc : public Xbox360::SysFunc
{
    // empty
};
}
#elif __WII__
#include "core/wii/wiisysfunc.h"
namespace Core
{
class SysFunc : public Wii::SysFunc
{
    // empty
};
} // namespace Core
#elif __PS3__
#include "core/ps3/ps3sysfunc.h"
namespace Core
{
class SysFunc : public PS3::SysFunc
{
    // empty
};
} // namespace Core;
#elif __OSX__
#include "core/osx/osxsysfunc.h"
namespace Core
{
class SysFunc : public OSX::SysFunc
{
    // empty
};
} // namespace Core
#else
#error "Core::SysFunc not implemented on this platform!"
#endif
//------------------------------------------------------------------------------
