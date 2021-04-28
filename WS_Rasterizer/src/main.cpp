#include <windows.h>

LRESULT CALLBACK eventHandler(HWND windowId, UINT eventType, WPARAM eventParameter, LPARAM parameterFlags) 
{
    switch (eventType) {
        case WM_CREATE:
            //OutputDebugStringA("\nWindow created!\n\n");
        break;
        case WM_CLOSE:
            PostQuitMessage(0);
            break;  
        default:
            return DefWindowProcA(windowId, eventType, eventParameter, parameterFlags);
        break;  
    }
    return 0;
}


int WINAPI WinMain(HINSTANCE processId, HINSTANCE hPrevInstance, PSTR lpCmdLine, int nCmdShow)
{   
    
    int displayWidth = 512; // image width
    int displayHeight = 512; // image height
     
    //________________________________  Creating a window
    LPCSTR windowClassName = "rasterizerWindow";

    WNDCLASSEXA windowDefinition = {
        sizeof(WNDCLASSEX),
        CS_OWNDC | CS_HREDRAW | CS_VREDRAW, // window styles, has its own context, redraws when the size changes
        eventHandler,
        0,0, /// extra bytes for extra stuff
        processId,
        NULL, // window icon
        NULL, // window cursor
        NULL, // background colour 
        NULL, // menu name
        windowClassName, // class identifier
        NULL
    };


    if (!RegisterClassExA(&windowDefinition))
        return -1;


    HWND windowId = CreateWindowExA(
        NULL, // window styles
        windowClassName, // registered class identifier
        "3D_Rasterizer", // title
        WS_VISIBLE | WS_OVERLAPPED | WS_CAPTION  | WS_SYSMENU | WS_MINIMIZEBOX, // more window styles and attributes (resizing buttons and whatsoever) 
        100,100, // x,y origin     
        displayWidth, displayHeight, // width and height 
        NULL, // parent window ID 
        NULL, // menu Id
        NULL, // process ID (passed to the registered class)
        NULL // values to be passed to the window (used in createDestruct event)
    );

    //________________________ program main loop.
    MSG msg = {0};
    while (GetMessage(&msg, NULL,NULL,NULL)) {
        TranslateMessage(&msg);
        DispatchMessageA(&msg);
    }

    return 0;
}