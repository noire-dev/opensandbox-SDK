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

#include "g_local.h"

/*
================
readFile_int
This parses an integer for the "tag" specified (cnf)
================
*/
void readFile_int( char **cnf, int *v )
{
  char *t;

  //COM_MatchToken(cnf, "=");
  t = COM_ParseExt( cnf, qfalse );
  if( !strcmp( t, "=" ) )
  {
    t = COM_ParseExt( cnf, qfalse );
  }
  else
  {
    COM_ParseWarning( "expected '=' before \"%s\"", t );
  }
  *v = atoi( t );
}

/*
================
readFile_string
This parses a literal string for the "tag" specified
Color characters and escape sequences are parsed as well. 
================
*/
void readFile_string( char **cnf, char *s, int size )
{
  char *t;

  //COM_MatchToken(cnf, "=");
  s[ 0 ] = '\0';
  t = COM_ParseExt( cnf, qfalse );
  if( strcmp( t, "=" ) )
  {
    COM_ParseWarning( "expected '=' before \"%s\"", t );
    Q_strncpyz( s, t, size );
  }
  while( 1 )
  {
    t = COM_ParseExt( cnf, qfalse );
    if( !*t )
      break;
    if( strlen( t ) + strlen( s ) >= size )
      break;
    if( *s )
      Q_strcat( s, size, " " );
    Q_strcat( s, size, t );
  }
}

/*
================
writeFile_int
This writes an integer to the file.
Since there is no logic as to where it writes, it must be called "just-in-time."  
================
*/
void writeFile_int( int v, fileHandle_t f )
{
  char buf[ 32 ];

  Com_sprintf( buf, sizeof( buf ), "%d", v );
  trap_FS_Write( buf, strlen( buf ), f );
  trap_FS_Write( "\n", 1, f );
}

/*
================
writeFile_string
This writes a string to the file. 
Since there is no logic as to where it writes, it must be called "just-in-time."
================
*/
void writeFile_string( char *s, fileHandle_t f )
{
  char buf[ MAX_STRING_CHARS ];

  buf[ 0 ] = '\0';
  if( s[ 0 ] )
  {
    //Q_strcat(buf, sizeof(buf), s);
    Q_strncpyz( buf, s, sizeof( buf ) );
    trap_FS_Write( buf, strlen( buf ), f );
  }
  trap_FS_Write( "\n", 1, f );
}

