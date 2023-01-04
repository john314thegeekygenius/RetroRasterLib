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
#include "RR_Window.h"

std::vector<SDL_WindowInfo> SDL_Windows;

uint64_t g_RR_FPSNextTick = 0;
uint64_t g_RR_FrameTime = 0;
uint64_t g_RR_LastFrameTick = 0;
uint32_t g_RR_FrameCount = 0;
uint32_t g_RR_FPS = 0;

uint32_t g_RR_MaxFPS = 144; // 144hz
uint64_t g_RR_InputNextRead = 0;


RR_Window RR_CreateWindow(std::string name, int screen_width, int screen_height, float pixel_res_x, float pixel_res_y, uint32_t flags){
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

        // If we keep the aspect ratio
        if(local_window.keep_aspect){
            // Set the minimum window size to 16x16
            SDL_SetWindowMinimumSize(temp_sdl_window.window_ptr,local_window.screen_width,local_window.screen_height);
        }else{
            // Set the minimum window size to 16x16
            SDL_SetWindowMinimumSize(temp_sdl_window.window_ptr,64,64);
        }

//        RR_WriteLog("Created window with SDL id:"+std::to_string(temp_sdl_window.window_ptr->id));

        // Get the window surface
        temp_sdl_window.window_surface = SDL_GetWindowSurface(temp_sdl_window.window_ptr);
        if(temp_sdl_window.window_surface == NULL){
            RR_WriteLog("Error! Window surface could not be created! SDL_Error: " + std::string(SDL_GetError()) );
            RR_ForceQuit();
            return local_window;
        }

        // Create a surface for the screen pixels
        temp_sdl_window.screen_surface = SDL_CreateRGBSurface(0, local_window.screen_width, local_window.screen_height, temp_sdl_window.window_surface->format->BitsPerPixel, temp_sdl_window.window_surface->format->Rmask, temp_sdl_window.window_surface->format->Gmask, temp_sdl_window.window_surface->format->Bmask, temp_sdl_window.window_surface->format->Amask);
        if(temp_sdl_window.screen_surface == NULL){
            RR_WriteLog("Error! Screen surface could not be created! SDL_Error: " + std::string(SDL_GetError()) );
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

void RR_WipeSDLWindow(SDL_WindowInfo &info){
    if(info.screen_surface != NULL){
        SDL_FreeSurface(info.screen_surface);
        info.screen_surface = NULL;
    }
    if(info.window_ptr != NULL){
        SDL_DestroyWindow(info.window_ptr);
        info.window_ptr = NULL;
    }
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


//SDL_SetWindowFullscreen( mWindow, SDL_FALSE );

void RR_UpdateWindow(RR_Window &window){
    /*if(SDL_GetTicks64() < g_RR_InputNextRead){
        return;
    }

    g_RR_InputNextRead = SDL_GetTicks64() + 100; // Add some input delay
*/
//  if (SDL_WaitEventTimeout(&event, 10)) {
    if(window.window_index < 0 || window.window_index >= (int)SDL_Windows.size() ){
        RR_WriteLog("Error! Invalid window index for window \""+window.window_name+"\"");
        RR_ForceQuit();
        return;
    }
    SDL_WindowInfo &sdl_window = SDL_Windows.at(window.window_index);

    uint64_t pollEndTick = SDL_GetTicks64() + 1000; // If events are happening over 1 second, uh...
    while( SDL_PollEvent( &sdl_window.window_events ) != 0 ){
        if(SDL_GetTicks64() >= pollEndTick) break;
		//User requests quit
		switch(sdl_window.window_events.type){
            case SDL_QUIT:
    			RR_Quit();
                break;
            case SDL_KEYDOWN:
                switch( sdl_window.window_events.key.keysym.sym ){
                    default:
                    break;
                }
                break;
            case SDL_WINDOWEVENT:
                switch (sdl_window.window_events.window.event) {
                    case SDL_WINDOWEVENT_RESIZED:
                    case SDL_WINDOWEVENT_SIZE_CHANGED:
                        window.win_width = sdl_window.window_events.window.data1;
                        window.win_height = sdl_window.window_events.window.data2;
                        // Update the window surface
                        SDL_Windows.at(window.window_index).window_surface = SDL_GetWindowSurface(SDL_Windows.at(window.window_index).window_ptr);
                        if(SDL_Windows.at(window.window_index).window_surface == NULL){
                            RR_WriteLog("Error! Window surface could not be updated! SDL_Error: " + std::string(SDL_GetError()) );
                            RR_ForceQuit();
                        }
                        break;
                        // ???
//                    case SDL_WINDOWEVENT_CLOSE:
//                        RR_Quit();
//                    break;
                }
                break;
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
	SDL_LockSurface(sdl_window.screen_surface);

    // Copy the pixels over
    uint32_t *surfacePixels = (Uint32 *)sdl_window.screen_surface->pixels;
	for(int y = 0; y < window.screen_height; y++){
		for(int x = 0; x < window.screen_width; x++){
			surfacePixels[(y*sdl_window.screen_surface->w)+x] = window.screen_pixels[(y*window.screen_width)+x].rgba;
		}
	}

    // Unlock the surface so we can display it
	SDL_UnlockSurface(sdl_window.screen_surface);

    if(window.keep_aspect){
        window.pixel_width = round(window.win_width / window.screen_width);
        if(window.pixel_width < 0) window.pixel_width = 1;
        window.pixel_height = round(window.win_height / window.screen_height);
        if(window.pixel_height < 0) window.pixel_height = 1;
        if(window.pixel_width > window.pixel_height){
            window.pixel_width = window.pixel_height;
        }else{
            window.pixel_height = window.pixel_width;
        }
    }else{
        window.pixel_width = (float)window.win_width / (float)window.screen_width;
        if(window.pixel_width < 0) window.pixel_width = 0.01f;
        window.pixel_height = (float)window.win_height / (float)window.screen_height;
        if(window.pixel_height < 0) window.pixel_height = 0.01f;
    }

    // Figure out where to place the screen
	SDL_Rect blitRect = {
        0,0,
        (int)round(window.screen_width * window.pixel_width), 
        (int)round(window.screen_height * window.pixel_height)
    };
    blitRect.x = (window.win_width>>1) - (blitRect.w>>1);
    blitRect.y = (window.win_height>>1) - (blitRect.h>>1);

    // Blit the surface to the screen
	SDL_BlitScaled( sdl_window.screen_surface, NULL, sdl_window.window_surface, &blitRect);

	//Update the surface
	SDL_UpdateWindowSurface( sdl_window.window_ptr );

    g_RR_FrameCount += 1;

    // Calculate the frame time
    g_RR_FrameTime = SDL_GetTicks64() - g_RR_LastFrameTick;

    // Generate the frames per second
    if(SDL_GetTicks64() >= g_RR_FPSNextTick){
        g_RR_FPSNextTick = SDL_GetTicks64() + 1000;
        g_RR_FPS = g_RR_FrameCount;
        g_RR_FrameCount = 0;
        //RR_WriteLog("FPS: "+std::to_string(RR_GetFPS()));        
    }
    // Limit the FPS
    uint64_t fpsdelay = round(1000.0f/(float)(g_RR_MaxFPS)) - g_RR_FrameTime;
    uint64_t fpslimit = SDL_GetTicks64()+fpsdelay;
    while(SDL_GetTicks64() < fpslimit){};

    // Start the ticks after we wait for the FPS limiter
    g_RR_LastFrameTick = SDL_GetTicks64();
};

uint32_t RR_GetFPS(){
    return g_RR_FPS;
};

void RR_SetMaxFPS(uint32_t maxfps){
    g_RR_MaxFPS = maxfps;
    RR_WriteLog("Set max FPS to "+std::to_string(maxfps));
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

void RR_Plot(RR_Window &window, int x, int y, RR_Pixel pixel){
    if(x < 0 || y < 0 || y >= window.screen_height || x >= window.screen_width)
        return;
    RR_Pixel &scr_pixel = window.screen_pixels[(y*window.screen_width) + x];
    if(scr_pixel.depth <= pixel.depth){
        scr_pixel = pixel;
    }
};

void RR_BlitEllipse(RR_Window &window, int x, int y, int w, int h, RR_Pixel pixel){
    int squared_width = w*w;
    int squared_height = h*h;
    int squared_size = (squared_width*squared_height);

    // Draw the ellipse
    for(int i = 0; i < h; i++){
        int sy = (i*i*squared_width);
        for(int e = 0; e < w; e++){
            if( sy + (e*e*squared_height) > squared_size ) continue; // It's ouside the ellipse
            RR_Plot(window, x+e, y+i, pixel);
            RR_Plot(window, x+e, y-i, pixel);
            RR_Plot(window, x-e, y+i, pixel);
            RR_Plot(window, x-e, y-i, pixel);
        }
    }    
};

void RR_BlitLine(RR_Window &window, int x1, int y1, int x2, int y2, RR_Pixel pixel){
    int dx = x2-x1;
    int dy = y2-y1;
    int dx1 = abs(dx);
    int dy1 = abs(dy);
    int px = 2 * dy1 - dx1;
    int py = 2 * dx1 - dy1;
    int x, y, xe, ye, i;
    if(dy1 <= dx1){
        if(dx >= 0){
            x = x1;
            y = y1;
            xe = x2;
        }else{
            x = x2;
            y = y2;
            xe = x1;
        }
        RR_Plot(window,x,y,pixel);

        for(i = 0; x < xe; i++){
            x += 1;
            if(px < 0){
                px += 2*dy1;
            }else{
                if( (dx < 0 && dy < 0) || (dx > 0 && dy > 0)){
                    y += 1;
                }else{
                    y -= 1;
                }
                px += 2*(dy1-dx1);
            }
            RR_Plot(window,x,y,pixel);
        }
    }else{
        if(dy >= 0){
            x = x1;
            y = y1;
            ye = y2;
        }else{
            x = x2;
            y = y2;
            ye = y1;
        }
        RR_Plot(window,x,y,pixel);

        for(i = 0; y < ye; i++){
            y += 1;
            if(py <= 0){
                py += 2*dx1;
            }else{
                if( (dx < 0 && dy < 0) || (dx > 0 && dy > 0)){
                    x += 1;
                }else{
                    x -= 1;
                }
                py += 2*(dx1-dy1);
            }
            RR_Plot(window,x,y,pixel);
        }
    }
};

void RR_BlitTriangle(RR_Window &window, int x1, int y1, int x2, int y2, int x3, int y3, RR_Pixel pixel){
    RR_BlitLine(window,x1,y1,x2,y2,pixel);
    RR_BlitLine(window,x2,y2,x3,y3,pixel);
    RR_BlitLine(window,x1,y1,x3,y3,pixel);
};

void RR_BlitFillTriangle(RR_Window &window, int x1, int y1, int x2, int y2, int x3, int y3, RR_Pixel pixel){
    // TODO:
    // Add triangle rendering
};

void RR_ClearScreen(RR_Window &window, RR_Pixel pixel){
    for(int i = 0; i < (int)window.screen_pixels.size(); i++){
        window.screen_pixels.at(i) = pixel;
    }
};
