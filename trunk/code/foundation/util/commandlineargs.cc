//------------------------------------------------------------------------------
//  util/commandlineargs.cc
//  (C) 2005 Radon Labs GmbH
//------------------------------------------------------------------------------
#include "stdneb.h"
#include "util/commandlineargs.h"

namespace Util
{
#if !__OSX__
using namespace Math;
#endif
    
//------------------------------------------------------------------------------
/**
*/
CommandLineArgs::CommandLineArgs()
{
    // empty
}

//------------------------------------------------------------------------------
/**
*/
CommandLineArgs::CommandLineArgs(const String& l)
{
    this->args = l.Tokenize(" \t\n=", '"');
}

//------------------------------------------------------------------------------
/**
*/
CommandLineArgs::CommandLineArgs(int argc, const char** argv)
{
    int i;
    for (i = 0; i < argc; i++)
    {
        this->args.Append(argv[i]);
    }
}

//------------------------------------------------------------------------------
/**
    Put out a missing argument error.
*/
void
CommandLineArgs::Error(const String& argName) const
{
    n_error("CmdLine: arg '%s' not found!", argName.AsCharPtr());
}

//------------------------------------------------------------------------------
/**
    Returns the command name.
*/
const String&
CommandLineArgs::GetCmdName() const
{
    n_assert(this->args.Size() > 0);
    return this->args[0];
}

//------------------------------------------------------------------------------
/**
    Returns true if argument exists.
*/
bool
CommandLineArgs::HasArg(const String& name) const
{
    IndexT index = this->args.FindIndex(name);
    return (index != InvalidIndex);
}

//------------------------------------------------------------------------------
/**
    Returns index of argument. Throws an error if argument not found.
*/
int
CommandLineArgs::FindArgIndex(const String& name) const
{
    n_assert(name.IsValid());
    IndexT i = this->args.FindIndex(name);
    if (InvalidIndex == i)
    {
        this->Error(name);
    }
    return i;
}

//------------------------------------------------------------------------------
/**
*/
const String&
CommandLineArgs::GetString(const String& name, const String& defaultValue) const
{
    if (this->HasArg(name))
    {
        return this->args[this->FindArgIndex(name) + 1];
    }
    else
    {
        return defaultValue;
    }
}

//------------------------------------------------------------------------------
/**
*/
int
CommandLineArgs::GetInt(const String& name, int defaultValue) const
{
    if (this->HasArg(name))
    {
        return this->args[this->FindArgIndex(name) + 1].AsInt();
    }
    else
    {
        return defaultValue;
    }
}

//------------------------------------------------------------------------------
/**
*/
float
CommandLineArgs::GetFloat(const String& name, float defaultValue) const
{
    if (this->HasArg(name))
    {
        return this->args[this->FindArgIndex(name) + 1].AsFloat();
    }
    else
    {
        return defaultValue;
    }
}

//------------------------------------------------------------------------------
/**
*/
bool
CommandLineArgs::GetBool(const String& name, bool defaultValue) const
{
    if (this->HasArg(name))
    {
        return this->args[this->FindArgIndex(name) + 1].AsBool();
    }
    else
    {
        return defaultValue;
    }
}

//------------------------------------------------------------------------------
/**
*/
bool
CommandLineArgs::GetBoolFlag(const String& name) const
{
    return this->HasArg(name);
}

#if !__OSX__
//------------------------------------------------------------------------------
/**
*/
float4
CommandLineArgs::GetFloat4(const String& name, const float4& defaultValue) const
{
    if (this->HasArg(name))
    {
        return this->args[this->FindArgIndex(name) + 1].AsFloat4();
    }
    else
    {
        return defaultValue;
    }
}

//------------------------------------------------------------------------------
/**
*/
matrix44
CommandLineArgs::GetMatrix44(const String& name, const matrix44& defaultValue) const
{
    if (this->HasArg(name))
    {
        return this->args[this->FindArgIndex(name) + 1].AsMatrix44();
    }
    else
    {
        return defaultValue;
    }
}
#endif
    
//------------------------------------------------------------------------------
/**
*/
SizeT
CommandLineArgs::GetNumArgs() const
{
    n_assert(this->args.Size() > 0);
    return this->args.Size() - 1;
}

//------------------------------------------------------------------------------
/**
*/
const String&
CommandLineArgs::GetStringAtIndex(IndexT index) const
{
    return this->args[index + 1];
}

//------------------------------------------------------------------------------
/**
*/
int
CommandLineArgs::GetIntAtIndex(IndexT index) const
{
    return this->args[index + 1].AsInt();
}

//------------------------------------------------------------------------------
/**
*/
float
CommandLineArgs::GetFloatAtIndex(IndexT index) const
{
    return this->args[index + 1].AsFloat();
}

//------------------------------------------------------------------------------
/**
*/
bool
CommandLineArgs::GetBoolAtIndex(IndexT index) const
{
    return this->args[index + 1].AsBool();
}

#if !__OSX__    
//------------------------------------------------------------------------------
/**
*/
float4
CommandLineArgs::GetFloat4AtIndex(IndexT index) const
{
    return this->args[index + 1].AsFloat4();
}

//------------------------------------------------------------------------------
/**
*/
matrix44
CommandLineArgs::GetMatrix44AtIndex(IndexT index) const
{
    return this->args[index + 1].AsMatrix44();
}
#endif
    
//------------------------------------------------------------------------------
/**
*/
void 
CommandLineArgs::AppendCommandString(const CommandLineArgs& otherArgs)
{
    IndexT i;
    for (i = 0; i < otherArgs.args.Size(); ++i)
    {
        this->args.Append(otherArgs.args[i]);
    }
}
} // namespace Util
