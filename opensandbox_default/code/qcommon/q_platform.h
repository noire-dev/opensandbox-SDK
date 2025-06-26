// Copyright (C) 1999-2005 ID Software, Inc.
// Copyright (C) 2023-2025 Noire.dev
// Copyright (C) 2025 OpenSandbox Team
// OpenSandbox — GPLv2; see LICENSE for details.

#ifndef __Q_PLATFORM_H
#define __Q_PLATFORM_H

#define QDECL
#define ID_INLINE

//endianness
short ShortSwap (short l);
int LongSwap (int l);
float FloatSwap (const float *f);

#define LittleShort
#define LittleLong
#define LittleFloat
#define BigShort
#define BigLong
#define BigFloat

#endif
