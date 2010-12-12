#pragma once
//------------------------------------------------------------------------------
/**
    @class Debug::DebugShapeRenderer
    
    Client-side proxy for rendering debug shapes. Packs shape render requests
    into a message which is sent to the render thread once per frame.
    
    (C) 2008 Radon Labs GmbH
*/
#include "core/refcounted.h"
#include "core/singleton.h"
#include "math/matrix44.h"
#include "math/float4.h"
#include "coregraphics/primitivetopology.h"
#include "coregraphics/indextype.h"
#include "coregraphics/shaperenderer.h"
#include "io/memorystream.h"

//------------------------------------------------------------------------------
namespace Debug
{
class DebugShapeRenderer : public Core::RefCounted
{
    __DeclareClass(DebugShapeRenderer);
    __DeclareSingleton(DebugShapeRenderer);
public:
    /// constructor
    DebugShapeRenderer();
    /// destructor
    virtual ~DebugShapeRenderer();
    /// draw a box
    void DrawBox(const Math::matrix44& modelTransform, const Math::float4& color);
    /// draw a sphere
    void DrawSphere(const Math::matrix44& modelTransform, const Math::float4& color);
    /// draw a cylinder
    void DrawCylinder(const Math::matrix44& modelTransform, const Math::float4& color);
    /// draw a torus
    void DrawTorus(const Math::matrix44& modelTransform, const Math::float4& color);
    /// draw primitives
    void DrawPrimitives(const Math::matrix44& modelTransform, CoreGraphics::PrimitiveTopology::Code topology, SizeT numPrimitives, const void* vertices, SizeT vertexWidth, const Math::float4& color);
    /// draw indexed primitives
    void DrawIndexedPrimitives(const Math::matrix44& modelTransform, CoreGraphics::PrimitiveTopology::Code topology, SizeT numPrimitives, const void* vertices, SizeT numVertices, SizeT vertexWidth, const void* indices, CoreGraphics::IndexType::Code indexType, const Math::float4& color);
    /// call once per frame to send of accumulated draw commands
    void OnFrame();

private:
    Util::Array<CoreGraphics::RenderShape> shapes;
}; 

} // namespace Debug
//------------------------------------------------------------------------------
    
    