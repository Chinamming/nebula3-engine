//------------------------------------------------------------------------------
//  lightserver.cc
//  (C) 2007 Radon Labs GmbH
//------------------------------------------------------------------------------
#include "stdneb.h"
#include "lighting/lightserver.h"

namespace Lighting
{
#if __XBOX360__
    __ImplementClass(Lighting::LightServer, 'LISV', Lighting::LightPrePassServer);
#elif __WII__
__ImplementClass(Lighting::LightServer, 'LISV', Wii::WiiLightServer);
#elif (__WIN32__ || __PS3__)
__ImplementClass(Lighting::LightServer, 'LISV', Lighting::LightPrePassServer);
#else
#error "LightServer class not implemented on this platform!"
#endif
__ImplementSingleton(Lighting::LightServer);

//------------------------------------------------------------------------------
/**
*/
LightServer::LightServer()
{
    __ConstructSingleton;
}

//------------------------------------------------------------------------------
/**
*/
LightServer::~LightServer()
{
    __DestructSingleton;
}

} // namespace Lighting


