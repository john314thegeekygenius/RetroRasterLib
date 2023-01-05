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

void RR_InitLibrary(void) {
	RR_OpenLog();
	RR_WriteLog("[Init] Initializing...");
	// Initialize SDL2
	if(SDL_Init( SDL_INIT_VIDEO | SDL_INIT_EVENTS ) < 0){
		RR_WriteLog("[Init] Error! SDL cound not initialize!\nSDL_Error: " + std::string(SDL_GetError()) );
		rr_quitFlag = true;
		return;
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
