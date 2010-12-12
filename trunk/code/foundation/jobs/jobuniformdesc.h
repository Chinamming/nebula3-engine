#pragma once
//------------------------------------------------------------------------------
/**
    @class Jobs::JobUniformDesc
    
    Descriptor for uniform data of a Job.
    
    (C) 2009 Radon Labs GmbH
*/
#include "core/types.h"

//------------------------------------------------------------------------------
namespace Jobs
{
class JobUniformDesc
{
public:
    static const SizeT MaxNumBuffers = 2;

    /// default constructor
    JobUniformDesc();
    /// constructor with 1 uniform buffer
    JobUniformDesc(void* ptr, SizeT bufSize, SizeT scratchSize);
    /// constructor with 2 uniform buffers
    JobUniformDesc(void* ptr0, SizeT bufSize0, void* ptr1, SizeT bufSize1, SizeT scratchSize);

    /// update the uniform desc, all sizes in bytes
    void Update(IndexT index, void* ptr, SizeT bufSize, SizeT scratchSize);

    /// get number of buffers
    SizeT GetNumBuffers() const;
    /// get buffer pointer
    void* GetPointer(IndexT i) const;
    /// get buffer size
    SizeT GetBufferSize(IndexT i) const;
    /// get scratch size
    SizeT GetScratchSize() const;

private:
    SizeT numBuffers;
    void* ptr[MaxNumBuffers];
    SizeT bufferSize[MaxNumBuffers];
    SizeT scratchSize;
};

//------------------------------------------------------------------------------
/**
*/
inline
JobUniformDesc::JobUniformDesc() :
    numBuffers(0),
    scratchSize(0)
{
    IndexT i;
    for (i = 0; i < MaxNumBuffers; i++)
    {
        this->ptr[i] = 0;
        this->bufferSize[i] = 0;
    }
}

//------------------------------------------------------------------------------
/**
*/
inline
JobUniformDesc::JobUniformDesc(void* ptr_, SizeT bufSize_, SizeT scratchSize_) :
    numBuffers(1)
{
    this->ptr[0] = ptr_;
    this->bufferSize[0] = bufSize_;
    this->ptr[1] = 0;
    this->bufferSize[1] = 0;
    this->scratchSize = scratchSize_;
}

//------------------------------------------------------------------------------
/**
*/
inline
JobUniformDesc::JobUniformDesc(void* ptr0_, SizeT bufSize0_, void* ptr1_, SizeT bufSize1_, SizeT scratchSize_) :
    numBuffers(2)
{
    this->ptr[0] = ptr0_;
    this->bufferSize[0] = bufSize0_;
    this->ptr[1] = ptr1_;
    this->bufferSize[1] = bufSize1_;
    this->scratchSize = scratchSize_;
}

//------------------------------------------------------------------------------
/**
*/
inline void
JobUniformDesc::Update(IndexT index, void* ptr_, SizeT size_, SizeT scratchSize_)
{
    n_assert(index < this->numBuffers);
    this->ptr[index] = ptr_;
    this->bufferSize[index] = size_;
    this->scratchSize = scratchSize_;
}

//------------------------------------------------------------------------------
/**
*/
inline SizeT
JobUniformDesc::GetNumBuffers() const
{
    return this->numBuffers;
}

//------------------------------------------------------------------------------
/**
*/
inline void*
JobUniformDesc::GetPointer(IndexT i) const
{
    n_assert(i < MaxNumBuffers);
    return this->ptr[i];
}

//------------------------------------------------------------------------------
/**
*/
inline SizeT
JobUniformDesc::GetBufferSize(IndexT i) const
{
    n_assert(i < MaxNumBuffers);
    return this->bufferSize[i];
}

//------------------------------------------------------------------------------
/**
*/
inline SizeT 
JobUniformDesc::GetScratchSize() const
{
    return this->scratchSize;
}

} // namespace Jobs
//------------------------------------------------------------------------------
   