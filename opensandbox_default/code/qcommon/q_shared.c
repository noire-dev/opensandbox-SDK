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
// q_shared.c -- stateless support routines that are included in each code dll
#include "q_shared.h"
#include "bg_public.h"

float Com_Clamp( float min, float max, float value ) {
	if ( value < min ) {
		return min;
	}
	if ( value > max ) {
		return max;
	}
	return value;
}

float VectorToYaw( const vec3_t vec ) {
	float	yaw;
	
	if (vec[YAW] == 0 && vec[PITCH] == 0) {
		yaw = 0;
	} else {
		if (vec[PITCH]) {
			yaw = ( atan2( vec[YAW], vec[PITCH]) * M_180_PI );
		} else if (vec[YAW] > 0) {
			yaw = 90;
		} else {
			yaw = 270;
		}
		if (yaw < 0) {
			yaw += 360;
		}
	}

	return yaw;
}

/*
==============
AngleDifference

returns the difference between the two angles
==============
*/
float AngleDifference(float ang1, float ang2) {
	float diff;

	diff = ang1 - ang2;
	if (ang1 > ang2) {
		if (diff > 180.0) diff -= 360.0;
	}
	else {
		if (diff < -180.0) diff += 360.0;
	}
	return diff;
}

/*
============
COM_SkipPath
============
*/
char *COM_SkipPath (char *pathname)
{
	char	*last;
	
	last = pathname;
	while (*pathname)
	{
		if (*pathname=='/')
			last = pathname+1;
		pathname++;
	}
	return last;
}

/*
============
COM_GetExtension
============
*/
const char *COM_GetExtension( const char *name ) {
	int length, i;

	length = strlen(name)-1;
	i = length;

	while (name[i] != '.')
	{
		i--;
		if (name[i] == '/' || i == 0)
			return ""; // no extension
	}

	return &name[i+1];
}


/*
============
COM_StripExtension
============
*/
void COM_StripExtension(const char *in, char *out, int destsize) {
	int length;

	if (!in || !out || destsize <= 0)
		return;

	Q_strncpyz(out, in, destsize);

	length = strlen(out);
	if (length == 0)
		return;

	length--; // move to last valid index

	while (length > 0 && out[length] != '.') {
		if (out[length] == '/')
			return; // no extension
		length--;
	}

	if (length > 0 && out[length] == '.')
		out[length] = '\0';
}

/*
==================
COM_DefaultExtension
==================
*/
void COM_DefaultExtension (char *path, int maxSize, const char *extension ) {
	char	oldPath[MAX_QPATH];
	char    *src;

//
// if path doesn't have a .EXT, append extension
// (extension should include the .)
//
	src = path + strlen(path) - 1;

	while (*src != '/' && src != path) {
		if ( *src == '.' ) {
			return;                 // it has an extension
		}
		src--;
	}

	Q_strncpyz( oldPath, path, sizeof( oldPath ) );
	Com_sprintf( path, maxSize, "%s%s", oldPath, extension );
}

/*
============================================================================

					BYTE ORDER FUNCTIONS

============================================================================
*/

short   ShortSwap (short l)
{
	byte    b1,b2;

	b1 = l&255;
	b2 = (l>>8)&255;

	return (b1<<8) + b2;
}

short	ShortNoSwap (short l)
{
	return l;
}

int    LongSwap (int l)
{
	byte    b1,b2,b3,b4;

	b1 = l&255;
	b2 = (l>>8)&255;
	b3 = (l>>16)&255;
	b4 = (l>>24)&255;

	return ((int)b1<<24) + ((int)b2<<16) + ((int)b3<<8) + b4;
}

int	LongNoSwap (int l)
{
	return l;
}

typedef union {
    float	f;
    unsigned int i;
} _FloatByteUnion;

float FloatSwap (const float *f) {
	_FloatByteUnion out;

	out.f = *f;
	out.i = LongSwap(out.i);

	return out.f;
}

float FloatNoSwap (const float *f)
{
	return *f;
}

/*
============================================================================

PARSING

============================================================================
*/

static	char	com_token[MAX_TOKEN_CHARS];
static	int		com_lines;

char *COM_Parse( char **data_p )
{
	return COM_ParseExt( data_p, qtrue );
}

/*
==============
COM_Parse

Parse a token out of a string
Will never return NULL, just empty strings

If "allowLineBreaks" is qtrue then an empty
string will be returned if the next token is
a newline.
==============
*/
static char *SkipWhitespace( char *data, qboolean *hasNewLines ) {
	int c;

	while( (c = *data) <= ' ') {
		if( !c ) {
			return NULL;
		}
		if( c == '\n' ) {
			com_lines++;
			*hasNewLines = qtrue;
		}
		data++;
	}

	return data;
}

int COM_Compress( char *data_p ) {
	char *in, *out;
	int c;
	qboolean newline = qfalse, whitespace = qfalse;

	in = out = data_p;
	if (in) {
		while ((c = *in) != 0) {
			// skip double slash comments
			if ( c == '/' && in[1] == '/' ) {
				while (*in && *in != '\n') {
					in++;
				}
			// skip /* */ comments
			} else if ( c == '/' && in[1] == '*' ) {
				while ( *in && ( *in != '*' || in[1] != '/' ) ) 
					in++;
				if ( *in ) 
					in += 2;
            // record when we hit a newline
            } else if ( c == '\n' || c == '\r' ) {
                newline = qtrue;
                in++;
            // record when we hit whitespace
            } else if ( c == ' ' || c == '\t') {
                whitespace = qtrue;
                in++;
            // an actual token
			} else {
                // if we have a pending newline, emit it (and it counts as whitespace)
                if (newline) {
                    *out++ = '\n';
                    newline = qfalse;
                    whitespace = qfalse;
                } if (whitespace) {
                    *out++ = ' ';
                    whitespace = qfalse;
                }
                
                // copy quoted strings unmolested
                if (c == '"') {
                    *out++ = c;
                    in++;
                    while (1) {
                        c = *in;
                        if (c && c != '"') {
                            *out++ = c;
                            in++;
                        } else {
                            break;
                        }
                    }
                    if (c == '"') {
                        *out++ = c;
                        in++;
                    }
                } else {
                    *out = c;
                    out++;
                    in++;
                }
			}
		}
    	*out = 0;
    	return out - data_p;
	}
    return 0;
}

char *COM_ParseExt( char **data_p, qboolean allowLineBreaks )
{
	int c = 0, len;
	qboolean hasNewLines = qfalse;
	char *data;

	data = *data_p;
	len = 0;
	com_token[0] = 0;

	// make sure incoming data is valid
	if ( !data )
	{
		*data_p = NULL;
		return com_token;
	}

	while ( 1 )
	{
		// skip whitespace
		data = SkipWhitespace( data, &hasNewLines );
		if ( !data )
		{
			*data_p = NULL;
			return com_token;
		}
		if ( hasNewLines && !allowLineBreaks )
		{
			*data_p = data;
			return com_token;
		}

		c = *data;

		// skip double slash comments
		if ( c == '/' && data[1] == '/' )
		{
			data += 2;
			while (*data && *data != '\n') {
				data++;
			}
		}
		// skip /* */ comments
		else if ( c=='/' && data[1] == '*' ) 
		{
			data += 2;
			while ( *data && ( *data != '*' || data[1] != '/' ) ) 
			{
				data++;
			}
			if ( *data ) 
			{
				data += 2;
			}
		}
		else
		{
			break;
		}
	}

	// handle quoted strings
	if (c == '\"')
	{
		data++;
		while (1)
		{
			c = *data++;
			if (c=='\"' || !c)
			{
				com_token[len] = 0;
				*data_p = ( char * ) data;
				return com_token;
			}
			if (len < MAX_TOKEN_CHARS - 1)
			{
				com_token[len] = c;
				len++;
			}
		}
	}

	// parse a regular word
	do
	{
		if (len < MAX_TOKEN_CHARS - 1)
		{
			com_token[len] = c;
			len++;
		}
		data++;
		c = *data;
		if ( c == '\n' )
			com_lines++;
	} while (c>32);

	com_token[len] = 0;

	*data_p = ( char * ) data;
	return com_token;
}

/*
============================================================================

					LIBRARY REPLACEMENT FUNCTIONS

============================================================================
*/

int Q_isprint( int c )
{
	if ( c >= 0x20 && c <= 0x7E )
		return ( 1 );
	return ( 0 );
}

int Q_islower( int c )
{
	if (c >= 'a' && c <= 'z')
		return ( 1 );
	return ( 0 );
}

int Q_isupper( int c )
{
	if (c >= 'A' && c <= 'Z')
		return ( 1 );
	return ( 0 );
}

int Q_isalpha( int c )
{
	if ((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z'))
		return ( 1 );
	return ( 0 );
}

char* Q_strrchr( const char* string, int c )
{
	char cc = c;
	char *s;
	char *sp=(char *)0;

	s = (char*)string;

	while (*s)
	{
		if (*s == cc)
			sp = s;
		s++;
	}
	if (cc == 0)
		sp = s;

	return sp;
}

/*
=============
Q_strncpyz
 
Safe strncpy that ensures a trailing zero
=============
*/
void Q_strncpyz( char *dest, const char *src, int destsize ) {
  	if ( !dest ) {
    	Com_Printf( "Q_strncpyz: NULL dest \n" );
		return; 
  	}
	if ( !src ) {
		return; 
	}
	if ( destsize < 1 ) {
		Com_Printf( "Q_strncpyz: destsize < 1 \n" );
		return; 
	}

	strncpy( dest, src, destsize-1 );
  	dest[destsize-1] = 0;
}
                 
int Q_stricmpn (const char *s1, const char *s2, int n) {
	int		c1, c2;

        if ( s1 == NULL ) {
           if ( s2 == NULL )
             return 0;
           else
             return -1;
        }
        else if ( s2==NULL )
          return 1;


	
	do {
		c1 = *s1++;
		c2 = *s2++;

		if (!n--) {
			return 0;		// strings are equal until end point
		}
		
		if (c1 != c2) {
			if (c1 >= 'a' && c1 <= 'z') {
				c1 -= ('a' - 'A');
			}
			if (c2 >= 'a' && c2 <= 'z') {
				c2 -= ('a' - 'A');
			}
			if (c1 != c2) {
				return c1 < c2 ? -1 : 1;
			}
		}
	} while (c1);
	
	return 0;		// strings are equal
}

int Q_strncmp (const char *s1, const char *s2, int n) {
	int		c1, c2;
	
	do {
		c1 = *s1++;
		c2 = *s2++;

		if (!n--) {
			return 0;		// strings are equal until end point
		}
		
		if (c1 != c2) {
			return c1 < c2 ? -1 : 1;
		}
	} while (c1);
	
	return 0;		// strings are equal
}

int Q_stricmp (const char *s1, const char *s2) {
	return (s1 && s2) ? Q_stricmpn (s1, s2, 99999) : -1;
}


char *Q_strlwr( char *s1 ) {
    char	*s;

    s = s1;
	while ( *s ) {
		*s = tolower(*s);
		s++;
	}
    return s1;
}

char *Q_strupr( char *s1 ) {
    char	*s;

    s = s1;
	while ( *s ) {
		*s = toupper(*s);
		s++;
	}
    return s1;
}


// never goes past bounds or leaves without a terminating 0
void Q_strcat( char *dest, int size, const char *src ) {
	int		l1;

	l1 = strlen( dest );
	if ( l1 >= size ) {
		Com_Error( ERR_FATAL, "Q_strcat: already overflowed" );
	}
	Q_strncpyz( dest + l1, src, size - l1 );
}

/*
* Find the first occurrence of find in s.
*/
const char *Q_stristr( const char *s, const char *find)
{
  char c, sc;
  size_t len;

  if ((c = *find++) != 0)
  {
    if (c >= 'a' && c <= 'z')
    {
      c -= ('a' - 'A');
    }
    len = strlen(find);
    do
    {
      do
      {
        if ((sc = *s++) == 0)
          return NULL;
        if (sc >= 'a' && sc <= 'z')
        {
          sc -= ('a' - 'A');
        }
      } while (sc != c);
    } while (Q_stricmpn(s, find, len) != 0);
    s--;
  }
  return s;
}

char *Q_CleanStr( char *string ) {
return string;
}

void QDECL Com_sprintf( char *dest, int size, const char *fmt, ...) {
	int		len;
	va_list		argptr;
	char	bigbuffer[32000];	// big, but small enough to fit in PPC stack

	va_start (argptr,fmt);
	len = Q_vsnprintf (bigbuffer, sizeof(bigbuffer), fmt,argptr);
	va_end (argptr);
	if ( len >= sizeof( bigbuffer ) ) {
		Com_Error( ERR_FATAL, "Com_sprintf: overflowed bigbuffer" );
	}
	if (len >= size) {
		Com_Printf ("Com_sprintf: overflow of %i in %i\n", len, size);
#ifdef	_DEBUG
		__asm {
			int 3;
		}
#endif
	}
	Q_strncpyz (dest, bigbuffer, size );
}


/*
============
va

does a varargs printf into a temp buffer, so I don't need to have
varargs versions of all text functions.
============
*/
void convert_to_negative(char *str) {
	size_t i;
    for (i = 0; str[i] != '\0';) {
        unsigned char c = (unsigned char)str[i];
        if (c >= 128) {
            str[i] = (char)(c - 256);
        }
        if (c >= 0xC0) {
            if (c >= 0xF0) {
                i += 4;
            } else if (c >= 0xE0) {
                i += 3;
            } else {
                i += 2;
            }
        } else {
            i++;
        }
    }
}

void convert_to_positive(char *str) {
	size_t i;
    for (i = 0; str[i] != '\0';) {
        signed char c = (signed char)str[i];
        if (c < 0) {
            str[i] = (char)(c + 256);
        }
        if ((unsigned char)c >= 0xC0) {
            if (c >= 0xF0) {
                i += 4;
            } else if (c >= 0xE0) {
                i += 3;
            } else {
                i += 2;
            }
        } else {
            i++;
        }
    }
}

// Ваша функция va
char *QDECL va(char *format, ...) {
    va_list argptr;
    static char string[2][32000]; // В случае, если va будет вызвана из вложенных функций
    static int index = 0;
    char *buf;

    buf = string[index & 1];
    index++;

    va_start(argptr, format);

    convert_to_negative(buf);
    
    Q_vsnprintf(buf, sizeof(string[0]), format, argptr);
    
    convert_to_positive(buf);

    va_end(argptr);

    return buf;
}

/*
=====================================================================

  INFO STRINGS

=====================================================================
*/

/*
===============
Info_ValueForKey

Searches the string for the given
key and returns the associated value, or an empty string.
FIXME: overflow check?
===============
*/
char *Info_ValueForKey( const char *s, const char *key ) {
	char	pkey[BIG_INFO_KEY];
	static	char value[2][BIG_INFO_VALUE];	// use two buffers so compares
											// work without stomping on each other
	static	int	valueindex = 0;
	char	*o;
	
	if ( !s || !key ) {
		return "";
	}

	if ( strlen( s ) >= BIG_INFO_STRING ) {
		Com_Error( ERR_DROP, "Info_ValueForKey: oversize infostring" );
	}

	valueindex ^= 1;
	if (*s == '\\')
		s++;
	while (1)
	{
		o = pkey;
		while (*s != '\\')
		{
			if (!*s)
				return "";
			*o++ = *s++;
		}
		*o = 0;
		s++;

		o = value[valueindex];

		while (*s != '\\' && *s)
		{
			*o++ = *s++;
		}
		*o = 0;

		if (!Q_stricmp (key, pkey) )
			return value[valueindex];

		if (!*s)
			break;
		s++;
	}

	return "";
}


/*
===================
Info_NextPair

Used to itterate through all the key/value pairs in an info string
===================
*/
void Info_NextPair( const char **head, char *key, char *value ) {
	char	*o;
	const char	*s;

	s = *head;

	if ( *s == '\\' ) {
		s++;
	}
	key[0] = 0;
	value[0] = 0;

	o = key;
	while ( *s != '\\' ) {
		if ( !*s ) {
			*o = 0;
			*head = s;
			return;
		}
		*o++ = *s++;
	}
	*o = 0;
	s++;

	o = value;
	while ( *s != '\\' && *s ) {
		*o++ = *s++;
	}
	*o = 0;

	*head = s;
}


/*
===================
Info_RemoveKey
===================
*/
void Info_RemoveKey( char *s, const char *key ) {
	char	*start;
	char	pkey[MAX_INFO_KEY];
	char	value[MAX_INFO_VALUE];
	char	*o;

	if ( strlen( s ) >= MAX_INFO_STRING ) {
		Com_Error( ERR_DROP, "Info_RemoveKey: oversize infostring" );
	}

	if (strchr (key, '\\')) {
		return;
	}

	while (1)
	{
		start = s;
		if (*s == '\\')
			s++;
		o = pkey;
		while (*s != '\\')
		{
			if (!*s)
				return;
			*o++ = *s++;
		}
		*o = 0;
		s++;

		o = value;
		while (*s != '\\' && *s)
		{
			if (!*s)
				return;
			*o++ = *s++;
		}
		*o = 0;

		if (!strcmp (key, pkey) )
		{
			memmove(start, s, strlen(s) + 1); // remove this part
			
			return;
		}

		if (!*s)
			return;
	}

}

/*
===================
Info_RemoveKey_Big
===================
*/
void Info_RemoveKey_Big( char *s, const char *key ) {
	char	*start;
	char	pkey[BIG_INFO_KEY];
	char	value[BIG_INFO_VALUE];
	char	*o;

	if ( strlen( s ) >= BIG_INFO_STRING ) {
		Com_Error( ERR_DROP, "Info_RemoveKey_Big: oversize infostring" );
	}

	if (strchr (key, '\\')) {
		return;
	}

	while (1)
	{
		start = s;
		if (*s == '\\')
			s++;
		o = pkey;
		while (*s != '\\')
		{
			if (!*s)
				return;
			*o++ = *s++;
		}
		*o = 0;
		s++;

		o = value;
		while (*s != '\\' && *s)
		{
			if (!*s)
				return;
			*o++ = *s++;
		}
		*o = 0;

		if (!strcmp (key, pkey) )
		{
			strcpy (start, s);	// remove this part
			return;
		}

		if (!*s)
			return;
	}

}

/*
==================
Info_Validate

Some characters are illegal in info strings because they
can mess up the server's parsing
==================
*/
qboolean Info_Validate( const char *s ) {
	if ( strchr( s, '\"' ) ) {
		return qfalse;
	}
	if ( strchr( s, ';' ) ) {
		return qfalse;
	}
	return qtrue;
}

/*
==================
Info_SetValueForKey

Changes or adds a key/value pair
==================
*/
void Info_SetValueForKey( char *s, const char *key, const char *value ) {
	char	newi[MAX_INFO_STRING];
	const char* blacklist = "\\;\"";

	if ( strlen( s ) >= MAX_INFO_STRING ) {
		Com_Error( ERR_DROP, "Info_SetValueForKey: oversize infostring" );
	}

	for(; *blacklist; ++blacklist)
	{
		if (strchr (key, *blacklist) || strchr (value, *blacklist))
		{
			Com_Printf (S_COLOR_YELLOW "Can't use keys or values with a '%c': %s = %s\n", *blacklist, key, value);
			return;
		}
	}
	
	Info_RemoveKey (s, key);
	if (!value || !strlen(value))
		return;

	Com_sprintf (newi, sizeof(newi), "\\%s\\%s", key, value);

	if (strlen(newi) + strlen(s) >= MAX_INFO_STRING)
	{
		Com_Printf ("Info string length exceeded\n");
		return;
	}

	strcat (newi, s);
	strcpy (s, newi);
}

/*
==================
Info_SetValueForKey_Big

Changes or adds a key/value pair
==================
*/
void Info_SetValueForKey_Big( char *s, const char *key, const char *value ) {
	char	newi[BIG_INFO_STRING];
	const char* blacklist = "\\;\"";

	if ( strlen( s ) >= BIG_INFO_STRING ) {
		Com_Error( ERR_DROP, "Info_SetValueForKey: oversize infostring" );
	}

	for(; *blacklist; ++blacklist)
	{
		if (strchr (key, *blacklist) || strchr (value, *blacklist))
		{
			Com_Printf (S_COLOR_YELLOW "Can't use keys or values with a '%c': %s = %s\n", *blacklist, key, value);
			return;
		}
	}

	Info_RemoveKey_Big (s, key);
	if (!value || !strlen(value))
		return;

	Com_sprintf (newi, sizeof(newi), "\\%s\\%s", key, value);

	if (strlen(newi) + strlen(s) >= BIG_INFO_STRING)
	{
		Com_Printf ("BIG Info string length exceeded\n");
		return;
	}

	strcat (s, newi);
}

/*
==================
Com_CharIsOneOfCharset
==================
*/
static qboolean Com_CharIsOneOfCharset( char c, char *set )
{
	int i;

	for( i = 0; i < strlen( set ); i++ )
	{
		if( set[ i ] == c )
			return qtrue;
	}

	return qfalse;
}

/*
==================
COM_LoadLevelScores
Loads the current highscores for a level
==================
*/

highscores_t COM_LoadLevelScores( char *levelname ) {
	highscores_t	highScores;
	playerscore_t	scores;
	char			*filename;
	int				len;
	int				i;
	fileHandle_t	f;
	char			buf[MAX_INFO_STRING];

	filename = va("games/%s-1.1.epgame", levelname);
	len = trap_FS_FOpenFile( filename, &f, FS_READ );
	if ( len > 0 ) {
		trap_FS_Read( buf, len, f );
		trap_FS_FCloseFile( f );

		for ( i = 0; i < SCOREBOARD_LENGTH; i++ ) {
			if ( strlen(Info_ValueForKey(buf, va("%i%s", i, SIK_TOTALSCORE))) > 0 ) {
				highScores.highscores[i].accuracy = atoi(Info_ValueForKey(buf, va("%i%s", i, SIK_ACCURACY)));
				highScores.highscores[i].accuracyScore = atoi(Info_ValueForKey(buf, va("%i%s", i, SIK_ACCURACYSCORE)));
				highScores.highscores[i].carnageScore = atoi(Info_ValueForKey(buf, va("%i%s", i, SIK_CARNAGESCORE)));
				highScores.highscores[i].deaths = atoi(Info_ValueForKey(buf, va("%i%s", i, SIK_DEATHS)));
				highScores.highscores[i].deathsScore = atoi(Info_ValueForKey(buf, va("%i%s", i, SIK_DEATHSSCORE)));
				highScores.highscores[i].secretsCount = atoi(Info_ValueForKey(buf, va("%i%s", i, SIK_SECRETSCOUNT)));
				highScores.highscores[i].secretsFound = atoi(Info_ValueForKey(buf, va("%i%s", i, SIK_SECRETSFOUND)));
				highScores.highscores[i].secretsScore = atoi(Info_ValueForKey(buf, va("%i%s", i, SIK_SECRETSSCORE)));
				highScores.highscores[i].skill = atof(Info_ValueForKey(buf, va("%i%s", i, SIK_SKILL)));
				highScores.highscores[i].skillModifier = atoi(Info_ValueForKey(buf, va("%i%s", i, SIK_SKILLMODIFIER)));
				highScores.highscores[i].skillScore = atoi(Info_ValueForKey(buf, va("%i%s", i, SIK_SKILLSCORE)));
				highScores.highscores[i].totalScore = atoi(Info_ValueForKey(buf, va("%i%s", i, SIK_TOTALSCORE)));
			} else {
				highScores.highscores[i].accuracy = 0;
				highScores.highscores[i].accuracyScore = 0;
				highScores.highscores[i].carnageScore = 0;
				highScores.highscores[i].deaths = 0;
				highScores.highscores[i].deathsScore = 0;
				highScores.highscores[i].secretsCount = 0;
				highScores.highscores[i].secretsFound = 0;
				highScores.highscores[i].secretsScore = 0;
				highScores.highscores[i].skill = 0;
				highScores.highscores[i].skillModifier = 0;
				highScores.highscores[i].skillScore = 0;
				highScores.highscores[i].totalScore = 0;
			}
		}
	} else {
		//Com_Printf("No highscore file found for %s\n", filename);
		for ( i = 0; i < SCOREBOARD_LENGTH; i++ ) {
			highScores.highscores[i].accuracy = 0;
			highScores.highscores[i].accuracyScore = 0;
			highScores.highscores[i].carnageScore = 0;
			highScores.highscores[i].deaths = 0;
			highScores.highscores[i].deathsScore = 0;
			highScores.highscores[i].secretsCount = 0;
			highScores.highscores[i].secretsFound = 0;
			highScores.highscores[i].secretsScore = 0;
			highScores.highscores[i].skill = 0;
			highScores.highscores[i].skillModifier = 0;
			highScores.highscores[i].skillScore = 0;
			highScores.highscores[i].totalScore = 0;
		}
	}

	return highScores;
}

/*
==================
COM_AccuracyToScore
Calculates the score based on accuracy
==================
*/
int COM_AccuracyToScore(int accuracy, int score) {
	return ((score * SCORE_ACCURACY) / 100) * accuracy;
}

/*
==================
COM_CalculateScore
Calculates the player's score
==================
*/
playerscore_t COM_CalculatePlayerScore(int persistant[MAX_PERSISTANT], int accuracy, float skill) {
	char			var[MAX_TOKEN_CHARS];
	int				mutators;
	playerscore_t	scores;

	//carnage score
	scores.carnageScore = persistant[PERS_SCORE];

	//accuracy bonus
	scores.accuracy = accuracy;
	scores.accuracyScore = COM_AccuracyToScore( accuracy, scores.carnageScore );

	//skill bonus
	scores.skill = skill;
	scores.skillModifier = (skill - 1) * SCORE_SKILL;
	scores.skillScore = scores.carnageScore * scores.skillModifier;

	//secrets
	scores.secretsFound = persistant[PERS_SECRETS] & 0x7F;
	scores.secretsCount = (persistant[PERS_SECRETS] >> 7) & 0x7F;
	scores.secretsScore = (scores.secretsFound * SCORE_SECRET) * scores.carnageScore;

	//deaths
	scores.deaths = persistant[PERS_KILLED];
	scores.deathsScore = 0 - ((scores.deaths * SCORE_DEATH) * scores.carnageScore);

	//total score
	scores.totalScore = scores.carnageScore + scores.accuracyScore + scores.skillScore + scores.secretsScore + scores.deathsScore ;

	return scores;
}

float VectorDistance(const vec3_t v1, const vec3_t v2) {
    vec3_t diff;
    
    diff[0] = v1[0] - v2[0];
    diff[1] = v1[1] - v2[1];
    diff[2] = v1[2] - v2[2];
    
    return sqrt(diff[0] * diff[0] + diff[1] * diff[1] + diff[2] * diff[2]);
}

/*
==========================
FS_FileExists
==========================
*/
static qboolean	FS_FileExists(const char *filename) {
	int len;

	len = trap_FS_FOpenFile( filename, NULL, FS_READ );
	if (len>0) {
		return qtrue;
	}
	return qfalse;
}
