// Copyright (C) 1999-2005 ID Software, Inc.
// Copyright (C) 2023-2025 Noire.dev
// OpenSandbox — GPLv2; see LICENSE for details.

#include "g_local.h"

#include "botlib.h"		//bot lib interface

#include "ai_main.h"
#include "ai_dmq3.h"
#include "ai_chat.h"
#include "ai_cmd.h"


#include "ai_dmnet.h"

#include "sai_local.h"

int AI_Intermission(sai_state_t *bs) {
	if (!level.intermissiontime) bs->state = AI_Stand;

	return qtrue;
}

int AI_Observer(sai_state_t *bs) {
	if (!bs->cur_ps.pm_type != PM_SPECTATOR) bs->state = AI_Stand;

	return qtrue;
}

int AI_Respawn(sai_state_t *bs) {
	if (!bs->cur_ps.pm_type != PM_DEAD){
	    bs->state = AI_Seek;
	} else {
	    trap_EA_Respawn(bs->client);
	}

	return qtrue;
}

int AI_Battle(sai_state_t *bs) {
	aas_entityinfo_t entinfo;
	
	G_Printf("SAI: AI_Battle\n");

	//if there is another better enemy
	SAI_FindEnemy(bs, bs->enemy);

	//if no enemy
	if (bs->enemy < 0) {
		bs->state = AI_Seek;
		return qfalse;
	}

	trap_AAS_EntityInfo(bs->enemy, &entinfo);
	if (EntityIsDead(&entinfo)) {   //if the enemy is dead
		bs->state = AI_Stand;
		return qfalse;
	}
	//if the enemy is invisible and not shooting the bot looses track easily
	if (entinfo.powerups & (1 << PW_INVIS) && !(entinfo.flags & EF_FIRING)) {
		if (random() < 0.2) {
			bs->state = AI_Seek;
			return qfalse;
		}
	}
	
	//if the enemy is not visible
	if (!SAI_EntityVisible(bs->entitynum, bs->eye, bs->enemy) && bs->npcType != NT_NEXTBOT) {
		bs->state = AI_Seek;
		return qfalse;
	}

	bs->tfl = TFL_DEFAULT;
	
	//if in lava or slime the bot should be able to get out
	if (SAI_InLavaOrSlime(bs)) bs->tfl |= TFL_LAVA|TFL_SLIME;

	//choose the best weapon to fight with
	SAI_SelectWeapon(bs);
	
	SAI_AttackMove(bs, bs->tfl);

	SAI_AimAtEnemy(bs);
	SAI_CheckAttack(bs);

	return qtrue;
}

int AI_Seek(sai_state_t *bs) {
	bot_goal_t goal;
	bot_moveresult_t moveresult;
	int range;

    G_Printf("SAI: AI_Seek\n");

	if (bs->cur_ps.pm_type == PM_SPECTATOR) {
		bs->state = AI_Observer;
		return qfalse;
	}

	if (level.intermissiontime) {
		bs->state = AI_Intermission;
		return qfalse;
	}

	if (bs->cur_ps.pm_type == PM_DEAD) {
		bs->state = AI_Respawn;
		return qfalse;
	}

	bs->enemy = -1;     //no enemy

	//if there is an enemy
	if (SAI_FindEnemy(bs, -1)) {
		bs->state = AI_Battle;
		return qfalse;
	}
	
	//check for nearby goals periodicly
	if (bs->check_time < FloatTime()) {
		bs->check_time = FloatTime() + 0.5;
		
		if (SAI_FindItem(bs, bs->tfl, &goal, range)) {
			bs->state = AI_Seek;
			return qfalse;
		}
	}
	
	//initialize the movement state
	SAI_SetupMovement(bs);
	//move towards the goal
	trap_BotMoveToGoal(&moveresult, bs->ms, &goal, bs->tfl);
	//if the movement failed
	if (moveresult.failure) trap_BotResetAvoidReach(bs->ms);
	
	//if the viewangles are used for the movement
	if (moveresult.flags & (MOVERESULT_MOVEMENTVIEWSET|MOVERESULT_MOVEMENTVIEW|MOVERESULT_SWIMVIEW)) {
		VectorCopy(moveresult.ideal_viewangles, bs->ideal_viewangles);
	}

	return qtrue;
}

int AI_Stand(sai_state_t *bs) {
    G_Printf("SAI: AI_Stand\n");
	if (SAI_FindEnemy(bs, -1)) {
		bs->state = AI_Battle;
		return qfalse;
	}

	bs->state = AI_Seek;

	return qtrue;
}
