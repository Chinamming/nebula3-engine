#pragma once
//------------------------------------------------------------------------------
/**
    @class Win360::Win360ThreadId
    
    A thread id uniquely identifies a thread within the process.
    
    (C) 2008 Radon Labs GmbH
*/
#include "core/types.h"

namespace Threading
{
typedef DWORD ThreadId;
static const ThreadId InvalidThreadId = 0xffffffff;
}
//------------------------------------------------------------------------------
