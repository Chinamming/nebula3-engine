//------------------------------------------------------------------------------
//  float4test.cc
//  (C) 2009 Radon Labs GmbH
//------------------------------------------------------------------------------
#include "stdneb.h"

#include "float4test.h"
#include "math/float4.h"
#include "math/matrix44.h"
#include "mathtestutil.h"
#include "stackalignment.h"
#include "testbase/stackdebug.h"

using namespace Math;

namespace Test
{
__ImplementClass(Test::Float4Test, 'F4TS', Test::TestCase);

//------------------------------------------------------------------------------
/**
*/
void
Float4Test::Run()
{
    STACK_CHECKPOINT("Test::Float4Test::Run()");

	// construction
    float4 v0(1.0f, 2.0f, 3.0f, 4.0f);
    float4 v1(4.0f, 3.0f, 2.0f, 1.0f);
    float4 v2(v0);
    float4 v3(v1);
    this->Verify(v0 == v2);
    this->Verify(v1 == v3);
    this->Verify(v0 != v1);
    this->Verify(v2 != v3);
    this->Verify(v0 == float4(1.0f, 2.0f, 3.0f, 4.0));

    // assignemt
    v2 = v1;
    this->Verify(v2 == v1);
    v2 = v0;
    this->Verify(v2 == v0);

    // operators
	// float4 operator-() const;
    v0 = -v0;
    this->Verify(v0 == float4(-1.0f, -2.0f, -3.0f, -4.0f));
    v0 = -v0;
    this->Verify(v0 == v2);
	// void operator+=(const float4& rhs);
    v2 += v3;
    this->Verify(v2 == float4(5.0f, 5.0f, 5.0f, 5.0f));
	// void operator-=(const float4& rhs);
    v2 -= v3;
    this->Verify(v2 == v0);
	// void operator*=(scalar s);
    v2 *= 2.0f;
    this->Verify(v2 == float4(2.0f, 4.0f, 6.0f, 8.0f));
	// float4 operator+(const float4& rhs) const;
    v2 = v0 + v1;
    this->Verify(v2 == float4(5.0f, 5.0f, 5.0f, 5.0f));
	// float4 operator-(const float4& rhs) const;
    v2 = v0 - v1;
    this->Verify(v2 == float4(-3.0f, -1.0f, 1.0f, 3.0f));
	// float4 operator*(scalar s) const;
    v2 = v0 * 2.0f;
    this->Verify(v2 == float4(2.0f, 4.0f, 6.0f, 8.0f));

    // setting and getting content
    v2.set(2.0f, 3.0f, 4.0f, 5.0f);
    this->Verify(v2.x() == 2.0f);
    this->Verify(v2.y() == 3.0f);
    this->Verify(v2.z() == 4.0f);
    this->Verify(v2.w() == 5.0f);
    this->Verify(v2 == float4(2.0f, 3.0f, 4.0f, 5.0f));
    v2.set_x(1.0f);
    v2.set_y(2.0f);
    v2.set_z(3.0f);
    v2.set_w(4.0f);
    this->Verify(v2 == float4(1.0f, 2.0f, 3.0f, 4.0f));
    v2.x() = 5.0f;
    v2.y() = 6.0f;
    v2.z() = 7.0f;
    v2.w() = 8.0f;
    this->Verify(v2 == float4(5.0f, 6.0f, 7.0f, 8.0f));

#ifndef __WII__
    // load and store aligned
    NEBULA3_ALIGN16 const scalar fAlignedLoad[4] = { 1.0f, 2.0f, 3.0f, 4.0f };
    NEBULA3_ALIGN16 scalar fAlignedStore[4];
	// check alignment
	n_assert(!((unsigned int)fAlignedLoad & 0xF));
	n_assert(!((unsigned int)fAlignedStore & 0xF));
    v2.load(fAlignedLoad);
    this->Verify(v2 == float4(1.0f, 2.0f, 3.0f, 4.0f));
	// load unaligned must work with aligned data too
    v2.loadu(fAlignedLoad);
    this->Verify(v2 == float4(1.0f, 2.0f, 3.0f, 4.0f));
	v2.load_float3(fAlignedLoad,4.0f);
    this->Verify(v2 == float4(1.0f, 2.0f, 3.0f, 4.0f));
    v2.store(fAlignedStore);
    this->Verify((fAlignedStore[0] == 1.0f) && (fAlignedStore[1] == 2.0f) && (fAlignedStore[2] == 3.0f) && (fAlignedStore[3] == 4.0f));
	// store unaligned must work with aligned data too
    v2.storeu(fAlignedStore);
    this->Verify((fAlignedStore[0] == 1.0f) && (fAlignedStore[1] == 2.0f) && (fAlignedStore[2] == 3.0f) && (fAlignedStore[3] == 4.0f));
    v2.stream(fAlignedStore);
    this->Verify((fAlignedStore[0] == 1.0f) && (fAlignedStore[1] == 2.0f) && (fAlignedStore[2] == 3.0f) && (fAlignedStore[3] == 4.0f));
	
	// load and store unaligned
	NEBULA3_ALIGN16 const scalar fAlignedLoadBase[5] = { 0.0f, 1.0f, 2.0f, 3.0f, 4.0f };
	NEBULA3_ALIGN16 scalar fAlignedStoreBase[5];
	const scalar *fUnalignedLoad = fAlignedLoadBase + 1;
	scalar *fUnalignedStore = fAlignedStoreBase + 1;
	// check un-alignment
	n_assert(((unsigned int)fUnalignedLoad & 0xF));
	n_assert(((unsigned int)fUnalignedStore & 0xF));
    v2.loadu(fUnalignedLoad);
    this->Verify(v2 == float4(1.0f, 2.0f, 3.0f, 4.0f));
	v2.load_float3(fUnalignedLoad,4.0f);
    this->Verify(v2 == float4(1.0f, 2.0f, 3.0f, 4.0f));
    v2.storeu(fUnalignedStore);
    this->Verify((fUnalignedStore[0] == 1.0f) && (fUnalignedStore[1] == 2.0f) && (fUnalignedStore[2] == 3.0f) && (fUnalignedStore[3] == 4.0f));

/*
	// load_ubyte4n_signed
	NEBULA3_ALIGN16 const uchar ucArrAligned[8] = {0, 0, 128, 255, 0xEE, 0xDD, 0xBB, 0xAA };
	// check alignment
	n_assert(!((unsigned int)ucArrAligned & 0xF));
	const uchar *ucArrUnaligned = ucArrAligned + 1;
	// check un-alignment

    // access ucArrAligned[1]
	n_assert(((unsigned int)ucArrUnaligned & 0xF));
	v2.load_ubyte4n_signed(ucArrUnaligned, 2.0f);
print(v2);
	//this->Verify(float4equal(v2, float4(-1.0f, 0.003922f, 1.0f, 2.0f)));

    // access ucArrAligned[2]
    ++ucArrUnaligned;
	v2.load_ubyte4n_signed(ucArrUnaligned, 2.0f);
print(v2);
	//this->Verify(float4equal(v2, float4(-1.0f, 0.003922f, 1.0f, 2.0f)));

    // access ucArrAligned[3]
    ++ucArrUnaligned;
	v2.load_ubyte4n_signed(ucArrUnaligned, 2.0f);
print(v2);
	//this->Verify(float4equal(v2, float4(-1.0f, 0.003922f, 1.0f, 2.0f)));

    // access ucArrAligned[4]
    ++ucArrUnaligned;
	v2.load_ubyte4n_signed(ucArrUnaligned, 2.0f);
print(v2);
	//this->Verify(float4equal(v2, float4(-1.0f, 0.003922f, 1.0f, 2.0f)));
*/
#endif

    // test 16-byte alignment of embedded members on the stack, if we use SSE/SSE2 on windows or
    // xbox or ps3
#if (__WIN32__ && !defined(_XM_NO_INTRINSICS_)) || __XBOX360__ || __PS3__    
    {
        testStackAlignment16<float4>(this);
    }
#endif

    // length and abs
    v2.set(0.0f, 2.0f, 0.0f, 0.0f);
    this->Verify(n_fequal(v2.length(), 2.0f, 0.0001f));
    this->Verify(n_fequal(v2.lengthsq(), 4.0f, 0.0001f));
    v2.set(-1.0f, 2.0f, -3.0f, 4.0f);
    this->Verify(v2.abs() == float4(1.0f, 2.0f, 3.0f, 4.0f));
    
	// reciprocal
	v0.set(1.0f, 10.0f, 5.0f, 2.0f);
	v0 = float4::reciprocal(v0);
	this->Verify(float4equal(v0, float4(1.0f, 0.1f, 0.2f, 0.5f)));

    // cross3
    v0.set(1.0f, 0.0f, 0.0f, 0.0f);
    v1.set(0.0f, 0.0f, 1.0f, 0.0f);
    v2 = float4::cross3(v0, v1);
    this->Verify(float4equal(v2, float4(0.0f, -1.0f, 0.0f, 0.0f)));

	// multiply
    v0.set(1.0f, 2.0f, -3.0f, -4.0f);
    v1.set(5.0f, 6.0f, -7.0f, 8.0f);
	v2 = float4::multiply(v0, v1);
	this->Verify(float4equal(v2, float4(5.0f, 12.0f, 21.0f, -32.0f)));

    // dot3
    v0.set(1.0f, 0.0f, 0.0f, 0.0f);
    v1.set(1.0f, 0.0f, 0.0f, 0.0f);    
    this->Verify(float4::dot3(v0, v1) == 1.0f);
    v1.set(-1.0f, 0.0f, 0.0f, 0.0f);
    this->Verify(float4::dot3(v0, v1) == -1.0f);
    v1.set(0.0f, 1.0f, 0.0f, 0.0f);
    this->Verify(float4::dot3(v0, v1) == 0.0f);

    // @todo: test barycentric(), catmullrom(), hermite()

    // lerp
    v0.set(1.0f, 2.0f, 3.0f, 4.0f);
    v1.set(2.0f, 3.0f, 4.0f, 5.0f);
    v2 = float4::lerp(v0, v1, 0.5f);
    this->Verify(v2 == float4(1.5f, 2.5f, 3.5f, 4.5f));
    v2 = float4::lerp(v0, v1, 0.0f);
    this->Verify(v2 == float4(1.0f, 2.0f, 3.0f, 4.0f));
    v2 = float4::lerp(v0, v1, 1.0f);
    this->Verify(v2 == float4(2.0f, 3.0f, 4.0f, 5.0f));
    
    // maximize/minimize
    v0.set(1.0f, 2.0f, 3.0f, 4.0f);
    v1.set(4.0f, 3.0f, 2.0f, 1.0f);
    v2 = float4::maximize(v0, v1);
    this->Verify(v2 == float4(4.0f, 3.0f, 3.0f, 4.0f));
    v2 = float4::minimize(v0, v1);
    this->Verify(v2 == float4(1.0f, 2.0f, 2.0f, 1.0f));

    // normalize
    v0.set(2.5f, 0.0f, 0.0f, 0.0f);
    v1 = float4::normalize(v0);
    this->Verify(v1 == float4(1.0f, 0.0f, 0.0f, 0.0f));

	// transform (point and vector)
    matrix44 m = matrix44::translation(1.0f, 2.0f, 3.0f);
    v0.set(1.0f, 0.0f, 0.0f, 1.0f);
    v1 = matrix44::transform(v0, m);
    this->Verify(v1 == float4(2.0f, 2.0f, 3.0f, 1.0f));
    v0.set(1.0f, 0.0f, 0.0f, 0.0f);
    v1 = matrix44::transform(v0, m);
    this->Verify(v0 == float4(1.0f, 0.0f, 0.0f, 0.0f));
    const matrix44 m0(float4(1.0f, 0.0f, 0.0f, 0.0f),
					  float4(0.0f, 1.0f, 0.0f, 0.0f),
					  float4(0.0f, 0.0f, 1.0f, 0.0f),
					  float4(1.0f, 2.0f, 3.0f, 1.0f));
	const float4 in(0.0f, 0.0f, 0.0f, 1.0f);
	const float4 out = matrix44::transform(in, m0);
	this->Verify(float4equal(out, float4(1.0f, 2.0f, 3.0f, 1.0f)));

    // component-wise comparison
	const float4 v0000(0.0f, 0.0f, 0.0f, 0.0f);
	const float4 v0011(0.0f, 0.0f, 1.0f, 1.0f);
    const float4 v1111(1.0f, 1.0f, 1.0f, 1.0f);
    const float4 v1222(1.0f, 2.0f, 2.0f, 2.0f);
    const float4 v2222(2.0f, 2.0f, 2.0f, 2.0f);
	// 3 components
	this->Verify(!float4::less3_any(v1222, v1111));
	this->Verify( float4::less3_any(v1222, v2222));
	this->Verify(!float4::less3_all(v1111, v1222));
	this->Verify( float4::less3_all(v0000, v1222));
	this->Verify( float4::lessequal3_any(v1111, v1222));
	this->Verify(!float4::lessequal3_any(v2222, v1111));
	this->Verify(!float4::lessequal3_all(v1222, v1111));
	this->Verify( float4::lessequal3_all(v2222, v2222));
	this->Verify( float4::lessequal3_all(v1222, v2222));
	this->Verify(!float4::greater3_any(v1222, v2222));
	this->Verify( float4::greater3_any(v2222, v1222));
	this->Verify( float4::greater3_all(v1222, v0000));
	this->Verify(!float4::greater3_all(v1111, v1222));
	this->Verify( float4::greaterequal3_any(v1222, v1111));
	this->Verify(!float4::greaterequal3_any(v0000, v1222));
	this->Verify(!float4::greaterequal3_all(v0011, v1222));
	this->Verify( float4::greaterequal3_all(v1222, v1111));
	this->Verify( float4::greaterequal3_all(v0000, v0000));
	this->Verify( float4::greaterequal3_all(v1111, v0000));
	this->Verify(!float4::equal3_any(v1111, v0000));
	this->Verify( float4::equal3_any(v1111, v1222));
	this->Verify( float4::equal3_any(v1111, v1111));
	this->Verify( float4::equal3_all(v1111, v1111));
	this->Verify(!float4::equal3_all(v1111, v1222));    
	this->Verify( float4::nearequal3(v0000, float4( 0.001f, 0.1f, 0.05f, 3.0f), float4(0.001f, 0.1f, 0.05f, 0.0f)));
	this->Verify( float4::nearequal3(v0000, float4(-0.001f,-0.1f,-0.05f,-3.0f), float4(0.001f, 0.1f, 0.05f, 0.0f)));
	this->Verify( !float4::nearequal3(v0000, float4( 0.002f, 0.1f, 0.05f, 3.0f), float4(0.001f, 0.1f, 0.05f, 0.0f)));
	this->Verify( !float4::nearequal3(v0000, float4(-0.001f,-0.2f,-0.05f,-3.0f), float4(0.001f, 0.1f, 0.05f, 0.0f)));
	// 4 components
	this->Verify(!float4::less4_any(v2222, v1222));    
	this->Verify( float4::less4_any(v1111, v1222));    
	this->Verify( float4::less4_any(v1222, v2222));    
	this->Verify(!float4::less4_all(v1111, v1222));
	this->Verify( float4::less4_all(v0000, v1222));
	this->Verify( float4::lessequal4_any(v1111, v1222));
	this->Verify(!float4::lessequal4_any(v1222, v0000));
	this->Verify( float4::lessequal4_all(v1111, v1222));
	this->Verify( float4::lessequal4_all(v1111, v1111));
	this->Verify(!float4::lessequal4_all(v1222, v1111));
	this->Verify(!float4::greater4_any(v1111, v1111));
	this->Verify(!float4::greater4_any(v0000, v1111));
	this->Verify( float4::greater4_any(v1222, v1111));
	this->Verify(!float4::greater4_all(v1222, v1111));
	this->Verify( float4::greater4_all(v1111, v0000));
	this->Verify( float4::greaterequal4_any(v1222, v2222));
	this->Verify(!float4::greaterequal4_any(v0000, v1111));
	this->Verify( float4::greaterequal4_any(v0000, v0000));
	this->Verify( float4::greaterequal4_all(v0000, v0000));
	this->Verify(!float4::greaterequal4_all(v0000, v0011));
	this->Verify( float4::greaterequal4_all(v1111, v0000));
	this->Verify( float4::equal4_any(v0011, v0000));
	this->Verify(!float4::equal4_any(v1222, v0000));
	this->Verify(!float4::equal4_all(v0011, v0000));
	this->Verify(!float4::equal4_all(v1222, v0000));
	this->Verify( float4::equal4_all(v0000, v0000));
	this->Verify( float4::nearequal4(v0000, float4( 0.001f, 0.1f, 0.05f, 3.0f), float4(0.001f, 0.1f, 0.05f, 3.0f)));
	this->Verify( float4::nearequal4(v0000, float4(-0.001f,-0.1f,-0.05f,-3.0f), float4(0.001f, 0.1f, 0.05f, 3.0f)));
	this->Verify( !float4::nearequal4(v0000, float4( 0.002f, 0.1f, 0.05f, 3.0f), float4(0.001f, 0.1f, 0.05f, 3.0f)));
	this->Verify( !float4::nearequal4(v0000, float4(-0.001f,-0.2f,-0.05f,-3.0f), float4(0.001f, 0.1f, 0.05f, 3.0f)));

    // check splat
    v1.set(1.0f, 2.0f, 3.0f, 4.0f);
    v0 = float4::splat(v1, 0);
    this->Verify(v0 == float4(1.0f, 1.0f, 1.0f, 1.0f));
    v0 = float4::splat_x(v1);
    this->Verify(v0 == float4(1.0f, 1.0f, 1.0f, 1.0f));
    v0 = float4::splat(v1, 1);
    this->Verify(v0 == float4(2.0f, 2.0f, 2.0f, 2.0f));
    v0 = float4::splat_y(v1);
    this->Verify(v0 == float4(2.0f, 2.0f, 2.0f, 2.0f));
    v0 = float4::splat(v1, 2);
    this->Verify(v0 == float4(3.0f, 3.0f, 3.0f, 3.0f));
    v0 = float4::splat_z(v1);
    this->Verify(v0 == float4(3.0f, 3.0f, 3.0f, 3.0f));
    v0 = float4::splat(v1, 3);
    this->Verify(v0 == float4(4.0f, 4.0f, 4.0f, 4.0f));
    v0 = float4::splat_w(v1);
    this->Verify(v0 == float4(4.0f, 4.0f, 4.0f, 4.0f));
}

}