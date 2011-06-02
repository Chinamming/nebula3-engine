#pragma once
//------------------------------------------------------------------------------
/**
    @file core/win32/win32singleton.h

    Provides helper macros to implement singleton objects:
    
    - __DeclareSingleton      put this into class declaration
    - __ImplementSingleton    put this into the implemention file
    - __ConstructSingleton    put this into the constructor
    - __DestructSingleton     put this into the destructor

    Get a pointer to a singleton object using the static Instance() method:

    Core::Server* coreServer = Core::Server::Instance();
    
    (C) 2007 Radon Labs GmbH
*/
#include "core/types.h"

#if !NEBULA3_EDITOR
//------------------------------------------------------------------------------
#define __DeclareSingleton(type) \
public: \
    ThreadLocal static type * Singleton; \
    static type * Instance() { n_assert(0 != Singleton); return Singleton; }; \
    static bool HasInstance() { return 0 != Singleton; }; \
private:

#define __DeclareInterfaceSingleton(type) \
public: \
    static type * Singleton; \
    static type * Instance() { n_assert(0 != Singleton); return Singleton; }; \
    static bool HasInstance() { return 0 != Singleton; }; \
private:

#define __ImplementSingleton(type) \
    ThreadLocal type * type::Singleton = 0;

#define __ImplementInterfaceSingleton(type) \
    type * type::Singleton = 0;

#define __ConstructSingleton \
    n_assert(0 == Singleton); Singleton = this;

#define __ConstructInterfaceSingleton \
    n_assert(0 == Singleton); Singleton = this;

#define __DestructSingleton \
    n_assert(Singleton); Singleton = 0;

#define __DestructInterfaceSingleton \
    n_assert(Singleton); Singleton = 0;
	
#else

#define __DeclareSingleton(type) \
public: \
    ThreadLocal static type * Singleton; \
	static type * Instance() \
	{ \
		critSectInst.Enter(); \
		n_assert(0 != Singleton); \
		type *hInst = Singleton; \
		critSectInst.Leave(); \
		return hInst; \
	}; \
	static bool HasInstance() \
	{ \
		critSectInst.Enter(); \
		bool hasInst = (0 != Singleton)?true:false;\
		critSectInst.Leave(); \
		return hasInst; \
	}; \
private: \
	ThreadLocal static Threading::CriticalSection critSectInst; \
private:

#define __DeclareInterfaceSingleton(type) \
public: \
    static type * Singleton; \
	static type * Instance() \
	{ \
		critSectInst.Enter(); \
		n_assert(0 != Singleton); \
		type *hInst = Singleton; \
		critSectInst.Leave(); \
		return hInst; \
	}; \
    static bool HasInstance() \
	{ \
		critSectInst.Enter(); \
		bool hasInst = (0 != Singleton)?true:false;\
		critSectInst.Leave(); \
		return hasInst; \
	}; \
private: \
	ThreadLocal static Threading::CriticalSection critSectInst; \
private:

#define __ImplementSingleton(type) \
    ThreadLocal type * type::Singleton = 0; \
	ThreadLocal Threading::CriticalSection type::critSectInst; 

#define __ImplementInterfaceSingleton(type) \
	ThreadLocal type * type::Singleton = 0; \
	ThreadLocal Threading::CriticalSection type::critSectInst; 

#define __ConstructSingleton \
	critSectInst.Enter(); \
    n_assert(0 == Singleton); Singleton = this; \
	critSectInst.Leave(); 

#define __ConstructInterfaceSingleton \
	critSectInst.Enter(); \
	n_assert(0 == Singleton); Singleton = this; \
	critSectInst.Leave(); 

#define __DestructSingleton \
	critSectInst.Enter(); \
    n_assert(Singleton); Singleton = 0; \
	critSectInst.Leave(); 

#define __DestructInterfaceSingleton \
	critSectInst.Enter(); \
    n_assert(Singleton); Singleton = 0; \
	critSectInst.Leave(); 

#endif
//------------------------------------------------------------------------------
