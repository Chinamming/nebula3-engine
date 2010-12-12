#pragma once
#ifndef HTTP_HTMLPAGEWRITER_H
#define HTTP_HTMLPAGEWRITER_H
//------------------------------------------------------------------------------
/**
    @class Http::HtmlPageWriter
    
    A stream writer which supports writing a HTML-formatted page into a stream.
    
    (C) 2007 Radon Labs GmbH
*/
#include "io/streamwriter.h"
#include "http/html/htmlelement.h"
#include "io/xmlwriter.h"
#include "util/array.h"
#include "util/stack.h"

//------------------------------------------------------------------------------
namespace Http
{
class HtmlPageWriter : public IO::StreamWriter
{
    __DeclareClass(HtmlPageWriter);
public:
    /// constructor
    HtmlPageWriter();
    /// destructor
    virtual ~HtmlPageWriter();
    /// set the title of the page
    void SetTitle(const Util::String& title);
    /// begin writing the stream
    virtual bool Open();
    /// end writing the stream
    virtual void Close();

    /// add an attribute for the next element
    void AddAttr(const Util::String& name, const Util::String& value);
    /// begin a generic element
    void Begin(HtmlElement::Code e);
    /// end a generic element
    void End(HtmlElement::Code e);
    /// shortcut for Begin()/Text()/End()
    void Element(HtmlElement::Code e, const Util::String& text);
    /// write a line break
    void LineBreak();
    /// write a horizontal rule
    void HorizontalRule();
    /// add inline text
    void Text(const Util::String& t);

    /// write a 2-element table row
    void TableRow2(const Util::String& col0, const Util::String& col1);

private:
    Util::String title;
    Ptr<IO::XmlWriter> xmlWriter;
    Util::Array<Util::KeyValuePair<Util::String,Util::String> > attrs;
    Util::Stack<HtmlElement::Code> elementStack;
};

//------------------------------------------------------------------------------
/**
*/
inline void
HtmlPageWriter::SetTitle(const Util::String& t)
{
    this->title = t;
}

} // namespace Http
//------------------------------------------------------------------------------
#endif
    