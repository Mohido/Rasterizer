#pragma once
#ifndef Renderer
#define Renderer

#ifndef _WINDOWS_ // if windows api not included yet
#include <windows.h>
#endif 


namespace renderer {

	struct Color {
		char r; char g; char b; char a; 
	};

	void gradGenerator(char* pixels, const unsigned int& width, const unsigned int& height, const unsigned int& pixelSize, const unsigned int& offset); // fill the array of pixels with a gradient colour
	void renderCanvas(const HWND& windowId, void* pixelsArray, const int& displayWidth, const int& displayHeight, const int& bytesPerPixel); // initialize the bitmap and assign it to the screen
	void clearPixels(char* pixels, const unsigned int& width, const unsigned int& height, const unsigned int& pixelSize); // render all pixels to black
	void renderPixel(char* pixels, const unsigned int& width, const unsigned int& height, const unsigned int& pixelSize,
					const unsigned int& xCord, const unsigned int& yCord, const Color& col, const unsigned int& size = 1); // draw a single pixel according to the given parameters
}	
#endif
