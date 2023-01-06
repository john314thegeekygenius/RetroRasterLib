/*
    Multiple Window test for RetroRasterLib
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

    RR_WriteLog("Testing multiple windows");

    // Create a window
	RR_Window local_window = RR_CreateWindow("Test Window", 320, 240, 2, 2);

    // Try to make another window
	RR_Window local_window2 = RR_CreateWindow("Test Window 2", 1080, 720, 1, 1);

    RR_Position2D boxpos = {128,128};

    while(!RR_HasQuit()){
        RR_UpdateInput();
        RR_UpdateWindow(local_window);
        RR_UpdateWindow(local_window2);
        if(RR_CheckWindowClosed(local_window) || RR_CheckWindowClosed(local_window2)){ break; }

        // Test inputs
        RR_Mouse local_mouse = RR_GetMouse(local_window2);

        // Check for arrow keys
        if(RR_GetKey(local_window, RRK_LEFT)){
            boxpos.x -= 1;
        }
        if(RR_GetKey(local_window, RRK_RIGHT)){
            boxpos.x += 1;
        }
        if(RR_GetKey(local_window, RRK_UP)){
            boxpos.y -= 1;
        }
        if(RR_GetKey(local_window, RRK_DOWN)){
            boxpos.y += 1;
        }
  
        // WINDOW 1
        // Clear the screen
        RR_ClearScreen(local_window, RR_Pixel(RR_RGBA(255,0,0), 0));
        RR_BlitRect(local_window, boxpos.x, boxpos.y, 32, 32, RR_Pixel(RR_RGBA(0,255,0), 0));
        // Render the scene
        RR_RasterWindow(local_window);
        // WINDOW 2
        // Clear the screen
        RR_ClearScreen(local_window2, RR_Pixel(RR_RGBA(0,0,255), 0));
        RR_BlitRect(local_window2, local_mouse.x, local_mouse.y, 16, 16, RR_Pixel(RR_RGBA(255,255,0), 0));
        // Render the scene
        RR_RasterWindow(local_window2);
    }

    // Shutdown the library
    RR_DestroyLibrary();

    return 0;
};