//------------------------------------------------------------------------------
//  game/server.cc
//  (C) 2003 RadonLabs GmbH
//------------------------------------------------------------------------------
#include "stdneb.h"
#include "game/gameserver.h"
#include "core/factory.h"
#include "appgame/appconfig.h"
#include "input/inputserver.h"
#include "input/keyboard.h"
#include "input/gamepad.h"
#include "debugrender/debugrender.h"

namespace Game
{
__ImplementClass(Game::GameServer, 'GMSV', Core::RefCounted);
__ImplementSingleton(Game::GameServer);

//------------------------------------------------------------------------------
/**
*/
GameServer::GameServer() :
    isOpen(false),
    isStarted(false),
    quitRequested(false)
{
    __ConstructSingleton;
    _setup_timer(GameServerOnFrame);
}

//------------------------------------------------------------------------------
/**
*/
GameServer::~GameServer()
{
    n_assert(!this->isOpen);
    _discard_timer(GameServerOnFrame);
    __DestructSingleton;
}

//------------------------------------------------------------------------------
/**
    Initialize the game server object. This will create and initialize all
    subsystems.
*/
bool
GameServer::Open()
{
    n_assert(!this->isOpen);
    n_assert(!this->isStarted);
    this->isOpen = true;
    return true;
}

//------------------------------------------------------------------------------
/**
    Close the game server object.
*/
void
GameServer::Close()
{
    n_assert(!this->isStarted);
    n_assert(this->isOpen);
    
    // remove all gameFeatures
    while (this->gameFeatures.Size() > 0)
    {
        this->gameFeatures[0]->OnDeactivate();
        this->gameFeatures.EraseIndex(0);
    }
    this->isOpen = false;
}

//------------------------------------------------------------------------------
/**
*/
void
GameServer::AttachGameFeature(const Ptr<FeatureUnit>& feature)
{
    n_assert(0 != feature);
    n_assert(InvalidIndex == this->gameFeatures.FindIndex(feature));
    feature->OnActivate();
    this->gameFeatures.Append(feature);
}

//------------------------------------------------------------------------------
/**
*/
void
GameServer::RemoveGameFeature(const Ptr<FeatureUnit>& feature)
{
    n_assert(0 != feature);
    IndexT index = this->gameFeatures.FindIndex(feature);
    n_assert(InvalidIndex != index);
    feature->OnDeactivate();
    this->gameFeatures.EraseIndex(index);
}

//------------------------------------------------------------------------------
/**
    Start the game world, called after loading has completed.
*/
bool
GameServer::Start()
{
    n_assert(this->isOpen);
    n_assert(!this->isStarted);

    // call the OnStart method on all gameFeatures
    int i;
    int num = this->gameFeatures.Size();
    for (i = 0; i < num; i++)
    {
        this->gameFeatures[i]->OnStart();
    }
    
    this->isStarted = true;
    return true;
}

//------------------------------------------------------------------------------
/**
*/
bool
GameServer::HasStarted() const
{
    return this->isStarted;
}

//------------------------------------------------------------------------------
/**
    Stop the game world, called before the world(current level) is cleaned up.
*/
void
GameServer::Stop()
{
    n_assert(this->isOpen);
    n_assert(this->isStarted);
    
    this->isStarted = false;
}


//------------------------------------------------------------------------------
/**
    Trigger the game server. If your application introduces new or different
    manager objects, you may also want to override the Game::GameServer::Trigger()
    method if those gameFeatures need per-frame callbacks.
*/
void
GameServer::OnFrame()
{
    _start_timer(GameServerOnFrame);

    // call trigger functions on game features   
    int i;
    int num = this->gameFeatures.Size();
    for (i = 0; i < num; i++)
    {
        this->gameFeatures[i]->OnBeginFrame();
    }
    for (i = 0; i < num; i++)
    {
        this->gameFeatures[i]->OnFrame();
    } 
    for (i = 0; i < num; i++)
    {
        this->gameFeatures[i]->OnEndFrame();
    }   

	// debug rendering stuff
	this->CheckDebugRendering();
	if(this->debugRenderFeature.isvalid())
	{
		// set render info
		_debug_text(this->debugRenderFeature->GetRtti()->GetName(), Math::float2(0.5f, 0.004f), Math::float4(1.0f, 1.0f, 0.0f, 1.0f));
		this->debugRenderFeature->OnRenderDebug();
	}

    _stop_timer(GameServerOnFrame);
}

//------------------------------------------------------------------------------
/**    
*/
void
GameServer::CheckDebugRendering()
{
#if !PUBLIC_BUILD
    if (Input::InputServer::HasInstance())
    {
	    // first check input
	    Ptr<Input::InputServer> inputServer = Input::InputServer::Instance();
	    Ptr<Input::Keyboard> keyBoard = inputServer->GetDefaultKeyboard();
        Ptr<Input::GamePad> gamePad = inputServer->GetDefaultGamePad(0);

        if (keyBoard->KeyDown(Input::Key::F4) || gamePad.isvalid() && gamePad->ButtonDown(Input::GamePad::RightThumbButton))
	    {
		    // find current index
		    IndexT index = InvalidIndex;
		    if(this->debugRenderFeature.isvalid())
		    {
                this->debugRenderFeature->StopRenderDebug();
			    index = this->gameFeatures.FindIndex(this->debugRenderFeature) + 1;
		    }
		    else
		    {
			    index = 0;
		    }
		    n_assert(index != InvalidIndex);

		    // find next feature with render debug enabled
		    bool noFeature = true;
		    for (; index < this->gameFeatures.Size(); index++)
		    {
			    if (this->gameFeatures[index]->HasRenderDebug())
			    {                
				    this->debugRenderFeature = this->gameFeatures[index];
                    this->debugRenderFeature->StartRenderDebug();
				    noFeature = false;
				    break;
			    }
		    }

		    // check if no feature found
		    if(noFeature)
		    {
			    this->debugRenderFeature = 0;
		    }
	    }
    }
#endif
}

//------------------------------------------------------------------------------
/**    
*/
void
GameServer::NotifyGameLoad()
{
    // call the OnLoad method on all gameFeatures
    int i;
    int num = this->gameFeatures.Size();
    for (i = 0; i < num; i++)
    {
        this->gameFeatures[i]->OnLoad();
    }
}
//------------------------------------------------------------------------------
/**    
*/
void
GameServer::NotifyGameSave()
{
    // call the OnLoad method on all gameFeatures
    int i;
    int num = this->gameFeatures.Size();
    for (i = 0; i < num; i++)
    {
        this->gameFeatures[i]->OnSave();
    }
}

//------------------------------------------------------------------------------
/**
*/
bool 
GameServer::IsFeatureAttached(const Util::String& stringName) const
{
    int i;
    int num = this->gameFeatures.Size();
    for (i = 0; i < num; i++)
    {
        if (this->gameFeatures[i]->GetRtti()->GetName() == stringName)
        {
            return true;
        }
    }
    return false;
}
} // namespace Game