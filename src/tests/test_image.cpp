/*
    Displaying and loading images
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
	RR_Window local_window = RR_CreateWindow("Test Window", 480, 480, 2, 2);

    // Load some images
    RR_Image backdrop = RR_LoadImage(local_window, "images/tests/background.jpg");
    RR_Image block = RR_LoadImage(local_window, "images/tests/block.bmp");
    RR_Image smile = RR_LoadImage(local_window, "images/tests/smile.png");
    RR_Image blob = RR_LoadImage(local_window, "images/tests/blob.bmp");
    RR_Image logo32 = RR_LoadImage(local_window, "logo32.png");

    // Set the window image
    RR_SetWindowIcon(local_window, logo32);

    // Clear the screen
    RR_ClearScreen(local_window, RR_Pixel(RR_RGBA(0,0,255), 0));

    // Draw images
    RR_BlitImage(local_window, backdrop, -20, 0, 2, 3);
    RR_BlitImage(local_window, block, 16, 32);
    RR_BlitImage(local_window, smile, 240-32, 240-32, 3, 3);
    RR_BlitImage(local_window, blob, 200, 40, 4, 4);
    

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
