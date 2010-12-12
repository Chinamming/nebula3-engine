//------------------------------------------------------------------------------
//  jobstestapplication.cc
//  (C) 2009 Radon Labs GmbH
//------------------------------------------------------------------------------
#include "stdneb.h"
#include "jobstestapplication.h"
#include "math/matrix44.h"
#include "math/float4.h"
#include "timing/timer.h"
#include "core/debug.h"
#include "particles/particle.h"
#include "testdata.h"

using namespace Jobs;
using namespace Math;
using namespace Timing;

namespace Particles
{

#if __PS3__
extern "C" {
    extern const char _binary_jqjob_render_particlejob_ps3_bin_start[];
    extern const char _binary_jqjob_render_particlejob_ps3_bin_size[];
}
#else
extern void ParticleJobFunc(const JobFuncContext& ctx);
#endif


//------------------------------------------------------------------------------
/**
*/
JobsTestApplication::JobsTestApplication()
{
    // empty
}

//------------------------------------------------------------------------------
/**
*/
JobsTestApplication::~JobsTestApplication()
{
    n_assert(!this->IsOpen());
}

//------------------------------------------------------------------------------
/**
*/
bool
JobsTestApplication::Open()
{
    if (ConsoleApplication::Open())
    {
        // setup jobs subsystem
        this->jobSystem = JobSystem::Create();
        this->jobSystem->Setup();

        this->gameContentServer = IO::GameContentServer::Create();
        this->gameContentServer->SetTitle("RL Test Title");
        this->gameContentServer->SetTitleId("RLTITLEID");
        this->gameContentServer->SetVersion("1.00");
        this->gameContentServer->Setup();

        return true;
    }
    else
    {
        return false;
    }
}

//------------------------------------------------------------------------------
/**
*/
void
JobsTestApplication::Close()
{
    this->gameContentServer->Discard();
    this->gameContentServer = 0;
    this->jobSystem->Discard();
    this->jobSystem = 0;
    ConsoleApplication::Close();
}

//------------------------------------------------------------------------------
/**
*/
void
JobsTestApplication::Run()
{
    // uniform    
	JobUniformData *un = n_new(JobUniformData);
	Memory::Copy(uniform, un, sizeof(JobUniformData));
    JobUniformDesc uniformDesc(un, sizeof(JobUniformData), 0);
    const int input_size_bytes = sizeof(Particle) * particleCount; 
    const SizeT inputSliceSize = PARTICLE_JOB_INPUT_SLICE_SIZE__VSTREAM_OFF;
    JobDataDesc inputDesc(particlesInput, input_size_bytes, inputSliceSize);


    // output
    Jobs::JobDataDesc outputDesc;
    Particle *particlesOutput = n_new_array(Particle, particleCount);
    n_assert(particlesOutput);
    const int inputBufferSize = input_size_bytes;
    int sliceCount = (inputBufferSize +(inputSliceSize-1)) / inputSliceSize;
    n_assert(sliceCount > 0);
    
    // and resize the slice-outputbuffer if necessary
    JobSliceOutputData *sliceOutput = n_new_array(JobSliceOutputData, sliceCount);
    n_assert(sliceOutput);
    n_assert(!((int)sliceOutput & 0xF));

    {
        outputDesc = Jobs::JobDataDesc(particlesOutput, inputBufferSize, inputSliceSize,
                                       sliceOutput, sizeof(JobSliceOutputData) * sliceCount, sizeof(JobSliceOutputData));
    }


    ///////////////////////
    // setup job and run it
    Jobs::JobFuncDesc jobFunc(ParticleJobFunc);
    this->job = Jobs::Job::Create();
    job->Setup(uniformDesc, inputDesc, outputDesc, jobFunc);

    this->jobPort = Jobs::JobPort::Create();
    jobPort->Setup();

    // 1st job takes very long
    jobPort->PushFlush();
    jobPort->PushJob(job);
    jobPort->WaitDone();


    Timer parallelTimer;
    parallelTimer.Start();
    IndexT runIndex;
    SizeT numRuns = 10000;
    for (runIndex = 0; runIndex < numRuns; runIndex++)
    {
        this->jobPort->PushFlush();
        this->jobPort->PushJob(this->job);
        this->jobPort->WaitDone();
    }
    parallelTimer.Stop();
    n_printf("(runs %d): %fs\n", runIndex, parallelTimer.GetTime());
    job->Discard();
    job = 0;
    jobPort->Discard();
    jobPort = 0;

    n_printf("result:\n");
    for(int i = 0; i < sliceCount; ++i)
    {
        n_printf("  slice %d numLivingParticles: %d\n", i, sliceOutput[i].numLivingParticles);
    }
    n_delete_array(sliceOutput);
    sliceOutput = NULL;
}

} // namespace Test
