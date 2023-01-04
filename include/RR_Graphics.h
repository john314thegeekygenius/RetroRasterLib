/*
	RetroRasterLib - a 2D graphics engine
	Written by Jonathan Clevenger
	January 3, 2023
	Copyright (c) 2023 TimeLabs LLC

	Use of this libaray is allowed
	as long as the name and author is 
	refrenced to in the application
	using it, such as:
    
				Made using RetroRasterLib
	www.github.com/john314thegeekygenius/RetroRasterLib

	The following comment must appear in
	all released source code using the library:

	=================================================================
	Uses RetroRasterLib written by Jonathan Clevenger
	Find it here: www.github.com/john314thegeekygenius/RetroRasterLib
	=================================================================

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <https://www.gnu.org/licenses/>.
	
*/
#pragma once

#include <RR_Headers.h>

typedef uint32_t RR_Color; // Uses ARGB format: 0xAARRGGBB

typedef struct RR_Pixel_t {
    RR_Color rgba;  // Color to be displayed
    uint32_t depth = 0; // Depth of pixel
}RR_Pixel;

typedef struct RR_Window_t {
    int win_width;    // Width of the window
    int win_height;   // Height of the window
    int screen_width; // Width of the pixel screen
    int screen_height;// Height of the pixel screen
    float pixel_width;  // Width of pixels
    float pixel_height; // Height of pixels
    bool keep_aspect; // Keep the aspect ratio of the screen
    RR_Color overscan_color; // Color to display when the 
                             // screen is smaller than the window
    std::vector<RR_Pixel> screen_pixels; // List of pixels to be displayed

    std::string window_name; // Name of the window
    int window_index; // Index into the window handler
}RR_Window;

/*
Returns a new window
==Please note only one window is allowed right now==
name          -> Name of the window
screen_width  -> Width of the pixel screen
screen_height -> Height of the pixel screen
pixel_res_x   -> Width of the pixels rendered
pixel_res_y   -> Height of the pixels rendered
*/
RR_Window RR_CreateWindow(std::string name, int screen_width, int screen_height, float pixel_res_x, float pixel_res_y, uint32_t flags = 0);

// window   ->   Refrence to window to be destroyed
void RR_DestroyWindow(RR_Window &window);

// Destroys all windows (should not be called by user)
void RR_DestroyWindows();

// Updates the window and checks window events 
// window   ->   Refrence to window to be rastered
void RR_UpdateWindow(RR_Window &window);

// Renders the graphics to the window
// window   ->   Refrence to window to be rastered
void RR_RasterWindow(RR_Window &window);


// Returns the current Frames Per Second of the window
uint32_t RR_GetFPS();

// Sets the maximum fps
void RR_SetMaxFPS(uint32_t maxfps);

/*
r -> Red value
g -> Green value
b -> Blue value
a -> Alpha value (optional, default 255)
*/
uint32_t RR_RGBA(unsigned char r,unsigned char g,unsigned char b,unsigned char a = 0xFF);

/*
Draws a rectangle on the screen
window  -> window to render to
x       -> position horizontaly on the screen
y       -> position vertically on the screen
w       -> width of rectangle
h       -> height of rectangle
pixel   -> pixel info to render
*/
void RR_BlitRect(RR_Window &window, int x, int y, int w, int h, RR_Pixel pixel);

/*
Draws an ellipse on the screen
window  -> window to render to
x       -> centered position horizontaly on the screen
y       -> centered position vertically on the screen
w       -> width of ellipse
h       -> height of ellipse
pixel   -> pixel info to render
*/
void RR_BlitEllipse(RR_Window &window, int x, int y, int w, int h, RR_Pixel pixel);


/*
Draws a line on the screen
window  -> window to render to
x1      -> point A position horizontaly on the screen
y1      -> point A position vertically on the screen
x2      -> point B position horizontaly on the screen
y2      -> point B position vertically on the screen
pixel   -> pixel info to render
*/
void RR_BlitLine(RR_Window &window, int x1, int y1, int x2, int y2, RR_Pixel pixel);

/*
Draws an ellipse on the screen
window  -> window to render to
x[1-3]  -> point positions horizontaly on the screen
y[1-3]  -> point positions vertically on the screen
pixel   -> pixel info to render
*/
void RR_BlitTriangle(RR_Window &window, int x1, int y1, int x2, int y2, int x3, int y3, RR_Pixel pixel);


/*
Clears the screen to a set pixel
window -> window to clear screen of
pixel  -> pixel data to clear with
*/
void RR_ClearScreen(RR_Window &window, RR_Pixel pixel);
