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
 * name:		be_aas_main.h
 *
 * desc:		AAS
 *
 * $Archive: /source/code/botlib/be_aas_main.h $
 *
 *****************************************************************************/

#ifdef AASINTERN

extern aas_t aasworld;

//AAS error message
void QDECL AAS_Error(char *fmt, ...);
//set AAS initialized
void AAS_SetInitialized(void);
//setup AAS with the given number of entities and clients
int AAS_Setup(void);
//shutdown AAS
void AAS_Shutdown(void);
//start a new map
int AAS_LoadMap(const char *mapname);
//start a new time frame
int AAS_StartFrame(float time);
#endif //AASINTERN

//returns true if AAS is initialized
int AAS_Initialized(void);
//returns true if the AAS file is loaded
int AAS_Loaded(void);
//returns the model name from the given index
char *AAS_ModelFromIndex(int index);
//returns the index from the given model name
int AAS_IndexFromModel(char *modelname);
//returns the current time
float AAS_Time(void);
//
void AAS_ProjectPointOntoVector( vec3_t point, vec3_t vStart, vec3_t vEnd, vec3_t vProj );
