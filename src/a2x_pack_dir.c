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

#include "a2x_pack_dir.v.h"

struct ADir {
    char* path;
    char* name;
    AList* files;
    int num;
};

static int defaultFilter(const struct dirent* Entry)
{
    return strlen(Entry->d_name) > 0 && Entry->d_name[0] != '.';
}

ADir* a_dir_open(const char* Path)
{
    return a_dir_openFilter(Path, defaultFilter);
}

ADir* a_dir_openFilter(const char* Path, int (*filter)(const struct dirent* Entry))
{
    extern int scandir(
        const char *dirp, struct dirent ***namelist,
        int (*filter)(const struct dirent *),
        int (*compar)(const struct dirent **, const struct dirent **));

    extern int alphasort(const struct dirent **, const struct dirent **);

    struct dirent** dlist = NULL;
    const int numFiles = scandir(Path, &dlist, filter, alphasort);

    ADir* d = a_mem_malloc(sizeof(ADir));

    d->path = a_str_dup(Path);
    d->name = a_str_getSuffixLastFind(Path, '/');
    d->files = a_list_new();
    d->num = a_math_max(0, numFiles);

    if(d->name == NULL) {
        d->name = a_str_dup(Path);
    }

    for(int i = d->num; i--; ) {
        ADirEntry* e = a_mem_malloc(sizeof(ADirEntry));

        e->name = a_str_dup(dlist[i]->d_name);
        e->full = a_str_merge(Path, "/", e->name);

        a_list_addFirst(d->files, e);
        free(dlist[i]);
    }

    free(dlist);
    return d;
}

void a_dir_close(ADir* Dir)
{
    free(Dir->path);
    free(Dir->name);

    A_LIST_ITERATE(Dir->files, ADirEntry*, e) {
        free(e->name);
        free(e->full);
        free(e);
    }

    a_list_free(Dir->files);
    free(Dir);
}

AList* a_dir__files(const ADir* Dir)
{
    return Dir->files;
}

const char* a_dir_path(const ADir* Dir)
{
    return Dir->path;
}

const char* a_dir_name(const ADir* Dir)
{
    return Dir->name;
}

int a_dir_num(const ADir* Dir)
{
    return Dir->num;
}

bool a_dir_exists(const char* Path)
{
    DIR* const d = opendir(Path);

    if(d) {
        closedir(d);
        return true;
    }

    return false;
}

void a_dir_make(const char* Path)
{
    mkdir(Path, S_IRWXU);
}
