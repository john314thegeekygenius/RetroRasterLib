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
std::vector<RR_Controler> RR_GlobalControlers; 

int OS_GlobalMouseX = 0;
int OS_GlobalMouseY = 0;


void RR_InitLibrary(void) {
#ifdef _WIN64 || _WIN32
	SDL_SetMainReady();
#endif
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
		for(int i = 0; i < SDL_NumJoysticks(); i++){
			RR_AddControler(i);
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
	// Close the joysticks
	for(int i = 0; i < (int)SDL_GlobalControlers.size(); i ++){
		RR_RemoveControler(i);
	}
	SDL_GlobalControlers.clear();

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
	int joystick_id;

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
			case SDL_JOYAXISMOTION:
				joystick_id = SDL_GlobalEvents.jaxis.which;
				if(SDL_GlobalEvents.jaxis.axis==-1){
					RR_WriteLog("Warn! Controler sent invalid axis!");
				}else{
					// Joystick X1
					if(SDL_GlobalEvents.jaxis.axis==0){
						RR_GlobalControlers.at(joystick_id).real_x[0] = SDL_GlobalEvents.jaxis.value;
						RR_GlobalControlers.at(joystick_id).x[0] = (float)RR_GlobalControlers.at(joystick_id).real_x[0] / (float)0x7FFF;
					}
					if(SDL_GlobalEvents.jaxis.axis==1){
						RR_GlobalControlers.at(joystick_id).real_y[0] = SDL_GlobalEvents.jaxis.value;
						RR_GlobalControlers.at(joystick_id).y[0] = (float)RR_GlobalControlers.at(joystick_id).real_y[0] / (float)0x7FFF;
					}
					// Joystick Y1
					if(SDL_GlobalEvents.jaxis.axis==3){
						RR_GlobalControlers.at(joystick_id).real_x[1] = SDL_GlobalEvents.jaxis.value;
						RR_GlobalControlers.at(joystick_id).x[1] = (float)RR_GlobalControlers.at(joystick_id).real_x[1] / (float)0x7FFF;
					}
					if(SDL_GlobalEvents.jaxis.axis==4){
						RR_GlobalControlers.at(joystick_id).real_y[1] = SDL_GlobalEvents.jaxis.value;
						RR_GlobalControlers.at(joystick_id).y[1] = (float)RR_GlobalControlers.at(joystick_id).real_y[1] / (float)0x7FFF;
					}
					// Joystick Triggers
					if(SDL_GlobalEvents.jaxis.axis==2){
						RR_GlobalControlers.at(joystick_id).real_trigger_value[0] = SDL_GlobalEvents.jaxis.value;
						RR_GlobalControlers.at(joystick_id).trigger_value[0] = (float)(RR_GlobalControlers.at(joystick_id).real_trigger_value[0]+0x8000) / (float)0xFFFF;
//						RR_GlobalControlers.at(joystick_id).trigger_value[0] = (float)RR_GlobalControlers.at(joystick_id).real_trigger_value[0] / (float)0x7FFF;
					}
					if(SDL_GlobalEvents.jaxis.axis==SDL_CONTROLLER_AXIS_TRIGGERRIGHT){
						RR_GlobalControlers.at(joystick_id).real_trigger_value[1] = SDL_GlobalEvents.jaxis.value;
						RR_GlobalControlers.at(joystick_id).trigger_value[1] = (float)(RR_GlobalControlers.at(joystick_id).real_trigger_value[1]+0x8000) / (float)0xFFFF;
//						RR_GlobalControlers.at(joystick_id).trigger_value[1] = (float)RR_GlobalControlers.at(joystick_id).real_trigger_value[1] / (float)0x7FFF;
					}
				}
			break;
			case SDL_JOYBUTTONDOWN:
				joystick_id = SDL_GlobalEvents.jbutton.which;
				if(SDL_GlobalEvents.jbutton.button < 0 || SDL_GlobalEvents.jbutton.button >= RR_MAX_CONTROLER_BUTTONS){
					RR_WriteLog("Warn! Controler sent invalid button id!");
				}else{
					RR_GlobalControlers.at(joystick_id).buttons[SDL_GlobalEvents.jbutton.button] = true;
				}
				break;
			case SDL_JOYBUTTONUP:
				joystick_id = SDL_GlobalEvents.jbutton.which;
				if(SDL_GlobalEvents.jbutton.button < 0 || SDL_GlobalEvents.jbutton.button >= RR_MAX_CONTROLER_BUTTONS){
					RR_WriteLog("Warn! Controler sent invalid button id!");
				}else{
					RR_GlobalControlers.at(joystick_id).buttons[SDL_GlobalEvents.jbutton.button] = false;
				}
				break;
			case SDL_JOYHATMOTION:
				joystick_id = SDL_GlobalEvents.jhat.which;
				switch(SDL_GlobalEvents.jhat.value){
					case SDL_HAT_CENTERED:
						RR_GlobalControlers.at(joystick_id).hat_x = 0;
						RR_GlobalControlers.at(joystick_id).hat_y = 0;
						break;
					case SDL_HAT_LEFTUP:
						RR_GlobalControlers.at(joystick_id).hat_x = -1;
						RR_GlobalControlers.at(joystick_id).hat_y = -1;
						break;
					case SDL_HAT_UP:
						RR_GlobalControlers.at(joystick_id).hat_x = 0;
						RR_GlobalControlers.at(joystick_id).hat_y = -1;
						break;
					case SDL_HAT_RIGHTUP:
						RR_GlobalControlers.at(joystick_id).hat_x = 1;
						RR_GlobalControlers.at(joystick_id).hat_y = -1;
						break;
					case SDL_HAT_RIGHT:
						RR_GlobalControlers.at(joystick_id).hat_x = 1;
						RR_GlobalControlers.at(joystick_id).hat_y = 0;
						break;
					case SDL_HAT_RIGHTDOWN:
						RR_GlobalControlers.at(joystick_id).hat_x = 1;
						RR_GlobalControlers.at(joystick_id).hat_y = 1;
						break;
					case SDL_HAT_DOWN:
						RR_GlobalControlers.at(joystick_id).hat_x = 0;
						RR_GlobalControlers.at(joystick_id).hat_y = 1;
						break;
					case SDL_HAT_LEFTDOWN:
						RR_GlobalControlers.at(joystick_id).hat_x = -1;
						RR_GlobalControlers.at(joystick_id).hat_y = 1;
						break;
					case SDL_HAT_LEFT:
						RR_GlobalControlers.at(joystick_id).hat_x = -1;
						RR_GlobalControlers.at(joystick_id).hat_y = 0;
						break;
				}
				break;
			case SDL_JOYDEVICEADDED:
				// Add the controler
				joystick_id = SDL_GlobalEvents.jdevice.which;
				RR_AddControler(joystick_id);
				break;
			case SDL_JOYDEVICEREMOVED:
				// Remove the controler
				joystick_id = SDL_GlobalEvents.jdevice.which;
				RR_RemoveControler(joystick_id);
				break;
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

void RR_AddControler(int joystick_id){
	// Check if the controler was already plugged in??
	RR_WriteLog("Added controler: "+std::to_string(joystick_id));
	for(RR_Controler &controler : RR_GlobalControlers){
		if(controler.controler_id == joystick_id){
			controler.available = true;
			return;
		}
	}

	if(!( SDL_GlobalControlers.size() && SDL_GlobalControlers.back() == NULL )){
		SDL_GlobalControlers.push_back(NULL);
	}
	SDL_GlobalControlers.back() = SDL_JoystickOpen( joystick_id );
	if( SDL_GlobalControlers.back() == NULL ){
		RR_WriteLog("Warn: Unable to use controller! SDL Error:" + std::string(SDL_GetError()));
		//SDL_GlobalControlers.erase(SDL_GlobalControlers.end());
	}else{
		RR_GlobalControlers.push_back(RR_Controler());
		RR_GlobalControlers.back().available = true;
		RR_GlobalControlers.back().num_axis = SDL_JoystickNumAxes(SDL_GlobalControlers.back());
		RR_GlobalControlers.back().num_buttons = SDL_JoystickNumButtons(SDL_GlobalControlers.back());
		RR_GlobalControlers.back().can_rumble = SDL_JoystickHasRumble(SDL_GlobalControlers.back());

		RR_GlobalControlers.back().controler_id = joystick_id;
	}

};

void RR_RemoveControler(int joystick_id){
	if(joystick_id < 0 || joystick_id >= (int)SDL_GlobalControlers.size()){
		RR_WriteLog("Error! Bad joystick ID to remove!");
		return;
	}
	RR_GlobalControlers.at(joystick_id).available = false;
	if(SDL_GlobalControlers.at(joystick_id) != NULL){
		SDL_JoystickClose(SDL_GlobalControlers.at(joystick_id));
		SDL_GlobalControlers.at(joystick_id) = NULL;
	}
};


int RR_GetControlerCount(){
	return RR_GlobalControlers.size();
};

RR_Controler RR_GetControler(int controler_id){
	if(controler_id < 0 || controler_id >= (int)RR_GlobalControlers.size()){
		RR_WriteLog("Error! Invalid controler id: "+std::to_string(controler_id));
		//RR_ForceQuit();
		return RR_Controler(); // Returns invalid controler?? 
	}
	return RR_GlobalControlers.at(controler_id);
};

std::string RR_GetControlerName(int controler_id){
	if(controler_id < 0 || controler_id >= (int)RR_GlobalControlers.size()){
		RR_WriteLog("Error! Invalid controler id: "+std::to_string(controler_id));
		return ""; // Returns invalid controler name 
	}
	return std::string(SDL_JoystickName(SDL_GlobalControlers.at(controler_id)));
};

void RR_RumbleControler(RR_Controler &controler, float l_rumble, float r_rumble, int len_ms){
	int controler_id = controler.controler_id;
	if(controler_id < 0 || controler_id >= (int)RR_GlobalControlers.size()){
		RR_WriteLog("Error! Invalid controler id: "+std::to_string(controler_id));
		return;
	}
	if(!RR_GlobalControlers.at(controler_id).can_rumble){
		RR_WriteLog("Warn! Controler does not support rumble!");
		return;
	}
	SDL_JoystickRumble(SDL_GlobalControlers.at(controler_id), l_rumble*0xFFFF, r_rumble*0xFFFF, len_ms);
};

