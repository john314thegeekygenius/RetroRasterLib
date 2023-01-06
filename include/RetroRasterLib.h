/*
	RetroRasterLib - a 2D graphics engine
	Written by Jonathan Clevenger
	January 3, 2023
	Copyright (c) 2023 TimeLabs LLC

	Use of this libaray is allowed
	as long as the name and author is 
	refrenced to in the application
	using it, such as:
	
				Made using RetroRasterLib
	www.github.com/john314thegeekygenius/RetroRasterLib

	The following comment must appear in
	all released source code using the library:

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

#pragma once

#define RR_VER_STRING "RR_v.0.0.1"

typedef struct RR_Window_t RR_Window;
typedef struct RR_Image_t RR_Image;

#include <RR_Headers.h>
#include <RR_Input.h>
#include <RR_Window.h>
#include <RR_Graphics.h>
#include <RR_Logger.h>

// Setup the library
// Make sure to call this first
void RR_InitLibrary(void);

// Shutdown the library
// Make sure to call this last
void RR_DestroyLibrary(void);

// Forces RetroRaster to quit everything
// Should only be called if an error occurs
void RR_ForceQuit();

// Set the Quit Flag to true
void RR_Quit();

// Returns the Quit Flag (Have we quit yet)
bool RR_HasQuit();

// "Fixes" the flags (i.e. Quit flag) - Sets the flags to default values
void RR_FixFlags();

