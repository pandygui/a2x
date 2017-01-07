/*
    Copyright 2016 Alex Margarit

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

#include "a2x_system_includes.h"

typedef struct ABitfield ABitfield;

extern ABitfield* a_bitfield_new(size_t NumBits);
extern void a_bitfield_free(ABitfield* Bitfield);

extern void a_bitfield_set(ABitfield* Bitfield, size_t Bit);
extern void a_bitfield_clear(ABitfield* Bitfield, size_t Bit);
extern void a_bitfield_reset(ABitfield* Bitfield);

extern bool a_bitfield_test(const ABitfield* Bitfield, size_t Bit);
extern bool a_bitfield_testMask(const ABitfield* Bitfield, const ABitfield* Mask);