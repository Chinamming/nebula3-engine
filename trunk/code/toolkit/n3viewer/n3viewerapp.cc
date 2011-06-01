//------------------------------------------------------------------------------
//  n3viewerapp.cc
//  (C) 2009 Radon Labs GmbH
//------------------------------------------------------------------------------
#include "stdneb.h"
#include "n3viewerapp.h"
#include "system/win32/win32registry.h"
#include "input/keyboard.h"

namespace Toolkit
{
using namespace IO;
using namespace Util;
using namespace App;
using namespace Remote;
using namespace Graphics;
using namespace Math;
using namespace System;
using namespace Input;

__ImplementSingleton(Toolkit::N3ViewerApp);

//------------------------------------------------------------------------------
/**
*/
N3ViewerApp::N3ViewerApp()
{
    __ConstructSingleton;
}

//------------------------------------------------------------------------------
/**
*/
N3ViewerApp::~N3ViewerApp()
{
    if (this->IsOpen())
    {
        this->Close();
    }
    __DestructSingleton;
}

//------------------------------------------------------------------------------
/**
*/
bool
N3ViewerApp::Open()
{
    if (ViewerApplication::Open())
    {
        // setup remote subsystem
        this->remoteInterface = RemoteInterface::Create();
        if (this->args.HasArg("-commandport"))
        {
            this->remoteInterface->SetPortNumber(this->args.GetInt("-commandport",2102));
        }
        this->remoteInterface->Open();
        this->remoteControlProxy = RemoteControlProxy::Create();
        this->remoteControlProxy->Open();
        this->remoteControlProxy->AttachCommandHandler(N3ViewerRemoteCommandHandler::Create());

        // setup the lights
        this->SetupLights();

        // parse command line args
        this->SetupSceneFromCmdLineArgs();

        return true;
    }
    else
    {
        return false;
    }
}

//------------------------------------------------------------------------------
/**
*/
void
N3ViewerApp::Close()
{
    // discard scene objects
    this->DiscardModels();
    this->DiscardLights();

    // shutdown remote system
    this->remoteControlProxy->Close();
    this->remoteControlProxy = 0;
    this->remoteInterface->Close();
    this->remoteInterface = 0;

    ViewerApplication::Close();
}

//------------------------------------------------------------------------------
/**
*/
void
N3ViewerApp::OnConfigureDisplay()
{
    // setup assigns, system resources and shaders need to be loaded
    // from the toolkit directory, while all other resources must
    // be loaded from the project directory
    String exportDir = AssignRegistry::Instance()->GetAssign("export").Tokenize(":/").Back();
    String homeShdPath, homeFramePath, homeMshPath, homeTexPath, homeAnimPath;
    homeShdPath.Format("home:%s/shaders", exportDir.AsCharPtr());
    homeFramePath.Format("home:%s/frame", exportDir.AsCharPtr());
    homeMshPath.Format("home:%s/meshes", exportDir.AsCharPtr());
    homeTexPath.Format("home:%s/textures", exportDir.AsCharPtr());
    homeAnimPath.Format("home:%s/anims", exportDir.AsCharPtr());
        
    AssignRegistry* assReg = AssignRegistry::Instance();
    assReg->SetAssign(Assign("shd", homeShdPath));
    assReg->SetAssign(Assign("frame", homeFramePath));
    assReg->SetAssign(Assign("sysmsh", homeMshPath));
    assReg->SetAssign(Assign("systex", homeTexPath));
    assReg->SetAssign(Assign("sysanim", homeAnimPath));

    String projDir = this->QueryProjectPathFromRegistry();
    assReg->SetAssign(Assign("root", projDir));

    this->display->Settings().SetDisplayModeSwitchEnabled(false);

    ViewerApplication::OnConfigureDisplay();
}

//------------------------------------------------------------------------------
/**
*/
void
N3ViewerApp::OnProcessInput()
{
    // handle remote commands
    this->remoteControlProxy->HandlePendingRequests();
    this->remoteControlProxy->HandlePendingCommands();

	InputServer* inputServer = InputServer::Instance();
	const Ptr<Keyboard>& keyboard = inputServer->GetDefaultKeyboard();

	if( keyboard->KeyPressed(Key::F1))
	{
		Ptr<Debug::RenderDebugView> msg = Debug::RenderDebugView::Create();
		msg->SetEnableDebugRendering(true);
		GraphicsInterface::Instance()->Send(msg.cast<Messaging::Message>());
	}
	else if( keyboard->KeyPressed(Key::F2))
	{
		this->LoadAnimation(this->args.GetString("-view"),"animatior_clip");
	}
	else if( keyboard->KeyPressed(Key::F3))
	{
		static bool isload = false;
		if(!isload)
		{
			this->LoadSkin(this->args.GetString("-view"),"skinNode");
			isload = true;
		}
		else
		{
			this->RemoveSkin(this->args.GetString("-view"),"skinNode");
			isload = false;
		}
	}

    ViewerApplication::OnProcessInput();
}

//------------------------------------------------------------------------------
/**
*/
void
N3ViewerApp::OnUpdateFrame()
{
    ViewerApplication::OnUpdateFrame();
}

//------------------------------------------------------------------------------
/**
*/
void
N3ViewerApp::SetupSceneFromCmdLineArgs()
{
    // setup the initial camera position
    Math::point eyePos = this->args.GetFloat4("-eyepos", Math::point(0.0f, 0.0f, 9.0f));
    Math::point eyeCoi = this->args.GetFloat4("-eyecoi", Math::point(0.0f, 0.0f, 0.0f));
    Math::vector eyeUp = this->args.GetFloat4("-eyeupx", Math::vector(0.0f, 1.0f, 0.0f));
    this->mayaCameraUtil.Setup(eyeCoi, eyePos, eyeUp);

    // read model argument and load object
    if (this->args.HasArg("-view"))
    {
        this->LoadModel(this->args.GetString("-view"));

		if (this->args.HasArg("-anim"))
		{
			//this->LoadAnimation(this->args.GetString("-view"),this->args.GetString("-anim"));
		}
    }
}

//------------------------------------------------------------------------------
/**
*/
void
N3ViewerApp::SetupLights()
{
    n_assert(!this->globalLightEntity.isvalid());

    // setup the global light
    matrix44 lightTransform = matrix44::rotationx(n_deg2rad(-45.0f));
    this->globalLightEntity = GlobalLightEntity::Create();
    this->globalLightEntity->SetTransform(lightTransform);
    this->globalLightEntity->SetColor(float4(1.0f, 1.0f, 1.0f, 1.0f));
    this->globalLightEntity->SetBackLightColor(float4(1.0f, 1.0f, 1.0f, 1.0f));
    this->globalLightEntity->SetAmbientLightColor(float4(0.6f, 0.6f, 0.6f, 1.0f));
    this->globalLightEntity->SetCastShadows(false);
    this->stage->AttachEntity(this->globalLightEntity.cast<GraphicsEntity>());
}

//------------------------------------------------------------------------------
/**
*/
void
N3ViewerApp::DiscardLights()
{
    // discard the global light entity
    this->stage->RemoveEntity(this->globalLightEntity.cast<GraphicsEntity>());
    this->globalLightEntity = 0;
}

//------------------------------------------------------------------------------
/**
*/
void
N3ViewerApp::LoadModel(const Resources::ResourceId& resId)
{
    Ptr<ModelEntity> modelEntity = ModelEntity::Create();
    modelEntity->SetResourceId(resId);
    this->stage->AttachEntity(modelEntity.cast<GraphicsEntity>());
    this->modelEntities.Append(modelEntity);
}

//------------------------------------------------------------------------------
/**	
*/
void
N3ViewerApp::LoadSkin(const Resources::ResourceId& modelResId, const Resources::ResourceId& skinResId)
{
    IndexT i;
    for (i = 0; i < this->modelEntities.Size(); i++)
    {
        if (this->modelEntities[i]->GetResourceId() == modelResId)
        {
            // apply skin
            Ptr<Graphics::ShowSkin> showSkin = Graphics::ShowSkin::Create();
            showSkin->SetSkin(skinResId);
            this->modelEntities[i]->SendMsg(showSkin.cast<GraphicsEntityMessage>());
        }
    }
}

//------------------------------------------------------------------------------
/**	
*/
void
N3ViewerApp::RemoveSkin(const Resources::ResourceId& modelResId, const Resources::ResourceId& skinResId)
{
    IndexT i;
    for (i = 0; i < this->modelEntities.Size(); i++)
    {
        if (this->modelEntities[i]->GetResourceId() == modelResId)
        {
            // apply skin
            Ptr<Graphics::HideSkin> hideSkin = Graphics::HideSkin::Create();
            hideSkin->SetSkin(skinResId);
            this->modelEntities[i]->SendMsg(hideSkin.cast<GraphicsEntityMessage>());
        }
    }
}

//------------------------------------------------------------------------------
/**	
*/
void
N3ViewerApp::LoadAnimation(const Resources::ResourceId& modelResId, const Resources::ResourceId& animResId)
{
    IndexT i;
    for (i = 0; i < this->modelEntities.Size(); i++)
    {
        if (this->modelEntities[i]->GetResourceId() == modelResId)
        {
            // replace animation
            Ptr<Graphics::AnimStopAllTracks> stopAll = Graphics::AnimStopAllTracks::Create();
            stopAll->SetAllowFadeOut(false);
            this->modelEntities[i]->SendMsg(stopAll.cast<GraphicsEntityMessage>());
            Ptr<Graphics::AnimPlayClip> playClip = Graphics::AnimPlayClip::Create();
            playClip->SetBlendWeight(1);
            playClip->SetClipName(animResId);
            playClip->SetFadeInTime(0);
            playClip->SetFadeOutTime(0);
            playClip->SetLoopCount(0);
            this->modelEntities[i]->SendMsg(playClip.cast<GraphicsEntityMessage>());
        }
    }
}

//------------------------------------------------------------------------------
/**	
*/
void
N3ViewerApp::LoadVariation(const Resources::ResourceId& modelResId, const Resources::ResourceId& varResId)
{
    IndexT i;
    for (i = 0; i < this->modelEntities.Size(); i++)
    {
        if (this->modelEntities[i]->GetResourceId() == modelResId)
        {
            // replace variation
            Ptr<Graphics::SetVariation> setVar = Graphics::SetVariation::Create();
            setVar->SetVariationName(varResId);
            this->modelEntities[i]->SendMsg(setVar.cast<GraphicsEntityMessage>());
        }
    }
}

//------------------------------------------------------------------------------
/**
*/
void
N3ViewerApp::DiscardModels()
{
    IndexT i;
    for (i = 0; i < this->modelEntities.Size(); i++)
    {
        this->stage->RemoveEntity(this->modelEntities[i].cast<GraphicsEntity>());
    }
    this->modelEntities.Clear();
}

//------------------------------------------------------------------------------
/**
*/
String
N3ViewerApp::QueryProjectPathFromRegistry() const
{
    String projDirectory;
    if (Win32Registry::Exists(Win32Registry::CurrentUser, "Software\\RadonLabs\\Toolkit", "project"))
    {
        projDirectory = Win32Registry::ReadString(Win32Registry::CurrentUser, "Software\\RadonLabs\\Toolkit", "project");
    }
    else
    {
        projDirectory = "home:";
    }
    return projDirectory;
}

} // namespace Toolkit