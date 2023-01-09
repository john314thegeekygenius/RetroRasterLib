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

// Pre-define some structures
typedef struct RR_Window_t RR_Window;
typedef struct RR_Pixel_t RR_Pixel;
typedef uint32_t RR_Color; // Uses ARGB format: 0xAARRGGBB

typedef enum {
	RR_BLEND_NONE 			= 0,
	RR_BLEND_TRANSPARENT 	= 1,
	RR_BLEND_PIXELS 		= 2,
}RR_BlendType;

typedef struct RR_Pixel_t {
    RR_Color rgba;  // Color to be displayed
    uint32_t depth = 0; // Depth of pixel
	void blend(RR_Pixel &p){
		int ba = ((rgba>>24) + (p.rgba>>24))>>1; // Alpha
		int br = (((rgba>>16)&0xFF) + ((p.rgba>>16)&0xFF))>>1; // Red
		int bg = (((rgba>>8)&0xFF) + ((p.rgba>>8)&0xFF))>>1; // Green
		int bb = ((rgba&0xFF) + (p.rgba&0xFF))>>1; // Blue
		rgba = (ba<<24)|(br<<16)|(bg<<8)|bb;
	};
}RR_Pixel;


typedef struct RR_Image_t {
    int width; // Width of the image
    int height; // Height of the image
    std::vector<RR_Pixel> pixels; // List of pixels to be displayed
	RR_Pixel sample(int x, int y){
		if(x<0 || y<0 || x>=width ||y>=height) return RR_Pixel(0,0);
		return pixels.at((y*width)+x);
	};
	RR_Pixel sample(float x, float y){
		if(x<0.0f || y<0.0f || x>1.0f ||y>1.0f) return RR_Pixel(0,0);
		return pixels.at((int)((y*height*width)+(x*width)));
	};

}RR_Image;


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
Sets the alpha blend type
window -> window to affect
blend  -> blend type to use
*/
void RR_SetAlphaBlend(RR_Window &window, uint32_t blend);


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
Draws a bezier line on the screen
window  -> window to render to
x1      -> start point horizontaly on the screen
y1      -> start point vertically on the screen
cx1     -> control point 1 horizontaly
cy1     -> control point 1 vertically
cx2     -> control point 2 horizontaly
cy2     -> control point 2 vertically
x2      -> end point horizontaly on the screen
y2      -> end point vertically on the screen
pixel   -> pixel info to render
*/
void RR_BlitBezier(RR_Window &window, int x1, int y1, int cx1, int cy1, int cx2, int cy2, int x2, int y2, RR_Pixel pixel);

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
sw -> optional scale width
sh -> optional scale height
*/
void RR_BlitImage(RR_Window &window, RR_Image &image, int x, int y, float sw = 1, float sh = 1);


// Fancy graphics stuffs


typedef struct RR_Position2D_t {
    float x;
    float y;
}RR_Position2D;

typedef float RR_Angle; // in radians

typedef struct RR_Font_t {
	std::string name; // ?????
	std::vector<RR_Position2D> anchor_points;
	std::vector<RR_Position2D> control_points;
}RR_Font;


// Make a dummy delare
typedef struct RR_Matrix3x3_t RR_Matrix3x3;

/* 
Setup an identity matrix
mat -> matrix to set
*/
void RR_MatrixSetIdentity(RR_Matrix3x3_t &mat);

/* 
Setup a translation matrix
mat    -> matrix to set
offset -> 2d position to translate to
*/
void RR_MatrixTranslate(RR_Matrix3x3_t &mat, RR_Position2D offset);

/* 
Setup a rotation matrix
mat   -> matrix to set
theta -> angle to rotate to (in radians)
*/
void RR_MatrixRotate(RR_Matrix3x3_t &mat, RR_Angle theta);

/* 
Setup a translation matrix
mat  -> matrix to set
size -> 2d size to scale with
*/
void RR_MatrixScale(RR_Matrix3x3_t &mat, RR_Position2D size);

/* 
Setup a translation matrix
mat   -> matrix to set
shift -> 2d amount to sheer with
*/
void RR_MatrixSheer(RR_Matrix3x3_t &mat, RR_Position2D shift);

/*
Multiply the matrices
result -> matrix to receive multiplied matrices
matA   -> matrix A to multiply by
matB   -> matrix B to multiply by
*/
void RR_MatrixMultiply(RR_Matrix3x3_t &result, RR_Matrix3x3_t &matA, RR_Matrix3x3_t &matB);

/*
Returns a point modified by the matrix
mat -> matrix to modify point
in  -> 2d position to modify
out -> 2d position to receive modified point
*/
void RR_MatrixPointForward(RR_Matrix3x3_t&mat, RR_Position2D &in, RR_Position2D &out);


/*
Returns an inverted matrix
in  -> matrix in
out -> matrix out
*/
void RR_MatrixInvert(RR_Matrix3x3 &in, RR_Matrix3x3 &out);


// Handle matrix
typedef struct RR_Matrix3x3_t {
	float m[3][3];

// Will crash the library
//	RR_Matrix3x3_t(){
//		identity();
//	};

	void self(RR_Matrix3x3_t &out){
		memcpy(&out.m, &m, sizeof(m));
	};

	void identity(){
		RR_Matrix3x3_t a;
		RR_MatrixSetIdentity(a);
		memcpy(&m, &a.m, sizeof(m));
	};

	void translate(RR_Position2D pos){
		RR_Matrix3x3_t a,b,c;
		memcpy(&b.m, &m, sizeof(m));
		RR_MatrixTranslate(a, pos);
		RR_MatrixMultiply(c, a, b);
		memcpy(&m, &c.m, sizeof(m));
	};

	void rotate(RR_Angle theta){
		RR_Matrix3x3_t a,b,c;
		memcpy(&b.m, &m, sizeof(m));
		RR_MatrixRotate(a, theta);
		RR_MatrixMultiply(c, a, b);
		memcpy(&m, &c.m, sizeof(m));
	};

	void scale(RR_Position2D size){
		RR_Matrix3x3_t a,b,c;
		memcpy(&b.m, &m, sizeof(m));
		RR_MatrixScale(a, size);
		RR_MatrixMultiply(c, a, b);
		memcpy(&m, &c.m, sizeof(m));
	};
	
	void sheer(RR_Position2D dist){
		RR_Matrix3x3_t a,b,c;
		memcpy(&b.m, &m, sizeof(m));
		RR_MatrixSheer(a, dist);
		RR_MatrixMultiply(c, a, b);
		memcpy(&m, &c.m, sizeof(m));
	};
}RR_Matrix3x3;


/*
Draws an image on the screen with matrix modificator
x      -> horizontal position on the screen
y      -> vertical position on the screen
mat    -> 3x3 matrix to modify image with
matInv -> inverse of mat
*/
void RR_BlitImage(RR_Window &window, RR_Image &image, RR_Matrix3x3_t &mat, RR_Matrix3x3 &matInv);

/*
Gets a portion of the screen as an image
x -> horizontal position on the screen
y -> vertical position on the screen
w -> width of pixels to grab
h -> height of pixels to grab
*/
RR_Image RR_GetImage(RR_Window &window, int x, int y, int w, int h);

/*
Loads a font file (TTF / RetroRasterFont)
f_name  -> font file name
*/
RR_Font RR_LoadFont(std::string f_name);

