/*
    Displaying simple shapes test 
    for RetroRasterLib
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

    // Create a window
	RR_Window local_window = RR_CreateWindow("Test Window", 320, 240, 4, 4);

    // Create a clear pixel (blue)
    RR_Pixel clear_pixel = { RR_RGBA(0,0,255), 0};

    // Clear the screen
    RR_ClearScreen(local_window, clear_pixel);

    // Draw 5 rectangles
    RR_BlitRect(local_window, -32, 64, 128, 64, RR_Pixel(RR_RGBA(255,0,0), 0));

    // Wait for the user to quit
    while(!RR_HasQuit()){
        // Get inputs
        RR_UpdateWindow(local_window);
        // Render the scene
        RR_RasterWindow(local_window);
    }

    // Shutdown the library
    RR_DestroyLibrary();

    return 0;
};