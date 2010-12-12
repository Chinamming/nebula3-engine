#pragma once
//------------------------------------------------------------------------------
/**
    @class Net::TcpServer

    Front-end wrapper class for StdTcpServer, see StdTcpServer for details!

    (C) 2009 Radon Labs GmbH
*/
#include "core/config.h"
#if (__WIN32__ || __XBOX360__ || __PS3__)
#include "net/tcp/stdtcpserver.h"
namespace Net
{
class TcpServer : public StdTcpServer
{
    __DeclareClass(TcpServer);
};
}
#elif __WII__
#include "net/wii/wiitcpserver.h"
namespace Net
{
class TcpServer : public Wii::WiiTcpServer
{
    __DeclareClass(TcpServer);
};
}
#else
#error "Net::TcpServer not implemented on this platform!"
#endif
//------------------------------------------------------------------------------
