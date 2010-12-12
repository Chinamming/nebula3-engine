#pragma once
//------------------------------------------------------------------------------
/**
    @class Win32::Win32DisplayDevice
    
    Win32 implementation of DisplayDevice class. Manages the application
    window.
    
    (C) 2006 Radon Labs GmbH
*/
#include "coregraphics/base/displaydevicebase.h"

//------------------------------------------------------------------------------
namespace Win32
{
class Win32DisplayDevice : public Base::DisplayDeviceBase
{
    __DeclareClass(Win32DisplayDevice);
    __DeclareSingleton(Win32DisplayDevice);
public:
    /// constructor
    Win32DisplayDevice();
    /// destructor
    virtual ~Win32DisplayDevice();

    /// open the display
    virtual bool Open();
    /// close the display
    virtual void Close();
    /// process window system messages, call this method once per frame
    virtual void ProcessWindowMessages();
    
    /// get the application window HWND
    HWND GetHwnd() const;

protected:
    /// the WinProc
    static LRESULT CALLBACK WinProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
    /// open the application window
    virtual bool OpenWindow();
    /// close the application window
    virtual void CloseWindow();
    /// called on WM_SIZE when window is minimized
    virtual void OnMinimized();
    /// called on WM_SIZE when window is restored
    virtual void OnRestored();
    /// called on WM_SETCURSOR
    virtual bool OnSetCursor();
    /// called on WM_PAINT
    virtual void OnPaint();
    /// called on WM_SETFOCUS
    virtual void OnSetFocus();
    /// called on WM_KILLFOCUS
    virtual void OnKillFocus();
    /// called on WM_CLOSE to request if window should be closed
    virtual void OnCloseRequested();
    /// called when Alt-Enter is pressed
    virtual void OnToggleFullscreenWindowed();
    /// called on WM_KEYDOWN
    virtual void OnKeyDown(WPARAM wParam);
    /// called on WM_KEYUP
    virtual void OnKeyUp(WPARAM wParam);
    /// called on WM_CHAR
    virtual void OnChar(WPARAM wParam);
    /// called on mouse button event
    virtual void OnMouseButton(UINT uMsg, LPARAM lParam);
    /// called on WM_MOUSEMOVE
    virtual void OnMouseMove(LPARAM lParam);
    /// called on WM_MOUSEWHEEL
    virtual void OnMouseWheel(WPARAM wParam);
    /// translate a Windows virtual key code into a Nebula3 key code
    Input::Key::Code TranslateKeyCode(WPARAM wParam) const;
    /// adjust window size taking client area into account
    virtual CoreGraphics::DisplayMode ComputeAdjustedWindowRect();
    /// compute absolute mouse position from lParam
    Math::float2 ComputeAbsMousePos(LPARAM lParam) const;
    /// compute normalized mouse position from absolute mouse pos
    Math::float2 ComputeNormMousePos(const Math::float2& absMousePos) const;

    HINSTANCE hInst;
    HWND hWnd;
    HACCEL hAccel;
    DWORD windowedStyle;
    DWORD fullscreenStyle;
    DWORD childWindowStyle;

    enum
    {
        AccelToggleFullscreen = 1001,
    };
};

//------------------------------------------------------------------------------
/**
*/
inline HWND
Win32DisplayDevice::GetHwnd() const
{
    return this->hWnd;
}

} // namespace CoreGraphics
//------------------------------------------------------------------------------

    