#pragma once
//------------------------------------------------------------------------------
/**
    @class Core::CoreServer
    
    The central core server object initializes a minimal Nebula3 runtime
    environment necessary to boot up the rest of Nebula3. It should be the
    first object a Nebula3 application creates, and the last to destroy
    before shutdown.
    
    (C) 2006 Radon Labs GmbH
*/    
#include "core/ptr.h"
#include "core/refcounted.h"
#include "core/singleton.h"
#include "io/console.h"
#include "util/stringatom.h"

//------------------------------------------------------------------------------
namespace Core
{
class CoreServer : public RefCounted
{
    __DeclareClass(CoreServer);
    __DeclareSingleton(CoreServer);
public:
    /// constructor
    CoreServer();
    /// destructor
    virtual ~CoreServer();
    /// set the company name
    void SetCompanyName(const Util::StringAtom& s);
    /// get the company name
    const Util::StringAtom& GetCompanyName() const;
    /// set the application name
    void SetAppName(const Util::StringAtom& s);
    /// get the application name
    const Util::StringAtom& GetAppName() const;
    /// set the root directory of the application (default is "home:")
    void SetRootDirectory(const Util::StringAtom& s);
    /// get the root directory of the application
    const Util::StringAtom& GetRootDirectory() const;

    /// open the core server
    void Open();
    /// close the core server
    void Close();
    /// return true if currently open
    bool IsOpen() const;
    /// trigger core server, updates console
    void Trigger();

private:
    Ptr<IO::Console> con;
    Util::StringAtom companyName;
    Util::StringAtom appName;
    Util::StringAtom rootDirectory;
    bool isOpen;
};

//------------------------------------------------------------------------------
/*
*/
inline bool
CoreServer::IsOpen() const
{
    return this->isOpen;
}

//------------------------------------------------------------------------------
/*
*/
inline void
CoreServer::SetCompanyName(const Util::StringAtom& s)
{
    this->companyName = s;
}

//------------------------------------------------------------------------------
/*
*/
inline const Util::StringAtom&
CoreServer::GetCompanyName() const
{
    return this->companyName;
}

//------------------------------------------------------------------------------
/*
*/
inline void
CoreServer::SetAppName(const Util::StringAtom& s)
{
    this->appName = s;
}

//------------------------------------------------------------------------------
/*
*/
inline const Util::StringAtom&
CoreServer::GetAppName() const
{
    return this->appName;
}

//------------------------------------------------------------------------------
/*
*/
inline void
CoreServer::SetRootDirectory(const Util::StringAtom& s)
{
    this->rootDirectory = s;
}

//------------------------------------------------------------------------------
/*
*/
inline const Util::StringAtom&
CoreServer::GetRootDirectory() const
{
    return this->rootDirectory;
}

} // namespace Core
//------------------------------------------------------------------------------
