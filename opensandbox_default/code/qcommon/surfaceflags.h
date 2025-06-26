// Copyright (C) 1999-2005 ID Software, Inc.
// Copyright (C) 2023-2025 Noire.dev
// Copyright (C) 2025 OpenSandbox Team
// OpenSandbox — GPLv2; see LICENSE for details.

// contents flags are seperate bits
// a given brush can contribute multiple content bits

// these definitions also need to be in q_shared.h!

#define	CONTENTS_SOLID	            1		// an eye is never valid in a solid
#define	CONTENTS_LAVA	            8
#define	CONTENTS_SLIME	            16
#define	CONTENTS_WATER	            32
#define	CONTENTS_FOG	            64
#define	CONTENTS_NOTTEAM1           128
#define	CONTENTS_NOTTEAM2           256
#define	CONTENTS_NOBOTCLIP          512
#define	CONTENTS_AREAPORTAL         32768
#define	CONTENTS_PLAYERCLIP         65536
#define	CONTENTS_MONSTERCLIP        131072
#define	CONTENTS_TELEPORTER         262144
#define	CONTENTS_JUMPPAD            524288
#define	CONTENTS_CLUSTERPORTAL      1048576
#define	CONTENTS_DONOTENTER         2097152
#define	CONTENTS_BOTCLIP            4194304
#define	CONTENTS_MOVER              8388608
#define	CONTENTS_ORIGIN             16777216
#define	CONTENTS_BODY               33554432
#define	CONTENTS_CORPSE             67108864
#define	CONTENTS_DETAIL             134217728
#define	CONTENTS_STRUCTURAL         268435456
#define	CONTENTS_TRANSLUCENT        536870912
#define	CONTENTS_TRIGGER            1073741824
#define	CONTENTS_NODROP             2147483648

#define	SURF_NODAMAGE			    0x1		// never give falling damage
#define	SURF_SLICK				    0x2		// effects game physics
#define	SURF_SKY				    0x4		// lighting from environment map
#define	SURF_LADDER				    0x8
#define	SURF_NOIMPACT			    0x10	// don't make missile explosions
#define	SURF_NOMARKS			    0x20	// don't leave missile marks
#define	SURF_FLESH				    0x40	// make flesh sounds and effects
#define	SURF_NODRAW				    0x80	// don't generate a drawsurface at all
#define	SURF_HINT				    0x100	// make a primary bsp splitter
#define	SURF_SKIP				    0x200	// completely ignore, allowing non-closed brushes
#define	SURF_NOLIGHTMAP			    0x400	// surface doesn't need a lightmap
#define	SURF_POINTLIGHT			    0x800	// generate lighting info at vertexes
#define	SURF_METALSTEPS			    0x1000	// clanking footsteps
#define	SURF_NOSTEPS			    0x2000	// no footstep sounds
#define	SURF_NONSOLID			    0x4000	// don't collide against curves with this set
#define	SURF_LIGHTFILTER		    0x8000	// act as a light filter during q3map -light
#define	SURF_ALPHASHADOW		    0x10000	// do per-pixel light shadow casting in q3map
#define	SURF_NODLIGHT			    0x20000	// don't dlight even if solid (solid lava, skies)
#define SURF_DUST				    0x40000 // leave a dust trail when walking on this surface
