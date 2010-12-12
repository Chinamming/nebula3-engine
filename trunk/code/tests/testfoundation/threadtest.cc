//------------------------------------------------------------------------------
//  threadtest.cc
//  (C) 2007 Radon Labs GmbH
//------------------------------------------------------------------------------
#include "stdneb.h"
#include "threadtest.h"
#include "io/console.h"

namespace Test
{
__ImplementClass(Test::ThreadTest, 'THRT', Test::TestCase);

__ImplementClass(Test::TestSingle, 'TSTS', Core::RefCounted);
__ImplementSingleton(Test::TestSingle);

__ImplementClass(Test::MyThread, 'MYTD', Threading::Thread);

bool ThreadTest::InstanceCheck = true;
Threading::Event ThreadTest::threadEvent;

using namespace Util;

//------------------------------------------------------------------------------
/**
*/
void
MyThread::DoWork()
{
    Ptr<IO::Console> con;
    ThreadTest::InstanceCheck = TestSingle::HasInstance();

    if (!IO::Console::HasInstance())
    {
        con = IO::Console::Create();
        con->Open();
    }
	
    if (!ThreadTest::InstanceCheck)
    {
        n_printf("no test single available \n");
        Ptr<Test::TestSingle> ptr = TestSingle::Create();
        n_printf("thread single address 0x%x \n", (uint) TestSingle::Instance());
    }
    else
    {
        n_printf("already available\n");
    }

    ThreadTest::threadEvent.Signal();    
}


//------------------------------------------------------------------------------
/**
*/
TestSingle::TestSingle()
{
    __ConstructSingleton;     
}

//------------------------------------------------------------------------------
/**
*/
TestSingle::~TestSingle()
{
}

//------------------------------------------------------------------------------
/**
*/
void
ThreadTest::Run()
{
    this->mythread = MyThread::Create();
    this->mythread->SetCoreId(System::Cpu::MainThreadCore);
    this->single = TestSingle::Create();

    this->single = TestSingle::Instance();
    n_printf("main address 0x%x \n", (uint) TestSingle::Instance());
    this->mythread->Start();
    
    threadEvent.Wait();
    this->Verify(!InstanceCheck);    
}

} // namespace Test