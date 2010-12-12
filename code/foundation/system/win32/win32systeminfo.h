#pragma once
//------------------------------------------------------------------------------
/**
    @class Win32::Win32SystemInfo
    
    Provide information about the system we're running on.
    
    (C) 2008 Radon Labs GmbH
*/
#include "system/base/systeminfobase.h"

//------------------------------------------------------------------------------
namespace Win32
{
class Win32SystemInfo : public Base::SystemInfoBase
{
public:
    /// constructor
    Win32SystemInfo();
};

} // namespace Win32
//------------------------------------------------------------------------------
