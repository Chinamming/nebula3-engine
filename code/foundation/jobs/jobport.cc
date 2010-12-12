//------------------------------------------------------------------------------
//  jobport.cc
//  (C) 2009 Radon Labs GmbH
//------------------------------------------------------------------------------
#include "stdneb.h"
#include "jobs/jobport.h"

namespace Jobs
{
#if __PS3__
__ImplementClass(Jobs::JobPort, 'JBPT', PS3::PS3JobPort);
#elif (NEBULA3_USE_SERIAL_JOBSYSTEM || __WII__)
__ImplementClass(Jobs::JobPort, 'JBPT', Jobs::SerialJobPort);
#elif (__WIN32__ || __XBOX360__)
__ImplementClass(Jobs::JobPort, 'JBPT', Jobs::TPJobPort);
#else
#error "Jobs::JobPort not implemented on this platform!"
#endif
}
