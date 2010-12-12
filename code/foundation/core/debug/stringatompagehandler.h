#pragma once
//------------------------------------------------------------------------------
/**
    @class Debug::CorePageHandler
  
    Display information about the global string atom table.
    
    (C) 2009 Radon Labs GmbH
*/
#include "http/httprequesthandler.h"

//------------------------------------------------------------------------------
namespace Debug
{
class StringAtomPageHandler : public Http::HttpRequestHandler
{
    __DeclareClass(StringAtomPageHandler);
public:
    /// constructor
    StringAtomPageHandler();
    /// handle a http request, the handler is expected to fill the content stream with response data
    virtual void HandleRequest(const Ptr<Http::HttpRequest>& request);        
};

} // namespace Debug
//------------------------------------------------------------------------------
