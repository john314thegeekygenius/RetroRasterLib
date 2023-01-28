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
#if (defined(_WIN64) || defined(_WIN32))
#include <windows.h>
#endif

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

    std::string escape_sequence = "";
    #if defined(__linux__) || (defined(__CYGWIN__) && !defined(_WIN32))
        // Linux or Cygwin, so make the colors use linux format
        escape_sequence = "\033";
    #endif

    #if (defined(_WIN64) || defined(_WIN32))
        // Microsoft Windows, so use windows console color format
        HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
        DWORD dwMode = 0;
        GetConsoleMode(hConsole, &dwMode);
        dwMode |= ENABLE_VIRTUAL_TERMINAL_PROCESSING;
        SetConsoleMode(hConsole, dwMode);
        escape_sequence = "\x1b";
    #endif


    int write_color = -1;
    bool color_changed = false;
    int write_bold = 0;
    int write_flash = 0;

    // Convert the formated string to a valid string
    for(int i = 0; i < (int)log_str.length(); i++){
        switch(log_str.at(i)){
            case '\r':
                // Reset the color
                color_changed = true;
            break;
            case '\b':
                // set to bold
                if(write_bold == 0)
                    write_bold = 1;
                if(write_bold == 2)
                    write_bold = -1;
            break;
            case '\f':
                // set to flash
                if(write_flash == 0)
                    write_flash = 1;
                if(write_flash == 2)
                    write_flash = -1;
            break;
            case '\v':
                // Set the color
                i++;
                if(i>=(int)log_str.length()){
                    RR_WriteLog("Error writing string with color! Improper formating: \n::::" + log_str);
                    return; // Don't write anything???
                }
                // Find the color
                for(int cidx = 0; cidx < 16; cidx ++){
                    if(tolower(log_str[i]) == "0123456789abcdef"[cidx]){
                        write_color = cidx;
                        break;
                    }
                }
                color_changed = true;
            break;
            default:
                // It's a normal character
                output_string.push_back(log_str[i]);
                raw_string.push_back(log_str[i]);
            break;
        }
        if(write_bold == 1){
            output_string += escape_sequence;
            output_string += "[1m";
            write_bold = 2;
        }
        if(write_bold == -1){
            output_string += escape_sequence;
            output_string += "[0m";
            write_bold = 0;
            color_changed = true;
        }
        if(write_flash == 1){
            output_string += escape_sequence;
            output_string += "[5m";
            write_flash = 2;
        }
        if(write_flash == -1){
            output_string += escape_sequence;
            output_string += "[0m";
            // Re color the text
            write_flash = 0;
            color_changed = true;
        }

        if(color_changed){
            color_changed = false;
            output_string += escape_sequence;
            output_string += "[";
            if(write_color < 8){
                //output_string += "0;";
                output_string += std::to_string(30+write_color);
            }else{
                //output_string += "1;";
                output_string += std::to_string(82+write_color);
            }
            output_string += "m";
        }
    }
    // Reset the color after every line
    output_string += escape_sequence;

    output_string += "[0m";

    if(RR_LogFile.is_open()){
        RR_LogFile << raw_string << std::endl;
    }
    std::cout << output_string << std::endl;
};

void RR_CloseLog(){
    if(RR_LogFile.is_open()){
        RR_LogFile.close();
    }
};
