//------------------------------------------------------------------------------
//  Nvx2OrderFreeStreamReader.cc
//  (C) 2007 Radon Labs GmbH
//------------------------------------------------------------------------------
#include "stdneb.h"
#include "nvx2orderfreestreamreader.h"
#include "coregraphics/memoryvertexbufferloader.h"
#include "coregraphics/memoryindexbufferloader.h"

#if NEBULA3_LEGACY_SUPPORT
namespace Legacy
{
__ImplementClass(Legacy::Nvx2OrderFreeStreamReader, 'N2SR', IO::StreamReader);

using namespace CoreGraphics;
using namespace Util;
using namespace Math;
using namespace Resources;
using namespace System;

// file format structs
#pragma pack(push, 1)
struct Nvx2Header
{
    uint magic;
    uint numGroups;
    uint numVertices;
    uint vertexWidth;
    uint numIndices;
    uint numEdges;
    uint vertexComponentMask;
};
struct Nvx2Group
{
    uint firstVertex;
    uint numVertices;
    uint firstTriangle;
    uint numTriangles;
    uint firstEdge;
    uint numEdges;
};
#pragma pack(pop)

//------------------------------------------------------------------------------
/**
*/
Nvx2OrderFreeStreamReader::Nvx2OrderFreeStreamReader() :
    usage(Base::ResourceBase::UsageImmutable),
    access(Base::ResourceBase::AccessNone),
    rawMode(false),
    mapPtr(0),
    groupDataPtr(0),
    vertexDataPtr(0),
    indexDataPtr(0),
    groupDataSize(0),
    vertexDataSize(0),
    indexDataSize(0),
    numGroups(0),
    numVertices(0),
    vertexWidth(0),
    numIndices(0),
    numEdges(0),
    vertexComponentMask(0),
    sourceEndianess(System::ByteOrder::LittleEndian)
{
    // empty
}

//------------------------------------------------------------------------------
/**
*/
Nvx2OrderFreeStreamReader::~Nvx2OrderFreeStreamReader()
{
    if (this->IsOpen())
    {
        this->Close();
    }
}

//------------------------------------------------------------------------------
/**
*/
bool
Nvx2OrderFreeStreamReader::Open()
{
    n_assert(0 == this->primGroups.Size());
    n_assert(this->stream->CanBeMapped());
    n_assert(this->vertexComponents.IsEmpty());
    n_assert(0 == this->mapPtr);
    if (StreamReader::Open())
    {
        // map the stream to memory
        this->mapPtr = this->stream->Map();
        n_assert(0 != this->mapPtr);

        // read data
        this->ReadHeaderData();
        this->ReadPrimitiveGroups();
        this->SetupVertexComponents();
        if (!this->rawMode)
        {
            this->SetupVertexBuffer();
            this->SetupIndexBuffer();
            this->UpdateGroupBoundingBoxes();
        }
        return true;
    }
    return false;
}

//------------------------------------------------------------------------------
/**
*/
void
Nvx2OrderFreeStreamReader::Close()
{
    this->mapPtr = 0;
    this->groupDataPtr = 0;
    this->vertexDataPtr = 0;
    this->indexDataPtr = 0;
    this->vertexBuffer = 0;
    this->indexBuffer = 0;
    this->primGroups.Clear();
    this->vertexComponents.Clear();
    stream->Unmap();
    StreamReader::Close();
}

//------------------------------------------------------------------------------
/**
    This reads the nvx2 header data and checks whether the file is 
    actually an nvx2 file through the magic number. All header data
    will be read into member variables, and pointers to the
    start of the group-, vertex- and index-data will be setup.
*/
void
Nvx2OrderFreeStreamReader::ReadHeaderData()
{
    n_assert(0 != this->mapPtr);
    ByteOrder byteOrder(this->sourceEndianess, ByteOrder::Host);
    
    // endian-convert header
    struct Nvx2Header* header = (struct Nvx2Header*) this->mapPtr;
    byteOrder.ConvertInPlace<uint>(header->magic);
    byteOrder.ConvertInPlace<uint>(header->numGroups);
    byteOrder.ConvertInPlace<uint>(header->numVertices);
    byteOrder.ConvertInPlace<uint>(header->vertexWidth);
    byteOrder.ConvertInPlace<uint>(header->numIndices);
    header->numIndices *= 3; // header holds number of tris, not indices
    byteOrder.ConvertInPlace<uint>(header->numEdges);
    byteOrder.ConvertInPlace<uint>(header->vertexComponentMask);

    // check magic number
    if (FourCC(header->magic) != FourCC('NVX2'))
    {
        // not a nvx2 file, break hard
        n_error("Nvx2OrderFreeStreamReader: '%s' is not a nvx2 file!", this->stream->GetURI().AsString().AsCharPtr());
    }    
    this->numGroups = header->numGroups;
    this->numVertices = header->numVertices;
    this->vertexWidth = header->vertexWidth;
    this->numIndices = header->numIndices;
    this->numEdges = header->numEdges;
    this->vertexComponentMask = header->vertexComponentMask;
    this->groupDataSize = 6 * sizeof(uint) * this->numGroups;
    this->vertexDataSize = this->numVertices * this->vertexWidth * sizeof(float);
    this->indexDataSize = this->numIndices * sizeof(ushort);
    this->groupDataPtr = header + 1;
    this->vertexDataPtr = ((uchar*)this->groupDataPtr) + this->groupDataSize;
    this->indexDataPtr = ((uchar*)this->vertexDataPtr) + this->vertexDataSize;
}

//------------------------------------------------------------------------------
/**
*/
void
Nvx2OrderFreeStreamReader::ReadPrimitiveGroups()
{
    n_assert(this->primGroups.IsEmpty());
    n_assert(this->numGroups > 0);
    n_assert(0 != this->groupDataPtr);
    ByteOrder byteOrder(this->sourceEndianess, ByteOrder::Host);    
    Nvx2Group* group = (Nvx2Group*) this->groupDataPtr;
    IndexT i;
    for (i = 0; i < (SizeT)this->numGroups; i++)
    {
        // endian-conversion
        byteOrder.ConvertInPlace<uint>(group->firstVertex);
        byteOrder.ConvertInPlace<uint>(group->numVertices);
        byteOrder.ConvertInPlace<uint>(group->firstTriangle);
        byteOrder.ConvertInPlace<uint>(group->numTriangles);

        // setup a primitive group object
        PrimitiveGroup primGroup;
        primGroup.SetBaseVertex(group->firstVertex);
        primGroup.SetNumVertices(group->numVertices);
        primGroup.SetBaseIndex(group->firstTriangle * 3);
        primGroup.SetNumIndices(group->numTriangles * 3);
        primGroup.SetPrimitiveTopology(PrimitiveTopology::TriangleList);
        this->primGroups.Append(primGroup);

        // set top next group
        group++;
    }
}

//------------------------------------------------------------------------------
/**
*/
void
Nvx2OrderFreeStreamReader::SetupVertexComponents()
{
    n_assert(this->vertexComponents.IsEmpty());

    IndexT i;
    for (i = 0; i < N2NumVertexComponents; i++)
    {
        VertexComponent::SemanticName sem;
        VertexComponent::Format fmt;
        IndexT index = 0;
        if (vertexComponentMask & (1<<i))
        {
            switch (1<<i)
            {
                case N2Coord:        sem = VertexComponent::Position;     fmt = VertexComponent::Float3; break;
                case N2Normal:       sem = VertexComponent::Normal;       fmt = VertexComponent::Float3; break;
                case N2NormalUB4N:   sem = VertexComponent::Normal;       fmt = VertexComponent::UByte4N; break;
                case N2Uv0:          sem = VertexComponent::TexCoord;     fmt = VertexComponent::Float2; index = 0; break;
                case N2Uv0S2:        sem = VertexComponent::TexCoord;     fmt = VertexComponent::Short2; index = 0; break;
                case N2Uv1:          sem = VertexComponent::TexCoord;     fmt = VertexComponent::Float2; index = 1; break;
                case N2Uv1S2:        sem = VertexComponent::TexCoord;     fmt = VertexComponent::Short2; index = 1; break;
                case N2Uv2:          sem = VertexComponent::TexCoord;     fmt = VertexComponent::Float2; index = 2; break;
                case N2Uv2S2:        sem = VertexComponent::TexCoord;     fmt = VertexComponent::Short2; index = 2; break;
                case N2Uv3:          sem = VertexComponent::TexCoord;     fmt = VertexComponent::Float2; index = 3; break;
                case N2Uv3S2:        sem = VertexComponent::TexCoord;     fmt = VertexComponent::Short2; index = 3; break;
                case N2Color:        sem = VertexComponent::Color;        fmt = VertexComponent::Float4; break;
                case N2ColorUB4N:    sem = VertexComponent::Color;        fmt = VertexComponent::UByte4N; break;
                case N2Tangent:      sem = VertexComponent::Tangent;      fmt = VertexComponent::Float3; break;
                case N2TangentUB4N:  sem = VertexComponent::Tangent;      fmt = VertexComponent::UByte4N; break;
                case N2Binormal:     sem = VertexComponent::Binormal;     fmt = VertexComponent::Float3; break;
                case N2BinormalUB4N: sem = VertexComponent::Binormal;     fmt = VertexComponent::UByte4N; break;
                case N2Weights:      sem = VertexComponent::SkinWeights;  fmt = VertexComponent::Float4; break;
                case N2WeightsUB4N:  sem = VertexComponent::SkinWeights;  fmt = VertexComponent::UByte4N; break;
                case N2JIndices:     sem = VertexComponent::SkinJIndices; fmt = VertexComponent::Float4; break;
                case N2JIndicesUB4:  sem = VertexComponent::SkinJIndices; fmt = VertexComponent::UByte4; break;
                default:
                    n_error("Invalid Nebula2 VertexComponent in Nvx2OrderFreeStreamReader::SetupVertexComponents");
                    sem = VertexComponent::Position;
                    fmt = VertexComponent::Float3;
                    break;
            }
            this->vertexComponents.Append(VertexComponent(sem, index, fmt));
        }
    }
}

//------------------------------------------------------------------------------
/**
    Since nvx2 files don't contain any bounding box information
    we need to compute per-primitive-group bounding boxes
    manually by walking the triangle indices. This may be inefficient
    with large meshes.
*/
void
Nvx2OrderFreeStreamReader::UpdateGroupBoundingBoxes()
{
    n_assert(0 != this->vertexDataPtr);
    n_assert(0 != this->indexDataPtr);
    n_assert(this->primGroups.Size() > 0);

    float* vertexPtr = (float*) this->vertexDataPtr;
    ushort* indexPtr = (ushort*) this->indexDataPtr;
    IndexT groupIndex;
    for (groupIndex = 0; groupIndex < this->primGroups.Size(); groupIndex++)
    {
        PrimitiveGroup& group = this->primGroups[groupIndex];        
        bbox box;
        box.begin_extend();
        point p;
        IndexT ii;
        for (ii = 0; ii < group.GetNumIndices(); ii++)
        {
            float* curVertexPtr = vertexPtr + (indexPtr[ii + group.GetBaseIndex()] * this->vertexWidth);
            p.set(curVertexPtr[0], curVertexPtr[1], curVertexPtr[2]);
            box.extend(p);
        }
        group.SetBoundingBox(box);
    }
}

//------------------------------------------------------------------------------
/**
*/
void
Nvx2OrderFreeStreamReader::SetupVertexBuffer()
{
    n_assert(!this->rawMode);
    n_assert(0 != this->vertexDataPtr);
    n_assert(this->vertexDataSize > 0);
    n_assert(this->numVertices > 0);    
    n_assert(this->vertexComponents.Size() > 0);

    // endian convert the vertex buffer
    #pragma warning(push)   
    #pragma warning(disable : 4127)  // expression is constant
    if (ByteOrder::Host != this->sourceEndianess)
    {
        this->ConvertVertexBufferEndianess(this->vertexDataPtr, this->numVertices, this->vertexComponents);
    }
    #pragma warning(pop)

    // setup new vertex buffer
    if (!this->vertexBuffer.isvalid())
    {
        this->vertexBuffer = VertexBuffer::Create();
    }
    if (!this->vertexBufferLoader.isvalid())
    {
        this->vertexBufferLoader = MemoryVertexBufferLoader::Create();
    }
    this->vertexBufferLoader->Setup(this->vertexComponents, this->numVertices, this->vertexDataPtr, 
                                    this->vertexDataSize, this->usage, this->access);
    this->vertexBuffer->SetLoader(this->vertexBufferLoader.upcast<ResourceLoader>());
    this->vertexBuffer->Load();
    this->vertexBuffer->SetLoader(0);
    this->vertexBufferLoader = 0;
    n_assert(this->vertexBuffer->GetState() == VertexBuffer::Loaded);
}

//------------------------------------------------------------------------------
/**
*/
void
Nvx2OrderFreeStreamReader::ConvertVertexBufferEndianess(void* vertexPtr, SizeT numVertices, const Util::Array<VertexComponent>& vertexComps)
{
    ByteOrder byteOrder(this->sourceEndianess, ByteOrder::Host);
    IndexT vertexIndex;
    IndexT compIndex;
    char* ptr = (char*) vertexPtr;
    for (vertexIndex = 0; vertexIndex < numVertices; vertexIndex++)
    {      
        for (compIndex = 0; compIndex < vertexComps.Size(); compIndex++)
        {   
            VertexComponent::Format format = vertexComps[compIndex].GetFormat();    
            switch(format)
            {
                case VertexComponent::Float:
                    byteOrder.ConvertInPlace<float>(((float*)ptr)[0]);
                    break;

                case VertexComponent::Float2:
                    byteOrder.ConvertInPlace<float>(((float*)ptr)[0]);
                    byteOrder.ConvertInPlace<float>(((float*)ptr)[1]);
                    break;

                case VertexComponent::Float3:
                    byteOrder.ConvertInPlace<float>(((float*)ptr)[0]);
                    byteOrder.ConvertInPlace<float>(((float*)ptr)[1]);
                    byteOrder.ConvertInPlace<float>(((float*)ptr)[2]);
                    break;

                case VertexComponent::Float4:
                    byteOrder.ConvertInPlace<float>(((float*)ptr)[0]);
                    byteOrder.ConvertInPlace<float>(((float*)ptr)[1]);
                    byteOrder.ConvertInPlace<float>(((float*)ptr)[2]);
                    byteOrder.ConvertInPlace<float>(((float*)ptr)[3]);
                    break;

                case VertexComponent::Short2:
                case VertexComponent::Short2N:
                    byteOrder.ConvertInPlace<short>(((short*)ptr)[0]);
                    byteOrder.ConvertInPlace<short>(((short*)ptr)[1]);
                    break;

                case VertexComponent::Short4:
                case VertexComponent::Short4N:
                    byteOrder.ConvertInPlace<short>(((short*)ptr)[0]);
                    byteOrder.ConvertInPlace<short>(((short*)ptr)[1]);
                    byteOrder.ConvertInPlace<short>(((short*)ptr)[2]);
                    byteOrder.ConvertInPlace<short>(((short*)ptr)[3]);
                    break;

                case VertexComponent::UByte4:
                case VertexComponent::UByte4N:
                    byteOrder.ConvertInPlace<uint>(((uint*)ptr)[0]);
                    break;

                default:
                    n_error("Invalid VertexComponent Format!");
            }
            ptr += vertexComps[compIndex].GetByteSize();
        }
    }    
}

//------------------------------------------------------------------------------
/**
*/
void
Nvx2OrderFreeStreamReader::SetupIndexBuffer()
{
    n_assert(!this->rawMode);
    n_assert(0 != this->indexDataPtr);
    n_assert(this->indexDataSize > 0);
    n_assert(this->numIndices > 0);
    
    // endian convert the index buffer
    #pragma warning(push)   
    #pragma warning(disable : 4127) // expression is constant
    if (ByteOrder::Host != this->sourceEndianess)
    {
        ByteOrder byteOrder(this->sourceEndianess, ByteOrder::Host);
        IndexT i;
        ushort* ptr = (ushort*) this->indexDataPtr;
        for (i = 0; i < (SizeT)this->numIndices; i++)
        {
            byteOrder.ConvertInPlace<ushort>(ptr[i]);
        }
    }
    #pragma warning(pop)

    // setup a new index buffer
    if (!this->indexBuffer.isvalid())
    {
        this->indexBuffer = IndexBuffer::Create();
    }
    if (!this->indexBufferLoader.isvalid())
    {
        this->indexBufferLoader = MemoryIndexBufferLoader::Create();
    }
    this->indexBufferLoader->Setup(IndexType::Index16, this->numIndices, this->indexDataPtr, 
                                   this->indexDataSize, this->usage, this->access);
    this->indexBuffer->SetLoader(this->indexBufferLoader.upcast<ResourceLoader>());
    this->indexBuffer->Load();
    this->indexBuffer->SetLoader(0);
    this->indexBufferLoader = 0;
    n_assert(this->indexBuffer->GetState() == IndexBuffer::Loaded);
}
} // namespace Legacy

#endif // NEBULA3_LEGACY_SUPPORT
