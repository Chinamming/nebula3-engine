//------------------------------------------------------------------------------
//  uirendermodule.cc
//  (C) 2009 Radon Labs GmbH
//------------------------------------------------------------------------------
#include "stdneb.h"
#include "GfxRendermodule.h"
#include "GfxRTPlugin.h"
#include "graphics/graphicsinterface.h"
#include "graphics/graphicsprotocol.h"
#include "input/mouse.h"
#include "input/inputevent.h"
#include "input/inputserver.h"

namespace ScaleForms
{
__ImplementClass(ScaleForms::GfxRenderModule, 'GFRM', RenderModules::RenderModule);
__ImplementSingleton(ScaleForms::GfxRenderModule);

using namespace RenderModules;
using namespace Graphics;
using namespace Messaging;
using namespace Util;
using namespace Input;

//------------------------------------------------------------------------------
/**
*/
GfxRenderModule::GfxRenderModule()
{
    __ConstructSingleton;
}

//------------------------------------------------------------------------------
/**
*/
GfxRenderModule::~GfxRenderModule()
{
    __DestructSingleton;
}

//------------------------------------------------------------------------------
/**
*/
void
GfxRenderModule::Setup()
{
    n_assert(!this->IsValid());
    RenderModule::Setup();

    // setup UIRenderer and UIManager
    //this->uiRenderer = UIRenderer::Create();
    //this->uiRenderer->Setup();
    //this->uiManager = UIManager::Create();
    //this->uiManager->Open();

    // register the render-thread plugin
    Ptr<RegisterRTPlugin> msg = RegisterRTPlugin::Create();
    msg->SetType(&ScaleForms::GfxRTPlugin::RTTI);
    GraphicsInterface::Instance()->Send(msg.cast<Message>());
}

//------------------------------------------------------------------------------
/**
*/
void
GfxRenderModule::Discard()
{
    n_assert(this->IsValid());
    RenderModule::Discard();

    // unregister the render-thread plugin
    Ptr<UnregisterRTPlugin> msg = UnregisterRTPlugin::Create();
    msg->SetType(&ScaleForms::GfxRTPlugin::RTTI);
    GraphicsInterface::Instance()->Send(msg.cast<Message>());

    // discard UIRenderer and UIManager
    //this->uiManager->Close();
    //this->uiManager = 0;
    //this->uiRenderer->Discard();
    //this->uiRenderer = 0;
}

//------------------------------------------------------------------------------
/**
*/
void
GfxRenderModule::SendInputEvents(const Array<InputEvent>& inputEvents)
{
    // accumulate input events for one frame, and send them as batch 
    // during OnFrame()
    this->inputEvents.AppendArray(inputEvents);
}

//------------------------------------------------------------------------------
/**
    The OnFrame() method sends input events and receives SUI events.
*/
void
GfxRenderModule::OnFrame()
{
	Ptr<Input::Mouse> mouse = Input::InputServer::Instance()->GetDefaultMouse();

	if (mouse.isvalid())
	{
		// record current mouse position
		this->mousePosition = mouse->GetScreenPosition();

		if(mouse->GetMovement().lengthsq() > N_TINY)
		{
			Input::InputEvent mevent;
			mevent.SetType(InputEvent::MouseMove);
			mevent.SetAbsMousePos(this->mousePosition);
			this->inputEvents.Append(mevent);
		}


	}

    // may not send new message before previous has been handled
    if (!this->processEventsMsg.isvalid() && this->inputEvents.Size() > 0)
    {
        this->processEventsMsg = ProcessEvents::Create();
        this->processEventsMsg->SetInputEvents(this->inputEvents);
        GraphicsInterface::Instance()->Send(this->processEventsMsg.cast<Message>());
        this->inputEvents.Clear();
    }
}

} // namespace UI