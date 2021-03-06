////////////////////////////////////////////////////////////
//
// SFML - Simple and Fast Multimedia Library
// Copyright (C) 2007-2017 Laurent Gomila (laurent@sfml-dev.org)
//
// This software is provided 'as-is', without any express or implied warranty.
// In no event will the authors be held liable for any damages arising from the use of this software.
//
// Permission is granted to anyone to use this software for any purpose,
// including commercial applications, and to alter it and redistribute it freely,
// subject to the following restrictions:
//
// 1. The origin of this software must not be misrepresented;
//    you must not claim that you wrote the original software.
//    If you use this software in a product, an acknowledgment
//    in the product documentation would be appreciated but is not required.
//
// 2. Altered source versions must be plainly marked as such,
//    and must not be misrepresented as being the original software.
//
// 3. This notice may not be removed or altered from any source distribution.
//
////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////
// Headers
////////////////////////////////////////////////////////////
#include <AE/Window/WindowStyle.hpp> // important to be included first (conflict with None)
#include <AE/Window/Unix/WindowImplX11.hpp>
#include <AE/Window/Unix/Display.hpp>
#include <AE/Window/Unix/InputImpl.hpp>
#include <AE/System/Utf.hpp>
#include <AE/System/Err.hpp>
#include <AE/System/Mutex.hpp>
#include <AE/System/Lock.hpp>
#include <AE/System/Sleep.hpp>
#include <X11/Xlibint.h>
#include <X11/Xutil.h>
#include <X11/Xatom.h>
#include <X11/keysym.h>
#include <X11/extensions/Xrandr.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <libgen.h>
#include <fcntl.h>
#include <algorithm>
#include <vector>
#include <string>
#include <cstring>

#ifdef SFML_OPENGL_ES
    #include <AE/Window/EglContext.hpp>
    typedef ae::priv::EglContext ContextType;
#else
    #include <AE/Window/Unix/GlxContext.hpp>
    typedef ae::priv::GlxContext ContextType;
#endif

////////////////////////////////////////////////////////////
// Private data
////////////////////////////////////////////////////////////
namespace
{
    ae::priv::WindowImplX11*              fullscreenWindow = NULL;
    std::vector<ae::priv::WindowImplX11*> allWindows;
    ae::Mutex                             allWindowsMutex;
    ae::String                            windowManagerName;

    static const unsigned long            eventMask = FocusChangeMask      | ButtonPressMask     |
                                                      ButtonReleaseMask    | ButtonMotionMask    |
                                                      PointerMotionMask    | KeyPressMask        |
                                                      KeyReleaseMask       | StructureNotifyMask |
                                                      EnterWindowMask      | LeaveWindowMask     |
                                                      VisibilityChangeMask | PropertyChangeMask;

    static const unsigned int             maxTrialsCount = 5;

    // Filter the events received by windows (only allow those matching a specific window)
    Bool checkEvent(::Display*, XEvent* event, XPointer userData)
    {
        // Just check if the event matches the window
        return event->xany.window == reinterpret_cast< ::Window >(userData);
    }

    // Find the name of the current executable
    std::string findExecutableName()
    {
        // We use /proc/self/cmdline to get the command line
        // the user used to invoke this instance of the application
        int file = ::open("/proc/self/cmdline", O_RDONLY | O_NONBLOCK);

        if (file < 0)
            return "sfml";

        std::vector<char> buffer(256, 0);
        std::size_t offset = 0;
        ssize_t result = 0;

        while ((result = read(file, &buffer[offset], 256)) > 0)
        {
            buffer.resize(buffer.size() + result, 0);
            offset += result;
        }

        ::close(file);

        if (offset)
        {
            buffer[offset] = 0;

            // Remove the path to keep the executable name only
            return basename(&buffer[0]);
        }

        // Default fallback name
        return "sfml";
    }

    // Check if Extended Window Manager Hints are supported
    bool ewmhSupported()
    {
        static bool checked = false;
        static bool ewmhSupported = false;

        if (checked)
            return ewmhSupported;

        checked = true;

        Atom netSupportingWmCheck = ae::priv::getAtom("_NET_SUPPORTING_WM_CHECK", true);
        Atom netSupported = ae::priv::getAtom("_NET_SUPPORTED", true);

        if (!netSupportingWmCheck || !netSupported)
            return false;

        ::Display* display = ae::priv::OpenDisplay();

        Atom actualType;
        int actualFormat;
        unsigned long numItems;
        unsigned long numBytes;
        unsigned char* data;

        int result = XGetWindowProperty(display,
                                        DefaultRootWindow(display),
                                        netSupportingWmCheck,
                                        0,
                                        1,
                                        False,
                                        XA_WINDOW,
                                        &actualType,
                                        &actualFormat,
                                        &numItems,
                                        &numBytes,
                                        &data);

        if (result != Success || actualType != XA_WINDOW || numItems != 1)
        {
            if(result == Success)
                XFree(data);

            ae::priv::CloseDisplay(display);
            return false;
        }

        ::Window rootWindow = *reinterpret_cast< ::Window* >(data);

        XFree(data);

        if (!rootWindow)
        {
            ae::priv::CloseDisplay(display);
            return false;
        }

        result = XGetWindowProperty(display,
                                    rootWindow,
                                    netSupportingWmCheck,
                                    0,
                                    1,
                                    False,
                                    XA_WINDOW,
                                    &actualType,
                                    &actualFormat,
                                    &numItems,
                                    &numBytes,
                                    &data);

        if (result != Success || actualType != XA_WINDOW || numItems != 1)
        {
            if(result == Success)
                XFree(data);

            ae::priv::CloseDisplay(display);
            return false;
        }

        ::Window childWindow = *reinterpret_cast< ::Window* >(data);

        XFree(data);

        if (!childWindow)
        {
            ae::priv::CloseDisplay(display);
            return false;
        }

        // Conforming window managers should return the same window for both queries
        if (rootWindow != childWindow)
        {
            ae::priv::CloseDisplay(display);
            return false;
        }

        ewmhSupported = true;

        // We try to get the name of the window manager
        // for window manager specific workarounds
        Atom netWmName = ae::priv::getAtom("_NET_WM_NAME", true);

        if (!netWmName)
        {
            ae::priv::CloseDisplay(display);
            return true;
        }

        Atom utf8StringType = ae::priv::getAtom("UTF8_STRING");

        if (!utf8StringType)
            utf8StringType = XA_STRING;

        result = XGetWindowProperty(display,
                                    rootWindow,
                                    netWmName,
                                    0,
                                    0x7fffffff,
                                    False,
                                    utf8StringType,
                                    &actualType,
                                    &actualFormat,
                                    &numItems,
                                    &numBytes,
                                    &data);

        if (actualType && numItems)
        {
            // It seems the wm name string reply is not necessarily
            // null-terminated. The work around is to get its actual
            // length to build a proper string
            const char* begin = reinterpret_cast<const char*>(data);
            const char* end = begin + numItems;
            windowManagerName = ae::String::fromUtf8(begin, end);
        }

        if(result == Success)
            XFree(data);

        ae::priv::CloseDisplay(display);

        return true;
    }

    ae::Keyboard::Key keysymToSF(KeySym symbol)
    {
        switch (symbol)
        {
            case XK_Shift_L:      return ae::Keyboard::LShift;
            case XK_Shift_R:      return ae::Keyboard::RShift;
            case XK_Control_L:    return ae::Keyboard::LControl;
            case XK_Control_R:    return ae::Keyboard::RControl;
            case XK_Alt_L:        return ae::Keyboard::LAlt;
            case XK_Alt_R:        return ae::Keyboard::RAlt;
            case XK_Super_L:      return ae::Keyboard::LSystem;
            case XK_Super_R:      return ae::Keyboard::RSystem;
            case XK_Menu:         return ae::Keyboard::Menu;
            case XK_Escape:       return ae::Keyboard::Escape;
            case XK_semicolon:    return ae::Keyboard::SemiColon;
            case XK_slash:        return ae::Keyboard::Slash;
            case XK_equal:        return ae::Keyboard::Equal;
            case XK_minus:        return ae::Keyboard::Dash;
            case XK_bracketleft:  return ae::Keyboard::LBracket;
            case XK_bracketright: return ae::Keyboard::RBracket;
            case XK_comma:        return ae::Keyboard::Comma;
            case XK_period:       return ae::Keyboard::Period;
            case XK_apostrophe:   return ae::Keyboard::Quote;
            case XK_backslash:    return ae::Keyboard::BackSlash;
            case XK_grave:        return ae::Keyboard::Tilde;
            case XK_space:        return ae::Keyboard::Space;
            case XK_Return:       return ae::Keyboard::Return;
            case XK_KP_Enter:     return ae::Keyboard::Return;
            case XK_BackSpace:    return ae::Keyboard::BackSpace;
            case XK_Tab:          return ae::Keyboard::Tab;
            case XK_Prior:        return ae::Keyboard::PageUp;
            case XK_Next:         return ae::Keyboard::PageDown;
            case XK_End:          return ae::Keyboard::End;
            case XK_Home:         return ae::Keyboard::Home;
            case XK_Insert:       return ae::Keyboard::Insert;
            case XK_Delete:       return ae::Keyboard::Delete;
            case XK_KP_Add:       return ae::Keyboard::Add;
            case XK_KP_Subtract:  return ae::Keyboard::Subtract;
            case XK_KP_Multiply:  return ae::Keyboard::Multiply;
            case XK_KP_Divide:    return ae::Keyboard::Divide;
            case XK_Pause:        return ae::Keyboard::Pause;
            case XK_F1:           return ae::Keyboard::F1;
            case XK_F2:           return ae::Keyboard::F2;
            case XK_F3:           return ae::Keyboard::F3;
            case XK_F4:           return ae::Keyboard::F4;
            case XK_F5:           return ae::Keyboard::F5;
            case XK_F6:           return ae::Keyboard::F6;
            case XK_F7:           return ae::Keyboard::F7;
            case XK_F8:           return ae::Keyboard::F8;
            case XK_F9:           return ae::Keyboard::F9;
            case XK_F10:          return ae::Keyboard::F10;
            case XK_F11:          return ae::Keyboard::F11;
            case XK_F12:          return ae::Keyboard::F12;
            case XK_F13:          return ae::Keyboard::F13;
            case XK_F14:          return ae::Keyboard::F14;
            case XK_F15:          return ae::Keyboard::F15;
            case XK_Left:         return ae::Keyboard::Left;
            case XK_Right:        return ae::Keyboard::Right;
            case XK_Up:           return ae::Keyboard::Up;
            case XK_Down:         return ae::Keyboard::Down;
            case XK_KP_Insert:    return ae::Keyboard::Numpad0;
            case XK_KP_End:       return ae::Keyboard::Numpad1;
            case XK_KP_Down:      return ae::Keyboard::Numpad2;
            case XK_KP_Page_Down: return ae::Keyboard::Numpad3;
            case XK_KP_Left:      return ae::Keyboard::Numpad4;
            case XK_KP_Begin:     return ae::Keyboard::Numpad5;
            case XK_KP_Right:     return ae::Keyboard::Numpad6;
            case XK_KP_Home:      return ae::Keyboard::Numpad7;
            case XK_KP_Up:        return ae::Keyboard::Numpad8;
            case XK_KP_Page_Up:   return ae::Keyboard::Numpad9;
            case XK_a:            return ae::Keyboard::A;
            case XK_b:            return ae::Keyboard::B;
            case XK_c:            return ae::Keyboard::C;
            case XK_d:            return ae::Keyboard::D;
            case XK_e:            return ae::Keyboard::E;
            case XK_f:            return ae::Keyboard::F;
            case XK_g:            return ae::Keyboard::G;
            case XK_h:            return ae::Keyboard::H;
            case XK_i:            return ae::Keyboard::I;
            case XK_j:            return ae::Keyboard::J;
            case XK_k:            return ae::Keyboard::K;
            case XK_l:            return ae::Keyboard::L;
            case XK_m:            return ae::Keyboard::M;
            case XK_n:            return ae::Keyboard::N;
            case XK_o:            return ae::Keyboard::O;
            case XK_p:            return ae::Keyboard::P;
            case XK_q:            return ae::Keyboard::Q;
            case XK_r:            return ae::Keyboard::R;
            case XK_s:            return ae::Keyboard::S;
            case XK_t:            return ae::Keyboard::T;
            case XK_u:            return ae::Keyboard::U;
            case XK_v:            return ae::Keyboard::V;
            case XK_w:            return ae::Keyboard::W;
            case XK_x:            return ae::Keyboard::X;
            case XK_y:            return ae::Keyboard::Y;
            case XK_z:            return ae::Keyboard::Z;
            case XK_0:            return ae::Keyboard::Num0;
            case XK_1:            return ae::Keyboard::Num1;
            case XK_2:            return ae::Keyboard::Num2;
            case XK_3:            return ae::Keyboard::Num3;
            case XK_4:            return ae::Keyboard::Num4;
            case XK_5:            return ae::Keyboard::Num5;
            case XK_6:            return ae::Keyboard::Num6;
            case XK_7:            return ae::Keyboard::Num7;
            case XK_8:            return ae::Keyboard::Num8;
            case XK_9:            return ae::Keyboard::Num9;
        }

        return ae::Keyboard::Unknown;
    }
}


namespace ae
{
namespace priv
{
////////////////////////////////////////////////////////////
WindowImplX11::WindowImplX11(WindowHandle handle) :
m_window         (0),
m_screen         (0),
m_inputMethod    (NULL),
m_inputContext   (NULL),
m_isExternal     (true),
m_oldVideoMode   (0),
m_hiddenCursor   (0),
m_keyRepeat      (true),
m_previousSize   (-1, -1),
m_useSizeHints   (false),
m_fullscreen     (false),
m_cursorGrabbed  (false),
m_windowMapped   (false),
m_iconPixmap     (0),
m_iconMaskPixmap (0),
m_lastInputTime  (0)
{
    // Open a connection with the X server
    m_display = OpenDisplay();

    // Make sure to check for EWMH support before we do anything
    ewmhSupported();

    m_screen = DefaultScreen(m_display);

    // Save the window handle
    m_window = handle;

    if (m_window)
    {
        // Make sure the window is listening to all the required events
        XSetWindowAttributes attributes;
        attributes.event_mask = eventMask;

        XChangeWindowAttributes(m_display, m_window, CWEventMask, &attributes);

        // Set the WM protocols
        setProtocols();

        // Do some common initializations
        initialize();
    }
}


////////////////////////////////////////////////////////////
WindowImplX11::WindowImplX11(VideoMode mode, const String& title, unsigned long style, const ContextSettings& settings) :
m_window         (0),
m_screen         (0),
m_inputMethod    (NULL),
m_inputContext   (NULL),
m_isExternal     (false),
m_oldVideoMode   (0),
m_hiddenCursor   (0),
m_keyRepeat      (true),
m_previousSize   (-1, -1),
m_useSizeHints   (false),
m_fullscreen     ((style & Style::Fullscreen) != 0),
m_cursorGrabbed  (m_fullscreen),
m_windowMapped   (false),
m_iconPixmap     (0),
m_iconMaskPixmap (0),
m_lastInputTime  (0)
{
    // Open a connection with the X server
    m_display = OpenDisplay();

    // Make sure to check for EWMH support before we do anything
    ewmhSupported();

    m_screen = DefaultScreen(m_display);

    // Compute position and size
    int left = m_fullscreen ? 0 : (DisplayWidth(m_display, m_screen)  - mode.width) / 2;
    int top = m_fullscreen ? 0 : (DisplayHeight(m_display, m_screen) - mode.height) / 2;
    int width  = mode.width;
    int height = mode.height;

    // Choose the visual according to the context settings
    XVisualInfo visualInfo = ContextType::selectBestVisual(m_display, mode.bitsPerPixel, settings);

    // Define the window attributes
    XSetWindowAttributes attributes;
    attributes.colormap = XCreateColormap(m_display, DefaultRootWindow(m_display), visualInfo.visual, AllocNone);
    attributes.event_mask = eventMask;
    attributes.override_redirect = (m_fullscreen && !ewmhSupported()) ? True : False;

    m_window = XCreateWindow(m_display,
                             DefaultRootWindow(m_display),
                             left, top,
                             width, height,
                             0,
                             visualInfo.depth,
                             InputOutput,
                             visualInfo.visual,
                             CWEventMask | CWOverrideRedirect | CWColormap,
                             &attributes);

    if (!m_window)
    {
        err() << "Failed to create window" << std::endl;
        return;
    }

    // Set the WM protocols
    setProtocols();

    // Set the WM initial state to the normal state
    XWMHints* hints = XAllocWMHints();
    hints->flags         = StateHint;
    hints->initial_state = NormalState;
    XSetWMHints(m_display, m_window, hints);
    XFree(hints);

    // If not in fullscreen, set the window's style (tell the window manager to
    // change our window's decorations and functions according to the requested style)
    if (!m_fullscreen)
    {
        Atom WMHintsAtom = getAtom("_MOTIF_WM_HINTS", false);
        if (WMHintsAtom)
        {
            static const unsigned long MWM_HINTS_FUNCTIONS   = 1 << 0;
            static const unsigned long MWM_HINTS_DECORATIONS = 1 << 1;

            //static const unsigned long MWM_DECOR_ALL         = 1 << 0;
            static const unsigned long MWM_DECOR_BORDER      = 1 << 1;
            static const unsigned long MWM_DECOR_RESIZEH     = 1 << 2;
            static const unsigned long MWM_DECOR_TITLE       = 1 << 3;
            static const unsigned long MWM_DECOR_MENU        = 1 << 4;
            static const unsigned long MWM_DECOR_MINIMIZE    = 1 << 5;
            static const unsigned long MWM_DECOR_MAXIMIZE    = 1 << 6;

            //static const unsigned long MWM_FUNC_ALL          = 1 << 0;
            static const unsigned long MWM_FUNC_RESIZE       = 1 << 1;
            static const unsigned long MWM_FUNC_MOVE         = 1 << 2;
            static const unsigned long MWM_FUNC_MINIMIZE     = 1 << 3;
            static const unsigned long MWM_FUNC_MAXIMIZE     = 1 << 4;
            static const unsigned long MWM_FUNC_CLOSE        = 1 << 5;

            struct WMHints
            {
                unsigned long flags;
                unsigned long functions;
                unsigned long decorations;
                long          inputMode;
                unsigned long state;
            };

            WMHints hints;
            std::memset(&hints, 0, sizeof(hints));
            hints.flags       = MWM_HINTS_FUNCTIONS | MWM_HINTS_DECORATIONS;
            hints.decorations = 0;
            hints.functions   = 0;

            if (style & Style::Titlebar)
            {
                hints.decorations |= MWM_DECOR_BORDER | MWM_DECOR_TITLE | MWM_DECOR_MINIMIZE | MWM_DECOR_MENU;
                hints.functions   |= MWM_FUNC_MOVE | MWM_FUNC_MINIMIZE;
            }
            if (style & Style::Resize)
            {
                hints.decorations |= MWM_DECOR_MAXIMIZE | MWM_DECOR_RESIZEH;
                hints.functions   |= MWM_FUNC_MAXIMIZE | MWM_FUNC_RESIZE;
            }
            if (style & Style::Close)
            {
                hints.decorations |= 0;
                hints.functions   |= MWM_FUNC_CLOSE;
            }

            XChangeProperty(m_display,
                            m_window,
                            WMHintsAtom,
                            WMHintsAtom,
                            32,
                            PropModeReplace,
                            reinterpret_cast<const unsigned char*>(&hints),
                            5);
        }
    }

    // This is a hack to force some windows managers to disable resizing
    if (!(style & Style::Resize))
    {
        m_useSizeHints = true;
        XSizeHints* sizeHints = XAllocSizeHints();
        sizeHints->flags = PMinSize | PMaxSize;
        sizeHints->min_width = sizeHints->max_width = width;
        sizeHints->min_height = sizeHints->max_height = height;
        XSetWMNormalHints(m_display, m_window, sizeHints);
        XFree(sizeHints);
    }

    // Set the window's WM class (this can be used by window managers)
    XClassHint* hint = XAllocClassHint();

    // The instance name should be something unique to this invocation
    // of the application but is rarely if ever used these days.
    // For simplicity, we retrieve it via the base executable name.
    std::string executableName = findExecutableName();
    std::vector<char> windowInstance(executableName.size() + 1, 0);
    std::copy(executableName.begin(), executableName.end(), windowInstance.begin());
    hint->res_name = &windowInstance[0];

    // The class name identifies a class of windows that
    // "are of the same type". We simply use the initial window name as
    // the class name.
    std::string ansiTitle = title.toAnsiString();
    std::vector<char> windowClass(ansiTitle.size() + 1, 0);
    std::copy(ansiTitle.begin(), ansiTitle.end(), windowClass.begin());
    hint->res_class = &windowClass[0];

    XSetClassHint(m_display, m_window, hint);

    XFree(hint);

    // Set the window's name
    setTitle(title);

    // Do some common initializations
    initialize();

    // Set fullscreen video mode and switch to fullscreen if necessary
    if (m_fullscreen)
    {
        setPosition(Vector2i(0, 0));
        setVideoMode(mode);
        switchToFullscreen();
    }
}


////////////////////////////////////////////////////////////
WindowImplX11::~WindowImplX11()
{
    // Cleanup graphical resources
    cleanup();

    // Destroy icon pixmap
    if(m_iconPixmap)
        XFreePixmap(m_display, m_iconPixmap);

    // Destroy icon mask pixmap
    if(m_iconMaskPixmap)
        XFreePixmap(m_display, m_iconMaskPixmap);

    // Destroy the cursor
    if (m_hiddenCursor)
        XFreeCursor(m_display, m_hiddenCursor);

    // Destroy the input context
    if (m_inputContext)
        XDestroyIC(m_inputContext);

    // Destroy the window
    if (m_window && !m_isExternal)
    {
        XDestroyWindow(m_display, m_window);
        XFlush(m_display);
    }

    // Close the input method
    if (m_inputMethod)
        XCloseIM(m_inputMethod);

    // Close the connection with the X server
    CloseDisplay(m_display);

    // Remove this window from the global list of windows (required for focus request)
    Lock lock(allWindowsMutex);
    allWindows.erase(std::find(allWindows.begin(), allWindows.end(), this));
}


////////////////////////////////////////////////////////////
WindowHandle WindowImplX11::getSystemHandle() const
{
    return m_window;
}


////////////////////////////////////////////////////////////
void WindowImplX11::processEvents()
{
    XEvent event;
    while (XCheckIfEvent(m_display, &event, &checkEvent, reinterpret_cast<XPointer>(m_window)))
    {
        processEvent(event);
    }
}


////////////////////////////////////////////////////////////
Vector2i WindowImplX11::getPosition() const
{
    ::Window root, child;
    int localX, localY, x, y;
    unsigned int width, height, border, depth;

    XGetGeometry(m_display, m_window, &root, &localX, &localY, &width, &height, &border, &depth);
    XTranslateCoordinates(m_display, m_window, root, localX, localY, &x, &y, &child);

    return Vector2i(x, y);
}


////////////////////////////////////////////////////////////
void WindowImplX11::setPosition(const Vector2i& position)
{
    XMoveWindow(m_display, m_window, position.x, position.y);
    XFlush(m_display);
}


////////////////////////////////////////////////////////////
Vector2u WindowImplX11::getSize() const
{
    XWindowAttributes attributes;
    XGetWindowAttributes(m_display, m_window, &attributes);
    return Vector2u(attributes.width, attributes.height);
}


////////////////////////////////////////////////////////////
void WindowImplX11::setSize(const Vector2u& size)
{
    // If resizing is disable for the window we have to update the size hints (required by some window managers).
    if (m_useSizeHints)
    {
        XSizeHints* sizeHints = XAllocSizeHints();
        sizeHints->flags = PMinSize | PMaxSize;
        sizeHints->min_width = sizeHints->max_width = size.x;
        sizeHints->min_height = sizeHints->max_height = size.y;
        XSetWMNormalHints(m_display, m_window, sizeHints);
        XFree(sizeHints);
    }

    XResizeWindow(m_display, m_window, size.x, size.y);
    XFlush(m_display);
}


////////////////////////////////////////////////////////////
void WindowImplX11::setTitle(const String& title)
{
    // Bare X11 has no Unicode window title support.
    // There is however an option to tell the window manager your Unicode title via hints.

    // Convert to UTF-8 encoding.
    std::basic_string<Uint8> utf8Title;
    Utf32::toUtf8(title.begin(), title.end(), std::back_inserter(utf8Title));

    Atom useUtf8 = getAtom("UTF8_STRING", false);

    // Set the _NET_WM_NAME atom, which specifies a UTF-8 encoded window title.
    Atom wmName = getAtom("_NET_WM_NAME", false);
    XChangeProperty(m_display, m_window, wmName, useUtf8, 8,
                    PropModeReplace, utf8Title.c_str(), utf8Title.size());

    // Set the _NET_WM_ICON_NAME atom, which specifies a UTF-8 encoded window title.
    Atom wmIconName = getAtom("_NET_WM_ICON_NAME", false);
    XChangeProperty(m_display, m_window, wmIconName, useUtf8, 8,
                    PropModeReplace, utf8Title.c_str(), utf8Title.size());

    // Set the non-Unicode title as a fallback for window managers who don't support _NET_WM_NAME.
    #ifdef X_HAVE_UTF8_STRING
    Xutf8SetWMProperties(m_display,
                         m_window,
                         title.toAnsiString().c_str(),
                         title.toAnsiString().c_str(),
                         NULL,
                         0,
                         NULL,
                         NULL,
                         NULL);
    #else
    XmbSetWMProperties(m_display,
                       m_window,
                       title.toAnsiString().c_str(),
                       title.toAnsiString().c_str(),
                       NULL,
                       0,
                       NULL,
                       NULL,
                       NULL);
    #endif
}


////////////////////////////////////////////////////////////
void WindowImplX11::setIcon(unsigned int width, unsigned int height, const Uint8* pixels)
{
    // X11 wants BGRA pixels: swap red and blue channels
    // Note: this memory will be freed by XDestroyImage
    Uint8* iconPixels = static_cast<Uint8*>(std::malloc(width * height * 4));
    for (std::size_t i = 0; i < width * height; ++i)
    {
        iconPixels[i * 4 + 0] = pixels[i * 4 + 2];
        iconPixels[i * 4 + 1] = pixels[i * 4 + 1];
        iconPixels[i * 4 + 2] = pixels[i * 4 + 0];
        iconPixels[i * 4 + 3] = pixels[i * 4 + 3];
    }

    // Create the icon pixmap
    Visual*      defVisual = DefaultVisual(m_display, m_screen);
    unsigned int defDepth  = DefaultDepth(m_display, m_screen);
    XImage* iconImage = XCreateImage(m_display, defVisual, defDepth, ZPixmap, 0, (char*)iconPixels, width, height, 32, 0);
    if (!iconImage)
    {
        err() << "Failed to set the window's icon" << std::endl;
        return;
    }

    if(m_iconPixmap)
        XFreePixmap(m_display, m_iconPixmap);

    if(m_iconMaskPixmap)
        XFreePixmap(m_display, m_iconMaskPixmap);

    m_iconPixmap = XCreatePixmap(m_display, RootWindow(m_display, m_screen), width, height, defDepth);
    XGCValues values;
    GC iconGC = XCreateGC(m_display, m_iconPixmap, 0, &values);
    XPutImage(m_display, m_iconPixmap, iconGC, iconImage, 0, 0, 0, 0, width, height);
    XFreeGC(m_display, iconGC);
    XDestroyImage(iconImage);

    // Create the mask pixmap (must have 1 bit depth)
    std::size_t pitch = (width + 7) / 8;
    std::vector<Uint8> maskPixels(pitch * height, 0);
    for (std::size_t j = 0; j < height; ++j)
    {
        for (std::size_t i = 0; i < pitch; ++i)
        {
            for (std::size_t k = 0; k < 8; ++k)
            {
                if (i * 8 + k < width)
                {
                    Uint8 opacity = (pixels[(i * 8 + k + j * width) * 4 + 3] > 0) ? 1 : 0;
                    maskPixels[i + j * pitch] |= (opacity << k);
                }
            }
        }
    }
    m_iconMaskPixmap = XCreatePixmapFromBitmapData(m_display, m_window, (char*)&maskPixels[0], width, height, 1, 0, 1);

    // Send our new icon to the window through the WMHints
    XWMHints* hints = XAllocWMHints();
    hints->flags       = IconPixmapHint | IconMaskHint;
    hints->icon_pixmap = m_iconPixmap;
    hints->icon_mask   = m_iconMaskPixmap;
    XSetWMHints(m_display, m_window, hints);
    XFree(hints);

    // ICCCM wants BGRA pixels: swap red and blue channels
    // ICCCM also wants the first 2 unsigned 32-bit values to be width and height
    std::vector<unsigned long> icccmIconPixels(2 + width * height, 0);
    unsigned long* ptr = &icccmIconPixels[0];

    *ptr++ = width;
    *ptr++ = height;

    for (std::size_t i = 0; i < width * height; ++i)
    {
        *ptr++ = (pixels[i * 4 + 2] << 0 ) |
                 (pixels[i * 4 + 1] << 8 ) |
                 (pixels[i * 4 + 0] << 16) |
                 (pixels[i * 4 + 3] << 24);
    }

    Atom netWmIcon = getAtom("_NET_WM_ICON");

    XChangeProperty(m_display,
                    m_window,
                    netWmIcon,
                    XA_CARDINAL,
                    32,
                    PropModeReplace,
                    reinterpret_cast<const unsigned char*>(&icccmIconPixels[0]),
                    2 + width * height);

    XFlush(m_display);
}


////////////////////////////////////////////////////////////
void WindowImplX11::setVisible(bool visible)
{
    if (visible)
    {
        XMapWindow(m_display, m_window);

        XFlush(m_display);

        // Before continuing, make sure the WM has
        // internally marked the window as viewable
        while (!m_windowMapped && !m_isExternal)
            processEvents();
    }
    else
    {
        XUnmapWindow(m_display, m_window);

        XFlush(m_display);

        // Before continuing, make sure the WM has
        // internally marked the window as unviewable
        while (m_windowMapped && !m_isExternal)
            processEvents();
    }
}


////////////////////////////////////////////////////////////
void WindowImplX11::setMouseCursorVisible(bool visible)
{
    XDefineCursor(m_display, m_window, visible ? None : m_hiddenCursor);
    XFlush(m_display);
}


////////////////////////////////////////////////////////////
void WindowImplX11::setMouseCursorGrabbed(bool grabbed)
{
    // This has no effect in fullscreen mode
    if (m_fullscreen || (m_cursorGrabbed == grabbed))
        return;

    if (grabbed)
    {
        // Try multiple times to grab the cursor
        for (unsigned int trial = 0; trial < maxTrialsCount; ++trial)
        {
            int result = XGrabPointer(m_display, m_window, True, None, GrabModeAsync, GrabModeAsync, m_window, None, CurrentTime);

            if (result == GrabSuccess)
            {
                m_cursorGrabbed = true;
                break;
            }

            // The cursor grab failed, trying again after a small sleep
            ae::sleep(ae::milliseconds(50));
        }

        if (!m_cursorGrabbed)
            err() << "Failed to grab mouse cursor" << std::endl;
    }
    else
    {
        XUngrabPointer(m_display, CurrentTime);
    }
}


////////////////////////////////////////////////////////////
void WindowImplX11::setKeyRepeatEnabled(bool enabled)
{
    m_keyRepeat = enabled;
}


////////////////////////////////////////////////////////////
void WindowImplX11::requestFocus()
{
    // Focus is only stolen among SFML windows, not between applications
    // Check the global list of windows to find out whether an SFML window has the focus
    // Note: can't handle console and other non-SFML windows belonging to the application.
    bool sfmlWindowFocused = false;

    {
        Lock lock(allWindowsMutex);
        for (std::vector<WindowImplX11*>::iterator itr = allWindows.begin(); itr != allWindows.end(); ++itr)
        {
            if ((*itr)->hasFocus())
            {
                sfmlWindowFocused = true;
                break;
            }
        }
    }

    // Check if window is viewable (not on other desktop, ...)
    // TODO: Check also if minimized
    XWindowAttributes attributes;
    if (XGetWindowAttributes(m_display, m_window, &attributes) == 0)
    {
        ae::err() << "Failed to check if window is viewable while requesting focus" << std::endl;
        return; // error getting attribute
    }

    bool windowViewable = (attributes.map_state == IsViewable);

    if (sfmlWindowFocused && windowViewable)
    {
        // Another SFML window of this application has the focus and the current window is viewable:
        // steal focus (i.e. bring window to the front and give it input focus)
        grabFocus();
    }
    else
    {
        // Otherwise: display urgency hint (flashing application logo)
        // Ensure WM hints exist, allocate if necessary
        XWMHints* hints = XGetWMHints(m_display, m_window);
        if (hints == NULL)
            hints = XAllocWMHints();

        // Add urgency (notification) flag to hints
        hints->flags |= XUrgencyHint;
        XSetWMHints(m_display, m_window, hints);
        XFree(hints);
    }
}


////////////////////////////////////////////////////////////
bool WindowImplX11::hasFocus() const
{
    ::Window focusedWindow = 0;
    int revertToReturn = 0;
    XGetInputFocus(m_display, &focusedWindow, &revertToReturn);

    return (m_window == focusedWindow);
}


////////////////////////////////////////////////////////////
void WindowImplX11::grabFocus()
{
    Atom netActiveWindow = None;

    if (ewmhSupported())
        netActiveWindow = getAtom("_NET_ACTIVE_WINDOW");

    // Only try to grab focus if the window is mapped
    XWindowAttributes attr;

    XGetWindowAttributes(m_display, m_window, &attr);

    if (attr.map_state == IsUnmapped)
        return;

    if (netActiveWindow)
    {
        XEvent event;
        std::memset(&event, 0, sizeof(event));

        event.type = ClientMessage;
        event.xclient.window = m_window;
        event.xclient.format = 32;
        event.xclient.message_type = netActiveWindow;
        event.xclient.data.l[0] = 1; // Normal application
        event.xclient.data.l[1] = m_lastInputTime;
        event.xclient.data.l[2] = 0; // We don't know the currently active window

        int result = XSendEvent(m_display,
                                DefaultRootWindow(m_display),
                                False,
                                SubstructureNotifyMask | SubstructureRedirectMask,
                                &event);

        XFlush(m_display);

        if (!result)
            err() << "Setting fullscreen failed, could not send \"_NET_ACTIVE_WINDOW\" event" << std::endl;
    }
    else
    {
        XRaiseWindow(m_display, m_window);
        XSetInputFocus(m_display, m_window, RevertToPointerRoot, CurrentTime);
        XFlush(m_display);
    }
}


////////////////////////////////////////////////////////////
void WindowImplX11::setVideoMode(const VideoMode& mode)
{
    // Skip mode switching if the new mode is equal to the desktop mode
    if (mode == VideoMode::getDesktopMode())
        return;

    // Check if the XRandR extension is present
    int version;
    if (!XQueryExtension(m_display, "RANDR", &version, &version, &version))
    {
        // XRandR extension is not supported: we cannot use fullscreen mode
        err() << "Fullscreen is not supported, switching to window mode" << std::endl;
        return;
    }

    // Get the current configuration
    XRRScreenConfiguration* config = XRRGetScreenInfo(m_display, RootWindow(m_display, m_screen));

    if (!config)
    {
        // Failed to get the screen configuration
        err() << "Failed to get the current screen configuration for fullscreen mode, switching to window mode" << std::endl;
        return;
    }

    // Save the current video mode before we switch to fullscreen
    Rotation currentRotation;
    m_oldVideoMode = XRRConfigCurrentConfiguration(config, &currentRotation);

    // Get the available screen sizes
    int nbSizes;
    XRRScreenSize* sizes = XRRConfigSizes(config, &nbSizes);

    // Search for a matching size
    for (int i = 0; (sizes && i < nbSizes); ++i)
    {
        XRRConfigRotations(config, &currentRotation);

        if (currentRotation == RR_Rotate_90 || currentRotation == RR_Rotate_270)
            std::swap(sizes[i].height, sizes[i].width);

        if ((sizes[i].width  == static_cast<int>(mode.width)) && (sizes[i].height == static_cast<int>(mode.height)))
        {
            // Switch to fullscreen mode
            XRRSetScreenConfig(m_display, config, RootWindow(m_display, m_screen), i, currentRotation, CurrentTime);

            // Set "this" as the current fullscreen window
            fullscreenWindow = this;
            break;
        }
    }

    // Free the configuration instance
    XRRFreeScreenConfigInfo(config);
}


////////////////////////////////////////////////////////////
void WindowImplX11::resetVideoMode()
{
    if (fullscreenWindow == this)
    {
        // Get current screen info
        XRRScreenConfiguration* config = XRRGetScreenInfo(m_display, RootWindow(m_display, m_screen));
        if (config)
        {
            // Get the current rotation
            Rotation currentRotation;
            XRRConfigCurrentConfiguration(config, &currentRotation);

            // Reset the video mode
            XRRSetScreenConfig(m_display, config, RootWindow(m_display, m_screen), m_oldVideoMode, currentRotation, CurrentTime);

            // Free the configuration instance
            XRRFreeScreenConfigInfo(config);
        }

        // Reset the fullscreen window
        fullscreenWindow = NULL;
    }
}


////////////////////////////////////////////////////////////
void WindowImplX11::switchToFullscreen()
{
    grabFocus();

    if (ewmhSupported())
    {
        Atom netWmBypassCompositor = getAtom("_NET_WM_BYPASS_COMPOSITOR");

        if (netWmBypassCompositor)
        {
            static const unsigned long bypassCompositor = 1;

            XChangeProperty(m_display,
                            m_window,
                            netWmBypassCompositor,
                            XA_CARDINAL,
                            32,
                            PropModeReplace,
                            reinterpret_cast<const unsigned char*>(&bypassCompositor),
                            1);
        }

        Atom netWmState = getAtom("_NET_WM_STATE", true);
        Atom netWmStateFullscreen = getAtom("_NET_WM_STATE_FULLSCREEN", true);

        if (!netWmState || !netWmStateFullscreen)
        {
            err() << "Setting fullscreen failed. Could not get required atoms" << std::endl;
            return;
        }

        XEvent event;
        std::memset(&event, 0, sizeof(event));

        event.type = ClientMessage;
        event.xclient.window = m_window;
        event.xclient.format = 32;
        event.xclient.message_type = netWmState;
        event.xclient.data.l[0] = 1; // _NET_WM_STATE_ADD
        event.xclient.data.l[1] = netWmStateFullscreen;
        event.xclient.data.l[2] = 0; // No second property
        event.xclient.data.l[3] = 1; // Normal window

        int result = XSendEvent(m_display,
                                DefaultRootWindow(m_display),
                                False,
                                SubstructureNotifyMask | SubstructureRedirectMask,
                                &event);

        if (!result)
            err() << "Setting fullscreen failed, could not send \"_NET_WM_STATE\" event" << std::endl;
    }
}


////////////////////////////////////////////////////////////
void WindowImplX11::setProtocols()
{
    Atom wmProtocols = getAtom("WM_PROTOCOLS");
    Atom wmDeleteWindow = getAtom("WM_DELETE_WINDOW");

    if (!wmProtocols)
    {
        err() << "Failed to request WM_PROTOCOLS atom." << std::endl;
        return;
    }

    std::vector<Atom> atoms;

    if (wmDeleteWindow)
    {
        atoms.push_back(wmDeleteWindow);
    }
    else
    {
        err() << "Failed to request WM_DELETE_WINDOW atom." << std::endl;
    }

    Atom netWmPing = None;
    Atom netWmPid = None;

    if (ewmhSupported())
    {
        netWmPing = getAtom("_NET_WM_PING", true);
        netWmPid = getAtom("_NET_WM_PID", true);
    }

    if (netWmPing && netWmPid)
    {
        const long pid = getpid();

        XChangeProperty(m_display,
                        m_window,
                        netWmPid,
                        XA_CARDINAL,
                        32,
                        PropModeReplace,
                        reinterpret_cast<const unsigned char*>(&pid),
                        1);

        atoms.push_back(netWmPing);
    }

    if (!atoms.empty())
    {
        XChangeProperty(m_display,
                        m_window,
                        wmProtocols,
                        XA_ATOM,
                        32,
                        PropModeReplace,
                        reinterpret_cast<const unsigned char*>(&atoms[0]),
                        atoms.size());
    }
    else
    {
        err() << "Didn't set any window protocols" << std::endl;
    }
}


////////////////////////////////////////////////////////////
void WindowImplX11::initialize()
{
    // Create the input context
    m_inputMethod = XOpenIM(m_display, NULL, NULL, NULL);

    if (m_inputMethod)
    {
        m_inputContext = XCreateIC(m_inputMethod,
                                   XNClientWindow,
                                   m_window,
                                   XNFocusWindow,
                                   m_window,
                                   XNInputStyle,
                                   XIMPreeditNothing | XIMStatusNothing,
                                   reinterpret_cast<void*>(NULL));
    }
    else
    {
        m_inputContext = NULL;
    }

    if (!m_inputContext)
        err() << "Failed to create input context for window -- TextEntered event won't be able to return unicode" << std::endl;

    Atom wmWindowType = getAtom("_NET_WM_WINDOW_TYPE", false);
    Atom wmWindowTypeNormal = getAtom("_NET_WM_WINDOW_TYPE_NORMAL", false);

    if (wmWindowType && wmWindowTypeNormal)
    {
        XChangeProperty(m_display,
                        m_window,
                        wmWindowType,
                        XA_ATOM,
                        32,
                        PropModeReplace,
                        reinterpret_cast<const unsigned char*>(&wmWindowTypeNormal),
                        1);
    }

    // Show the window
    setVisible(true);

    // Raise the window and grab input focus
    grabFocus();

    // Create the hidden cursor
    createHiddenCursor();

    // Flush the commands queue
    XFlush(m_display);

    // Add this window to the global list of windows (required for focus request)
    Lock lock(allWindowsMutex);
    allWindows.push_back(this);
}


////////////////////////////////////////////////////////////
void WindowImplX11::updateLastInputTime(::Time time)
{
    if (time && (time != m_lastInputTime))
    {
        Atom netWmUserTime = getAtom("_NET_WM_USER_TIME", true);

        if(netWmUserTime)
        {
            XChangeProperty(m_display,
                            m_window,
                            netWmUserTime,
                            XA_CARDINAL,
                            32,
                            PropModeReplace,
                            reinterpret_cast<const unsigned char*>(&time),
                            1);
        }

        m_lastInputTime = time;
    }
}


////////////////////////////////////////////////////////////
void WindowImplX11::createHiddenCursor()
{
    // Create the cursor's pixmap (1x1 pixels)
    Pixmap cursorPixmap = XCreatePixmap(m_display, m_window, 1, 1, 1);
    GC graphicsContext = XCreateGC(m_display, cursorPixmap, 0, NULL);
    XDrawPoint(m_display, cursorPixmap, graphicsContext, 0, 0);
    XFreeGC(m_display, graphicsContext);

    // Create the cursor, using the pixmap as both the shape and the mask of the cursor
    XColor color;
    color.flags = DoRed | DoGreen | DoBlue;
    color.red = color.blue = color.green = 0;
    m_hiddenCursor = XCreatePixmapCursor(m_display, cursorPixmap, cursorPixmap, &color, &color, 0, 0);

    // We don't need the pixmap any longer, free it
    XFreePixmap(m_display, cursorPixmap);
}


////////////////////////////////////////////////////////////
void WindowImplX11::cleanup()
{
    // Restore the previous video mode (in case we were running in fullscreen)
    resetVideoMode();

    // Unhide the mouse cursor (in case it was hidden)
    setMouseCursorVisible(true);
}


////////////////////////////////////////////////////////////
bool WindowImplX11::processEvent(XEvent& windowEvent)
{
    // This function implements a workaround to properly discard
    // repeated key events when necessary. The problem is that the
    // system's key events policy doesn't match SFML's one: X server will generate
    // both repeated KeyPress and KeyRelease events when maintaining a key down, while
    // SFML only wants repeated KeyPress events. Thus, we have to:
    // - Discard duplicated KeyRelease events when KeyRepeatEnabled is true
    // - Discard both duplicated KeyPress and KeyRelease events when KeyRepeatEnabled is false

    // Detect repeated key events
    // (code shamelessly taken from SDL)
    if (windowEvent.type == KeyRelease)
    {
        // Check if there's a matching KeyPress event in the queue
        XEvent nextEvent;
        if (XPending(m_display))
        {
            // Grab it but don't remove it from the queue, it still needs to be processed :)
            XPeekEvent(m_display, &nextEvent);
            if (nextEvent.type == KeyPress)
            {
                // Check if it is a duplicated event (same timestamp as the KeyRelease event)
                if ((nextEvent.xkey.keycode == windowEvent.xkey.keycode) &&
                    (nextEvent.xkey.time - windowEvent.xkey.time < 2))
                {
                    // If we don't want repeated events, remove the next KeyPress from the queue
                    if (!m_keyRepeat)
                        XNextEvent(m_display, &nextEvent);

                    // This KeyRelease is a repeated event and we don't want it
                    return false;
                }
            }
        }
    }

    // Convert the X11 event to a ae::Event
    switch (windowEvent.type)
    {
        // Destroy event
        case DestroyNotify:
        {
            // The window is about to be destroyed: we must cleanup resources
            cleanup();
            break;
        }

        // Gain focus event
        case FocusIn:
        {
            // Update the input context
            if (m_inputContext)
                XSetICFocus(m_inputContext);

            // Grab cursor
            if (m_cursorGrabbed)
            {
                // Try multiple times to grab the cursor
                for (unsigned int trial = 0; trial < maxTrialsCount; ++trial)
                {
                    int result = XGrabPointer(m_display, m_window, True, None, GrabModeAsync, GrabModeAsync, m_window, None, CurrentTime);

                    if (result == GrabSuccess)
                    {
                        m_cursorGrabbed = true;
                        break;
                    }

                    // The cursor grab failed, trying again after a small sleep
                    ae::sleep(ae::milliseconds(50));
                }

                if (!m_cursorGrabbed)
                    err() << "Failed to grab mouse cursor" << std::endl;
            }

            Event event;
            event.type = Event::GainedFocus;
            pushEvent(event);

            // If the window has been previously marked urgent (notification) as a result of a focus request, undo that
            XWMHints* hints = XGetWMHints(m_display, m_window);
            if (hints != NULL)
            {
                // Remove urgency (notification) flag from hints
                hints->flags &= ~XUrgencyHint;
                XSetWMHints(m_display, m_window, hints);
                XFree(hints);
            }

            break;
        }

        // Lost focus event
        case FocusOut:
        {
            // Update the input context
            if (m_inputContext)
                XUnsetICFocus(m_inputContext);

            // Release cursor
            if (m_cursorGrabbed)
                XUngrabPointer(m_display, CurrentTime);

            Event event;
            event.type = Event::LostFocus;
            pushEvent(event);
            break;
        }

        // Resize event
        case ConfigureNotify:
        {
            // ConfigureNotify can be triggered for other reasons, check if the size has actually changed
            if ((windowEvent.xconfigure.width != m_previousSize.x) || (windowEvent.xconfigure.height != m_previousSize.y))
            {
                Event event;
                event.type        = Event::Resized;
                event.size.width  = windowEvent.xconfigure.width;
                event.size.height = windowEvent.xconfigure.height;
                pushEvent(event);

                m_previousSize.x = windowEvent.xconfigure.width;
                m_previousSize.y = windowEvent.xconfigure.height;
            }
            break;
        }

        // Close event
        case ClientMessage:
        {
            static Atom wmProtocols = getAtom("WM_PROTOCOLS");

            // Handle window manager protocol messages we support
            if (windowEvent.xclient.message_type == wmProtocols)
            {
                static Atom wmDeleteWindow = getAtom("WM_DELETE_WINDOW");
                static Atom netWmPing = ewmhSupported() ? getAtom("_NET_WM_PING", true) : None;

                if ((windowEvent.xclient.format == 32) && (windowEvent.xclient.data.l[0]) == static_cast<long>(wmDeleteWindow))
                {
                    // Handle the WM_DELETE_WINDOW message
                    Event event;
                    event.type = Event::Closed;
                    pushEvent(event);
                }
                else if (netWmPing && (windowEvent.xclient.format == 32) && (windowEvent.xclient.data.l[0]) == static_cast<long>(netWmPing))
                {
                    // Handle the _NET_WM_PING message, send pong back to WM to show that we are responsive
                    windowEvent.xclient.window = DefaultRootWindow(m_display);

                    XSendEvent(m_display, DefaultRootWindow(m_display), False, SubstructureNotifyMask | SubstructureRedirectMask, &windowEvent);
                }
            }
            break;
        }

        // Key down event
        case KeyPress:
        {
            Keyboard::Key key = Keyboard::Unknown;

            // Try each KeySym index (modifier group) until we get a match
            for (int i = 0; i < 4; ++i)
            {
                // Get the SFML keyboard code from the keysym of the key that has been pressed
                key = keysymToSF(XLookupKeysym(&windowEvent.xkey, i));

                if (key != Keyboard::Unknown)
                    break;
            }

            // Fill the event parameters
            // TODO: if modifiers are wrong, use XGetModifierMapping to retrieve the actual modifiers mapping
            Event event;
            event.type        = Event::KeyPressed;
            event.key.code    = key;
            event.key.alt     = windowEvent.xkey.state & Mod1Mask;
            event.key.control = windowEvent.xkey.state & ControlMask;
            event.key.shift   = windowEvent.xkey.state & ShiftMask;
            event.key.system  = windowEvent.xkey.state & Mod4Mask;
            pushEvent(event);

            // Generate a TextEntered event
            if (!XFilterEvent(&windowEvent, None))
            {
                #ifdef X_HAVE_UTF8_STRING
                if (m_inputContext)
                {
                    Status status;
                    Uint8  keyBuffer[16];

                    int length = Xutf8LookupString(
                        m_inputContext,
                        &windowEvent.xkey,
                        reinterpret_cast<char*>(keyBuffer),
                        sizeof(keyBuffer),
                        NULL,
                        &status
                    );

                    if (length > 0)
                    {
                        Uint32 unicode = 0;
                        Utf8::decode(keyBuffer, keyBuffer + length, unicode, 0);
                        if (unicode != 0)
                        {
                            Event textEvent;
                            textEvent.type         = Event::TextEntered;
                            textEvent.text.unicode = unicode;
                            pushEvent(textEvent);
                        }
                    }
                }
                else
                #endif
                {
                    static XComposeStatus status;
                    char keyBuffer[16];
                    if (XLookupString(&windowEvent.xkey, keyBuffer, sizeof(keyBuffer), NULL, &status))
                    {
                        Event textEvent;
                        textEvent.type         = Event::TextEntered;
                        textEvent.text.unicode = static_cast<Uint32>(keyBuffer[0]);
                        pushEvent(textEvent);
                    }
                }
            }

            updateLastInputTime(windowEvent.xkey.time);

            break;
        }

        // Key up event
        case KeyRelease:
        {
            Keyboard::Key key = Keyboard::Unknown;

            // Try each KeySym index (modifier group) until we get a match
            for (int i = 0; i < 4; ++i)
            {
                // Get the SFML keyboard code from the keysym of the key that has been released
                key = keysymToSF(XLookupKeysym(&windowEvent.xkey, i));

                if (key != Keyboard::Unknown)
                    break;
            }

            // Fill the event parameters
            Event event;
            event.type        = Event::KeyReleased;
            event.key.code    = key;
            event.key.alt     = windowEvent.xkey.state & Mod1Mask;
            event.key.control = windowEvent.xkey.state & ControlMask;
            event.key.shift   = windowEvent.xkey.state & ShiftMask;
            event.key.system  = windowEvent.xkey.state & Mod4Mask;
            pushEvent(event);

            break;
        }

        // Mouse button pressed
        case ButtonPress:
        {
            // XXX: Why button 8 and 9?
            // Because 4 and 5 are the vertical wheel and 6 and 7 are horizontal wheel ;)
            unsigned int button = windowEvent.xbutton.button;
            if ((button == Button1) ||
                (button == Button2) ||
                (button == Button3) ||
                (button == 8) ||
                (button == 9))
            {
                Event event;
                event.type          = Event::MouseButtonPressed;
                event.mouseButton.x = windowEvent.xbutton.x;
                event.mouseButton.y = windowEvent.xbutton.y;
                switch(button)
                {
                    case Button1: event.mouseButton.button = Mouse::Left;     break;
                    case Button2: event.mouseButton.button = Mouse::Middle;   break;
                    case Button3: event.mouseButton.button = Mouse::Right;    break;
                    case 8:       event.mouseButton.button = Mouse::XButton1; break;
                    case 9:       event.mouseButton.button = Mouse::XButton2; break;
                }
                pushEvent(event);
            }

            updateLastInputTime(windowEvent.xbutton.time);

            break;
        }

        // Mouse button released
        case ButtonRelease:
        {
            unsigned int button = windowEvent.xbutton.button;
            if ((button == Button1) ||
                (button == Button2) ||
                (button == Button3) ||
                (button == 8) ||
                (button == 9))
            {
                Event event;
                event.type          = Event::MouseButtonReleased;
                event.mouseButton.x = windowEvent.xbutton.x;
                event.mouseButton.y = windowEvent.xbutton.y;
                switch(button)
                {
                    case Button1: event.mouseButton.button = Mouse::Left;     break;
                    case Button2: event.mouseButton.button = Mouse::Middle;   break;
                    case Button3: event.mouseButton.button = Mouse::Right;    break;
                    case 8:       event.mouseButton.button = Mouse::XButton1; break;
                    case 9:       event.mouseButton.button = Mouse::XButton2; break;
                }
                pushEvent(event);
            }
            else if ((button == Button4) || (button == Button5))
            {
                Event event;

                event.type             = Event::MouseWheelMoved;
                event.mouseWheel.delta = (button == Button4) ? 1 : -1;
                event.mouseWheel.x     = windowEvent.xbutton.x;
                event.mouseWheel.y     = windowEvent.xbutton.y;
                pushEvent(event);

                event.type                   = Event::MouseWheelScrolled;
                event.mouseWheelScroll.wheel = Mouse::VerticalWheel;
                event.mouseWheelScroll.delta = (button == Button4) ? 1 : -1;
                event.mouseWheelScroll.x     = windowEvent.xbutton.x;
                event.mouseWheelScroll.y     = windowEvent.xbutton.y;
                pushEvent(event);
            }
            else if ((button == 6) || (button == 7))
            {
                Event event;
                event.type                   = Event::MouseWheelScrolled;
                event.mouseWheelScroll.wheel = Mouse::HorizontalWheel;
                event.mouseWheelScroll.delta = (button == 6) ? 1 : -1;
                event.mouseWheelScroll.x     = windowEvent.xbutton.x;
                event.mouseWheelScroll.y     = windowEvent.xbutton.y;
                pushEvent(event);
            }
            break;
        }

        // Mouse moved
        case MotionNotify:
        {
            Event event;
            event.type        = Event::MouseMoved;
            event.mouseMove.x = windowEvent.xmotion.x;
            event.mouseMove.y = windowEvent.xmotion.y;
            pushEvent(event);
            break;
        }

        // Mouse entered
        case EnterNotify:
        {
            if (windowEvent.xcrossing.mode == NotifyNormal)
            {
                Event event;
                event.type = Event::MouseEntered;
                pushEvent(event);
            }
            break;
        }

        // Mouse left
        case LeaveNotify:
        {
            if (windowEvent.xcrossing.mode == NotifyNormal)
            {
                Event event;
                event.type = Event::MouseLeft;
                pushEvent(event);
            }
            break;
        }

        // Window unmapped
        case UnmapNotify:
        {
            if (windowEvent.xunmap.window == m_window)
                m_windowMapped = false;

            break;
        }

        // Window visibility change
        case VisibilityNotify:
        {
            // We prefer using VisibilityNotify over MapNotify because
            // some window managers like awesome don't internally flag a
            // window as viewable even after it is mapped but before it
            // is visible leading to certain function calls failing with
            // an unviewable error if called before VisibilityNotify arrives

            // Empirical testing on most widely used window managers shows
            // that mapping a window will always lead to a VisibilityNotify
            // event that is not VisibilityFullyObscured
            if (windowEvent.xvisibility.window == m_window)
            {
                if (windowEvent.xvisibility.state != VisibilityFullyObscured)
                    m_windowMapped = true;
            }

            break;
        }

        // Window property change
        case PropertyNotify:
        {
            if (!m_lastInputTime)
                m_lastInputTime = windowEvent.xproperty.time;

            break;
        }
    }

    return true;
}

} // namespace priv

} // namespace ae
