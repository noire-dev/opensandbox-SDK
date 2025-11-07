// Copyright (C) 1999-2005 ID Software, Inc.
// Copyright (C) 2023-2025 Noire.dev
// Copyright (C) 2025 OpenSandbox Team
// OpenSandbox — GPLv2; see LICENSE for details.

#if !defined(BG_LIB_H) && defined(QVM)
#define BG_LIB_H

#define __attribute__(x)

#ifndef NULL
#define NULL ((void *)0)
#endif

typedef int size_t;

typedef char *va_list;
#define _INTSIZEOF(n) ((sizeof(n) + sizeof(int) - 1) & ~(sizeof(int) - 1))
#define va_start(ap, v) (ap = (va_list) & v + _INTSIZEOF(v))
#define va_arg(ap, t) (*(t *)((ap += _INTSIZEOF(t)) - _INTSIZEOF(t)))
#define va_end(ap) (ap = (va_list)0)

#define INT_MAX 0x7fffffff       /* maximum (signed) int value */
#define INT_MIN (-INT_MAX - 1)   /* minimum (signed) int value */
#define LONG_MAX 0x7fffffffL     /* maximum (signed) long value */
#define LONG_MIN (-LONG_MAX - 1) /* minimum (signed) long value */

#define isalnum(c) (isalpha(c) || isdigit(c))
#define isalpha(c) (isupper(c) || islower(c))
#define isascii(c) ((c) > 0 && (c) <= 0x7f)
#define iscntrl(c) (((c) >= 0) && (((c) <= 0x1f) || ((c) == 0x7f)))
#define isdigit(c) ((c) >= '0' && (c) <= '9')
#define isgraph(c) ((c) != ' ' && isprint(c))
#define islower(c) ((c) >= 'a' && (c) <= 'z')
#define isprint(c) ((c) >= ' ' && (c) <= '~')
#define ispunct(c) (((c) > ' ' && (c) <= '~') && !isalnum(c))
#define isspace(c) ((c) == ' ' || (c) == '\f' || (c) == '\n' || (c) == '\r' || (c) == '\t' || (c) == '\v')
#define isupper(c) ((c) >= 'A' && (c) <= 'Z')
#define isxdigit(c) (isxupper(c) || isxlower(c))
#define isxlower(c) (isdigit(c) || (c >= 'a' && c <= 'f'))
#define isxupper(c) (isdigit(c) || (c >= 'A' && c <= 'F'))

// Misc functions
typedef int cmp_t(const void *, const void *);

// bg_lib.c
void qsort(void *a, size_t n, size_t es, cmp_t *cmp);
size_t strlen(const char *string);
int strlenru(const char *string);
int ifstrlenru(const char *string);
char *strcat(char *strDestination, const char *strSource);
char *strcpy(char *strDestination, const char *strSource);
int strcmp(const char *string1, const char *string2);
char *strchr(const char *string, int c);
char *strrchr(const char *string, int c);
char *strstr(const char *string, const char *strCharSet);
int tolower(int c);
int toupper(int c);
void *memmove(void *dest, const void *src, size_t count);
double tan(double x);
void srand(unsigned seed);
int rand(void);
double atof(const char *string);
int atoi(const char *string);
int abs(int n);
double fabs(double x);
int Q_vsnprintf(char *str, size_t length, const char *fmt, va_list args);
int Q_snprintf(char *str, size_t length, const char *fmt, ...);
int sscanf(const char *buffer, const char *fmt, ...);

#endif  // BG_LIB_H
