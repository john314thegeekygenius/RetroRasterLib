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

#include <iostream>
#include <iomanip>
#include <ctime>
#include <sstream>
#include <fstream>
#include <filesystem>

#include <RetroRasterLib.h>

// Log file to write to
std::ofstream RR_LogFile;

std::string RR_GetDateString(){
    auto t = std::time(nullptr);
    auto tm = *std::localtime(&t);

    std::ostringstream oss;
    oss << std::put_time(&tm, "%d-%m-%Y_%H-%M-%S");
    auto str = oss.str();
    return str;
};

void RR_OpenLog(){
    // Make a "logs" directory
    std::filesystem::create_directories("logs");
    RR_LogFile.open(std::string("logs/rr_log_")+RR_GetDateString()+".log");
    if(!RR_LogFile.is_open()){
        RR_WriteLog("Error! Could not open output file.");
        return;
    }
    RR_WriteLog("Opened log file");
};

void RR_WriteLog(std::string log_str){
    // String to write to console
    std::string output_string = "[RetroRasterLib] ";
    // String to write to file
    std::string raw_string = output_string;

#if defined(__linux__) || (defined(__CYGWIN__) && !defined(_WIN32))
    // Linux or Cygwin, so make the colors use linux format
#endif
#if defined(_WIN64) || defined(_WIN32)
    // Microsoft Windows, so use windows console color format
#endif

    if(RR_LogFile.is_open()){
    }
    std::cout << output_string << log_str << std::endl;

};

void RR_CloseLog(){
    if(RR_LogFile.is_open()){
        RR_LogFile.close();
    }
};
