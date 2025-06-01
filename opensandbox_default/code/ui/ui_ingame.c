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

#include "ui_local.h"
#include "ui_dynamicmenu.h"

/*
=================
UI_CurrentPlayerTeam
=================
*/
int UI_CurrentPlayerTeam( void )
{
	static uiClientState_t	cs;
	static char	info[MAX_INFO_STRING];

	trap_GetClientState( &cs );
	trap_GetConfigString( CS_PLAYERS + cs.clientNum, info, MAX_INFO_STRING );
	return atoi(Info_ValueForKey(info, "t"));
}
