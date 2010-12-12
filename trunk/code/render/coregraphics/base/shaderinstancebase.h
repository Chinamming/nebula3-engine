#pragma once
//------------------------------------------------------------------------------
/**
    @class Base::ShaderInstanceBase
    
    A shader instance object is created from a shader and contains a local copy
    of the original shader state which can be modified through ShaderVariable
    objects. Shader instance objects are created directly through the 
    shader server.
    
    (C) 2007 Radon Labs GmbH
*/
#include "core/refcounted.h"
#include "util/string.h"
#include "coregraphics/shadervariable.h"
#include "coregraphics/shaderfeature.h"
#include "coregraphics/shadervariation.h"

namespace CoreGraphics
{
    class Shader;
}

//------------------------------------------------------------------------------
namespace Base
{
class ShaderInstanceBase : public Core::RefCounted
{
    __DeclareClass(ShaderInstanceBase);
public:
    /// constructor
    ShaderInstanceBase();
    /// destructor
    virtual ~ShaderInstanceBase();
    
    /// discard the shader instance, must be called when instance no longer needed
    void Discard();
    /// return true if this object is valid
    bool IsValid() const;
    /// get pointer to original shader which created this instance
    const Ptr<CoreGraphics::Shader>& GetOriginalShader() const;

    /// return true if the shader instance has a variable by name
    bool HasVariableByName(const CoreGraphics::ShaderVariable::Name& n) const;
    /// return true if shader has variable by semantic
    bool HasVariableBySemantic(const CoreGraphics::ShaderVariable::Semantic& n) const;
    /// get number of variables
    SizeT GetNumVariables() const;
    /// get a variable by index
    const Ptr<CoreGraphics::ShaderVariable>& GetVariableByIndex(IndexT i) const;
    /// get a variable by name
    const Ptr<CoreGraphics::ShaderVariable>& GetVariableByName(const CoreGraphics::ShaderVariable::Name& n) const;
    /// get a variable by semantic
    const Ptr<CoreGraphics::ShaderVariable>& GetVariableBySemantic(const CoreGraphics::ShaderVariable::Semantic& s) const;

    /// return true if variation exists by matching feature mask
    bool HasVariation(CoreGraphics::ShaderFeature::Mask featureMask) const;
    /// get number of variations in the shader
    SizeT GetNumVariations() const;
    /// get shader variation by index
    const Ptr<CoreGraphics::ShaderVariation>& GetVariationByIndex(IndexT i) const;
    /// get shader variation by feature mask
    const Ptr<CoreGraphics::ShaderVariation>& GetVariationByFeatureMask(CoreGraphics::ShaderFeature::Mask featureMask) const;
    /// select active variation by feature mask, return true if active variation has been changed
    bool SelectActiveVariation(CoreGraphics::ShaderFeature::Mask featureMask);
    /// get currently active variation
    const Ptr<CoreGraphics::ShaderVariation>& GetActiveVariation() const;

    /// begin rendering through the currently selected variation, returns no. passes
    SizeT Begin();
    /// begin pass
    void BeginPass(IndexT passIndex);
    /// commit changes before rendering
    void Commit();
    /// end pass
    void EndPass();
    /// end rendering through variation
    void End();
    
protected:   
    friend class ShaderBase;

    /// setup the shader instance from its original shader object
    virtual void Setup(const Ptr<CoreGraphics::Shader>& origShader);
    /// discard the shader instance
    virtual void Cleanup();

    bool inBegin;
    bool inBeginPass;
    Ptr<CoreGraphics::Shader> originalShader;
    Util::Array<Ptr<CoreGraphics::ShaderVariable> > variables;
    Util::Dictionary<CoreGraphics::ShaderVariable::Name, Ptr<CoreGraphics::ShaderVariable> > variablesByName;
    Util::Dictionary<CoreGraphics::ShaderVariable::Semantic, Ptr<CoreGraphics::ShaderVariable> > variablesBySemantic;
    Util::Dictionary<CoreGraphics::ShaderFeature::Mask, Ptr<CoreGraphics::ShaderVariation> > variations;
    Ptr<CoreGraphics::ShaderVariation> activeVariation;
};

//------------------------------------------------------------------------------
/**
*/
inline const Ptr<CoreGraphics::Shader>&
ShaderInstanceBase::GetOriginalShader() const
{
    return this->originalShader;
}

//------------------------------------------------------------------------------
/**
*/
inline bool
ShaderInstanceBase::HasVariableByName(const CoreGraphics::ShaderVariable::Name& n) const
{
    return this->variablesByName.Contains(n);
}

//------------------------------------------------------------------------------
/**
*/
inline bool
ShaderInstanceBase::HasVariableBySemantic(const CoreGraphics::ShaderVariable::Semantic& s) const
{
    return this->variablesBySemantic.Contains(s);
}

//------------------------------------------------------------------------------
/**
*/
inline SizeT
ShaderInstanceBase::GetNumVariables() const
{
    return this->variables.Size();
}

//------------------------------------------------------------------------------
/**
*/
inline const Ptr<CoreGraphics::ShaderVariable>&
ShaderInstanceBase::GetVariableByIndex(IndexT i) const
{
    return this->variables[i];
}

//------------------------------------------------------------------------------
/**
*/
inline SizeT
ShaderInstanceBase::GetNumVariations() const
{
    return this->variations.Size();
}

//------------------------------------------------------------------------------
/**
*/
inline const Ptr<CoreGraphics::ShaderVariation>& 
ShaderInstanceBase::GetVariationByIndex(IndexT i) const
{
    return this->variations.ValueAtIndex(i);
}

//------------------------------------------------------------------------------
/**
*/
inline bool
ShaderInstanceBase::HasVariation(CoreGraphics::ShaderFeature::Mask featureMask) const
{
    return this->variations.Contains(featureMask);
}

//------------------------------------------------------------------------------
/**
*/
inline const Ptr<CoreGraphics::ShaderVariation>&
ShaderInstanceBase::GetVariationByFeatureMask(CoreGraphics::ShaderFeature::Mask featureMask) const
{
    return this->variations[featureMask];
}

//------------------------------------------------------------------------------
/**
*/
inline const Ptr<CoreGraphics::ShaderVariation>&
ShaderInstanceBase::GetActiveVariation() const
{
    return this->activeVariation;
}

} // namespace CoreGraphics
//------------------------------------------------------------------------------


    