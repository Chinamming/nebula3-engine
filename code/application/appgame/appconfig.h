#pragma once
//------------------------------------------------------------------------------
/**
    @file appgame/appconfig.h

    Configuration header.

    (C) 2007 Radon Labs GmbH
*/

// disabled, profilers cost to much performance
#define __ENTITY_STATS__ (0)
#define __MANAGERS_STATS__ (0)
#define __DEBUG_ACTIVE__ (1)
#define __REMOTE_ACTIVE__ (1)
#define __USE_XACT_AUDIO__ (1)
#define __DISABLE_AUDIO__ (0)
#if __USE_XACT_AUDIO__ && __DISABLE_AUDIO__
#error  Only one define allowed!!!
#endif
#define __XACT_AUDITIONING__ (0) //activate the auditioning server for the XACT remote tool. 
#define __USE_VEGETATION_MANAGER__ (0)

// soft assert messages will be displayed while playing
#define __SOFT_ASSERT_WITH_INGAME_MESSAGES_FOR_DIALOG_SUBSYSTEM__ (1)
#define __SOFT_ASSERT_WITH_INGAME_MESSAGES_FOR_QUEST_SUBSYSTEM__ (1)

// physics stuff
#if  __WII__
#define __USE_PHYSICS__ (0)
#else
#define __USE_PHYSICS__ (1)
#endif

//------------------------------------------------------------------------------
