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
    // Originaly from: https://www.avrfreaks.net/sites/default/files/triangles.c 
    // However it appears that this link no longer works.
    // Code taken from OLCGameEngine: https://www.github.com/onelonecoder
    // and tweaked because I couldn't find better triangle drawing code
    // TODO:
    // Make this better
    
    int t1x, t2x, minx, maxx, t1xp, t2xp;
    int signx1, signx2;
    int e1, e2;
    bool changed1 = false;
    bool changed2 = false;

    auto blitline = [&](int sx, int ex, int ny) { 
        for (int i = sx; i <= ex; i++) 
            RR_Plot(window, i, ny, pixel); 
    };

    // Sort vertices
    if (y1 > y2) { 
        std::swap(y1, y2); 
        std::swap(x1, x2); 
    }
    if (y1 > y3) { 
        std::swap(y1, y3); 
        std::swap(x1, x3); 
    }
    if (y2 > y3) { 
        std::swap(y2, y3); 
        std::swap(x2, x3); 
    }

    t1x = t2x = x1; 
    int y = y1;   // Starting points
    int dx1 = (int)(x2 - x1); 

    if (dx1<0) { 
        dx1 = -dx1; // Would ^80000000 be faster??
        signx1 = -1; 
    } else {
        signx1 = 1;
    }
    int dy1 = (int)(y2 - y1);
    int dx2 = (int)(x3 - x1); 

    signx2 = 1;
    
    if (dx2<0) { 
        dx2 = -dx2; 
        signx2 = -1; 
    }
    
    int dy2 = (int)(y3 - y1);

    if (dy1 > dx1) {   // swap values
        std::swap(dx1, dy1);
        changed1 = true;
    }
    if (dy2 > dx2) {   // swap values
        std::swap(dy2, dx2);
        changed2 = true;
    }

    e2 = (int)(dx2 >> 1);

    //
    // WARNING: MESSY CODE INCOMING!!!
    //

    // Flat top, just process the second half
    if (y1 == y2) {
        goto next;
    }

    e1 = (int)(dx1 >> 1);

    for (int i = 0; i < dx1;) {
        t1xp = t2xp = 0;
        if (t1x<t2x) { 
            minx = t1x; 
            maxx = t2x; 
        }else { 
            minx = t2x; 
            maxx = t1x; 
        }
        // process first line until y value is about to change
        while (i<dx1) {
            i++;
            e1 += dy1;
            while (e1 >= dx1) {
                e1 -= dx1;
                if (changed1) {
                    t1xp = signx1;//t1x += signx1;
                }else {
                    // Skip to bottom half
                    goto next1;
                }
            }
            if (changed1) { 
                break; 
            }else { 
                t1x += signx1; 
            }
        }
        // Move line
    next1:
        // process second line until y value is about to change
        while (1) {
            e2 += dy2;
            while (e2 >= dx2) {
                e2 -= dx2;
                if (changed2) {
                    t2xp = signx2;//t2x += signx2;
                }else {
                    goto next2;
                }
            }
            if (changed2) {
                break;
            } else {
                t2x += signx2;
            }
        }
    next2:
        if (minx>t1x) {
            minx = t1x;
        }
        if (minx>t2x) {
            minx = t2x;
        }
        if (maxx<t1x) {
            maxx = t1x; 
        }
        if (maxx<t2x) {
            maxx = t2x;
        }

        // Draw line from min to max points found on the y
        blitline(minx, maxx, y);

        // Now increase y
        if (!changed1) {
            t1x += signx1;
        }

        t1x += t1xp;
        if (!changed2) {
            t2x += signx2;
        }

        t2x += t2xp;
        y += 1;

        if (y == y2) {
            break;
        }
    }
next:
    // Second half
    dx1 = (int)(x3 - x2); 
    if (dx1<0) { 
        dx1 = -dx1; 
        signx1 = -1; 
    } else {
        signx1 = 1;
    }

    dy1 = (int)(y3 - y2);
    t1x = x2;

    if (dy1 > dx1) {   // swap values
        std::swap(dy1, dx1);
        changed1 = true;
    }
    else {
        changed1 = false;
    }

    e1 = (int)(dx1 >> 1);

    for (int i = 0; i <= dx1; i++) {
        t1xp = t2xp = 0;
        if (t1x<t2x) { 
            minx = t1x; 
            maxx = t2x; 
        } else { 
            minx = t2x; 
            maxx = t1x; 
        }
        // process first line until y value is about to change
        while (i<dx1) {
            e1 += dy1;
            while (e1 >= dx1) {
                e1 -= dx1;
                if (changed1) { 
                    t1xp = signx1; 
                    //t1x += signx1;
                    break; 
                } else {
                    goto next3;
                }
            }
            if (changed1) {
                break;
            } else {
                t1x += signx1;
            }
            if (i<dx1) {
                i++;
            }
        }
    next3:
        // process second line until y value is about to change
        while (t2x != x3) {
            e2 += dy2;
            while (e2 >= dx2) {
                e2 -= dx2;
                if (changed2) {
                    t2xp = signx2;
                } else {
                    goto next4;
                }
            }
            if (changed2) {
                break;
            } else {
                t2x += signx2;
            }
        }
    next4:
        if (minx>t1x) {
            minx = t1x;
        }
        if (minx>t2x) {
            minx = t2x;
        }
        if (maxx<t1x) {
            maxx = t1x; 
        }
        if (maxx<t2x) {
            maxx = t2x;
        }
        // Draw line from min to max points found on the y
        blitline(minx, maxx, y); 
        if (!changed1) {
            t1x += signx1;
        }
        t1x += t1xp;
        if (!changed2) {
            t2x += signx2;
        }
        t2x += t2xp;
        y += 1;
        if (y>y3) {
            return;
        }
    }
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
    if(ext.compare("pcx")==0){
        RR_WriteLog("Reading PCX file: " + f_name);
        temp_surface = IMG_LoadPCX_RW(SDL_RWFromFile(f_name.c_str(), "rb"));
        if(temp_surface == NULL){
            RR_WriteLog("Error loading image! SDL_Error:" + std::string(SDL_GetError()));
            return temp_img;
        }
        LoadSDL = true;
    }
    if(ext.compare("tga")==0){
        RR_WriteLog("Reading TGA file: " + f_name);
        temp_surface = IMG_LoadTGA_RW(SDL_RWFromFile(f_name.c_str(), "rb"));
        if(temp_surface == NULL){
            RR_WriteLog("Error loading image! SDL_Error:" + std::string(SDL_GetError()));
            return temp_img;
        }
        LoadSDL = true;
    }
    if(ext.compare("ico")==0){
        RR_WriteLog("Reading ICO file: " + f_name);
        temp_surface = IMG_LoadICO_RW(SDL_RWFromFile(f_name.c_str(), "rb"));
        if(temp_surface == NULL){
            RR_WriteLog("Error loading image! SDL_Error:" + std::string(SDL_GetError()));
            return temp_img;
        }
        LoadSDL = true;
    }
    if(ext.compare("gif")==0){
        RR_WriteLog("Reading GIF file: " + f_name);
        temp_surface = IMG_LoadGIF_RW(SDL_RWFromFile(f_name.c_str(), "rb"));
        if(temp_surface == NULL){
            RR_WriteLog("Error loading image! SDL_Error:" + std::string(SDL_GetError()));
            return temp_img;
        }
        LoadSDL = true;
    }
    if(ext.compare("svg")==0){
        RR_WriteLog("Reading SVG file: " + f_name);
        temp_surface = IMG_LoadSVG_RW(SDL_RWFromFile(f_name.c_str(), "rb"));
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
    if(ext.compare("rri")==0){
        // Read a RetroRasterImage file
        RR_WriteLog("Reading RRI file: " + f_name);
        RR_RRIHeader header;
        std::ifstream image_file;

        image_file.open(f_name);
        if(!image_file.is_open()){
            RR_WriteLog("Error loading image!");
            return temp_img;
        }
        // Read the header
        image_file.read((char*)&header.hash,sizeof(char)*3);
        image_file.read((char*)&header.width,sizeof(uint32_t));
        image_file.read((char*)&header.height,sizeof(uint32_t));
        image_file.read((char*)&header.name_len,sizeof(uint32_t));
        image_file.read((char*)header.name_string.data(),sizeof(char)*header.name_len);
        // Get the image size
        temp_img.width = header.width;
        temp_img.height = header.height;
        RR_WriteLog("Image size: "+std::to_string(temp_img.width)+"x"+std::to_string(temp_img.height));
        temp_img.pixels.resize(temp_img.width*temp_img.height);
        // Read the pixels
        for(int i = 0; i < temp_img.width*temp_img.height; i++){
            uint32_t pixel_data;
            image_file.read((char*)&pixel_data,sizeof(uint32_t));
            temp_img.pixels.at(i).rgba = pixel_data;
            temp_img.pixels.at(i).depth = 0;
        }

        return temp_img;
    }
    RR_WriteLog("Error! Invalid format! Not reading image!");
    return temp_img;
};

