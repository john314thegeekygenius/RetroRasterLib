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

    // Load an image
    RR_Image smile = RR_LoadImage(local_window, "images/tests/smile.png");

    RR_Position2D smile_pos = { 0, 0 };
    RR_Position2D smile_scale = { 3, 3 };

    // Wait for the user to quit
    while(!RR_HasQuit()){
        // Get inputs
        RR_UpdateWindow(local_window);

        // Check for arrow keys
        if(RR_GetKey(local_window, RRK_LEFT)){
            smile_pos.x -= 1;
        }
        if(RR_GetKey(local_window, RRK_RIGHT)){
            smile_pos.x += 1;
        }
        if(RR_GetKey(local_window, RRK_UP)){
            smile_pos.y -= 1;
        }
        if(RR_GetKey(local_window, RRK_DOWN)){
            smile_pos.y += 1;
        }

        // Check for WASD
        if(RR_GetKey(local_window, RRK_A)){
            smile_scale.x -= 0.1;
        }
        if(RR_GetKey(local_window, RRK_D)){
            smile_scale.x += 0.1;
        }
        if(RR_GetKey(local_window, RRK_W)){
            smile_scale.y -= 0.1;
        }
        if(RR_GetKey(local_window, RRK_S)){
            smile_scale.y += 0.1;
        }

        // Clear the screen
        RR_ClearScreen(local_window, RR_Pixel(RR_RGBA(0,0,255), 0));

        // Draw images
        RR_BlitImage(local_window, smile, smile_pos.x, smile_pos.y, smile_scale.x, smile_scale.y);

        // Render the scene
        RR_RasterWindow(local_window);
    }

    // Shutdown the library
    RR_DestroyLibrary();

    return 0;
};
