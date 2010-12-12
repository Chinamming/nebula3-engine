#pragma once
#ifndef TEST_MATRIX44TEST_H
#define TEST_MATRIX44TEST_H
//------------------------------------------------------------------------------
/**
    @class Test::Matrix44Test
    
    Test matrix44 functionality.
    
    (C) 2007 Radon Labs GmbH
*/
#include "testbase/testcase.h"

//------------------------------------------------------------------------------
namespace Test
{
class Matrix44Test : public TestCase
{
    __DeclareClass(Matrix44Test);
public:
    /// run the test
    virtual void Run();
};

}; // namespace Test
//------------------------------------------------------------------------------
#endif        
