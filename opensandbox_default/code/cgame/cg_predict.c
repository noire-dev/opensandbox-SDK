// Copyright (C) 1999-2005 ID Software, Inc.
// Copyright (C) 2023-2025 Noire.dev
// Copyright (C) 2025 OpenSandbox Team
// OpenSandbox — GPLv2; see LICENSE for details.

#include "../qcommon/ns_local.h"

static pmove_t cg_pmove;

static int cg_numSolidEntities;
static centity_t *cg_solidEntities[MAX_ENTITIES_IN_SNAPSHOT];
static int cg_numTriggerEntities;
static centity_t *cg_triggerEntities[MAX_ENTITIES_IN_SNAPSHOT];

/*
====================
CG_BuildSolidList

When a new cg.snap has been set, this function builds a sublist
of the entities that are actually solid, to make for more
efficient collision detection
====================
*/
void CG_BuildSolidList(void) {
	int i;
	centity_t *cent;
	snapshot_t *snap;
	entityState_t *ent;

	cg_numSolidEntities = 0;
	cg_numTriggerEntities = 0;

	if(cg.nextSnap && !cg.nextFrameTeleport && !cg.thisFrameTeleport) {
		snap = cg.nextSnap;
	} else {
		snap = cg.snap;
	}

	for(i = 0; i < snap->numEntities; i++) {
		cent = &cg_entities[snap->entities[i].number];
		ent = &cent->currentState;

		if(ent->eType == ET_ITEM || ent->eType == ET_PUSH_TRIGGER || ent->eType == ET_TELEPORT_TRIGGER) {
			cg_triggerEntities[cg_numTriggerEntities] = cent;
			cg_numTriggerEntities++;
			continue;
		}

		if(cent->nextState.solid) {
			cg_solidEntities[cg_numSolidEntities] = cent;
			cg_numSolidEntities++;
			continue;
		}
	}
}

static void CG_ClipMoveToEntities(const vec3_t start, const vec3_t mins, const vec3_t maxs, const vec3_t end, int skipNumber, int mask, trace_t *tr, qboolean skip) {
	int i, x, zd, zu;
	trace_t trace;
	entityState_t *ent;
	clipHandle_t cmodel;
	vec3_t bmins, bmaxs;
	vec3_t origin, angles;
	centity_t *cent;

	for(i = 0; i < cg_numSolidEntities; i++) {
		cent = cg_solidEntities[i];
		ent = &cent->currentState;

		if(ent->number == skipNumber) {
			continue;
		}

		// FIXME: AABB colisions is coded to ONE INT, instead two vec3
		if(ent->solid == SOLID_BMODEL) {
			// special value for bmodel
			cmodel = trap_CM_InlineModel(ent->modelindex);
			VectorCopy(cent->lerpAngles, angles);
			BG_EvaluateTrajectory(&cent->currentState.pos, cg.physicsTime, origin);
		} else {
			// encoded bbox
			x = (ent->solid & 255);
			zd = ((ent->solid >> 8) & 255);
			zu = ((ent->solid >> 16) & 255) - 32;

			bmins[0] = bmins[1] = -x;
			bmaxs[0] = bmaxs[1] = x;
			bmins[2] = -zd;
			bmaxs[2] = zu;

			cmodel = trap_CM_TempBoxModel(bmins, bmaxs);
			VectorCopy(cent->lerpAngles, angles);
			VectorCopy(cent->lerpOrigin, origin);
		}

		trap_CM_TransformedBoxTrace(&trace, start, end, mins, maxs, cmodel, mask, origin, angles);

		if(trace.allsolid || trace.fraction < tr->fraction) {
			trace.entityNum = ent->number;
			*tr = trace;
		} else if(trace.startsolid) {
			tr->startsolid = qtrue;
		}
		if(tr->allsolid) {
			return;
		}
	}
}

void CG_Trace(trace_t *result, const vec3_t start, const vec3_t mins, const vec3_t maxs, const vec3_t end, int skipNumber, int mask) {
	trace_t t;

	trap_CM_BoxTrace(&t, start, end, mins, maxs, 0, mask);
	t.entityNum = t.fraction != 1.0 ? ENTITYNUM_WORLD : ENTITYNUM_NONE;
	// check all other solid models
	CG_ClipMoveToEntities(start, mins, maxs, end, skipNumber, mask, &t, qfalse);

	*result = t;
}

int CG_PointContents(const vec3_t point, int passEntityNum) {
	int i;
	entityState_t *ent;
	centity_t *cent;
	clipHandle_t cmodel;
	int contents;

	contents = trap_CM_PointContents(point, 0);

	for(i = 0; i < cg_numSolidEntities; i++) {
		cent = cg_solidEntities[i];

		ent = &cent->currentState;

		if(ent->number == passEntityNum) {
			continue;
		}

		if(ent->solid != SOLID_BMODEL) {  // special value for bmodel
			continue;
		}

		cmodel = trap_CM_InlineModel(ent->modelindex);
		if(!cmodel) {
			continue;
		}

		contents |= trap_CM_TransformedPointContents(point, cmodel, cent->lerpOrigin, cent->lerpAngles);
	}

	return contents;
}

/*
========================
CG_InterpolatePlayerState

Generates cg.predictedPlayerState by interpolating between
cg.snap->player_state and cg.nextFrame->player_state
========================
*/
static void CG_InterpolatePlayerState(qboolean grabAngles) {
	float f;
	int i;
	playerState_t *out;
	snapshot_t *prev, *next;

	out = &cg.predictedPlayerState;
	prev = cg.snap;
	next = cg.nextSnap;

	*out = cg.snap->ps;

	// if we are still allowing local input, short circuit the view angles
	if(grabAngles) {
		usercmd_t cmd;
		int cmdNum;

		cmdNum = trap_GetCurrentCmdNumber();
		trap_GetUserCmd(cmdNum, &cmd);

		PM_UpdateViewAngles(out, &cmd);
	}

	// if the next frame is a teleport, we can't lerp to it
	if(cg.nextFrameTeleport) {
		return;
	}

	if(!next || next->serverTime <= prev->serverTime) {
		return;
	}

	f = (float)(cg.time - prev->serverTime) / (next->serverTime - prev->serverTime);

	i = next->ps.bobCycle;
	if(i < prev->ps.bobCycle) {
		i += 256;
	}
	out->bobCycle = prev->ps.bobCycle + f * (i - prev->ps.bobCycle);

	for(i = 0; i < 3; i++) {
		out->origin[i] = prev->ps.origin[i] + f * (next->ps.origin[i] - prev->ps.origin[i]);
		if(!grabAngles) {
			out->viewangles[i] = LerpAngle(prev->ps.viewangles[i], next->ps.viewangles[i], f);
		}
		out->velocity[i] = prev->ps.velocity[i] + f * (next->ps.velocity[i] - prev->ps.velocity[i]);
	}
}

static void CG_TouchItem(centity_t *cent) {
	item_t *item;
	// For instantgib
	qboolean canBePicked;

	// normally we can
	canBePicked = qtrue;

	if(!BG_PlayerTouchesItem(&cg.predictedPlayerState, &cent->currentState, cg.time)) {
		return;
	}

	// never pick an item up twice in a prediction
	if(cent->miscTime == cg.time) {
		return;
	}

	if(!BG_CanItemBeGrabbed(cgs.gametype, &cent->currentState, &cg.predictedPlayerState)) {
		return;  // can't hold it
	}

	item = &gameInfoItems[cent->currentState.modelindex];

	// Special case for flags.
	// We don't predict touching our own flag
	if(cgs.gametype == GT_1FCTF) {
		if(item->giTag != PW_NEUTRALFLAG) {
			return;
		}
	}
	if(cgs.gametype == GT_CTF || cgs.gametype == GT_HARVESTER) {
		if(cg.predictedPlayerState.persistant[PERS_TEAM] == TEAM_RED && item->giTag == PW_REDFLAG) return;
		if(cg.predictedPlayerState.persistant[PERS_TEAM] == TEAM_BLUE && item->giTag == PW_BLUEFLAG) return;
		if(cg.predictedPlayerState.persistant[PERS_TEAM] == TEAM_RED && item->giTag == PW_BLUEFLAG) canBePicked = qtrue;
		if(cg.predictedPlayerState.persistant[PERS_TEAM] == TEAM_BLUE && item->giTag == PW_REDFLAG) canBePicked = qtrue;
	}

	// grab it
	if(canBePicked) {
		BG_AddPredictableEventToPlayerstate(EV_ITEM_PICKUP, cent->currentState.modelindex, &cg.predictedPlayerState);

		// remove it from the frame so it won't be drawn
		cent->currentState.eFlags |= EF_NODRAW;

		// don't touch it again this prediction
		cent->miscTime = cg.time;
	}
}

/*
=========================
CG_TouchTriggerPrediction

Predict push triggers and items
=========================
*/
static void CG_TouchTriggerPrediction(void) {
	int i;
	trace_t trace;
	entityState_t *ent;
	clipHandle_t cmodel;
	centity_t *cent;
	qboolean spectator;

	// dead clients don't activate triggers
	if(cg.predictedPlayerState.stats[STAT_HEALTH] <= 0) {
		return;
	}

	spectator = (cg.predictedPlayerState.pm_type == PM_SPECTATOR);

	if(cg.predictedPlayerState.pm_type != PM_NORMAL && !spectator) {
		return;
	}

	for(i = 0; i < cg_numTriggerEntities; i++) {
		cent = cg_triggerEntities[i];
		ent = &cent->currentState;

		if(ent->eType == ET_ITEM && !spectator) {
			CG_TouchItem(cent);
			continue;
		}

		if(ent->solid != SOLID_BMODEL) {
			continue;
		}

		cmodel = trap_CM_InlineModel(ent->modelindex);
		if(!cmodel) {
			continue;
		}

		trap_CM_BoxTrace(&trace, cg.predictedPlayerState.origin, cg.predictedPlayerState.origin, cg_pmove.mins, cg_pmove.maxs, cmodel, -1);

		if(!trace.startsolid) {
			continue;
		}

		if(ent->eType == ET_TELEPORT_TRIGGER) {
			cg.hyperspace = qtrue;
		} else if(ent->eType == ET_PUSH_TRIGGER) {
			BG_TouchJumpPad(&cg.predictedPlayerState, ent);
		}
	}

	// if we didn't touch a jump pad this pmove frame
	if(cg.predictedPlayerState.jumppad_frame != cg.predictedPlayerState.pmove_framecount) {
		cg.predictedPlayerState.jumppad_frame = 0;
		cg.predictedPlayerState.jumppad_ent = 0;
	}
}

/*
=================
CG_PredictPlayerState

Generates cg.predictedPlayerState for the current cg.time
cg.predictedPlayerState is guaranteed to be valid after exiting.
=================
*/
void CG_PredictPlayerState(void) {
	int cmdNum, current;
	playerState_t oldPlayerState;
	qboolean moved;
	usercmd_t oldestCmd;
	usercmd_t latestCmd;

	cg.hyperspace = qfalse;  // will be set if touching a trigger_teleport

	// if this is the first frame we must guarantee
	// predictedPlayerState is valid even if there is some
	// other error condition
	if(!cg.validPPS) {
		cg.validPPS = qtrue;
		cg.predictedPlayerState = cg.snap->ps;
	}

	// demo playback just copies the moves
	if(cg.demoPlayback || (cg.snap->ps.pm_flags & PMF_FOLLOW)) {
		CG_InterpolatePlayerState(qfalse);
		return;
	}

	// prepare for pmove
	cg_pmove.ps = &cg.predictedPlayerState;
	cg_pmove.trace = CG_Trace;
	cg_pmove.pointcontents = CG_PointContents;
	if(cg_pmove.ps->pm_type == PM_DEAD) {
		cg_pmove.tracemask = MASK_PLAYERSOLID & ~CONTENTS_BODY;
	} else {
		cg_pmove.tracemask = MASK_PLAYERSOLID;
	}

	if(cg.snap->ps.persistant[PERS_TEAM] == TEAM_SPECTATOR) {
		cg_pmove.tracemask &= ~CONTENTS_BODY;  // spectators can fly through bodies
	}

	// save the state before the pmove so we can detect transitions
	oldPlayerState = cg.predictedPlayerState;

	current = trap_GetCurrentCmdNumber();

	// if we don't have the commands right after the snapshot, we
	// can't accurately predict a current position, so just freeze at
	// the last good position we had
	cmdNum = current - CMD_BACKUP + 1;
	trap_GetUserCmd(cmdNum, &oldestCmd);
	if(oldestCmd.serverTime > cg.snap->ps.commandTime && oldestCmd.serverTime < cg.time) return;  // special check for map_restart

	// get the latest command so we can know which commands are from previous map_restarts
	trap_GetUserCmd(current, &latestCmd);

	// get the most recent information we have, even if
	// the server time is beyond our current cg.time,
	// because predicted player positions are going to
	// be ahead of everything else anyway
	if(cg.nextSnap && !cg.nextFrameTeleport && !cg.thisFrameTeleport) {
		cg.predictedPlayerState = cg.nextSnap->ps;
		cg.physicsTime = cg.nextSnap->serverTime;
	} else {
		cg.predictedPlayerState = cg.snap->ps;
		cg.physicsTime = cg.snap->serverTime;
	}

	// run cmds
	moved = qfalse;
	for(cmdNum = current - CMD_BACKUP + 1; cmdNum <= current; cmdNum++) {
		// get the command
		trap_GetUserCmd(cmdNum, &cg_pmove.cmd);

		if(trap_Key_GetCatcher() == KEYCATCH_UI || trap_Key_GetCatcher() & KEYCATCH_CONSOLE) {
			cg.savedSens = 0;
		} else {
			if(cg_pmove.cmd.buttons & BUTTON_GESTURE && cg_pmove.cmd.buttons & BUTTON_ATTACK && cg_pmove.cmd.weapon == WP_PHYSGUN) {
				NS_setCvar("sensitivity", "0.025");
			} else {
				if(cg.savedSens && cg.savedSens != 0.025) NS_setCvar("sensitivity", va("%.3f", cg.savedSens));
				if(cg.savedSens != 0.025) cg.savedSens = get_cvar_float("sensitivity");
			}
		}

		// don't do anything if the time is before the snapshot player time
		if(cg_pmove.cmd.serverTime <= cg.predictedPlayerState.commandTime) continue;

		// don't do anything if the command was from a previous map_restart
		if(cg_pmove.cmd.serverTime > latestCmd.serverTime) continue;

		// check for a prediction error from last frame
		// on a lan, this will often be the exact value
		// from the snapshot, but on a wan we will have
		// to predict several commands to get to the point
		// we want to compare
		if(cg.predictedPlayerState.commandTime == oldPlayerState.commandTime) {
			vec3_t delta;
			float len;

			if(cg.thisFrameTeleport) {
				// a teleport will not cause an error decay
				VectorClear(cg.predictedError);
				cg.thisFrameTeleport = qfalse;
			} else {
				vec3_t adjusted;
				CG_AdjustPositionForMover(cg.predictedPlayerState.origin, cg.predictedPlayerState.groundEntityNum, cg.physicsTime, cg.oldTime, adjusted);
				VectorSubtract(oldPlayerState.origin, adjusted, delta);
				len = VectorLength(delta);
				if(len > 0.1) {
					VectorClear(cg.predictedError);
					VectorAdd(delta, cg.predictedError, cg.predictedError);
					cg.predictedErrorTime = cg.oldTime;
				}
			}
		}

		// don't predict gauntlet firing, which is only supposed to happen
		// when it actually inflicts damage
		cg_pmove.gauntletHit = qfalse;

		// run the Pmove
		Pmove(&cg_pmove);

		moved = qtrue;

		// add push trigger movement effects
		CG_TouchTriggerPrediction();
	}

	if(!moved) return;

	// adjust for the movement of the groundentity
	CG_AdjustPositionForMover(cg.predictedPlayerState.origin, cg.predictedPlayerState.groundEntityNum, cg.physicsTime, cg.time, cg.predictedPlayerState.origin);

	// fire events and other transition triggered things
	CG_TransitionPlayerState(&cg.predictedPlayerState, &oldPlayerState);
}
