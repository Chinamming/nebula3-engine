#pragma once
//------------------------------------------------------------------------------
/**
    @class Net::IpAddress

    Represents an IP address, consisting of a IPv4 host address
    and a port number. Can extract address information from an URI and    
    automatically converts host names to addresses, and offers the special 
    hostnames "localhost", "any", "broadcast", "self" and "inetself" where:

    - "localhost" will translate to 127.0.0.1
    - "any" will translate to INADDR_ANY, which is 0.0.0.0
    - "broadcast" will translate to INADDR_BROADCAST, which is 255.255.255.255
    - "self" will translate to the first valid tcp/ip address for this host
      (there may be more then one address bound to the host)
    - "inetself" will translate to the first host address which is not 
      a LAN address (which is not a class A, B, or C network) if none such
      exists the address will fall back to "self"

    (C) 2006 Radon Labs GmbH
*/
#if (__WIN32__ || __XBOX360__)
#include "net/win360/win360ipaddress.h"
namespace Net
{
typedef Win360::Win360IpAddress IpAddress;
}
#elif __WII__
// class is just an empty stub on Wii
#include "net/wii/wiiipaddress.h"
namespace Net
{
typedef Wii::WiiIpAddress IpAddress;
}
#elif __PS3__
#include "net/ps3/ps3ipaddress.h"
namespace Net
{
typedef PS3::PS3IpAddress IpAddress;
}
#else
#error "IpAddress class not implemented on this platform"
#endif
//------------------------------------------------------------------------------
