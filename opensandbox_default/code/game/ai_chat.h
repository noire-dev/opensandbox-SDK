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
 * name:		ai_chat.h
 *
 * desc:		Quake3 bot AI
 *
 * $Archive: /source/code/botai/ai_chat.c $
 *
 *****************************************************************************/

//
int BotChat_EnterGame(bot_state_t *bs);
//
int BotChat_ExitGame(bot_state_t *bs);
//
int BotChat_StartLevel(bot_state_t *bs);
//
int BotChat_EndLevel(bot_state_t *bs);
//
int BotChat_HitTalking(bot_state_t *bs);
//
int BotChat_HitNoDeath(bot_state_t *bs);
//
int BotChat_HitNoKill(bot_state_t *bs);
//
int BotChat_Death(bot_state_t *bs);
//
int BotChat_Kill(bot_state_t *bs);
//
int BotChat_EnemySuicide(bot_state_t *bs);
//
int BotChat_Random(bot_state_t *bs);
// time the selected chat takes to type in
float BotChatTime(bot_state_t *bs);
// returns true if the bot can chat at the current position
int BotValidChatPosition(bot_state_t *bs);
// test the initial bot chats
void BotChatTest(bot_state_t *bs);

