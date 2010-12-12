#pragma once
//------------------------------------------------------------------------------
/**
    @class FX::FxHandler
    
    Handles visualization effects from requested from other threads.
    
    (C) 2009 Radon Labs GmbH
*/
#include "interface/interfacehandlerbase.h"
#include "fx/fxprotocol.h"

//------------------------------------------------------------------------------
namespace FX
{
class FxHandler : public Interface::InterfaceHandlerBase
{
    __DeclareClass(FxHandler);

public:
    /// constructor
    FxHandler();
    /// destructor
    virtual ~FxHandler();    
    /// handle a message, return true if handled
    virtual bool HandleMessage(const Ptr<Messaging::Message>& msg);

private:

    /// attach an effectmixer
    void OnAttachEffectMixer(const Ptr<FX::AttachEffectMixer>& msg);
    /// attach an effect
    void OnAttachEffect(const Ptr<FX::AttachEffect>& msg);
};

} // namespace FX
//------------------------------------------------------------------------------