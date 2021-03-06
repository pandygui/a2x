/*
    Copyright 2010, 2016-2019 Alex Margarit <alex@alxm.org>
    This file is part of a2x, a C video game framework.

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#pragma once

#include "a2x_pack_platform.p.h"

typedef struct APlatformInputAnalog APlatformInputAnalog;
typedef struct APlatformInputButton APlatformInputButton;
typedef struct APlatformInputController APlatformInputController;
typedef struct APlatformInputTouch APlatformInputTouch;

typedef void APlatformSoundMusic;
typedef void APlatformSoundSample;

typedef struct APlatformTexture APlatformTexture;

#include "a2x_pack_input_analog.v.h"
#include "a2x_pack_input_button.v.h"
#include "a2x_pack_sprite.v.h"

extern void a_platform__init(void);
extern void a_platform__uninit(void);

extern uint32_t a_platform_api__timeMsGet(void);
extern void a_platform_api__timeMsWait(uint32_t Ms);

extern void a_platform_api__screenInit(int Width, int Height);
extern void a_platform_api__screenClear(void);
extern void a_platform_api__screenShow(void);
extern AVectorInt a_platform_api__screenResolutionGetNative(void);
extern bool a_platform_api__screenVsyncGet(void);
extern int a_platform_api__screenZoomGet(void);
extern void a_platform_api__screenZoomSet(int Zoom);
extern bool a_platform_api__screenFullscreenGet(void);
extern void a_platform_api__screenFullscreenFlip(void);
extern void a_platform_api__screenMouseCursorSet(bool Show);

extern void a_platform_api__renderSetDrawColor(void);
extern void a_platform_api__renderSetBlendMode(void);
extern void a_platform_api__renderTargetSet(APlatformTexture* Texture);
extern void a_platform_api__renderTargetPixelsGet(APixel* Pixels, int Width);
extern void a_platform_api__renderTargetClipSet(int X, int Y, int Width, int Height);

extern void a_platform_api__drawPixel(int X, int Y);
extern void a_platform_api__drawLine(int X1, int Y1, int X2, int Y2);
extern void a_platform_api__drawHLine(int X1, int X2, int Y);
extern void a_platform_api__drawVLine(int X, int Y1, int Y2);
extern void a_platform_api__drawRectangleFilled(int X, int Y, int Width, int Height);
extern void a_platform_api__drawRectangleOutline(int X, int Y, int Width, int Height);
extern void a_platform_api__drawCircleOutline(int X, int Y, int Radius);
extern void a_platform_api__drawCircleFilled(int X, int Y, int Radius);

extern APlatformTexture* a_platform_api__textureNewScreen(int Width, int Height);
extern APlatformTexture* a_platform_api__textureNewSprite(const ASprite* Sprite);
extern void a_platform_api__textureFree(APlatformTexture* Texture);
extern void a_platform_api__textureBlit(const APlatformTexture* Texture, int X, int Y, bool FillFlat);
extern void a_platform_api__textureBlitEx(const APlatformTexture* Texture, int X, int Y, AFix Scale, unsigned Angle, int CenterX, int CenterY, bool FillFlat);

extern bool a_platform_api__soundMuteGet(void);
extern void a_platform_api__soundMuteFlip(void);
extern int a_platform_api__soundVolumeGetMax(void);

extern APlatformSoundMusic* a_platform_api__soundMusicNew(const char* Path);
extern void a_platform_api__soundMusicFree(APlatformSoundMusic* Music);
extern void a_platform_api__soundMusicVolumeSet(int Volume);
extern void a_platform_api__soundMusicPlay(APlatformSoundMusic* Music);
extern void a_platform_api__soundMusicStop(void);

extern APlatformSoundSample* a_platform_api__soundSampleNewFromFile(const char* Path);
extern APlatformSoundSample* a_platform_api__soundSampleNewFromData(const uint8_t* Data, int Size);
extern void a_platform_api__soundSampleFree(APlatformSoundSample* Sample);
extern void a_platform_api__soundSampleVolumeSet(APlatformSoundSample* Sample, int Volume);
extern void a_platform_api__soundSampleVolumeSetAll(int Volume);
extern void a_platform_api__soundSamplePlay(APlatformSoundSample* Sample, int Channel, bool Loop);
extern void a_platform_api__soundSampleStop(int Channel);
extern bool a_platform_api__soundSampleIsPlaying(int Channel);
extern int a_platform_api__soundSampleChannelGet(void);

extern void a_platform_api__inputPoll(void);

extern APlatformInputButton* a_platform_api__inputButtonGet(int Id);
extern const char* a_platform_api__inputButtonNameGet(const APlatformInputButton* Button);
extern bool a_platform_api__inputButtonPressGet(const APlatformInputButton* Button);
extern void a_platform_api__inputButtonForward(int Source, int Destination);

extern APlatformInputAnalog* a_platform_api__inputAnalogGet(int Id);
extern const char* a_platform_api__inputAnalogNameGet(const APlatformInputAnalog* Analog);
extern int a_platform_api__inputAnalogValueGet(const APlatformInputAnalog* Analog);
extern void a_platform_api__inputAnalogForward(AAxisId Source, AButtonId Negative, AButtonId Positive);

extern APlatformInputTouch* a_platform_api__inputTouchGet(void);
extern AVectorInt a_platform_api__inputTouchCoordsGet(const APlatformInputTouch* Touch);
extern AVectorInt a_platform_api__inputTouchDeltaGet(const APlatformInputTouch* Touch);
extern bool a_platform_api__inputTouchTapGet(const APlatformInputTouch* Touch);

extern unsigned a_platform_api__inputControllerNumGet(void);
extern void a_platform_api__inputControllerSet(unsigned Index);
extern bool a_platform_api__inputControllerIsMapped(void);
