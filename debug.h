#ifndef ktr_debug_h
#define ktr_debug_h

#include "chunk.h"
#include "world.h"

void disassembleChunk(Chunk* chunk, const char* name);
int disassembleInstruction(Chunk* chunk, int offset);

#endif
