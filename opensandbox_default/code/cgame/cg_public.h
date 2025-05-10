// 
// OpenSandbox
// 
// Copyright (C) 1999-2005 ID Software, Inc.
// Copyright (C) 2008-2012 OpenArena Team
// Copyright (C) 2023-2024 Noire.dev
// Copyright (C) 2025 OpenSandbox Team
// 
// This file is part of OpenSandbox.
// 
// OpenSandbox is free software; you can redistribute it and/or modify
// it under the terms of the GNU General Public License, version 2,
// as published by the Free Software Foundation.
// 
// This modified code is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
// GNU General Public License for more details.
// 
// You should have received a copy of the GNU General Public License
// along with this project. If not, see <http://www.gnu.org/licenses/>.
// 
// Contact: opensandboxteam@gmail.com
// 
//

#define	CMD_BACKUP			64	
#define	CMD_MASK			(CMD_BACKUP - 1)

#define	MAX_ENTITIES_IN_SNAPSHOT	4096

// Snapshots are generated at regular time intervals by the server,
// but they may not be sent if a client's rate level is exceeded, or
// they may be dropped by the network.
typedef struct {
	int				snapFlags;			// SNAPFLAG_RATE_DELAYED, etc
	int				ping;
	int				serverTime;		// server time the message is valid for (in msec)
	byte			areamask[MAX_MAP_AREA_BYTES];		// portalarea visibility bits
	playerState_t	ps;						// complete information about the current player at this time
	int				numEntities;			// all of the entities that need to be presented
	entityState_t	entities[MAX_ENTITIES_IN_SNAPSHOT];	// at the time of this snapshot
	int				numServerCommands;		// text based server commands to execute when this
	int				serverCommandSequence;	// snapshot becomes current
} snapshot_t;

typedef enum {
	CG_INIT,
	CG_SHUTDOWN,
	CG_CONSOLE_COMMAND,
	CG_DRAW_ACTIVE_FRAME,
	CG_CROSSHAIR_PLAYER,
	CG_LAST_ATTACKER
} cgameExport_t;
