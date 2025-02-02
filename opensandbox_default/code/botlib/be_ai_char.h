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

/*****************************************************************************
 * name:		be_ai_char.h
 *
 * desc:		bot characters
 *
 * $Archive: /source/code/botlib/be_ai_char.h $
 *
 *****************************************************************************/

//loads a bot character from a file
int BotLoadCharacter(char *charfile, float skill);
//frees a bot character
void BotFreeCharacter(int character);
//returns a float characteristic
float Characteristic_Float(int character, int index);
//returns a bounded float characteristic
float Characteristic_BFloat(int character, int index, float min, float max);
//returns an integer characteristic
int Characteristic_Integer(int character, int index);
//returns a bounded integer characteristic
int Characteristic_BInteger(int character, int index, int min, int max);
//returns a string characteristic
void Characteristic_String(int character, int index, char *buf, int size);
//free cached bot characters
void BotShutdownCharacters(void);
