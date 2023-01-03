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

void __attribute__ ((constructor)) RR_InitLibrary(void) {
	RR_OpenLog();
	RR_WriteLog("Initializing...");
	// Initialize SDL2
	if(SDL_Init( SDL_INIT_VIDEO | SDL_INIT_EVENTS ) < 0){
//		RR_WriteLog("\cError! \dSDL cound not initialize!\n\cSDL_Error: \cf" + std::string(SDL_GetError()) );
	}
	RR_WriteLog("Done");
};

void __attribute__ ((destructor)) RR_DestroyLibrary(void) {
	RR_WriteLog("Shutting Down...");
	RR_ForceQuit();
	RR_CloseLog();
};

void RR_ForceQuit(){
	RR_DestroyWindows();
};
