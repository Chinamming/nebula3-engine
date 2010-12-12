//------------------------------------------------------------------------------
//  particlesysteminstancebase.cc
//  (C) 2008 Radon Labs GmbH
//------------------------------------------------------------------------------
#include "stdneb.h"
#include "particles/base/particlesysteminstancebase.h"
#include "math/polar.h"
#include "coregraphics/shaperenderer.h"
#include "threading/thread.h"
#include "particles/particlerenderer.h"
#include "jobs/jobport.h"
#include "jobs/job.h"
#if __PS3__
#include "coregraphics/ps3/ps3dynamicgeometryserver.h"
#endif // __PS3__

#define USE_FIXED_UPDATE_TIME    0

namespace Particles
{
__ImplementAbstractClass(Particles::ParticleSystemInstanceBase, 'PSIB', Core::RefCounted);

using namespace Math;
using namespace Util;
using namespace CoreGraphics;

// job function declaration
#if __PS3__
extern "C" {
    extern const char _binary_jqjob_render_particlejob_ps3_bin_start[];
    extern const char _binary_jqjob_render_particlejob_ps3_bin_size[];
}
#else
extern void ParticleJobFunc(const JobFuncContext& ctx);
#endif

Particles::JOB_ID ParticleSystemInstanceBase::jobIdCounter = 0;

//------------------------------------------------------------------------------
/**
*/
ParticleSystemInstanceBase::ParticleSystemInstanceBase() :
    transform(Math::matrix44::identity()),
    velocity(vector::nullvec()),
    curStepTime(0.0f),
    lastEmissionTime(0.0f),
    timeSinceEmissionStart(0.0f),
    stateMask(ParticleSystemState::Initial),
    stateChangeMask(0),
    emissionCounter(0),
    firstEmissionFrame(false),
    particleId(0),
    jobId(0),
    numLivingParticles(0)
{
    // sizeof JobSliceOutputData must be a multiple of 16
    // since the vector-members can not be accessed if not aligned on 16 bytes
#if !__WII__    
    n_assert(!(sizeof(JobSliceOutputData)%16));
#endif    
    this->jobData.running = false;
    this->jobData.sliceCount = 0;
    this->jobData.sliceOutputCapacity = 0;
    this->jobData.sliceOutput = NULL;
}

//------------------------------------------------------------------------------
/**
*/
ParticleSystemInstanceBase::~ParticleSystemInstanceBase()
{
    n_assert(!this->IsValid());
}

//------------------------------------------------------------------------------
/**
*/
void
ParticleSystemInstanceBase::Setup(const Ptr<ParticleSystem>& psys)
{
    n_assert(!this->IsValid());

    this->stateMask = ParticleSystemState::Initial;
    this->stateChangeMask = 0;
    this->curStepTime = 0.0f;
    this->lastEmissionTime = 0.0f;
    this->timeSinceEmissionStart = 0.0f;
    this->firstEmissionFrame = true;
    this->numLivingParticles = 0;
    this->particleSystem = psys;
    this->particles.SetCapacity(this->particleSystem->GetMaxNumParticles());

    // setup static uniform data
    const EnvelopeSampleBuffer& envSampleBuffer = this->particleSystem->GetEnvelopeSampleBuffer();
    n_assert(envSampleBuffer.GetNumSamples() == Particles::ParticleSystemNumEnvelopeSamples);
    Memory::Copy(envSampleBuffer.GetSampleBuffer(), this->jobUniformData.sampleBuffer,
                 sizeof(float) * Particles::ParticleSystemNumEnvelopeSamples * Particles::EmitterAttrs::NumEnvelopeAttrs);
    const EmitterAttrs& emAttrs = this->particleSystem->GetEmitterAttrs();
    this->jobUniformData.gravity = float4(0.0f, emAttrs.GetFloat(EmitterAttrs::Gravity), 0.0f, 0.0f);
    this->jobUniformData.stretchToStart = emAttrs.GetBool(EmitterAttrs::StretchToStart);
    this->jobUniformData.stretchTime = emAttrs.GetFloat(EmitterAttrs::ParticleStretch);

    n_assert(!this->jobData.running);
    n_assert(!this->jobData.sliceCount);
    n_assert(!this->jobData.sliceOutputCapacity);
    n_assert(!this->jobData.sliceOutput);
    // setup a job system port
    this->jobData.jobPort = Jobs::JobPort::Create();
    this->jobData.jobPort->Setup();

    this->boundingBox.pmin = this->GetTransform().get_position();
    this->boundingBox.pmax = this->GetTransform().get_position();
}

//------------------------------------------------------------------------------
/**
*/
void
ParticleSystemInstanceBase::Discard()
{
    n_assert(this->IsValid());
    this->jobData.jobPort->Discard();
    this->jobData.jobPort = 0;
    if(this->jobData.sliceOutput)
    {
        n_delete_array(this->jobData.sliceOutput);
        this->jobData.sliceOutput = NULL;
    }
    this->jobData.sliceCount = 0;
    this->jobData.sliceOutputCapacity = 0;
    this->jobData.running = false;
    this->particleSystem = 0;
}

//------------------------------------------------------------------------------
/**
*/
void
ParticleSystemInstanceBase::Start()
{
    n_assert(this->IsValid());
    this->stateChangeMask = ParticleSystemState::Playing;
}

//------------------------------------------------------------------------------
/**
*/
void
ParticleSystemInstanceBase::Stop()
{
    n_assert(this->IsValid());
    this->stateChangeMask = ParticleSystemState::Stopped;
}

//------------------------------------------------------------------------------
/**
*/
void
ParticleSystemInstanceBase::OnRenderBefore()
{
    this->renderInfo.Clear();
}

//------------------------------------------------------------------------------
/**
    The Update() method must be called once per frame with a global time
    stamp. If the particle system is currently in the playing state,
    the method will emit new particles as needed, and update the current
    state of existing particles.
*/
void
ParticleSystemInstanceBase::Update(Timing::Time time)
{
    n_assert(this->IsValid());

    // update the state of the particle system (started, stopped, etc...)
    this->UpdateState((float)time);

    // this must be after UpdateState, since UpdateState can change this->curStepTime
    // when particle system is starting
    Timing::Time timeDiff = time - this->curStepTime;
    n_assert(timeDiff >= 0.0f);
    if(timeDiff > 0.5f)
    {
        this->curStepTime = time - ParticleSystem::StepTime;
        timeDiff = ParticleSystem::StepTime;
    }

    // if we are currently playing, update the particle system in discrete time steps,
    // but never more then 5 times in a row (which would mean we're running
    // at 12 fps)
    if (this->IsPlaying())
    {
#if USE_FIXED_UPDATE_TIME
        IndexT curStep = 0;
        while (this->curStepTime <= time)
        {
            this->EmitParticles(ParticleSystem::StepTime);
            bool isLastJob = (this->curStepTime+ParticleSystem::StepTime) > time;
            this->StartJobStepParticles(ParticleSystem::StepTime, isLastJob);
            this->curStepTime += ParticleSystem::StepTime;
            curStep++;
        }
#else
        this->EmitParticles((float)timeDiff);
        this->StartJobStepParticles((float)timeDiff, true);
        this->curStepTime += timeDiff;
#endif
    }
}

//------------------------------------------------------------------------------
/**
    Update the current state of the particle system (Started, Stopping,
    Stopped, etc...).
*/
void
ParticleSystemInstanceBase::UpdateState(float time)
{
    // first check if a state switch has been requested
    if (0 != this->stateChangeMask)
    {
        // if requested to play, and not already playing...
        if ((0 != (ParticleSystemState::Playing & this->stateChangeMask)) && !this->IsPlaying())
        {
            this->curStepTime = time - ParticleSystem::StepTime;
            this->lastEmissionTime = 0.0f;
            this->timeSinceEmissionStart = 0.0f;
            this->firstEmissionFrame = true;
            this->numLivingParticles = 0;
            this->particles.Reset();
            this->stateMask = ParticleSystemState::Playing;
        }

        // if requested to stop, and currently playing...
        if ((0 != (ParticleSystemState::Stopped & this->stateChangeMask)) && this->IsPlaying())
        {
            this->stateMask |= ParticleSystemState::Stopping;
        }
    }

    // if we're currently in Stopping state, and no more particles are alive,
    // go into Stopped state, the numLivingParticles member is
    // updated by the StepParticles method
    if (this->IsStopping())
    {
        if (0 == this->numLivingParticles)
        {
            this->stateMask = ParticleSystemState::Stopped;

            // set box dirty, and reset box
            this->boundingBox = Math::bbox();
        }
    }

    // reset the stateChangeMask
    this->stateChangeMask = 0;
}

//------------------------------------------------------------------------------
/**
    Emits new particles if necessary. This method takes the start time,
    curStepTime, startDelay, precalcTime and lastEmissionTime into account.
*/
void
ParticleSystemInstanceBase::EmitParticles(float stepTime)
{
    const EmitterAttrs& emAttrs = this->particleSystem->GetEmitterAttrs();
    const EnvelopeSampleBuffer& sampleBuffer = this->particleSystem->GetEnvelopeSampleBuffer();

    // get the (wrapped around if looping) time since emission has started
    this->timeSinceEmissionStart += stepTime;
    Timing::Time emDuration = emAttrs.GetFloat(EmitterAttrs::EmissionDuration);
    Timing::Time startDelay = emAttrs.GetFloat(EmitterAttrs::StartDelay);
    Timing::Time loopTime = emDuration + startDelay;
    bool looping = emAttrs.GetBool(EmitterAttrs::Looping);
    if (looping && (this->timeSinceEmissionStart > loopTime))
    {
        // a wrap-around
        this->timeSinceEmissionStart = n_fmod((float)this->timeSinceEmissionStart, (float)loopTime);
    }

    // if we are before the start delay, we definitely don't need to emit anything
    if (this->timeSinceEmissionStart < startDelay)
    {
        // we're before the start delay
        return;
    }
    else if ((this->timeSinceEmissionStart > loopTime) && !looping)
    {
        // we're past the emission time
        if (0 == (this->stateMask & (ParticleSystemState::Stopping|ParticleSystemState::Stopped)))
        {
            this->Stop();
        }
        return;
    }

    // compute the relative emission time (0.0 .. 1.0)
    Timing::Time relEmissionTime = (this->timeSinceEmissionStart - startDelay) / emDuration;
    IndexT emSampleIndex = IndexT(relEmissionTime * (Particles::ParticleSystemNumEnvelopeSamples-1));

    // lookup current emission frequency
    float emFrequency = sampleBuffer.LookupSamples(emSampleIndex)[EmitterAttrs::EmissionFrequency];
    if (emFrequency > N_TINY)
    {
        Timing::Time emTimeStep = 1.0 / emFrequency;

        // if we haven't emitted particles in this run yet, we need to compute
        // the precalc-time and initialize the lastEmissionTime
        if (this->firstEmissionFrame)
        {
            this->firstEmissionFrame = false;

            // handle pre-calc if necessary, this will instantly produce
            // a number of particles to let the particle system
            // appear as if has been emitting for quite a while already
            float preCalcTime = emAttrs.GetFloat(EmitterAttrs::PrecalcTime);
            if (preCalcTime > N_TINY)
            {
                // during pre-calculation we need to update the particle system,
                // but we do this with a lower step-rate for better performance
                // (but less precision)
                this->lastEmissionTime = 0.0f;
                Timing::Time updateTime = 0.0f;
                Timing::Time updateStep = 0.05f;
                if (updateStep < emTimeStep)
                {
                    updateStep = emTimeStep;
                }
                while (this->lastEmissionTime < preCalcTime)
                {
                    this->EmitParticle(emSampleIndex, 0.0f);
                    this->lastEmissionTime += emTimeStep;
                    updateTime += emTimeStep;
                    if (updateTime >= updateStep)
                    {
                        this->StartJobStepParticles((float)updateStep, false);
                        updateTime = 0.0f;
                    }
                }
            }

            // setup the lastEmissionTime for particle emission so that at least
            // one frame's worth of particles is emitted, this is necessary
            // for very short-emitting particles
            this->lastEmissionTime = this->curStepTime;
            if (stepTime > emTimeStep)
            {
                this->lastEmissionTime -= (stepTime + N_TINY);
            }
            else
            {
                this->lastEmissionTime -= (emTimeStep + N_TINY);
            }
        }

        // handle the "normal" particle emission while the particle system is emitting
        while ((this->lastEmissionTime + emTimeStep) <= this->curStepTime)
        {
            this->lastEmissionTime += emTimeStep;
            this->EmitParticle(emSampleIndex, (float)(this->curStepTime - this->lastEmissionTime));
        }
    }
}

//------------------------------------------------------------------------------
/**
    Emits a single particle at the current position. The emission
    sample index is used to lookup sampled envelope curves which are
    modulated over the emission duration or particle lifetime.
*/
void
ParticleSystemInstanceBase::EmitParticle(IndexT emissionSampleIndex, float initialAge)
{
    n_assert(initialAge >= 0.0f);

    Particle particle;
    const EmitterMesh& emMesh = this->particleSystem->GetEmitterMesh();
    const EmitterAttrs emAttrs = this->particleSystem->GetEmitterAttrs();
    const EnvelopeSampleBuffer& envSampleBuffer = this->particleSystem->GetEnvelopeSampleBuffer();
    float* particleEnvSamples = envSampleBuffer.LookupSamples(0);
    float* emissionEnvSamples = envSampleBuffer.LookupSamples(emissionSampleIndex);

    // lookup pseudo-random emitter vertex from the emitter mesh
    const EmitterMesh::EmitterPoint &emPoint = emMesh.GetEmitterPoint(this->emissionCounter++);

    // setup particle position and start position
    particle.position = matrix44::transform(emPoint.position, this->transform);
    n_assert(particle.position.w() > 0.95f);
    n_assert(particle.position.w() < 1.05f);
    particle.startPosition = particle.position;
    particle.stretchPosition = particle.position;

    // compute emission direction
    float minSpread = emissionEnvSamples[EmitterAttrs::SpreadMin];
    float maxSpread = emissionEnvSamples[EmitterAttrs::SpreadMax];
    float theta = n_deg2rad(n_lerp(minSpread, maxSpread, n_rand()));
    float rho = N_PI_DOUBLE * n_rand();
    matrix44 rot = matrix44::multiply(matrix44::rotationaxis(emPoint.tangent, theta), matrix44::rotationaxis(emPoint.normal, rho));
    float4 emNormal = matrix44::transform(emPoint.normal, rot);

    // compute start velocity
    float velocityVariation = 1.0f - (n_rand() * emAttrs.GetFloat(EmitterAttrs::VelocityRandomize));
    float startVelocity = emissionEnvSamples[EmitterAttrs::StartVelocity] * velocityVariation;

    // setup particle velocity vector
    particle.velocity = emNormal * startVelocity;

    // setup uvMinMax to a random texture tile
    // FIXME: what's up with the horizontal flip?
    float texTile = n_clamp(emAttrs.GetFloat(EmitterAttrs::TextureTile), 1.0f, 16.0f);
    float step = 1.0f / texTile;
    float tileIndex = floorf(n_rand() * texTile);
    float vMin = step * tileIndex;
    float vMax = vMin + step;
    particle.uvMinMax.set(1.0f, 1.0f - vMin, 0.0f, 1.0f - vMax);

    // setup initial particle color
    particle.color.load(&(particleEnvSamples[EmitterAttrs::Red]));

    // setup rotation and rotationVariation
    float startRotMin = emAttrs.GetFloat(EmitterAttrs::StartRotationMin);
    float startRotMax = emAttrs.GetFloat(EmitterAttrs::StartRotationMax);
    particle.rotation = n_lerp(startRotMin, startRotMax, n_rand());
    float rotVar = 1.0f - (n_rand() * emAttrs.GetFloat(EmitterAttrs::RotationRandomize));
    if (emAttrs.GetBool(EmitterAttrs::RandomizeRotation) && (n_rand() < 0.5f))
    {
        rotVar = -rotVar;
    }
    particle.rotationVariation = rotVar;

    // setup particle size and size variation
    particle.size = particleEnvSamples[EmitterAttrs::Size];
    particle.sizeVariation = 1.0f - (n_rand() * emAttrs.GetFloat(EmitterAttrs::SizeRandomize));

    // setup particle age and oneDivLifetime
    particle.oneDivLifeTime = 1.0f / emissionEnvSamples[EmitterAttrs::LifeTime];
    particle.relAge = initialAge * particle.oneDivLifeTime;
    particle.age = initialAge;

    // group particles dependend on its lifetime
    if (particle.relAge < 0.25f)
    {
        particle.particleId = 4;
    }
    else if (particle.relAge < 0.5f)
    {
        particle.particleId = 3;
    }
    else if (particle.relAge < 0.75f)
    {
        particle.particleId = 2;
    }
    else
    {
        particle.particleId = 1;
    }
    //this->particleId = !this->particleId;
    // set particle id, just switch between 0 and 1 
    //particle.particleId = (float)this->particleId;    
    //if (++this->particleId > 3) this->particleId = 0;         

    // add the new particle to the particle ring buffer
    this->particles.Add(particle);
}

//------------------------------------------------------------------------------
/**
*/
void
ParticleSystemInstanceBase::FinalizeJobs()
{
    if(!this->jobData.running) return;

    if (!this->jobData.jobPort->CheckDone())
    {
        this->jobData.jobPort->WaitDone();
    }

    // collect slice-specific output data
    this->numLivingParticles = 0;
    this->boundingBox.begin_extend();
    for(int i = 0; i < this->jobData.sliceCount; ++i)
    {
        if(this->jobData.sliceOutput[i].numLivingParticles)
        {
            this->numLivingParticles += this->jobData.sliceOutput[i].numLivingParticles;
            this->boundingBox.extend(this->jobData.sliceOutput[i].bboxMin);
            this->boundingBox.extend(this->jobData.sliceOutput[i].bboxMax);
        }
    }
    this->boundingBox.end_extend();
    if(!this->numLivingParticles)
    {
        this->boundingBox.pmin = this->GetTransform().get_position();
        this->boundingBox.pmax = this->GetTransform().get_position();
    }

    this->jobData.running = false;
}

//------------------------------------------------------------------------------
/**
*/
void
ParticleSystemInstanceBase::PrepareJobUniformData(float stepTime, bool generateVertexList, Jobs::JobUniformDesc &uniformDesc)
{
    this->jobUniformData.stepTime = stepTime;
#if !__WII__    
    n_assert(!((int)&this->jobUniformData & 0xF));
#endif    
    uniformDesc = Jobs::JobUniformDesc(&this->jobUniformData, sizeof(this->jobUniformData), 0);
}

//------------------------------------------------------------------------------
/**
*/
void
ParticleSystemInstanceBase::PrepareJobOutputData(bool generateVertexList, int inputBufferSize, Jobs::JobDataDesc &outputDesc)
{
#if __PS3__
    const SizeT sliceSize = generateVertexList ? PARTICLE_JOB_INPUT_SLICE_SIZE__VSTREAM_ON :
                                                 PARTICLE_JOB_INPUT_SLICE_SIZE__VSTREAM_OFF;
#else
    const SizeT sliceSize = PARTICLE_JOB_INPUT_SLICE_SIZE__VSTREAM_OFF;
#endif
    outputDesc = Jobs::JobDataDesc(this->particles.GetBuffer(), inputBufferSize, sliceSize,
                                   this->jobData.sliceOutput, sizeof(JobSliceOutputData) * this->jobData.sliceCount, sizeof(JobSliceOutputData));
}

//------------------------------------------------------------------------------
/**
*/
void
ParticleSystemInstanceBase::StartJobStepParticles(float stepTime, bool generateVertexList)
{
    n_assert(this->renderInfo.IsEmpty());

    if(!this->particles.Size()) return;

    // already any jobs started?
    if(this->jobId > 0)
    {
        // wait for previous jobs, if still running for this particle system
        this->FinalizeJobs();
    }

    ++ParticleSystemInstanceBase::jobIdCounter;
    // skip zero on overflow
    if(!ParticleSystemInstanceBase::jobIdCounter) ++ParticleSystemInstanceBase::jobIdCounter;
    this->jobId = ParticleSystemInstanceBase::jobIdCounter;

    ///////////////////////
    // setup uniform data
    Jobs::JobUniformDesc uniformDesc;
    this->PrepareJobUniformData(stepTime, generateVertexList, uniformDesc);

    ///////////////////////
    // setup input data
    n_assert(this->particles.GetBuffer());
#if !__WII__    
    n_assert(!((int)this->particles.GetBuffer() & 0xF));
#endif    
    const int inputBufferSize = this->particles.Size() * PARTICLE_JOB_INPUT_ELEMENT_SIZE;


#if __PS3__
    const SizeT intputSliceSize = generateVertexList ? PARTICLE_JOB_INPUT_SLICE_SIZE__VSTREAM_ON :
                                                       PARTICLE_JOB_INPUT_SLICE_SIZE__VSTREAM_OFF;
#else
    const SizeT intputSliceSize = PARTICLE_JOB_INPUT_SLICE_SIZE__VSTREAM_OFF;
#endif
    Jobs::JobDataDesc inputDesc(this->particles.GetBuffer(), inputBufferSize, intputSliceSize);

    ///////////////////////
    // setup output data
    //   buffer 0: particles, written back to this->particles
    //   buffer 1: each slices generates output into this buffer (per slice: bounding box, numLivingParticles)
    //   buffer 2: (only ps3, only if generateVertexList == true) vertex stream for rendering
    // calculate number of slices
    this->jobData.sliceCount = (inputBufferSize +(intputSliceSize-1)) / intputSliceSize;
    n_assert(this->jobData.sliceCount > 0);
    // and resize the slice-outputbuffer if necessary
    if(this->jobData.sliceCount > this->jobData.sliceOutputCapacity)
    {
        this->jobData.sliceOutputCapacity = this->jobData.sliceCount;
        if(this->jobData.sliceOutput) n_delete_array(this->jobData.sliceOutput);
        this->jobData.sliceOutput = n_new_array(JobSliceOutputData, this->jobData.sliceOutputCapacity);
        n_assert(this->jobData.sliceOutput);
#if !__WII__    
        n_assert(!((int)this->jobData.sliceOutput & 0xF));
#endif        
    }
    Jobs::JobDataDesc outputDesc;
    this->PrepareJobOutputData(generateVertexList, inputBufferSize, outputDesc);

    ///////////////////////
    // setup job and run it
    #if __PS3__
    Jobs::JobFuncDesc jobFunc(_binary_jqjob_render_particlejob_ps3_bin_start, _binary_jqjob_render_particlejob_ps3_bin_size);
    #else
    Jobs::JobFuncDesc jobFunc(ParticleJobFunc);
    #endif
    this->jobData.running = true;
    this->jobData.job = Jobs::Job::Create();
    this->jobData.job->Setup(uniformDesc, inputDesc, outputDesc, jobFunc);
    // !!! this is very important, otherwise there is no guarantee, that the job uses the
    // !!! current read only uniform data, there were cases where the last uniform data was used
    this->jobData.jobPort->PushFlush();
    this->jobData.jobPort->PushJob(this->jobData.job);
}

//------------------------------------------------------------------------------
/**
    Render a debug visualization of the particle system.
*/
void
ParticleSystemInstanceBase::RenderDebug()
{
    if (this->numLivingParticles > 0)
    {
        Array<float4> lineList;
        lineList.Reserve(this->numLivingParticles * 6);
        vector xAxis(1.0f, 0.0f, 0.0f);
        vector yAxis(0.0f, 1.0f, 0.0f);
        vector zAxis(0.0f, 0.0f, 1.0f);

        // render each living particle as a x/y/z cross
        IndexT i;
        for (i = 0; i < this->particles.Size(); i++)
        {
            const Particle& particle = this->particles[i];
            if (particle.relAge < 1.0f)
            {
                // cross 1
                lineList.Append(particle.position - xAxis * particle.size);
                lineList.Append(particle.position + xAxis * particle.size);
                lineList.Append(particle.position - yAxis * particle.size);
                lineList.Append(particle.position + yAxis * particle.size);
                lineList.Append(particle.position - zAxis * particle.size);
                lineList.Append(particle.position + zAxis * particle.size);

                // connection
                lineList.Append(particle.position);
                lineList.Append(particle.stretchPosition);
            }
        }
        if (!lineList.IsEmpty())
        {
            RenderShape particlesShape;
            particlesShape.SetupPrimitives(Threading::Thread::GetMyThreadId(),
                                           matrix44::identity(),
                                           PrimitiveTopology::LineList,
                                           lineList.Size() / 2,
                                           &(lineList.Front()),
                                           4,
                                           float4(1.0f, 0.0f, 1.0f, 0.8f));
            //ShapeRenderer::Instance()->AddShape(particlesShape);
           
            RenderShape boxShape;
            boxShape.SetupSimpleShape(Threading::Thread::GetMyThreadId(),
                                      RenderShape::Box,
                                      this->boundingBox.to_matrix44(),
                                      float4(1.0f, 0.0f, 0.0f, 0.25f));
            ShapeRenderer::Instance()->AddShape(boxShape); 

            RenderShape centerShape;
            matrix44 centerPoint = matrix44::scaling(vector(0.1f,0.1f,0.1f));
            centerPoint.set_position(this->boundingBox.center());
            centerShape.SetupSimpleShape(Threading::Thread::GetMyThreadId(),
                                         RenderShape::Box,
                                         centerPoint,
                                         float4(1.0f, 0.0f, 0.0f, 0.75f));
            ShapeRenderer::Instance()->AddShape(centerShape); 
        }
    }
}

//------------------------------------------------------------------------------
/**
*/
void ParticleSystemInstanceBase::UpdateVertexStreams()
{
    n_assert(ParticleRenderer::Instance()->IsInAttach());
    n_assert(this->renderInfo.IsEmpty());
}

} // namespace Particles