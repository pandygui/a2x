/*
    Copyright 2018-2019 Alex Margarit <alex@alxm.org>
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

#include "a2x_pack_block.v.h"

#include "a2x_pack_file.v.h"
#include "a2x_pack_main.v.h"
#include "a2x_pack_mem.v.h"
#include "a2x_pack_str.v.h"
#include "a2x_pack_strhash.v.h"

static AList* g_emptyList;

struct ABlock {
    char* text; // own content
    AList* blocks; // list of ABlock indented under this block
    AStrHash* index; // table of AList of ABlock, the blocks indexed by name
    const ABlock** array; // the blocks indexed by line # relative to parent
    unsigned arrayLen; // number of blocks under parent
    unsigned refs; // take a ref when inheriting
};

void a_block__init(void)
{
    g_emptyList = a_list_new();
}

void a_block__uninit(void)
{
    a_list_free(g_emptyList);
}

static ABlock* blockNew(const char* Content)
{
    ABlock* block = a_mem_zalloc(sizeof(ABlock));

    block->text = a_str_dup(Content);

    return block;
}

static void blockFree(ABlock* Block)
{
    if(Block->refs-- > 0) {
        // This block will be freed later as part of freeing another block
        return;
    }

    free(Block->text);

    if(Block->blocks) {
        a_list_freeEx(Block->blocks, (AFree*)blockFree);
        a_strhash_freeEx(Block->index, (AFree*)a_list_free);
        free(Block->array);
    }

    free(Block);
}

static void blockAdd(ABlock* Parent, ABlock* Child)
{
    if(Parent->blocks == NULL) {
        Parent->blocks = a_list_new();
        Parent->index = a_strhash_new();
    }

    AList* indexList = a_strhash_get(Parent->index, Child->text);

    if(indexList == NULL) {
        indexList = a_list_new();
        a_strhash_add(Parent->index, Child->text, indexList);
    }

    a_list_addLast(Parent->blocks, Child);
    a_list_addLast(indexList, Child);
}

static void blockCacheLines(ABlock* Block)
{
    if(Block->blocks != NULL) {
        Block->array = (const ABlock**)a_list_toArray(Block->blocks);
        Block->arrayLen = a_list_sizeGet(Block->blocks);
    }
}

static inline const ABlock* blockGet(const ABlock* Block, unsigned LineNumber)
{
    if(LineNumber == 0) {
        return Block;
    }

    if(Block && LineNumber <= Block->arrayLen) {
        return Block->array[LineNumber - 1];
    }

    return NULL;
}

ABlock* a_block_new(const char* File)
{
    ABlock* root = blockNew("");
    AFile* f = a_file_new(File, A_FILE_READ);
    AList* stack = a_list_new();
    int lastIndent = -1;

    a_list_push(stack, root);

    while(a_file_lineRead(f)) {
        const char* const lineStart = a_file_lineBufferGet(f);
        const char* textStart = lineStart;

        while(*textStart == ' ') {
            textStart++;
        }

        int currentIndent = (int)(textStart - lineStart) / 4;

        if((textStart - lineStart) % 4 || currentIndent > lastIndent + 1) {
            A__FATAL("a_block_new: Bad indent in %s:%d <%s>",
                     File,
                     a_file_lineNumberGet(f),
                     textStart);
        }

        // Each subsequent entry has -1 indentation, pop until reach parent
        while(currentIndent <= lastIndent) {
            blockCacheLines(a_list_pop(stack));
            lastIndent--;
        }

        lastIndent = currentIndent;

        ABlock* parent = a_list_peek(stack);
        ABlock* block = blockNew(textStart);

        if(currentIndent == 0) {
            // This is a root-level block, check if it inherits from another
            for(char* base = a_str_prefixGetToLast(textStart, '.'); base; ) {
                const ABlock* baseBlock = a_block_keyGetBlock(root, base);

                if(baseBlock == NULL) {
                    char* nextBaseId = a_str_prefixGetToLast(base, '.');

                    free(base);
                    base = nextBaseId;
                } else {
                    if(baseBlock->blocks) {
                        A_LIST_ITERATE(baseBlock->blocks, ABlock*, b) {
                            blockAdd(block, b);
                            b->refs++;
                        }
                    }

                    free(base);
                    base = NULL;
                }
            }
        }

        blockAdd(parent, block);
        a_list_push(stack, block);
    }

    while(!a_list_isEmpty(stack)) {
        blockCacheLines(a_list_pop(stack));
    }

    a_file_free(f);
    a_list_free(stack);

    return root;
}

void a_block_free(ABlock* Block)
{
    if(!a_str_equal(Block->text, "")) {
        A__FATAL("a_block_free: Must call on root block");
    }

    blockFree(Block);
}

AList* a_block_blocksGet(const ABlock* Block)
{
    return Block->blocks ? Block->blocks : g_emptyList;
}

const ABlock* a_block_keyGetBlock(const ABlock* Block, const char* Key)
{
    if(Block->index) {
        AList* list = a_strhash_get(Block->index, Key);

        if(list) {
            return a_list_getFirst(list);
        }
    }

    return NULL;
}

AList* a_block_keyGetBlocks(const ABlock* Block, const char* Key)
{
    if(Block->index) {
        AList* list = a_strhash_get(Block->index, Key);

        if(list) {
            return list;
        }
    }

    return g_emptyList;
}

bool a_block_keyExists(const ABlock* Block, const char* Key)
{
    return Block->index && a_strhash_contains(Block->index, Key);
}

int a_block_lineGetInt(const ABlock* Block, unsigned LineNumber)
{
    const ABlock* line = blockGet(Block, LineNumber);

    return line ? atoi(line->text) : 0;
}

unsigned a_block_lineGetIntu(const ABlock* Block, unsigned LineNumber)
{
    const ABlock* line = blockGet(Block, LineNumber);

    return line ? (unsigned)atoi(line->text) : 0;
}

AFix a_block_lineGetFix(const ABlock* Block, unsigned LineNumber)
{
    const ABlock* line = blockGet(Block, LineNumber);

    return line ? a_fix_fromDouble(atof(line->text)) : 0;
}

AFixu a_block_lineGetAngle(const ABlock* Block, unsigned LineNumber)
{
    const ABlock* line = blockGet(Block, LineNumber);

    if(line) {
        return a_fix_angleFromDegf((unsigned)atoi(line->text));
    } else {
        return 0;
    }
}

APixel a_block_lineGetPixel(const ABlock* Block, unsigned LineNumber)
{
    const ABlock* line = blockGet(Block, LineNumber);

    return line ? a_pixel_fromHex((uint32_t)strtol(line->text, NULL, 16)) : 0;
}

const char* a_block_lineGetString(const ABlock* Block, unsigned LineNumber)
{
    const ABlock* line = blockGet(Block, LineNumber);

    return line ? line->text : "";
}

AVectorInt a_block_lineGetCoords(const ABlock* Block, unsigned LineNumber)
{
    const ABlock* line = blockGet(Block, LineNumber);
    AVectorInt v = {0, 0};

    if(line) {
        sscanf(line->text, "%d, %d", &v.x, &v.y);
    }

    return v;
}

int a_block_lineGetFmt(const ABlock* Block, unsigned LineNumber, const char* Format, ...)
{
    va_list args;
    va_start(args, Format);

    int ret = a_block_lineGetFmtv(Block, LineNumber, Format, args);

    va_end(args);

    return ret;
}

int a_block_lineGetFmtv(const ABlock* Block, unsigned LineNumber, const char* Format, va_list Args)
{
    const ABlock* line = blockGet(Block, LineNumber);

    if(line) {
        return vsscanf(line->text, Format, Args);
    }

    return -1;
}

int a_block_keyGetFmt(const ABlock* Block, const char* Key, const char* Format, ...)
{
    va_list args;
    va_start(args, Format);

    int ret = a_block_lineGetFmtv(
                a_block_keyGetBlock(Block, Key), 1, Format, args);

    va_end(args);

    return ret;
}
