//------------------------------------------------------------------------------
//  texturepagehandler.cc
//  (C) 2007 Radon Labs GmbH
//------------------------------------------------------------------------------
#include "stdneb.h"
#include "coregraphics/debug/texturepagehandler.h"
#include "coregraphics/texture.h"
#include "http/html/htmlpagewriter.h"
#include "resources/resourcemanager.h"
#include "coregraphics/streamtexturesaver.h"
#include "io/ioserver.h"
#include "coregraphics/imagefileformat.h"

namespace Debug
{
__ImplementClass(Debug::TexturePageHandler, 'DTXH', Http::HttpRequestHandler);

using namespace IO;
using namespace CoreGraphics;
using namespace Util;
using namespace Http;
using namespace Resources;

//------------------------------------------------------------------------------
/**
*/
TexturePageHandler::TexturePageHandler()
{
    this->SetName("Textures");
    this->SetDesc("show debug information about texture resources");
    this->SetRootLocation("texture");
}

//------------------------------------------------------------------------------
/**
*/
void
TexturePageHandler::HandleRequest(const Ptr<HttpRequest>& request)
{
    n_assert(HttpMethod::Get == request->GetMethod());

    // first check if a command has been defined in the URI
    Dictionary<String,String> query = request->GetURI().ParseQuery();
    if (query.Contains("img"))
    {
        request->SetStatus(this->HandleImageRequest(query, request->GetResponseContentStream()));
        return;
    }
    else if (query.Contains("texinfo"))
    {
        request->SetStatus(this->HandleTextureInfoRequest(ResourceId(query["texinfo"]), request->GetResponseContentStream()));
        return;
    }

    // no command, send the Texture home page
    Ptr<HtmlPageWriter> htmlWriter = HtmlPageWriter::Create();
    htmlWriter->SetStream(request->GetResponseContentStream());
    htmlWriter->SetTitle("Nebula3 Textures");
    if (htmlWriter->Open())
    {
        htmlWriter->Element(HtmlElement::Heading1, "Texture Resources");
        htmlWriter->AddAttr("href", "/index.html");
        htmlWriter->Element(HtmlElement::Anchor, "Home");
        htmlWriter->LineBreak();
        htmlWriter->LineBreak();

        // get all texture resources
        Array<Ptr<Resource> > texResources = ResourceManager::Instance()->GetResourcesByType(Texture::RTTI);

        // create a table of all existing textures
        htmlWriter->AddAttr("border", "1");
        htmlWriter->AddAttr("rules", "cols");
        htmlWriter->Begin(HtmlElement::Table);
            htmlWriter->AddAttr("bgcolor", "lightsteelblue");
            htmlWriter->Begin(HtmlElement::TableRow);
                htmlWriter->Element(HtmlElement::TableHeader, "ResId");
                htmlWriter->Element(HtmlElement::TableHeader, "State");
                htmlWriter->Element(HtmlElement::TableHeader, "UseCount");
                htmlWriter->Element(HtmlElement::TableHeader, "Type");
                htmlWriter->Element(HtmlElement::TableHeader, "Width");
                htmlWriter->Element(HtmlElement::TableHeader, "Height");
                htmlWriter->Element(HtmlElement::TableHeader, "Depth");
                htmlWriter->Element(HtmlElement::TableHeader, "Mips");
                htmlWriter->Element(HtmlElement::TableHeader, "Format");
            htmlWriter->End(HtmlElement::TableRow);

            // iterate over shared resources
            IndexT i;
            for (i = 0; i < texResources.Size(); i++)
            {
                const Ptr<Texture>& tex = texResources[i].downcast<Texture>();
                htmlWriter->Begin(HtmlElement::TableRow);
                    if (tex->GetState() == Resource::Loaded)
                    {
                        // only loaded texture can be inspected
                        htmlWriter->Begin(HtmlElement::TableData);
                            htmlWriter->AddAttr("href", "/texture?texinfo=" + tex->GetResourceId().AsString());
                            htmlWriter->Element(HtmlElement::Anchor, tex->GetResourceId().Value());
                        htmlWriter->End(HtmlElement::TableData);
                    }
                    else
                    {
                        htmlWriter->Element(HtmlElement::TableData, tex->GetResourceId().Value());
                    }

                    String resState;
                    switch (tex->GetState())
                    {
                        case Resource::Initial:     resState = "Initial"; break;
                        case Resource::Loaded:      resState = "Loaded"; break;
                        case Resource::Pending:     resState = "Pending"; break;
                        case Resource::Failed:      resState = "FAILED"; break;
                        case Resource::Cancelled:   resState = "Cancelled"; break;
                        default:                    resState = "CANT HAPPEN"; break;
                    }
                    htmlWriter->Element(HtmlElement::TableData, resState);
                    if (tex->GetState() == Resource::Loaded)
                    {
                        htmlWriter->Element(HtmlElement::TableData, String::FromInt(tex->GetUseCount()));
                        switch (tex->GetType())
                        {
                            case Texture::Texture2D:    htmlWriter->Element(HtmlElement::TableData, "2D"); break;
                            case Texture::Texture3D:    htmlWriter->Element(HtmlElement::TableData, "3D"); break;
                            case Texture::TextureCube:  htmlWriter->Element(HtmlElement::TableData, "CUBE"); break;
                            default:                    htmlWriter->Element(HtmlElement::TableData, "ERROR"); break;
                        }
                        htmlWriter->Element(HtmlElement::TableData, String::FromInt(tex->GetWidth()));
                        htmlWriter->Element(HtmlElement::TableData, String::FromInt(tex->GetHeight()));
                        htmlWriter->Element(HtmlElement::TableData, String::FromInt(tex->GetDepth()));
                        htmlWriter->Element(HtmlElement::TableData, String::FromInt(tex->GetNumMipLevels()));
                        htmlWriter->Element(HtmlElement::TableData, PixelFormat::ToString(tex->GetPixelFormat()));
                    }
                    else
                    {
                        // texture not currently loaded
                    }
                htmlWriter->End(HtmlElement::TableRow);
            }
        htmlWriter->End(HtmlElement::Table);
        htmlWriter->Close();
        request->SetStatus(HttpStatus::OK);
    }
    else
    {
        request->SetStatus(HttpStatus::InternalServerError);
    }
}

//------------------------------------------------------------------------------
/**
    Handle a "raw" texture image request.
*/
HttpStatus::Code
TexturePageHandler::HandleImageRequest(const Dictionary<String,String>& query, const Ptr<Stream>& responseStream)
{
    n_assert(query.Contains("img"));
    const Ptr<ResourceManager>& resManager = ResourceManager::Instance();
    
    // get input args
    ResourceId resId = ResourceId(query["img"]);
    ImageFileFormat::Code format = ImageFileFormat::InvalidImageFileFormat;
    if (query.Contains("fmt"))
    {
        format = ImageFileFormat::FromString(query["fmt"]);
    }
    if (ImageFileFormat::InvalidImageFileFormat == format)
    {
        format = ImageFileFormat::PNG;
    }
    IndexT mipLevel = 0;
    if (query.Contains("mip"))
    {
        mipLevel = query["mip"].AsInt();
    }

    // check if the request resource exists and is a texture
    if (!resManager->HasResource(resId))
    {
        return HttpStatus::NotFound;
    }
    const Ptr<Resource>& res = resManager->LookupResource(resId);
    if (!res->IsA(Texture::RTTI))
    {
        // resource exists but is not a texture
        return HttpStatus::NotFound;
    }
    const Ptr<Texture>& tex = res.downcast<Texture>();

    // attach a StreamTextureSaver to the texture
    // NOTE: the StreamSaver is expected to set the media type on the stream!
    HttpStatus::Code httpStatus = HttpStatus::InternalServerError;
    Ptr<StreamTextureSaver> saver = StreamTextureSaver::Create();
    saver->SetStream(responseStream);
    saver->SetFormat(format);
    saver->SetMipLevel(mipLevel);
    tex->SetSaver(saver.upcast<ResourceSaver>());
    if (tex->Save())
    {
        httpStatus = HttpStatus::OK;
    }
    tex->SetSaver(0);
    return httpStatus;
}

//------------------------------------------------------------------------------
/**
    Handle a texture info request.
*/
HttpStatus::Code
TexturePageHandler::HandleTextureInfoRequest(const ResourceId& resId, const Ptr<Stream>& responseContentStream)
{
    // lookup the texture in the ResourceManager
    const Ptr<ResourceManager>& resManager = ResourceManager::Instance();
    if (!resManager->HasResource(resId))
    {
        return HttpStatus::NotFound;
    }
    const Ptr<Resource>& res = resManager->LookupResource(resId);
    if (!res->IsA(Texture::RTTI))
    {
        // resource exists but is not a texture
        return HttpStatus::NotFound;
    }
    const Ptr<Texture>& tex = res.downcast<Texture>();

    Ptr<HtmlPageWriter> htmlWriter = HtmlPageWriter::Create();
    htmlWriter->SetStream(responseContentStream);
    htmlWriter->SetTitle("Nebula3 Texture Info");
    if (htmlWriter->Open())
    {
        htmlWriter->Element(HtmlElement::Heading1, resId.Value());
        htmlWriter->AddAttr("href", "/index.html");
        htmlWriter->Element(HtmlElement::Anchor, "Home");
        htmlWriter->LineBreak();
        htmlWriter->AddAttr("href", "/texture");
        htmlWriter->Element(HtmlElement::Anchor, "Textures Home");
        htmlWriter->LineBreak();
        htmlWriter->LineBreak();
    
        // display some info about the texture
        htmlWriter->Begin(HtmlElement::Table);
            htmlWriter->Begin(HtmlElement::TableRow);
                htmlWriter->Element(HtmlElement::TableData, "Resource Id: ");
                htmlWriter->Element(HtmlElement::TableData, resId.Value());
            htmlWriter->End(HtmlElement::TableRow);
            htmlWriter->Begin(HtmlElement::TableRow);
                htmlWriter->Element(HtmlElement::TableData, "Resolved Path: ");
                htmlWriter->Element(HtmlElement::TableData, AssignRegistry::Instance()->ResolveAssigns(resId.Value()).AsString());
            htmlWriter->End(HtmlElement::TableRow);
            htmlWriter->Begin(HtmlElement::TableRow);
                htmlWriter->Element(HtmlElement::TableData, "Use Count: ");
                htmlWriter->Element(HtmlElement::TableData, String::FromInt(tex->GetUseCount()));
            htmlWriter->End(HtmlElement::TableRow);
            htmlWriter->Begin(HtmlElement::TableRow);
                htmlWriter->Element(HtmlElement::TableData, "Type: ");
                switch (tex->GetType())
                {
                    case Texture::Texture2D:    htmlWriter->Element(HtmlElement::TableData, "2D"); break;
                    case Texture::Texture3D:    htmlWriter->Element(HtmlElement::TableData, "3D"); break;
                    case Texture::TextureCube:  htmlWriter->Element(HtmlElement::TableData, "CUBE"); break;
                    default:                    htmlWriter->Element(HtmlElement::TableData, "ERROR"); break;
                }
            htmlWriter->End(HtmlElement::TableRow);
            htmlWriter->Begin(HtmlElement::TableRow);
                htmlWriter->Element(HtmlElement::TableData, "Width: ");
                htmlWriter->Element(HtmlElement::TableData, String::FromInt(tex->GetWidth()));
            htmlWriter->End(HtmlElement::TableRow);
            htmlWriter->Begin(HtmlElement::TableRow);
                htmlWriter->Element(HtmlElement::TableData, "Height: ");
                htmlWriter->Element(HtmlElement::TableData, String::FromInt(tex->GetHeight()));
            htmlWriter->End(HtmlElement::TableRow);
            htmlWriter->Begin(HtmlElement::TableRow);
                htmlWriter->Element(HtmlElement::TableData, "Depth: ");
                htmlWriter->Element(HtmlElement::TableData, String::FromInt(tex->GetDepth()));
            htmlWriter->End(HtmlElement::TableRow);
            htmlWriter->Begin(HtmlElement::TableRow);
                htmlWriter->Element(HtmlElement::TableData, "Mip Levels: ");
                htmlWriter->Element(HtmlElement::TableData, String::FromInt(tex->GetNumMipLevels()));
            htmlWriter->End(HtmlElement::TableRow);
            htmlWriter->Begin(HtmlElement::TableRow);
                htmlWriter->Element(HtmlElement::TableData, "Pixel Format: ");
                htmlWriter->Element(HtmlElement::TableData, PixelFormat::ToString(tex->GetPixelFormat()));
            htmlWriter->End(HtmlElement::TableRow);
        htmlWriter->End(HtmlElement::Table);
        htmlWriter->LineBreak();

        // display the texture image data
        IndexT mipLevel;
        for (mipLevel = 0; mipLevel < tex->GetNumMipLevels(); mipLevel++)
        {
            String fmt;
            fmt.Format("/texture?img=%s&mip=%d", resId.Value(), mipLevel);
            htmlWriter->AddAttr("src", fmt);
            htmlWriter->Element(HtmlElement::Image, "");
        }
        htmlWriter->Close();
        return HttpStatus::OK;
    }
    return HttpStatus::InternalServerError;
}

} // namespace CoreGraphics
