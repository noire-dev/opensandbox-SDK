// Copyright (C) 1999-2005 ID Software, Inc.
// Copyright (C) 2023-2025 Noire.dev
// OpenSandbox — GPLv2; see LICENSE for details.

#define CMD_BACKUP 64
#define CMD_MASK (CMD_BACKUP - 1)
#define MAX_ENTITIES_IN_SNAPSHOT 4096

typedef struct {
	int snapFlags;  // SNAPFLAG_RATE_DELAYED, etc
	int ping;
	int serverTime;                                    // server time the message is valid for (in msec)
	byte areamask[MAX_MAP_AREA_BYTES];                 // portalarea visibility bits
	playerState_t ps;                                  // complete information about the current player at this time
	int numEntities;                                   // all of the entities that need to be presented
	entityState_t entities[MAX_ENTITIES_IN_SNAPSHOT];  // at the time of this snapshot
	int numServerCommands;                             // text based server commands to execute when this
	int serverCommandSequence;                         // snapshot becomes current
} snapshot_t;

typedef enum {
	CG_INIT,
	CG_SHUTDOWN,
	CG_CONSOLE_COMMAND,
	CG_DRAW_ACTIVE_FRAME,
	CG_CROSSHAIR_PLAYER
} cgameExport_t;
