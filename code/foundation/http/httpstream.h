#pragma once
//------------------------------------------------------------------------------
/**
    @class Http::HttpStream
  
    Wraps client HTTP requests to a HTTP server into an IO::Stream.
    
    (C) 2009 Radon Labs GmbH
*/    
#include "core/config.h"

// HttpStream not implemented on Wii
#if __NEBULA3_HTTP_FILESYSTEM__
#include "io/memorystream.h"
#include "http/httpclient.h"

//------------------------------------------------------------------------------
namespace Http
{
class HttpStream : public IO::MemoryStream
{
    __DeclareClass(HttpStream);
public:
    /// constructor
    HttpStream();
    /// destructor
    virtual ~HttpStream();
    
    /// open the stream
    virtual bool Open();
};

} // namespace Http
//------------------------------------------------------------------------------
#endif // __WII__