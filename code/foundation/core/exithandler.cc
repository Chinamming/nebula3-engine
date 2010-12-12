//------------------------------------------------------------------------------
//  exithandler.cc
//  (C) 2008 Radon Labs GmbH
//------------------------------------------------------------------------------
#include "stdneb.h"
#include "core/exithandler.h"
#include "core/sysfunc.h"

namespace Core
{

//------------------------------------------------------------------------------
/**
*/
ExitHandler::ExitHandler()
{
    this->nextExitHandler = SysFunc::RegisterExitHandler(this);
}        

//------------------------------------------------------------------------------
/**
*/
ExitHandler::~ExitHandler()
{
    // empty
}

//------------------------------------------------------------------------------
/**
*/
const ExitHandler*
ExitHandler::Next() const
{
    return this->nextExitHandler;
}

//------------------------------------------------------------------------------
/**
*/
void
ExitHandler::OnExit() const
{
    // empty, override in subclass!
}

} // namespace Core
