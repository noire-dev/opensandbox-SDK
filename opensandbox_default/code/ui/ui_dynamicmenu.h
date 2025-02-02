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

#define PT_FRIENDLY 			1
#define PT_ENEMY 				2
#define PT_BOTONLY				4
#define PT_PLAYERONLY			8
#define PT_EXCLUDEPARENT		16
#define PT_EXCLUDEGRANDPARENT	32
#define PT_ALL					64




typedef void (*createHandler)(void);
typedef void (*eventHandler)(int index);


// manipulate menu
qboolean DynamicMenu_AddItem( const char* string, int id, createHandler crh, eventHandler evh);
qboolean DynamicMenu_AddIconItem( const char* string, int id, const char* icon, createHandler crh, eventHandler evh);
void DynamicMenu_AddBackground( const char* background);

qboolean DynamicMenu_SubMenuInit( void);
void DynamicMenu_FinishSubMenuInit( void );
void DynamicMenu_MenuInit( qboolean fullscreen, qboolean wraparound );

void DynamicMenu_ClearFocus( int pos );
void DynamicMenu_SetFocus( int pos );

void DynamicMenu_SetFlags(int depth, int id, int flags);

int DynamicMenu_ServerGametype(void);

//
// information about the menu structure
//

// general
int DynamicMenu_Depth(void);

// takes an index and returns data
qboolean DynamicMenu_OnActiveList( int index );
const char* DynamicMenu_StringAtIndex(int index);
int DynamicMenu_DepthOfIndex( int index );
int DynamicMenu_IdAtIndex(int index);

// takes a depth and returns data
int DynamicMenu_ActiveIdAtDepth(int depth);
int DynamicMenu_ActiveIndexAtDepth(int depth);


// returns data about items
const char* DynamicMenu_ItemShortname(int index);



// core services
void DynamicMenu_AddListOfPlayers( int type, createHandler crh, eventHandler evh );
void DynamicMenu_AddListOfItems( int exclude, createHandler crh, eventHandler evh );

