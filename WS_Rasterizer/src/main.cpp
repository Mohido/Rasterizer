#include <windows.h>
#include <stdlib.h>     /* srand, rand */
#include <time.h>       /* time */

LRESULT CALLBACK eventHandler(HWND, UINT, WPARAM, LPARAM);
HWND initWindow(HINSTANCE processId, int displayWidth ,int displayHeight);
void gradGenerator(char* pixels, unsigned int width, unsigned int height, unsigned int pixelSize);
void initCanvas(HWND windowId, void* pixelsArray, int displayWidth, int displayHeight, int bytesPerPixel);


int WINAPI WinMain(HINSTANCE processId, HINSTANCE hPrevInstance, PSTR lpCmdLine, int nCmdShow)
{   
    //___________________________ Varialbe initializiation area
    int displayWidth = 512; // image width. Pixels
    int displayHeight = 512; // image height. Pixels
    int bytesPerPixel = 4; /// 4 bytes per pixel
    int imageSize = displayHeight * displayWidth * bytesPerPixel;
    void* pixelsArray = VirtualAlloc(0, imageSize , MEM_RESERVE | MEM_COMMIT, PAGE_READWRITE);
    gradGenerator((char*)pixelsArray, displayWidth,displayHeight, bytesPerPixel); // initialize the pixel array

    //__________________________ Initializing the window area
    HWND windowId = initWindow(processId, displayWidth, displayHeight);
    initCanvas(windowId, pixelsArray, displayWidth, displayHeight, bytesPerPixel);
    
    
    //________________________ program main loop.
    MSG msg = {0};
    while (GetMessage(&msg, NULL,NULL,NULL)) {
        TranslateMessage(&msg);
        DispatchMessageA(&msg);
    }
    
    return 0;
}




/*
* Explanation: This funciton initializes the canvas ( Client Area ) with the given parameters
* Parameters:
    windowId - A handler to the window
    pixelsArray - a pointer to the memory address with the pixel values ( the bitmap will use ) to be copied to the client area of the window
    displayWidth - the image width (display / client_area / Canvas width)
    displayHeight - the image height (display / client_area / Canvas height)
    bytesPerPixel - How many bytes do each pixel occupy?
*/
void initCanvas(HWND windowId, void* pixelsArray, int displayWidth, int displayHeight, int bytesPerPixel) {
    //_______________________________  Creating a bitmap to render the window Client area (visible area) with
    HDC windowCanvas = GetDC(windowId);
    BITMAPINFO bmi = { 0 }; // background bitmap
    BITMAPINFOHEADER bmiHeader = { sizeof(BITMAPINFOHEADER),         // size of the structure
                                        displayWidth, -displayHeight,   // width and height of the bitmap
                                        1,                              // must be set to 1 according to MSDN
                                        8 * bytesPerPixel,                // bits per pixel
                                        BI_RGB,                         // uncompressed bitmap. Compressiono of bitmap option
                                        displayHeight * displayWidth * 4,   // size of the image in bytes: (32/8) * w * h
                                        GetDeviceCaps(windowCanvas, HORZRES) / GetDeviceCaps(windowCanvas,HORZSIZE), // the pixels per meter ratio of the width
                                        GetDeviceCaps(windowCanvas, VERTRES) / GetDeviceCaps(windowCanvas,VERTSIZE), // the pixels per meter ratio of the height
                                        0 /*colour table indices*/ ,0  /*0 = all colours are important*/
    };
    bmi.bmiHeader = bmiHeader;

    if (!StretchDIBits(windowCanvas, // window canvas
        0, 0, displayWidth, displayHeight,
        0, 0, displayWidth, displayHeight, // streched width and height
        pixelsArray, // pixels to be streched
        &bmi, // the bitmap info
        DIB_RGB_COLORS, SRCCOPY))
        MessageBox(NULL, L"hello", L"HOOO", MB_OK);

    ReleaseDC(windowId, windowCanvas);

}


/*
* Explanation: A function that takes an array of pixels (1byte array). Then it fills it up with a radiant filling through the index x
* Parameters : 
    pixels - A pointer to the begining of the pixels array
    width - width of the pixel array
    height - height of the pixel arra
    pixelSize - bytes per pixel for the remapping from coordinates to the exact pixel values
*/
void gradGenerator(char* pixels, unsigned int width, unsigned int height, unsigned int pixelSize) {
    if (pixelSize != 4) {
        return;
    }
    for (unsigned int x = 0; x < width; x++) {
        for (unsigned int y = 0; y < height; y++) {
            unsigned int index = (x + y * width ) * pixelSize;
            pixels[index] = x % 255;            //r
            pixels[index + 1] = y % 255;        //g
            pixels[index + 2] = index % 255;    //b
       }   
    }
}




/*
* Explanation: It craetes a window that is used in the current rasterizer software
* Parameters: 
    processId - The main program Id.
    displayWidth - width of the window in pixels
    display height - height of the window in pixels
* Returns: A handle to the created window.
*/
HWND initWindow(HINSTANCE processId,  int displayWidth, int displayHeight) {
    //________________________________  Creating a window
    LPCSTR windowClassName = "rasterizerWindow";

    WNDCLASSEXA windowDefinition = {
        sizeof(WNDCLASSEX),
        CS_OWNDC | CS_HREDRAW | CS_VREDRAW, // window styles, has its own context, redraws when the size changes
        eventHandler,   // The funciton that handles the event of the window
        0,0, /// extra bytes for extra stuff
        processId, // main process that the window will sit on
        NULL, // window icon
        NULL, // window cursor
        NULL, // background colour 
        NULL, // menu name
        windowClassName, // class identifier
        NULL
    };


    if (!RegisterClassExA(&windowDefinition))
    {
        MessageBox(NULL, L"Failed to register the window", L"Failed" ,MB_OK);
        exit(-1);
    }

    //From the canvas size we get the window size
    RECT canvas = {0,0, displayWidth, displayHeight};
    AdjustWindowRect(&canvas, WS_CAPTION | WS_THICKFRAME, false);

    //creating the window
    HWND windowId = CreateWindowExA(
        NULL, // window styles
        windowClassName, // registered class identifier
        "3D_Rasterizer", // title
        WS_VISIBLE | WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX, // more window styles and attributes (resizing buttons and whatsoever) 
        100, 100, // x,y origin     
        canvas.right  - canvas.left, canvas.bottom - canvas.top, // width and height 
        NULL, // parent window ID 
        NULL, // menu Id
        NULL, // process ID (passed to the registered class)
        NULL // values to be passed to the window (used in createDestruct event)
    );
    return windowId;
}

/*
* Event handling function.
*/
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