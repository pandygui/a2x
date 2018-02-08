/*
    Copyright 2011, 2016, 2017 Alex Margarit

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

#if A_PLATFORM_SYSTEM_DESKTOP || A_PLATFORM_SYSTEM_EMSCRIPTEN || A_PLATFORM_SYSTEM_PANDORA
    #define A_DEVICE_HAS_KEYBOARD 1
#else
    #define A_DEVICE_HAS_KEYBOARD 0
#endif

#define A_UNUSED(X) (X = X)
#define A_ARRAY_LEN(A) (sizeof(A) / sizeof(A[0]))
typedef void AFree(void* Pointer);

#include <ctype.h>
#include <limits.h>
#include <math.h>
#include <stdarg.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
