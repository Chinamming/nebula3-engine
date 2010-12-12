#pragma once
//------------------------------------------------------------------------------
/**
    @class ToolkitUtil::ModelWriter
    
    Generic Nebula3 model file writer.
    
    (C) 2008 Radon Labs GmbH
*/
#include "io/streamwriter.h" 
#include "toolkitutil/platform.h"

//------------------------------------------------------------------------------
namespace ToolkitUtil
{
class ModelWriter : public IO::StreamWriter
{
    __DeclareClass(ModelWriter);
public:
    /// constructor
    ModelWriter();
    /// destructor
    virtual ~ModelWriter();

    /// set the target platform 
    void SetPlatform(Platform::Code platform);
    /// set a version number
    void SetVersion(int version);

    /// begin reading from the stream
    virtual bool Open() = 0;
    /// end reading from the stream
    virtual void Close() = 0;

    /// begin writing a new model
    virtual bool BeginModel(const Util::String& className, Util::FourCC classFourCC, const Util::String& name) = 0;
    /// begin writing a model node
    virtual bool BeginModelNode(const Util::String& className, Util::FourCC classFourCC, const Util::String& name) = 0;
    /// begin a data tag
    virtual void BeginTag(const Util::String& name, Util::FourCC tagFourCC) = 0;
    /// write int value
    virtual void WriteBool(bool b) = 0;
    /// write int value
    virtual void WriteInt(int i) = 0;
    /// write float value
    virtual void WriteFloat(float f) = 0;
    /// write float4 value
    virtual void WriteFloat4(const Math::float4& f) = 0;
    /// write string value
    virtual void WriteString(const Util::String& s) = 0;
    /// write int array value
    virtual void WriteIntArray(const Util::Array<int>& a) = 0;
    /// end a data tag
    virtual void EndTag() = 0;
    /// end a model node
    virtual void EndModelNode() = 0;
    /// end model
    virtual void EndModel() = 0;

protected:
    Platform::Code platform;
    int version;
};

//------------------------------------------------------------------------------
/**
*/
inline void
ModelWriter::SetPlatform(Platform::Code p)
{
    this->platform = p;
}

//------------------------------------------------------------------------------
/**
*/
inline void
ModelWriter::SetVersion(int v)
{
    this->version = v;
}

} // namespace ToolkitUtil
//------------------------------------------------------------------------------
