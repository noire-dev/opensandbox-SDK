// Copyright (C) 1999-2005 ID Software, Inc.
// Copyright (C) 2023-2025 Noire.dev
// OpenSandbox — GPLv2; see LICENSE for details.

#include "g_local.h"
#include "botlib.h"

#include "ai_main.h"
#include "ai_dmq3.h"
#include "ai_chat.h"
#include "ai_cmd.h"
#include "ai_dmnet.h"
#include "ai_team.h"
//data file headers
#include "chars.h"			//characteristics
#include "inv.h"			//indexes into the inventory
#include "syn.h"			//synonyms
#include "match.h"			//string matching types and vars

int AINode_Nothing(bot_state_t *bs) {
	return qtrue;
}

int AINode_Respawn(bot_state_t *bs) {
	if (!BotIsDead(bs)) {
		bs->ainode = AINode_Seek_LTG;
	} else {
	    trap_BotResetMoveState(bs->ms);
	    trap_BotResetGoalState(bs->gs);
	    trap_BotResetAvoidGoals(bs->gs);
	    trap_BotResetAvoidReach(bs->ms);
		trap_EA_Respawn(bs->client);
	}

	return qtrue;
}

void BotAimAtMovementDirection(bot_state_t *bs) {
    bot_input_t bi;
    vec3_t temp_dir;
    
    // Получаем текущий ввод бота
    trap_EA_GetInput(bs->client, (float) bs->thinktime / 1000, &bi);
    
    // Если бот не двигается, выходим
    if (VectorLength(bi.dir) < 0.1f) return;
    
    // Создаем временный вектор направления для вычисления углов
    VectorCopy(bi.dir, temp_dir);
    
    // Обнуляем вертикальную компоненту, чтобы смотреть только по горизонтали
    temp_dir[2] = 0;
    
    // Нормализуем вектор, если он не нулевой
    if (VectorNormalize(temp_dir) > 0) {
        // Преобразуем направление в углы
        vectoangles(temp_dir, bs->ideal_viewangles);
        
        // Сохраняем текущий вертикальный угол, меняем только горизонтальный
        bs->ideal_viewangles[PITCH] = bs->viewangles[PITCH];
        bs->ideal_viewangles[ROLL] = 0; // Обнуляем крен
        
        // Копируем идеальные углы в текущие
        VectorCopy(bs->ideal_viewangles, bs->viewangles);
        
        // Применяем новые углы обзора
        trap_EA_View(bs->client, bs->viewangles);
    }
}

bot_goal_t AI_GoalFromRandomItem(bot_state_t *bs){
    bot_goal_t result;
    gentity_t* item;
    
    memset(&result, 0, sizeof(bot_goal_t));
    
    item = FindRandomItem();
    if (item) {
        G_Printf("Goal finded: %s \n", item->classname);
        VectorCopy(item->r.currentOrigin, result.origin);
        result.areanum = BotPointAreaNum(item->r.currentOrigin);
        VectorSet(result.mins, -8, -8, -8);
		VectorSet(result.maxs, 8, 8, 8);
        result.entitynum = item->s.number;
    }
    
    return result;
}

int AINode_Seek_LTG(bot_state_t *bs) {
	vec3_t target, dir;
	bot_moveresult_t moveresult;

	if (BotIsObserver(bs)) {
		bs->ainode = AINode_Nothing;
		return qfalse;
	}
	if (BotIntermission(bs)) {
		bs->ainode = AINode_Nothing;
		return qfalse;
	}
	if (BotIsDead(bs)) {
		bs->ainode = AINode_Respawn;
		return qfalse;
	}

	bs->tfl = TFL_DEFAULT;
	
	if (BotInLavaOrSlime(bs)) bs->tfl |= TFL_LAVA|TFL_SLIME;
	
	bs->enemy = -1;     //no enemy
	if (BotFindEnemy(bs, -1)) {
		bs->ainode = AINode_Battle_Fight;
		return qfalse;
	}
	
    if (level.time > bs->item_searchtime || trap_BotTouchingGoal(bs->origin, &bs->maingoal)) {
        bs->maingoal = AI_GoalFromRandomItem(bs);
        bs->item_searchtime = level.time + 30000;
    }
	
	if(bs->maingoal.entitynum){
	    BotSetupForMovement(bs);
	    trap_BotMoveToGoal(&moveresult, bs->ms, &bs->maingoal, bs->tfl);
	    BotAimAtMovementDirection(bs);
	}
	return qtrue;
}

int AINode_Battle_Fight(bot_state_t *bs) {
	int areanum;
	vec3_t target;
	aas_entityinfo_t entinfo;
	bot_moveresult_t moveresult;

	BotFindEnemy(bs, bs->enemy);

	if (bs->enemy < 0) {
		bs->ainode = AINode_Seek_LTG;
		return qfalse;
	}

	BotEntityInfo(bs->enemy, &entinfo);

	//if the enemy is invisible and not shooting the bot looses track easily
	if (EntityIsInvisible(&entinfo) && !EntityIsShooting(&entinfo)) {
		if (random() < 0.2) {
			bs->ainode = AINode_Seek_LTG;
			return qfalse;
		}
	}
	
	VectorCopy(entinfo.origin, target);
	
	//update the reachability area and origin if possible
	areanum = BotPointAreaNum(target);
	if (areanum && trap_AAS_AreaReachability(areanum)) {
		VectorCopy(target, bs->lastenemyorigin);
		bs->lastenemyareanum = areanum;
	}

	//if the enemy is not visible
	if (!BotEntityVisible(bs->entitynum, bs->eye, bs->viewangles, 360, bs->enemy) && bs->npcType != NT_NEXTBOT) {
		bs->ainode = AINode_Seek_LTG;
		return qfalse;
	}

	bs->tfl = TFL_DEFAULT;

	if (BotInLavaOrSlime(bs)) bs->tfl |= TFL_LAVA|TFL_SLIME;

	BotChooseWeapon(bs);
	BotAttackMove(bs, bs->tfl);
	BotAimAtEnemy(bs);
	BotCheckAttack(bs);
	return qtrue;
}
