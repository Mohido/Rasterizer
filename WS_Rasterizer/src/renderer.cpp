#include "renderer.h"


/*
* Explanation: A function that takes an array of pixels (1byte array). Then it fills it up with a radiant filling through the index x
* Parameters :
    pixels - A pointer to the begining of the pixels array
    width - width of the pixel array
    height - height of the pixel arra
    pixelSize - bytes per pixel for the remapping from coordinates to the exact pixel values
*/
void renderer::gradGenerator(char* pixels, const unsigned int& width, const unsigned int& height, const unsigned int& pixelSize, const unsigned int& offset) {
    if (pixelSize != 4) {
        return;
    }
    for (unsigned int x = 0; x < width; x++) {
        for (unsigned int y = 0; y < height; y++) {
            unsigned int index = (x + y * width) * pixelSize;
            pixels[index] = (x + offset) % 255;            //r
            pixels[index + 1] = (y + offset) % 255;        //g
            pixels[index + 2] = index % 255;               //b
        }
    }
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
void renderer::renderCanvas(const HWND& windowId, void* pixelsArray, const int& displayWidth, const int& displayHeight, const int& bytesPerPixel) {
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
* Explanation: Renders all the raaster pixels to black
* Parameters :
    pixels - A pointer to the begining of the pixels array
    width - width of the pixel array
    height - height of the pixel arra
    pixelSize - bytes per pixel for the remapping from coordinates to the exact pixel values
* 
*/
void renderer::clearPixels(char* pixels, const unsigned int& width, const unsigned int& height, const unsigned int& pixelSize) {
    if (pixelSize != 4 && pixelSize != 3)
        return;
    for (unsigned int x = 0; x < width; x++) {
        for (unsigned int y = 0; y < height; y++) {
            unsigned int index = (x + y * width) * pixelSize;
            pixels[index] = 0;            //r
            pixels[index + 1] = 0;        //g
            pixels[index + 2] = 0;        //b
        }
    }
}




/*
* Explanation: Draws a single pixel with the given colour to the given pixel array.
* Parameters:
    pixelsArray - a pointer to the memory address with the pixel values ( the bitmap will use ) to be copied to the client area of the window
    displayWidth - the image width (display / client_area / Canvas width)
    displayHeight - the image height (display / client_area / Canvas height)
    bytesPerPixel - How many bytes do each pixel occupy?
    xCord - the x-cordinate of the pixel
    yCord - the y-cordinate of the pixel
    col - A Color Structure that determines the color of the pixel
    size - the intensity of the pixel (it can take multiple pixels according to the given input)
*/
void renderer::renderPixel(char* pixels, const unsigned int& width, const unsigned int& height, const unsigned int& pixelSize,
                            const unsigned int& xCord, const unsigned int& yCord, const Color& col, const unsigned int& size) 
{
    if (xCord >= width - size || yCord >= height - size || xCord <= 0 + size || yCord <= 0 + size) return;
    for (int i = 0; i < size; i++) {
        for (int j = 0; j < size; j++) {
            pixels[(xCord+i + (yCord + j) * width) * pixelSize] = col.r;
            pixels[(xCord+i + (yCord + j) * width) * pixelSize + 1] = col.g;
            pixels[(xCord+i + (yCord + j) * width) * pixelSize + 2] = col.b;
            pixels[(xCord+i + (yCord + j) * width) * pixelSize + 3] = col.a;
        }
    }
}