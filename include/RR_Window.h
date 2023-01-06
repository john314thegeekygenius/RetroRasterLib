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

typedef struct SDL_WindowInfo_t {
	SDL_Window *window_ptr = NULL;
	SDL_Surface *window_surface = NULL;
    SDL_Surface *screen_surface = NULL;
}SDL_WindowInfo;

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
    bool window_closed; // Is the window closed

	bool keycodes[256] = {false}; // List of keycodes
	RR_Mouse window_mouse; // Mouse info
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

// Returns if the window specified was closed (should be checked before using window)
bool RR_CheckWindowClosed(RR_Window &window);

/*
Set the window title
window   -> window use
title    -> name to use for window
*/
void RR_SetWindowTitle(RR_Window &window, std::string title);

/*
Set the window icon to an image
window   -> window use
icon_img -> image to be used as an icon
*/
void RR_SetWindowIcon(RR_Window &window, RR_Image &icon_img);

/*
Set the window into or out of fullscreen mode
window   -> window use
set_flag -> should the window be fullscreen
*/
void RR_SetWindowFullscreen(RR_Window &window, bool set_flag);

/*
Set the window resizeable
window   -> window use
set_flag -> should the window be resizeable
*/
void RR_SetWindowResizeable(RR_Window &window, bool set_flag);

/*
Changes the overscan color
window -> window use
pixel  -> pixel with color data
*/
void RR_SetOverscanColor(RR_Window &window, RR_Pixel pixel);

/*
Keep the pixels square
window   -> window use
set_flag -> keep aspect ratio
*/
void RR_SetAspectRatio(RR_Window &window, bool set_flag);

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

