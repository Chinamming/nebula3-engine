#pragma once
//------------------------------------------------------------------------------
/**
    @class Characters::CharacterNodeInstance
  
    The CharacterNodeInstance class wraps a CharacterInstance object into
    a ModelNodeInstance class.
    
    (C) 2008 Radon Labs GmbH
*/    
#include "models/nodes/transformnodeinstance.h"
#include "characters/characterinstance.h"

//------------------------------------------------------------------------------
namespace Characters
{
class CharacterNodeInstance : public Models::TransformNodeInstance
{
    __DeclareClass(CharacterNodeInstance);
public:
    /// constructor
    CharacterNodeInstance();
    /// destructor
    virtual ~CharacterNodeInstance();

    /// called from ModelEntity::OnNotifyCullingVisible
    virtual void OnNotifyCullingVisible(IndexT frameIndex, Timing::Time time);
    /// get the node's character instance object
    const Ptr<CharacterInstance>& GetCharacterInstance() const;

protected:
    /// called when attached to ModelInstance
    virtual void Setup(const Ptr<Models::ModelInstance>& inst, const Ptr<Models::ModelNode>& node, const Ptr<Models::ModelNodeInstance>& parentNodeInst);
    /// called when removed from ModelInstance
    virtual void Discard();
    /// render node specific debug shape
    virtual void RenderDebug();

    Ptr<CharacterInstance> charInstance;
};

//------------------------------------------------------------------------------
/**
*/
inline const Ptr<CharacterInstance>&
CharacterNodeInstance::GetCharacterInstance() const
{
    return this->charInstance;
}

} // namespace Characters
//------------------------------------------------------------------------------
