/*
    Copyright 2016 Alex Margarit

    This file is part of a2x-framework.

    a2x-framework is free software: you can redistribute it and/or modify
    it under the terms of the GNU Lesser General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    a2x-framework is distributed in the hope that it will be useful,
    a2x-framework is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU Lesser General Public License for more details.

    You should have received a copy of the GNU Lesser General Public License
    along with a2x-framework.  If not, see <http://www.gnu.org/licenses/>.
*/

#include "a2x_pack_console.v.h"
#include "media/console.h"

typedef struct ALine {
    AConsoleOutType type;
    char* text;
} ALine;

#define LINE_HEIGHT 10

bool g_enabled;
bool g_show;
static AList* g_lines;
static int g_linesPerScreen;
static ASprite* g_titles[A_CONSOLE_MAX];
static AInput* g_toggle;

static ALine* line_new(AConsoleOutType Type, const char* Text)
{
    ALine* line = a_mem_malloc(sizeof(ALine));

    line->type = Type;
    line->text = a_str_dup(Text);

    return line;
}

static void line_free(ALine* Line)
{
    free(Line->text);
    free(Line);
}

static void inputCallback(void)
{
    if(a_button_getAndUnpress(g_toggle)) {
        g_show = !g_show;
    }
}

static void screenCallback(void)
{
    if(!g_enabled || !g_show) {
        return;
    }

    a_pixel_push();
    a_pixel_setBlend(A_PIXEL_BLEND_RGB75);
    a_pixel_setRGB(0x28, 0x18, 0x18);
    a_draw_fill();
    a_pixel_pop();

    int y = 2;
    const int xOffset = 1 + g_titles[A_CONSOLE_MESSAGE]->w + 2;

    a_font_setCoords(2, y);
    a_font_setFace(A_FONT_FACE_BLUE); a_font_text("a");
    a_font_setFace(A_FONT_FACE_GREEN); a_font_text("2");
    a_font_setFace(A_FONT_FACE_YELLOW); a_font_text("x");
    a_font_setFace(A_FONT_FACE_WHITE);
    a_font_textf(" %s, %s",
        A__MAKE_CURRENT_GIT_BRANCH,
        A__MAKE_COMPILE_TIME);

    a_font_setCoords(2, y + LINE_HEIGHT);
    a_font_textf("%s %s by %s, %s",
        a_settings_getString("app.title"),
        a_settings_getString("app.version"),
        a_settings_getString("app.author"),
        a_settings_getString("app.buildtime"));

    a_font_setAlign(A_FONT_ALIGN_RIGHT);
    a_font_setFace(A_FONT_FACE_YELLOW);
    a_font_setCoords(a_screen__width - 2, y);
    a_font_textf("%u fps", a_fps_getFps());
    a_font_setFace(A_FONT_FACE_GREEN);
    a_font_setCoords(a_screen__width - 2, y + LINE_HEIGHT);
    a_font_textf("%u max", a_fps_getMaxFps());
    a_font_setFace(A_FONT_FACE_BLUE);
    a_font_setCoords(a_screen__width - 2, y + 2 * LINE_HEIGHT);
    a_font_textf("%u skip", a_fps_getFrameSkip());

    y += 2 * LINE_HEIGHT;

    a_font_setFace(A_FONT_FACE_LIGHT_GRAY);
    a_font_setAlign(A_FONT_ALIGN_LEFT);

    A_LIST_ITERATE(g_lines, ALine*, line) {
        ASprite* graphic = g_titles[line->type];
        a_sprite_blit(graphic, 1, y);

        a_font_setCoords(xOffset, y);
        a_font_setLineWidth(a_screen__width - xOffset);
        a_font_text(line->text);
        a_font_resetLineWidth();

        y += LINE_HEIGHT;
    }
}

void a_console__init(void)
{
    g_enabled = true;
    g_show = false;
    g_lines = a_list_new();
    g_linesPerScreen = INT_MAX;
}

void a_console__init2(void)
{
    ASprite* graphics = a_sprite_fromData(g_media_console);
    ASpriteFrames* frames = a_spriteframes_new(graphics, 0, 0, 0);

    g_titles[A_CONSOLE_MESSAGE] = a_spriteframes_geti(frames, 0);
    g_titles[A_CONSOLE_WARNING] = a_spriteframes_geti(frames, 1);
    g_titles[A_CONSOLE_ERROR] = a_spriteframes_geti(frames, 2);
    g_titles[A_CONSOLE_STATE] = a_spriteframes_geti(frames, 4);

    a_spriteframes_free(frames, false);
    a_sprite_free(graphics);

    g_linesPerScreen = a_settings_getInt("video.height") / LINE_HEIGHT - 2;

    // In case messages were logged between init and init2
    while(a_list_size(g_lines) > g_linesPerScreen) {
        line_free(a_list_pop(g_lines));
    }

    g_show = a_settings_getBool("console.on");
    g_toggle = a_input_new(a_settings_getString("console.button"));

    a_input__addCallback(inputCallback);
    a_screen__addOverlay(screenCallback);
}

void a_console__uninit(void)
{
    g_enabled = false;

    A_LIST_ITERATE(g_lines, ALine*, line) {
        line_free(line);
    }

    a_list_free(g_lines);
}

void a_console__write(AConsoleOutType Type, const char* Text)
{
    if(!g_enabled) {
        return;
    }

    a_list_addLast(g_lines, line_new(Type, Text));

    if(a_list_size(g_lines) > g_linesPerScreen) {
        line_free(a_list_pop(g_lines));
    }
}
