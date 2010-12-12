//------------------------------------------------------------------------------
//  shapenodeinstance.cc
//  (C) 2007 Radon Labs GmbH
//------------------------------------------------------------------------------
#include "stdneb.h"
#include "models/nodes/shapenodeinstance.h"
#include "coregraphics/renderdevice.h"
#include "models/nodes/transformnode.h"

namespace Models
{
__ImplementClass(Models::ShapeNodeInstance, 'SPNI', Models::StateNodeInstance);

using namespace CoreGraphics;

//------------------------------------------------------------------------------
/**
*/
ShapeNodeInstance::ShapeNodeInstance()
{
    // empty
}

//------------------------------------------------------------------------------
/**
*/
ShapeNodeInstance::~ShapeNodeInstance()
{
    // empty
}

//------------------------------------------------------------------------------
/**
*/
void
ShapeNodeInstance::OnVisibilityResolve(IndexT resolveIndex, float distanceToViewer)
{
    // check LOD distance and tell our model node that we are a visible instance
    const Ptr<TransformNode>& transformNode = this->modelNode.downcast<TransformNode>();
    if (transformNode->CheckLodDistance(distanceToViewer))
    {
        this->modelNode->AddVisibleNodeInstance(resolveIndex, this);
        StateNodeInstance::OnVisibilityResolve(resolveIndex, distanceToViewer);
    }
}

//------------------------------------------------------------------------------
/**
*/
void
ShapeNodeInstance::Render()
{
    StateNodeInstance::Render();
    RenderDevice::Instance()->Draw();
}    

} // namespace Models
