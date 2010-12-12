//------------------------------------------------------------------------------
//  core/coreserver.cc
//  (C) 2006 Radon Labs GmbH
//------------------------------------------------------------------------------
#include "stdneb.h"
#include "core/coreserver.h"

namespace Core
{
__ImplementClass(Core::CoreServer, 'CORS', Core::RefCounted);
__ImplementSingleton(Core::CoreServer);

using namespace IO;

//------------------------------------------------------------------------------
/**
*/
CoreServer::CoreServer() :
    companyName("Radon Labs GmbH"),
    appName("Nebula3"),
    rootDirectory("home:"),
    isOpen(false)
{
    __ConstructSingleton;
}

//------------------------------------------------------------------------------
/**
*/
CoreServer::~CoreServer()
{
    if (this->IsOpen())
    {
        this->Close();
    }
    __DestructSingleton;
}

//------------------------------------------------------------------------------
/**
*/
void
CoreServer::Open()
{
    n_assert(!this->IsOpen());
    this->isOpen = true;

    // create console object
    this->con = Console::Create();
    this->con->Open();
}

//------------------------------------------------------------------------------
/**
*/
void
CoreServer::Close()
{
    n_assert(this->IsOpen());

    // shutdown console
    this->con = 0;

    this->isOpen = false;
}

//------------------------------------------------------------------------------
/**
*/
void 
CoreServer::Trigger()
{
    this->con->Update();
}
} // namespace Core
