//------------------------------------------------------------------------------
//  triggerproperty.cc
//  (C) 2008 Radon Labs GmbH
//------------------------------------------------------------------------------
#include "stdneb.h"
#include "properties/triggerproperty.h"
#include "game/entity.h"
#include "basegamefeature/basegametiming/gametimesource.h"
#include "managers/envquerymanager.h"
#include "physicsprotocol.h"
#include "basegameprotocol.h"
#include "managers/entitymanager.h"
#include "physics/physicsserver.h"
#include "physics/level.h"
#include "physics/materialtable.h"
#include "physicsattr/physicsattributes.h"
#include "debugrender/debugshaperenderer.h"

namespace PhysicsFeature
{

using namespace Game;
using namespace Math;
using namespace BaseGameFeature;
using namespace Util;
using namespace Debug;

__ImplementClass(PhysicsFeature::TriggerProperty, 'PHTP', TransformableProperty);

//------------------------------------------------------------------------------
/**
*/
TriggerProperty::TriggerProperty() :	triggerActive(false),
										isOneWayTrigger(false)
{
    // empty
}

//------------------------------------------------------------------------------
/**
*/
TriggerProperty::~TriggerProperty()
{
    // empty
}

//------------------------------------------------------------------------------
/**
*/
void
TriggerProperty::SetupCallbacks()
{
    this->entity->RegisterPropertyCallback(this, BeginFrame);
    this->entity->RegisterPropertyCallback(this, RenderDebug);
}

//------------------------------------------------------------------------------
/**
*/
void
TriggerProperty::SetupDefaultAttributes()
{
    Property::SetupDefaultAttributes();
    SetupAttr(Attr::TriggerActive);
    SetupAttr(Attr::OneWayTrigger);
    SetupAttr(Attr::TriggerTimeStamp);
    SetupAttr(Attr::EntityTriggerRadius);
    SetupAttr(Attr::TriggerPhaseTime);
    SetupAttr(Attr::ReactivateTrigger);
    SetupAttr(Attr::ReactivateTriggerTime);
}

//------------------------------------------------------------------------------
/**
*/
void
TriggerProperty::SetupAcceptedMessages()
{
    this->RegisterMessage(PhysicsFeature::SetTriggerActive::Id);
    this->RegisterMessage(PhysicsFeature::IsTriggerActive::Id);
    this->RegisterMessage(PhysicsFeature::SetTriggerScale::Id);
    this->RegisterMessage(BaseGameFeature::SetTransform::Id);
}

//------------------------------------------------------------------------------
/**
*/
void
TriggerProperty::OnActivate()
{
    // initialize the time stamp with the current time
	float time = (float) BaseGameFeature::GameTimeSource::Instance()->GetTime();
    this->GetEntity()->SetFloat(Attr::TriggerTimeStamp, time);

	// set if active or not
	this->triggerActive = this->GetEntity()->GetBool(Attr::TriggerActive);

    // setup the custom trigger radius, so that the entity manager
    // will trigger the entity even when out of the global trigger radius
	float entityTriggerRadius = this->GetTriggerScale().length() * 1.25f;
    this->GetEntity()->SetFloat(Attr::EntityTriggerRadius, entityTriggerRadius);

	// get shape of the trigger, must be a box or a sphere
    this->shape = this->GetEntity()->GetString(Attr::Shape);

	// get the scaling of the trigger
    this->triggerScale = this->GetTriggerScale();

    // create our trigger shape
    this->CreateCollisionShape();

    TransformableProperty::OnActivate();
}

//------------------------------------------------------------------------------
/**
*/
void
TriggerProperty::OnDeactivate()
{
    this->DestroyCollisionShape();

    Property::OnDeactivate();
}

//------------------------------------------------------------------------------
/**
*/
void
TriggerProperty::HandleMessage(const Ptr<Messaging::Message>& msg)
{
    n_assert(msg != 0);

	if (msg->CheckId(PhysicsFeature::SetTriggerActive::Id))
    {
        // set the trigger activ flag, depending on status send by message
        Ptr<PhysicsFeature::SetTriggerActive> message = msg.cast<PhysicsFeature::SetTriggerActive>();
        n_assert(message != 0);
		this->triggerActive = message->GetActive();

		// set current timestamp
		float time = (float) BaseGameFeature::GameTimeSource::Instance()->GetTime();
		this->entity->SetFloat(Attr::TriggerTimeStamp, time);
    }
    else if (msg->CheckId(PhysicsFeature::IsTriggerActive::Id))
    {
        // ask for is this Trigger activ, so we give is activ flag 
        Ptr<PhysicsFeature::IsTriggerActive> isTriggerActive = msg.cast<PhysicsFeature::IsTriggerActive>();
        n_assert(isTriggerActive != 0);
		isTriggerActive->SetActive(this->triggerActive);
    }
    else if(msg->CheckId(PhysicsFeature::SetTriggerScale::Id))
    {
        // set new scale
		Ptr<PhysicsFeature::SetTriggerScale> scaleMsg = msg.cast<PhysicsFeature::SetTriggerScale>();
        n_assert(0 != scaleMsg);
        this->triggerScale = scaleMsg->GetScale();
        // destroy collision shape and recreate it with new scale
        this->DestroyCollisionShape();
        this->CreateCollisionShape();
    }
    else if (msg->CheckId(BaseGameFeature::SetTransform::Id))
    {
        if (this->collisionShape.isvalid())
        {
			// set position of shape
			Ptr<BaseGameFeature::SetTransform> message = msg.cast<BaseGameFeature::SetTransform>();
			const matrix44& m = message->GetMatrix();
            // set position of shape
            this->collisionShape->SetTransform(m);
        }
    }
    else
    {
        TransformableProperty::HandleMessage(msg);
    }
}

//------------------------------------------------------------------------------
/**
*/
void 
TriggerProperty::OnLoad()
{ 
    // get if trigger activated
    this->triggerActive = this->GetEntity()->GetBool(Attr::TriggerActive);

    // get if trigger should be used once
    this->isOneWayTrigger = this->GetEntity()->GetBool(Attr::OneWayTrigger);

    // get shape of the trigger, must be a box or a sphere
    this->shape = this->GetEntity()->GetString(Attr::Shape);
    n_assert(this->shape == "box" || this->shape == "sphere"); 

    // get the scaling of the trigger
    this->triggerScale = this->GetTriggerScale();

    // initialize trigger name if available
    if (this->GetEntity()->HasAttr(Attr::Name))
    {
        triggerName = this->GetEntity()->GetString(Attr::Name);
    }

	TransformableProperty::OnLoad();
}
//------------------------------------------------------------------------------
/**
*/
void 
TriggerProperty::OnSave()
{ 
    this->GetEntity()->SetBool(Attr::TriggerActive, this->triggerActive);
	TransformableProperty::OnSave();
}

//------------------------------------------------------------------------------
/**
*/
void
TriggerProperty::OnBeginFrame()
{   
    bool wasActive = this->triggerActive;

	if (this->triggerActive && !BaseGameFeature::GameTimeSource::Instance()->IsPaused())
    {
		// get timestamp
        float timestamp = this->GetEntity()->GetFloat(Attr::TriggerTimeStamp);
 
        // get all entities in trigger area
        // must be done first!
        this->SetCurrentEntitiesInTrigger();
        
        // fill enterdEntities, insideEntities, leftEntities
        this->FilterEntities();
                
        // oninside action on entities inside
        if(timestamp + this->GetEntity()->GetFloat(Attr::TriggerPhaseTime) < BaseGameFeature::GameTimeSource::Instance()->GetTime() )
        {
            // do work
			this->TriggerAction();

			// set new timestamp
            this->GetEntity()->SetFloat(Attr::TriggerTimeStamp,(float)BaseGameFeature::GameTimeSource::Instance()->GetTime());
        }        

        // now cleanup entitiesLastFrameInTrigger and refill with entities accepted by filter
        // this block must be done last! because entitiesLastFrameInTrigger is needed to calulate if 
        // an entity has left the trigger
        this->SetEntitiesLastFrameInTrigger();
    }
    else if(this->GetEntity()->GetBool(Attr::ReactivateTrigger))
    {
        float timestamp = this->GetEntity()->GetFloat(Attr::TriggerTimeStamp);
          
        // reactivate after reactivatetime         
        if(timestamp + this->GetEntity()->GetFloat(Attr::ReactivateTriggerTime) < BaseGameFeature::GameTimeSource::Instance()->GetTime() )
        {
			this->SetActiveStatus(true);           
		} 
    }
    
    if (wasActive && !this->triggerActive)
    {
        // clear entity caches on deactivation
        this->ClearEntities();
    }
}

//------------------------------------------------------------------------------
/**
       cleanup array:
       enterdEntities, insideEntities, leftEntities
       and fill them with entity from currEntitiesInTrigger and entities that where in trigger
*/
void 
TriggerProperty::FilterEntities()
{
    // cleanup my entity lists
    this->enterdEntities.Clear();
    this->insideEntities.Clear();
    this->leftEntities.Clear();

    // fill lists enterdEntities and insideEntities 
    IndexT i;
    for (i = 0; i < this->currEntitiesInTrigger.Size(); i++)
    {
        if ((this->entitiesLastFrameInTrigger.BinarySearchIndex(this->currEntitiesInTrigger[i]) == InvalidIndex))
        {
            //just entered
            this->enterdEntities.InsertSorted(this->currEntitiesInTrigger[i]);
            continue;
        } 

        //still inside
        this->insideEntities.InsertSorted(this->currEntitiesInTrigger[i]);
    }

    // fill leftEntities 
    for (i = 0; i < this->entitiesLastFrameInTrigger.Size(); i++)
    {
        if (this->insideEntities.BinarySearchIndex(entitiesLastFrameInTrigger[i]) == InvalidIndex 
            && this->enterdEntities.BinarySearchIndex(entitiesLastFrameInTrigger[i]) == InvalidIndex)
        {
            // left entities
			if(entitiesLastFrameInTrigger[i].isvalid() && entitiesLastFrameInTrigger[i]->IsActive())
			{
				this->leftEntities.Append(entitiesLastFrameInTrigger[i]);
			}
        }
    }
}

//------------------------------------------------------------------------------
/**
    cleanup currEntitiesInTrigger
    and fill with entities currently in trigger area
*/
void 
TriggerProperty::SetCurrentEntitiesInTrigger()
{
    // cleanup currEntitiesInTrigger list
    this->currEntitiesInTrigger.Clear();

    // enable trigger collide
    this->collisionShape->SetCategoryBits(Physics::Trigger);
    this->collisionShape->SetCollideBits(Physics::Trigger);

    Physics::FilterSet excludeSet;
    Array<Physics::ContactPoint> contacts;
    this->collisionShape->Collide(excludeSet, contacts);
    // convert physics entities back into game entities
    uint i;
    uint numContacts = contacts.Size();
    uint stamp = Physics::PhysicsServer::Instance()->GetUniqueStamp();
    const Ptr<EntityManager> entityManager = EntityManager::Instance();
    this->currEntitiesInTrigger.Clear();
    for (i = 0; i < numContacts; i++)
    {
        const Ptr<Physics::PhysicsEntity>& entity = contacts[i].GetPhysicsEntity();
        if (entity && (entity->GetStamp() != stamp)) 
        {
            entity->SetStamp(stamp);
            if (entityManager->ExistsEntityByUniqueId(entity->GetUserData()))
            {
                this->currEntitiesInTrigger.Append(entityManager->GetEntityByUniqueId(entity->GetUserData()));
            }
        }
    }

    //disable trigger collide bits
    this->collisionShape->SetCategoryBits(Physics::None);
    this->collisionShape->SetCollideBits(Physics::None);
}

//------------------------------------------------------------------------------
/**
    method must be invoked at at the end of the frame
*/
void 
TriggerProperty::SetEntitiesLastFrameInTrigger()
{
    // cleanup list
    this->entitiesLastFrameInTrigger.Clear();

    IndexT i;
    //make a new entities in Trigger list 
    for (i = 0; i < this->insideEntities.Size(); i++)
    {
        this->entitiesLastFrameInTrigger.InsertSorted(this->insideEntities[i]);
    }
    for (i = 0; i < this->enterdEntities.Size(); i++)
    {
        this->entitiesLastFrameInTrigger.InsertSorted(this->enterdEntities[i]);
    }
}

//------------------------------------------------------------------------------
/**
    get position of trigger entity 
*/
point
TriggerProperty::GetTriggerPosition()
{
    return this->GetEntity()->GetMatrix44(Attr::Transform).get_position();
}

//------------------------------------------------------------------------------
/**
    get transform of the trigger entity
*/
const matrix44
TriggerProperty::GetTriggerTransform()
{
    return this->GetEntity()->GetMatrix44(Attr::Transform);
}

//------------------------------------------------------------------------------
/**
*/
vector
TriggerProperty::GetTriggerScale()
{
    // get scale
	const matrix44& m = this->GetEntity()->GetMatrix44(Attr::Transform);
    vector scale(	m.get_xaxis().length(),
					m.get_yaxis().length(),
					m.get_zaxis().length());

    return scale;
}

//------------------------------------------------------------------------------
/**
*/
void
TriggerProperty::TriggerAction()
{
	// override in subclasses
}
//------------------------------------------------------------------------------
/**
    Render a debug visualization of the trigger.

    @param  t   transform matrix of my parent rigid body
*/
void
TriggerProperty::RenderDebugVisualization()
{
    n_assert(this->shape == "box" || this->shape == "sphere"); 
    float4 colorEnabled(0.9f, 0.58f, 1.0f, 0.3f); //(purple)
    float4 colorDisabled(0.0f, 0.0f, 0.0f, 0.08f);//(black)

    // check if sphere shape or box
    matrix44 transform = this->GetEntity()->GetMatrix44(Attr::Transform);
    float4 color = colorDisabled;
    if (this->triggerActive)
    {
        color = colorEnabled;
    }
    if (this->shape == "sphere")
    {
        //parameter = shape, transform, color + visibility 
        DebugShapeRenderer::Instance()->DrawSphere(transform, color);   
    }
    else
    {
        //parameter = shape, transform, color  + visibility 
        DebugShapeRenderer::Instance()->DrawBox(transform, color);   
    }
}

//------------------------------------------------------------------------------
/**
    Render a debug visualization of the trigger.

    @param  t   transform matrix of my parent rigid body
*/
void
TriggerProperty::OnRenderDebug()
{
    this->RenderDebugVisualization();
}

//------------------------------------------------------------------------------
/**
    Render a debug visualization of the trigger.

    @param t transform matrix of my parent rigid body
*/
bool
TriggerProperty::IsTriggerActive()
{
    return this->triggerActive;
}

//------------------------------------------------------------------------------
/**
    reset entity caches, use if trigger was inactive and gets activated
*/
void
TriggerProperty::ClearEntities()
{
    this->enterdEntities.Clear();
    this->entitiesLastFrameInTrigger.Clear();
    this->enterdEntities.Clear();
    this->insideEntities.Clear();
    this->leftEntities.Clear();
    this->currEntitiesInTrigger.Clear();
}

//------------------------------------------------------------------------------
/**
    send a message do it self, some other trigger properties may have 
    interest in the fact that the trigger has been turned off
*/
void
TriggerProperty::SetActiveStatus(bool status)
{
	Ptr<PhysicsFeature::SetTriggerActive> triggerStatusMgs = PhysicsFeature::SetTriggerActive::Create();
    triggerStatusMgs->SetActive(status);
	this->GetEntity()->SendSync(triggerStatusMgs.upcast<Messaging::Message>());
}

//------------------------------------------------------------------------------
/**
*/
void
TriggerProperty::CreateCollisionShape()
{
    n_assert(!this->collisionShape.isvalid());
    Ptr<Physics::PhysicsServer> physicsServer = Physics::PhysicsServer::Instance();
    const matrix44& m = GetEntity()->GetMatrix44(Attr::Transform);
    if (this->shape == "sphere")
    {
        this->collisionShape = (Physics::Shape*)physicsServer->CreateSphereShape(m, Physics::MaterialTable::StringToMaterialType("Wood"), this->triggerScale.z());
    }
    else
    {
        this->collisionShape = (Physics::Shape*)physicsServer->CreateBoxShape(m, Physics::MaterialTable::StringToMaterialType("Wood"), this->GetTriggerScale());
    }
    this->collisionShape->SetCollideBits(Physics::None);
    this->collisionShape->SetCategoryBits(Physics::None);
    Ptr<Physics::Level> curLevel = physicsServer->GetLevel();
    this->collisionShape->Attach(curLevel->GetOdeDynamicSpaceId());
}

//------------------------------------------------------------------------------
/**
*/
void
TriggerProperty::DestroyCollisionShape()
{
    n_assert(this->collisionShape.isvalid());
    Ptr<Physics::PhysicsServer> physicsServer = Physics::PhysicsServer::Instance();    
    Ptr<Physics::Level> curLevel = physicsServer->GetLevel();
    this->collisionShape->RemoveFromSpace();
    this->collisionShape = 0;
}
}; // namespace BaseGameFeature
