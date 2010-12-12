#pragma once
//------------------------------------------------------------------------------
/**
    @class Core::ExitHandler
    
    ExitHandlers are static objects which register themselves automatically
    once at startup and are called back from the Core::SysFunc::Exit()
    static method which is called right before a Nebula3 application exists.  
    Please note that the Nebula3 runtime usually doesn't yet exist when
    the ExitHandler is created or destroyed, so don't put anything complex
    into the constructor or destructor of the class!
    
    
    (C) 2008 Radon Labs GmbH
*/    
#include "core/types.h"

//------------------------------------------------------------------------------
namespace Core
{
class ExitHandler
{
public:
    /// constructor
    ExitHandler();
    /// destructor
    virtual ~ExitHandler();
    /// virtual method called from SysFunc::Exit()
    virtual void OnExit() const;
    /// get pointer to next exit handler in forward linked list
    const ExitHandler* Next() const;

private:
    const ExitHandler* nextExitHandler; // for forward linking...
};

} // namespace Core
//------------------------------------------------------------------------------
