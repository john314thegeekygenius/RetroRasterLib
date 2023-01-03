#pragma once

#include <RR_Headers.h>

//
// Opens a log file
//
void RR_OpenLog();

//
// log_str  ->  String to write to logfile (or console)
// 
// Log strings support color for consoles that support color.
// Colors are set by commands:
// '\cX' Set the color to X. X is a hex value between 0-F
// '\d' Reset the color to the default color 
// 
void RR_WriteLog(std::string log_str);

//
// Closes the log file
//
void RR_CloseLog();

