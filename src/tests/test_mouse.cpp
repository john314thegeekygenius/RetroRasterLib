/*
    Testing mouse input
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
	RR_Window local_window = RR_CreateWindow("Test Window", 256, 256, 2, 2);

    // Load an image
    RR_Image block_img = RR_LoadImage(local_window, "images/tests/block.bmp");

    bool block_positions[32][32] = {0};

    int CFlash = 0;

    // Wait for the user to quit
    while(!RR_HasQuit()){
        // Get inputs
        RR_UpdateInput();
        RR_UpdateWindow(local_window);
        if(RR_CheckWindowClosed(local_window)){ break; }

        RR_Mouse local_mouse = RR_GetMouse(local_window);

        RR_Position2D chunk_pos = { floorf(local_mouse.x/block_img.width), floorf(local_mouse.y/block_img.height) };

        if(local_mouse.buttons[0].cur_state==1){
            block_positions[(int)chunk_pos.y][(int)chunk_pos.x] = true;
        }
        if(local_mouse.buttons[2].cur_state==0 && local_mouse.buttons[2].last_state==1){
            block_positions[(int)chunk_pos.y][(int)chunk_pos.x] = false;
        }

        // Clear the screen
        RR_ClearScreen(local_window, RR_Pixel(RR_RGBA(0,128,255), 0));

        // Draw images
        for(int y = 0; y < 16; y++)
            for(int x = 0; x < 16; x++)
                if(block_positions[y][x])
                    RR_BlitImage(local_window, block_img, x*block_img.width, y*block_img.height);

        RR_BlitBox(local_window, chunk_pos.x*block_img.width, chunk_pos.y*block_img.height, block_img.width, block_img.height, RR_Pixel(RR_RGBA(CFlash*255,CFlash*255,0), 0));

        CFlash = !CFlash;

        // Render the scene
        RR_RasterWindow(local_window);
    }

    // Shutdown the library
    RR_DestroyLibrary();

    return 0;
};
