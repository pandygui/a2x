/*
    Copyright 2010 Alex Margarit

    This file is part of a2x-framework.

    a2x-framework is free software: you can redistribute it and/or modify
    it under the terms of the GNU Lesser General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    a2x-framework is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU Lesser General Public License for more details.

    You should have received a copy of the GNU Lesser General Public License
    along with a2x-framework.  If not, see <http://www.gnu.org/licenses/>.
*/

/*
    Many thanks:

    - to Orkie for the Wiz framebuffer direction code
*/

#include "a2x_pack_screen.v.h"

APixel* a_screen__pixels = NULL;
int a_screen__width = 0;
int a_screen__height = 0;

APixel* a_screen__savedPixels = NULL;
static int g_savedWidth = 0;
static int g_savedHeight = 0;

static ASprite* g_spriteTarget = NULL;

static void displayVolume(void)
{
    #if A_PLATFORM_GP2X || A_PLATFORM_WIZ
        if(a_settings_getBool("sound.on")) {
            if(a_time_getMilis() - a__volumeAdjust > A_MILIS_VOLUME) {
                return;
            }

            a_pixel_setBlend(A_PIXEL_PLAIN);

            a_pixel_setPixel(0);
            a_draw_rectangle(0, 181, a__volumeMax / A_VOLUME_STEP + 5, 197);

            a_pixel_setRGB(255, 156, 107);
            a_draw_hline(0, a__volumeMax / A_VOLUME_STEP + 4 + 1, 180);
            a_draw_hline(0, a__volumeMax / A_VOLUME_STEP + 4 + 1, 183 + 14);
            a_draw_vline(a__volumeMax / A_VOLUME_STEP + 4 + 1, 181, 183 + 14);

            a_pixel_setRGB(255, 84, 0);
            a_draw_rectangle(0, 186, a__volume / A_VOLUME_STEP, 192);
        }
    #endif
}

void a_screen__init(void)
{
    if(!a_settings_getBool("video.window")) {
        return;
    }

    a_screen__width = a_settings_getInt("video.width");
    a_screen__height = a_settings_getInt("video.height");

    a_sdl__screen_set();

    if(a_settings_getBool("video.wizTear")) {
        #if A_PLATFORM_WIZ
            #define FBIO_MAGIC 'D'
            #define FBIO_LCD_CHANGE_CONTROL _IOW(FBIO_MAGIC, 90, unsigned int[2])
            #define LCD_DIRECTION_ON_CMD 5 // 320x240
            #define LCD_DIRECTION_OFF_CMD 6 // 240x320

            unsigned int send[2];
            int fb_fd = open("/dev/fb0", O_RDWR);
            send[0] = LCD_DIRECTION_OFF_CMD;
            ioctl(fb_fd, FBIO_LCD_CHANGE_CONTROL, &send);
            close(fb_fd);

            a_settings__set("video.doubleBuffer", "1");
        #else
            a_settings__set("video.wizTear", "0");
        #endif
    }

    if(a_settings_getBool("video.doubleBuffer")) {
        a_screen__pixels = a_mem_malloc(A_SCREEN_SIZE);
        memset(a_screen__pixels, 0, A_SCREEN_SIZE);
    }

    g_savedWidth = a_screen__width;
    g_savedHeight = a_screen__height;
    a_screen__savedPixels = a_screen__pixels;
}

void a_screen__uninit(void)
{
    if(!a_settings_getBool("video.window")) {
        return;
    }

    if(a_settings_getBool("video.doubleBuffer")) {
        free(a_screen__pixels);
    }
}

APixel* a_screen_pixels(void)
{
    return a_screen__pixels;
}

int a_screen_width(void)
{
    return a_screen__width;
}

int a_screen_height(void)
{
    return a_screen__height;
}

void a_screen_show(void)
{
    displayVolume();
    a_console__draw();

    a_sdl__screen_show();
}

APixel* a_screen_dup(void)
{
    APixel* const dst = a_screen_new();
    a_screen_copy(dst, a_screen__pixels);

    return dst;
}

APixel* a_screen_new(void)
{
    return a_mem_malloc(A_SCREEN_SIZE);
}

void a_screen_copy(APixel* Dst, const APixel* Src)
{
    memcpy(Dst, Src, A_SCREEN_SIZE);
}

void a_screen_copyPart(APixel* Dst, int X, int Y, int Width, int Height)
{
    const APixel* screen = a_screen__pixels + Y * a_screen__width + X;

    for(int i = Height; i--; ) {
        memcpy(Dst, screen, Width * sizeof(APixel));
        Dst += Width;
        screen += a_screen__width;
    }
}

void a_screen_setTarget(APixel* Pixels, int Width, int Height)
{
    a_screen__pixels = Pixels;
    a_screen__width = Width;
    a_screen__height = Height;
}

void a_screen_setTargetSprite(ASprite* Sprite)
{
    a_screen__pixels = Sprite->data;
    a_screen__width = Sprite->w;
    a_screen__height = Sprite->h;

    g_spriteTarget = Sprite;
}

void a_screen_resetTarget(void)
{
    a_screen__pixels = a_screen__savedPixels;
    a_screen__width = g_savedWidth;
    a_screen__height = g_savedHeight;

    if(g_spriteTarget) {
        g_spriteTarget = NULL;
    }
}
