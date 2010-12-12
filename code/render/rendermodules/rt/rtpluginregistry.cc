//------------------------------------------------------------------------------
//  renderthreadpluginregistry.cc
//  (C) 2009 Radon Labs GmbH
//------------------------------------------------------------------------------
#include "stdneb.h"
#include "rendermodules/rt/rtpluginregistry.h"

namespace RenderModules
{
__ImplementClass(RenderModules::RTPluginRegistry, 'RXRG', Core::RefCounted);
__ImplementSingleton(RenderModules::RTPluginRegistry);

using namespace InternalGraphics;

//------------------------------------------------------------------------------
/**
*/ 
RTPluginRegistry::RTPluginRegistry() :
    isValid(false)
{
    __ConstructSingleton;
}

//------------------------------------------------------------------------------
/**
*/
RTPluginRegistry::~RTPluginRegistry()
{
    n_assert(!this->IsValid());
    __DestructSingleton;
}

//------------------------------------------------------------------------------
/**
*/
void
RTPluginRegistry::Setup()
{
    n_assert(!this->IsValid());
    this->isValid = true;
}

//------------------------------------------------------------------------------
/**
*/
void
RTPluginRegistry::Discard()
{
    n_assert(this->IsValid());

    // unregister pluginss...
    IndexT i;
    for (i = 0; i < this->plugins.Size(); i++)
    {
        this->plugins[i]->OnUnregister();
    }
    this->plugins.Clear();
    this->isValid = false;
}

//------------------------------------------------------------------------------
/**
*/
void
RTPluginRegistry::RegisterRTPlugin(const Core::Rtti* rtti)
{
    n_assert(this->IsValid());
    n_assert(0 != rtti);
    n_assert(rtti->IsDerivedFrom(RTPlugin::RTTI));
    if (InvalidIndex == this->FindPlugin(rtti))
    {
        Ptr<RTPlugin> ext = (RTPlugin*) rtti->Create();
        this->plugins.Append(ext);
        ext->OnRegister();
    }
}

//------------------------------------------------------------------------------
/**
*/
void
RTPluginRegistry::UnregisterRTPlugin(const Core::Rtti* rtti)
{
    n_assert(this->IsValid());
    n_assert(0 != rtti);
    IndexT extIndex = this->FindPlugin(rtti);
    if (InvalidIndex != extIndex)
    {
        this->plugins[extIndex]->OnUnregister();
        this->plugins.EraseIndex(extIndex);
    }
}

//------------------------------------------------------------------------------
/**
*/
IndexT
RTPluginRegistry::FindPlugin(const Core::Rtti* rtti)
{
    n_assert(this->IsValid());

    // need to do linear search because the order of plugins is 
    // important (must remain in order of registration)
    IndexT i;
    for (i = 0; i < this->plugins.Size(); i++)
    {
        if (this->plugins[i]->GetRtti() == rtti)
        {
            return i;
        }
    }
    // fallthrough: plugin not found 
    return InvalidIndex;
}

//------------------------------------------------------------------------------
/**
*/
void
RTPluginRegistry::OnStageCreated(const Ptr<InternalStage>& stage)
{
    n_assert(this->IsValid());
    IndexT i;
    for (i = 0; i < this->plugins.Size(); i++)
    {
        this->plugins[i]->OnStageCreated(stage);
    }
}

//------------------------------------------------------------------------------
/**
*/
void
RTPluginRegistry::OnDiscardStage(const Ptr<InternalStage>& stage)
{
    n_assert(this->IsValid());
    IndexT i;
    for (i = 0; i < this->plugins.Size(); i++)
    {
        this->plugins[i]->OnDiscardStage(stage);
    }
}

//------------------------------------------------------------------------------
/**
*/
void
RTPluginRegistry::OnViewCreated(const Ptr<InternalView>& view)
{
    n_assert(this->IsValid());
    IndexT i;
    for (i = 0; i < this->plugins.Size(); i++)
    {
        this->plugins[i]->OnViewCreated(view);
    }
}

//------------------------------------------------------------------------------
/**
*/
void
RTPluginRegistry::OnDiscardView(const Ptr<InternalView>& view)
{
    n_assert(this->IsValid());
    IndexT i;
    for (i = 0; i < this->plugins.Size(); i++)
    {
        this->plugins[i]->OnDiscardView(view);
    }
}

//------------------------------------------------------------------------------
/**
*/
void
RTPluginRegistry::OnAttachEntity(const Ptr<InternalGraphicsEntity>& entity)
{
    n_assert(this->IsValid());
    IndexT i;
    for (i = 0; i < this->plugins.Size(); i++)
    {
        this->plugins[i]->OnAttachEntity(entity);
    }
}

//------------------------------------------------------------------------------
/**
*/
void
RTPluginRegistry::OnRemoveEntity(const Ptr<InternalGraphicsEntity>& entity)
{
    n_assert(this->IsValid());
    IndexT i;
    for (i = 0; i < this->plugins.Size(); i++)
    {
        this->plugins[i]->OnRemoveEntity(entity);
    }
}

//------------------------------------------------------------------------------
/**
*/
void
RTPluginRegistry::OnUpdateBefore(IndexT frameId, Timing::Time time)
{
    n_assert(this->IsValid());
    IndexT i;
    for (i = 0; i < this->plugins.Size(); i++)
    {
        this->plugins[i]->OnUpdateBefore(frameId, time);
    }
}

//------------------------------------------------------------------------------
/**
*/
void
RTPluginRegistry::OnUpdateAfter(IndexT frameId, Timing::Time time)
{
    n_assert(this->IsValid());
    IndexT i;
    for (i = 0; i < this->plugins.Size(); i++)
    {
        this->plugins[i]->OnUpdateAfter(frameId, time);
    }
}

//------------------------------------------------------------------------------
/**
*/
void
RTPluginRegistry::OnRenderBefore(IndexT frameId, Timing::Time time)
{
    n_assert(this->IsValid());
    IndexT i;
    for (i = 0; i < this->plugins.Size(); i++)
    {
        this->plugins[i]->OnRenderBefore(frameId, time);
    }
}

//------------------------------------------------------------------------------
/**
*/
void
RTPluginRegistry::OnRenderAfter(IndexT frameId, Timing::Time time)
{
    n_assert(this->IsValid());
    IndexT i;
    for (i = 0; i < this->plugins.Size(); i++)
    {
        this->plugins[i]->OnRenderAfter(frameId, time);
    }
}

//------------------------------------------------------------------------------
/**
*/
void
RTPluginRegistry::OnRenderFrameBatch(const Ptr<Frame::FrameBatch>& frameBatch)
{
    n_assert(this->IsValid());
    IndexT i;
    for (i = 0; i < this->plugins.Size(); i++)
    {
        this->plugins[i]->OnRenderFrameBatch(frameBatch);
    }
}

//------------------------------------------------------------------------------
/**
*/
void
RTPluginRegistry::OnRenderWithoutView(IndexT frameId, Timing::Time time)
{
    n_assert(this->IsValid());
    IndexT i;
    for (i = 0; i < this->plugins.Size(); i++)
    {
        this->plugins[i]->OnRenderWithoutView(frameId, time);
    }
}

} // namespace RenderModules