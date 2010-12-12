//------------------------------------------------------------------------------
//  httpserverproxy.cc
//  (C) 2008 Radon Labs GmbH
//------------------------------------------------------------------------------
#include "stdneb.h"
#include "http/httpserverproxy.h"
#include "http/httpprotocol.h"
#include "http/httpinterface.h"

namespace Http
{
__ImplementClass(Http::HttpServerProxy, 'HTSP', Core::RefCounted);
__ImplementSingleton(Http::HttpServerProxy);

//------------------------------------------------------------------------------
/**
*/
HttpServerProxy::HttpServerProxy() :
    isOpen(false)
{
    __ConstructSingleton;
}

//------------------------------------------------------------------------------
/**
*/
HttpServerProxy::~HttpServerProxy()
{
    n_assert(!this->isOpen);
    __DestructSingleton;
}

//------------------------------------------------------------------------------
/**
*/
void
HttpServerProxy::Open()
{
    n_assert(!this->isOpen);
    n_assert(this->requestHandlers.IsEmpty());
    this->isOpen = true;
}

//------------------------------------------------------------------------------
/**
*/
void
HttpServerProxy::Close()
{
    n_assert(this->isOpen);
    
    // cleanup request handlers
    while (this->requestHandlers.Size() > 0)
    {
        this->RemoveRequestHandler(this->requestHandlers.Back());
    }
    this->isOpen = false;
}

//------------------------------------------------------------------------------
/**
*/
void
HttpServerProxy::AttachRequestHandler(const Ptr<HttpRequestHandler>& requestHandler)
{
    n_assert(this->isOpen);
    this->requestHandlers.Append(requestHandler);

    // register request handler with HttpServer thread 
    Ptr<Http::AttachRequestHandler> msg = Http::AttachRequestHandler::Create();
    msg->SetRequestHandler(requestHandler);
    HttpInterface::Instance()->Send(msg.cast<Messaging::Message>());
}

//------------------------------------------------------------------------------
/**
*/
void
HttpServerProxy::RemoveRequestHandler(const Ptr<HttpRequestHandler>& requestHandler)
{
    n_assert(this->isOpen);    
    IndexT index = this->requestHandlers.FindIndex(requestHandler);
    n_assert(InvalidIndex != index);
    
    // unregister request handler from HttpServer thread
    Ptr<Http::RemoveRequestHandler> msg = Http::RemoveRequestHandler::Create();
    msg->SetRequestHandler(requestHandler);
    HttpInterface::Instance()->Send(msg.cast<Messaging::Message>());

    // delete from local array
    this->requestHandlers.EraseIndex(index);
}

//------------------------------------------------------------------------------
/**
*/
void
HttpServerProxy::HandlePendingRequests()
{
    n_assert(this->isOpen);

    IndexT i;
    for (i = 0; i < this->requestHandlers.Size(); i++)
    {
        this->requestHandlers[i]->HandlePendingRequests();
    }
}

} // namespace Http