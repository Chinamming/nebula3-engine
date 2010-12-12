//------------------------------------------------------------------------------
//  colorparams.cc
//  (C) 2006 Radon Labs GmbH
//------------------------------------------------------------------------------
#include "stdneb.h"
#include "colorparams.h"

namespace PostEffect
{
using namespace Math;

__ImplementClass(PostEffect::ColorParams, 'PECP', PostEffect::ParamBase);

//------------------------------------------------------------------------------
/**
*/
ColorParams::ColorParams() :
    colorSaturation(1.0f),
    colorBalance(1.0f, 1.0f, 1.0f, 1.0f)
{
    // empty
}

//------------------------------------------------------------------------------
/**
*/
ColorParams::~ColorParams()
{
    // empty
}

//------------------------------------------------------------------------------
/**
*/
void
ColorParams::operator=(const Ptr<ParamBase>& rhs)
{
    n_assert(rhs->IsA(ColorParams::RTTI));

    if (this != rhs.get())
    {
        this->colorSaturation = rhs.cast<ColorParams>()->colorSaturation;
        this->colorBalance = rhs.cast<ColorParams>()->colorBalance;
    }
    ParamBase::operator =(rhs);
}

//------------------------------------------------------------------------------
/**
*/
void
ColorParams::BlendTo(const Ptr<ParamBase>& p, float l)
{
    n_assert(p->IsA(ColorParams::RTTI));

    this->colorSaturation       = n_lerp(this->colorSaturation, p.cast<ColorParams>()->colorSaturation, l);
    this->colorBalance          = float4::lerp(this->colorBalance, p.cast<ColorParams>()->colorBalance, l);
}

//------------------------------------------------------------------------------
/**
*/
bool
ColorParams::Equals(const Ptr<ParamBase>& v2, float threshold) const
{
    n_assert(v2->IsA(ColorParams::RTTI));
    const Ptr<ColorParams> temp = v2.cast<ColorParams>();

    if( Math::float4::nearequal4(this->GetColorBalance(), temp->GetColorBalance(), Math::float4(threshold, threshold, threshold, threshold)) &&
        Math::n_abs(temp->GetColorSaturation() - this->GetColorSaturation()) < threshold)
    {
        return true;
    }
    return false;
}
} // namespace PostEffects
