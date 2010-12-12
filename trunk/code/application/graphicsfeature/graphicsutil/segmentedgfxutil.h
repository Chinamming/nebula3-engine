#pragma once
//------------------------------------------------------------------------------
/**
    @class GraphicsFeature::SegmentedGfxUtil

    Check if a Nebula3 graphics resource consists of hierarchy nodes below
    the toplevel node, and if yes, create one graphics entity for each
    hierarchy node. Otherwise create just a single graphics entity. This
    segmentation helps in visibility culling large environmental objects.

    (C) 2007 Radon Labs GmbH
*/
#include "graphics/modelentity.h"

//------------------------------------------------------------------------------
namespace GraphicsFeature
{
class SegmentedGfxUtil
{
public:
    /// constructor
    SegmentedGfxUtil();
    /// create and setup one or more graphics entities from hierarchy nodes
    Util::Array<Ptr<Graphics::ModelEntity> > CreateAndSetupGraphicsEntities(const Util::String& resName, const Math::matrix44& worldMatrix, const Ptr<Graphics::Stage> stage = 0);
};

}; // namespace GraphicsFeature
//------------------------------------------------------------------------------
