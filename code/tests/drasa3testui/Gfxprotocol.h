#pragma once
//------------------------------------------------------------------------------
/**
    This file was generated with Nebula3's idlc compiler tool.
    DO NOT EDIT
*/
#include "messaging/message.h"
#include "util/array.h"
#include "util/stringatom.h"
#include "resources/resourceid.h"
#include "input/inputevent.h"

//------------------------------------------------------------------------------
namespace ScaleForms
{

//------------------------------------------------------------------------------
class ProcessEvents : public Messaging::Message
{
	__DeclareClass(ProcessEvents);
	__DeclareMsgId;
public:
	ProcessEvents() 
	{ };
public:
	void SetInputEvents(const Util::Array<Input::InputEvent>& val)
	{
		n_assert(!this->handled);
		this->inputevents = val;
	};
	const Util::Array<Input::InputEvent>& GetInputEvents() const
	{
		return this->inputevents;
	};
private:
	Util::Array<Input::InputEvent> inputevents;
};

}