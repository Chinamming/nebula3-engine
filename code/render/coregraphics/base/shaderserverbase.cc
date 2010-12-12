//------------------------------------------------------------------------------
//  shaderserverbase.cc
//  (C) 2007 Radon Labs GmbH
//------------------------------------------------------------------------------
#include "stdneb.h"
#include "coregraphics/base/shaderserverbase.h"
#include "coregraphics/streamshaderloader.h"
#include "io/ioserver.h"
#include "io/textreader.h"
#include "coregraphics/shadersemantics.h"

namespace Base
{
__ImplementClass(Base::ShaderServerBase, 'SSRV', Core::RefCounted);
__ImplementSingleton(Base::ShaderServerBase);

using namespace CoreGraphics;
using namespace IO;
using namespace Util;
using namespace Resources;

//------------------------------------------------------------------------------
/**
*/
ShaderServerBase::ShaderServerBase() :
    curShaderFeatureBits(0),        
    isOpen(false)
{
    __ConstructSingleton;
}

//------------------------------------------------------------------------------
/**
*/
ShaderServerBase::~ShaderServerBase()
{
    n_assert(!this->IsOpen());
    __DestructSingleton;
}

//------------------------------------------------------------------------------
/**
*/
bool
ShaderServerBase::Open()
{
    n_assert(!this->isOpen);
    n_assert(this->shaders.IsEmpty());

    // open the shaders dictionary file
    Ptr<Stream> stream = IoServer::Instance()->CreateStream("shd:shaders.dic");
    Ptr<TextReader> textReader = TextReader::Create();
    textReader->SetStream(stream);
    if (textReader->Open())
    {
        Array<String> shaderPaths = textReader->ReadAllLines();
        textReader->Close();
        textReader = 0;
        
        IndexT i;
        for (i = 0; i < shaderPaths.Size(); i++)
        {
            ResourceId resId = shaderPaths[i];
            Ptr<Shader> newShader = Shader::Create();
            newShader->SetResourceId(resId);
            newShader->SetLoader(StreamShaderLoader::Create());
            newShader->SetAsyncEnabled(false);
            newShader->Load();
            if (newShader->IsLoaded())
            {
                newShader->SetLoader(0);
                this->shaders.Add(resId, newShader);
            }
            else
            {
                n_error("Failed to load shader '%s'!", shaderPaths[i].AsCharPtr());
            }
        }
    }
    else
    {
        n_error("ShaderServerBase: Failed to open shader dictionary!\n");
    }

    // create standard shader for access to shared variables
    if (this->HasShader(ResourceId("shd:shared")))
    {        
        this->sharedVariableShaderInst = this->CreateShaderInstance(ResourceId("shd:shared"));        
        n_assert(this->sharedVariableShaderInst.isvalid());

        // get shared object id shader variable
#if !__WII__ && !__PS3__
        this->objectIdShaderVar = this->sharedVariableShaderInst->GetVariableBySemantic(NEBUlA3_SEMANTIC_OBJECTID);
        n_assert(this->objectIdShaderVar.isvalid());
#endif
    }

    this->isOpen = true;
    return true;
}

//------------------------------------------------------------------------------
/**
*/
void
ShaderServerBase::Close()
{
    n_assert(this->isOpen);

    // release shared instance shader
    if (this->sharedVariableShaderInst.isvalid())
    {        
        this->sharedVariableShaderInst->Discard();
        this->sharedVariableShaderInst = 0;
    }

    // unload all currently loaded shaders
    IndexT i;
    for (i = 0; i < this->shaders.Size(); i++)
    {
        this->shaders.ValueAtIndex(i)->Unload();
    }
    this->shaders.Clear();

    this->isOpen = false;
}

//------------------------------------------------------------------------------
/**
    This creates a clone of a template shader. This is the only method
    to create a new shader object. When the shader instance is no longer
    needed, call UnregisterShaderInstance() for proper cleanup.
*/
Ptr<ShaderInstance>
ShaderServerBase::CreateShaderInstance(const ResourceId& resId)
{
    n_assert(resId.IsValid());

    // first check if the shader is already loaded
    if (!this->shaders.Contains(resId))
    {
        n_error("ShaderServer: shader '%s' not found!", resId.Value());
    }

    // create a shader instance object from the shader
    Ptr<ShaderInstance> shaderInstance = this->shaders[resId]->CreateShaderInstance();
    return shaderInstance;
}

//------------------------------------------------------------------------------
/**
*/
void 
ShaderServerBase::ApplyObjectId(IndexT i)
{   
    n_assert(i >= 0);
    n_assert(i < 256);
#if __PS3__
    if (this->GetActiveShaderInstance()->HasVariableBySemantic(NEBUlA3_SEMANTIC_OBJECTID))
    {
        this->objectIdShaderVar = this->GetActiveShaderInstance()->GetVariableBySemantic(NEBUlA3_SEMANTIC_OBJECTID);
    }       
#endif
    if (this->objectIdShaderVar.isvalid())
    {
        this->objectIdShaderVar->SetFloat(((float)i) / 255.0f);  
    }       
}
} // namespace Base
