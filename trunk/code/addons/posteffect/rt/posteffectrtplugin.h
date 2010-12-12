//------------------------------------------------------------------------------
/**
    @class PostEffect::PostEffectRTPlugin
    
    Setup render-thread environment for the posteffect subsystem

    (C) 2009 Radon Labs GmbH
*/

#include "posteffect/rt/posteffecthandler.h"
#include "posteffect/rt/posteffectserver.h"
#include "rendermodules/rt/rtplugin.h"

namespace PostEffect
{
class PostEffectRTPlugin : public RenderModules::RTPlugin
{
__DeclareClass(PostEffectRTPlugin);

public:
    /// Constructor
    PostEffectRTPlugin();
    /// Destructor
    virtual ~PostEffectRTPlugin();
    
    /// Called when registered on the render-thread side
    virtual void OnRegister();
    /// Called when unregistered from the render-thread side
    virtual void OnUnregister();
    /// Called before rendering
    virtual void OnRenderBefore(IndexT frameId, Timing::Time time);

private:
    
    Ptr<PostEffectServer> postEffectServer;
    Ptr<PostEffectHandler> postEffectHandler;

};
} // namespace PostEffect