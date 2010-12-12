#pragma once
//------------------------------------------------------------------------------
/**
    @class CoreGraphics::PrimitiveGroup
  
    Defines a group of primitives as a subset of a vertex buffer and index
    buffer plus the primitive topology (triangle list, etc...).
    
    (C) 2006 Radon Labs GmbH
*/    
#include "coregraphics/primitivetopology.h"
#include "math/bbox.h"

//------------------------------------------------------------------------------
namespace CoreGraphics
{
class PrimitiveGroup
{
public:
    /// constructor
    PrimitiveGroup();

    /// set base vertex index
    void SetBaseVertex(IndexT i);
    /// get index of base vertex
    IndexT GetBaseVertex() const;
    /// set number of vertices
    void SetNumVertices(SizeT n);
    ///get number of vertices
    SizeT GetNumVertices() const;
    /// set base index index
    void SetBaseIndex(IndexT i);
    /// get base index index
    IndexT GetBaseIndex() const;
    /// set number of indices
    void SetNumIndices(SizeT n);
    /// get number of indices
    SizeT GetNumIndices() const;
    /// set the primitive topology
    void SetPrimitiveTopology(PrimitiveTopology::Code topology);
    /// get the primitive topology
    PrimitiveTopology::Code GetPrimitiveTopology() const;
    /// set the primitive group's local bounding box
    void SetBoundingBox(const Math::bbox& b);
    /// get the primitive group's local bounding box
    const Math::bbox& GetBoundingBox() const;
    /// get computed number of primitives
    SizeT GetNumPrimitives() const;

private:
    IndexT baseVertex;
    SizeT numVertices;
    IndexT baseIndex;
    SizeT numIndices;
    PrimitiveTopology::Code primitiveTopology;
    Math::bbox boundingBox;
};

//------------------------------------------------------------------------------
/**
*/
inline
PrimitiveGroup::PrimitiveGroup() :
    baseVertex(0),
    numVertices(0),
    baseIndex(0),
    numIndices(0),
    primitiveTopology(PrimitiveTopology::InvalidPrimitiveTopology)
{
    // empty
}

//------------------------------------------------------------------------------
/**
*/
inline void
PrimitiveGroup::SetBaseVertex(IndexT i)
{
    this->baseVertex = i;
}

//------------------------------------------------------------------------------
/**
*/
inline IndexT
PrimitiveGroup::GetBaseVertex() const
{
    return this->baseVertex;
}

//------------------------------------------------------------------------------
/**
*/
inline void
PrimitiveGroup::SetNumVertices(SizeT n)
{
    this->numVertices = n;
}

//------------------------------------------------------------------------------
/**
*/
inline SizeT
PrimitiveGroup::GetNumVertices() const
{
    return this->numVertices;
}

//------------------------------------------------------------------------------
/**
*/
inline void
PrimitiveGroup::SetBaseIndex(IndexT i)
{
    this->baseIndex = i;
}

//------------------------------------------------------------------------------
/**
*/
inline IndexT
PrimitiveGroup::GetBaseIndex() const
{
    return this->baseIndex;
}

//------------------------------------------------------------------------------
/**
*/
inline void
PrimitiveGroup::SetNumIndices(SizeT n)
{
    this->numIndices = n;
}

//------------------------------------------------------------------------------
/**
*/
inline SizeT
PrimitiveGroup::GetNumIndices() const
{
    return this->numIndices;
}

//------------------------------------------------------------------------------
/**
*/
inline void
PrimitiveGroup::SetPrimitiveTopology(PrimitiveTopology::Code t)
{
    this->primitiveTopology = t;
}

//------------------------------------------------------------------------------
/**
*/
inline PrimitiveTopology::Code
PrimitiveGroup::GetPrimitiveTopology() const
{
    return this->primitiveTopology;
}

//------------------------------------------------------------------------------
/**
*/
inline void
PrimitiveGroup::SetBoundingBox(const Math::bbox& b)
{
    this->boundingBox = b;
}

//------------------------------------------------------------------------------
/**
*/
inline const Math::bbox&
PrimitiveGroup::GetBoundingBox() const
{
    return this->boundingBox;
}

//------------------------------------------------------------------------------
/**
*/
inline SizeT
PrimitiveGroup::GetNumPrimitives() const
{
    if (this->numIndices > 0)
    {
        return PrimitiveTopology::NumberOfPrimitives(this->primitiveTopology, this->numIndices);
    }
    else
    {
        return PrimitiveTopology::NumberOfPrimitives(this->primitiveTopology, this->numVertices);
    }
}

} // namespace PrimitiveGroup
//------------------------------------------------------------------------------

