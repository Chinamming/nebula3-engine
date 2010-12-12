#pragma once
//------------------------------------------------------------------------------
/**
    @class Animation::AnimEventHandlerBase
    
    This is the baseclass for handling animation events.

    Create this �n Main application and register it via RegisterAnimEventHandler
    to the AnimEventServer. This will call the Handler to handle the AnimEvents.

    Finally throw away the pointer on the application side and unregister the
    handler via name (wich equls the category name)!

    Use in subclasses just internal graphics code stuff, cause this handler will
    work at the render thread side once they are attached!!!
    
    (C) 2009 Radon Labs GmbH
*/
#include "messaging/handler.h"
#include "util/stringatom.h"
#include "animation/animeventinfo.h"

//------------------------------------------------------------------------------
namespace Animation
{
class AnimEventHandlerBase : public Messaging::Handler
{
    __DeclareClass(AnimEventHandlerBase);
public:
    /// constructor
    AnimEventHandlerBase();
    /// destructor
    virtual ~AnimEventHandlerBase();   
    /// handle a event
    virtual bool HandleEvent(const Animation::AnimEventInfo& event);
 
    /// optional on frame method
    virtual void OnFrame(Timing::Time time);

    /// set the category name
    void SetCategoryName(const Util::StringAtom& catName);
    /// get the category name
    const Util::StringAtom& GetCategoryName() const;

 private:
    bool isOpen;
    Util::StringAtom categoryName;
};

//------------------------------------------------------------------------------
/**
*/
inline void
AnimEventHandlerBase::SetCategoryName(const Util::StringAtom& catName)
{
    this->categoryName = catName;
}

//------------------------------------------------------------------------------
/**
*/
inline const Util::StringAtom&
AnimEventHandlerBase::GetCategoryName() const
{
    return this->categoryName;
}


} // namespace Debug
//------------------------------------------------------------------------------
