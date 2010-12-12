//------------------------------------------------------------------------------
//  renderapplication.cc
//  (C) 2008 Radon Labs GmbH
//------------------------------------------------------------------------------
#include "stdneb.h"
#include "render_classregistry.h"
#include "apprender/renderapplication.h"
#include "io/logfileconsolehandler.h"
#include "memory/debug/memorypagehandler.h"
#include "core/debug/corepagehandler.h"
#include "core/debug/stringatompagehandler.h"
#include "io/debug/iopagehandler.h"
#include "http/debug/svgtestpagehandler.h"
#include "http/debug/helloworldrequesthandler.h"
#include "threading/debug/threadpagehandler.h"
#include "io/debug/consolepagehandler.h"
#include "resources/simpleresourcemapper.h"
#include "coregraphics/streamtextureloader.h"
#include "coregraphics/streammeshloader.h"
#include "coreanimation/streamanimationloader.h"
#include "coreanimation/animresource.h"
#include "coreanimation/managedanimresource.h"

#if __WIN32__
#include "system/win32/win32registry.h"
#endif

namespace App
{
using namespace Core;
using namespace Debug;
using namespace IO;
using namespace Graphics;
using namespace Input;
using namespace CoreGraphics;
using namespace Timing;
using namespace Util;
using namespace Jobs;
using namespace FrameSync;
using namespace Resources;

//------------------------------------------------------------------------------
/**
*/
RenderApplication::RenderApplication() :
    time(0.0),
    frameTime(0.0),
    quitRequested(false),
    logFileEnabled(true),
    mountStandardArchivesEnabled(true)
{
    // empty
}

//------------------------------------------------------------------------------
/**
*/
RenderApplication::~RenderApplication()
{
    n_assert(!this->IsOpen());
}

//------------------------------------------------------------------------------
/**
*/
String
RenderApplication::LookupProjectDirectory()
{
    String projectDir = "home:";
    #if __WIN32__
        if (System::Win32Registry::Exists(System::Win32Registry::CurrentUser, "Software\\RadonLabs\\Toolkit", "project"))
        {
            projectDir = System::Win32Registry::ReadString(System::Win32Registry::CurrentUser, "Software\\RadonLabs\\Toolkit", "project");
        }
    #endif
    return projectDir;
}

//------------------------------------------------------------------------------
/**
*/
bool
RenderApplication::Open()
{
    n_assert(!this->IsOpen());
    if (Application::Open())
    {
        // check if a toolkit project key is set, if yes, use this as root directory
        String rootDir;
        if (this->overrideRootDirectory.IsValid())
        {
            rootDir = this->overrideRootDirectory;
        }
        else
        {
            rootDir = "home:";
        }

        // setup core subsystem
        this->coreServer = CoreServer::Create();
        this->coreServer->SetCompanyName(this->companyName);
        this->coreServer->SetAppName(this->appName);
        this->coreServer->SetRootDirectory(rootDir);
        this->coreServer->Open();

        // setup the job system
        this->jobSystem = JobSystem::Create();
        this->jobSystem->Setup();

        // setup io subsystem
        this->gameContentServer = GameContentServer::Create(); 
        this->gameContentServer->SetTitle(this->GetAppTitle());
        this->gameContentServer->SetTitleId(this->GetAppID());
        this->gameContentServer->SetVersion(this->GetAppVersion());
        this->gameContentServer->Setup();
        this->ioServer = IoServer::Create();
        if (this->mountStandardArchivesEnabled)
        {
            this->ioServer->MountStandardArchives();
        }
        this->ioInterface = IoInterface::Create();
        this->ioInterface->Open();

        // attach a log file console handler
        #if __WIN32__
        if (this->logFileEnabled)
        {
            Ptr<LogFileConsoleHandler> logFileHandler = LogFileConsoleHandler::Create();
            Console::Instance()->AttachHandler(logFileHandler.upcast<ConsoleHandler>());
        }
        #endif
            
        #if __NEBULA3_HTTP_FILESYSTEM__
            // setup http subsystem
            this->httpClientRegistry = Http::HttpClientRegistry::Create();
            this->httpClientRegistry->Setup();
        #endif
                
        #if __NEBULA3_HTTP__
            this->httpInterface = Http::HttpInterface::Create();
            this->httpInterface->Open();
            this->httpServerProxy = Http::HttpServerProxy::Create();
            this->httpServerProxy->Open();
            this->httpServerProxy->AttachRequestHandler(Debug::CorePageHandler::Create());
            this->httpServerProxy->AttachRequestHandler(Debug::StringAtomPageHandler::Create());
            this->httpServerProxy->AttachRequestHandler(Debug::ThreadPageHandler::Create());
            this->httpServerProxy->AttachRequestHandler(Debug::MemoryPageHandler::Create());
            this->httpServerProxy->AttachRequestHandler(Debug::ConsolePageHandler::Create());
            this->httpServerProxy->AttachRequestHandler(Debug::IoPageHandler::Create());
            this->httpServerProxy->AttachRequestHandler(Debug::SvgTestPageHandler::Create());
            this->httpServerProxy->AttachRequestHandler(Debug::HelloWorldRequestHandler::Create()); 
            
            // setup debug subsystem, needs http system for visualiuzation in browser
            this->debugInterface = DebugInterface::Create();
            this->debugInterface->Open();
        #endif    

        // setup asynchronous graphics subsystem
        this->graphicsInterface = GraphicsInterface::Create();
        this->graphicsInterface->Open();
        this->display = Display::Create();
        this->OnConfigureDisplay();
        this->OnSetupResourceMappers();
        this->display->Open();

        // setup frame sync timer (must happen after graphics exists)
        this->frameSyncTimer = FrameSyncTimer::Create();
        this->frameSyncTimer->Setup();

        // setup input subsystem
        this->inputServer = InputServer::Create();
        this->inputServer->Open();

        // setup debug timers and counters
        _setup_timer(MainThreadFrameTimeAll);
        _setup_timer(MainThreadWaitForGraphicsFrame);

        return true;
    }
    return false;
}

//------------------------------------------------------------------------------
/**
*/
void
RenderApplication::OnConfigureDisplay()
{
    // display adapter
    Adapter::Code adapter = Adapter::Primary;
    if (this->args.HasArg("-adapter"))
    {
        adapter = Adapter::FromString(this->args.GetString("-adapter"));
        if (this->display->AdapterExists(adapter))
        {
            this->display->Settings().SetAdapter(adapter);
        }
    }

    // optional parent window handle, under Win32, this must be a valid HWND!
    if (this->args.HasArg("-parentwindow"))
    {
        this->display->SetParentWindow((void*)this->args.GetInt("-parentwindow"));
    }

    // display mode
    DisplayMode displayMode;
    if (this->args.HasArg("-x"))
    {
        displayMode.SetXPos(this->args.GetInt("-x"));
    }
    if (this->args.HasArg("-y"))
    {
        displayMode.SetYPos(this->args.GetInt("-y"));
    }
    if (this->args.HasArg("-w"))
    {
        displayMode.SetWidth(this->args.GetInt("-w"));
    }
    if (this->args.HasArg("-h"))
    {
        displayMode.SetHeight(this->args.GetInt("-h"));
    }
    displayMode.SetAspectRatio(float(displayMode.GetWidth()) / float(displayMode.GetHeight()));
    this->display->Settings().DisplayMode() = displayMode;
    this->display->Settings().SetFullscreen(this->args.GetBoolFlag("-fullscreen"));
    this->display->Settings().SetAlwaysOnTop(this->args.GetBoolFlag("-alwaysontop"));
    this->display->Settings().SetVerticalSyncEnabled(this->args.GetBoolFlag("-vsync"));
    if (this->args.HasArg("-aa"))
    {
        this->display->Settings().SetAntiAliasQuality(AntiAliasQuality::FromString(this->args.GetString("-aa")));
    }
    #if (__XBOX360__)
        this->display->Settings().SetAntiAliasQuality(AntiAliasQuality::Medium);
    #endif
}

//------------------------------------------------------------------------------
/**
    Configure the resource mapper objects for the render thread.
    NOTE: ResourceMapper objects are created and configured here (the 
    main thread) and then HANDED OVER to the render thread. DO NOT access
    ResourceMappers after Display::Open() is called. It's best to create
    ResourceMappers and then immediately forget about them.
*/
void
RenderApplication::OnSetupResourceMappers()
{
    Array<Ptr<ResourceMapper> > resourceMappers;

    // setup resource mapper for textures
    Ptr<SimpleResourceMapper> texMapper = SimpleResourceMapper::Create();
    texMapper->SetPlaceholderResourceId(ResourceId(NEBULA3_PLACEHOLDER_TEXTURENAME));
    texMapper->SetResourceClass(Texture::RTTI);
    texMapper->SetResourceLoaderClass(CoreGraphics::StreamTextureLoader::RTTI);
    texMapper->SetManagedResourceClass(ManagedTexture::RTTI);
    resourceMappers.Append(texMapper.upcast<ResourceMapper>());

    // setup resource mapper for meshes
    Ptr<SimpleResourceMapper> meshMapper = SimpleResourceMapper::Create();
    meshMapper->SetPlaceholderResourceId(ResourceId(NEBULA3_PLACEHOLDER_MESHNAME));
    meshMapper->SetResourceClass(Mesh::RTTI);
    meshMapper->SetResourceLoaderClass(CoreGraphics::StreamMeshLoader::RTTI);
    meshMapper->SetManagedResourceClass(ManagedMesh::RTTI);
    resourceMappers.Append(meshMapper.upcast<ResourceMapper>());

    // setup resource mapper for animations
    // FIXME: should be configurable!
    Ptr<SimpleResourceMapper> animMapper = SimpleResourceMapper::Create();
    animMapper->SetResourceClass(CoreAnimation::AnimResource::RTTI);
    animMapper->SetResourceLoaderClass(CoreAnimation::StreamAnimationLoader::RTTI);
    animMapper->SetManagedResourceClass(CoreAnimation::ManagedAnimResource::RTTI);
    resourceMappers.Append(animMapper.upcast<ResourceMapper>());

    this->display->SetResourceMappers(resourceMappers);
}

//------------------------------------------------------------------------------
/**
*/
void
RenderApplication::Close()
{
    n_assert(this->IsOpen());
    
    _discard_timer(MainThreadFrameTimeAll);
    _discard_timer(MainThreadWaitForGraphicsFrame);

    this->inputServer->Close();
    this->inputServer = 0;

    this->display->Close();
    this->display = 0;

    this->graphicsInterface->Close();
    this->graphicsInterface = 0;
     
    this->ioInterface->Close();
    this->ioInterface = 0;
    this->ioServer = 0;
    this->gameContentServer->Discard();
    this->gameContentServer = 0;

    this->frameSyncTimer->Discard();
    this->frameSyncTimer = 0;

    this->jobSystem->Discard();
    this->jobSystem = 0;

#if __NEBULA3_HTTP__           
    this->debugInterface->Close();
    this->debugInterface = 0;

    this->httpServerProxy->Close();
    this->httpServerProxy = 0;
    this->httpInterface->Close();
    this->httpInterface = 0;
#endif

#if __NEBULA3_HTTP_FILESYSTEM__
    this->httpClientRegistry->Discard();
    this->httpClientRegistry = 0;
#endif

    this->coreServer->Close();
    this->coreServer = 0;

    Application::Close();
}

//------------------------------------------------------------------------------
/**
*/
void
RenderApplication::Run()
{
    GraphicsInterface::Instance()->EnterLockStepMode();

    n_assert(this->isOpen);
    while (!(this->inputServer->IsQuitRequested() || this->IsQuitRequested()))
    {
        _start_timer(MainThreadFrameTimeAll);

        // synchronize with the graphics frame, to prevent the game thread
        // to run ahead of the graphics thread
        _start_timer(MainThreadWaitForGraphicsFrame);
        GraphicsInterface::Instance()->GameThreadWaitForFrameSync();
        _stop_timer(MainThreadWaitForGraphicsFrame);
             
#if __NEBULA3_HTTP__
        // handle any http requests from the HttpServer thread
        this->httpServerProxy->HandlePendingRequests();
#endif

        // process input
        this->inputServer->BeginFrame();
        this->inputServer->OnFrame();
        this->OnProcessInput();

        // update time
        this->UpdateTime();

        // run "game logic"
        this->OnUpdateFrame();

        // send batched graphics messages
        GraphicsInterface::Instance()->FlushBatchedMessages();

        this->inputServer->EndFrame();

        _stop_timer(MainThreadFrameTimeAll);
    }
    GraphicsInterface::Instance()->LeaveLockStepMode();
}

//------------------------------------------------------------------------------
/**
*/
void
RenderApplication::OnProcessInput()
{
    // empty, override this method in a subclass
}

//------------------------------------------------------------------------------
/**
*/
void
RenderApplication::OnUpdateFrame()
{
    // empty, override this method in a subclass
}

//------------------------------------------------------------------------------
/**
*/
void
RenderApplication::UpdateTime()
{    
    this->frameTime = this->frameSyncTimer->GetFrameTime();
    this->time = this->frameSyncTimer->GetTime();
}

} // namespace App
