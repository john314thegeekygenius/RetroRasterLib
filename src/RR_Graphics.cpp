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

#include <RetroRasterLib.h>
#include "RR_Window.h"
#include "RR_Graphics.h"

extern std::vector<SDL_WindowInfo> SDL_Windows;


uint32_t RR_RGBA(unsigned char r,unsigned char g,unsigned char b,unsigned char a){
#if SDL_BYTEORDER == SDL_BIG_ENDIAN
	return (r<<24)|(g<<16)|(b<<8)|a;
#else
	return (a<<24)|(r<<16)|(g<<8)|b;
#endif
};


// Primitives

void RR_Plot(RR_Window &window, int x, int y, RR_Pixel pixel){
    if(x < 0 || y < 0 || y >= window.screen_height || x >= window.screen_width)
        return;
    RR_Pixel &scr_pixel = window.screen_pixels[(y*window.screen_width) + x];
    if(scr_pixel.depth <= pixel.depth){
        scr_pixel = pixel;
    }
};

void RR_BlitRect(RR_Window &window, int x, int y, int w, int h, RR_Pixel pixel){
    // Clip the rect
    if(x < 0){
        w += x;
        x = 0;
    }
    if(y < 0){
        h += y;
        y = 0;
    }
    if(x+w >= window.screen_width){
        w += (window.screen_width-(x+w));
    }
    if(y+h >= window.screen_height){
        h += (window.screen_height-(y+h));
    }
    if(x >= window.screen_width) return; // Nothing to draw
    if(y >= window.screen_height) return; // Nothing to draw

    // Draw the rectangle
    for(int i = y; i < y+h; i++){
        for(int e = x; e < x+w; e++){
            RR_Pixel &scr_pixel = window.screen_pixels[(i*window.screen_width) + e];
            if(scr_pixel.depth <= pixel.depth){
                scr_pixel = pixel;
            }
        }
    }
};

void RR_BlitBox(RR_Window &window, int x, int y, int w, int h, RR_Pixel pixel){
    // Draw the box
    for(int e = x; e < x+w; e++){
        RR_Plot(window,e,y,pixel);
        RR_Plot(window,e,y+h-1,pixel);
    }
    for(int i = y; i < y+h; i++){
        RR_Plot(window,x,i,pixel);
        RR_Plot(window,x+w-1,i,pixel);
    }
};

void RR_BlitEllipse(RR_Window &window, int x, int y, int w, int h, RR_Pixel pixel){
    int squared_width = w*w;
    int squared_height = h*h;
    int squared_size = (squared_width*squared_height);

    // Draw the ellipse
    for(int i = 0; i < h; i++){
        int sy = (i*i*squared_width);
        for(int e = 0; e < w; e++){
            if( sy + (e*e*squared_height) > squared_size ) continue; // It's ouside the ellipse
            RR_Plot(window, x+e, y+i, pixel);
            RR_Plot(window, x+e, y-i, pixel);
            RR_Plot(window, x-e, y+i, pixel);
            RR_Plot(window, x-e, y-i, pixel);
        }
    }    
};

void RR_BlitLine(RR_Window &window, int x1, int y1, int x2, int y2, RR_Pixel pixel){
    int dx = x2-x1;
    int dy = y2-y1;
    int dx1 = abs(dx);
    int dy1 = abs(dy);
    int px = 2 * dy1 - dx1;
    int py = 2 * dx1 - dy1;
    int x, y, xe, ye, i;
    if(dy1 <= dx1){
        if(dx >= 0){
            x = x1;
            y = y1;
            xe = x2;
        }else{
            x = x2;
            y = y2;
            xe = x1;
        }
        RR_Plot(window,x,y,pixel);

        for(i = 0; x < xe; i++){
            x += 1;
            if(px < 0){
                px += 2*dy1;
            }else{
                if( (dx < 0 && dy < 0) || (dx > 0 && dy > 0)){
                    y += 1;
                }else{
                    y -= 1;
                }
                px += 2*(dy1-dx1);
            }
            RR_Plot(window,x,y,pixel);
        }
    }else{
        if(dy >= 0){
            x = x1;
            y = y1;
            ye = y2;
        }else{
            x = x2;
            y = y2;
            ye = y1;
        }
        RR_Plot(window,x,y,pixel);

        for(i = 0; y < ye; i++){
            y += 1;
            if(py <= 0){
                py += 2*dx1;
            }else{
                if( (dx < 0 && dy < 0) || (dx > 0 && dy > 0)){
                    x += 1;
                }else{
                    x -= 1;
                }
                py += 2*(dx1-dy1);
            }
            RR_Plot(window,x,y,pixel);
        }
    }
};

void RR_BlitTriangle(RR_Window &window, int x1, int y1, int x2, int y2, int x3, int y3, RR_Pixel pixel){
    RR_BlitLine(window,x1,y1,x2,y2,pixel);
    RR_BlitLine(window,x2,y2,x3,y3,pixel);
    RR_BlitLine(window,x1,y1,x3,y3,pixel);
};

void RR_BlitFillTriangle(RR_Window &window, int x1, int y1, int x2, int y2, int x3, int y3, RR_Pixel pixel){
    // TODO:
    // Add triangle rendering
};

void RR_ClearScreen(RR_Window &window, RR_Pixel pixel){
    for(int i = 0; i < (int)window.screen_pixels.size(); i++){
        window.screen_pixels.at(i) = pixel;
    }
};

void RR_BlitImage(RR_Window &window, RR_Image &image, int x, int y, float sw, float sh){
    // Make sure it's even on the screen first
    if( x+(image.width*sw) < 0 || y+(image.height*sh) < 0 || x >= window.screen_width || y >= window.screen_height)
        return;
    // Blit the image to screen
    int sx = 0;
    int sy = 0;
    int dy = 0;
    int dx = 0;
    for(int by = 0; by < image.height*sh; by++){
        dy = y + by;
        sy = (by/sh);
        for(int bx = 0; bx < image.width*sw; bx++){
            sx = (bx/sw);
            dx = x + bx;
            if(dx < 0 || dy < 0 || dy >= window.screen_height || dx >= window.screen_width)
                continue;
            if(image.pixels.at((sy*image.width)+sx).rgba>>24){
                window.screen_pixels.at((dy*window.screen_width) + dx) = image.pixels.at((sy*image.width)+sx);
            }
        }
    }
};


// Graphics formats

void RR_WriteScreenToFile(RR_Window &window, std::string f_name, float sw, float sh){
    // Figure out what the image format is
    std::string ext;
    if(f_name.length() < 3){
        RR_WriteLog("Warn! Could not deduce format to write screen to - assuming PNG");
        ext = "png";
    }else{
        ext = f_name.substr(f_name.length()-3,3);
    }
    // Make sure it's all lowercase
    for(char &c : ext){ c = tolower(c); }
    if(ext.compare("bmp")==0){
        RR_WriteLog("Writing to BMP file: " + f_name);
        SDL_SaveBMP(SDL_Windows.at(window.window_index).screen_surface, f_name.c_str());
        return;
    }
    if(ext.compare("png")==0){
        RR_WriteLog("Writing to PNG file: " + f_name);
        IMG_SavePNG(SDL_Windows.at(window.window_index).screen_surface, f_name.c_str());
        return;
    }
    if(ext.compare("jpg")==0 || ext.compare("peg")==0){
        RR_WriteLog("Writing to JPEG file: " + f_name);
        IMG_SaveJPG(SDL_Windows.at(window.window_index).screen_surface, f_name.c_str(),100);
        return;
    }
    if(ext.compare("raw")==0){
        RR_WriteLog("Writing to RAW file: " + f_name);
        // TODO:
        // Make this write the raw pixel values to a FILE
        return;
    }
    if(ext.compare("rri")==0){
        RR_WriteLog("Writing to RRI file: " + f_name);
        // TODO:
        // Make this write the pixels to a RetroRasterImage file
        return;
    }
    RR_WriteLog("Error! Invalid format! Not writing image!");
};

uint32_t RR_FixPixel(uint32_t p, const SDL_PixelFormat *format){
    uint8_t r = p>>format->Rshift;
    uint8_t g = p>>format->Gshift;
    uint8_t b = p>>format->Bshift;
    uint8_t a = p>>format->Ashift;
#if SDL_BYTEORDER == SDL_BIG_ENDIAN
	return (r<<24)|(g<<16)|(b<<8)|a;
#else
	return (a<<24)|(r<<16)|(g<<8)|b;
#endif
};

RR_Image RR_LoadImage(RR_Window &window, std::string f_name){
    // Image we will return
    RR_Image temp_img;

    // Clear the image first
    temp_img.width = 0;
    temp_img.height = 0;
    temp_img.pixels.clear();

    // Figure out what the image format is
    std::string ext;
    if(f_name.length() < 3){
        RR_WriteLog("Warn! Could not deduce format to write screen to - assuming PNG");
        ext = "png";
    }else{
        ext = f_name.substr(f_name.length()-3,3);
    }
    // Make sure it's all lowercase
    for(char &c : ext){ c = tolower(c); }

    SDL_Surface *temp_surface = NULL;
    bool LoadSDL = false;

    if(ext.compare("bmp")==0){
        RR_WriteLog("Reading BMP file: " + f_name);
        temp_surface = SDL_LoadBMP_RW(SDL_RWFromFile(f_name.c_str(), "rb"), 1);
        if(temp_surface == NULL){
            RR_WriteLog("Error loading image! SDL_Error:" + std::string(SDL_GetError()));
            return temp_img;
        }
        LoadSDL = true;
    }
    if(ext.compare("png")==0){
        RR_WriteLog("Reading PNG file: " + f_name);
        temp_surface = IMG_LoadPNG_RW(SDL_RWFromFile(f_name.c_str(), "rb"));
        if(temp_surface == NULL){
            RR_WriteLog("Error loading image! SDL_Error:" + std::string(SDL_GetError()));
            return temp_img;
        }
        LoadSDL = true;
    }
    if(ext.compare("jpg")==0 || ext.compare("peg")==0){
        RR_WriteLog("Reading JPEG file: " + f_name);
        temp_surface = IMG_LoadJPG_RW(SDL_RWFromFile(f_name.c_str(), "rb"));
        if(temp_surface == NULL){
            RR_WriteLog("Error loading image! SDL_Error:" + std::string(SDL_GetError()));
            return temp_img;
        }
        LoadSDL = true;
    }
    if(LoadSDL){
        // Fix the surface
        SDL_Surface* formatted_surface = SDL_ConvertSurfaceFormat(temp_surface, SDL_PIXELFORMAT_ARGB8888, 0);
        if(formatted_surface == NULL){
            RR_WriteLog("Error formating surface! SDL_Error:" + std::string(SDL_GetError()));
            return temp_img;
        }
        SDL_FreeSurface(temp_surface);
        temp_surface = NULL;
        // Get the image size
        temp_img.width = formatted_surface->w;
        temp_img.height = formatted_surface->h;
        RR_WriteLog("Image size: "+std::to_string(temp_img.width)+"x"+std::to_string(temp_img.height));
        temp_img.pixels.resize(temp_img.width*temp_img.height);
        SDL_LockSurface(formatted_surface);
        // Copy the pixels over
        uint32_t *surfacePixels = (uint32_t *)formatted_surface->pixels;
        for(int y = 0; y < temp_img.height; y++){
            for(int x = 0; x < temp_img.width; x++){
                temp_img.pixels.at((y*temp_img.width)+x).rgba = surfacePixels[(y*formatted_surface->w)+x];
                temp_img.pixels.at((y*temp_img.width)+x).depth = 0;
            }
        }
        RR_WriteLog("Freeing surface...");
        SDL_UnlockSurface(formatted_surface);
        SDL_FreeSurface(formatted_surface);
        formatted_surface = NULL;
        return temp_img;
    }
    if(ext.compare("raw")==0){
        RR_WriteLog("Reading RAW file: " + f_name);
        RR_WriteLog("Warn! Size can not be determined from a RAW file!");
        RR_WriteLog("Image size will be set to 0");
        // TODO:
        // Make this read the raw pixel values to a FILE
        return temp_img;
    }
    if(ext.compare("rri")==0){
        RR_WriteLog("Reading RRI file: " + f_name);
        // TODO:
        // Make this read the pixels of a RetroRasterImage file
        return temp_img;
    }
    RR_WriteLog("Error! Invalid format! Not reading image!");
    return temp_img;
};

