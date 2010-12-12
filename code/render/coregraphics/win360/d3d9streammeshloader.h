#pragma once
//------------------------------------------------------------------------------
/**
    @class Win360::D3D9StreamMeshLoader
    
    Setup a Mesh object from a stream. Supports the following file formats:
    
    - nvx2 (Nebula2 binary mesh file format)
    - nvx3 (Nebula3 binary mesh file format)
    - n3d3 (Nebula3 ascii mesh file format)
    
    @todo: document file formats
    
    (C) 2007 Radon Labs GmbH
*/
#include "resources/streamresourceloader.h"
#include "coregraphics/base/resourcebase.h"

//------------------------------------------------------------------------------
namespace Win360
{
class D3D9StreamMeshLoader : public Resources::StreamResourceLoader
{
    __DeclareClass(D3D9StreamMeshLoader);
public:
    /// constructor
    D3D9StreamMeshLoader();
    /// set the intended resource usage (default is UsageImmutable)
    void SetUsage(Base::ResourceBase::Usage usage);
    /// get resource usage
    Base::ResourceBase::Usage GetUsage() const;
    /// set the intended resource access (default is AccessNone)
    void SetAccess(Base::ResourceBase::Access access);
    /// get the resource access
    Base::ResourceBase::Access GetAccess() const;

private:
    /// setup mesh from generic stream, branches to specialized loader methods
    virtual bool SetupResourceFromStream(const Ptr<IO::Stream>& stream);
    #if NEBULA3_LEGACY_SUPPORT
    /// setup mesh from nvx2 file in memory
    bool SetupMeshFromNvx2(const Ptr<IO::Stream>& stream);
    #endif
    /// setup mesh from nvx3 file in memory
    bool SetupMeshFromNvx3(const Ptr<IO::Stream>& stream);
    /// setup mesh from n3d3 file in memory
    bool SetupMeshFromN3d3(const Ptr<IO::Stream>& stream);

protected:
    Base::ResourceBase::Usage usage;
    Base::ResourceBase::Access access;
};

//------------------------------------------------------------------------------
/**
*/
inline void
D3D9StreamMeshLoader::SetUsage(Base::ResourceBase::Usage usage_)
{
    this->usage = usage_;
}

//------------------------------------------------------------------------------
/**
*/
inline Base::ResourceBase::Usage
D3D9StreamMeshLoader::GetUsage() const
{
    return this->usage;
}

//------------------------------------------------------------------------------
/**
*/
inline void
D3D9StreamMeshLoader::SetAccess(Base::ResourceBase::Access access_)
{
    this->access = access_;
}

//------------------------------------------------------------------------------
/**
*/
inline Base::ResourceBase::Access
D3D9StreamMeshLoader::GetAccess() const
{
    return this->access;
}

} // namespace Win360
//------------------------------------------------------------------------------
