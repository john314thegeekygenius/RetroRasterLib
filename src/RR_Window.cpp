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
#include "RR_Graphics.h"

std::vector<SDL_WindowInfo> SDL_Windows;

// Pulled from RR_Lib.cpp
extern bool RR_GlobalKeyPresses[256];
extern RR_Mouse RR_GlobalMouse;
extern int RR_GlobalWindowSelected;
extern int RR_GlobalResizeWindow;
extern int RR_GlobalCloseWindow;
extern int RR_GlobalNewWindowWidth;
extern int RR_GlobalNewWindowHeight;
extern int OS_GlobalMouseX;
extern int OS_GlobalMouseY;

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
    // FPS
    local_window.frame_time = 0;
    local_window.fps = 0;
    local_window.frame_count = 0;
    local_window.max_fps = RR_DEFAULT_FPS;
    local_window.next_fps_tick = 0;
    local_window.last_frame_time = 0;
    local_window.current_frame = 0;

	// Create an SDL2 window
	SDL_WindowInfo temp_sdl_window;

    uint32_t SDL_winFlags = SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE;

    //if(!SDL_Windows.size()){
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

        // Find the first window and fix the selected window variable
        if(RR_GlobalWindowSelected == -1){
            RR_GlobalWindowSelected = SDL_GetWindowID(temp_sdl_window.window_ptr);
        }

        SDL_Windows.push_back(temp_sdl_window);

        local_window.window_closed = false;
    /*}else{
        RR_WriteLog("Warn! Failed to create new window!");
        RR_WriteLog("Window support is currently limited to one window!");
        return local_window;

    }*/

    return local_window;
};

void RR_WipeSDLWindow(SDL_WindowInfo &info){
    RR_WriteLog("Destroyed SDL Window!");
    if(info.screen_surface != NULL){
        SDL_FreeSurface(info.screen_surface);
        info.screen_surface = NULL;
    }
    if(info.window_ptr != NULL){
        SDL_DestroyWindow(info.window_ptr);
        info.window_ptr = NULL;
    }
};

bool RR_CheckWindow(RR_Window &window){
    if(window.window_closed){
        RR_WriteLog("Error! Window was closed! Please handle this properly!");
        RR_ForceQuit();
        return true;
    }
    if(window.window_index < 0 || window.window_index >= (int)SDL_Windows.size() ){
        RR_WriteLog("Error! Invalid window index for window \""+window.window_name+"\"");
        RR_ForceQuit();
        return true;
    }
    return false;
};

bool RR_CheckWindowClosed(RR_Window &window){
    return window.window_closed;
};

void RR_DestroyWindow(RR_Window &window){
    if(RR_CheckWindowClosed(window)) return; // Nothing to do (We hope)
    if(RR_CheckWindow(window)) return;
    RR_WipeSDLWindow(SDL_Windows.at(window.window_index));
    // Remove it from the list (BUG)
    //SDL_Windows.erase(SDL_Windows.begin()+window.window_index);
    // Tell the window it can't be used now
    window.window_closed = true;
};


void RR_DestroyWindows(){
    for(long unsigned int i = 0; i < SDL_Windows.size(); i++){
        RR_WipeSDLWindow(SDL_Windows.at(i));
    }
    // Remove all windows
    SDL_Windows.clear();
};

void RR_SetWindowTitle(RR_Window &window, std::string title){
    if(RR_CheckWindow(window)) return;
    SDL_SetWindowTitle(SDL_Windows.at(window.window_index).window_ptr, title.c_str());
};

void RR_SetWindowIcon(RR_Window &window, RR_Image &icon_img){
    if(RR_CheckWindow(window)) return;
    SDL_Surface *icon_surface = NULL;
    icon_surface = SDL_CreateRGBSurfaceWithFormat(0, icon_img.width, icon_img.height, 32, SDL_PIXELFORMAT_ABGR8888);
    if(icon_surface == NULL){
        RR_WriteLog("Warn! Could not create SDL Surface for Icon! SDL_Error: "+std::string(SDL_GetError()));
        return;
    }
    SDL_LockSurface(icon_surface);
    // Copy pixels over
    uint32_t *surfacePixels = (uint32_t *)icon_surface->pixels;
    for(int y = 0; y < icon_img.height; y++){
        for(int x = 0; x < icon_img.width; x++){
            surfacePixels[(y*icon_surface->w)+x] = icon_img.pixels.at((y*icon_img.width)+x).rgba;
        }
    }
    SDL_UnlockSurface(icon_surface);
    SDL_SetWindowIcon(SDL_Windows.at(window.window_index).window_ptr, icon_surface);
};

void RR_SetWindowFullscreen(RR_Window &window, bool set_flag){
    if(RR_CheckWindow(window)) return;
    SDL_SetWindowFullscreen( SDL_Windows.at(window.window_index).window_ptr, (SDL_bool)set_flag );
};

void RR_SetWindowResizeable(RR_Window &window, bool set_flag){
    if(RR_CheckWindow(window)) return;
    SDL_SetWindowResizable( SDL_Windows.at(window.window_index).window_ptr, (SDL_bool)set_flag );
};

void RR_SetOverscanColor(RR_Window &window, RR_Pixel pixel){
    window.overscan_color = pixel.rgba;
};

void RR_SetAspectRatio(RR_Window &window, bool set_flag){
    window.keep_aspect = set_flag;
};


void RR_UpdateWindow(RR_Window &window){
    if(RR_CheckWindowClosed(window)) return;
    if(RR_CheckWindow(window)) return;
    SDL_WindowInfo &sdl_window = SDL_Windows.at(window.window_index);

    // Get the current SDL window id
    int local_win_id = SDL_GetWindowID(sdl_window.window_ptr);

    // Close the window if needed
    if(RR_GlobalCloseWindow == local_win_id){
        RR_WriteLog("Info: Closed window.");
        RR_DestroyWindow(window);
        RR_GlobalCloseWindow = -1;
        return;
    }

    // Update the mouse buttons
    for(int i = 0; i < RR_MAX_MOUSE_BUTTONS; i++)
        window.window_mouse.buttons[i].last_state = window.window_mouse.buttons[i].cur_state;  

    if(RR_GlobalWindowSelected == local_win_id){
        if(RR_GlobalMouse.window_id == local_win_id){
            // Update the mouse
            window.window_mouse = RR_GlobalMouse;
            // Code to use global x pos
            /*
            int win_x, win_y;
            SDL_GetWindowPosition(sdl_window.window_ptr, &win_x, &win_y);

            window.window_mouse.real_x = OS_GlobalMouseX - win_x;
            window.window_mouse.real_y = OS_GlobalMouseY - win_y;
            */
            
            window.window_mouse.x = window.window_mouse.real_x / window.pixel_width;
            window.window_mouse.y = window.window_mouse.real_y / window.pixel_height;
        }

        // Update the keys
        for(int i = 0; i < 256; i++)
            window.keycodes[i] = RR_GlobalKeyPresses[i];
    }else{
        // Clear all the keys
        for(int i = 0; i < 256; i++)
            window.keycodes[i] = 0;
    }

    // Update the window surface if needed
    if(RR_GlobalResizeWindow == local_win_id){
        window.win_width = RR_GlobalNewWindowWidth;
        window.win_height = RR_GlobalNewWindowHeight;
        // Update the window surface
        sdl_window.window_surface = SDL_GetWindowSurface(sdl_window.window_ptr);
        if(sdl_window.window_surface == NULL){
            RR_WriteLog("Error! Window surface could not be updated! SDL_Error: " + std::string(SDL_GetError()) );
            RR_ForceQuit();
        }
        RR_GlobalResizeWindow = -1;
    }
};

void RR_RasterWindow(RR_Window &window){
    if(RR_CheckWindow(window)) return;
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

    window.frame_count += 1;
    window.current_frame += 1;

    // Calculate the frame time
    window.frame_time = SDL_GetTicks64() - window.last_frame_time;

    // Generate the frames per second
    if(SDL_GetTicks64() >= window.next_fps_tick){
        window.next_fps_tick = SDL_GetTicks64() + 1000;
        window.fps = window.frame_count;
        window.frame_count = 0;
        //RR_WriteLog("FPS: "+std::to_string(RR_GetFPS()));        
    }
    // Limit the FPS
    uint64_t fpsdelay = round(1000.0f/(float)(window.max_fps)) - window.frame_time;
    uint64_t fpslimit = SDL_GetTicks64()+fpsdelay;
    while(SDL_GetTicks64() < fpslimit){};

    // Start the ticks after we wait for the FPS limiter
    window.last_frame_time = SDL_GetTicks64();
};

uint64_t RR_GetFrameTick(RR_Window &window){
    return window.current_frame;
};

float RR_ElapsedTime(RR_Window &window){
    // TODO:
    // Make this return the correct value
    /*
    // INIT
    auto tp1 = std::chrono::system_clock::now();
    auto tp2 = std::chrono::system_clock::now();

    // IN LOOP
    // Handle Timing
    tp2 = std::chrono::system_clock::now();
    std::chrono::duration<float> elapsedTime = tp2 - tp1;
    tp1 = tp2;
    float fElapsedTime = elapsedTime.count();
    */
    return 0.05f;//window.frame_time/1000.0f;
};

uint32_t RR_GetFPS(RR_Window &window){
    return window.fps;
};

void RR_SetMaxFPS(RR_Window &window, uint32_t maxfps){
    window.max_fps = maxfps;
};

// Input

bool RR_GetKey(RR_Window &window, RR_KeyCode key){
    if(key < 0 || key >= 256) {
        RR_WriteLog("Warn! Bad keycode! " + std::to_string(key));
        return false;
    }
    return window.keycodes[key];
};

RR_Mouse RR_GetMouse(RR_Window &window){
    return window.window_mouse;
};

int RR_GetMouseX(RR_Window &window){
    return window.window_mouse.x;
};

int RR_GetMouseY(RR_Window &window){
    return window.window_mouse.y;
};

int RR_GetMouseRealX(RR_Window &window){
    return window.window_mouse.real_x;
};

int RR_GetMouseRealY(RR_Window &window){
    return window.window_mouse.real_y;
};
int RR_GetMouseWheelX(RR_Window &window){
    return window.window_mouse.wheel_x;
};

int RR_GetMouseWheelY(RR_Window &window){
    return window.window_mouse.wheel_y;
};

RR_MButton RR_GetMouseButton(RR_Window &window, int id){
    if(id < 0 || id >= 10){
        RR_WriteLog("Error! Bad mouse button id! ");
        return RR_MButton(); // Return invalid mouse button????
    }
    return window.window_mouse.buttons[id];
};
