#pragma once
//------------------------------------------------------------------------------
/**
    @class ToolkitUtil::MeshBuilder
    
    A mesh builder utility class. Useful for exporter and converter tools.
    
    (C) 2009 Radon Labs GmbH
*/
#include "toolkitutil/meshutil/meshbuildervertex.h"
#include "toolkitutil/meshutil/meshbuildertriangle.h"
#include "toolkitutil/meshutil/meshbuildergroup.h"
#include "util/fixedarray.h"
#include "util/array.h"
#include "math/bbox.h"

//------------------------------------------------------------------------------
namespace ToolkitUtil
{
class MeshBuilder
{
public:
    /// constructor
    MeshBuilder();
    
    /// reserve dynamic arrays
    void Reserve(SizeT numVertices, SizeT numTriangles);

    /// add a vertex
    void AddVertex(const MeshBuilderVertex& v);
    /// get number of vertices
    SizeT GetNumVertices() const;
    /// get vertex at index
    MeshBuilderVertex& VertexAt(IndexT i) const;

    /// add a triangle
    void AddTriangle(const MeshBuilderTriangle& t);
    /// get number of triangles
    SizeT GetNumTriangles() const;
    /// get triangle at index
    MeshBuilderTriangle& TriangleAt(IndexT i) const;

    /// get number of triangles of given group id starting at triangle index
    SizeT CountGroupTriangles(IndexT groupId, IndexT startTriangleIndex) const;
    /// find the min/max vertex range for a given group id
    bool FindGroupVertexRange(IndexT groupId, IndexT& outMinVertexIndex, IndexT& outMaxVertexIndex) const;
    /// copy triangle with its vertices, do not generate redundant vertices
    void CopyTriangle(const MeshBuilder& srcMesh, IndexT triIndex, Util::FixedArray<IndexT>& indexMap);
    /// compute bounding box for a given vertex group
    Math::bbox ComputeGroupBoundingBox(IndexT groupId) const;
    /// compute overall bounding box
    Math::bbox ComputeBoundingBox() const;

    /// build group mapping array
    void BuildGroupMap(Util::Array<MeshBuilderGroup>& groupMap);
    /// sort triangles by group id
    void SortTriangles();
    /// clear object (deletes internal arrays)
    void Clear();
    /// erase or create vertex components
    void ForceVertexComponents(MeshBuilderVertex::ComponentMask compMask);
    /// make sure all vertices have the same vertex components
    void ExtendVertexComponents();
    /// transform vertices
    void Transform(const Math::matrix44& m);
    /// remove redundant vertices
    void Deflate(Util::FixedArray<Util::Array<IndexT>>* collapsMap);
    /// inflate mesh to 3 unique vertices per triangles, created redundant vertices
    void Inflate();
    /// flip v texture coordinates
    void FlipUvs();
    /// move uv coords of given triangle into range (requires unflattened mesh!)
    bool MoveTriangleUvsIntoRange(IndexT triIndex, float minUv, float maxUv);

private:
    /// a qsort() hook for generating a sorted index array
    static int __cdecl VertexSorter(const void* elm0, const void* elm1);

    Util::Array<MeshBuilderVertex> vertexArray;
    Util::Array<MeshBuilderTriangle> triangleArray;
    static MeshBuilder* qsortData;
};

//------------------------------------------------------------------------------
/**
*/
inline void
MeshBuilder::AddVertex(const MeshBuilderVertex& v)
{
    this->vertexArray.Append(v);
}

//------------------------------------------------------------------------------
/**
*/
inline SizeT
MeshBuilder::GetNumVertices() const
{
    return this->vertexArray.Size();
}

//------------------------------------------------------------------------------
/**
*/
inline MeshBuilderVertex&
MeshBuilder::VertexAt(IndexT i) const
{
    return this->vertexArray[i];
}

//------------------------------------------------------------------------------
/**
*/
inline void
MeshBuilder::AddTriangle(const MeshBuilderTriangle& t)
{
    this->triangleArray.Append(t);
}

//------------------------------------------------------------------------------
/**
*/
inline SizeT
MeshBuilder::GetNumTriangles() const
{
    return this->triangleArray.Size();
}

//------------------------------------------------------------------------------
/**
*/
inline MeshBuilderTriangle&
MeshBuilder::TriangleAt(IndexT i) const
{
    return this->triangleArray[i];
}

} // namespace ToolkitUtil
//------------------------------------------------------------------------------
    