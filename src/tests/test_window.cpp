/*
    Window test for RetroRasterLib
    Written by Jonathan Clevenger

	=================================================================
	Uses RetroRasterLib written by Jonathan Clevenger
	Find it here: www.github.com/john314thegeekygenius/RetroRasterLib
	=================================================================

*/

#include <RetroRasterLib.h>


int main(int argc, char *args[]){
    // Create a window
	RR_Window local_window = RR_CreateWindow("Test Window", 320, 240, 1, 1);

    RR_WriteLog("This is a test of the window");

    return 0;
};