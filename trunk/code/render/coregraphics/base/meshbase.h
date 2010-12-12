#pragma once
//------------------------------------------------------------------------------
/**
    @class Base::MeshBase
  
    A mesh maintains a vertex buffer, an optional index buffer
    and a number of PrimitiveGroup objects. Meshes can be loaded directly 
    from a mesh resource file.
    
    (C) 2007 Radon Labs GmbH
*/    
#include "resources/resource.h"
#include "coregraphics/vertexbuffer.h"
#include "coregraphics/indexbuffer.h"
#include "coregraphics/primitivegroup.h"

//------------------------------------------------------------------------------
namespace Base
{
class MeshBase : public Resources::Resource
{
    __DeclareClass(MeshBase);
public:
    /// constructor
    MeshBase();
    /// destructor
    virtual ~MeshBase();

    /// unload mesh resource
    virtual void Unload();
    
    /// return true if the mesh has a vertex buffer
    bool HasVertexBuffer() const;
    /// set the vertex buffer object
    void SetVertexBuffer(const Ptr<CoreGraphics::VertexBuffer>& vb);
    /// get the vertex buffer object
    const Ptr<CoreGraphics::VertexBuffer>& GetVertexBuffer() const;
    /// return true if the mesh has an index buffer
    bool HasIndexBuffer() const;
    /// set the index buffer object
    void SetIndexBuffer(const Ptr<CoreGraphics::IndexBuffer>& ib);
    /// get the index buffer object
    const Ptr<CoreGraphics::IndexBuffer>& GetIndexBuffer() const;
    /// set primitive groups
    void SetPrimitiveGroups(const Util::Array<CoreGraphics::PrimitiveGroup>& groups);
    /// get the number of primitive groups in the mesh
    SizeT GetNumPrimitiveGroups() const;
    /// get primitive group at index
    const CoreGraphics::PrimitiveGroup& GetPrimitiveGroupAtIndex(IndexT i) const;    

    /// apply mesh data for rendering in renderdevice
    void ApplyPrimitives(IndexT primGroupIndex);
 
protected:   
    Ptr<CoreGraphics::VertexBuffer> vertexBuffer;
    Ptr<CoreGraphics::IndexBuffer> indexBuffer;
    Util::Array<CoreGraphics::PrimitiveGroup> primitiveGroups;
};

//------------------------------------------------------------------------------
/**
*/
inline bool
MeshBase::HasVertexBuffer() const
{
    return this->vertexBuffer.isvalid();
}

//------------------------------------------------------------------------------
/**
*/
inline void
MeshBase::SetVertexBuffer(const Ptr<CoreGraphics::VertexBuffer>& vb)
{
    this->vertexBuffer = vb;
}

//------------------------------------------------------------------------------
/**
*/
inline const Ptr<CoreGraphics::VertexBuffer>&
MeshBase::GetVertexBuffer() const
{
    return this->vertexBuffer;
}

//------------------------------------------------------------------------------
/**
*/
inline bool
MeshBase::HasIndexBuffer() const
{
    return this->indexBuffer.isvalid();
}

//------------------------------------------------------------------------------
/**
*/
inline void
MeshBase::SetIndexBuffer(const Ptr<CoreGraphics::IndexBuffer>& ib)
{
    this->indexBuffer = ib;
}

//------------------------------------------------------------------------------
/**
*/
inline const Ptr<CoreGraphics::IndexBuffer>&
MeshBase::GetIndexBuffer() const
{
    return this->indexBuffer;
}

//------------------------------------------------------------------------------
/**
*/
inline void
MeshBase::SetPrimitiveGroups(const Util::Array<CoreGraphics::PrimitiveGroup>& groups)
{
    this->primitiveGroups = groups;
}

//------------------------------------------------------------------------------
/**
*/
inline SizeT
MeshBase::GetNumPrimitiveGroups() const
{
    return this->primitiveGroups.Size();
}

//------------------------------------------------------------------------------
/**
*/
inline const CoreGraphics::PrimitiveGroup&
MeshBase::GetPrimitiveGroupAtIndex(IndexT i) const
{
    return this->primitiveGroups[i];
}
    
} // namespace Base
//------------------------------------------------------------------------------


