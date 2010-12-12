#pragma once
//------------------------------------------------------------------------------
/**
    @class Characters::CharacterSkeletonInstance
    
    Contains the per-instance skeleton data of a character.
    
    (C) 2008 Radon Labs GmbH
*/
#include "core/types.h"
#include "characters/characterskeleton.h"
#include "characters/charjointcomponents.h"
#include "jobs/job.h"
#include "jobs/jobport.h"

//------------------------------------------------------------------------------
namespace Characters
{
class CharacterSkeletonInstance
{
public:
    /// constructor
    CharacterSkeletonInstance();
    /// destructor
    ~CharacterSkeletonInstance();
    
    /// setup from CharacterSkeleton
    void Setup(const CharacterSkeleton& skeleton);
    /// discard the object
    void Discard();
    /// return true if the object has been setup
    bool IsValid() const;
    /// render a debug visualization of the character
    void RenderDebug(const Math::matrix44& modelTransform);

    /// get number of joint instances in the skeleton
    SizeT GetNumJoints() const;
    /// get the skin matrix array
    // FIXME: synchronization problems -> updated in asynchronous job!
    const Util::FixedArray<Math::matrix44>& GetSkinMatrixArray() const;
    /// get a joint matrix by joint index
    // FIXME: synchronization problems -> updated in asynchronous job!
    const Math::matrix44& GetJointMatrix(IndexT i) const;
    /// apply joint components
    void ApplyJointComponents(const Util::FixedArray<CharJointComponents>& set);

private:
    friend class CharacterInstance;

    /// setup skeleton evaluation job
    void SetupEvalJob();
    /// setup a single joint
    void SetupJoint(const CharacterSkeleton& skeleton, IndexT jointIndex);
    /// evaluate the joints (computes new skin matrices)
    void EvaluateAsync(const Ptr<Jobs::JobPort>& jobPort, const Math::float4* sampleBuffer, SizeT numSamples, void* jointTextureRowPtr, SizeT jointTextureRowSize, bool waitAnimJobsDone);

    const CharacterSkeleton* skeletonPtr;
    Ptr<Jobs::Job> evalJob;
    Util::FixedArray<CharJointComponents> startJointComponentsArray;         
    Util::FixedArray<CharJointComponents>* jointComponentsArrayPtr;  
    bool jointComponentsDirty;
    Util::FixedArray<Math::matrix44> scaledMatrixArray;
    Util::FixedArray<Math::matrix44> skinMatrixArray;
    bool isValid;
};

//------------------------------------------------------------------------------
/**
*/
inline const Math::matrix44&
CharacterSkeletonInstance::GetJointMatrix(IndexT i) const
{
    return this->scaledMatrixArray[i];
}

//------------------------------------------------------------------------------
/**
*/
inline bool
CharacterSkeletonInstance::IsValid() const
{
    return this->isValid;
}

//------------------------------------------------------------------------------
/**
*/
inline SizeT
CharacterSkeletonInstance::GetNumJoints() const
{
    return this->startJointComponentsArray.Size();
}

//------------------------------------------------------------------------------
/**
*/
inline const Util::FixedArray<Math::matrix44>&
CharacterSkeletonInstance::GetSkinMatrixArray() const
{
    return this->skinMatrixArray;
}

} // namespace Characters
//------------------------------------------------------------------------------

    