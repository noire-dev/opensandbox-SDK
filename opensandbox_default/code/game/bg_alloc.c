// Copyright (C) 1999-2005 ID Software, Inc.
// Copyright (C) 2023-2025 Noire.dev
// Copyright (C) 2025 OpenSandbox Team
// OpenSandbox — GPLv2; see LICENSE for details.

#include "../qcommon/q_shared.h"
#include "bg_public.h"
#include "g_local.h"

#define  POOLSIZE ( 1024 * 1024 ) * 16       //QVM_MEMORY note: use 16 for 32bit

#define  FREEMEMCOOKIE  ((int)0xDEADBE3F)  // Any unlikely to be used value
#define  ROUNDBITS    (unsigned int)31          // Round to 32 bytes

typedef struct freeMemNode_s {
  int cookie, size;        // Size includes node (obviously)
  struct freeMemNode_s *prev, *next;
} freeMemNode_t;

static char           memoryPool[POOLSIZE];
static freeMemNode_t  *freeHead;
static int            freeMem;

qboolean BG_CanAlloc( unsigned int size) {
	freeMemNode_t *fmn;
	int allocsize = ( size + sizeof(int) + ROUNDBITS ) & ~ROUNDBITS;    // Round to 32-byte boundary
	for( fmn = freeHead; fmn; fmn = fmn->next ) {
		if( fmn->cookie != FREEMEMCOOKIE ) {
			//Memory curroption
			return qfalse;
		}
		if( fmn->size >= allocsize ) {
			//At least one useable block
			return qtrue;
		}
	}
	return qfalse;
}

void *BG_Alloc( unsigned int size ) { // Find a free block and allocate.
  freeMemNode_t *fmn, *prev, *next, *smallest;
  int allocsize, smallestsize;
  char *endptr;
  int *ptr;
  int usedMem;
  usedMem = POOLSIZE - freeMem;

  allocsize = ( size + sizeof(int) + ROUNDBITS ) & ~ROUNDBITS;    // Round to 32-byte boundary
  ptr = NULL;

  smallest = NULL;
  smallestsize = POOLSIZE + 1;    // Guaranteed not to miss any slots :)
  for( fmn = freeHead; fmn; fmn = fmn->next ) {
	if( fmn->cookie != FREEMEMCOOKIE )
	  Com_Error( ERR_DROP, "BG_Alloc: Memory corruption detected!\n" );

	if( fmn->size >= allocsize ) {
	  // We've got a block
	  if( fmn->size == allocsize ) {
		// Same size, just remove

		prev = fmn->prev;
		next = fmn->next;
		if( prev )
		  prev->next = next;      // Point previous node to next
		if( next )
		  next->prev = prev;      // Point next node to previous
		if( fmn == freeHead )
		  freeHead = next;      // Set head pointer to next
		ptr = (int *) fmn;
		break;              // Stop the loop, this is fine
	  } else {
		// Keep track of the smallest free slot
		if( fmn->size < smallestsize ) {
		  smallest = fmn;
		  smallestsize = fmn->size;
		}
	  }
	}
  }

  if( !ptr && smallest ) {
	// We found a slot big enough
	smallest->size -= allocsize;
	endptr = (char *) smallest + smallest->size;
	ptr = (int *) endptr;
  }

  if( ptr ) {
	freeMem -= allocsize;
	memset( ptr, 0, allocsize );
	*ptr++ = allocsize;        // Store a copy of size for deallocation
	return( (void *) ptr );
  }

	G_Printf( "BG_Alloc: failed on allocation of %i bytes\n", usedMem, POOLSIZE );
  return( NULL );
}

void BG_Free( void *ptr ) { // Release allocated memory, add it to the free list.
  freeMemNode_t *fmn;
  char *freeend;
  int *freeptr;

  freeptr = ptr;
  freeptr--;

  freeMem += *freeptr;

  for( fmn = freeHead; fmn; fmn = fmn->next ) {
	freeend = ((char *) fmn) + fmn->size;
	if( freeend == (char *) freeptr ) {
	  // Released block can be merged to an existing node

	  fmn->size += *freeptr;    // Add size of node.
	  return;
	}
  }
  // No merging, add to head of list

  fmn = (freeMemNode_t *) freeptr;
  fmn->size = *freeptr;        // Set this first to avoid corrupting *freeptr
  fmn->cookie = FREEMEMCOOKIE;
  fmn->prev = NULL;
  fmn->next = freeHead;
  freeHead->prev = fmn;
  freeHead = fmn;
}

void BG_InitMemory( void ) {
  // Set up the initial node
  freeHead = (freeMemNode_t *)memoryPool;
  freeHead->cookie = FREEMEMCOOKIE;
  freeHead->size = POOLSIZE;
  freeHead->next = NULL;
  freeHead->prev = NULL;
  freeMem = sizeof( memoryPool );
}
