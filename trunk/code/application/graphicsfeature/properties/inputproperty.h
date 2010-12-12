#pragma once
//------------------------------------------------------------------------------
/**
    @class Properties::InputProperty

    An input property adds the ability to handle user input to an entity.
    If an InputProperty is attached to an entity it can become the input
    focus entity. Global input focus is managed by the Game::FocusManager
    singleton.

    If you want the concept of an input focus in your application you should
    derive your own input property classes from the InputProperty class,
    because then the FocusManager will be aware of it (otherwise it will
    just ignore the entity).
    
    (C) 2007 Radon Labs GmbH
*/
#include "game/property.h"
#include "graphicsattr/graphicsattributes.h"

//------------------------------------------------------------------------------
namespace Attr
{
    DeclareBool(InputFocus, 'IFOC', ReadWrite);
};

//------------------------------------------------------------------------------
namespace GraphicsFeature
{
class InputProperty : public Game::Property
{	
    __DeclareClass(InputProperty);
public:
    /// constructor
    InputProperty();
    /// destructor
    virtual ~InputProperty();
    /// setup accepted messages
    virtual void SetupAcceptedMessages();
    /// setup callbacks for this property
    virtual void SetupCallbacks();
    /// setup default entity attributes
    virtual void SetupDefaultAttributes();
    /// called from Entity::ActivateProperties()
    virtual void OnActivate();
    /// called from Entity::DeactivateProperties()
    virtual void OnDeactivate();
    /// called when input focus is gained
    virtual void OnObtainInputFocus();
    /// called when input focus is lost
    virtual void OnLoseInputFocus();
    /// return true if currently has input focus
    virtual bool HasFocus() const;
    /// called on begin of frame
    virtual void OnBeginFrame();
    /// handle a single message
    virtual void HandleMessage(const Ptr<Messaging::Message>& msg);
    // handle camera zoom in
    virtual void OnCameraZoomIn();
    // handle camera zoom out
    virtual void OnCameraZoomOut();
private:
    bool lastFrameSendMovement;
};
__RegisterClass(InputProperty);

}; // namespace Property
//------------------------------------------------------------------------------
