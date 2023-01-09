/*
    Displaying more complex graphics test 
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
	RR_Window local_window = RR_CreateWindow("Test Window", 256, 240, 4, 4);

    // Load an image
    RR_Image block_img = RR_LoadImage(local_window, "images/tests/block.bmp");
    RR_Image smile_img = RR_LoadImage(local_window, "images/tests/smile.png");

    float roate_value = 0.0f;

    // Wait for the user to quit
    while(!RR_HasQuit()){
        // Get inputs
        RR_UpdateInput();
        RR_UpdateWindow(local_window);
        if(RR_CheckWindowClosed(local_window)){ break; }

        if(RR_GetKey(local_window, RRK_Z)){
            roate_value -= 2.0f * RR_ElapsedTime(local_window);
        }
        if(RR_GetKey(local_window, RRK_X)){
            roate_value += 2.0f * RR_ElapsedTime(local_window);
        }

        // Clear the screen
        RR_ClearScreen(local_window, RR_Pixel(RR_RGBA(0,64,128), 0));

        RR_Matrix3x3_t matFinal, matFinalInv;
        matFinal.identity();
        matFinal.translate(RR_Position2D(-(block_img.width/2),-(block_img.height/2)));
        matFinal.rotate(roate_value);
        matFinal.translate(RR_Position2D(local_window.screen_width/2, local_window.screen_height-32));
        RR_MatrixInvert(matFinal,matFinalInv);

        // Draw an ellipse in the background
        RR_BlitEllipse(local_window, local_window.screen_width/2,local_window.screen_height/2,128,64,RR_Pixel(RR_RGBA(0,255,0),0));

        // Draw the image
        RR_SetAlphaBlend(local_window, RR_BLEND_PIXELS | RR_BLEND_TRANSPARENT);
        RR_BlitImage(local_window, block_img, matFinal, matFinalInv);

        matFinal.identity();
        matFinal.translate(RR_Position2D(-(smile_img.width/2),-(smile_img.height/2)));
        matFinal.scale(RR_Position2D(2,2));
        matFinal.rotate(roate_value);
        matFinal.translate(RR_Position2D(64, local_window.screen_height/2));
        RR_MatrixInvert(matFinal,matFinalInv);

        RR_BlitImage(local_window, smile_img, matFinal, matFinalInv);

        matFinal.identity();
        matFinal.translate(RR_Position2D(-(smile_img.width/2),-(smile_img.height/2)));
        matFinal.scale(RR_Position2D(2,2));
        matFinal.rotate(-roate_value);
        matFinal.translate(RR_Position2D(local_window.screen_width-64, local_window.screen_height/2));
        RR_MatrixInvert(matFinal,matFinalInv);

        RR_BlitImage(local_window, smile_img, matFinal, matFinalInv);

        // Draw a transparent rectangle over everything
        RR_BlitRect(local_window, 32,32,local_window.screen_width-64,local_window.screen_height-64,RR_Pixel(RR_RGBA(255,0,0,128),1));
        RR_SetAlphaBlend(local_window, RR_BLEND_NONE);

        for(int recurse = 0; recurse < 3; recurse++){
            RR_Image scr_img = RR_GetImage(local_window, 0, 0, local_window.screen_width, local_window.screen_height);

            matFinal.identity();
            matFinal.translate(RR_Position2D(-(scr_img.width/2),-(scr_img.height/2)));
            matFinal.scale(RR_Position2D(0.5,0.5));
            matFinal.translate(RR_Position2D(local_window.screen_width/2, scr_img.height/2));
            RR_MatrixInvert(matFinal,matFinalInv);
            RR_BlitImage(local_window, scr_img, matFinal, matFinalInv);
        }

        // Render the scene
        RR_RasterWindow(local_window);
    }

    // Shutdown the library
    RR_DestroyLibrary();

    return 0;
};
