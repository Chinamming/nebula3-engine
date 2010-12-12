//------------------------------------------------------------------------------
//  textelement.cc
//  (C) 2008 Radon Labs GmbH
//------------------------------------------------------------------------------
#include "stdneb.h"
#include "coregraphics/textelement.h"

namespace CoreGraphics
{
using namespace Threading;
using namespace Util;
using namespace Math;

//------------------------------------------------------------------------------
/**
*/
TextElement::TextElement() :
    threadId(Threading::InvalidThreadId),
    color(1.0f, 1.0f, 1.0f, 1.0f),
    pos(0.0f, 0.0f)
{
    // empty
}

//------------------------------------------------------------------------------
/**
*/
TextElement::TextElement(ThreadId threadId_, const String& text_, const float4& color_, const float2& pos_) :
    threadId(threadId_),
    text(text_),
    color(color_),
    pos(pos_)
{
    // empty
}

} // namespace CoreGraphics
