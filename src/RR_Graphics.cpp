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

#include <RetroRasterLib.h>

std::vector<SDL_WindowInfo> SDL_Windows;

RR_Window RR_CreateWindow(std::string name, int screen_width, int screen_height, int pixel_res_x, int pixel_res_y){
	RR_Window local_window;
	local_window.win_width = screen_width * pixel_res_x;
	local_window.win_height = screen_height * pixel_res_y;
	local_window.screen_width = screen_width;
	local_window.screen_height = screen_height;
    local_window.pixel_width = pixel_res_x;
    local_window.pixel_height = pixel_res_y;
	local_window.screen_pixels.resize(local_window.screen_width*local_window.screen_height);
	// Other stuff
    local_window.overscan_color = RR_RGBA(0,0,0);
    local_window.window_name = name;
    local_window.window_index = SDL_Windows.size();

	// Create an SDL2 window
	SDL_WindowInfo temp_sdl_window;

    uint32_t SDL_winFlags = SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE;

    if(!SDL_Windows.size()){
    	temp_sdl_window.window_ptr = SDL_CreateWindow(local_window.window_name.c_str(), SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, local_window.win_width, local_window.win_height, SDL_winFlags );

        // Make sure it was made
        if(temp_sdl_window.window_ptr == NULL){
            RR_WriteLog("Error! Window could not be created! SDL_Error: " + std::string(SDL_GetError()) );
            RR_ForceQuit();
            return local_window;
        }

        // Get the window surface
        temp_sdl_window.window_surface = SDL_GetWindowSurface(temp_sdl_window.window_ptr);
        if(temp_sdl_window.window_surface == NULL){
            RR_WriteLog("Error! Window surface could not be created! SDL_Error: " + std::string(SDL_GetError()) );
            RR_ForceQuit();
            return local_window;
        }
        
        SDL_Windows.push_back(temp_sdl_window);
    }else{
        RR_WriteLog("Warn! Failed to create new window!");
        RR_WriteLog("Window support is currently limited to one window!");
        return local_window;

    }

    return local_window;
};

void RR_DestroyWindow(RR_Window &window){
    if(window.window_index < 0 || window.window_index >= (int)SDL_Windows.size() ){
        RR_WriteLog("Error! Invalid window index for window \""+window.window_name+"\"");
        RR_ForceQuit();
        return;
    }
    RR_WipeSDLWindow(SDL_Windows.at(window.window_index));
    // Remove it from the list
    SDL_Windows.erase(SDL_Windows.begin()+window.window_index);
};


void RR_DestroyWindows(){
    for(long unsigned int i = 0; i < SDL_Windows.size(); i++){
        RR_WipeSDLWindow(SDL_Windows.at(i));
    }
    // Remove all windows
    SDL_Windows.clear();
};

void RR_WipeSDLWindow(SDL_WindowInfo &info){
    if(info.window_surface != NULL){
        SDL_FreeSurface(info.window_surface);
        info.window_surface = NULL;
    }
    if(info.window_ptr != NULL){
        SDL_DestroyWindow(info.window_ptr);
        info.window_ptr = NULL;
    }
};

void RR_UpdateWindow(RR_Window &window){
    if(window.window_index < 0 || window.window_index >= (int)SDL_Windows.size() ){
        RR_WriteLog("Error! Invalid window index for window \""+window.window_name+"\"");
        RR_ForceQuit();
        return;
    }
    SDL_WindowInfo &sdl_window = SDL_Windows.at(window.window_index);
    while( SDL_PollEvent( &sdl_window.window_events ) != 0 ){
		//User requests quit
		if( sdl_window.window_events.type == SDL_QUIT ){
			RR_Quit();
		} else if( sdl_window.window_events.type == SDL_KEYDOWN ){
			//Select surfaces based on key press
			switch( sdl_window.window_events.key.keysym.sym ){
				default:
				break;
			}
        }
    }
};

void RR_RasterWindow(RR_Window &window){
    if(window.window_index < 0 || window.window_index >= (int)SDL_Windows.size() ){
        RR_WriteLog("Error! Invalid window index for window \""+window.window_name+"\"");
        RR_ForceQuit();
        return;
    }
    SDL_WindowInfo &sdl_window = SDL_Windows.at(window.window_index);

    // Draw the overscan directly to the surface
	SDL_Rect overscanRect = {0, 0, window.win_width, window.win_height};
	SDL_FillRect( sdl_window.window_surface, &overscanRect,  window.overscan_color);

    // Lock the surface so we can draw to it
	SDL_LockSurface(sdl_window.window_surface);
/*
    uint32_t *framePixels = (Uint32 *)NE_FrameSurface->pixels;
	for(int y = 0; y < NE_FrameBufferHeight; y++){
		for(int x = 0; x < NE_FrameBufferWidth; x++){
			framePixels[(y*NE_FrameSurface->w)+x] = EGA_Color(NE_FrameBuffer[(y*NE_FrameBufferWidth)+x]);
		}
	}*/
	SDL_UnlockSurface(sdl_window.window_surface);
    // Unlock the surface so we can display it

	//Update the surface
	SDL_UpdateWindowSurface( sdl_window.window_ptr );

};

uint32_t RR_RGBA(unsigned char r,unsigned char g,unsigned char b,unsigned char a){
	return (a<<24)|(r<<16)|(g<<8)|b;
};

// Primitives

void RR_BlitRect(RR_Window &window, int x, int y, int w, int h, RR_Pixel pixel){
    // Clip the rect
    if(x < 0){
        w += x;
        x = 0;
    }
    if(y < 0){
        h += y;
        y = 0;
    }
    if(x+w >= window.screen_width){
        w += (window.screen_width-(x+w));
    }
    if(y+h >= window.screen_height){
        h += (window.screen_height-(y+h));
    }
    if(x >= window.screen_width) return; // Nothing to draw
    if(y >= window.screen_height) return; // Nothing to draw

    // Draw the rectangle
    for(int i = y; i < y+h; i++){
        for(int e = x; e < x+w; e++){
            RR_Pixel &scr_pixel = window.screen_pixels[(i*window.screen_width) + e];
            if(scr_pixel.depth <= pixel.depth){
                scr_pixel = pixel;
            }
        }
    }
};

void RR_ClearScreen(RR_Window &window, RR_Pixel pixel){
    for(int i = 0; i < (int)window.screen_pixels.size(); i++){
        window.screen_pixels.at(i) = pixel;
    }
};
