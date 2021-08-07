#define NOMINMAX
#include <windows.h>    // _WINDOWS_ 
#include <string>
#include <limits> // limits to get Infinity (used in the depth buffer)
#define _USE_MATH_DEFINES
#include <math.h>
#include "renderer.h"
#include "geometry.h"


#define SEC_TO_NANOSEC 1000000000LL
#define SEC_TO_MILLISEC 1000LL
#define NANOSECONDS_PER_FRAME (SEC_TO_NANOSEC/60LL)



void printMessage(const std::wstring& msg); //printing a message to the console
LRESULT CALLBACK eventHandler(HWND, UINT, WPARAM, LPARAM); // handle the window events
HWND initWindow(HINSTANCE processId, const unsigned int& displayWidth , const unsigned int& displayHeight); // initialize the window
long long getCurrentTime_nanoseconds(); // gets a high resolution time in nanoseconds



/*======================== MAIN FUNCTION ============================*/

int WINAPI WinMain(HINSTANCE processId, HINSTANCE hPrevInstance, PSTR lpCmdLine, int nCmdShow)
{   

    //TODO: Loading .obj to the game and trying out to render a whole mesh
    //TODO: Fix the rotation problem,
    //TODO: Add Camera projection and better projection algorithm, with aspect ratio fixing
    //TODO: seperate the updating and rendering functionalities to their own functions rather than all the JUNK in the update loop()

    //___________________________ Varialbe initializiation area
    int displayWidth = 512; // image width. Pixels
    int displayHeight = 512; // image height. Pixels
    int bytesPerPixel = 4; /// 4 bytes per pixel
    int imageSize = displayHeight * displayWidth * bytesPerPixel;
    void* pixelsArray = VirtualAlloc(0, imageSize, MEM_RESERVE | MEM_COMMIT, PAGE_READWRITE);

    /*Z-Buffer*/
    float* depthArray = (float*)VirtualAlloc(0, displayHeight * displayWidth * 4, MEM_RESERVE | MEM_COMMIT, PAGE_READWRITE);

    // _________________________ Initializing the triangles
    std::vector<geometry::Triangle_3D<float>> mesh;

    /*Triangle 1*/
    double rotator = 0;

    geometry::Vector_3D<float> vec1(1, -1, 0);
    geometry::Vector_3D<float> vec2(-1, -1, 0);
    geometry::Vector_3D<float> vec3(0, 1, 0);

    geometry::Vertex_3D<float> ver1(vec1, renderer::Color(255,0,0,0));
    geometry::Vertex_3D<float> ver2(vec2, renderer::Color( 0, 255, 0, 0));
    geometry::Vertex_3D<float> ver3(vec3, renderer::Color( 0, 0, 255, 0));

    mesh.push_back( geometry::Triangle_3D<float>(ver1, ver2, ver3) );


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
          
            /*Setting up depth buffer*/
            memset(depthArray, std::numeric_limits<char>::max(), displayHeight * displayWidth * 4);


            /*Rendering logic*/
            renderer::clearPixels((char*)pixelsArray, displayWidth, displayHeight, bytesPerPixel);                  // clearing hte pixels sheet
           
            for ( auto tri : mesh) { //going through the mesh triangles individualy
                // Transformation Matrices
                // TODO: Using 1 number for rotation instead of the expensive sin and cosin 1 - 0.1(degree of rotation)
                geometry::Matrix_4x4<float> rotation_z    ( cos(rotator) ,sin(rotator),0,0,
                                                           -sin(rotator), cos(rotator),0,0,
                                                            0,0,1,0,
                                                            0,0,0,1); // transitioning the x

                geometry::Matrix_4x4<float> rotation_y (cos(rotator),0, sin(rotator),0,
                                                       0,1,0, 0,
                                                       -sin(rotator), 0 , cos(rotator), 0,
                                                       0, 0, 0, 1); // transitioning the x

                
                geometry::Matrix_4x4<float> translation ( 1, 0, 0, 0,
                                                          0, 1, 0, 0,
                                                          0, 0, 1, 0,
                                                          0, 0, 2, 1 );


                //rotator = ( rotator >= 1.0 )? 0: rotator + 0.01;
                rotator += 0.01;
                
                
                std::vector<geometry::Vertex_3D<float>> vertices = tri.getVertices(); // the vertix contain col and pos for now

                /*getting the vertices positions (points/vectors) to simplify writing them*/
                geometry::Vector_3D<float> v1 = vertices[0].position * translation; //(vertices[0].position * rotation_y) * translation;
                geometry::Vector_3D<float> v2 = vertices[1].position * translation; //(vertices[1].position * rotation_y) * translation;
                geometry::Vector_3D<float> v3 = vertices[2].position * translation; //(vertices[2].position * rotation_y) * translation;

               /* std::string te = v2.toString() + v1.toString() + v3.toString();
                printMessage(std::wstring(te.begin(), te.end()));*/

                if (v1.z == 0.0 || v2.z == 0.0 || v3.z == 0.0) continue; // if the triangle is beyond the camera veiw, don't draw it

                /*Projecting Triangle Vertices*/
                // TODO:: Implementing of Camera projection + aspect ratio editing
                geometry::Vector_2D<int> sv1( (v1.x / v1.z)*displayWidth + displayWidth / 2.0 , (v1.y / -v1.z) * displayWidth +displayHeight / 2.0);
                geometry::Vector_2D<int> sv2( (v2.x / v2.z)*displayWidth + displayWidth / 2.0 , (v2.y / -v2.z) * displayWidth +displayHeight / 2.0);
                geometry::Vector_2D<int> sv3( (v3.x / v3.z)*displayWidth + displayWidth / 2.0 , (v3.y / -v3.z) * displayWidth +displayHeight / 2.0);
                geometry::Triangle_2D<int> tri_screen(sv1, sv2, sv3); // used for simplifying the pointInside() function and might come in handy later

                /*
                * The triangle looks as follows:
                           sv3
                           / \
                          /   \
                         /     \
                     sv2 ------ sv1
                */


                /*Calculating the inverses of the orig depth values.. This way we will avoid deviding by small numbers => less numerical roudning errors*/
                const float z_1_inv = 1 / v1.z;
                const float z_2_inv = 1 / v2.z;
                const float z_3_inv = 1 / v3.z;


                /*Going through the boxing boundries of the triangle*/
                for (int y = std::min(sv1.y, std::min(sv2.y, sv3.y)); y <= std::max(sv1.y, std::max(sv2.y, sv3.y)); y++) {
                    if (y < 0) continue;
                    if (y >= displayHeight) break; // rendering from top to bottom since the image is read from top to bottom
                    for (int x = std::min(sv1.x, std::min(sv2.x, sv3.x)); x <= std::max(sv1.x, std::max(sv2.x, sv3.x)); x++) {
                        if (x < 0) continue;
                        if (x >= displayWidth) break;

                        geometry::Vector_2D<int> point(x, y);

                        // Note that the edgeFunction algorithm clips the triangle if it was reverse (inversed). Which is a great additional feature.
                        // not rendering the backside of the triangle is because of the clockwise triangle rendering. rotating (inversing) will breach the clockwise rendering rule.
                        bool isInside = tri_screen.pointInside(point);  // Using the EdgeFunction Algorithm to quickly check if a point is inside a triangle or not
                        
                        if (isInside) {                        
                            /*Getting the Ratios of the point (Barcyntric Interpolation)*/
                            float area_1 = abs((point - sv2).crossProduct(point - sv3)) / 2; // Triangle area = Parallelgram Area / 2... 
                            float ratio_1 = area_1 / tri_screen.getArea();

                            float area_2 = abs((sv1 - point).crossProduct(sv3 - point)) / 2;
                            float ratio_2 = area_2 / tri_screen.getArea();

                            float ratio_3 = 1 - (ratio_1 + ratio_2); // this will have some errors mostly because of close operations on floats

                            /*Interpolating the depth of the current ratio by using the inverse of Z Baarcyntric interpolation*/
                            float fullRatio_1 = z_1_inv * ratio_1;
                            float fullRatio_2 = z_2_inv * ratio_2;
                            float fullRatio_3 = z_3_inv * ratio_3;

                            float depth = 1 / (fullRatio_1 + fullRatio_2 + fullRatio_3);
                            if (depth <= 0) continue;

                            if (depth < depthArray[x + y * displayWidth]) {

                                /*Interpolating colours*/
                                unsigned char red = depth *
                                      ( fullRatio_1 * (vertices[0].col.r ) + 
                                          fullRatio_2 * (vertices[1].col.r) +
                                          fullRatio_3 * (vertices[2].col.r)
                                          );
                                 
                                unsigned char green = depth *
                                    (fullRatio_1 * (vertices[0].col.g ) +
                                        fullRatio_2 * (vertices[1].col.g ) +
                                        fullRatio_3 * (vertices[2].col.g )
                                        );

                                unsigned char blue = depth *
                                    (fullRatio_1 * (vertices[0].col.b) +
                                        fullRatio_2 * (vertices[1].col.b) +
                                        fullRatio_3 * (vertices[2].col.b)
                                        ); // attr = z ( (attr_0 / z_0) *ratio_0 + (attr_1 / z_1) * ratio_1... ) 


                                renderer::Color col(red, green, blue, 0);

                                depthArray[x + y * displayWidth] = depth;
                                /*Rendering the point*/
                                renderer::renderPixel((char*)pixelsArray, displayWidth, displayHeight, bytesPerPixel, x, y, col, 1);
                            }
                        }
                    }
                }
            }
            


            /*Rendering the canvas to the screen*/
            renderer::renderCanvas(windowId, pixelsArray, displayWidth, displayHeight, bytesPerPixel);              // re-initialize the canvas
            /*Last time is updated*/
            lastTime = current_time;
        }
    }



    /*releasing the reserved memory addresses*/
    if (pixelsArray != 0) VirtualFree(pixelsArray, 0, MEM_RELEASE);
    if (depthArray != 0) VirtualFree(depthArray, 0, MEM_RELEASE);

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