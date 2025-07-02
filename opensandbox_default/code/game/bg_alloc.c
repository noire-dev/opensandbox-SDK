// Copyright (C) 1999-2005 ID Software, Inc.
// Copyright (C) 2023-2025 Noire.dev
// Copyright (C) 2025 OpenSandbox Team
// OpenSandbox — GPLv2; see LICENSE for details.

#include "g_local.h"

#define POOLSIZE (1024 * 1024) * 16  // QVM_MEMORY note: use 16 for 32bit
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
