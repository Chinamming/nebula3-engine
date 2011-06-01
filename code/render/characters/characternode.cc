//------------------------------------------------------------------------------
//  characternode.cc
//  (C) 2008 Radon Labs GmbH
//------------------------------------------------------------------------------
#include "stdneb.h"
#include "characters/characternode.h"
#include "characters/characternodeinstance.h"
#include "resources/resourcemanager.h"
#include "models/modelinstance.h"

namespace Characters
{
__ImplementClass(Characters::CharacterNode, 'CHRN', Models::TransformNode);

using namespace Models;
using namespace Util;
using namespace IO;
using namespace CoreAnimation;
using namespace Resources;
using namespace Math;

//------------------------------------------------------------------------------
/**
*/
CharacterNode::CharacterNode()
{
    // empty
}

//------------------------------------------------------------------------------
/**
*/
CharacterNode::~CharacterNode()
{
    n_assert(!this->managedAnimResource.isvalid());
    n_assert(!this->managedVariationResource.isvalid());
    n_assert(!this->character.isvalid());
}

//------------------------------------------------------------------------------
/**
*/
Ptr<ModelNodeInstance>
CharacterNode::CreateNodeInstance() const
{
    Ptr<ModelNodeInstance> newInst = (ModelNodeInstance*) CharacterNodeInstance::Create();
    return newInst;
}

//------------------------------------------------------------------------------
/**
    Recursively create node instances and attach them to the provided
    model instance. The character node will not initially create node
    instances for the skins, but will only create skin node instances
    when they are actually set to visible. This eliminates a lot of
    dead weight for invisible skins!
*/
Ptr<ModelNodeInstance>
CharacterNode::RecurseCreateNodeInstanceHierarchy(const Ptr<ModelInstance>& modelInst, const Ptr<ModelNodeInstance>& parentNodeInst)
{
    // create a ModelNodeInstance of myself
    Ptr<ModelNodeInstance> myNodeInst = this->CreateNodeInstance();
    myNodeInst->Setup(modelInst, this, parentNodeInst);

    // DO NOT recurse into children (the child nodes represent the
    // skin model nodes)
    return myNodeInst;
}
//------------------------------------------------------------------------------
/**
*/
void
CharacterNode::OnAttachToModel(const Ptr<Model>& m)
{
    n_assert(!this->character.isvalid());
    TransformNode::OnAttachToModel(m);

    // create a Character object and prepare it for setup
    this->character = Character::Create();
    this->character->Setup();
}

//------------------------------------------------------------------------------
/**
*/
void
CharacterNode::OnRemoveFromModel()
{
    n_assert(this->character.isvalid());
    
    // discard character object
    this->character->Discard();
    this->character = 0;

    // up to parent class
    TransformNode::OnRemoveFromModel();
}

//------------------------------------------------------------------------------
/**
*/
void
CharacterNode::LoadResources()
{
    n_assert(!this->managedAnimResource.isvalid());
    n_assert(this->animResId.IsValid());

    // setup the managed resource
    this->managedAnimResource = ResourceManager::Instance()->CreateManagedResource(AnimResource::RTTI, this->animResId).downcast<ManagedAnimResource>();
    n_assert(this->managedAnimResource.isvalid());

    // setup the character's skin library from our children
    // (every child node represents one character skin)
    // FIXME: handle skin category!
    CharacterSkinLibrary& skinLib = this->character->SkinLibrary();
    StringAtom category("UnknownCategory");
    SizeT numSkins = this->children.Size();
    IndexT skinIndex;
    skinLib.ReserveSkins(numSkins);
    for (skinIndex = 0; skinIndex < numSkins; skinIndex++)
    {
        CharacterSkin skin(this->children[skinIndex], category, this->children[skinIndex]->GetName());
        this->character->SkinLibrary().AddSkin(skin);
    }

    if (this->variationResId.IsValid())
    {            
        // setup the managed resource for variations
        this->managedVariationResource = ResourceManager::Instance()->CreateManagedResource(AnimResource::RTTI, this->variationResId).downcast<ManagedAnimResource>();
        n_assert(this->managedVariationResource.isvalid()); 
    }

    // call parent class
    TransformNode::LoadResources();
}

//------------------------------------------------------------------------------
/**
*/
void
CharacterNode::UnloadResources()
{
    n_assert(this->managedAnimResource.isvalid());

    // discard managed anim resource
    ResourceManager::Instance()->DiscardManagedResource(this->managedAnimResource.upcast<ManagedResource>());
    this->managedAnimResource = 0;

    // discard variation resource name
    if (this->managedVariationResource.isvalid())
    {
        ResourceManager::Instance()->DiscardManagedResource(this->managedVariationResource.upcast<ManagedResource>());
        this->managedVariationResource = 0;
    }                                 

    // call parent class
    TransformNode::UnloadResources();
}

//------------------------------------------------------------------------------
/**
*/
Resource::State
CharacterNode::GetResourceState() const
{
    Resource::State state = TransformNode::GetResourceState();
    if (this->managedAnimResource->GetState() > state)
    {
        state = this->managedAnimResource->GetState();
    }
    if (this->managedVariationResource.isvalid()
        && this->managedVariationResource->GetState() > state)
    {
        state = this->managedVariationResource->GetState();
    }
    return state;
}

//------------------------------------------------------------------------------
/**
    Called when all resources of this Model are loaded. We need to setup
    the animation and variation libraries once this has happened.
*/
void
CharacterNode::OnResourcesLoaded()
{
    n_assert(this->managedAnimResource->GetState() == Resource::Loaded);

    // setup the character's animation library
    this->character->AnimationLibrary().Setup(this->managedAnimResource->GetAnimResource());

    if (this->managedVariationResource.isvalid())
    {
        n_assert(this->managedVariationResource->GetState() == Resource::Loaded);
        // setup the character's variation library
        this->character->VariationLibrary().Setup(this->managedVariationResource->GetAnimResource(), this->character->Skeleton());        
    }

    // call parent class
    TransformNode::OnResourcesLoaded();
}

//------------------------------------------------------------------------------
/**
*/
bool
CharacterNode::ParseDataTag(const FourCC& fourCC, const Ptr<BinaryReader>& reader)
{
    bool retval = true;
    if (FourCC('ANIM') == fourCC)
    {
        // Animation
        this->animResId = reader->ReadString();
    }
    else if (FourCC('NJNT') == fourCC)
    {
        // NumJoints
        SizeT numJoints = reader->ReadInt();
        this->character->Skeleton().Setup(numJoints);
    }
    else if (FourCC('JONT') == fourCC)
    {
        // Joint
        IndexT jointIndex       = reader->ReadInt();
        IndexT parentJointIndex = reader->ReadInt();
        vector poseTranslation  = reader->ReadFloat4();
        quaternion poseRotation = quaternion(reader->ReadFloat4());
        vector poseScale        = reader->ReadFloat4();
        StringAtom jointName    = reader->ReadString();

        // FIXME: Maya likes to return quaternions with de-normalized numbers in it,
        // this should better be fixed by the asset pipeline!
        poseRotation.undenormalize();

        this->character->Skeleton().SetupJoint(jointIndex, parentJointIndex, poseTranslation, poseRotation, poseScale, jointName);
    }
    else if (FourCC('VART') == fourCC)
    {
        // variation resource name
        this->variationResId = reader->ReadString();
    }
    else if (FourCC('NSKL') == fourCC)
    {
        // NumSkinLists
        this->character->SkinLibrary().ReserveSkinLists(reader->ReadInt());
    }
    else if (FourCC('SKNL') == fourCC)
    {
        // SkinList
        CharacterSkinList skinList;
        skinList.SetName(reader->ReadString());
        SizeT num = reader->ReadInt();
        IndexT i;
        Array<StringAtom> skins;
        for (i = 0; i < num; i++)
        {
            skins.Append(reader->ReadString());
        }
        skinList.SetSkins(skins);
        this->character->SkinLibrary().AddSkinList(skinList);
    }
    else
    {
        retval = TransformNode::ParseDataTag(fourCC, reader);
    }
    return retval;
}

#if NEBULA3_EDITOR
//------------------------------------------------------------------------------
/**
*/
bool
CharacterNode::WriteDataTag(Ptr<Models::ModelWriter>& writer)
{
	if(TransformNode::WriteDataTag(writer))
	{
		if( animResId.IsValid())
		{
			writer->BeginTag("Animation", FourCC('ANIM'));
			writer->WriteString(this->animResId.AsString());
			writer->EndTag();
		}		

		if( variationResId.IsValid())
		{
			writer->BeginTag("Variation", FourCC('VART'));
			writer->WriteString(this->variationResId.AsString());
			writer->EndTag();
		}		



		//for (IndexT index = 0 ; index < this->jointArray.Size();index++ )
		//{
		//	writer->BeginTag("Joint", 'JONT');
		//	writer->WriteInt(this->jointArray[index].jointIndex);
		//	writer->WriteInt(this->jointArray[index].parentJointIndex);
		//	writer->WriteFloat4(this->jointArray[index].poseTranslation);
		//	writer->WriteFloat4(this->jointArray[index].poseRotation);
		//	writer->WriteFloat4(this->jointArray[index].poseScale);
		//	writer->WriteString(this->jointArray[index].jointName);
		//	writer->EndTag();
		//}

		const Util::Array<CharacterJoint>& jointList = this->jointMaps.ValuesAsArray();
		if( !jointList.IsEmpty())
		{
			writer->BeginTag("NumJoints", 'NJNT');
			writer->WriteInt(jointList.Size());
			writer->EndTag();


			for (IndexT index=0; index < jointList.Size();index++ )
			{
				jointList[index].WriteDataTag(writer);
			}/// end for
		}

		if( !skinList.IsEmpty())
		{
			writer->BeginTag("NumSkinLists", 'NSKL');
			writer->WriteInt(1);
			writer->EndTag();

			writer->BeginTag("SkinList", 'SKNL');
			String skinName = this->GetName().AsString();
			skinName.Append("_skin");
			writer->WriteString(skinName);
			writer->WriteInt( skinList.Size() );
			for(int i = 0; i< skinList.Size(); i++ )
			{
				writer->WriteString( skinList[i] );
			}/// end for
			writer->EndTag();
		}		
	}

	return false;
}
#endif

} // namespace Characters