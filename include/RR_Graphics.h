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

typedef struct RR_Image_t {
    int width; // Width of the image
    int height; // Height of the image
    std::vector<RR_Pixel> pixels; // List of pixels to be displayed
}RR_Image;

typedef struct RR_Position2D_t {
    float x;
    float y;
}RR_Position2D;

typedef struct RR_RRIHeader_t {
	char hash[3];
	uint32_t width;
	uint32_t height;
	uint32_t name_len;
	std::string name_string;
}RR_RRIHeader;


/*
r -> Red value
g -> Green value
b -> Blue value
a -> Alpha value (optional, default 255)
*/
uint32_t RR_RGBA(unsigned char r,unsigned char g,unsigned char b,unsigned char a = 0xFF);

/*
Sets a pixel value on the screen
window -> window to change pixel on
x      -> horizontal position of pixel
y      -> vertical position of pixel
pixel  -> pixel data to plot
*/
void RR_Plot(RR_Window &window, int x, int y, RR_Pixel pixel);

/*
Draws a rectangle on the screen
window  -> window to render to
x       -> position horizontaly on the screen
y       -> position vertically on the screen
w       -> width of rectangle
h       -> height of rectangle
pixel   -> pixel info to render
*/
void RR_BlitRect(RR_Window &window, int x, int y, int w, int h, RR_Pixel pixel);

/*
Draws a box on the screen
window  -> window to render to
x       -> position horizontaly on the screen
y       -> position vertically on the screen
w       -> width of rectangle
h       -> height of rectangle
pixel   -> pixel info to render
*/
void RR_BlitBox(RR_Window &window, int x, int y, int w, int h, RR_Pixel pixel);

/*
Draws an ellipse on the screen
window  -> window to render to
x       -> centered position horizontaly on the screen
y       -> centered position vertically on the screen
w       -> width of ellipse
h       -> height of ellipse
pixel   -> pixel info to render
*/
void RR_BlitEllipse(RR_Window &window, int x, int y, int w, int h, RR_Pixel pixel);


/*
Draws a line on the screen
window  -> window to render to
x1      -> point A position horizontaly on the screen
y1      -> point A position vertically on the screen
x2      -> point B position horizontaly on the screen
y2      -> point B position vertically on the screen
pixel   -> pixel info to render
*/
void RR_BlitLine(RR_Window &window, int x1, int y1, int x2, int y2, RR_Pixel pixel);

/*
Draws a trianlge on the screen
window  -> window to render to
x[1-3]  -> point positions horizontaly on the screen
y[1-3]  -> point positions vertically on the screen
pixel   -> pixel info to render
*/
void RR_BlitTriangle(RR_Window &window, int x1, int y1, int x2, int y2, int x3, int y3, RR_Pixel pixel);


/*
Draws a filled trianlge on the screen
window  -> window to render to
x[1-3]  -> point positions horizontaly on the screen
y[1-3]  -> point positions vertically on the screen
pixel   -> pixel info to render
*/
void RR_BlitFillTriangle(RR_Window &window, int x1, int y1, int x2, int y2, int x3, int y3, RR_Pixel pixel);

/*
Clears the screen to a set pixel
window -> window to clear screen of
pixel  -> pixel data to clear with
*/
void RR_ClearScreen(RR_Window &window, RR_Pixel pixel);

/*
Writes the current pixel buffer to a PNG file scaled
window -> window to get pixels from
f_name -> name of image to be opened (valid are bmp, png, jpg/jpeg, raw, rri)
sw     -> horizontal scale factor
sh     -> vertical scale factor
*/
void RR_WriteScreenToFile(RR_Window &window, std::string f_name, float sw = 1, float sh = 1);

/*
Opens and loads an image
window -> window for pixel format
f_name -> name of image to be opened (valid are bmp, png, jpg/jpeg, raw, rri)
*/
RR_Image RR_LoadImage(RR_Window &window, std::string f_name);

/*
Draws an image on the screen
x  -> horizontal position on the screen
y  -> vertical position on the screen
*/
void RR_BlitImage(RR_Window &window, RR_Image &image, int x, int y, float sw = 1, float sh = 1);

