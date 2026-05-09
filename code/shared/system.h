// Copyright (C) 1999-2005 ID Software, Inc.
// OpenSandbox by noire.dev — GPLv2

#define NONE -1

#define iferr(cond) do { \
    if (cond) { \
        error("Error: %s (%s:%d)", #cond, __FILE__, __LINE__); \
    } \
} while(0)

#define err(text) do { \
    error("Error: %s (%s:%d)", #text, __FILE__, __LINE__); \
} while(0)

#define bounds(x, max, name) do { \
    if((x) < 0) { \
        print("#ff5Out of Bounds in %s array: %d < 0", name, (x)); \
        (x) = 0; \
    } else if((x) >= (max)) { \
        print("#ff5Out of Bounds in %s array: %d >= %d", name, (x), (max)); \
        (x) = (max)-1; \
    } \
} while(0)

#define clamp(x, min, max) ((x) < (min) ? (min) : ((x) > (max) ? (max) : (x)))
