#pragma once
//------------------------------------------------------------------------------
/**
    @class Math::matrix44
    
    A matrix44 class on top of Xbox360 math functions.
    
    (C) 2007 Radon Labs GmbH
*/
#include "core/types.h"
#include "math/scalar.h"
#include "math/float4.h"
#include "math/plane.h"
#include "math/quaternion.h"

//------------------------------------------------------------------------------
namespace Math
{
class quaternion;
class plane;

// could not get the compiler to really pass it in registers for xbox, so
// this is a reference so far
typedef const matrix44& __Matrix44Arg;

_DECLSPEC_ALIGN_16_ 
class matrix44
{
public:
    /// default constructor, NOTE: does NOT setup components!
    matrix44();
    /// construct from components
    matrix44(float4 const &row0, float4 const &row1, float4 const &row2, float4 const &row3);
    /// copy constructor
    //matrix44(const matrix44& rhs);
    /// construct from XMMATRIX
    matrix44(const XMMATRIX& rhs);
    
    /// assignment operator
    void operator=(const matrix44& rhs);
    /// assign XMMATRIX
    void operator=(const XMMATRIX& rhs);
    /// equality operator
    bool operator==(const matrix44& rhs) const;
    /// inequality operator
    bool operator!=(const matrix44& rhs) const;

    /// load content from 16-byte-aligned memory
    void load(const scalar* ptr);
    /// load content from unaligned memory
    void loadu(const scalar* ptr);
    /// write content to 16-byte-aligned memory through the write cache
    void store(scalar* ptr) const;
    /// write content to unaligned memory through the write cache
    void storeu(scalar* ptr) const;
    /// stream content to 16-byte-aligned memory circumventing the write-cache
    void stream(scalar* ptr) const;

    /// set content
    void set(float4 const &row0, float4 const &row1, float4 const &row2, float4 const &row3);
    /// write access to x component
    void setrow0(float4 const &row0);
    /// write access to y component
    void setrow1(float4 const &row1);
    /// write access to z component
    void setrow2(float4 const &row2);
    /// write access to w component
    void setrow3(float4 const &row3);
    /// read-only access to x component
    const float4& getrow0() const;
    /// read-only access to y component
    const float4& getrow1() const;
    /// read-only access to z component
    const float4& getrow2() const;
    /// read-only access to w component
    const float4& getrow3() const;

    /// write access to x component
    void set_xaxis(float4 const &x);
    /// write access to y component
    void set_yaxis(float4 const &y);
    /// write access to z component
    void set_zaxis(float4 const &z);
    /// write access to w component / pos component
    void set_position(float4 const &pos);
    /// read access to x component
    const float4& get_xaxis() const;
    /// read access to y component
    const float4& get_yaxis() const;
    /// read access to z component
    const float4& get_zaxis() const;
    /// read access to w component / pos component
    const float4& get_position() const;
    /// add a translation to pos_component
    void translate(float4 const &t);
    /// scale matrix
    void scale(float4 const &v);

    /// return true if matrix is identity
    bool isidentity() const;
    /// return determinant of matrix
    scalar determinant() const;
    /// decompose into scale, rotation and translation
    /// !!! Note: 
    void decompose(float4& outScale, quaternion& outRotation, float4& outTranslation) const;

    /// build identity matrix
    static matrix44 identity();
    /// build matrix from affine transformation
    static matrix44 affinetransformation(scalar scaling, float4 const &rotationCenter, const quaternion& rotation, float4 const &translation);
    /// compute the inverse of a matrix
    static matrix44 inverse(const matrix44& m);
    /// build left handed lookat matrix
    static matrix44 lookatlh(float4 const &eye, float4 const &at, float4 const &up);
    /// build right handed lookat matrix
    static matrix44 lookatrh(float4 const &eye, float4 const &at, float4 const &up);
    /// multiply 2 matrices
    static matrix44 multiply(const matrix44& m0, const matrix44& m1);
    /// build left handed orthogonal projection matrix
    static matrix44 ortholh(scalar w, scalar h, scalar zn, scalar zf);
    /// build right handed orthogonal projection matrix
    static matrix44 orthorh(scalar w, scalar h, scalar zn, scalar zf);
    /// build left-handed off-center orthogonal projection matrix
    static matrix44 orthooffcenterlh(scalar l, scalar r, scalar b, scalar t, scalar zn, scalar zf);
    /// build right-handed off-center orthogonal projection matrix
    static matrix44 orthooffcenterrh(scalar l, scalar r, scalar b, scalar t, scalar zn, scalar zf);
    /// build left-handed perspective projection matrix based on field-of-view
    static matrix44 perspfovlh(scalar fovy, scalar aspect, scalar zn, scalar zf);
    /// build right-handed perspective projection matrix based on field-of-view
    static matrix44 perspfovrh(scalar fovy, scalar aspect, scalar zn, scalar zf);
    /// build left-handed perspective projection matrix
    static matrix44 persplh(scalar w, scalar h, scalar zn, scalar zf);
    /// build right-handed perspective projection matrix
    static matrix44 persprh(scalar w, scalar h, scalar zn, scalar zf);
    /// build left-handed off-center perspective projection matrix
    static matrix44 perspoffcenterlh(scalar l, scalar r, scalar b, scalar t, scalar zn, scalar zf);
    /// build right-handed off-center perspective projection matrix
    static matrix44 perspoffcenterrh(scalar l, scalar r, scalar b, scalar t, scalar zn, scalar zf);
    /// build matrix that reflects coordinates about a plance
    static matrix44 reflect(const plane& p);
    /// build rotation matrix around arbitrary axis
    static matrix44 rotationaxis(float4 const &axis, scalar angle);
    /// build rotation matrix from quaternion
    static matrix44 rotationquaternion(const quaternion& q);
    /// build x-axis-rotation matrix
    static matrix44 rotationx(scalar angle);
    /// build y-axis-rotation matrix
    static matrix44 rotationy(scalar angle);
    /// build z-axis-rotation matrix
    static matrix44 rotationz(scalar angle);
    /// build rotation matrix from yaw, pitch and roll
    static matrix44 rotationyawpitchroll(scalar yaw, scalar pitch, scalar roll);
    /// build a scaling matrix from components
    static matrix44 scaling(scalar sx, scalar sy, scalar sz);
    /// build a scaling matrix from float4
    static matrix44 scaling(float4 const &s);
    /// build a transformation matrix
    static matrix44 transformation(float4 const &scalingCenter, const quaternion& scalingRotation, float4 const &scaling, float4 const &rotationCenter, const quaternion& rotation, float4 const &translation);
    /// build a translation matrix
    static matrix44 translation(scalar x, scalar y, scalar z);
    /// build a translation matrix from point
    static matrix44 translation(float4 const &t);
    /// return the transpose of a matrix
    static matrix44 transpose(const matrix44& m);
    /// transform 4d vector by matrix44, faster inline version than float4::transform
    static float4 transform(const float4 &v, const matrix44 &m);
    /// return a quaternion from rotational part of the 4x4 matrix
    static quaternion rotationmatrix(const matrix44& m);
    /// transform a plane with a matrix
    static plane transform(const plane& p, const matrix44& m);
    /// check if point lies inside matrix frustum
    static bool ispointinside(const float4& p, const matrix44& m);
    /// convert to any type
    template<typename T> T as() const;

private:
    friend class float4;
    friend class plane;
    friend class quaternion;

    XMMATRIX mx;
};

//------------------------------------------------------------------------------
/**
*/
__forceinline
matrix44::matrix44():
    mx(XMMatrixIdentity())
{
    // empty
}

//------------------------------------------------------------------------------
/**
*/
__forceinline
matrix44::matrix44(float4 const &row0, float4 const &row1, float4 const &row2, float4 const &row3)
{
    this->mx.r[0] = row0.vec;
    this->mx.r[1] = row1.vec;
    this->mx.r[2] = row2.vec;
    this->mx.r[3] = row3.vec;
}

//------------------------------------------------------------------------------
/**
*/
/*
__forceinline
matrix44::matrix44(const matrix44& rhs) :
    mx(rhs.mx)
{
    // empty
}
*/

//------------------------------------------------------------------------------
/**
*/
__forceinline
matrix44::matrix44(const XMMATRIX& rhs) :
    mx(rhs)
{
    // empty
}

//------------------------------------------------------------------------------
/**
*/
__forceinline void
matrix44::operator=(const matrix44& rhs)
{
    this->mx = rhs.mx;
}

//------------------------------------------------------------------------------
/**
*/
__forceinline void
matrix44::operator=(const XMMATRIX& rhs)
{
    this->mx = rhs;
}

//------------------------------------------------------------------------------
/**
*/
__forceinline bool
matrix44::operator==(const matrix44& rhs) const
{
    return XMVector4Equal(this->mx.r[0], rhs.mx.r[0]) && 
           XMVector4Equal(this->mx.r[1], rhs.mx.r[1]) && 
           XMVector4Equal(this->mx.r[2], rhs.mx.r[2]) && 
           XMVector4Equal(this->mx.r[3], rhs.mx.r[3]);
}

//------------------------------------------------------------------------------
/**
*/
__forceinline bool
matrix44::operator!=(const matrix44& rhs) const
{
    return XMVector4NotEqual(this->mx.r[0], rhs.mx.r[0]) ||
           XMVector4NotEqual(this->mx.r[1], rhs.mx.r[1]) || 
           XMVector4NotEqual(this->mx.r[2], rhs.mx.r[2]) || 
           XMVector4NotEqual(this->mx.r[3], rhs.mx.r[3]);
}

//------------------------------------------------------------------------------
/**
*/
__forceinline void
matrix44::load(const scalar* ptr)
{
    this->mx.r[0] = XMLoadFloat4A((XMFLOAT4A*)ptr);
    this->mx.r[1] = XMLoadFloat4A((XMFLOAT4A*)(ptr + 4));
    this->mx.r[2] = XMLoadFloat4A((XMFLOAT4A*)(ptr + 8));
    this->mx.r[3] = XMLoadFloat4A((XMFLOAT4A*)(ptr + 12));
}

//------------------------------------------------------------------------------
/**
*/
__forceinline void
matrix44::loadu(const scalar* ptr)
{
    this->mx.r[0] = XMLoadFloat4((XMFLOAT4*)ptr);
    this->mx.r[1] = XMLoadFloat4((XMFLOAT4*)(ptr + 4));
    this->mx.r[2] = XMLoadFloat4((XMFLOAT4*)(ptr + 8));
    this->mx.r[3] = XMLoadFloat4((XMFLOAT4*)(ptr + 12));
}

//------------------------------------------------------------------------------
/**
*/
__forceinline void
matrix44::store(scalar* ptr) const
{
    XMStoreFloat4A((XMFLOAT4A*)ptr, this->mx.r[0]);
    XMStoreFloat4A((XMFLOAT4A*)(ptr + 4), this->mx.r[1]);
    XMStoreFloat4A((XMFLOAT4A*)(ptr + 8), this->mx.r[2]);
    XMStoreFloat4A((XMFLOAT4A*)(ptr + 12), this->mx.r[3]);
}

//------------------------------------------------------------------------------
/**
*/
__forceinline void
matrix44::storeu(scalar* ptr) const
{
    XMStoreFloat4((XMFLOAT4*)ptr, this->mx.r[0]);
    XMStoreFloat4((XMFLOAT4*)(ptr + 4), this->mx.r[1]);
    XMStoreFloat4((XMFLOAT4*)(ptr + 8), this->mx.r[2]);
    XMStoreFloat4((XMFLOAT4*)(ptr + 12), this->mx.r[3]);
}

//------------------------------------------------------------------------------
/**
*/
__forceinline void
matrix44::stream(scalar* ptr) const
{
    XMStoreFloat4A((XMFLOAT4A*)ptr, this->mx.r[0]);
    XMStoreFloat4A((XMFLOAT4A*)(ptr + 4), this->mx.r[1]);
    XMStoreFloat4A((XMFLOAT4A*)(ptr + 8), this->mx.r[2]);
    XMStoreFloat4A((XMFLOAT4A*)(ptr + 12), this->mx.r[3]);
}

//------------------------------------------------------------------------------
/**
*/
__forceinline void
matrix44::set(float4 const &row0, float4 const &row1, float4 const &row2, float4 const &row3)
{
    this->mx.r[0] = row0.vec;
    this->mx.r[1] = row1.vec;
    this->mx.r[2] = row2.vec;
    this->mx.r[3] = row3.vec;
}

//------------------------------------------------------------------------------
/**
*/
__forceinline void
matrix44::setrow0(float4 const &r)
{
    this->mx.r[0] = r.vec;
}

//------------------------------------------------------------------------------
/**
*/
__forceinline const float4&
matrix44::getrow0() const
{
    return *(float4*)&(this->mx.r[0]);
}

//------------------------------------------------------------------------------
/**
*/
__forceinline void
matrix44::setrow1(float4 const &r)
{
    this->mx.r[1] = r.vec;
}

//------------------------------------------------------------------------------
/**
*/
__forceinline const float4&
matrix44::getrow1() const
{
    return *(float4*)&(this->mx.r[1]);
}

//------------------------------------------------------------------------------
/**
*/
__forceinline void
matrix44::setrow2(float4 const &r)
{
    this->mx.r[2] = r.vec;
}

//------------------------------------------------------------------------------
/**
*/
__forceinline const float4&
matrix44::getrow2() const
{
    return *(float4*)&(this->mx.r[2]);
}

//------------------------------------------------------------------------------
/**
*/
__forceinline void
matrix44::setrow3(float4 const &r)
{
    this->mx.r[3] = r.vec;
}

//------------------------------------------------------------------------------
/**
*/
__forceinline const float4&
matrix44::getrow3() const
{
    return *(float4*)&(this->mx.r[3]);
}

//------------------------------------------------------------------------------
/**
*/
__forceinline void
matrix44::set_xaxis(float4 const &x)
{
    this->mx.r[0] = x.vec;
}

//------------------------------------------------------------------------------
/**
*/
__forceinline void
matrix44::set_yaxis(float4 const &y)
{
    this->mx.r[1] = y.vec;
}

//------------------------------------------------------------------------------
/**
*/
__forceinline void
matrix44::set_zaxis(float4 const &z)
{
    this->mx.r[2] = z.vec;
}

//------------------------------------------------------------------------------
/**
*/
__forceinline void
matrix44::set_position(float4 const &pos)
{
    this->mx.r[3] = pos.vec;
}

//------------------------------------------------------------------------------
/**
*/
__forceinline const float4&
matrix44::get_xaxis() const
{
    return *(float4*)&(this->mx.r[0]);
}

//------------------------------------------------------------------------------
/**
*/
__forceinline const float4&
matrix44::get_yaxis() const
{
    return *(float4*)&(this->mx.r[1]);
}

//------------------------------------------------------------------------------
/**
*/
__forceinline const float4&
matrix44::get_zaxis() const
{
    return *(float4*)&(this->mx.r[2]);
}

//------------------------------------------------------------------------------
/**
*/
__forceinline const float4&
matrix44::get_position() const
{
    return *(float4*)&(this->mx.r[3]);
}

//------------------------------------------------------------------------------
/**
*/
__forceinline
void 
matrix44::translate(float4 const &t)
{
    #if _DEBUG
    n_assert2(t.w() == 0, "w component not 0, use vector for translation not a point!");
    #endif
    this->mx.r[3] += t.vec;
}

//------------------------------------------------------------------------------
/**
*/
__forceinline void
matrix44::scale(float4 const &s) 
{    
    // need to make sure that last column isn't erased
    float4 scl = s;
    scl.set_w(1.0f);

    this->mx.r[0] = float4::multiply(this->mx.r[0], scl).vec;
    this->mx.r[1] = float4::multiply(this->mx.r[1], scl).vec;
    this->mx.r[2] = float4::multiply(this->mx.r[2], scl).vec;
    this->mx.r[3] = float4::multiply(this->mx.r[3], scl).vec;
}

//------------------------------------------------------------------------------
/**
*/
__forceinline bool
matrix44::isidentity() const
{
    return (0 != XMMatrixIsIdentity(this->mx));
}

//------------------------------------------------------------------------------
/**
*/
__forceinline scalar
matrix44::determinant() const
{
    return float4::unpack_x(XMMatrixDeterminant(this->mx));
}

//------------------------------------------------------------------------------
/**
*/
__forceinline matrix44
matrix44::identity()
{
    return XMMatrixIdentity();
}

//------------------------------------------------------------------------------
/**
*/
__forceinline matrix44
matrix44::inverse(const matrix44& m)
{
    XMVECTOR det;
    return XMMatrixInverse(&det, m.mx);
}

//------------------------------------------------------------------------------
/**
*/
__forceinline matrix44
matrix44::lookatlh(float4 const &eye, float4 const &at, float4 const &up)
{
#if NEBULA3_DEBUG
    n_assert(up.length() > 0);
#endif
    // hmm the XM lookat functions are kinda pointless, because they
    // return a VIEW matrix, which is already inverse (so one would
    // need to reverse again!)
    const float4 zaxis = float4::normalize(at - eye);
    float4 normUp = float4::normalize(up);
    if (n_abs(float4::dot3(zaxis, normUp)) > 0.9999999f)
    {
        // need to choose a different up vector because up and lookat point
        // into same or opposite direction
        // just rotate y->x, x->z and z->y
        normUp = float4::permute(normUp, normUp, float4::permute_control(1, 2, 0, 3));        
    }
    const float4 xaxis = float4::cross3(normUp, zaxis);
    const float4 yaxis = float4::cross3(zaxis, xaxis);
    return matrix44(xaxis, yaxis, zaxis, eye);
}

//------------------------------------------------------------------------------
/**
*/
__forceinline matrix44
matrix44::lookatrh(float4 const &eye, float4 const &at, float4 const &up)
{
#if NEBULA3_DEBUG
    n_assert(up.length() > 0);
#endif
    // hmm the XM lookat functions are kinda pointless, because they
    // return a VIEW matrix, which is already inverse (so one would
    // need to reverse again!)
    const float4 zaxis = float4::normalize(eye - at);
    float4 normUp = float4::normalize(up);
    if (n_abs(float4::dot3(zaxis, normUp)) > 0.9999999f)
    {
        // need to choose a different up vector because up and lookat point
        // into same or opposite direction
        // just rotate y->x, x->z and z->y
        normUp = float4::permute(normUp, normUp, float4::permute_control(1, 2, 0, 3));        
    }
    const float4 xaxis = float4::cross3(normUp, zaxis);
    const float4 yaxis = float4::cross3(zaxis, xaxis);
    return matrix44(xaxis, yaxis, zaxis, eye);
}

//------------------------------------------------------------------------------
/**
*/
__forceinline matrix44
matrix44::multiply(const matrix44& m0, const matrix44& m1)
{
    return XMMatrixMultiply(m0.mx, m1.mx);
}

//------------------------------------------------------------------------------
/**
*/
__forceinline matrix44
matrix44::ortholh(scalar w, scalar h, scalar zn, scalar zf)
{
    return XMMatrixOrthographicLH(w, h, zn, zf);
}

//------------------------------------------------------------------------------
/**
*/
__forceinline matrix44
matrix44::orthorh(scalar w, scalar h, scalar zn, scalar zf)
{
    return XMMatrixOrthographicRH(w, h, zn, zf);
}

//------------------------------------------------------------------------------
/**
*/
__forceinline matrix44
matrix44::orthooffcenterlh(scalar l, scalar r, scalar b, scalar t, scalar zn, scalar zf)
{
    return XMMatrixOrthographicOffCenterLH(l, r, b, t, zn, zf);
}

//------------------------------------------------------------------------------
/**
*/
__forceinline matrix44
matrix44::orthooffcenterrh(scalar l, scalar r, scalar b, scalar t, scalar zn, scalar zf)
{
    return XMMatrixOrthographicOffCenterRH(l, r, b, t, zn, zf);
}

//------------------------------------------------------------------------------
/**
*/
__forceinline matrix44
matrix44::perspfovlh(scalar fovy, scalar aspect, scalar zn, scalar zf)
{
    return XMMatrixPerspectiveFovLH(fovy, aspect, zn, zf);
}

//------------------------------------------------------------------------------
/**
*/
__forceinline matrix44
matrix44::perspfovrh(scalar fovy, scalar aspect, scalar zn, scalar zf)
{
    return XMMatrixPerspectiveFovRH(fovy, aspect, zn, zf);
}

//------------------------------------------------------------------------------
/**
*/
__forceinline matrix44
matrix44::persplh(scalar w, scalar h, scalar zn, scalar zf)
{
    return XMMatrixPerspectiveLH(w, h, zn, zf);
}

//------------------------------------------------------------------------------
/**
*/
__forceinline matrix44
matrix44::persprh(scalar w, scalar h, scalar zn, scalar zf)
{
    return XMMatrixPerspectiveRH(w, h, zn, zf);
}

//------------------------------------------------------------------------------
/**
*/
__forceinline matrix44
matrix44::perspoffcenterlh(scalar l, scalar r, scalar b, scalar t, scalar zn, scalar zf)
{
    return XMMatrixPerspectiveOffCenterLH(l, r, b, t, zn, zf);
}

//------------------------------------------------------------------------------
/**
*/
__forceinline matrix44
matrix44::perspoffcenterrh(scalar l, scalar r, scalar b, scalar t, scalar zn, scalar zf)
{
    return XMMatrixPerspectiveOffCenterRH(l, r, b, t, zn, zf);
}

//------------------------------------------------------------------------------
/**
*/
__forceinline matrix44
matrix44::rotationaxis(float4 const &axis, scalar angle)
{
    return XMMatrixRotationAxis(axis.vec, angle);
}

//------------------------------------------------------------------------------
/**
*/
__forceinline matrix44
matrix44::rotationx(scalar angle)
{
    return XMMatrixRotationX(angle);
}

//------------------------------------------------------------------------------
/**
*/
__forceinline matrix44
matrix44::rotationy(scalar angle)
{
    return XMMatrixRotationY(angle);
}

//------------------------------------------------------------------------------
/**
*/
__forceinline matrix44
matrix44::rotationz(scalar angle)
{
    return XMMatrixRotationZ(angle);
}

//------------------------------------------------------------------------------
/**
*/
__forceinline matrix44
matrix44::rotationyawpitchroll(scalar yaw, scalar pitch, scalar roll)
{
    return XMMatrixRotationRollPitchYaw(pitch, yaw, roll);
}

//------------------------------------------------------------------------------
/**
*/
__forceinline matrix44
matrix44::scaling(scalar sx, scalar sy, scalar sz)
{
    return XMMatrixScaling(sx, sy, sz);
}

//------------------------------------------------------------------------------
/**
*/
__forceinline matrix44
matrix44::scaling(float4 const &s)
{
    return XMMatrixScalingFromVector(s.vec);
}

//------------------------------------------------------------------------------
/**
*/
__forceinline matrix44
matrix44::translation(scalar x, scalar y, scalar z)
{
    return XMMatrixTranslation(x, y, z);
}

//------------------------------------------------------------------------------
/**
*/
__forceinline matrix44
matrix44::translation(float4 const &t)
{
    return XMMatrixTranslationFromVector(t.vec);
}

//------------------------------------------------------------------------------
/**
*/
__forceinline matrix44
matrix44::transpose(const matrix44& m)
{
    return XMMatrixTranspose(m.mx);
}

//------------------------------------------------------------------------------
/**
*/
__forceinline
float4
matrix44::transform(const float4 &v, const matrix44 &m)
{
    return XMVector4Transform(v.vec, m.mx);
}

//------------------------------------------------------------------------------
/**
*/
__forceinline
quaternion
matrix44::rotationmatrix(const matrix44& m)
{
    return XMQuaternionRotationMatrix(m.mx);
}

//------------------------------------------------------------------------------
/**
*/
__forceinline
plane
matrix44::transform(const plane &p, const matrix44& m)
{
    return XMPlaneTransform(p.vec, m.mx);
}

} // namespace Math
//------------------------------------------------------------------------------

