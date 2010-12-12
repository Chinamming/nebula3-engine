#pragma once
#ifndef SCRIPTING_LUASERVER_H
#define SCRIPTING_LUASERVER_H
//------------------------------------------------------------------------------
/**
    @class Scripting::LuaServer
  
    LUA backend for the Nebula3 scripting subsystem.
    
    (C) 2006 Radon Labs GmbH
*/
#include "scripting/scriptserver.h"
#include "lua/lua.h"

//------------------------------------------------------------------------------
namespace Scripting
{
class LuaServer : public ScriptServer
{
    __DeclareClass(LuaServer);
    __DeclareSingleton(LuaServer);
public:
    /// constructor
    LuaServer();
    /// destructor
    virtual ~LuaServer();
    /// open the script server
    virtual bool Open();
    /// close the script server
    virtual void Close();
    /// register a command with the script server
    virtual void RegisterCommand(const Util::String& name, const Ptr<Command>& cmd);
    /// unregister a command from the script server
    virtual void UnregisterCommand(const Util::String& name);
    /// evaluate a script statement in a string
    virtual bool Eval(const Util::String& str);
    /// evaluate a script file
    //virtual bool EvalScript(const IO::URI& uri);

private:
    struct LuaStringReaderData
    {
        const Util::String* str;
    };

    /// LUA function callback
    static int LuaFunctionCallback(lua_State* s);
    /// LUA string chunk reader
    static const char* LuaStringReader(lua_State* s, LuaStringReaderData* data, size_t* size);
    /// LUA memory allocator
    static void* LuaAllocator(void* ud, void* ptr, size_t osize, size_t nsize);

    lua_State* luaState;
};

} // namespace Scripting
//------------------------------------------------------------------------------
#endif

