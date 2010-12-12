//------------------------------------------------------------------------------
//  gamecontentserver.cc
//  (C) 2009 Radon Labs GmbH
//------------------------------------------------------------------------------
#include "stdneb.h"
#include "io/gamecontentserver.h"

namespace IO
{
#if (__WIN32__ || __XBOX360__ || __WII__ || __OSX__)
__ImplementClass(IO::GameContentServer, 'IGCS', Base::GameContentServerBase);
#elif __PS3__
__ImplementClass(IO::GameContentServer, 'IGCS', PS3::PS3GameContentServer);
#else
#error "IO::GameContentServer not implemented on this platform!"
#endif
__ImplementInterfaceSingleton(IO::GameContentServer);

//------------------------------------------------------------------------------
/**
*/
GameContentServer::GameContentServer()
{
    __ConstructInterfaceSingleton;
}

//------------------------------------------------------------------------------
/**
*/
GameContentServer::~GameContentServer()
{
    __DestructInterfaceSingleton;
}

} // namespace IO