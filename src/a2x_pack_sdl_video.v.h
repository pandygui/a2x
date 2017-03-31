/*
    Copyright 2010, 2016, 2017 Alex Margarit

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

#pragma once

#include "a2x_pack_sdl_video.p.h"

#if A_CONFIG_RENDER_SDL2
    typedef struct ASdlTexture ASdlTexture;
#endif

#include "a2x_pack_pixel.v.h"

extern void a_sdl_video__init(void);
extern void a_sdl_video__uninit(void);

extern void a_sdl_screen__set(int Width, int Height);
extern void a_sdl_screen__show(void);
extern void a_sdl_screen__setFullScreen(bool FullScreen);

#if A_CONFIG_RENDER_SDL2
    extern void a_sdl_render__setDrawColor(void);
    extern void a_sdl_render__setBlendMode(void);

    extern void a_sdl_render__fillRect(int X, int Y, int Width, int Height);

    extern ASdlTexture* a_sdl_render__makeScreenTexture(int Width, int Height);
    extern ASdlTexture* a_sdl_render__makeSpriteTexture(const APixel* Pixels, int Width, int Height);
    extern void a_sdl_render__freeTexture(ASdlTexture* Texture);
    extern void a_sdl_render__blitTexture(ASdlTexture* Texture, int X, int Y, int Width, int Height, bool FillFlat);
    extern void a_sdl_render__setRenderTarget(ASdlTexture* Texture);
    extern void a_sdl_render__resetRenderTarget(void);
    extern void a_sdl_render__getPixels(APixel* Pixels, int Width);
#endif
