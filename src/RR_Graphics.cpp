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

*/

#include <RetroRasterLib.h>

std::vector<SDL_WindowInfo> SDL_Windows;

RR_Window RR_CreateWindow(std::string name, int screen_width, int screen_height, int pixel_res_x, int pixel_res_y){
	RR_Window local_window;
	local_window.win_width = screen_width;
	local_window.win_height = screen_height;
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

    uint32_t SDL_winFlags = SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE | SDL_WINDOW_BORDERLESS;

	temp_sdl_window.window_ptr = SDL_CreateWindow(local_window.window_name.c_str(), SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, local_window.win_width, local_window.win_height, SDL_winFlags );

//	SDL_Surface *window_surface;
//	SDL_Event *window_events;
    SDL_Windows.push_back(temp_sdl_window);

    return local_window;
};

void RR_DestroyWindow(RR_Window &window){
    if(window.window_index < 0 || window.window_index >= (int)SDL_Windows.size() ){
        RR_WriteLog("Error! Invalid window index for window \""+window.window_name+"\"");
        RR_ForceQuit();
        return;
    }
    RR_WipeSDLWindow(SDL_Windows.at(window.window_index));
};


void RR_DestroyWindows(){
    for(long unsigned int i = 0; i < SDL_Windows.size(); i++){
        RR_WipeSDLWindow(SDL_Windows.at(i));
    }
};

void RR_WipeSDLWindow(SDL_WindowInfo &info){
    if(info.window_ptr != NULL){
        SDL_DestroyWindow(info.window_ptr);
        info.window_ptr = NULL;
    }
};

void RR_RasterWindow(RR_Window &window){

};

uint32_t RR_RGBA(unsigned char r,unsigned char g,unsigned char b,unsigned char a){
	return (a<<24)|(r<<16)|(g<<8)|b;
};
