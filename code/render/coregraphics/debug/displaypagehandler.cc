//------------------------------------------------------------------------------
//  displaypagehandler.cc
//  (C) 2007 Radon Labs GmbH
//------------------------------------------------------------------------------
#include "stdneb.h"
#include "coregraphics/debug/displaypagehandler.h"
#include "coregraphics/displaydevice.h"
#include "http/html/htmlpagewriter.h"
#include "coregraphics/renderdevice.h"

namespace Debug
{
__ImplementClass(Debug::DisplayPageHandler, 'DDPH', Http::HttpRequestHandler);

using namespace CoreGraphics;
using namespace Http;
using namespace Util;
using namespace IO;

//------------------------------------------------------------------------------
/**
*/
DisplayPageHandler::DisplayPageHandler()
{
    this->SetName("Display");
    this->SetDesc("show Display debug information");
    this->SetRootLocation("display");
}

//------------------------------------------------------------------------------
/**
*/
void
DisplayPageHandler::HandleRequest(const Ptr<Http::HttpRequest>& request)
{
    n_assert(HttpMethod::Get == request->GetMethod());
    DisplayDevice* disp = DisplayDevice::Instance();

    // check if a screenshot is requested
    Dictionary<String,String> query = request->GetURI().ParseQuery();
    if (String::MatchPattern(request->GetURI().LocalPath(), "*/screenshot*"))
    {
        String fmt = query.Contains("fmt") ? query["fmt"] : "png";
        request->SetStatus(this->WriteScreenshot(fmt, request->GetResponseContentStream()));
        return;
    }

    // Wii specific: enable/disable screenshot mode
    #if __WII__
    if (String::MatchPattern(request->GetURI().LocalPath(), "*/togglescreenshotmode*"))
    {
        disp->GetScreenShotHelper().ToggleScreenShotMode();
    }
    #endif

    // configure a HTML page writer
    Ptr<HtmlPageWriter> htmlWriter = HtmlPageWriter::Create();
    htmlWriter->SetStream(request->GetResponseContentStream());
    htmlWriter->SetTitle("Nebula3 Display Info");
    if (htmlWriter->Open())
    {
        String str;
        htmlWriter->Element(HtmlElement::Heading1, "Display");
        htmlWriter->AddAttr("href", "/index.html");
        htmlWriter->Element(HtmlElement::Anchor, "Home");
        
        // Wii specific: enable/disable screenshot mode
        #if __WII__
            htmlWriter->LineBreak();
            str.Format("Toggle Screenshot Mode (currently %s)",
                disp->GetScreenShotHelper().IsEnabled() ? "enabled" : "disabled");
            htmlWriter->AddAttr("href", "/display/togglescreenshotmode");
            htmlWriter->Element(HtmlElement::Anchor, str);
        #endif            

        // write a screenshot
        htmlWriter->Element(HtmlElement::Heading3, "Screenshot");
        htmlWriter->AddAttr("src", "/display/screenshot");
        htmlWriter->Element(HtmlElement::Image, "");

        // write current display properties
        htmlWriter->Element(HtmlElement::Heading3, "Display Properties");
        htmlWriter->Begin(HtmlElement::Table);        
            htmlWriter->Begin(HtmlElement::TableRow);
                htmlWriter->Element(HtmlElement::TableData, "Display Open:");
                htmlWriter->Element(HtmlElement::TableData, disp->IsOpen() ? "Yes" : "No");
            htmlWriter->End(HtmlElement::TableRow);
            htmlWriter->Begin(HtmlElement::TableRow);
                htmlWriter->Element(HtmlElement::TableData, "Adapter:");
                htmlWriter->Element(HtmlElement::TableData, Adapter::ToString(disp->GetAdapter()));
            htmlWriter->End(HtmlElement::TableRow);
            htmlWriter->Begin(HtmlElement::TableRow);
                htmlWriter->Element(HtmlElement::TableData, "Size:");
                str.Format("%d x %d", disp->GetDisplayMode().GetWidth(), disp->GetDisplayMode().GetHeight());
                htmlWriter->Element(HtmlElement::TableData, str);
            htmlWriter->End(HtmlElement::TableRow);
            htmlWriter->Begin(HtmlElement::TableRow);
                htmlWriter->Element(HtmlElement::TableData, "Pixel Format:");
                htmlWriter->Element(HtmlElement::TableData, PixelFormat::ToString(disp->GetDisplayMode().GetPixelFormat()));
            htmlWriter->End(HtmlElement::TableRow);
            htmlWriter->Begin(HtmlElement::TableRow);
                htmlWriter->Element(HtmlElement::TableData, "AA Mode:");
                htmlWriter->Element(HtmlElement::TableData, AntiAliasQuality::ToString(disp->GetAntiAliasQuality()));
            htmlWriter->End(HtmlElement::TableRow);
            htmlWriter->Begin(HtmlElement::TableRow);
                htmlWriter->Element(HtmlElement::TableData, "Fullscreen:");
                htmlWriter->Element(HtmlElement::TableData, disp->IsFullscreen() ? "Yes" : "No");
            htmlWriter->End(HtmlElement::TableRow);
            htmlWriter->Begin(HtmlElement::TableRow);
                htmlWriter->Element(HtmlElement::TableData, "Mode Switch Enabled:");
                htmlWriter->Element(HtmlElement::TableData, disp->IsDisplayModeSwitchEnabled() ? "Yes" : "No");
            htmlWriter->End(HtmlElement::TableRow);
            htmlWriter->Begin(HtmlElement::TableRow);
                htmlWriter->Element(HtmlElement::TableData, "Triple Buffering Enabled:");
                htmlWriter->Element(HtmlElement::TableData, disp->IsTripleBufferingEnabled() ? "Yes" : "No");
            htmlWriter->End(HtmlElement::TableRow);
            htmlWriter->Begin(HtmlElement::TableRow);
                htmlWriter->Element(HtmlElement::TableData, "Always On Top:");
                htmlWriter->Element(HtmlElement::TableData, disp->IsAlwaysOnTop() ? "Yes" : "No");
            htmlWriter->End(HtmlElement::TableRow);
            htmlWriter->Begin(HtmlElement::TableRow);
                htmlWriter->Element(HtmlElement::TableData, "Vertical Sync:");
                htmlWriter->Element(HtmlElement::TableData, disp->IsVerticalSyncEnabled() ? "Yes" : "No");
            htmlWriter->End(HtmlElement::TableRow);
            htmlWriter->Begin(HtmlElement::TableRow);
                htmlWriter->Element(HtmlElement::TableData, "Icon Name:");
                htmlWriter->Element(HtmlElement::TableData, disp->GetIconName());
            htmlWriter->End(HtmlElement::TableRow);
            htmlWriter->Begin(HtmlElement::TableRow);
                htmlWriter->Element(HtmlElement::TableData, "Window Title:");
                htmlWriter->Element(HtmlElement::TableData, disp->GetWindowTitle());
            htmlWriter->End(HtmlElement::TableRow);
        htmlWriter->End(HtmlElement::Table);        

        this->WriteAdapterInfo(Adapter::Primary, htmlWriter);
        this->WriteAdapterInfo(Adapter::Secondary, htmlWriter);

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
    Write information about adapter to web page.
*/
void
DisplayPageHandler::WriteAdapterInfo(Adapter::Code adapter, const Ptr<HtmlPageWriter>& htmlWriter)
{
    DisplayDevice* disp = DisplayDevice::Instance();
    if (disp->AdapterExists(adapter))
    {
        String str;
        htmlWriter->Element(HtmlElement::Heading3, "Adapter: " + Adapter::ToString(adapter));
        AdapterInfo info = disp->GetAdapterInfo(adapter);
        htmlWriter->Element(HtmlElement::Heading4, "Adapter Info:");
        htmlWriter->Begin(HtmlElement::Table);
            htmlWriter->Begin(HtmlElement::TableRow);
                htmlWriter->Element(HtmlElement::TableData, "Current Mode:");
                DisplayMode curMode = disp->GetCurrentAdapterDisplayMode(adapter);
                str.Format("%d x %d %s", curMode.GetWidth(), curMode.GetHeight(), PixelFormat::ToString(curMode.GetPixelFormat()).AsCharPtr());
                htmlWriter->Element(HtmlElement::TableData, str);
            htmlWriter->End(HtmlElement::TableRow);
            htmlWriter->Begin(HtmlElement::TableRow);
                htmlWriter->Element(HtmlElement::TableData, "Driver Name:");
                htmlWriter->Element(HtmlElement::TableData, info.GetDriverName());
            htmlWriter->End(HtmlElement::TableRow);
            htmlWriter->Begin(HtmlElement::TableRow);
                htmlWriter->Element(HtmlElement::TableData, "Description:");
                htmlWriter->Element(HtmlElement::TableData, info.GetDescription());
            htmlWriter->End(HtmlElement::TableRow);
            htmlWriter->Begin(HtmlElement::TableRow);
                htmlWriter->Element(HtmlElement::TableData, "Device Name:");
                htmlWriter->Element(HtmlElement::TableData, info.GetDeviceName());
            htmlWriter->End(HtmlElement::TableRow);
            htmlWriter->Begin(HtmlElement::TableRow);
                htmlWriter->Element(HtmlElement::TableData, "Driver Version (High Part):");
                htmlWriter->Element(HtmlElement::TableData, String::FromInt(info.GetDriverVersionHighPart()));
            htmlWriter->End(HtmlElement::TableRow);
            htmlWriter->Begin(HtmlElement::TableRow);
                htmlWriter->Element(HtmlElement::TableData, "Driver Version (Low Part):");
                htmlWriter->Element(HtmlElement::TableData, String::FromInt(info.GetDriverVersionLowPart()));
            htmlWriter->End(HtmlElement::TableRow);
            htmlWriter->Begin(HtmlElement::TableRow);
                htmlWriter->Element(HtmlElement::TableData, "Vendor Id:");
                htmlWriter->Element(HtmlElement::TableData, String::FromInt(info.GetVendorId()));
            htmlWriter->End(HtmlElement::TableRow);
            htmlWriter->Begin(HtmlElement::TableRow);
                htmlWriter->Element(HtmlElement::TableData, "Device Id:");
                htmlWriter->Element(HtmlElement::TableData, String::FromInt(info.GetDeviceId()));
            htmlWriter->End(HtmlElement::TableRow);
            htmlWriter->Begin(HtmlElement::TableRow);
                htmlWriter->Element(HtmlElement::TableData, "Subsystem Id:");
                htmlWriter->Element(HtmlElement::TableData, String::FromInt(info.GetSubSystemId()));
            htmlWriter->End(HtmlElement::TableRow);
            htmlWriter->Begin(HtmlElement::TableRow);
                htmlWriter->Element(HtmlElement::TableData, "Revision:");
                htmlWriter->Element(HtmlElement::TableData, String::FromInt(info.GetRevision()));
            htmlWriter->End(HtmlElement::TableRow);
        htmlWriter->End(HtmlElement::Table);
        
        // enumerate display modes
        htmlWriter->Element(HtmlElement::Heading4, "Adapter Display Modes:");
        htmlWriter->Begin(HtmlElement::UnorderedList);
        uint pixelFormat;
        for (pixelFormat = 0; pixelFormat < PixelFormat::NumPixelFormats; pixelFormat++)
        {
            Array<DisplayMode> modes = disp->GetAvailableDisplayModes(adapter, PixelFormat::Code(pixelFormat));
            if (modes.Size() > 0)
            {
                htmlWriter->Element(HtmlElement::ListItem, PixelFormat::ToString(PixelFormat::Code(pixelFormat)));
                htmlWriter->Begin(HtmlElement::UnorderedList);
                IndexT modeIndex;
                for (modeIndex = 0; modeIndex < modes.Size(); modeIndex++)
                {
                    str.Format("%d x %d", modes[modeIndex].GetWidth(), modes[modeIndex].GetHeight());
                    htmlWriter->Element(HtmlElement::ListItem, str);
                }
                htmlWriter->End(HtmlElement::UnorderedList);
            }
        }
        htmlWriter->End(HtmlElement::UnorderedList);
    }            
}

//------------------------------------------------------------------------------
/**
*/
HttpStatus::Code
DisplayPageHandler::WriteScreenshot(const String& fileFormat, const Ptr<Stream>& responseContentStream)
{
    ImageFileFormat::Code fmt = ImageFileFormat::FromString(fileFormat);
    if (ImageFileFormat::InvalidImageFileFormat == fmt)
    {
        fmt = ImageFileFormat::PNG;
    }
    RenderDevice::Instance()->SaveScreenshot(fmt, responseContentStream);
    return HttpStatus::OK;
}

} // namespace Debug
