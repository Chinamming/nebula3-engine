#ifndef VEGETATIONFEATURE_GRASSHEIGHTMAP_H
#define VEGETATIONFEATURE_GRASSHEIGHTMAP_H
//------------------------------------------------------------------------------
/**
    @class Vegetation::GrassHeightMap
    
    The grass height map provides fine-grained height information for a single
    grass patch. This is used by the grass patch to fix the Y-position of the
    vertices in the patch. The height information is created using an
    offscreen renderer which renders into a float texture, this is is then
    copied into system RAM for faster random access. 
    
    PERFORMANCE NOTE: with shader model 3.0 it would be possible
    to lookup the height map directly in the vertex shader, however this
    would have to happen every frame, so this may not be faster then
    the on-demand update.

    (C) 2008 Radon Labs GmbH
*/
#include "core/refcounted.h"
#include "vegetation/grasspatchcoords.h"
#include "internalgraphics/internalmodelentity.h"
#include "internalgraphics/internalview.h"
#include "coregraphics/texture.h"

//------------------------------------------------------------------------------
namespace Vegetation
{
class GrassHeightMap : public Core::RefCounted
{
    __DeclareClass(GrassHeightMap);
public:
    /// constructor
    GrassHeightMap();
    /// destructor
    virtual ~GrassHeightMap();

    /// setup the height map
    void Setup();
    /// begin lookup up heights
    bool Begin(const GrassPatchCoords& patchCoords, const Math::vector& cameraPos);
    /// compute surface index by model x/z-coordinates
    uint ComputeLookupIndex(float x, float z) const;
    /// lookup a height value by precomputed surface index
    float LookupHeight(uint index) const;
    /// end looking up heights
    void End();

private:
    /// update the height map
    void Update(const GrassPatchCoords& patchCoord, const Math::vector& cameraPos);
    /// convert viewer relative rendered heights to global space
    void ConvertHeightMapToGlobalSpace(float viewerPosY, float nearPlane, float farPlane);

    const int sizeInPixels;
    bool inBegin;
    Ptr<InternalGraphics::InternalView> view;
    Ptr<CoreGraphics::Texture> sysMemSurface;       // system ram buffer for fast CPU read access 
    CoreGraphics::Texture::MapInfo lockInfo;
    Ptr<InternalGraphics::InternalStage> stage;
    float lookupScale;
    int lookupMax;
    int lookupAdd;
    uint lookupSurfPitch;
    uint frameCounter;
    Ptr<InternalGraphics::InternalCameraEntity> cam;
};

//------------------------------------------------------------------------------
/**
    Compute the index into the height map by model space x/z coordinate.
    This is called by the class GrassPatch to precompute indices into the
    heigth map.
*/
inline
uint
GrassHeightMap::ComputeLookupIndex(float modelPosX, float modelPosZ) const
{
    int x = int(modelPosX * this->lookupScale) + this->lookupAdd;
    int y = int(modelPosZ * this->lookupScale) + this->lookupAdd;
    if (x < 0) x = 0;
    else if (x > this->lookupMax) x = this->lookupMax;
    if (y < 0) y = 0;
    else if (y > this->lookupMax) y = this->lookupMax;
    return (x * sizeof(float)) + y * this->lookupSurfPitch;
}

//------------------------------------------------------------------------------
/**
    Lookup a height value by a precomputed index generated by
    ComputeLookupIndex() from model coordinates.
*/
inline
float
GrassHeightMap::LookupHeight(uint index) const
{
    // compute pointer and return value
    float* ptr = (float*)(((uchar*)this->lockInfo.data) + index);
    return *ptr;
}

} // namespace GrassHeightMap
//------------------------------------------------------------------------------
#endif