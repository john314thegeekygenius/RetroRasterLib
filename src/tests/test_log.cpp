/*
    Log test for RetroRasterLib
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

    // Test writing to a log file
    RR_WriteLog("Testing writing to log file!");
    // Test color text
    RR_WriteLog("\v0BLACK \v1RED \v2GREEN \v3YELLOW \v4BLUE \v5PURPLE \v6CYAN \v7GREY");
    RR_WriteLog("\v8DGREY \v9LRED \vaLGREEN \vbLYELLOW \vcLBLUE \vdPINK \veLCYAN \vfWHITE");
    RR_WriteLog("===========================================================");
    RR_WriteLog("\rDEFAULT \fBLINKING\f \f\v2BLINKY GREEN\f NORMAL GREEN");
    RR_WriteLog("\v3YELLOW \bBOLD\b YELLOW");

    // Shutdown the library
    RR_DestroyLibrary();

    return 0;
};