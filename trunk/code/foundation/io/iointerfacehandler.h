#pragma once
//------------------------------------------------------------------------------
/**
    @class IO::IoInterfaceHandler
    
    Handler class for io interfaces.
    
    (C) 2006 Radon Labs GmbH
*/
#include "interface/interfacehandlerbase.h"
#include "io/console.h"
#include "io/ioserver.h"
#include "io/iointerfaceprotocol.h"
#if __NEBULA3_HTTP_FILESYSTEM__
#include "http/httpclientregistry.h"
#endif

//------------------------------------------------------------------------------
namespace IO
{
class IoInterfaceHandler : public Interface::InterfaceHandlerBase
{
    __DeclareClass(IoInterfaceHandler);
public:
    /// constructor
    IoInterfaceHandler();
    /// destructor
    virtual ~IoInterfaceHandler();
    
    /// open the handler
    virtual void Open();
    /// close the handler
    virtual void Close();
    /// handle a message, return true if handled
    virtual bool HandleMessage(const Ptr<Messaging::Message>& msg);

protected:
    /// handle CreateDirectory message
    void OnCreateDirectory(const Ptr<IO::CreateDirectory>& msg);
    /// handle DeleteDirectory message
    void OnDeleteDirectory(const Ptr<IO::DeleteDirectory>& msg);
    /// handle DeleteFile message
    void OnDeleteFile(const Ptr<IO::DeleteFile>& msg);
    /// handle WriteStream message
    void OnWriteStream(const Ptr<IO::WriteStream>& msg);
    /// handle ReadStream message
    void OnReadStream(const Ptr<IO::ReadStream>& msg);
    /// handle CopyFile message
    void OnCopyFile(const Ptr<IO::CopyFile>& msg);
    /// handle MountArchive message
    void OnMountArchive(const Ptr<IO::MountArchive>& msg);

    Ptr<IO::IoServer> ioServer;
    #if __NEBULA3_HTTP_FILESYSTEM__
    Ptr<Http::HttpClientRegistry> httpClientRegistry;
    #endif
};

} // namespace IO
//------------------------------------------------------------------------------
    
    