// Copyright (C) 1999-2005 ID Software, Inc.
// Copyright (C) 2023-2025 Noire.dev
// Copyright (C) 2025 OpenSandbox Team
// OpenSandbox — GPLv2; see LICENSE for details.

#include "cg_local.h"

static void CG_Obituary(entityState_t *ent) {
	int mod, target, attacker;
	const char *targetInfo, *attackerInfo;
	char targetName[36], attackerName[36];

	attacker = ent->otherEntityNum;
	target = ent->otherEntityNum2;
	mod = ent->eventParm;

	if(target < 0 || target >= MAX_CLIENTS) CG_Error("CG_Obituary: target out of range");

	if(attacker < 0 || attacker >= MAX_CLIENTS) {
		attackerInfo = NULL;
	} else {
		attackerInfo = CG_ConfigString(CS_PLAYERS + attacker);
	}

	targetInfo = CG_ConfigString(CS_PLAYERS + target);
	if(!targetInfo) return;
	Q_strncpyz(targetName, Info_ValueForKey(targetInfo, "n"), sizeof(targetName) - 2);
	strcat(targetName, S_COLOR_WHITE);

	if(attackerInfo) {
		Q_strncpyz(attackerName, Info_ValueForKey(attackerInfo, "n"), sizeof(attackerName) - 2);
		strcat(attackerName, S_COLOR_WHITE);
	}

	if(attackerInfo) {
		CG_AddNotify(va("%s -> %s", attackerName, targetName), NOTIFY_KILL, mod);
	} else {
		CG_AddNotify(va("%s", targetName), NOTIFY_KILL, mod);
	}
}

static void CG_UseItem(centity_t *cent) {
	int itemNum;
	item_t *item;
	entityState_t *es;

	es = &cent->currentState;

	itemNum = (es->event & ~EV_EVENT_BITS) - EV_USE_ITEM0;
	if(itemNum < 0 || itemNum > HI_NUM_HOLDABLE) itemNum = 0;

	// print a message if the local player
	if(es->number == cg.snap->ps.clientNum) {
		if(!itemNum) {
			CG_CenterPrint("No item to use", SCREEN_HEIGHT * 0.30, BIGCHAR_WIDTH);
		} else {
			item = BG_FindItemForHoldable(itemNum);
			CG_CenterPrint(va("Use %s", item->pickup_name), SCREEN_HEIGHT * 0.30, BIGCHAR_WIDTH);
		}
	}

	switch(itemNum) {
		default:
		case HI_NONE: trap_S_StartSound(NULL, es->number, CHAN_BODY, cgs.media.useNothingSound); break;
		case HI_TELEPORTER: break;
		case HI_MEDKIT: trap_S_StartSound(NULL, es->number, CHAN_BODY, cgs.media.medkitSound); break;
		case HI_KAMIKAZE: break;
		case HI_PORTAL: break;
		case HI_INVULNERABILITY: trap_S_StartSound(NULL, es->number, CHAN_BODY, cgs.media.useInvulnerabilitySound); break;
	}
}

static void CG_ItemPickup(int itemNum) {
	CG_AddNotify(gameInfoItems[itemNum].pickup_name, NOTIFY_ITEM, itemNum);

	// see if it should be the grabbed weapon
	if(gameInfoItems[itemNum].giType == IT_AMMO)
		if(cg.swep_listcl[gameInfoItems[itemNum].giTag] == WS_NOAMMO) cg.swep_listcl[gameInfoItems[itemNum].giTag] = WS_HAVE;
	if(gameInfoItems[itemNum].giType == IT_WEAPON) cg.swep_listcl[gameInfoItems[itemNum].giTag] = WS_HAVE;
}

static int CG_WaterLevel(centity_t *cent) {
	vec3_t point;
	int contents, sample1, sample2, anim, waterlevel;

	// get waterlevel, accounting for ducking
	waterlevel = 0;
	VectorCopy(cent->lerpOrigin, point);
	point[2] += MINS_Z + 1;
	anim = cent->currentState.legsAnim & ~ANIM_TOGGLEBIT;

	if(anim == LEGS_WALKCR || anim == LEGS_IDLECR) {
		point[2] += CROUCH_VIEWHEIGHT;
	} else {
		point[2] += DEFAULT_VIEWHEIGHT;
	}

	contents = CG_PointContents(point, -1);

	if(contents & MASK_WATER) {
		sample2 = point[2] - MINS_Z;
		sample1 = sample2 / 2;
		waterlevel = 1;
		point[2] = cent->lerpOrigin[2] + MINS_Z + sample1;
		contents = CG_PointContents(point, -1);

		if(contents & MASK_WATER) {
			waterlevel = 2;
			point[2] = cent->lerpOrigin[2] + MINS_Z + sample2;
			contents = CG_PointContents(point, -1);
			if(contents & MASK_WATER) waterlevel = 3;
		}
	}

	return waterlevel;
}

void CG_PainEvent(centity_t *cent, int health) {
	char *snd;

	// don't do more than two pain sounds a second
	if(cg.time - cent->pe.painTime < 500) {
		return;
	}

	if(health < 25) {
		snd = "*pain25_1.wav";
	} else if(health < 50) {
		snd = "*pain50_1.wav";
	} else if(health < 75) {
		snd = "*pain75_1.wav";
	} else {
		snd = "*pain100_1.wav";
	}
	// play a gurp sound instead of a normal pain sound
	if(CG_WaterLevel(cent) >= 1) {
		if(rand() & 1) {
			trap_S_StartSound(NULL, cent->currentState.number, CHAN_VOICE, CG_CustomSound(cent->currentState.number, "sound/player/gurp1.wav"));
		} else {
			trap_S_StartSound(NULL, cent->currentState.number, CHAN_VOICE, CG_CustomSound(cent->currentState.number, "sound/player/gurp2.wav"));
		}
	} else {
		trap_S_StartSound(NULL, cent->currentState.number, CHAN_VOICE, CG_CustomSound(cent->currentState.number, snd));
	}
	// save pain time for programitic twitch animation
	cent->pe.painTime = cg.time;
	cent->pe.painDirection ^= 1;
}

/*
==============
CG_EntityEvent

An entity has an event value
also called by CG_CheckPlayerstateEvents
==============
*/
void CG_EntityEvent(centity_t *cent, vec3_t position) {
	entityState_t *es;
	int event;
	vec3_t dir;
	const char *s;
	int clientNum;
	clientInfo_t *ci;
	byte r, g, b;
	int rnum;

	es = &cent->currentState;
	event = es->event & ~EV_EVENT_BITS;

	if(cg_debugEvents.integer) CG_Printf("ent:%3i  event:%3i ", es->number, event);

	if(!event) return;

	clientNum = es->clientNum;
	if(clientNum < 0 || clientNum >= MAX_CLIENTS) clientNum = 0;
	ci = &cgs.clientinfo[clientNum];

	switch(event) {
		case EV_FOOTSTEP: trap_S_StartSound(NULL, es->number, CHAN_BODY, cgs.media.footsteps[ci->footsteps][rand() & 3]); break;
		case EV_FOOTSTEP_METAL: trap_S_StartSound(NULL, es->number, CHAN_BODY, cgs.media.footsteps[FOOTSTEP_METAL][rand() & 3]); break;
		case EV_FOOTSTEP_FLESH: trap_S_StartSound(NULL, es->number, CHAN_BODY, cgs.media.footsteps[FOOTSTEP_FLESH][rand() & 3]); break;
		case EV_FOOTSPLASH: trap_S_StartSound(NULL, es->number, CHAN_BODY, cgs.media.footsteps[FOOTSTEP_SPLASH][rand() & 3]); break;
		case EV_SWIM: trap_S_StartSound(NULL, es->number, CHAN_BODY, cgs.media.footsteps[FOOTSTEP_SPLASH][rand() & 3]); break;
		case EV_FALL_SHORT:
			if(!cg.predictedPlayerState.stats[STAT_VEHICLE]) {
				trap_S_StartSound(NULL, es->number, CHAN_AUTO, cgs.media.landSound);
				if(clientNum == cg.predictedPlayerState.clientNum) {
					// smooth landing z changes
					cg.landChange = -8;
					cg.landTime = cg.time;
				}
			}
			break;
		case EV_FALL_MEDIUM:
			if(!cg.predictedPlayerState.stats[STAT_VEHICLE]) {
				// use normal pain sound
				trap_S_StartSound(NULL, es->number, CHAN_VOICE, CG_CustomSound(es->number, "*pain100_1.wav"));
				if(clientNum == cg.predictedPlayerState.clientNum) {
					// smooth landing z changes
					cg.landChange = -16;
					cg.landTime = cg.time;
				}
			}
			break;
		case EV_FALL_FAR:
			if(!cg.predictedPlayerState.stats[STAT_VEHICLE]) {
				trap_S_StartSound(NULL, es->number, CHAN_AUTO, CG_CustomSound(es->number, "*fall1.wav"));
				cent->pe.painTime = cg.time;  // don't play a pain sound right after this
				if(clientNum == cg.predictedPlayerState.clientNum) {
					// smooth landing z changes
					cg.landChange = -24;
					cg.landTime = cg.time;
				}
			}
			break;
		case EV_STEP_4:
		case EV_STEP_8:
		case EV_STEP_12:
		case EV_STEP_16:  // smooth out step up transitions
		{
			float oldStep;
			int delta;
			int step;

			if(clientNum != cg.predictedPlayerState.clientNum) {
				break;
			}
			// if we are interpolating, we don't need to smooth steps
			if(cg.demoPlayback || (cg.snap->ps.pm_flags & PMF_FOLLOW)) {
				break;
			}
			// check for stepping up before a previous step is completed
			delta = cg.time - cg.stepTime;
			if(delta < STEP_TIME) {
				oldStep = cg.stepChange * (STEP_TIME - delta) / STEP_TIME;
			} else {
				oldStep = 0;
			}

			// add this amount
			step = 4 * (event - EV_STEP_4 + 1);
			cg.stepChange = oldStep + step;
			if(cg.stepChange > MAX_STEP_CHANGE) {
				cg.stepChange = MAX_STEP_CHANGE;
			}
			cg.stepTime = cg.time;
			break;
		}
		case EV_JUMP_PAD:
			// boing sound at origin, jump sound on player
			trap_S_StartSound(cent->lerpOrigin, -1, CHAN_VOICE, cgs.media.jumpPadSound);
			trap_S_StartSound(NULL, es->number, CHAN_VOICE, CG_CustomSound(es->number, "*jump1.wav"));
			break;
		case EV_JUMP: trap_S_StartSound(NULL, es->number, CHAN_VOICE, CG_CustomSound(es->number, "*jump1.wav")); break;
		case EV_TAUNT: trap_S_StartSound(NULL, es->number, CHAN_VOICE, CG_CustomSound(es->number, "*taunt.wav")); break;
		case EV_HORN: trap_S_StartSound(NULL, es->number, CHAN_AUTO, CG_CustomSound(es->number, "sound/vehicle/horn.ogg")); break;
		case EV_CRASH25: trap_S_StartSound(NULL, es->number, CHAN_AUTO, CG_CustomSound(es->number, "sound/vehicle/damage25.ogg")); break;
		case EV_OT1_IMPACT:
			rnum = rand() % 3 + 1;
			if(rnum == 1) trap_S_StartSound(NULL, es->number, CHAN_AUTO, CG_CustomSound(es->number, "sound/objects/basic/impact1"));
			if(rnum == 2) trap_S_StartSound(NULL, es->number, CHAN_AUTO, CG_CustomSound(es->number, "sound/objects/basic/impact2"));
			if(rnum == 3) trap_S_StartSound(NULL, es->number, CHAN_AUTO, CG_CustomSound(es->number, "sound/objects/basic/impact3"));
			break;
		case EV_GRAVITYSOUND: trap_S_StartSound(NULL, es->number, CHAN_AUTO, CG_CustomSound(es->number, "sound/gravitygun")); break;
		case EV_WATER_TOUCH: trap_S_StartSound(NULL, es->number, CHAN_AUTO, cgs.media.watrInSound); break;
		case EV_WATER_LEAVE: trap_S_StartSound(NULL, es->number, CHAN_AUTO, cgs.media.watrOutSound); break;
		case EV_WATER_UNDER: trap_S_StartSound(NULL, es->number, CHAN_AUTO, cgs.media.watrUnSound); break;
		case EV_WATER_CLEAR: trap_S_StartSound(NULL, es->number, CHAN_AUTO, CG_CustomSound(es->number, "*gasp.wav")); break;
		case EV_ITEM_PICKUP: {
			sfxHandle_t pickupSound = 0;
			item_t *item;
			int index;

			index = es->eventParm;  // player predicted

			if(index < 1 || index >= gameInfoItemsNum) {
				break;
			}
			item = &gameInfoItems[index];

			// powerups and team items will have a separate global sound, this one
			// will be played at prediction time
			switch(item->giType) {
				case IT_WEAPON: pickupSound = trap_S_RegisterSound("sound/misc/we_pkup.wav", qfalse); break;
				case IT_AMMO: pickupSound = trap_S_RegisterSound("sound/misc/am_pkup.wav", qfalse); break;
				case IT_ARMOR: pickupSound = trap_S_RegisterSound("sound/misc/ar_pkup.wav", qfalse); break;
				case IT_HEALTH: pickupSound = trap_S_RegisterSound("sound/misc/he_pkup.wav", qfalse); break;
				case IT_POWERUP: pickupSound = trap_S_RegisterSound("sound/misc/he_pkup.wav", qfalse); break;
				case IT_HOLDABLE: pickupSound = trap_S_RegisterSound("sound/misc/ho_pkup.wav", qfalse); break;
				case IT_RUNE: pickupSound = trap_S_RegisterSound("sound/misc/ru_pkup.wav", qfalse); break;
				case IT_TEAM: pickupSound = trap_S_RegisterSound("sound/misc/he_pkup.wav", qfalse); break;
			}

			if(pickupSound) {
				trap_S_StartSound(NULL, es->number, CHAN_AUTO, pickupSound);
			}

			// show icon and name on status bar
			if(es->number == cg.snap->ps.clientNum) {
				CG_ItemPickup(index);
			}
		} break;
		case EV_NOAMMO: trap_S_StartSound(NULL, es->number, CHAN_AUTO, cgs.media.noAmmoSound); break;
		case EV_CHANGE_WEAPON: trap_S_StartSound(NULL, es->number, CHAN_AUTO, cgs.media.selectSound); break;
		case EV_FIRE_WEAPON: CG_FireWeapon(cent); break;
		case EV_USE_ITEM0: CG_UseItem(cent); break;
		case EV_USE_ITEM1: CG_UseItem(cent); break;
		case EV_USE_ITEM2: CG_UseItem(cent); break;
		case EV_USE_ITEM3: CG_UseItem(cent); break;
		case EV_USE_ITEM4: CG_UseItem(cent); break;
		case EV_USE_ITEM5: CG_UseItem(cent); break;
		case EV_USE_ITEM6: CG_UseItem(cent); break;
		case EV_USE_ITEM7: CG_UseItem(cent); break;
		case EV_USE_ITEM8: CG_UseItem(cent); break;
		case EV_USE_ITEM9: CG_UseItem(cent); break;
		case EV_USE_ITEM10: CG_UseItem(cent); break;
		case EV_USE_ITEM11: CG_UseItem(cent); break;
		case EV_USE_ITEM12: CG_UseItem(cent); break;
		case EV_USE_ITEM13: CG_UseItem(cent); break;
		case EV_USE_ITEM14: CG_UseItem(cent); break;
		case EV_PLAYER_TELEPORT_IN:
			trap_S_StartSound(NULL, es->number, CHAN_AUTO, cgs.media.teleInSound);
			CG_SpawnEffect(position);
			break;
		case EV_PLAYER_TELEPORT_OUT:
			trap_S_StartSound(NULL, es->number, CHAN_AUTO, cgs.media.teleOutSound);
			CG_SpawnEffect(position);
			break;
		case EV_ITEM_POP: trap_S_StartSound(NULL, es->number, CHAN_AUTO, cgs.media.respawnSound); break;
		case EV_ITEM_RESPAWN:
			cent->miscTime = cg.time;  // scale up from this
			trap_S_StartSound(NULL, es->number, CHAN_AUTO, cgs.media.respawnSound);
			break;
		case EV_GRENADE_BOUNCE:
			if(rand() & 1) {
				trap_S_StartSound(NULL, es->number, CHAN_AUTO, cgs.media.hgrenb1aSound);
			} else {
				trap_S_StartSound(NULL, es->number, CHAN_AUTO, cgs.media.hgrenb2aSound);
			}
			break;
		case EV_PROXIMITY_MINE_STICK:
			if(es->eventParm & SURF_FLESH) {
				trap_S_StartSound(NULL, es->number, CHAN_AUTO, cgs.media.wstbimplSound);
			} else if(es->eventParm & SURF_METALSTEPS) {
				trap_S_StartSound(NULL, es->number, CHAN_AUTO, cgs.media.wstbimpmSound);
			} else {
				trap_S_StartSound(NULL, es->number, CHAN_AUTO, cgs.media.wstbimpdSound);
			}
			break;
		case EV_PROXIMITY_MINE_TRIGGER: trap_S_StartSound(NULL, es->number, CHAN_AUTO, cgs.media.wstbactvSound); break;
		case EV_KAMIKAZE: CG_KamikazeEffect(cent->lerpOrigin); break;
		case EV_OBELISKEXPLODE: CG_ObeliskExplode(cent->lerpOrigin, es->eventParm); break;
		case EV_OBELISKPAIN: CG_ObeliskPain(cent->lerpOrigin); break;
		case EV_INVUL_IMPACT: CG_InvulnerabilityImpact(cent->lerpOrigin, cent->currentState.angles); break;
		case EV_JUICED: CG_InvulnerabilityJuiced(cent->lerpOrigin); break;
		case EV_LIGHTNINGBOLT: CG_LightningBoltBeam(es->origin2, es->pos.trBase); break;
		case EV_MISSILE_HIT:
			ByteToDir(es->eventParm, dir);
			CG_MissileHitPlayer(es->weapon, position, dir, es->otherEntityNum);
			break;
		case EV_MISSILE_MISS:
			ByteToDir(es->eventParm, dir);
			CG_MissileHitWall(es->weapon, 0, position, dir, IMPACTSOUND_DEFAULT);
			break;
		case EV_MISSILE_MISS_METAL:
			ByteToDir(es->eventParm, dir);
			CG_MissileHitWall(es->weapon, 0, position, dir, IMPACTSOUND_METAL);
			break;
		case EV_RAILTRAIL:
			cent->currentState.weapon = WP_RAILGUN;
			if(es->clientNum == cg.predictedPlayerState.clientNum) {
				// do nothing, because it was already predicted
			} else {
				CG_RailTrail(ci, es->origin2, es->pos.trBase, WP_RAILGUN);
				if(es->eventParm != 255) {
					ByteToDir(es->eventParm, dir);
					CG_MissileHitWall(es->weapon, es->clientNum, position, dir, IMPACTSOUND_DEFAULT);
				}
			}
			break;
		case EV_BULLET_HIT_WALL:
			if(es->clientNum == cg.predictedPlayerState.clientNum) {
				// do nothing, because it was already predicted
			} else {
				ByteToDir(es->eventParm, dir);
				CG_Bullet(es->pos.trBase, es->otherEntityNum, dir, qfalse, ENTITYNUM_WORLD);
			}
			break;
		case EV_BULLET_HIT_FLESH:
			if(es->clientNum == cg.predictedPlayerState.clientNum) {
				// do nothing, because it was already predicted
			} else {
				CG_Bullet(es->pos.trBase, es->otherEntityNum, dir, qtrue, es->eventParm);
			}
			break;
		case EV_SHOTGUN:
			if(es->otherEntityNum == cg.predictedPlayerState.clientNum) {
				// do nothing, because it was already predicted
			} else {
				CG_ShotgunFire(es, es->weapon);
			}
			break;
		case EV_GENERAL_SOUND:
			if(cgs.gameSounds[es->eventParm]) {
				trap_S_StartSound(NULL, es->number, CHAN_VOICE, cgs.gameSounds[es->eventParm]);
			} else {
				s = CG_ConfigString(CS_SOUNDS + es->eventParm);
				trap_S_StartSound(NULL, es->number, CHAN_VOICE, CG_CustomSound(es->number, s));
			}
			break;
		case EV_GLOBAL_SOUND:  // play from the player's head so it never diminishes
			if(cgs.gameSounds[es->eventParm]) {
				trap_S_StartSound(NULL, cg.snap->ps.clientNum, CHAN_AUTO, cgs.gameSounds[es->eventParm]);
			} else {
				s = CG_ConfigString(CS_SOUNDS + es->eventParm);
				trap_S_StartSound(NULL, cg.snap->ps.clientNum, CHAN_AUTO, CG_CustomSound(es->number, s));
			}
			break;
		case EV_GLOBAL_TEAM_SOUND:  // play from the player's head so it never diminishes
		{
			switch(es->eventParm) {
				case GTS_RED_CAPTURE:  // CTF: red team captured the blue flag, 1FCTF: red team captured the neutral flag
					if(cgs.clientinfo[cg.clientNum].team == TEAM_RED)
						CG_AddBufferedSound(cgs.media.captureYourTeamSound);
					else
						CG_AddBufferedSound(cgs.media.captureOpponentSound);
					break;
				case GTS_BLUE_CAPTURE:  // CTF: blue team captured the red flag, 1FCTF: blue team captured the neutral flag
					if(cgs.clientinfo[cg.clientNum].team == TEAM_BLUE)
						CG_AddBufferedSound(cgs.media.captureYourTeamSound);
					else
						CG_AddBufferedSound(cgs.media.captureOpponentSound);
					break;
				case GTS_RED_RETURN:  // CTF: blue flag returned, 1FCTF: never used
					if(cgs.clientinfo[cg.clientNum].team == TEAM_RED)
						CG_AddBufferedSound(cgs.media.returnYourTeamSound);
					else
						CG_AddBufferedSound(cgs.media.returnOpponentSound);
					CG_AddBufferedSound(cgs.media.blueFlagReturnedSound);
					break;
				case GTS_BLUE_RETURN:  // CTF red flag returned, 1FCTF: neutral flag returned
					if(cgs.clientinfo[cg.clientNum].team == TEAM_BLUE)
						CG_AddBufferedSound(cgs.media.returnYourTeamSound);
					else
						CG_AddBufferedSound(cgs.media.returnOpponentSound);
					CG_AddBufferedSound(cgs.media.redFlagReturnedSound);
					break;

				case GTS_RED_TAKEN:  // CTF: red team took blue flag, 1FCTF: blue team took the neutral flag
					if(cg.snap->ps.powerups[PW_BLUEFLAG] || cg.snap->ps.powerups[PW_NEUTRALFLAG]) {
					} else {
						if(cgs.clientinfo[cg.clientNum].team == TEAM_BLUE) {
							if(cgs.gametype == GT_1FCTF)
								CG_AddBufferedSound(cgs.media.yourTeamTookTheFlagSound);
							else
								CG_AddBufferedSound(cgs.media.enemyTookYourFlagSound);
						} else if(cgs.clientinfo[cg.clientNum].team == TEAM_RED) {
							if(cgs.gametype == GT_1FCTF)
								CG_AddBufferedSound(cgs.media.enemyTookTheFlagSound);
							else
								CG_AddBufferedSound(cgs.media.yourTeamTookEnemyFlagSound);
						}
					}
					break;
				case GTS_BLUE_TAKEN:  // CTF: blue team took the red flag, 1FCTF red team took the neutral flag
					if(cg.snap->ps.powerups[PW_REDFLAG] || cg.snap->ps.powerups[PW_NEUTRALFLAG]) {
					} else {
						if(cgs.clientinfo[cg.clientNum].team == TEAM_RED) {
							if(cgs.gametype == GT_1FCTF)
								CG_AddBufferedSound(cgs.media.yourTeamTookTheFlagSound);
							else
								CG_AddBufferedSound(cgs.media.enemyTookYourFlagSound);
						} else if(cgs.clientinfo[cg.clientNum].team == TEAM_BLUE) {
							if(cgs.gametype == GT_1FCTF)
								CG_AddBufferedSound(cgs.media.enemyTookTheFlagSound);
							else
								CG_AddBufferedSound(cgs.media.yourTeamTookEnemyFlagSound);
						}
					}
					break;
				case GTS_REDOBELISK_ATTACKED:  // Overload: red obelisk is being attacked
					if(cgs.clientinfo[cg.clientNum].team == TEAM_RED) {
						CG_AddBufferedSound(cgs.media.yourBaseIsUnderAttackSound);
					}
					break;
				case GTS_BLUEOBELISK_ATTACKED:  // Overload: blue obelisk is being attacked
					if(cgs.clientinfo[cg.clientNum].team == TEAM_BLUE) {
						CG_AddBufferedSound(cgs.media.yourBaseIsUnderAttackSound);
					}
					break;

				case GTS_REDTEAM_SCORED: CG_AddBufferedSound(cgs.media.redScoredSound); break;
				case GTS_BLUETEAM_SCORED: CG_AddBufferedSound(cgs.media.blueScoredSound); break;
				case GTS_REDTEAM_TOOK_LEAD: CG_AddBufferedSound(cgs.media.redLeadsSound); break;
				case GTS_BLUETEAM_TOOK_LEAD: CG_AddBufferedSound(cgs.media.blueLeadsSound); break;
				case GTS_TEAMS_ARE_TIED: CG_AddBufferedSound(cgs.media.teamsTiedSound); break;
				case GTS_KAMIKAZE: trap_S_StartLocalSound(cgs.media.kamikazeFarSound, CHAN_ANNOUNCER); break;
				default: break;
			}
			break;
		}
		case EV_PAIN:
			// local player sounds are triggered in CG_CheckLocalSounds,
			// so ignore events on the player
			if(cent->currentState.number != cg.snap->ps.clientNum) {
				CG_PainEvent(cent, es->eventParm);
			}
			break;
		case EV_DEATH1:
		case EV_DEATH2:
		case EV_DEATH3:
			if(CG_WaterLevel(cent) >= 1) {
				trap_S_StartSound(NULL, es->number, CHAN_VOICE, CG_CustomSound(es->number, "*drown.wav"));
			} else {
				trap_S_StartSound(NULL, es->number, CHAN_VOICE, CG_CustomSound(es->number, va("*death%i.wav", event - EV_DEATH1 + 1)));
			}

			break;
		case EV_OBITUARY: CG_Obituary(es); break;
		case EV_GIB_PLAYER:
			// don't play gib sound when using the kamikaze because it interferes
			// with the kamikaze sound, downside is that the gib sound will also
			// not be played when someone is gibbed while just carrying the kamikaze
			if(!(es->eFlags & EF_KAMIKAZE)) {
				trap_S_StartSound(NULL, es->number, CHAN_BODY, cgs.media.gibSound);
			}
			CG_GibPlayer(cent->lerpOrigin);
			break;
		case EV_STOPLOOPINGSOUND:
			trap_S_StopLoopingSound(es->number);
			es->loopSound = 0;
			break;
		case EV_EXPLOSION:
			// show explosion
			dir[0] = 0;
			dir[1] = 0;
			dir[2] = 0;
			CG_MakeExplosion(cent->lerpOrigin, dir, cgs.media.dishFlashModel, cgs.media.rocketExplosionShader, 1000, qtrue);
			break;
		case EV_SMOKEPUFF:
			// es->constantLight is used to specify color of the smoke puff
			r = es->constantLight & 255;
			g = (es->constantLight >> 8) & 255;
			b = (es->constantLight >> 16) & 255;

			// es->generic1 is used to specify movement speed of the smokepuff
			VectorSet(dir, es->angles[0] * es->generic1, es->angles[1] * es->generic1, es->angles[2] * es->generic1);
			CG_SmokePuff(cent->lerpOrigin, dir, es->otherEntityNum, r / 255, g / 255, b / 255, 0.33f, es->eventParm * 1000, cg.time, 0, 0, cgs.media.smokePuffShader);
			break;
		case EV_WATERPUFF:
			// es->generic1 is used to specify movement speed of the smokepuff
			CG_SmokePuff(cent->lerpOrigin, dir, es->otherEntityNum, 1.0, 1.0, 1.0, 1.00f, es->eventParm * 2000, cg.time, 0, 0, cgs.media.lsplShader);
			CG_SmokePuff(cent->lerpOrigin, dir, es->otherEntityNum * 0.80, 1.0, 1.0, 1.0, 1.00f, es->eventParm * 1500, cg.time, 0, 0, cgs.media.lsplShader);
			CG_SmokePuff(cent->lerpOrigin, dir, es->otherEntityNum * 0.60, 1.0, 1.0, 1.0, 1.00f, es->eventParm * 1000, cg.time, 0, 0, cgs.media.lsplShader);
			CG_SmokePuff(cent->lerpOrigin, dir, es->otherEntityNum * 0.40, 1.0, 1.0, 1.0, 1.00f, es->eventParm * 750, cg.time, 0, 0, cgs.media.lsplShader);
			CG_SmokePuff(cent->lerpOrigin, dir, es->otherEntityNum * 0.20, 1.0, 1.0, 1.0, 1.00f, es->eventParm * 500, cg.time, 0, 0, cgs.media.lsplShader);
			CG_SmokePuff(cent->lerpOrigin, dir, es->otherEntityNum * 0.10, 1.0, 1.0, 1.0, 1.00f, es->eventParm * 250, cg.time, 0, 0, cgs.media.lsplShader);
			trap_S_StartSound(NULL, es->number, CHAN_AUTO, cgs.media.watrInSound);
			break;
		case EV_PARTICLES_GRAVITY: CG_ParticlesFromEntityState(cent->lerpOrigin, PT_GRAVITY, es); break;
		case EV_PARTICLES_LINEAR: CG_ParticlesFromEntityState(cent->lerpOrigin, PT_LINEAR_BOTH, es); break;
		case EV_PARTICLES_LINEAR_UP: CG_ParticlesFromEntityState(cent->lerpOrigin, PT_LINEAR_UP, es); break;
		case EV_PARTICLES_LINEAR_DOWN: CG_ParticlesFromEntityState(cent->lerpOrigin, PT_LINEAR_DOWN, es); break;
		default: CG_Error("Unknown event: %i", event); break;
	}
}

void CG_CheckEvents(centity_t *cent) {
	// check for event-only entities
	if(cent->currentState.eType > ET_EVENTS) {
		if(cent->previousEvent) return;  // already fired
		// if this is a player event set the entity number of the client entity number
		if(cent->currentState.eFlags & EF_PLAYER_EVENT) cent->currentState.number = cent->currentState.otherEntityNum;

		cent->previousEvent = 1;

		cent->currentState.event = cent->currentState.eType - ET_EVENTS;
	} else {
		// check for events riding with another entity
		if(cent->currentState.event == cent->previousEvent) return;
		cent->previousEvent = cent->currentState.event;
		if((cent->currentState.event & ~EV_EVENT_BITS) == 0) return;
	}

	// calculate the position at exactly the frame time
	BG_EvaluateTrajectory(&cent->currentState.pos, cg.snap->serverTime, cent->lerpOrigin);
	CG_SetEntitySoundPosition(cent);
	CG_EntityEvent(cent, cent->lerpOrigin);
}
