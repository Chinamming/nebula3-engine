#pragma once
//------------------------------------------------------------------------------
/**
    @class GraphicsFeature::LightFlickerUtil
  
    Manipulate a light entity with position and intensity flickering.
    
    (C) 2007 Radon Labs GmbH
*/    
#include "graphics/abstractlightentity.h"
#include "math/vector.h"

//------------------------------------------------------------------------------
namespace GraphicsFeature
{
class LightFlickerUtil
{
public:
    /// constructor
    LightFlickerUtil();
    
    /// set light entity to manipulate
    void SetLightEntity(const Ptr<Graphics::AbstractLightEntity>& e);
    /// get light entity
    const Ptr<Graphics::AbstractLightEntity>& GetLightEntity() const;
    /// return true if entity is valid
    bool HasLightEntity() const;
    
    /// enable/disable flickering
    void SetEnabled(bool b);
    /// is flickering enabled?
    bool IsEnabled() const;
    /// set flicker frequency
    void SetFrequency(float f);
    /// get flicker frequency
    float GetFrequency() const;
    /// set intensity flicker amplitude
    void SetIntensityAmplitude(float f);
    /// get intensity flicker amplitude
    float GetIntensityAmplitude() const;
    /// set position flicker amplitude
    void SetPositionAmplitude(float f);
    /// get position flicker aplitude
    float GetPositionAmplitude() const;

    /// update the light source, call this method once per frame
    void Update();

private:
    Ptr<Graphics::AbstractLightEntity> lightEntity;
    bool enabled;
    float frequency;
    float intensityAmplitude;
    float positionAmplitude;
};

//------------------------------------------------------------------------------
/**
*/
inline void
LightFlickerUtil::SetLightEntity(const Ptr<Graphics::AbstractLightEntity>& e)
{
    this->lightEntity = e;
}

//------------------------------------------------------------------------------
/**
*/
inline const Ptr<Graphics::AbstractLightEntity>&
LightFlickerUtil::GetLightEntity() const
{
    return this->lightEntity;
}

//------------------------------------------------------------------------------
/**
*/
inline bool
LightFlickerUtil::HasLightEntity() const
{
    return this->lightEntity.isvalid();
}

//------------------------------------------------------------------------------
/**
*/
inline void
LightFlickerUtil::SetEnabled(bool b)
{
    this->enabled = b;
}

//------------------------------------------------------------------------------
/**
*/
inline bool
LightFlickerUtil::IsEnabled() const
{
    return this->enabled;
}

//------------------------------------------------------------------------------
/**
*/
inline void
LightFlickerUtil::SetFrequency(float f)
{
    this->frequency = f;
}

//------------------------------------------------------------------------------
/**
*/
inline float
LightFlickerUtil::GetFrequency() const
{
    return this->frequency;
}

//------------------------------------------------------------------------------
/**
*/
inline void
LightFlickerUtil::SetIntensityAmplitude(float f)
{
    this->intensityAmplitude = f;
}

//------------------------------------------------------------------------------
/**
*/
inline float
LightFlickerUtil::GetIntensityAmplitude() const
{
    return this->intensityAmplitude;
}

//------------------------------------------------------------------------------
/**
*/
inline void
LightFlickerUtil::SetPositionAmplitude(float f)
{
    this->positionAmplitude = f;
}

//------------------------------------------------------------------------------
/**
*/
inline float
LightFlickerUtil::GetPositionAmplitude() const
{
    return this->positionAmplitude;
}

} // namespace GraphicsFeature
//------------------------------------------------------------------------------

