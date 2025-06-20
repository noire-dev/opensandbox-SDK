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
// cg_event.c -- handle entity events at snapshot or playerstate transitions

#include "cg_local.h"

/*
===================
CG_PlaceString

Also called by scoreboard drawing
===================
*/
const char	*CG_PlaceString( int rank ) {
	static char	str[64];
	char	*s, *t;

	if ( rank & RANK_TIED_FLAG ) {
		rank &= ~RANK_TIED_FLAG;
		t = "Tied for ";
	} else {
		t = "";
	}

	if ( rank == 1 ) {
		s = S_COLOR_GREEN "1st" S_COLOR_WHITE;		// draw in GREEN
	} else if ( rank == 2 ) {
		s = S_COLOR_RED "2nd" S_COLOR_WHITE;		// draw in red
	} else if ( rank == 3 ) {
		s = S_COLOR_YELLOW "3rd" S_COLOR_WHITE;		// draw in yellow
	} else if ( rank == 11 ) {
		s = "11th";
	} else if ( rank == 12 ) {
		s = "12th";
	} else if ( rank == 13 ) {
		s = "13th";
	} else if ( rank % 10 == 1 ) {
		s = va("%ist", rank);
	} else if ( rank % 10 == 2 ) {
		s = va("%ind", rank);
	} else if ( rank % 10 == 3 ) {
		s = va("%ird", rank);
	} else {
		s = va("%ith", rank);
	}

	Com_sprintf( str, sizeof( str ), "%s%s", t, s );
	return str;
}

sfxHandle_t CG_GetPickupSound(int giType) {
	switch (giType) {
		case IT_WEAPON:   return trap_S_RegisterSound("sound/misc/we_pkup.wav", qfalse);
		case IT_AMMO:     return trap_S_RegisterSound("sound/misc/am_pkup.wav", qfalse);
		case IT_ARMOR:    return trap_S_RegisterSound("sound/misc/ar_pkup.wav", qfalse);
		case IT_HEALTH:   return trap_S_RegisterSound("sound/misc/he_pkup.wav", qfalse);
		case IT_POWERUP:  return trap_S_RegisterSound("sound/misc/po_pkup.wav", qfalse);
		case IT_HOLDABLE: return trap_S_RegisterSound("sound/misc/ho_pkup.wav", qfalse);
		case IT_RUNE:     return trap_S_RegisterSound("sound/misc/ru_pkup.wav", qfalse);
		default:          return 0;
	}
}

/*
=============
CG_Obituary
=============
*/
static void CG_Obituary( entityState_t *ent ) {
	int			mod;
	int			target, attacker;
	char		*message;
	char		*message2;
	const char	*targetInfo;
	const char	*attackerInfo;
	char		targetName[32];
	char		attackerName[32];
	gender_t	gender;
	clientInfo_t	*ci;

	target = ent->otherEntityNum;
	attacker = ent->otherEntityNum2;
	mod = ent->eventParm;

	if ( target < 0 || target >= MAX_CLIENTS ) {
		CG_Error( "CG_Obituary: target out of range" );
	}
	ci = &cgs.clientinfo[target];

	if ( attacker < 0 || attacker >= MAX_CLIENTS ) {
		attacker = ENTITYNUM_WORLD;
		attackerInfo = NULL;
	} else {
		attackerInfo = CG_ConfigString( CS_PLAYERS + attacker );
	}

	targetInfo = CG_ConfigString( CS_PLAYERS + target );
	if ( !targetInfo ) {
		return;
	}
	Q_strncpyz( targetName, Info_ValueForKey( targetInfo, "n" ), sizeof(targetName) - 2);
	strcat( targetName, S_COLOR_WHITE );

	message2 = "";

	// check for single client messages
	gender = ci->gender;
    if(attacker != ENTITYNUM_WORLD)
        message = NULL;
    else
        switch( mod ) {
        case MOD_SUICIDE:
                return;
                break;
        case MOD_FALLING:
                message = "cratered";
                break;
        case MOD_CRUSH:
                message = "was squished";
                break;
        case MOD_WATER:
                message = "sank like a rock";
                break;
        case MOD_SLIME:
                message = "melted";
                break;
        case MOD_LAVA:
                message = "does a back flip into the lava";
                break;
        case MOD_TARGET_LASER:
                message = "saw the light";
                break;
        case MOD_TRIGGER_HURT:
                message = "was in the wrong place";
                break;
        default:
                message = NULL;
                break;
        }
	gender = ci->gender;

	if (attacker == target) {
		switch (mod) {
		case MOD_KAMIKAZE:
			message = "goes out with a bang";
			break;
		case MOD_CAR:
		if ( gender == GENDER_FEMALE )
			message = "exploded in her car";
		else
			message = "exploded in his car";
			break;
		case MOD_CAREXPLODE:
		if ( gender == GENDER_FEMALE )
			message = "exploded by her car";
		else
			message = "exploded by his car";
			break;
		case MOD_PROP:
		if ( gender == GENDER_FEMALE )
			message = "was crushed by an object she threw herself";
		else
			message = "was crushed by an object he threw himself";
			break;
		case MOD_GRENADE_SPLASH:
			if ( gender == GENDER_FEMALE )
				message = "tripped on her own grenade";
			else if ( gender == GENDER_NEUTER )
				message = "tripped on its own grenade";
			else
				message = "tripped on his own grenade";
			break;
		case MOD_ROCKET_SPLASH:
			if ( gender == GENDER_FEMALE )
				message = "blew herself up";
			else if ( gender == GENDER_NEUTER )
				message = "blew itself up";
			else
				message = "blew himself up";
			break;
		case MOD_PLASMA_SPLASH:
			if ( gender == GENDER_FEMALE )
				message = "melted herself";
			else if ( gender == GENDER_NEUTER )
				message = "melted itself";
			else
				message = "melted himself";
			break;
		case MOD_BFG_SPLASH:
			message = "should have used a smaller gun";
			break;
		case MOD_PROXIMITY_MINE:
			if( gender == GENDER_FEMALE ) {
				message = "found her prox mine";
			} else if ( gender == GENDER_NEUTER ) {
				message = "found its prox mine";
			} else {
				message = "found his prox mine";
			}
			break;
		case MOD_FLAME_SPLASH:
			if ( gender == GENDER_FEMALE )
				message = "burned herself up";
			else if ( gender == GENDER_NEUTER )
				message = "burned itself up";
			else
				message = "burned himself up";
			break;
		case MOD_ANTIMATTER_SPLASH:
			if ( gender == GENDER_FEMALE )
				message = "blew herself up";
			else if ( gender == GENDER_NEUTER )
				message = "blew itself up";
			else
				message = "blew himself up";
			break;
		default:
			if ( gender == GENDER_FEMALE )
				message = "killed herself";
			else if ( gender == GENDER_NEUTER )
				message = "killed itself";
			else
				message = "killed himself";
			break;
		}
	}

    //If a suicide happens while disconnecting then we might not have a targetName
	if(mod == MOD_SUICIDE){
		return;
	}
	if (message) {
		CG_Printf( "%s> %s%s%s %s.%s\n", 
				S_COLOR_YELLOW,
				S_COLOR_WHITE,
				targetName,
				S_COLOR_YELLOW,
			       	message,
				S_COLOR_WHITE
				);
		return;
	}

	// check for kill messages from the current clientNum
	if ( attacker == cg.snap->ps.clientNum ) {
		char	*s;

		if ( cgs.gametype < GT_TEAM ) {
			s = va("You killed %s\n%s place with %i", targetName, 
				CG_PlaceString( cg.snap->ps.persistant[PERS_RANK] + 1 ),
				cg.snap->ps.persistant[PERS_SCORE] );
		} else {
                    if(ent->generic1)
                        s = va("You killed your ^2TEAMMATE^7 %s", targetName );
                    else
			s = va("You killed %s", targetName );
		}
	if(cgs.gametype != GT_SANDBOX && cgs.gametype != GT_MAPEDITOR){
		CG_CenterPrint( s, SCREEN_HEIGHT * 0.30, (int)(BIGCHAR_WIDTH * 0.6) );
	}

	// print the text message as well
	}

	// check for double client messages
	if ( !attackerInfo ) {
		attacker = ENTITYNUM_WORLD;
		strcpy( attackerName, "noname" );
	} else {
		Q_strncpyz( attackerName, Info_ValueForKey( attackerInfo, "n" ), sizeof(attackerName) - 2);
		strcat( attackerName, S_COLOR_WHITE );
		// check for kill messages about the current clientNum
		if ( target == cg.snap->ps.clientNum ) {
			Q_strncpyz( cg.killerName, attackerName, sizeof( cg.killerName ) );
		}
	}

	gender = ci->gender;
	if ( attacker != ENTITYNUM_WORLD ) {

        if(ent->generic1) {
            message = "was killed by ^2TEAMMATE^7";
        }
        else
		switch (mod) {
		case MOD_GRAPPLE:
			message = "was caught by";
			break;
		case MOD_GAUNTLET:
			message = "was pummeled by";
			break;
		case MOD_MACHINEGUN:
			message = "was machinegunned by";
			break;
		case MOD_SHOTGUN:
			message = "was gunned down by";
			break;
		case MOD_GRENADE:
			message = "ate";
			message2 = "'s grenade";
			break;
		case MOD_GRENADE_SPLASH:
			message = "was shredded by";
			message2 = "'s shrapnel";
			break;
		case MOD_ROCKET:
			message = "ate";
			message2 = "'s rocket";
			break;
		case MOD_ROCKET_SPLASH:
			message = "almost dodged";
			message2 = "'s rocket";
			break;
		case MOD_PLASMA:
			message = "was melted by";
			message2 = "'s plasmagun";
			break;
		case MOD_PLASMA_SPLASH:
			message = "was melted by";
			message2 = "'s plasmagun";
			break;
		case MOD_RAILGUN:
			message = "was railed by";
			break;
		case MOD_LIGHTNING:
			message = "was electrocuted by";
			break;
		case MOD_BFG:
		case MOD_BFG_SPLASH:
			message = "was blasted by";
			message2 = "'s BFG";
			break;
		case MOD_NAIL:
			message = "was nailed by";
			break;
		case MOD_CHAINGUN:
			message = "got lead poisoning from";
			message2 = "'s Chaingun";
			break;
		case MOD_PROXIMITY_MINE:
			message = "was too close to";
			message2 = "'s Prox Mine";
			break;
		case MOD_KAMIKAZE:
			message = "falls to";
			message2 = "'s Kamikaze blast";
			break;
		case MOD_JUICED:
			message = "was juiced by";
			break;
		case MOD_CAR:
			message = "could not withstand a collision with";
			break;
		case MOD_CAREXPLODE:
			message = "exploded by";
			message2 = "'s car";
			break;
		case MOD_PROP:
			message = "was crushed by an object thrown by";
			break;
		case MOD_TELEFRAG:
			message = "tried to invade";
			message2 = "'s personal space";
			break;
                case MOD_LAVA:
                        message = "was given a hot bath by";
                        break;
                case MOD_SLIME:
                        message = "was given a acid bath by";
                        break;
                case MOD_FALLING:
                        message = "was given a small push by";
                        break;
                case MOD_TRIGGER_HURT:
                        message = "was helped on the way by";
                        break;
                case MOD_CRUSH:
                        message = "was crushed in";
                        message2 = "'s trap";
                        break;
		default:
			message = "was killed by";
			break;
		}
	}
		if (message) {
			CG_Printf( "%s> %s%s%s %s%s %s%s%s\n", 
				S_COLOR_YELLOW,
			       	S_COLOR_WHITE,
			       	targetName,
			       	S_COLOR_YELLOW,
			       	message,
			       	S_COLOR_WHITE,
				attackerName,
			       	S_COLOR_YELLOW,
			       	message2,
			       	S_COLOR_WHITE
				);
			return;
		}

	CG_Printf( "%s> %s%s%s died.%s\n",S_COLOR_YELLOW,S_COLOR_WHITE,targetName,S_COLOR_YELLOW,S_COLOR_WHITE);
}

/*
===============
CG_UseItem
===============
*/
static void CG_UseItem( centity_t *cent ) {
	clientInfo_t *ci;
	int			itemNum, clientNum;
	item_t		*item;
	entityState_t *es;

	es = &cent->currentState;
	
	itemNum = (es->event & ~EV_EVENT_BITS) - EV_USE_ITEM0;
	if ( itemNum < 0 || itemNum > HI_NUM_HOLDABLE ) {
		itemNum = 0;
	}

	// print a message if the local player
	if ( es->number == cg.snap->ps.clientNum ) {
		if ( !itemNum ) {
			CG_CenterPrint( "No item to use", SCREEN_HEIGHT * 0.30, BIGCHAR_WIDTH );
		} else {
			item = BG_FindItemForHoldable( itemNum );
			CG_CenterPrint( va("Use %s", item->pickup_name), SCREEN_HEIGHT * 0.30, BIGCHAR_WIDTH );
		}
	}

	switch ( itemNum ) {
	default:
	case HI_NONE:
		trap_S_StartSound (NULL, es->number, CHAN_BODY, cgs.media.useNothingSound );
		break;

	case HI_TELEPORTER:
		break;

	case HI_MEDKIT:
		clientNum = cent->currentState.clientNum;
		if ( clientNum >= 0 && clientNum < MAX_CLIENTS ) {
			ci = &cgs.clientinfo[ clientNum ];
			ci->medkitUsageTime = cg.time;
		}
		trap_S_StartSound (NULL, es->number, CHAN_BODY, cgs.media.medkitSound );
		break;


	case HI_KAMIKAZE:
		break;

	case HI_PORTAL:
		break;
	case HI_INVULNERABILITY:
		trap_S_StartSound (NULL, es->number, CHAN_BODY, cgs.media.useInvulnerabilitySound );
		break;

	}
}

/*
================
CG_ItemPickup

A new item was picked up this frame
================
*/
static void CG_ItemPickup( int itemNum ) {
	cg.itemPickup = itemNum;
	cg.itemPickupTime = cg.time;
	cg.itemPickupBlendTime = cg.time;
	// see if it should be the grabbed weapon
	// OpenSandbox weapon predict
	if ( gameInfoItems[itemNum].giType == IT_AMMO ) {
		if(cg.swep_listcl[gameInfoItems[itemNum].giTag] == 2){
			cg.swep_listcl[gameInfoItems[itemNum].giTag] = 1;
		}
	}
	if ( gameInfoItems[itemNum].giType == IT_WEAPON ) {
		cg.swep_listcl[gameInfoItems[itemNum].giTag] = 1;
	}

}

/*
================
CG_WaterLevel

Returns waterlevel for entity origin
================
*/
int CG_WaterLevel(centity_t *cent) {
	vec3_t point;
	int contents, sample1, sample2, anim, waterlevel;

	// get waterlevel, accounting for ducking
	waterlevel = 0;
	VectorCopy(cent->lerpOrigin, point);
	point[2] += MINS_Z + 1;
	anim = cent->currentState.legsAnim & ~ANIM_TOGGLEBIT;

	if (anim == LEGS_WALKCR || anim == LEGS_IDLECR) {
		point[2] += CROUCH_VIEWHEIGHT;
	} else {
		point[2] += DEFAULT_VIEWHEIGHT;
	}

	contents = CG_PointContents(point, -1);

	if (contents & MASK_WATER) {
		sample2 = point[2] - MINS_Z;
		sample1 = sample2 / 2;
		waterlevel = 1;
		point[2] = cent->lerpOrigin[2] + MINS_Z + sample1;
		contents = CG_PointContents(point, -1);

		if (contents & MASK_WATER) {
			waterlevel = 2;
			point[2] = cent->lerpOrigin[2] + MINS_Z + sample2;
			contents = CG_PointContents(point, -1);

			if (contents & MASK_WATER) {
				waterlevel = 3;
			}
		}
	}

	return waterlevel;
}

/*
================
CG_PainEvent

Also called by playerstate transition
================
*/
void CG_PainEvent( centity_t *cent, int health ) {
	char	*snd;

	// don't do more than two pain sounds a second
	if ( cg.time - cent->pe.painTime < 500 ) {
		return;
	}

	if ( health < 25 ) {
		snd = "*pain25_1.wav";
	} else if ( health < 50 ) {
		snd = "*pain50_1.wav";
	} else if ( health < 75 ) {
		snd = "*pain75_1.wav";
	} else {
		snd = "*pain100_1.wav";
	}
	// play a gurp sound instead of a normal pain sound
	if (CG_WaterLevel(cent) >= 1) {
		if (rand()&1) {
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

void CG_PainVehicleEvent( centity_t *cent, int health ) {
	trap_S_StartSound (NULL, cent->currentState.number, CHAN_VOICE, CG_CustomSound(cent->currentState.number, "sound/vehicle/damage50.ogg") );
}

/*
==============
CG_EntityEvent

An entity has an event value
also called by CG_CheckPlayerstateEvents
==============
*/
#define	DEBUGNAME(x) if(cg_debugEvents.integer){CG_Printf(x"\n");}
void CG_EntityEvent( centity_t *cent, vec3_t position ) {
	entityState_t	*es;
	int				event;
	vec3_t			dir;
	const char		*s;
	int				clientNum;
	clientInfo_t	*ci;
	byte			r, g, b;
	int 			random_number;

	es = &cent->currentState;
	event = es->event & ~EV_EVENT_BITS;

	if ( cg_debugEvents.integer ) {
		CG_Printf( "ent:%3i  event:%3i ", es->number, event );
	}

	if ( !event ) {
		DEBUGNAME("ZEROEVENT");
		return;
	}

	clientNum = es->clientNum;
	if ( clientNum < 0 || clientNum >= MAX_CLIENTS ) {
		clientNum = 0;
	}
	ci = &cgs.clientinfo[ clientNum ];

	switch ( event ) {
	//
	// movement generated events
	//
	case EV_FOOTSTEP:
		DEBUGNAME("EV_FOOTSTEP");
		trap_S_StartSound (NULL, es->number, CHAN_BODY, cgs.media.footsteps[ ci->footsteps ][rand()&3] );
		break;
	case EV_FOOTSTEP_METAL:
		DEBUGNAME("EV_FOOTSTEP_METAL");
		trap_S_StartSound (NULL, es->number, CHAN_BODY, cgs.media.footsteps[ FOOTSTEP_METAL ][rand()&3] );
		break;
	case EV_FOOTSTEP_FLESH:
		DEBUGNAME("EV_FOOTSTEP_FLESH");
		trap_S_StartSound (NULL, es->number, CHAN_BODY, cgs.media.footsteps[ FOOTSTEP_FLESH ][rand()&3] );
		break;
	case EV_FOOTSPLASH:
		DEBUGNAME("EV_FOOTSPLASH");
		trap_S_StartSound (NULL, es->number, CHAN_BODY, cgs.media.footsteps[ FOOTSTEP_SPLASH ][rand()&3] );
		break;
	case EV_FOOTWADE:
		DEBUGNAME("EV_FOOTWADE");
		trap_S_StartSound (NULL, es->number, CHAN_BODY, cgs.media.footsteps[ FOOTSTEP_SPLASH ][rand()&3] );
		break;
	case EV_SWIM:
		DEBUGNAME("EV_SWIM");
		trap_S_StartSound (NULL, es->number, CHAN_BODY, cgs.media.footsteps[ FOOTSTEP_SPLASH ][rand()&3] );
		break;


	case EV_FALL_SHORT:
	if(!cg.predictedPlayerState.stats[STAT_VEHICLE]){
		DEBUGNAME("EV_FALL_SHORT");
		trap_S_StartSound (NULL, es->number, CHAN_AUTO, cgs.media.landSound );
		if ( clientNum == cg.predictedPlayerState.clientNum ) {
			// smooth landing z changes
			cg.landChange = -8;
			cg.landTime = cg.time;
		}
	}
		break;
	case EV_FALL_MEDIUM:
	if(!cg.predictedPlayerState.stats[STAT_VEHICLE]){
		DEBUGNAME("EV_FALL_MEDIUM");
		// use normal pain sound
		trap_S_StartSound( NULL, es->number, CHAN_VOICE, CG_CustomSound( es->number, "*pain100_1.wav" ) );
		if ( clientNum == cg.predictedPlayerState.clientNum ) {
			// smooth landing z changes
			cg.landChange = -16;
			cg.landTime = cg.time;
		}
	}
		break;
	case EV_FALL_FAR:
	if(!cg.predictedPlayerState.stats[STAT_VEHICLE]){
		DEBUGNAME("EV_FALL_FAR");
		trap_S_StartSound (NULL, es->number, CHAN_AUTO, CG_CustomSound( es->number, "*fall1.wav" ) );
		cent->pe.painTime = cg.time;	// don't play a pain sound right after this
		if ( clientNum == cg.predictedPlayerState.clientNum ) {
			// smooth landing z changes
			cg.landChange = -24;
			cg.landTime = cg.time;
		}
	}
		break;

	case EV_STEP_4:
	case EV_STEP_8:
	case EV_STEP_12:
	case EV_STEP_16:		// smooth out step up transitions
		DEBUGNAME("EV_STEP");
	{
		float	oldStep;
		int		delta;
		int		step;

		if ( clientNum != cg.predictedPlayerState.clientNum ) {
			break;
		}
		// if we are interpolating, we don't need to smooth steps
		if ( cg.demoPlayback || (cg.snap->ps.pm_flags & PMF_FOLLOW)) {
			break;
		}
		// check for stepping up before a previous step is completed
		delta = cg.time - cg.stepTime;
		if (delta < STEP_TIME) {
			oldStep = cg.stepChange * (STEP_TIME - delta) / STEP_TIME;
		} else {
			oldStep = 0;
		}

		// add this amount
		step = 4 * (event - EV_STEP_4 + 1 );
		cg.stepChange = oldStep + step;
		if ( cg.stepChange > MAX_STEP_CHANGE ) {
			cg.stepChange = MAX_STEP_CHANGE;
		}
		cg.stepTime = cg.time;
		break;
	}

	case EV_JUMP_PAD:
		DEBUGNAME("EV_JUMP_PAD");
		// boing sound at origin, jump sound on player
		trap_S_StartSound ( cent->lerpOrigin, -1, CHAN_VOICE, cgs.media.jumpPadSound );
		trap_S_StartSound (NULL, es->number, CHAN_VOICE, CG_CustomSound( es->number, "*jump1.wav" ) );
		break;

	case EV_JUMP:
		DEBUGNAME("EV_JUMP");
		trap_S_StartSound (NULL, es->number, CHAN_VOICE, CG_CustomSound( es->number, "*jump1.wav" ) );
		break;
	case EV_TAUNT:
		DEBUGNAME("EV_TAUNT");
		trap_S_StartSound (NULL, es->number, CHAN_VOICE, CG_CustomSound( es->number, "*taunt.wav" ) );
		break;
	case EV_HORN:
		DEBUGNAME("EV_HORN");
		trap_S_StartSound (NULL, es->number, CHAN_AUTO, CG_CustomSound(es->number, "sound/vehicle/horn.ogg") );
		break;
	case EV_CRASH25:
		DEBUGNAME("EV_CRASH25");
		trap_S_StartSound (NULL, es->number, CHAN_AUTO, CG_CustomSound(es->number, "sound/vehicle/damage25.ogg") );
		break;
	case EV_OT1_IMPACT:
		DEBUGNAME("EV_OT1_IMPACT");
		random_number = rand() % 3 + 1;
		if(random_number == 1){
		trap_S_StartSound (NULL, es->number, CHAN_AUTO, CG_CustomSound(es->number, "sound/objects/basic/impact1") );}
		if(random_number == 2){
		trap_S_StartSound (NULL, es->number, CHAN_AUTO, CG_CustomSound(es->number, "sound/objects/basic/impact2") );}
		if(random_number == 3){
		trap_S_StartSound (NULL, es->number, CHAN_AUTO, CG_CustomSound(es->number, "sound/objects/basic/impact3") );}
		break;
	case EV_GRAVITYSOUND:
		DEBUGNAME("EV_GRAVITYSOUND");
		trap_S_StartSound (NULL, es->number, CHAN_AUTO, CG_CustomSound(es->number, "sound/gravitygun") );
		break;
	case EV_WATER_TOUCH:
		DEBUGNAME("EV_WATER_TOUCH");
		trap_S_StartSound (NULL, es->number, CHAN_AUTO, cgs.media.watrInSound );
		break;
	case EV_WATER_LEAVE:
		DEBUGNAME("EV_WATER_LEAVE");
		trap_S_StartSound (NULL, es->number, CHAN_AUTO, cgs.media.watrOutSound );
		break;
	case EV_WATER_UNDER:
		DEBUGNAME("EV_WATER_UNDER");
		trap_S_StartSound (NULL, es->number, CHAN_AUTO, cgs.media.watrUnSound );
		break;
	case EV_WATER_CLEAR:
		DEBUGNAME("EV_WATER_CLEAR");
		trap_S_StartSound (NULL, es->number, CHAN_AUTO, CG_CustomSound( es->number, "*gasp.wav" ) );
		break;

	case EV_ITEM_PICKUP:
		DEBUGNAME("EV_ITEM_PICKUP");
		{
			sfxHandle_t pickupSound = 0;
			item_t	*item;
			int		index;

			index = es->eventParm;		// player predicted

			if ( index < 1 || index >= gameInfoItemsNum ) {
				break;
			}
			item = &gameInfoItems[ index ];

			// powerups and team items will have a separate global sound, this one
			// will be played at prediction time
			switch (item->giType) {
				case IT_WEAPON:   pickupSound = trap_S_RegisterSound("sound/misc/we_pkup.wav", qfalse); break;
				case IT_AMMO:     pickupSound = trap_S_RegisterSound("sound/misc/am_pkup.wav", qfalse); break;
				case IT_ARMOR:    pickupSound = trap_S_RegisterSound("sound/misc/ar_pkup.wav", qfalse); break;
				case IT_HEALTH:   pickupSound = trap_S_RegisterSound("sound/misc/he_pkup.wav", qfalse); break;
				case IT_POWERUP:  pickupSound = trap_S_RegisterSound("sound/misc/he_pkup.wav", qfalse); break;
				case IT_HOLDABLE: pickupSound = trap_S_RegisterSound("sound/misc/ho_pkup.wav", qfalse); break;
				case IT_RUNE:     pickupSound = trap_S_RegisterSound("sound/misc/ru_pkup.wav", qfalse); break;
				case IT_TEAM:     pickupSound = trap_S_RegisterSound("sound/misc/he_pkup.wav", qfalse); break;
			}

			if (pickupSound) {
				trap_S_StartSound(NULL, es->number, CHAN_AUTO, pickupSound);
			}

			// show icon and name on status bar
			if ( es->number == cg.snap->ps.clientNum ) {
				CG_ItemPickup( index );
			}
		}
		break;

	//
	// weapon events
	//
	case EV_NOAMMO:
		DEBUGNAME("EV_NOAMMO");
		trap_S_StartSound (NULL, es->number, CHAN_AUTO, cgs.media.noAmmoSound );
		break;
	case EV_CHANGE_WEAPON:
		DEBUGNAME("EV_CHANGE_WEAPON");
		trap_S_StartSound (NULL, es->number, CHAN_AUTO, cgs.media.selectSound );
		break;
	case EV_FIRE_WEAPON:
		DEBUGNAME("EV_FIRE_WEAPON");
		CG_FireWeapon( cent );
		break;

	case EV_USE_ITEM0:
		DEBUGNAME("EV_USE_ITEM0");
		CG_UseItem( cent );
		break;
	case EV_USE_ITEM1:
		DEBUGNAME("EV_USE_ITEM1");
		CG_UseItem( cent );
		break;
	case EV_USE_ITEM2:
		DEBUGNAME("EV_USE_ITEM2");
		CG_UseItem( cent );
		break;
	case EV_USE_ITEM3:
		DEBUGNAME("EV_USE_ITEM3");
		CG_UseItem( cent );
		break;
	case EV_USE_ITEM4:
		DEBUGNAME("EV_USE_ITEM4");
		CG_UseItem( cent );
		break;
	case EV_USE_ITEM5:
		DEBUGNAME("EV_USE_ITEM5");
		CG_UseItem( cent );
		break;
	case EV_USE_ITEM6:
		DEBUGNAME("EV_USE_ITEM6");
		CG_UseItem( cent );
		break;
	case EV_USE_ITEM7:
		DEBUGNAME("EV_USE_ITEM7");
		CG_UseItem( cent );
		break;
	case EV_USE_ITEM8:
		DEBUGNAME("EV_USE_ITEM8");
		CG_UseItem( cent );
		break;
	case EV_USE_ITEM9:
		DEBUGNAME("EV_USE_ITEM9");
		CG_UseItem( cent );
		break;
	case EV_USE_ITEM10:
		DEBUGNAME("EV_USE_ITEM10");
		CG_UseItem( cent );
		break;
	case EV_USE_ITEM11:
		DEBUGNAME("EV_USE_ITEM11");
		CG_UseItem( cent );
		break;
	case EV_USE_ITEM12:
		DEBUGNAME("EV_USE_ITEM12");
		CG_UseItem( cent );
		break;
	case EV_USE_ITEM13:
		DEBUGNAME("EV_USE_ITEM13");
		CG_UseItem( cent );
		break;
	case EV_USE_ITEM14:
		DEBUGNAME("EV_USE_ITEM14");
		CG_UseItem( cent );
		break;

	//
	// other events
	//
	case EV_PLAYER_TELEPORT_IN:
		DEBUGNAME("EV_PLAYER_TELEPORT_IN");
		trap_S_StartSound (NULL, es->number, CHAN_AUTO, cgs.media.teleInSound );
		CG_SpawnEffect( position);
		break;

	case EV_PLAYER_TELEPORT_OUT:
		DEBUGNAME("EV_PLAYER_TELEPORT_OUT");
		trap_S_StartSound (NULL, es->number, CHAN_AUTO, cgs.media.teleOutSound );
		CG_SpawnEffect(  position);
		break;

	case EV_ITEM_POP:
		DEBUGNAME("EV_ITEM_POP");
		trap_S_StartSound (NULL, es->number, CHAN_AUTO, cgs.media.respawnSound );
		break;
	case EV_ITEM_RESPAWN:
		DEBUGNAME("EV_ITEM_RESPAWN");
		cent->miscTime = cg.time;	// scale up from this
		trap_S_StartSound (NULL, es->number, CHAN_AUTO, cgs.media.respawnSound );
		break;

	case EV_GRENADE_BOUNCE:
		DEBUGNAME("EV_GRENADE_BOUNCE");
		if ( rand() & 1 ) {
			trap_S_StartSound (NULL, es->number, CHAN_AUTO, cgs.media.hgrenb1aSound );
		} else {
			trap_S_StartSound (NULL, es->number, CHAN_AUTO, cgs.media.hgrenb2aSound );
		}
		break;


	case EV_PROXIMITY_MINE_STICK:
		DEBUGNAME("EV_PROXIMITY_MINE_STICK");
		if( es->eventParm & SURF_FLESH ) {
			trap_S_StartSound (NULL, es->number, CHAN_AUTO, cgs.media.wstbimplSound );
		} else 	if( es->eventParm & SURF_METALSTEPS ) {
			trap_S_StartSound (NULL, es->number, CHAN_AUTO, cgs.media.wstbimpmSound );
		} else {
			trap_S_StartSound (NULL, es->number, CHAN_AUTO, cgs.media.wstbimpdSound );
		}
		break;

	case EV_PROXIMITY_MINE_TRIGGER:
		DEBUGNAME("EV_PROXIMITY_MINE_TRIGGER");
		trap_S_StartSound (NULL, es->number, CHAN_AUTO, cgs.media.wstbactvSound );
		break;
	case EV_KAMIKAZE:
		DEBUGNAME("EV_KAMIKAZE");
		CG_KamikazeEffect( cent->lerpOrigin );
		break;
	case EV_OBELISKEXPLODE:
		DEBUGNAME("EV_OBELISKEXPLODE");
		CG_ObeliskExplode( cent->lerpOrigin, es->eventParm );
		break;
	case EV_OBELISKPAIN:
		DEBUGNAME("EV_OBELISKPAIN");
		CG_ObeliskPain( cent->lerpOrigin );
		break;
	case EV_INVUL_IMPACT:
		DEBUGNAME("EV_INVUL_IMPACT");
		CG_InvulnerabilityImpact( cent->lerpOrigin, cent->currentState.angles );
		break;
        case EV_JUICED:
		DEBUGNAME("EV_JUICED");
		CG_InvulnerabilityJuiced( cent->lerpOrigin );
		break;
	case EV_LIGHTNINGBOLT:
		DEBUGNAME("EV_LIGHTNINGBOLT");
		CG_LightningBoltBeam(es->origin2, es->pos.trBase);
		break;

	//
	// missile impacts
	//
	case EV_MISSILE_HIT:
		DEBUGNAME("EV_MISSILE_HIT");
		ByteToDir( es->eventParm, dir );
		CG_MissileHitPlayer( es->weapon, position, dir, es->otherEntityNum );
		break;

	case EV_MISSILE_MISS:
		DEBUGNAME("EV_MISSILE_MISS");
		ByteToDir( es->eventParm, dir );
		CG_MissileHitWall( es->weapon, 0, position, dir, IMPACTSOUND_DEFAULT );
		break;

	case EV_MISSILE_MISS_METAL:
		DEBUGNAME("EV_MISSILE_MISS_METAL");
		ByteToDir( es->eventParm, dir );
		CG_MissileHitWall( es->weapon, 0, position, dir, IMPACTSOUND_METAL );
		break;

	case EV_RAILTRAIL:
		DEBUGNAME("EV_RAILTRAIL");
		cent->currentState.weapon = WP_RAILGUN;
		if ( es->clientNum == cg.predictedPlayerState.clientNum ) {
			// do nothing, because it was already predicted
		} else {
			CG_RailTrail( ci, es->origin2, es->pos.trBase );
			if ( es->eventParm != 255 ) {
				ByteToDir( es->eventParm, dir );
				CG_MissileHitWall( es->weapon, es->clientNum, position, dir, IMPACTSOUND_DEFAULT );
			}
		}
		break;

	case EV_BULLET_HIT_WALL:
		DEBUGNAME("EV_BULLET_HIT_WALL");
		if ( es->clientNum == cg.predictedPlayerState.clientNum ) {
			// do nothing, because it was already predicted
		} else {
			ByteToDir( es->eventParm, dir );
			CG_Bullet( es->pos.trBase, es->otherEntityNum, dir, qfalse, ENTITYNUM_WORLD );
		}
		break;

	case EV_BULLET_HIT_FLESH:
		DEBUGNAME("EV_BULLET_HIT_FLESH");
		if ( es->clientNum == cg.predictedPlayerState.clientNum ) {
			// do nothing, because it was already predicted
		} else {
			CG_Bullet( es->pos.trBase, es->otherEntityNum, dir, qtrue, es->eventParm );
		}
		break;

	case EV_SHOTGUN:
		DEBUGNAME("EV_SHOTGUN");
		if ( es->otherEntityNum == cg.predictedPlayerState.clientNum ) {
			// do nothing, because it was already predicted
		} else {
			CG_ShotgunFire( es );
		}
		break;

	case EV_GENERAL_SOUND:
		DEBUGNAME("EV_GENERAL_SOUND");
		if ( cgs.gameSounds[ es->eventParm ] ) {
			trap_S_StartSound (NULL, es->number, CHAN_VOICE, cgs.gameSounds[ es->eventParm ] );
		} else {
			s = CG_ConfigString( CS_SOUNDS + es->eventParm );
			trap_S_StartSound (NULL, es->number, CHAN_VOICE, CG_CustomSound( es->number, s ) );
		}
		break;

	case EV_GLOBAL_SOUND:	// play from the player's head so it never diminishes
		DEBUGNAME("EV_GLOBAL_SOUND");
		if ( cgs.gameSounds[ es->eventParm ] ) {
			trap_S_StartSound (NULL, cg.snap->ps.clientNum, CHAN_AUTO, cgs.gameSounds[ es->eventParm ] );
		} else {
			s = CG_ConfigString( CS_SOUNDS + es->eventParm );
			trap_S_StartSound (NULL, cg.snap->ps.clientNum, CHAN_AUTO, CG_CustomSound( es->number, s ) );
		}
		break;

	case EV_GLOBAL_TEAM_SOUND:	// play from the player's head so it never diminishes
		{
			DEBUGNAME("EV_GLOBAL_TEAM_SOUND");
			switch( es->eventParm ) {
				case GTS_RED_CAPTURE: // CTF: red team captured the blue flag, 1FCTF: red team captured the neutral flag
					if ( cgs.clientinfo[cg.clientNum].team == TEAM_RED )
						CG_AddBufferedSound( cgs.media.captureYourTeamSound );
					else
						CG_AddBufferedSound( cgs.media.captureOpponentSound );
					break;
				case GTS_BLUE_CAPTURE: // CTF: blue team captured the red flag, 1FCTF: blue team captured the neutral flag
					if ( cgs.clientinfo[cg.clientNum].team == TEAM_BLUE )
						CG_AddBufferedSound( cgs.media.captureYourTeamSound );
					else
						CG_AddBufferedSound( cgs.media.captureOpponentSound );
					break;
				case GTS_RED_RETURN: // CTF: blue flag returned, 1FCTF: never used
					if ( cgs.clientinfo[cg.clientNum].team == TEAM_RED )
						CG_AddBufferedSound( cgs.media.returnYourTeamSound );
					else
						CG_AddBufferedSound( cgs.media.returnOpponentSound );
					//
					CG_AddBufferedSound( cgs.media.blueFlagReturnedSound );
					break;
				case GTS_BLUE_RETURN: // CTF red flag returned, 1FCTF: neutral flag returned
					if ( cgs.clientinfo[cg.clientNum].team == TEAM_BLUE )
						CG_AddBufferedSound( cgs.media.returnYourTeamSound );
					else
						CG_AddBufferedSound( cgs.media.returnOpponentSound );
					//
					CG_AddBufferedSound( cgs.media.redFlagReturnedSound );
					break;

				case GTS_RED_TAKEN: // CTF: red team took blue flag, 1FCTF: blue team took the neutral flag
					// if this player picked up the flag then a sound is played in CG_CheckLocalSounds
					if (cg.snap->ps.powerups[PW_BLUEFLAG] || cg.snap->ps.powerups[PW_NEUTRALFLAG]) {
					}
					else {
					if (cgs.clientinfo[cg.clientNum].team == TEAM_BLUE) {

							if (cgs.gametype == GT_1FCTF) 
								CG_AddBufferedSound( cgs.media.yourTeamTookTheFlagSound );
							else

						 	CG_AddBufferedSound( cgs.media.enemyTookYourFlagSound );
						}
						else if (cgs.clientinfo[cg.clientNum].team == TEAM_RED) {

							if (cgs.gametype == GT_1FCTF)
								CG_AddBufferedSound( cgs.media.enemyTookTheFlagSound );
							else

 							CG_AddBufferedSound( cgs.media.yourTeamTookEnemyFlagSound );
						}
					}
					break;
				case GTS_BLUE_TAKEN: // CTF: blue team took the red flag, 1FCTF red team took the neutral flag
					// if this player picked up the flag then a sound is played in CG_CheckLocalSounds
					if (cg.snap->ps.powerups[PW_REDFLAG] || cg.snap->ps.powerups[PW_NEUTRALFLAG]) {
					}
					else {
						if (cgs.clientinfo[cg.clientNum].team == TEAM_RED) {

							if (cgs.gametype == GT_1FCTF)
								CG_AddBufferedSound( cgs.media.yourTeamTookTheFlagSound );
							else

							CG_AddBufferedSound( cgs.media.enemyTookYourFlagSound );
						}
						else if (cgs.clientinfo[cg.clientNum].team == TEAM_BLUE) {

							if (cgs.gametype == GT_1FCTF)
								CG_AddBufferedSound( cgs.media.enemyTookTheFlagSound );
							else

							CG_AddBufferedSound( cgs.media.yourTeamTookEnemyFlagSound );
						}
					}
					break;
				case GTS_REDOBELISK_ATTACKED: // Overload: red obelisk is being attacked
					if (cgs.clientinfo[cg.clientNum].team == TEAM_RED) {
						CG_AddBufferedSound( cgs.media.yourBaseIsUnderAttackSound );
					}
					break;
				case GTS_BLUEOBELISK_ATTACKED: // Overload: blue obelisk is being attacked
					if (cgs.clientinfo[cg.clientNum].team == TEAM_BLUE) {
						CG_AddBufferedSound( cgs.media.yourBaseIsUnderAttackSound );
					}
					break;

				case GTS_REDTEAM_SCORED:
					CG_AddBufferedSound(cgs.media.redScoredSound);
					break;
				case GTS_BLUETEAM_SCORED:
					CG_AddBufferedSound(cgs.media.blueScoredSound);
					break;
				case GTS_REDTEAM_TOOK_LEAD:
					CG_AddBufferedSound(cgs.media.redLeadsSound);
					break;
				case GTS_BLUETEAM_TOOK_LEAD:
					CG_AddBufferedSound(cgs.media.blueLeadsSound);
					break;
				case GTS_TEAMS_ARE_TIED:
					CG_AddBufferedSound( cgs.media.teamsTiedSound );
					break;
				case GTS_KAMIKAZE:
					trap_S_StartLocalSound(cgs.media.kamikazeFarSound, CHAN_ANNOUNCER);
					break;
				default:
					break;
			}
			break;
		}

	case EV_PAIN:
		// local player sounds are triggered in CG_CheckLocalSounds,
		// so ignore events on the player
		DEBUGNAME("EV_PAIN");
		if ( cent->currentState.number != cg.snap->ps.clientNum ) {
			CG_PainEvent( cent, es->eventParm );
		}
		break;
		
	case EV_PAINVEHICLE:
		// local player sounds are triggered in CG_CheckLocalSounds,
		// so ignore events on the player
		DEBUGNAME("EV_PAINVEHICLE");
		if ( cent->currentState.number != cg.snap->ps.clientNum ) {
			CG_PainVehicleEvent( cent, es->eventParm );
		}
		break;

	case EV_DEATH1:
	case EV_DEATH2:
	case EV_DEATH3:
		DEBUGNAME("EV_DEATHx");

		if (CG_WaterLevel(cent) >= 1) {
			trap_S_StartSound(NULL, es->number, CHAN_VOICE, CG_CustomSound(es->number, "*drown.wav"));
		} else {
			trap_S_StartSound(NULL, es->number, CHAN_VOICE, CG_CustomSound(es->number, va("*death%i.wav", event - EV_DEATH1 + 1)));
		}

		break;


	case EV_OBITUARY:
		DEBUGNAME("EV_OBITUARY");
		CG_Obituary( es );
		break;

	case EV_GIB_PLAYER:
		DEBUGNAME("EV_GIB_PLAYER");
		// don't play gib sound when using the kamikaze because it interferes
		// with the kamikaze sound, downside is that the gib sound will also
		// not be played when someone is gibbed while just carrying the kamikaze
		if ( !(es->eFlags & EF_KAMIKAZE) ) {
			trap_S_StartSound( NULL, es->number, CHAN_BODY, cgs.media.gibSound );
		}
		CG_GibPlayer( cent->lerpOrigin );
		break;

	case EV_STOPLOOPINGSOUND:
		DEBUGNAME("EV_STOPLOOPINGSOUND");
		trap_S_StopLoopingSound( es->number );
		es->loopSound = 0;
		break;

	case EV_DEBUG_LINE:
		DEBUGNAME("EV_DEBUG_LINE");
		CG_Beam( cent );
		break;

	case EV_EXPLOSION:
		DEBUGNAME("EV_EXPLOSION");
		// show explosion
		dir[0] = 0;
		dir[1] = 0;
		dir[2] = 0;
		CG_MakeExplosion( cent->lerpOrigin, dir, cgs.media.dishFlashModel, cgs.media.rocketExplosionShader, 1000, qtrue );
		break;
	
	case EV_SMOKEPUFF:
		DEBUGNAME("EV_SMOKEPUFF");

		//es->constantLight is used to specify color of the smoke puff
		r = es->constantLight & 255;
		g = (es->constantLight >> 8) & 255;
		b = (es->constantLight >> 16) & 255;

		//es->generic1 is used to specify movement speed of the smokepuff
		VectorSet(dir, es->angles[0] * es->generic1, es->angles[1] * es->generic1, es->angles[2] * es->generic1);
		CG_SmokePuff(cent->lerpOrigin, dir, es->otherEntityNum, r / 255, g / 255, b / 255, 0.33f, es->eventParm * 1000, cg.time, 0, 0, cgs.media.smokePuffShader);
		break;
		
	case EV_WATERPUFF:
		DEBUGNAME("EV_WATERPUFF");
		
		//es->generic1 is used to specify movement speed of the smokepuff
		CG_SmokePuff(cent->lerpOrigin, dir, es->otherEntityNum, 1.0, 1.0, 1.0, 1.00f, es->eventParm * 2000, cg.time, 0, 0, cgs.media.lsplShader);
		CG_SmokePuff(cent->lerpOrigin, dir, es->otherEntityNum*0.80, 1.0, 1.0, 1.0, 1.00f, es->eventParm * 1500, cg.time, 0, 0, cgs.media.lsplShader);
		CG_SmokePuff(cent->lerpOrigin, dir, es->otherEntityNum*0.60, 1.0, 1.0, 1.0, 1.00f, es->eventParm * 1000, cg.time, 0, 0, cgs.media.lsplShader);
		CG_SmokePuff(cent->lerpOrigin, dir, es->otherEntityNum*0.40, 1.0, 1.0, 1.0, 1.00f, es->eventParm * 750, cg.time, 0, 0, cgs.media.lsplShader);
		CG_SmokePuff(cent->lerpOrigin, dir, es->otherEntityNum*0.20, 1.0, 1.0, 1.0, 1.00f, es->eventParm * 500, cg.time, 0, 0, cgs.media.lsplShader);
		CG_SmokePuff(cent->lerpOrigin, dir, es->otherEntityNum*0.10, 1.0, 1.0, 1.0, 1.00f, es->eventParm * 250, cg.time, 0, 0, cgs.media.lsplShader);
		trap_S_StartSound (NULL, es->number, CHAN_AUTO, cgs.media.watrInSound );
		break;

	case EV_PARTICLES_GRAVITY:
		DEBUGNAME("EV_PARTICLES_GRAVITY");
		CG_ParticlesFromEntityState( cent->lerpOrigin, PT_GRAVITY, es );
		break;

	case EV_PARTICLES_LINEAR:
		DEBUGNAME("EV_PARTICLES_LINEAR");
		CG_ParticlesFromEntityState( cent->lerpOrigin, PT_LINEAR_BOTH, es );
		break;

	case EV_PARTICLES_LINEAR_UP:
		DEBUGNAME("EV_PARTICLES_LINEAR_UP");
		CG_ParticlesFromEntityState( cent->lerpOrigin, PT_LINEAR_UP, es );
		break;

	case EV_PARTICLES_LINEAR_DOWN:
		DEBUGNAME("EV_PARTICLES_LINEAR_DOWN");
		CG_ParticlesFromEntityState( cent->lerpOrigin, PT_LINEAR_DOWN, es );
		break;

	default:
		DEBUGNAME("UNKNOWN");
		CG_Error( "Unknown event: %i", event );
		break;
	}

}


/*
==============
CG_CheckEvents

==============
*/
void CG_CheckEvents( centity_t *cent ) {
	// check for event-only entities
	if ( cent->currentState.eType > ET_EVENTS ) {
		if ( cent->previousEvent ) {
			return;	// already fired
		}
		// if this is a player event set the entity number of the client entity number
		if ( cent->currentState.eFlags & EF_PLAYER_EVENT ) {
			cent->currentState.number = cent->currentState.otherEntityNum;
		}

		cent->previousEvent = 1;

		cent->currentState.event = cent->currentState.eType - ET_EVENTS;
	} else {
		// check for events riding with another entity
		if ( cent->currentState.event == cent->previousEvent ) {
			return;
		}
		cent->previousEvent = cent->currentState.event;
		if ( ( cent->currentState.event & ~EV_EVENT_BITS ) == 0 ) {
			return;
		}
	}

	// calculate the position at exactly the frame time
	BG_EvaluateTrajectory( &cent->currentState.pos, cg.snap->serverTime, cent->lerpOrigin );
	CG_SetEntitySoundPosition( cent );

	CG_EntityEvent( cent, cent->lerpOrigin );
}

