#pragma once
#ifndef DEBUG_THREADPAGEHANDLER_H
#define DEBUG_THREADPAGEHANDLER_H
//------------------------------------------------------------------------------
/**
    @class Debug::ThreadPageHandler
    
    Displays info about currently running Nebula3 threads.
    
    (C) 2008 Radon Labs GmbH
*/
#include "http/httprequesthandler.h"

//------------------------------------------------------------------------------
namespace Debug
{
class ThreadPageHandler : public Http::HttpRequestHandler
{
    __DeclareClass(ThreadPageHandler);
public:
    /// constructor
    ThreadPageHandler();
    /// handle a http request, the handler is expected to fill the content stream with response data
    virtual void HandleRequest(const Ptr<Http::HttpRequest>& request);        
};

} // namespace Debug
//------------------------------------------------------------------------------
#endif
