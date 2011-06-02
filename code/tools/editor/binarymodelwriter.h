#pragma once
//------------------------------------------------------------------------------
/**
    @class ToolkitUtil::BinaryModelWriter
    
    Stream writer for binary model files.
    
    (C) 2008 Radon Labs GmbH
*/
#include "models/modelwriter.h"
#include "io/binarywriter.h"

//------------------------------------------------------------------------------
namespace Editor
{
class BinaryModelWriter : public Models::ModelWriter
{
    __DeclareClass(BinaryModelWriter);
public:
    /// constructor
    BinaryModelWriter();
    /// destructor
    virtual ~BinaryModelWriter();

    /// begin reading from the stream
    virtual bool Open();
    /// end reading from the stream
    virtual void Close();

    /// begin writing a new model
    virtual bool BeginModel(const Util::String& className, Util::FourCC classFourCC, const Util::String& name);
    /// begin writing a model node
    virtual bool BeginModelNode(const Util::String& className, Util::FourCC classFourCC, const Util::String& name);
    /// begin a data tag
    virtual void BeginTag(const Util::String& name, Util::FourCC tagFourCC);
    /// write bool value
    virtual void WriteBool(bool b);
    /// write int value
    virtual void WriteInt(int i);
    /// write float value
    virtual void WriteFloat(float f);
    /// write float4 value
    virtual void WriteFloat4(const Math::float4& f);
    /// write string value
    virtual void WriteString(const Util::String& s);
    /// write int array value
    virtual void WriteIntArray(const Util::Array<int>& a);
    /// end a data tag
    virtual void EndTag();
    /// end a model node
    virtual void EndModelNode();
    /// end model
    virtual void EndModel();

private:
    Ptr<IO::BinaryWriter> writer;
};

} // namespace ToolkitUtil
//------------------------------------------------------------------------------

