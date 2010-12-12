//------------------------------------------------------------------------------
//  thread.cc
//  (C) 2007 Radon Labs GmbH
//------------------------------------------------------------------------------
#include "stdneb.h"
#include "threading/thread.h"

namespace Threading
{
#if (__WIN32__ || __XBOX360__)
__ImplementClass(Threading::Thread, 'TRED', Win360::Win360Thread);
#elif __WII__
__ImplementClass(Threading::Thread, 'TRED', Wii::WiiThread);
#elif __PS3__
__ImplementClass(Threading::Thread, 'TRED', PS3::PS3Thread);
#elif __OSX__
__ImplementClass(Threading::Thread, 'TRED', OSX::OSXThread);
#else
#error "Thread class not implemented on this platform!"
#endif
}
