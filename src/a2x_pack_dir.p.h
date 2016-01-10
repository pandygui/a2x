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

#pragma once

#include "a2x_app_includes.h"

typedef struct Dir Dir;

extern Dir* a_dir_open(const char* path);
extern Dir* a_dir_openFilter(const char* path, int (*filter)(const struct dirent* f));
extern void a_dir_close(Dir* d);

extern void a_dir_reverse(Dir* d);

extern const char** a_dir__next(Dir* d);

enum {A_DIR_NAME, A_DIR_PATH};

#define A_DIR_ITERATE(dir, name, path)                                                              \
    for(const char** a__pair; (a__pair = a_dir__next(dir)); )                                       \
        for(const char *name = a__pair[A_DIR_NAME], *path = a__pair[A_DIR_PATH]; name; name = NULL)

extern void a_dir_reset(Dir* d);

extern const char* a_dir_path(const Dir* d);
extern const char* a_dir_name(const Dir* d);
extern int a_dir_num(const Dir* d);

extern bool a_dir_exists(const char* path);
extern void a_dir_make(const char* path);
