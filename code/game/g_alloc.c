// Copyright (C) 1999-2005 ID Software, Inc.
// OpenSandbox by noire.dev — GPLv2

#include "../shared/javascript.h"

#define POOLSIZE (1024 * 1024) // QVM_MEMORY: 1mb
static char memoryPool[POOLSIZE];
static int allocPoint;

void *G_Alloc(int size) {
	char *p;

	if(allocPoint + size > POOLSIZE) {
		G_Error("G_Alloc: failed on allocation of %i bytes\n", size);
		return NULL;
	}

	p = &memoryPool[allocPoint];
	allocPoint += (size + 31) & ~31;
	return p;
}

void G_InitMemory(void) { allocPoint = 0; }
