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

/*****************************************************************************
 * name:		l_libvar.h
 *
 * desc:		botlib vars
 *
 * $Archive: /source/code/botlib/l_libvar.h $
 *
 *****************************************************************************/

//library variable
typedef struct libvar_s
{
	char		*name;
	char		*string;
	int		flags;
	qboolean	modified;	// set each time the cvar is changed
	float		value;
	struct	libvar_s *next;
} libvar_t;

//removes all library variables
void LibVarDeAllocAll(void);
//gets the library variable with the given name
libvar_t *LibVarGet(char *var_name);
//gets the string of the library variable with the given name
char *LibVarGetString(char *var_name);
//gets the value of the library variable with the given name
float LibVarGetValue(char *var_name);
//creates the library variable if not existing already and returns it
libvar_t *LibVar(char *var_name, char *value);
//creates the library variable if not existing already and returns the value
float LibVarValue(char *var_name, char *value);
//creates the library variable if not existing already and returns the value string
char *LibVarString(char *var_name, char *value);
//sets the library variable
void LibVarSet(char *var_name, char *value);
//returns true if the library variable has been modified
qboolean LibVarChanged(char *var_name);
//sets the library variable to unmodified
void LibVarSetNotModified(char *var_name);

