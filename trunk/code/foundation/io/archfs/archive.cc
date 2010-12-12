//------------------------------------------------------------------------------
//  archive.cc
//  (C) 2009 Radon Labs GmbH
//------------------------------------------------------------------------------
#include "stdneb.h"
#include "io/archfs/archive.h"

namespace IO
{
#if __WIN32__ || __XBOX360__
__ImplementClass(IO::Archive, 'ARCV', IO::ZipArchive);
#elif __WII__
__ImplementClass(IO::Archive, 'ARCV', Wii::WiiArchive);
#elif __PS3__
__ImplementClass(IO::Archive, 'ARCV', PS3::PS3Archive);
#else
#error "IO::Archive not implemented on this platform!"
#endif

} // namespace IO