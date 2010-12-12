#pragma once
//------------------------------------------------------------------------------
/**
	@class Video::VideoRTPlugin

    Setup render-thread environment for the video system.

    (C) 2009 Radon Labs GmbH
*/

#include "rendermodules/rt/rtplugin.h"
#include "video/rt/videoserver.h"
#include "video/rt/videohandler.h"

namespace Video
{
class VideoRTPlugin : public RenderModules::RTPlugin
{
    __DeclareClass(VideoRTPlugin);
public:
    /// Constructor
    VideoRTPlugin();
    /// Destructor
    virtual ~VideoRTPlugin();
    
    /// Called when registered on the render-thread side
    virtual void OnRegister();
    /// Called when unregistered from the render-thread side
    virtual void OnUnregister();
    /// Called after rendering
    virtual void OnRenderAfter(IndexT frameId, Timing::Time time);
    /// Called before rendering
    virtual void OnRenderBefore(IndexT frameId, Timing::Time time);

private:
    
    Ptr<VideoServer> videoServer;
    Ptr<VideoHandler> videoHandler;
};
} // namespace Video
//------------------------------------------------------------------------------