#pragma once
//------------------------------------------------------------------------------
/**
    @class Math::point
    
    A point in homogenous space. A point describes a position in space,
    and has its W component set to 1.0.
    
    (C) 2007 Radon Labs GmbH
*/
#include "math/float4.h"
#include "math/vector.h"

//------------------------------------------------------------------------------
namespace Math
{
class point;

#if (defined(_XM_VMX128_INTRINSICS_) && !defined(_XM_NO_INTRINSICS_))
typedef const point  __PointArg;
#else
// win32 VC++ does not support passing aligned objects as value so far
// here is a bug-report at microsoft saying so:
// http://connect.microsoft.com/VisualStudio/feedback/ViewFeedback.aspx?FeedbackID=334581
typedef const point& __PointArg;
#endif


_DECLSPEC_ALIGN_16_ 
#if __XBOX360__
__declspec(passinreg)
#endif
class point : public float4
{
public:
    /// default constructor
    point();
    /// construct from components
    point(scalar x, scalar y, scalar z);
    /// construct from float4
    point(const float4& rhs);
    /// !!!! copy constructor forbidden, otherwise passing point's to a function
    /// !!!! via Registers doesnt work
    //point(const point& rhs);
    /// construct from XMVECTOR
    point(XMVECTOR rhs);
    /// return a point at the origin (0, 0, 0)
    static point origin();
    /// assignment operator
    void operator=(const point& rhs);
    /// assign XMVECTOR
    void operator=(XMVECTOR rhs);
    /// inplace add vector
    void operator+=(const vector& rhs);
    /// inplace subtract vector
    void operator-=(const vector& rhs);
    /// add point and vector
    point operator+(const vector& rhs) const;
    /// subtract vectors from point
    point operator-(const vector& rhs) const;
    /// subtract point from point into a vector
    vector operator-(const point& rhs) const;
    /// equality operator
    bool operator==(const point& rhs) const;
    /// inequality operator
    bool operator!=(const point& rhs) const;
    /// set components
    void set(scalar x, scalar y, scalar z);
};

//------------------------------------------------------------------------------
/**
*/
__forceinline
point::point() :
    float4(0.0f, 0.0f, 0.0f, 1.0f)
{
    // empty
}

//------------------------------------------------------------------------------
/**
*/
__forceinline
point::point(scalar x, scalar y, scalar z) :
    float4(x, y, z, 1.0f)
{
    // empty
}

//------------------------------------------------------------------------------
/**
*/
__forceinline
point::point(const float4& rhs) :
    float4(rhs)
{
    this->set_w(1.0f);
}

//------------------------------------------------------------------------------
/**
*/
__forceinline
point::point(XMVECTOR rhs) :
    float4(rhs)
{
    // empty
}

//------------------------------------------------------------------------------
/**
*/
__forceinline point
point::origin()
{
    return point(0.0f, 0.0f, 0.0f);
}

//------------------------------------------------------------------------------
/**
*/
__forceinline void
point::operator=(const point& rhs)
{
    this->vec = rhs.vec;
}

//------------------------------------------------------------------------------
/**
*/
__forceinline void
point::operator+=(const vector& rhs)
{
    this->vec = XMVectorAdd(this->vec, rhs.vec);
}

//------------------------------------------------------------------------------
/**
*/
__forceinline void
point::operator-=(const vector& rhs)
{
    this->vec = XMVectorSubtract(this->vec, rhs.vec);
}

//------------------------------------------------------------------------------
/**
*/
__forceinline point
point::operator+(const vector& rhs) const
{
    return XMVectorAdd(this->vec, rhs.vec);
}

//------------------------------------------------------------------------------
/**
*/
__forceinline point
point::operator-(const vector& rhs) const
{
    return XMVectorSubtract(this->vec, rhs.vec);
}

//------------------------------------------------------------------------------
/**
*/
__forceinline vector
point::operator-(const point& rhs) const
{
    return XMVectorSubtract(this->vec, rhs.vec);
}

//------------------------------------------------------------------------------
/**
*/
__forceinline bool
point::operator==(const point& rhs) const
{
    return (0 != XMVector4Equal(this->vec, rhs.vec));
}

//------------------------------------------------------------------------------
/**
*/
__forceinline bool
point::operator!=(const point& rhs) const
{
    return (0 != XMVector4NotEqual(this->vec, rhs.vec));
}    

//------------------------------------------------------------------------------
/**
*/
__forceinline void
point::set(scalar x, scalar y, scalar z)
{
    float4::set(x, y, z, 1.0f);
}

} // namespace Math
//------------------------------------------------------------------------------
