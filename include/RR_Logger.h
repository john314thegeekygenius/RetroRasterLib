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

#include <RR_Headers.h>

//
// Returns a date and time string
// Format:
// D-M-T_H-M-S
std::string RR_GetDateString();

//
// Opens a log file
//
void RR_OpenLog();

// If you are looking at this comment in code, 
// use single slashes '\' not double '\\'
/*
log_str  ->  String to write to logfile (or console)
-----------------------------------------------------------
Log strings support color for consoles that support color.
Colors are set by commands:
'\\r' Reset the color to the default color 
'\\vX' Set the color to X. X is a hex value between 0-F
'\\f' Make the text blink
'\\b' Make the text bold
 */
void RR_WriteLog(std::string log_str);

//
// Closes the log file
//
void RR_CloseLog();

