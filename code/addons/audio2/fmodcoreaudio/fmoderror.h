#pragma once

//------------------------------------------------------------------------------
/**
    @macro FMOD_CHECK_ERROR

    utility macro to check for errors, print error data and exit     
    its a macro, since we need info about source-file and line in code,
    where the error occured

    (C) 2009 Radon Labs GmbH
*/
#include "core/debug.h"

#define FMOD_CHECK_ERROR(result)                FmodCoreAudio::fmodCheckError((result), __FILE__, __LINE__)
#define FMOD_CHECK_ERROR_EXT(result, msg, ...)  FmodCoreAudio::fmodCheckErrorExt((result), __FILE__, __LINE__, msg, __VA_ARGS__)
#define FMOD_COREAUDIO_WARNING(msg, ...)        n_warning("[FMOD_COREAUDIO WARNING] "msg"\n", __VA_ARGS__);
#if NEBULA3_FMOD_COREAUDIO_VERBOSE_ENABLED
#define FMOD_COREAUDIO_VERBOSE(msg, ...)        n_printf("[FMOD_COREAUDIO] "msg"\n", __VA_ARGS__);
#else
#define FMOD_COREAUDIO_VERBOSE(msg, ...)
#endif

//------------------------------------------------------------------------------
namespace FmodCoreAudio
{
    /// check result and raise an error in needed
    void fmodCheckError(int result, const char *file, int line);
    /// same as fmodCheckError, but with printf-style arguments
    void fmodCheckErrorExt(int result, const char *file, int line, const char *msg, ...);
} // namespace FmodCoreAudio
//------------------------------------------------------------------------------
