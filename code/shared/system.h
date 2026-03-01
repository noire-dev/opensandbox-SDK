// Copyright (C) 1999-2005 ID Software, Inc.
// OpenSandbox by noire.dev — GPLv2

#define iferr(cond) do { \
    if (cond) { \
        error("Error: %s (%s:%d)", #cond, __FILE__, __LINE__); \
    } \
} while(0)

#define err(text) do { \
    error("Error: %s (%s:%d)", #text, __FILE__, __LINE__); \
} while(0)
   