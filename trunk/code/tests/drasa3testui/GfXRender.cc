//------------------------------------------------------------------------------
//  suirendermodule.cc
//  (C) 2009 Radon Labs GmbH
//------------------------------------------------------------------------------
#include "stdneb.h"
#include "GFXRender.h"
#include "coregraphics/renderdevice.h"

namespace ScaleForms
{
__ImplementClass(ScaleForms::GfxRender, 'GFRR', Core::RefCounted);
__ImplementSingleton(ScaleForms::GfxRender);

using namespace Util;
using namespace CoreGraphics;
using namespace Input;

//------------------------------------------------------------------------------
/**
*/
GfxRender::GfxRender() 
: pStateBlock(NULL)
, isValid(false)
{
    __ConstructSingleton;
}

//------------------------------------------------------------------------------
/**
*/
GfxRender::~GfxRender()
{
    __DestructSingleton;
}

//------------------------------------------------------------------------------
/**
*/
void
GfxRender::Setup()
{
	n_assert(!this->IsValid());

	this->gfxTutorial = n_new(GFxTutorial);
	bool isLoad = this->gfxTutorial->InitGFx();
	n_assert(isLoad!=NULL);

	// Create a state block
	RenderDevice* renderDevice = RenderDevice::Instance();
	renderDevice->GetDirect3DDevice()->CreateStateBlock( D3DSBT_ALL, &pStateBlock );
	n_assert(pStateBlock != NULL);
	this->gfxTutorial->OnResetDevice();

	this->isValid = true;
}

//------------------------------------------------------------------------------
/**
*/
void
GfxRender::Discard()
{
    n_assert(this->IsValid());

	n_delete(this->gfxTutorial);
	if( pStateBlock!=NULL)
	{
		pStateBlock->Release();
		pStateBlock = NULL;
	}
	this->isValid = false;
}


//------------------------------------------------------------------------------
/**
*/
void
GfxRender::Render(const Ptr<Frame::FrameBatch>& frameBatch)
{    
	n_assert(pStateBlock != NULL);

	// Save DirectX state before calling GFx
	pStateBlock->Capture();
	// Render the frame and advance the time counter
	if(this->gfxTutorial!=NULL)
		this->gfxTutorial->AdvanceAndRender();
	// Restore DirectX state to avoid disturbing game render state
	pStateBlock->Apply();
}

//------------------------------------------------------------------------------
/**
*/
bool 
GfxRender::ProcessInputEvents(const Util::Array<Input::InputEvent>& inputEvents)
{	
	IndexT i;
	for (i = 0; i < inputEvents.Size(); i++)
	{
		Input::InputEvent& inputEvent =  inputEvents[i];
		if( inputEvent.GetType() == InputEvent::MouseMove )
		{
			Math::float2 position = inputEvent.GetAbsMousePos();
			GFxMouseEvent mevent(GFxEvent::MouseMove, 0, (Float)position.x(), (Float)position.y());
			this->gfxTutorial->ProcessEvent(mevent);
		}
	}

	return true;
}


} // namespace SUI