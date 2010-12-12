#pragma once
//------------------------------------------------------------------------------
/**
    @class Frame::SortingMode
    
    The sorting mode to perform when rendering a frame batch.
    
    (C) 2007 Radon Labs GmbH
*/
#include "util/string.h"

//------------------------------------------------------------------------------
namespace Frame
{
class SortingMode
{
public:
    /// sorting mode enum
    enum Code
    {
        None,           // no sorting
        FrontToBack,    // perform front-to-back sorting
        BackToFront,    // perform back-to-front sorting
    };

    /// convert from string
    static Code FromString(const Util::String& str);
    /// convert to string
    static Util::String ToString(Code c);
};

} // namespace Frame
//------------------------------------------------------------------------------

    