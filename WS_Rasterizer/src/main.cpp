#include <windows.h>    // _WINDOWS_ 
#include <string>
#include <algorithm>
#include <random>
#include "Geometry.h"
#include "renderer.h"

#define SEC_TO_NANOSEC 1000000000LL
#define SEC_TO_MILLISEC 1000LL
#define NANOSECONDS_PER_FRAME (SEC_TO_NANOSEC/60LL)
#define STARS_COUNT 10000 // you can go crazy here



void printMessage(const std::wstring& msg); //printing a message to the console
LRESULT CALLBACK eventHandler(HWND, UINT, WPARAM, LPARAM); // handle the window events
HWND initWindow(HINSTANCE processId, const unsigned int& displayWidth , const unsigned int& displayHeight); // initialize the window
long long getCurrentTime_nanoseconds(); // gets a high resolution time in nanoseconds



/*======================== MAIN FUNCTION ============================*/

int WINAPI WinMain(HINSTANCE processId, HINSTANCE hPrevInstance, PSTR lpCmdLine, int nCmdShow)
{   
    //___________________________ Varialbe initializiation area
    int displayWidth = 512; // image width. Pixels
    int displayHeight = 512; // image height. Pixels
    int bytesPerPixel = 4; /// 4 bytes per pixel
    int imageSize = displayHeight * displayWidth * bytesPerPixel;
    void* pixelsArray = VirtualAlloc(0, imageSize , MEM_RESERVE | MEM_COMMIT, PAGE_READWRITE);

    //__________________________ Initializing the window area
    HWND windowId = initWindow(processId, displayWidth, displayHeight);
    renderer::renderCanvas(windowId, pixelsArray, displayWidth, displayHeight, bytesPerPixel);
    
    //________________________ program main loop.
    long long lastTime = getCurrentTime_nanoseconds();
    MSG msg = { 0 };
    bool running = true;
    while (running) {
        /*Process new events*/ // Note: Peekmessage returns false if there is no new message
        while (PeekMessage(&msg, NULL, NULL, NULL, PM_REMOVE)) { // peek does not halt the program, unlike getMessage(); 
            if (msg.message == WM_QUIT) running = false;
            TranslateMessage(&msg);     // adding a message to the queue
            DispatchMessageA(&msg);    // we process the last message we caught
        }

        /*Update and render*/
        long long current_time = getCurrentTime_nanoseconds();
        if (current_time - lastTime >= NANOSECONDS_PER_FRAME) { // every frame 
            /*Inputing logic*/
            //..
            //..
            /*Rendering logic*/
            renderer::clearPixels((char*)pixelsArray, displayWidth, displayHeight, bytesPerPixel);                  // clearing hte pixels sheet


            /*Drawing the Triangles*/
            geometry::Vector_3D<float> v1(100.0, -100.0, 1.0);
            geometry::Vector_3D<float> v2(-100.0, -100.0, 1.0);
            geometry::Vector_3D<float> v3(0.0, 100.0, 1.0);
            //geometry::Triangle_3D<float> tri(v1,v2,v3);

            //projecting the 3 points..
            geometry::Vector_2D<int> sv1((v1.x / v1.z) + displayWidth / 2, (v1.y / -v1.z) + displayHeight / 2);
            geometry::Vector_2D<int> sv2((v2.x / v2.z) + displayWidth / 2, (v2.y / -v2.z) + displayHeight / 2);
            geometry::Vector_2D<int> sv3((v3.x / v3.z) + displayWidth / 2, (v3.y / -v3.z) + displayHeight / 2);

            renderer::Color col = { 255,255,255,0 };
            
            /*Going through the boxing boundries of the triangle*/
            for (int x = min(sv1.x, min(sv2.x, sv3.x)); x <= max(sv1.x, max(sv2.x, sv3.x)); x++) {
                for (int y = min(sv1.y, min(sv2.y, sv3.y)); y <= max(sv1.y, max(sv2.y, sv3.y)); y++) {
                    geometry::Vector_2D<int> point(x,y);
                    // Using the EdgeFunction Algorithm to quickly check if a point is inside a triangle or not
                    bool isInside = (sv2 - sv1).crossProduct(point - sv1) >= 0
                                 && (sv3 - sv2).crossProduct((point - sv2)) >= 0
                                 && (sv1 - sv3).crossProduct((point - sv3)) >= 0;

                    if(isInside)
                        renderer::renderPixel((char*)pixelsArray, displayWidth, displayHeight, bytesPerPixel, x, y, col, 1);
                }
            }
            

            /*Rendering the canvas to the screen*/
            renderer::renderCanvas(windowId, pixelsArray, displayWidth, displayHeight, bytesPerPixel);              // re-initialize the canvas
            lastTime = current_time;
        }
    }

    /*releasing the pixels sheet*/
    if(pixelsArray != 0)
        VirtualFree(pixelsArray, 0, MEM_RELEASE);

    //delete[] stars; // tried to render 1000000 stars .. and it works super fine lol!!
    return 0;
}




/*==================== EXTRA FUNCTIONS ============================*/


/*
* Explanation: Prints out a message to the console.
* Parameters: A wide string which is the message value
*/
void printMessage(const std::wstring& msg) {
    LPCWSTR out_c = (LPCWSTR)msg.c_str();
    OutputDebugStringW(out_c);
    OutputDebugStringW(L"\n");
}









/*
* Explanation: A high resolution time query.
* Returns: The current high-resolution system time in nanoseconds.
*/
long long getCurrentTime_nanoseconds() {
    LARGE_INTEGER ticks; LARGE_INTEGER ticksPerSec;
    QueryPerformanceCounter(&ticks);                                    //  ticks since the begining of the program
    QueryPerformanceFrequency(&ticksPerSec);                            //  ticks per second
    return (SEC_TO_NANOSEC * ticks.QuadPart) / ticksPerSec.QuadPart;    // return the elapsed time in nanoseconds
}










/*
* Explanation: It craetes a window that is used in the current rasterizer software
* Parameters: 
    processId - The main program Id.
    displayWidth - width of the window in pixels
    display height - height of the window in pixels
* Returns: A handle to the created window.
*/
HWND initWindow(HINSTANCE processId, const unsigned int& displayWidth, const unsigned int& displayHeight) {
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
        { MessageBox(NULL, L"Failed to register the window", L"Failed" ,MB_OK); exit(-1); }
    
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