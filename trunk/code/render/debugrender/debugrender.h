#pragma once
//------------------------------------------------------------------------------
/**
    Macros for debug rendering.

    (C) 2008 Radon Labs GmbH
*/

// debug text render macro
#include "debugrender/debugrenderprotocol.h"
#include "graphics/graphicsinterface.h"
#include "debugrender/debugtextrenderer.h"

#if (NEBULA3_DEBUG || __WIN32__ || __XBOX360__)
#define _debug_text(txt, pos, color) { Debug::DebugTextRenderer::Instance()->DrawText(txt, color, pos); }
#define _debug_text3D(txt, pos, color) { Debug::DebugTextRenderer::Instance()->DrawText3D(txt, color, pos); }
#define _debug_text_fade(txt, pos, color, time) { Debug::DebugTextRenderer::Instance()->DrawTextFade(txt, color, pos, time); }
#else
#define _debug_text(txt, pos, color)
#define _debug_text3D(txt, pos, color)
#define _debug_text_fade(txt, pos, color, time)
#endif
