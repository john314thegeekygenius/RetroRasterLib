/*
    Testing Joystick / Controlers
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
#include <iostream>

int main(int argc, char *args[]){

    // Setup the library
    RR_InitLibrary();

    int controler_count = RR_GetControlerCount();
    int joy_selected = -1;

    // Get the user to select controler
    if(controler_count > 1){
        std::cout << "Please select controler to test:" << std::endl;
        for(int i = 0; i < controler_count; i++){
            std::cout << (int)(i) << ") " << RR_GetControlerName(i) << std::endl;
        }
        std::cout << "Select:" << std::flush;
        std::cin >> joy_selected;
        if(joy_selected < 0 || joy_selected >= controler_count){
            std::cout << "Invalid controler!" << std::endl;
            // Shutdown the library
            RR_DestroyLibrary();
            return 0;
        }
    } else if(controler_count==1){
        joy_selected = 0;
    } else{
        std::cout << "No controlers found! Please connect one and try again." << std::endl;
        // Shutdown the library
        RR_DestroyLibrary();
        return 0;
    }

    // Create a window
	RR_Window local_window = RR_CreateWindow("Test Window", 320, 320, 2, 2);

    // Wait for the user to quit
    while(!RR_HasQuit()){
        // Get inputs
        RR_UpdateInput();
        RR_UpdateWindow(local_window);
        if(RR_CheckWindowClosed(local_window)){ break; }

        RR_Controler local_controler = RR_GetControler(joy_selected);

        if(RR_GetKey(local_window,RRK_SPACE)){
            RR_RumbleControler(local_controler,1.0f,1.0f,500);
        }

        // Clear the screen
        RR_ClearScreen(local_window, RR_Pixel(RR_RGBA(255,255,255), 0));

        // Draw some UI

        // Joystick Left
        RR_BlitEllipse(local_window, 32, 32, 32, 32, RR_Pixel(RR_RGBA(64,64,64), 0));
        RR_BlitEllipse(local_window, 32+(local_controler.x[0]*16), 32+(local_controler.y[0]*16), 16, 16, RR_Pixel(RR_RGBA(175,175,175), 0));
        // Joystick Right
        RR_BlitEllipse(local_window, 128, 32, 32, 32, RR_Pixel(RR_RGBA(64,64,64), 0));
        RR_BlitEllipse(local_window, 128+(local_controler.x[1]*16), 32+(local_controler.y[1]*16), 16, 16, RR_Pixel(RR_RGBA(175,175,175), 0));
        // Left Trigger
        RR_BlitRect(local_window, 32, 128, 32, 64, RR_Pixel(RR_RGBA(64,64,64), 0));
        RR_BlitRect(local_window, 32, 128+(local_controler.trigger_value[0]*48), 32, 16, RR_Pixel(RR_RGBA(175,175,175), 0));
        // Right Trigger
        RR_BlitRect(local_window, 96, 128, 32, 64, RR_Pixel(RR_RGBA(64,64,64), 0));
        RR_BlitRect(local_window, 96, 128+(local_controler.trigger_value[1]*48), 32, 16, RR_Pixel(RR_RGBA(175,175,175), 0));
        // Buttons
        for(int i = 0; i < local_controler.num_buttons; i++){
            if(local_controler.buttons[i])
                RR_BlitRect(local_window, 8+(i*12), 256, 8, 8, RR_Pixel(RR_RGBA(175,175,175), 0));
            else
                RR_BlitRect(local_window, 8+(i*12), 256, 8, 8, RR_Pixel(RR_RGBA(64,64,64), 0));
        }
        // Hat 
        RR_BlitRect(local_window, 256-16, 48, 48, 48, RR_Pixel(RR_RGBA(64,64,64), 0));
        RR_BlitRect(local_window, 256+(local_controler.hat_x*16), 64+(local_controler.hat_y*16), 16, 16, RR_Pixel(RR_RGBA(175,175,175), 0));

        // Render the scene
        RR_RasterWindow(local_window);
    }

    // Shutdown the library
    RR_DestroyLibrary();

    return 0;
};
