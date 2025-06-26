// Copyright (C) 1999-2005 ID Software, Inc.
// Copyright (C) 2023-2025 Noire.dev
// Copyright (C) 2025 OpenSandbox Team
// OpenSandbox — GPLv2; see LICENSE for details.

int BotChat_EnterGame(bot_state_t *bs);
int BotChat_ExitGame(bot_state_t *bs);
int BotChat_StartLevel(bot_state_t *bs);
int BotChat_EndLevel(bot_state_t *bs);
int BotChat_HitTalking(bot_state_t *bs);
int BotChat_HitNoDeath(bot_state_t *bs);
int BotChat_HitNoKill(bot_state_t *bs);
int BotChat_Death(bot_state_t *bs);
int BotChat_Kill(bot_state_t *bs);
int BotChat_EnemySuicide(bot_state_t *bs);
int BotChat_Random(bot_state_t *bs);
// time the selected chat takes to type in
float BotChatTime(bot_state_t *bs);
// returns true if the bot can chat at the current position
int BotValidChatPosition(bot_state_t *bs);
// test the initial bot chats
void BotChatTest(bot_state_t *bs);

