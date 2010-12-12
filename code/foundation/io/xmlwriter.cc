//------------------------------------------------------------------------------
//  xmlwriter.cc
//  (C) 2006 Radon Labs GmbH
//------------------------------------------------------------------------------
#include "stdneb.h"
#include "io/xmlwriter.h"

namespace IO
{
__ImplementClass(IO::XmlWriter, 'XMLW', IO::StreamWriter);

using namespace Util;
#if !__OSX__
using namespace Math;
#endif
    
//------------------------------------------------------------------------------
/**
*/
XmlWriter::XmlWriter() :
    xmlDocument(0),
    curNode(0)
{
    // empty
}

//------------------------------------------------------------------------------
/**
*/
XmlWriter::~XmlWriter()
{
    if (this->IsOpen())
    {
        this->Close();
    }
}

//------------------------------------------------------------------------------
/**
    Open the XML stream for writing. This will create a new TiXmlDocument
    object which will be written to the stream in Close().
*/
bool
XmlWriter::Open()
{
    n_assert(0 == this->xmlDocument);
    n_assert(0 == this->curNode);

    if (StreamWriter::Open())
    {
        // create xml document object
        this->xmlDocument = n_new(TiXmlDocument);

        // add declaration (<?xml version="1.0" encoding="UTF-8"?>)
        this->xmlDocument->InsertEndChild(TiXmlDeclaration("1.0", "UTF-8", ""));
        return true;
    }
    return false;
}

//------------------------------------------------------------------------------
/**
    Close the XML stream.
*/
void
XmlWriter::Close()
{
    n_assert(this->IsOpen());
    n_assert(0 != this->xmlDocument);

    // write XML data to stream
    this->xmlDocument->SaveStream(this->stream);
    
    // delete the XML document object
    n_delete(this->xmlDocument);
    this->xmlDocument = 0;
    this->curNode = 0;
        
    // close the stream
    StreamWriter::Close();
}

//------------------------------------------------------------------------------
/**
    Begin a new node. The new node will be set as the current
    node. Nodes may form a hierarchy. Make sure to finalize a node
    with a corresponding call to EndNode()!
*/
bool
XmlWriter::BeginNode(const String& name)
{
    n_assert(this->IsOpen());
    if (0 == this->curNode)
    {
        // create the root node
        this->curNode = this->xmlDocument->InsertEndChild(TiXmlElement(name.AsCharPtr()))->ToElement();
    }
    else
    {
        // create a child node
        this->curNode = this->curNode->InsertEndChild(TiXmlElement(name.AsCharPtr()))->ToElement();
    }
    return true;
}

//------------------------------------------------------------------------------
/**
    Finalize current node. This will set the parent of the current node as 
    new current node so that correct hierarchical behaviour is implemented.
*/
void
XmlWriter::EndNode()
{
    n_assert(this->IsOpen());
    n_assert(0 != this->curNode);

    TiXmlNode* parent = this->curNode->Parent();
    n_assert(parent);
    if (parent == this->xmlDocument)
    {
        // we're back at the root
        this->curNode = 0;
    }
    else
    {
        this->curNode = parent->ToElement();
    }
}

//------------------------------------------------------------------------------
/**
    Write inline text at current position.
*/
void
XmlWriter::WriteContent(const String& text)
{
    n_assert(0 != this->curNode);
    this->curNode->InsertEndChild(TiXmlText(text.AsCharPtr()));
}

//------------------------------------------------------------------------------
/**
    Write a comment into the XML file.
*/
void
XmlWriter::WriteComment(const String& comment)
{
    n_assert(0 != this->curNode);
    TiXmlComment tiXmlComment;
    tiXmlComment.SetValue(comment.AsCharPtr());
    this->curNode->InsertEndChild(tiXmlComment);
}

//------------------------------------------------------------------------------
/**
    Set the provided attribute to a string value.
*/
void
XmlWriter::SetString(const String& name, const String& value)
{
    n_assert(this->IsOpen());
    n_assert(0 != this->curNode);
    n_assert(name.IsValid());
    this->curNode->SetAttribute(name.AsCharPtr(), value.AsCharPtr());
}

//------------------------------------------------------------------------------
/**
    Set the provided attribute to a bool value.
*/
void
XmlWriter::SetBool(const String& name, bool value)
{
    String s;
    s.SetBool(value);
    this->SetString(name, s);
}

//------------------------------------------------------------------------------
/**
    Set the provided attribute to an int value.
*/
void
XmlWriter::SetInt(const String& name, int value)
{
    String s;
    s.SetInt(value);
    this->SetString(name, s);
}

//------------------------------------------------------------------------------
/**
    Set the provided attribute to a float value.
*/
void
XmlWriter::SetFloat(const String& name, float value)
{
    String s;
    s.SetFloat(value);
    this->SetString(name, s);
}

#if !__OSX__    
//------------------------------------------------------------------------------
/**
    Set the provided attribute to a float4 value.
*/
void
XmlWriter::SetFloat4(const String& name, const float4& value)
{
    String s;
    s.SetFloat4(value);
    this->SetString(name, s);
}

//------------------------------------------------------------------------------
/**
    Set the provided attribute to a matrix44 value.  The stream must be
    in Write or ReadWrite mode for this.
*/
void
XmlWriter::SetMatrix44(const String& name, const matrix44& value)
{
    String s;
    s.SetMatrix44(value);
    this->SetString(name, s);
}

//------------------------------------------------------------------------------
/**
*/
void 
XmlWriter::SetFloat2(const Util::String& name, const Math::float2& value)
{       
    String s;
    s.SetFloat2(value);
    this->SetString(name, s);
}
#endif
} // namespace IO
