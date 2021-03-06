/*
    Copyright 2016-2018 Alex Margarit <alex@alxm.org>
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

#include "a2x_system_includes.h"

typedef int ARandomPrng(void);
typedef void ARandomPrngSeed(unsigned Seed);

extern void a_random_generatorSet(ARandomPrng* Rand, ARandomPrngSeed* Srand);
extern void a_random_generatorReset(void);

extern unsigned a_random_seedGet(void);
extern void a_random_seedSet(unsigned Seed);

extern int a_random_int(int Max);
extern unsigned a_random_intu(unsigned Max);

extern int a_random_range(int Min, int Max);
extern unsigned a_random_rangeu(unsigned Min, unsigned Max);

extern bool a_random_chance(int Something, int OutOf);
extern bool a_random_chanceu(unsigned Something, unsigned OutOf);
