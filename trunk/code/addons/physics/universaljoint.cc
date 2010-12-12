//------------------------------------------------------------------------------
//  physics/universaljoint.cc
//  (C) 2003 RadonLabs GmbH
//------------------------------------------------------------------------------
#include "stdneb.h"
#include "physics/universaljoint.h"
#include "debugrender/debugshaperenderer.h"

namespace Physics
{
__ImplementClass(Physics::UniversalJoint, 'UJOI', Physics::Joint);

using namespace Math;
using namespace Debug;

//------------------------------------------------------------------------------
/**
*/
UniversalJoint::UniversalJoint() : 
    Joint(Joint::UniversalJoint),
    axisParams(2)
{
    this->axisParams[0].SetAxis(Math::vector(0.0f, 0.0f, 1.0f));
    this->axisParams[1].SetAxis(Math::vector(0.0f, 1.0f, 0.0f));
}

//------------------------------------------------------------------------------
/**
*/
UniversalJoint::~UniversalJoint()
{
    // empty
}

//------------------------------------------------------------------------------
/**
    NOTE: it is important that rigid bodies are added 
    (happens in Joint::Attach()) before joint transforms are set!!!
*/
void
UniversalJoint::Attach(dWorldID worldID, dJointGroupID groupID, const Math::matrix44& m)
{
    // create ODE joint
    this->odeJointId = dJointCreateUniversal(worldID, groupID);

    // configure ODE joint
    int i;
    for (i = 0; i < 2; i++)
    {
        const JointAxis& curAxis = this->axisParams[i];
        if (curAxis.IsLoStopEnabled())
        {
            dJointSetUniversalParam(this->odeJointId, dParamLoStop + dParamGroup * i, curAxis.GetLoStop());
        }
        if (curAxis.IsHiStopEnabled())
        {
            dJointSetUniversalParam(this->odeJointId, dParamHiStop + dParamGroup * i, curAxis.GetHiStop());
        }
        dJointSetUniversalParam(this->odeJointId, dParamVel + dParamGroup * i, curAxis.GetVelocity());
        dJointSetUniversalParam(this->odeJointId, dParamFMax + dParamGroup * i, curAxis.GetFMax());
        dJointSetUniversalParam(this->odeJointId, dParamFudgeFactor + dParamGroup * i, curAxis.GetFudgeFactor());
        dJointSetUniversalParam(this->odeJointId, dParamBounce + dParamGroup * i, curAxis.GetBounce());
        dJointSetUniversalParam(this->odeJointId, dParamCFM + dParamGroup * i, curAxis.GetCFM());
        dJointSetUniversalParam(this->odeJointId, dParamStopERP + dParamGroup * i, curAxis.GetStopERP());
        dJointSetUniversalParam(this->odeJointId, dParamStopCFM + dParamGroup * i, curAxis.GetStopCFM());
    }

    // hand to parent class
    Joint::Attach(worldID, groupID, m);

    // configure ODE joint
    this->UpdateTransform(m);
}

//------------------------------------------------------------------------------
/**
*/
void 
UniversalJoint::UpdateTransform(const Math::matrix44& m)
{
    Math::vector p = matrix44::transform(this->GetAnchor(), m);
    dJointSetUniversalAnchor(this->odeJointId, p.x(), p.y(), p.z());

    matrix44 m44(m.get_xaxis(), m.get_yaxis(), m.get_zaxis(), vector::nullvec());
    Math::vector a0 = matrix44::transform(this->axisParams[0].GetAxis(), m44);
    Math::vector a1 = matrix44::transform(this->axisParams[1].GetAxis(), m44);
    dJointSetUniversalAxis1(this->odeJointId, a0.x(), a0.y(), a0.z());
    dJointSetUniversalAxis2(this->odeJointId, a1.x(), a1.y(), a1.z());
}

//------------------------------------------------------------------------------
/**
*/
void
UniversalJoint::RenderDebug()
{
    if (this->IsAttached())
    {
        Math::matrix44 m = Math::matrix44::identity();
        dVector3 curAnchor;
        dJointGetUniversalAnchor(this->odeJointId, curAnchor);
        m.scale(Math::vector(0.1f, 0.1f, 0.1f));
        m.translate(Math::vector(curAnchor[0], curAnchor[1], curAnchor[2]));
        DebugShapeRenderer::Instance()->DrawSphere(m, this->GetDebugVisualizationColor());
    }
}

} // namespace Physics