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
    RR_Color rgba; // Color to be displayed
    int depth;     // Depth of pixel (default is 0)
}RR_Pixel;

typedef struct RR_Window_t {
    int win_width;    // Width of the window
    int win_height;   // Height of the window
    int screen_width; // Width of the pixel screen
    int screen_height;// Height of the pixel screen
    int pixel_width;  // Width of pixels
    int pixel_height; // Height of pixels
    RR_Color overscan_color; // Color to display when the 
                             // screen is smaller than the window
    std::vector<RR_Pixel> screen_pixels; // List of pixels to be displayed
    int window_index; // Index into the window handler
    std::string window_name; // Name of the window
}RR_Window;

typedef struct SDL_WindowInfo_t {
	SDL_Window *window_ptr;
	SDL_Surface *window_surface;
	SDL_Event *window_events;
}SDL_WindowInfo;


/*
name          -> Name of the window
screen_width  -> Width of the pixel screen
screen_height -> Height of the pixel screen
pixel_res_x   -> Width of the pixels rendered
pixel_res_y   -> Height of the pixels rendered
== Returns a window to be used
*/
RR_Window RR_CreateWindow(std::string name, int screen_width, int screen_height, int pixel_res_x, int pixel_res_y);

// window   ->   Refrence to window to be destroyed
void RR_DestroyWindow(RR_Window &window);

// Destroys all windows (should not be called by user)
void RR_DestroyWindows();

// Destroys all SDL windows (should not be called by user)
void RR_WipeSDLWindow(SDL_WindowInfo &info);

// window   ->   Refrence to window to be rastered
void RR_RasterWindow(RR_Window &window);

/*
r -> Red value
g -> Green value
b -> Blue value
a -> Alpha value (optional, default 255)
*/
uint32_t RR_RGBA(unsigned char r,unsigned char g,unsigned char b,unsigned char a = 0xFF);


