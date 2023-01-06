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

// Flag to indicate if we quit
bool rr_quitFlag = false;

std::vector<SDL_Joystick *> SDL_GlobalControlers;

int RR_GlobalWindowSelected = -1;
int RR_GlobalResizeWindow = -1;
int RR_GlobalCloseWindow = -1;
int RR_GlobalNewWindowWidth = 0;
int RR_GlobalNewWindowHeight = 0;
bool RR_GlobalKeyPresses[256];
RR_Mouse RR_GlobalMouse;
int OS_GlobalMouseX = 0;
int OS_GlobalMouseY = 0;

void RR_InitLibrary(void) {
	RR_OpenLog();
	RR_WriteLog("[Init] Initializing...");
	// Initialize SDL2
	if(SDL_Init( SDL_INIT_VIDEO | SDL_INIT_EVENTS | SDL_INIT_JOYSTICK ) < 0){
		RR_WriteLog("[Init] Error! SDL cound not initialize!\nSDL_Error: " + std::string(SDL_GetError()) );
		rr_quitFlag = true;
		return;
	}
	//Check for joysticks
	if( SDL_NumJoysticks() < 1 ) {
		RR_WriteLog("Info: No controller found.");
	} else {
		RR_WriteLog("Info: "+std::to_string(SDL_NumJoysticks())+" controllers found.");
		// Load the joysticks
		SDL_GlobalControlers.push_back(NULL);
		for(int i = 0; i < SDL_NumJoysticks(); i++){
			SDL_GlobalControlers.back() = SDL_JoystickOpen( i );
			if( SDL_GlobalControlers.back() == NULL ){
				RR_WriteLog("Warn: Unable to use controller! SDL Error:" + std::string(SDL_GetError()));
			}else{
				SDL_GlobalControlers.push_back(NULL);
			}
		}
	}


	RR_WriteLog("[Init] Loaded SDL2 sucessfully!");
	if (SDL_BYTEORDER == SDL_BIG_ENDIAN){
	RR_WriteLog("[Init] Using Big Endian format");		
	}else{
	RR_WriteLog("[Init] Using Little Endian format");
	}
	RR_WriteLog("[Init] Done");
	// Fix any flags
	RR_FixFlags();
};

//__attribute__ ((destructor)) 
void RR_DestroyLibrary(void) {
	RR_WriteLog("Shutting Down...");
	// Force everything to quit
	RR_ForceQuit();
	// Close the log
	RR_CloseLog();
    // Quit SDL2 subsystems
    SDL_Quit();
	// Incase something forgot
	RR_Quit();
};

void RR_ForceQuit(){
	RR_DestroyWindows();
	rr_quitFlag = true;
};

void RR_Quit(){
	rr_quitFlag = true;
};

bool RR_HasQuit(){
	return rr_quitFlag;
};

void RR_FixFlags(){
	rr_quitFlag = false;
};

void RR_UpdateInput(){

//  if (SDL_WaitEventTimeout(&event, 10)) {

    uint32_t sdl_key;
    int button_id;

	SDL_Event SDL_GlobalEvents;

	// May cause slow downs!
	SDL_GetGlobalMouseState(&OS_GlobalMouseX, &OS_GlobalMouseY);
	
    while( SDL_PollEvent( &SDL_GlobalEvents ) != 0 ){
		//User requests quit
		switch(SDL_GlobalEvents.type){
            case SDL_QUIT:
				RR_WriteLog("Quit!");
				RR_Quit();
                break;
            // Keyboard
            case SDL_KEYDOWN:
                // Convert SDL2 Keycodes to RRL Keycodes
                sdl_key = (uint32_t)SDL_GlobalEvents.key.keysym.sym;
                for(int i = 0; i < 256; i++){
                    if(SDL_KeyMapper[i] == sdl_key){
                        RR_GlobalKeyPresses[i] = true;
                    }
                }
                break;
            case SDL_KEYUP:
                // Convert SDL2 Keycodes to RRL Keycodes
                sdl_key = (uint32_t)SDL_GlobalEvents.key.keysym.sym;
                for(int i = 0; i < 256; i++){
                    if(SDL_KeyMapper[i] == sdl_key){
                        RR_GlobalKeyPresses[i] = false;
                    }
                }
                break;
            // Mouse
            case SDL_MOUSEMOTION:
                // Get the mouse position
				RR_GlobalMouse.real_x = SDL_GlobalEvents.motion.x;
				RR_GlobalMouse.real_y = SDL_GlobalEvents.motion.y;
				RR_GlobalMouse.window_id = SDL_GlobalEvents.motion.windowID;
                break;
            case SDL_MOUSEBUTTONDOWN:
            case SDL_MOUSEBUTTONUP:
                // Get mouse buttons
                button_id = SDL_GlobalEvents.button.button-1;
                RR_GlobalMouse.buttons[button_id].clicks = SDL_GlobalEvents.button.clicks;
                RR_GlobalMouse.buttons[button_id].cur_state = SDL_GlobalEvents.button.state;
                RR_GlobalMouse.buttons[button_id].x = SDL_GlobalEvents.button.x;
                RR_GlobalMouse.buttons[button_id].y = SDL_GlobalEvents.button.y;
				RR_GlobalMouse.window_id = SDL_GlobalEvents.button.windowID;
                break;
            case SDL_MOUSEWHEEL:
                RR_GlobalMouse.wheel_x = SDL_GlobalEvents.wheel.x;
                RR_GlobalMouse.wheel_y = SDL_GlobalEvents.wheel.y;
                RR_GlobalMouse.wheel_dir = SDL_GlobalEvents.wheel.direction;
				RR_GlobalMouse.window_id = SDL_GlobalEvents.wheel.windowID;
                break;

			// TODO:
            // Controler

            // Window
            case SDL_WINDOWEVENT:
                switch (SDL_GlobalEvents.window.event) {
					case SDL_WINDOWEVENT_RESTORED:
					case SDL_WINDOWEVENT_FOCUS_GAINED:
					case SDL_WINDOWEVENT_SHOWN:
					case SDL_WINDOWEVENT_MOVED:
						RR_GlobalWindowSelected = SDL_GlobalEvents.window.windowID;
						break;
                    case SDL_WINDOWEVENT_RESIZED:
                    case SDL_WINDOWEVENT_SIZE_CHANGED:
						RR_GlobalWindowSelected = SDL_GlobalEvents.window.windowID;
						RR_GlobalResizeWindow = SDL_GlobalEvents.window.windowID;
						RR_GlobalNewWindowWidth = SDL_GlobalEvents.window.data1;
						RR_GlobalNewWindowHeight = SDL_GlobalEvents.window.data2;
                        break;
                    case SDL_WINDOWEVENT_CLOSE:
                        RR_GlobalCloseWindow = SDL_GlobalEvents.window.windowID;
                    break;
                }
                break;
        }
    }
};


uint32_t RR_SwapByteOrder(uint32_t b){
    return ((b&0xFF)<<24) | (((b>>8)&0xFF)<<16) | (((b>>16)&0xFF)<<8) | ((b>>24)&0xFF);
};

uint16_t RR_SwapByteOrder(uint16_t b){
    return (((b>>16)&0xFF)<<8) | ((b>>24)&0xFF);
};

