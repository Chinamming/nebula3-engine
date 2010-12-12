#pragma once
#ifndef RENDERUTIL_MAYACAMERAUTIL_H
#define RENDERUTIL_MAYACAMERAUTIL_H
//------------------------------------------------------------------------------
/**
    @class RenderUtil::MayaCameraUtil

    Helper class to implement a "Maya camera" with pan/zoom/orbit. Just feed
    input into the class per its setter methods, call Update(), and get 
    the computed view matrix.

    (C) 2007 Radon Labs GmbH
*/
#include "math/matrix44.h"
#include "math/float2.h"
#include "math/point.h"
#include "math/polar.h"
#include "math/scalar.h"

//------------------------------------------------------------------------------
namespace RenderUtil
{
class MayaCameraUtil
{
public:
    /// constructor
    MayaCameraUtil();

    /// setup the object
    void Setup(const Math::point& defaultCenterOfInterest, const Math::point& defaultEyePos, const Math::vector& defaultUpVec);
    /// reset the object to its default settings
    void Reset();
    /// update the view matrix
    void Update();
    /// get the current camera transform
    const Math::matrix44& GetCameraTransform() const;

    /// set state of orbit button
    void SetOrbitButton(bool b);
    /// set state of panning button
    void SetPanButton(bool b);
    /// set state of zoom button
    void SetZoomButton(bool b);
    /// set state of zoom-in button
    void SetZoomInButton(bool b);
    /// set state of zoom-out button
    void SetZoomOutButton(bool b);
    /// set mouse movement
    void SetMouseMovement(const Math::float2& v);
    /// set zoom-in value
    void SetZoomIn(float v);
    /// set zoom-out value
    void SetZoomOut(float v);
    /// set panning vector
    void SetPanning(const Math::float2& v);
    /// set orbiting vector
    void SetOrbiting(const Math::float2& v);

private:
    Math::point defaultCenterOfInterest;
    Math::point defaultEyePos;
    Math::vector defaultUpVec;

    Math::polar viewAngles;
    Math::scalar viewDistance;
    Math::point centerOfInterest;

    Math::matrix44 cameraTransform;

    bool orbitButton;
    bool panButton;
    bool zoomButton;
    bool zoomInButton;
    bool zoomOutButton;
    Math::float2 mouseMovement;
    float zoomIn;
    float zoomOut;
    Math::float2 panning;
    Math::float2 orbiting;
};

//------------------------------------------------------------------------------
/**
*/
inline void
MayaCameraUtil::SetOrbitButton(bool b)
{
    this->orbitButton = b;
}

//------------------------------------------------------------------------------
/**
*/
inline void
MayaCameraUtil::SetPanButton(bool b)
{
    this->panButton = b;
}

//------------------------------------------------------------------------------
/**
*/
inline void
MayaCameraUtil::SetZoomButton(bool b)
{
    this->zoomButton = b;
}

//------------------------------------------------------------------------------
/**
*/
inline void
MayaCameraUtil::SetZoomInButton(bool b)
{
    this->zoomInButton = b;
}

//------------------------------------------------------------------------------
/**
*/
inline void
MayaCameraUtil::SetZoomOutButton(bool b)
{
    this->zoomOutButton = b;
}

//------------------------------------------------------------------------------
/**
*/
inline void
MayaCameraUtil::SetMouseMovement(const Math::float2& v)
{
    this->mouseMovement = v;
}

//------------------------------------------------------------------------------
/**
*/
inline const Math::matrix44&
MayaCameraUtil::GetCameraTransform() const
{
    return this->cameraTransform;
}

//------------------------------------------------------------------------------
/**
*/
inline void
MayaCameraUtil::SetZoomIn(float v)
{
    this->zoomIn = v;
}

//------------------------------------------------------------------------------
/**
*/
inline void
MayaCameraUtil::SetZoomOut(float v)
{
    this->zoomOut = v;
}

//------------------------------------------------------------------------------
/**
*/
inline void
MayaCameraUtil::SetPanning(const Math::float2& v)
{
    this->panning = v;
}

//------------------------------------------------------------------------------
/**
*/
inline void
MayaCameraUtil::SetOrbiting(const Math::float2& v)
{
    this->orbiting = v;
}

} // namespace RenderUtil
//------------------------------------------------------------------------------
#endif
