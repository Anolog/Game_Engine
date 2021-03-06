#include "pch.h"

#include <Xinput.h>

GameCore* g_pGameCore = 0;

#define SCREEN_WIDTH 600
#define SCREEN_HEIGHT 600

// Initialize opengl window on windows, huge chunks taken from nehe
//    http://nehe.gamedev.net/tutorial/creating_an_opengl_window_%28win32%29/13001/

bool g_EscapeButtonWillQuit;
bool g_CloseProgramRequested;

int g_RequestedWidth;
int g_RequestedHeight;

int g_InitialWidth;
int g_InitialHeight;

HGLRC hRenderingContext = 0;
HDC hDeviceContext = 0;
HWND hWnd = 0;
HINSTANCE hInstance;

int g_WindowWidth = 0;
int g_WindowHeight = 0;
bool g_KeyStates[256];
bool g_OldKeyStates[256];
bool g_MouseButtonStates[3];
bool g_WindowIsActive = true;
bool g_FullscreenMode = true;

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

static bool h_moviemode = false;
static bool h_takescreenshot = false;

bool GetKey(int value)
{
    assert( value >= 0 && value < 256 );
    return g_KeyStates[value];
}

GLvoid ResizeGLScene(GLsizei width, GLsizei height)
{
    if( height <= 0 ) height = 1;
    if( width <= 0 ) width = 1;

    g_WindowWidth = width;
    g_WindowHeight = height;

    if( g_pGameCore )
        g_pGameCore->OnSurfaceChanged( width, height );
}

void SetWindowSize(int width, int height)
{
    int maxwidth = GetSystemMetrics( SM_CXFULLSCREEN );
    int maxheight = GetSystemMetrics( SM_CYFULLSCREEN );

    float aspect = (float)width / height;

    if( width > maxwidth )
    {
        width = maxwidth;
        height = (int)(maxwidth / aspect);
    }

    if( height > maxheight )
    {
        width = (int)(maxheight * aspect);
        height = maxheight;
    }

    DWORD dwStyle = GetWindowLongPtr( hWnd, GWL_STYLE ) ;
    DWORD dwExStyle = GetWindowLongPtr( hWnd, GWL_EXSTYLE ) ;
    HMENU menu = GetMenu( hWnd ) ;

    // calculate the full size of the window needed to match our client area of width/height
    RECT WindowRect = { 0, 0, width, height } ;
    AdjustWindowRectEx( &WindowRect, dwStyle, menu ? TRUE : FALSE, dwExStyle );

    int windowwidth = WindowRect.right - WindowRect.left;
    int windowheight = WindowRect.bottom - WindowRect.top;

    SetWindowPos( hWnd, 0, 0, 0, windowwidth, windowheight, SWP_NOZORDER | SWP_NOMOVE ) ;

    ResizeGLScene( width, height );

    if( g_pGameCore )
    {
        g_pGameCore->OnSurfaceChanged( width, height );
    }

}

void GetMouseCoordinates(int* mx, int* my)
{
    extern HWND hWnd;

    POINT p;
    if( GetCursorPos( &p ) )
    {
        if( ScreenToClient( hWnd, &p ) )
        {
            *mx = p.x;
            *my = p.y;
        }
    }
}

GLvoid KillGLWindow()
{
    if( g_FullscreenMode )
    {
        ChangeDisplaySettings( 0, 0 );
        ShowCursor( true );
    }

    if( hRenderingContext )
    {
        if( !wglMakeCurrent( 0, 0 ) )
        {
            MessageBox( 0, "Release Of Device Context And Rendering Context Failed.", "SHUTDOWN ERROR", MB_OK | MB_ICONINFORMATION );
        }

        if( !wglDeleteContext( hRenderingContext ) )
        {
            MessageBox( 0, "Release Rendering Context Failed.", "SHUTDOWN ERROR", MB_OK | MB_ICONINFORMATION );
        }

        hRenderingContext = 0;
    }

    if( hDeviceContext && !ReleaseDC( hWnd, hDeviceContext ) )
    {
        MessageBox( 0, "Release Device Context Failed.", "SHUTDOWN ERROR", MB_OK | MB_ICONINFORMATION );
        hDeviceContext = 0;
    }

    if( hWnd && !DestroyWindow( hWnd ) )
    {
        MessageBox( 0, "Could Not Release hWnd.", "SHUTDOWN ERROR", MB_OK | MB_ICONINFORMATION );
        hWnd = 0;
    }

    if( !UnregisterClass( "OpenGL", hInstance ) )
    {
        MessageBox( 0, "Could Not Unregister Class.", "SHUTDOWN ERROR", MB_OK | MB_ICONINFORMATION );
        hInstance = 0;
    }
}

bool CreateGLWindow(char* title, int width, int height, char colorbits, char zbits, char stencilbits, bool fullscreenflag)
{
    GLuint PixelFormat;

    WNDCLASS wc;
    DWORD dwExStyle;
    DWORD dwStyle;

    RECT WindowRect;
    WindowRect.left = (long)0;
    WindowRect.right = (long)width;
    WindowRect.top = (long)0;
    WindowRect.bottom = (long)height;

    g_FullscreenMode = fullscreenflag;

    hInstance = GetModuleHandle( 0 );               // Grab An Instance For Our Window
    wc.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;  // Redraw On Move, And Own DC For Window
    wc.lpfnWndProc = (WNDPROC)WndProc;              // WndProc Handles Messages
    wc.cbClsExtra = 0;                              // No Extra Window Data
    wc.cbWndExtra = 0;                              // No Extra Window Data
    wc.hInstance = hInstance;                       // Set The Instance
    wc.hIcon = LoadIcon( 0, IDI_WINLOGO );          // Load The Default Icon
    wc.hCursor = LoadCursor( 0, IDC_ARROW );        // Load The Arrow Pointer
    wc.hbrBackground = 0;                           // No Background Required For GL
    wc.lpszMenuName = 0;                            // We Don't Want A Menu
    wc.lpszClassName = "OpenGL";                   // Set The Class Name

    if( !RegisterClass( &wc ) )                     // Attempt To Register The Window Class
    {
        MessageBox( 0, "Failed To Register The Window Class.", "ERROR", MB_OK|MB_ICONEXCLAMATION );
        return false;
    }

    if( g_FullscreenMode )
    {
        DEVMODE dmScreenSettings;                                   // Device Mode
        memset( &dmScreenSettings, 0, sizeof( dmScreenSettings ) ); // Makes Sure Memory's Cleared
        dmScreenSettings.dmSize = sizeof( dmScreenSettings );       // Size Of The Devmode Structure
        dmScreenSettings.dmPelsWidth  = width;                      // Selected Screen Width
        dmScreenSettings.dmPelsHeight = height;                     // Selected Screen Height
        dmScreenSettings.dmBitsPerPel = colorbits;                  // Selected Bits Per Pixel
        dmScreenSettings.dmFields = DM_BITSPERPEL | DM_PELSWIDTH | DM_PELSHEIGHT;

        // Try To Set Selected Mode And Get Results.  NOTE: CDS_FULLSCREEN Gets Rid Of Start Bar.
        if( ChangeDisplaySettings( &dmScreenSettings, CDS_FULLSCREEN ) != DISP_CHANGE_SUCCESSFUL )
        {
            // If The Mode Fails, Offer Two Options.  Quit Or Run In A Window.
            if( MessageBox( 0, "The Requested Fullscreen Mode Is Not Supported By\nYour Video Card. Use Windowed Mode Instead?", "", MB_YESNO|MB_ICONEXCLAMATION ) == IDYES )
            {
                g_FullscreenMode = false;
            }
            else
            {
                MessageBox( 0, "Program Will Now Close.", "ERROR", MB_OK|MB_ICONSTOP );
                return false;
            }
        }
    }

    if( g_FullscreenMode )
    {
        dwExStyle = WS_EX_APPWINDOW;
        dwStyle = WS_POPUP;
        ShowCursor( false );
    }
    else
    {
        dwExStyle = WS_EX_APPWINDOW | WS_EX_WINDOWEDGE;
        dwStyle = WS_OVERLAPPEDWINDOW;
    }

    AdjustWindowRectEx( &WindowRect, dwStyle, false, dwExStyle );   // Adjust Window To True Requested Size

    if( !( hWnd = CreateWindowEx( dwExStyle,                            // Extended Style For The Window
        "OpenGL",                             // Class Name
        title,                                // Window Title
        WS_CLIPSIBLINGS | WS_CLIPCHILDREN |   // Required Window Style
        dwStyle,                              // Selected Window Style
        0, 0,                                 // Window Position
        WindowRect.right-WindowRect.left,     // Calculate Adjusted Window Width
        WindowRect.bottom-WindowRect.top,     // Calculate Adjusted Window Height
        0,                                    // No Parent Window
        0,                                    // No Menu
        hInstance,                            // Instance
        0)))                                  // Don't Pass Anything To WM_CREATE
    {
        KillGLWindow();
        MessageBox( 0, "Window Creation Error.", "ERROR", MB_OK|MB_ICONEXCLAMATION );
        return false;
    }

    PIXELFORMATDESCRIPTOR pfd =  // pfd Tells Windows How We Want Things To Be
    {
        sizeof(PIXELFORMATDESCRIPTOR),  // Size Of This Pixel Format Descriptor
        1,                              // Version Number
        PFD_DRAW_TO_WINDOW |            // Format Must Support Window
        PFD_SUPPORT_OPENGL |          // Format Must Support OpenGL
        PFD_DOUBLEBUFFER,             // Must Support Double Buffering
        PFD_TYPE_RGBA,                  // Request An RGBA Format
        (BYTE)colorbits,                // Select Our Color Depth
        0, 0, 0, 0, 0, 0,               // Color Bits Ignored
        0,                              // No Alpha Buffer
        0,                              // Shift Bit Ignored
        0,                              // No Accumulation Buffer
        0, 0, 0, 0,                     // Accumulation Bits Ignored
        (BYTE)zbits,                    // Bits for Z-Buffer (Depth Buffer)
        (BYTE)stencilbits,              // Stencil bits
        0,                              // No Auxiliary Buffer
        PFD_MAIN_PLANE,                 // Main Drawing Layer
        0,                              // Reserved
        0, 0, 0                         // Layer Masks Ignored
    };

    if( !( hDeviceContext = GetDC(hWnd) ) ) // Did We Get A Device Context?
    {
        KillGLWindow();
        MessageBox( 0, "Can't Create A GL Device Context.", "ERROR", MB_OK|MB_ICONEXCLAMATION );
        return 0;
    }

    if( !( PixelFormat = ChoosePixelFormat(hDeviceContext, &pfd) ) ) // Did Windows Find A Matching Pixel Format?
    {
        KillGLWindow();
        MessageBox( 0, "Can't Find A Suitable PixelFormat.", "ERROR", MB_OK|MB_ICONEXCLAMATION );
        return 0;
    }

    if( !SetPixelFormat( hDeviceContext, PixelFormat, &pfd) ) // Are We Able To Set The Pixel Format?
    {
        KillGLWindow();
        MessageBox( 0, "Can't Set The PixelFormat.", "ERROR", MB_OK|MB_ICONEXCLAMATION );
        return 0;
    }

    if( !( hRenderingContext = wglCreateContext(hDeviceContext) ) ) // Are We Able To Get A Rendering Context?
    {
        KillGLWindow();
        MessageBox( 0, "Can't Create A GL Rendering Context.", "ERROR", MB_OK|MB_ICONEXCLAMATION );
        return 0;
    }

    if( !wglMakeCurrent( hDeviceContext, hRenderingContext ) ) // Try To Activate The Rendering Context
    {
        KillGLWindow();
        MessageBox( 0, "Can't Activate The GL Rendering Context.", "ERROR", MB_OK|MB_ICONEXCLAMATION );
        return 0;
    }

    ShowWindow( hWnd, SW_SHOW );     // Show The Window
    SetForegroundWindow( hWnd );     // Slightly Higher Priority
    SetFocus( hWnd );                // Sets Keyboard Focus To The Window
    ResizeGLScene( width, height );  // Set Up Our Perspective GL Screen

    return true;
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    switch( uMsg )
    {
    case WM_ACTIVATE:
        {
            if( !HIWORD(wParam) )
            {
                g_WindowIsActive = true;
            }
            else
            {
                g_WindowIsActive = false;
            } 
        }
        return 0;

    case WM_SYSCOMMAND:
        {
            switch( wParam )
            {
            // don't let screensaver or monitor power save mode kick in.
            case SC_SCREENSAVE:
            case SC_MONITORPOWER:
                return 0;
            }
        }
        break;

    case WM_CLOSE:
        {
            PostQuitMessage(0);
        }
        return 0;

    case WM_KEYDOWN:
        {
            g_KeyStates[wParam] = true;

            if( wParam == VK_ESCAPE && g_EscapeButtonWillQuit )
                g_CloseProgramRequested = true;

            InputEvent* pEvent = new InputEvent( InputDeviceType_Keyboard, InputState_Pressed, wParam, vec2(0,0) );
            g_pGameCore->QueueEvent( pEvent );
        }
        return 0;

    case WM_KEYUP:
        {
            g_KeyStates[wParam] = false;

            InputEvent* pEvent = new InputEvent( InputDeviceType_Keyboard, InputState_Released, wParam, vec2(0,0) );
            g_pGameCore->QueueEvent( pEvent );
        }
        return 0;

    case WM_LBUTTONDOWN:
        {
            int xPos = GET_X_LPARAM( lParam ); 
            int yPos = GET_Y_LPARAM( lParam );

            InputEvent* pEvent = new InputEvent( InputDeviceType_Mouse, InputState_Pressed, 0, vec2((float)xPos, (float)yPos) );
            g_pGameCore->QueueEvent( pEvent );
        }
        return 0;

    case WM_LBUTTONUP:
        {
            int xPos = GET_X_LPARAM( lParam ); 
            int yPos = GET_Y_LPARAM( lParam );

            InputEvent* pEvent = new InputEvent( InputDeviceType_Mouse, InputState_Released, 0, vec2((float)xPos, (float)yPos) );
            g_pGameCore->QueueEvent( pEvent );
        }
        return 0;

    case WM_SIZE:
        {
            ResizeGLScene( LOWORD(lParam), HIWORD(lParam) );
        }
        return 0;
    }

    // Pass all unhandled messages to DefWindowProc
    return DefWindowProc( hWnd, uMsg, wParam, lParam );
}

void ProcessXInputState()
{
    XINPUT_STATE state;
    ZeroMemory( &state, sizeof(XINPUT_STATE) );
    DWORD result = XInputGetState( 0, &state );

    if( result == ERROR_SUCCESS )
    {
        // deal with left stick
        {
            vec2 leftstick = vec2( state.Gamepad.sThumbLX, state.Gamepad.sThumbLY );

            float deadzone = XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE;

            float magnitude = leftstick.Length();

            if( magnitude < deadzone )
            {
                leftstick.Set( 0, 0 );
            }
            else
            {
                // get the normalized direction vector of the thumbstick
                vec2 direction = leftstick / magnitude;

                magnitude = (magnitude - deadzone) / (32767 - deadzone);

                // clamp the magnitude to 1
                if( magnitude > 1 )
                    magnitude = 1;

                leftstick = direction * magnitude;
            }

            // Send an event
            InputEvent* pEvent = new InputEvent( InputDeviceType_Gamepad, InputState_Held, GamepadID_LeftStick, vec2(leftstick.x, leftstick.y) );
            g_pGameCore->QueueEvent( pEvent );
        }
    }
    else
    {
        // Controller not connected
        //OutputMessage( "Controller not connected\n" );
    }
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
    int width = SCREEN_WIDTH;
    int height = SCREEN_HEIGHT;

    g_EscapeButtonWillQuit = true;
    g_CloseProgramRequested = false;

    g_RequestedWidth = width;
    g_RequestedHeight = height;

    g_InitialWidth = width;
    g_InitialHeight = height;

    if( g_InitialWidth > g_InitialHeight )
    {
        g_InitialWidth = height;
        g_InitialHeight = width;
    }

    MSG msg;
    bool done = false;

    // Horrid key handling
    for( int i=0; i<256; i++ )
        g_KeyStates[i] = false;

    // Create Our OpenGL Window
    if( !CreateGLWindow( "OpenGL Window", width, height, 32, 31, 1, false ) )
    {
        return 0;
    }

    // Initialize OpenGL Extensions, must be done after OpenGL Context is created
    WGL_InitExtensions();
    OpenGL_InitExtensions();

    if( wglSwapInterval )
        wglSwapInterval( 1 );

    g_pGameCore = new GameCore();
    g_pGameCore->OnSurfaceChanged( SCREEN_WIDTH, SCREEN_HEIGHT );
    g_pGameCore->LoadContent();

    double lasttime = MyGetSystemTime();

    // Main loop
    while( !done )
    {
        if( PeekMessage( &msg, 0, 0, 0, PM_REMOVE ) )
        {
            if( msg.message == WM_QUIT )
            {
                done = true;
            }
            else
            {
                TranslateMessage( &msg );
                DispatchMessage( &msg );
            }
        }
        else
        {
            double currtime = MyGetSystemTime();
            float deltatime = (float)(currtime - lasttime);
            lasttime = currtime;

            ProcessXInputState();

            if( g_WindowIsActive )
            {
                if( g_CloseProgramRequested )
                {
                    done = true;
                }
                else
                {
                    g_pGameCore->Update( deltatime );

                    g_pGameCore->Draw();

                    SwapBuffers( hDeviceContext );

                    // Horrid key handling
                    for( int i=0; i<256; i++ )
                        g_OldKeyStates[i] = g_KeyStates[i];
                }
            }
        }
    }

    delete( g_pGameCore );
    g_pGameCore = 0;

    MyMemory_ValidateAllocations( true );

    KillGLWindow();

    return msg.wParam;
}
