#pragma once
//------------------------------------------------------------------------------
/**
    @class Win360::Win360CriticalSection
  
    Win32/Xbox360-implementation of critical section. Critical section
    objects are used to protect a portion of code from parallel
    execution. Define a static critical section object and
    use its Enter() and Leave() methods to protect critical sections
    of your code.
    
    (C) 2006 Radon Labs GmbH
*/
#include "core/types.h"     
#include "core/sysfunc.h"

#if __XBOX360__
// on xbox360 the critical section implementation seems to do the same as the windows user implementation
#define NEBULA3_USER_CRITICAL_SECTION 0
#else
// user implementation bases on "Fast critical sections with timeout" by Vladislav Gelfer
#define NEBULA3_USER_CRITICAL_SECTION 1
#endif

#if NEBULA3_USER_CRITICAL_SECTION
extern "C" void _WriteBarrier();
extern "C" void _ReadWriteBarrier();
#pragma intrinsic(_WriteBarrier)
#pragma intrinsic(_ReadWriteBarrier)
#endif
//------------------------------------------------------------------------------
namespace Win360
{
class Win360CriticalSection
{
public:
    /// constructor
    Win360CriticalSection();
    /// destructor
    ~Win360CriticalSection();
    /// enter the critical section
    void Enter() const;
    /// leave the critical section
    void Leave() const;
private:

#if NEBULA3_USER_CRITICAL_SECTION
    /// lock with increment
    bool PerfLockImmediate(DWORD dwThreadID) const;
    /// lock with semaphore
    bool PerfLock(DWORD dwThreadID) const;
    /// performance lock with system wait and kernel mode 
    bool PerfLockKernel(DWORD dwThreadID) const;
    /// unlock
    void PerfUnlock() const; 
    /// increment waiter count
    void WaiterPlus() const; 
    /// decrement waiter count
    void WaiterMinus() const;
    /// allocate semaphore if not done yet
    void AllocateKernelSemaphore() const;

    // Declare all variables volatile, so that the compiler won't
    // try to optimize something important.
    mutable volatile DWORD	lockerThread;
    volatile DWORD	spinMax;   
    mutable volatile long	waiterCount; 
    mutable volatile HANDLE	semaphore;
    mutable uint recursiveLockCount;
#else                                        
    mutable CRITICAL_SECTION criticalSection;
#endif
};         

#if !NEBULA3_USER_CRITICAL_SECTION
//------------------------------------------------------------------------------
/**
*/      
inline
Win360CriticalSection::Win360CriticalSection()
{
    InitializeCriticalSectionAndSpinCount(&this->criticalSection, 4096);
}

//------------------------------------------------------------------------------
/**
*/
inline
Win360CriticalSection::~Win360CriticalSection()
{
    DeleteCriticalSection(&this->criticalSection);
}

//------------------------------------------------------------------------------
/**
*/
inline void
Win360CriticalSection::Enter() const
{
    EnterCriticalSection(&this->criticalSection);
}

//------------------------------------------------------------------------------
/**
*/
inline void
Win360CriticalSection::Leave() const
{
    LeaveCriticalSection(&this->criticalSection);
}

#else
//------------------------------------------------------------------------------
/**
*/
inline void Win360CriticalSection::WaiterPlus() const
{
    _InterlockedIncrement(&this->waiterCount);
}

//------------------------------------------------------------------------------
/**
*/
inline void Win360CriticalSection::WaiterMinus() const
{
    _InterlockedDecrement(&this->waiterCount);
}

//------------------------------------------------------------------------------
/**
*/
inline bool 
Win360CriticalSection::PerfLockImmediate(DWORD dwThreadID) const 
{
    return (0 == InterlockedCompareExchange((long*) &this->lockerThread, dwThreadID, 0));
}

//------------------------------------------------------------------------------
/**
*/
inline void
Win360CriticalSection::Enter() const
{
    DWORD threadId = GetCurrentThreadId();
    if (threadId != this->lockerThread)
    {
        if ((this->lockerThread == 0) &&
            this->PerfLockImmediate(threadId))
        {
            // single instruction atomic quick-lock was successful
        }            
        else
        {
            // potentially locked elsewhere, so do a more expensive lock with system critical section
            this->PerfLock(threadId);
        }
    }
    this->recursiveLockCount++;
}
//------------------------------------------------------------------------------
/**
*/
inline void 
Win360CriticalSection::AllocateKernelSemaphore() const
{
    if (!this->semaphore)
    {
        HANDLE handle = CreateSemaphore(NULL, 0, 0x7FFFFFFF, NULL);
        n_assert(NULL != handle);
        if (InterlockedCompareExchangePointer(&this->semaphore, handle, NULL))
            CloseHandle(handle); // we're late
    }
}

//------------------------------------------------------------------------------
/**
*/
inline bool
Win360CriticalSection::PerfLock(DWORD dwThreadID) const
{
    // Attempt spin-lock
    for (DWORD dwSpin = 0; dwSpin < spinMax; dwSpin++)
    {
        if (this->PerfLockImmediate(dwThreadID))
            return true;

        YieldProcessor();
    }
           
    // Ensure we have the kernel event created
    this->AllocateKernelSemaphore();

    // do a more expensive lock
    bool result = this->PerfLockKernel(dwThreadID);
    this->WaiterMinus();

    return result;
}

//------------------------------------------------------------------------------
/**
*/
inline bool 
Win360CriticalSection::PerfLockKernel(DWORD dwThreadID) const
{
    bool waiter = false;

    for (;;)
    {
        if (!waiter)
            this->WaiterPlus();

        if (PerfLockImmediate(dwThreadID))
            return true;

        n_assert(this->semaphore);
        switch (WaitForSingleObject(this->semaphore, INFINITE))
        {
        case WAIT_OBJECT_0:
            waiter = false;
            break;
        case WAIT_TIMEOUT:
            waiter = true;
            break;
        default:
            break;
        };
    }
    // unreachable
    //return false;
}

//------------------------------------------------------------------------------
/**
*/
inline void
Win360CriticalSection::Leave() const
{
    if (--recursiveLockCount == 0)
    {
        this->PerfUnlock();
    }        
}

//------------------------------------------------------------------------------
/**
*/
inline void
Win360CriticalSection::PerfUnlock() const
{
    _WriteBarrier(); // changes done to the shared resource are committed.

    this->lockerThread = 0;

    _ReadWriteBarrier(); // The CS is released.

    if (this->waiterCount > 0) // AFTER it is released we check if there're waiters.
    {
        WaiterMinus(); 
        ReleaseSemaphore(this->semaphore, 1, NULL);                  
    }
}
#endif
};
//------------------------------------------------------------------------------
