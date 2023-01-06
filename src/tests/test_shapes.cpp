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

    int RX = 0;
    // Wait for the user to quit
    while(!RR_HasQuit()){
        // Get inputs
        RR_UpdateWindow(local_window);


        // Create a clear pixel (blue)
        RR_Pixel clear_pixel = { RR_RGBA(0,0,255), 0};

        // Clear the screen
        RR_ClearScreen(local_window, clear_pixel);

        // Draw 5 rectangles
        RR_BlitRect(local_window, -32, 64, 128, 64, RR_Pixel(RR_RGBA(255,0,0), 0));
        RR_BlitRect(local_window, 320-32, 64, 128, 64, RR_Pixel(RR_RGBA(255,255,0), 0));
        RR_BlitRect(local_window, 128, -32, 128, 64, RR_Pixel(RR_RGBA(255,0,255), 0));
        RR_BlitRect(local_window, 128, 240-32, 128, 64, RR_Pixel(RR_RGBA(255,255,255), 0));

        RR_BlitRect(local_window, 160, 120, 1, 1, RR_Pixel(RR_RGBA(0,0,0), 0));

        // Draw some lines
        for(int i = 0; i < 320; i++){
            RR_BlitLine(local_window, 160, 0, i, 240, RR_Pixel(RR_RGBA((i*0.8f),255-(i*0.8f),0),0));
        }

        // Draw a solid triangle
        RR_BlitFillTriangle(local_window, 50,50,300,100,180,220, RR_Pixel(RR_RGBA(255,0,255),0));

        // Draw an ellipse
        RR_BlitEllipse(local_window, 160, 120, 64, 32, RR_Pixel(RR_RGBA(255,0,0),0));

        // Draw a triangle
        RR_BlitTriangle(local_window, 50, 200, 100, 50, 280, 80, RR_Pixel(RR_RGBA(255,255,255),0));

        // Draw a moving rect
        RR_BlitRect(local_window, RX, 120, 16, 16, RR_Pixel(RR_RGBA(128,128,128), 0));
        RX += 1;
        RX %= 320;
        // Render the scene
        RR_RasterWindow(local_window);
    }

    // Write the last frame to a file
    RR_WriteScreenToFile(local_window, "test_result.png",1 ,1);
    RR_WriteScreenToFile(local_window, "test_result_05.png",0.5 ,0.5);

    // Shutdown the library
    RR_DestroyLibrary();

    return 0;
};
