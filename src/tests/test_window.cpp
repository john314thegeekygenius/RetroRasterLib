/*
    Window test for RetroRasterLib
    Written by Jonathan Clevenger

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


int main(int argc, char *args[]){

    // Setup the library
    RR_InitLibrary();

    RR_WriteLog("Testing window");

    // Create a window
	RR_Window local_window = RR_CreateWindow("Test Window", 320, 240, 2, 2);

    // Try to make another window
	RR_Window local_window2 = RR_CreateWindow("Test Window 2", 1080, 720, 1, 1);

    while(!RR_HasQuit()){
        RR_UpdateInput();
        RR_UpdateWindow(local_window);
        RR_UpdateWindow(local_window2);
        if(RR_CheckWindowClosed(local_window) && RR_CheckWindowClosed(local_window2)){ break; }
    }

    // Remove the windows
    //RR_DestroyWindow(local_window);
    //RR_DestroyWindow(local_window2);

    // Fix the library
    RR_FixFlags();

    // Try to make another (valid) window
	RR_Window local_window3 = RR_CreateWindow("Test Window 3", 64, 64, 4, 4);

    while(!RR_HasQuit()){
        RR_UpdateInput();
        RR_UpdateWindow(local_window3);
        if(RR_CheckWindowClosed(local_window3)){ break; }
    }

    // Shutdown the library
    RR_DestroyLibrary();

    return 0;
};