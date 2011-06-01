//------------------------------------------------------------------------------
//  charevalskeletonjob.cc
//  (C) 2009 Radon Labs GmbH
//------------------------------------------------------------------------------
#include "stdneb.h"
#include "jobs/stdjob.h"
#include "math/float4.h"
#include "math/matrix44.h"
#include "characters/charjointcomponents.h"

namespace Characters
{
using namespace Math;

#if __XBOX360__
extern "C" void _WriteBarrier();
#pragma intrinsic(_WriteBarrier)
#endif

//------------------------------------------------------------------------------
/**
*/
void
CharEvalSkeletonJobFunc(const JobFuncContext& ctx)
{
    float4 finalScale, parentFinalScale, variationTranslation;
    float4 translate(0.0f, 0.0f, 0.0f, 1.0f);
    float4 scale(1.0f, 1.0f, 1.0f, 0.0f);
    float4 parentScale(1.0f, 1.0f, 1.0f, 0.0f);
    float4 parentTranslate(0.0f, 0.0f, 0.0f, 1.0f);
    quaternion rotate = quaternion::identity();
    float4 vec1111(1.0f, 1.0f, 1.0f, 1.0f);
    float4 replaceWControl = float4::permute_control(0, 1, 2, 7);

    // load pointers from context
    // NOTE: the samplesBase pointer may be NULL if no valid animation
    // data exists, in this case the skeleton should simply be set
    // to its jesus pose
    CharJointComponents* compsBase = (CharJointComponents*) ctx.inputs[0];
    n_assert(0 != compsBase);
    const float4* samplesBase = (const float4*) ctx.inputs[1];
    const float4* samplesPtr  = samplesBase;

    matrix44* scaledMatrixBase = (matrix44*) ctx.outputs[0];
    matrix44* skinMatrixBase = (matrix44*) ctx.outputs[1];
    matrix44* invPoseMatrixBase = (matrix44*) ctx.uniforms[0];
    matrix44* unscaledMatrixBase = (matrix44*) ctx.scratch;

    // input samples may optionally include velocity samples which we need to skip...
    uint numElements = ctx.inputSizes[0] / sizeof(CharJointComponents);
    uint sampleWidth = (ctx.inputSizes[1] / numElements) / sizeof(float4);

    // compute number of joints
    int jointIndex;
    int numJoints = ctx.inputSizes[0] / sizeof(CharJointComponents);
    for (jointIndex = 0; jointIndex < numJoints; jointIndex++)
    {
        // load joint translate/rotate/scale
        translate.load((scalar*)&(samplesPtr[0]));
        rotate.load((scalar*)&(samplesPtr[1]));
        scale.load((scalar*)&(samplesPtr[2]));
        samplesPtr += sampleWidth;
                                     
        const CharJointComponents& comps = compsBase[jointIndex];
        // load variation scale
        finalScale.load(&comps.varScaleX);                
        finalScale = float4::multiply(scale, finalScale);
        finalScale = float4::permute(finalScale, vec1111, replaceWControl);

        matrix44& unscaledMatrix = unscaledMatrixBase[jointIndex];
        matrix44& scaledMatrix = scaledMatrixBase[jointIndex];

        // update unscaled matrix
        // animation rotation
        unscaledMatrix = matrix44::rotationquaternion(rotate);   
        // load variation translation
        variationTranslation.load(&comps.varTranslationX);                  
        unscaledMatrix.translate(translate + variationTranslation);

        // update scaled matrix
        // scale after rotation
        scaledMatrix.set_xaxis(float4::multiply(unscaledMatrix.get_xaxis(), float4::splat_x(finalScale)));
        scaledMatrix.set_yaxis(float4::multiply(unscaledMatrix.get_yaxis(), float4::splat_y(finalScale)));
        scaledMatrix.set_zaxis(float4::multiply(unscaledMatrix.get_zaxis(), float4::splat_z(finalScale)));

        if (InvalidIndex == comps.parentJointIndex)
        {
            // no parent directly set translation
            scaledMatrix.set_position(unscaledMatrix.get_position());
        }
        else
        {
            // load parent animation scale
            parentScale.load((scalar*)(samplesBase + sampleWidth * jointIndex + 2));
            const CharJointComponents& parentComps = compsBase[comps.parentJointIndex];
            // load parent variation scale
            parentFinalScale.load(&parentComps.varScaleX);            
            // combine both scaling types
            parentFinalScale = float4::multiply(parentScale, parentFinalScale);
            parentFinalScale = float4::permute(parentFinalScale, vec1111, replaceWControl);
            
            // transform our unscaled position with parent scaling
            unscaledMatrix.set_position(float4::multiply(unscaledMatrix.get_position(), parentFinalScale));            
            scaledMatrix.set_position(unscaledMatrix.get_position());

            // apply rotation and relative animation translation of parent 
            const matrix44& parentUnscaledMatrix = unscaledMatrixBase[comps.parentJointIndex];
            unscaledMatrix = matrix44::multiply(unscaledMatrix, parentUnscaledMatrix);
            scaledMatrix = matrix44::multiply(scaledMatrix, parentUnscaledMatrix);
        }    
        
        skinMatrixBase[jointIndex] = matrix44::multiply(invPoseMatrixBase[jointIndex], scaledMatrix);
    }

    #if __XBOX360__
    // on Xbox360, also write to the joint texture
    matrix44 transSkinMatrix;
    XMHALF4* texRowPtr = (XMHALF4*) ctx.outputs[2];
    for (jointIndex = 0; jointIndex < numJoints; jointIndex++)
    {
        transSkinMatrix = matrix44::transpose(skinMatrixBase[jointIndex]);

        // Writing the 3x4 matrix to memory.
        // Note the usage of write barriers here; they instruct the compiler not to
        // reorder the writes, in case we're writing to write-combined memory.
        // If the writes happen out of order, the CPU's store-gathering hardware that
        // is used for write-combined memory will not be used effectively, and the 
        // entire write operation will be much slower.
        // cause we use a texture with 16-bit per component, we store a half vector
        XMStoreHalf4(texRowPtr++, transSkinMatrix.getrow0().vec);
        _WriteBarrier();
        XMStoreHalf4(texRowPtr++, transSkinMatrix.getrow1().vec);
        _WriteBarrier();
        XMStoreHalf4(texRowPtr++, transSkinMatrix.getrow2().vec);
    }
    #endif
}

} // namespace Characters
__ImplementSpursJob(Characters::CharEvalSkeletonJobFunc);

