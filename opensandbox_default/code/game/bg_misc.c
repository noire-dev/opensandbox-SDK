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
// bg_misc.c -- both games misc functions, all completely stateless

#include "../qcommon/q_shared.h"
#include "bg_public.h"
#include "g_local.h"

gitem_t	bg_itemlist[] = {
	{ NULL, 						NULL, 											NULL, 						NULL, 					0, 		0, 				0, },
	{ "item_armor_shard", 			"models/powerups/armor/shard.md3", 				"icons/iconr_shard", 		"Armor Shard", 			5, 		IT_ARMOR, 		0 },
	{ "item_armor_combat", 			"models/powerups/armor/armor_yel.md3", 			"icons/iconr_yellow", 		"Armor 50", 			50, 	IT_ARMOR, 		0 },
	{ "item_armor_body", 			"models/powerups/armor/armor_red.md3",			"icons/iconr_red",			"Heavy Armor",			100,	IT_ARMOR,		0 },
	{ "item_health_small",			"models/powerups/health/small_cross.md3", 		"icons/iconh_green",		"Health 5",				5,		IT_HEALTH,		0 },
	{ "item_health",				"models/powerups/health/medium_cross.md3",		"icons/iconh_yellow",		"Health 25",			25,		IT_HEALTH,		0 },
	{ "item_health_large",			"models/powerups/health/large_cross.md3",		"icons/iconh_red",			"Health 50",			50,		IT_HEALTH,		0 },
	{ "item_health_mega",			"models/powerups/health/mega_cross.md3",		"icons/iconh_mega",			"Mega Health",			100,	IT_HEALTH,		0 },
	{ "weapon_gauntlet",			"models/weapons2/gauntlet/gauntlet.md3",		"icons/iconw_gauntlet",		"Melee",				0,		IT_WEAPON,		WP_GAUNTLET },
	{ "weapon_shotgun",				"models/weapons2/shotgun/shotgun.md3",			"icons/iconw_shotgun",		"Shotgun",				10,		IT_WEAPON,		WP_SHOTGUN },
	{ "weapon_machinegun",			"models/weapons2/machinegun/machinegun.md3",	"icons/iconw_machinegun",	"Machinegun",			40,		IT_WEAPON,		WP_MACHINEGUN },
	{ "weapon_grenadelauncher",		"models/weapons2/grenadel/grenadel.md3",		"icons/iconw_grenade",		"Grenade Launcher",		10,		IT_WEAPON,		WP_GRENADE_LAUNCHER },
	{ "weapon_rocketlauncher",		"models/weapons2/rocketl/rocketl.md3",			"icons/iconw_rocket",		"Rocket Launcher",		10,		IT_WEAPON,		WP_ROCKET_LAUNCHER },
	{ "weapon_lightning",			"models/weapons2/lightning/lightning.md3",		"icons/iconw_lightning",	"Lightning Gun",		100,	IT_WEAPON,		WP_LIGHTNING },
	{ "weapon_railgun",				"models/weapons2/railgun/railgun.md3",			"icons/iconw_railgun",		"Railgun",				10,		IT_WEAPON,		WP_RAILGUN },
	{ "weapon_plasmagun",			"models/weapons2/plasma/plasma.md3",			"icons/iconw_plasma",		"Plasma Gun",			50,		IT_WEAPON,		WP_PLASMAGUN },
	{ "weapon_bfg",					"models/weapons2/bfg/bfg.md3",					"icons/iconw_bfg",			"BFG10K",				20,		IT_WEAPON,		WP_BFG },
	{ "weapon_grapplinghook",		"models/weapons2/grapple/grapple.md3",			"icons/iconw_grapple",		"Grappling Hook",		0,		IT_WEAPON,		WP_GRAPPLING_HOOK },
	{ "weapon_nailgun",				"models/weapons/nailgun/nailgun.md3",			"icons/iconw_nailgun",		"Nailgun",				10,		IT_WEAPON,		WP_NAILGUN },
	{ "weapon_prox_launcher",		"models/weapons/proxmine/proxmine.md3",			"icons/iconw_proxlauncher",	"Prox Launcher",		5,		IT_WEAPON,		WP_PROX_LAUNCHER },
	{ "weapon_chaingun",			"models/weapons/vulcan/vulcan.md3",				"icons/iconw_chaingun",		"Chaingun",				80,		IT_WEAPON,		WP_CHAINGUN },
	{ "weapon_flamethrower",		"models/weapons2/flamethrower/rocketl.md3",		"icons/iconw_flamethrower",	"Flamethrower",			100,	IT_WEAPON,		WP_FLAMETHROWER },
	{ "weapon_antimatter",			"models/weapons2/antimatter/plasma.md3",		"icons/iconw_voodoo",		"Dark Flare",			100,	IT_WEAPON,		WP_ANTIMATTER },
	{ "weapon_toolgun", 			"models/weapons/toolgun/toolgun.md3",			"icons/iconw_toolgun",		"Toolgun",				0,		IT_WEAPON,		WP_TOOLGUN },
	{ "weapon_physgun", 			"models/weapons/physgun/physgun.md3",			"icons/iconw_physgun",		"Physgun",				0,		IT_WEAPON,		WP_PHYSGUN },
	{ "weapon_gravitygun", 			"models/weapons/physgun/physgun.md3",			"icons/iconw_gravitygun",	"Gravitygun",			0,		IT_WEAPON,		WP_GRAVITYGUN },
	{ "weapon_thrower",				"models/weapons3/machinegun/machinegun.md3",	"icons/iconw_thrower",		"Thrower",				100,	IT_WEAPON,		WP_THROWER },
	{ "weapon_bouncer",				"models/weapons3/shotgun/shotgun.md3",			"icons/iconw_bouncer",		"Bouncer",				15,		IT_WEAPON,		WP_BOUNCER },
	{ "weapon_thunder",				"models/weapons3/grenadel/grenadel.md3",		"icons/iconw_thunder",		"Thunder",				10,		IT_WEAPON,		WP_THUNDER },
	{ "weapon_exploder",			"models/weapons3/rocketl/rocketl.md3",			"icons/iconw_exploder",		"Exploder",				10,		IT_WEAPON,		WP_EXPLODER },
	{ "weapon_knocker",				"models/weapons3/lightning/lightning.md3",		"icons/iconw_knocker",		"Knocker",				25,		IT_WEAPON,		WP_KNOCKER },
	{ "weapon_propgun",				"models/weapons3/railgun/railgun.md3",			"icons/iconw_propgun",		"Propgun",				15,		IT_WEAPON,		WP_PROPGUN },
	{ "weapon_regenerator",			"models/weapons3/plasma/plasma.md3",			"icons/iconw_regenerator",	"Regenerator",			15,		IT_WEAPON,		WP_REGENERATOR },
	{ "weapon_nuke",				"models/weapons3/bfg/bfg.md3",					"icons/iconw_nuke",			"Nuke",					2,		IT_WEAPON,		WP_NUKE },
	{ "ammo_bullets",				"models/powerups/ammo/machinegunam.md3",		"icons/icona_machinegun",	"Bullets",				50,		IT_AMMO,		WP_MACHINEGUN },
	{ "ammo_shells",				"models/powerups/ammo/shotgunam.md3",			"icons/icona_shotgun",		"Shells",				10,		IT_AMMO,		WP_SHOTGUN },
	{ "ammo_grenades",				"models/powerups/ammo/grenadeam.md3",			"icons/icona_grenade",		"Grenades",				5,		IT_AMMO,		WP_GRENADE_LAUNCHER },
	{ "ammo_cells",					"models/powerups/ammo/plasmaam.md3",			"icons/icona_plasma",		"Cells",				30,		IT_AMMO,		WP_PLASMAGUN },
	{ "ammo_lightning",				"models/powerups/ammo/lightningam.md3",			"icons/icona_lightning",	"Lightning",			60,		IT_AMMO,		WP_LIGHTNING },
	{ "ammo_rockets",				"models/powerups/ammo/rocketam.md3",			"icons/icona_rocket",		"Rockets",				5,		IT_AMMO,		WP_ROCKET_LAUNCHER },
	{ "ammo_slugs",					"models/powerups/ammo/railgunam.md3",			"icons/icona_railgun",		"Slugs",				10,		IT_AMMO,		WP_RAILGUN },
	{ "ammo_bfg",					"models/powerups/ammo/bfgam.md3",				"icons/icona_bfg",			"Bfg Ammo",				15,		IT_AMMO,		WP_BFG },
	{ "ammo_nails",					"models/powerups/ammo/nailgunam.md3",			"icons/icona_nailgun",		"Nails",				20,		IT_AMMO,		WP_NAILGUN },
	{ "ammo_mines",					"models/powerups/ammo/proxmineam.md3",			"icons/icona_proxlauncher",	"Proximity Mines",		10,		IT_AMMO,		WP_PROX_LAUNCHER },
	{ "ammo_belt",					"models/powerups/ammo/chaingunam.md3",			"icons/icona_chaingun",		"Chaingun Belt",		100,	IT_AMMO,		WP_CHAINGUN },
	{ "ammo_flame",					"models/powerups/ammo/flamethroweram.md3",		"icons/icona_flamethrower",	"Flame",				50,		IT_AMMO,		WP_FLAMETHROWER },
	{ "holdable_teleporter",		"models/powerups/holdable/teleporter.md3",		"icons/teleporter",			"Personal Teleporter",	0,		IT_HOLDABLE,	HI_TELEPORTER },
	{ "holdable_medkit", 			"models/powerups/holdable/medkit.md3",			"icons/medkit",				"Medkit", 				0,		IT_HOLDABLE,	HI_MEDKIT },
	{ "holdable_kamikaze", 			"models/powerups/kamikazi.md3",					"icons/kamikaze",			"Kamikaze",				0,		IT_HOLDABLE,	HI_KAMIKAZE },
	{ "holdable_portal", 			"models/powerups/holdable/porter.md3",			"icons/portal",				"Portal",				0,		IT_HOLDABLE,	HI_PORTAL },
	{ "holdable_invulnerability", 	"models/powerups/holdable/invulnerability.md3",	"icons/invulnerability",	"Invulnerability",		0,		IT_HOLDABLE,	HI_INVULNERABILITY },
	{ "item_quad",					"models/powerups/instant/quad.md3",				"icons/quad",				"Quad Damage",			30,		IT_POWERUP,		PW_QUAD },
	{ "item_enviro",				"models/powerups/instant/enviro.md3",			"icons/envirosuit",			"Battle Suit",			30,		IT_POWERUP,		PW_BATTLESUIT },
	{ "item_haste",					"models/powerups/instant/haste.md3",			"icons/haste",				"Speed",				30,		IT_POWERUP,		PW_HASTE },
	{ "item_invis",					"models/powerups/instant/invis.md3",			"icons/invis",				"Invisibility",			30,		IT_POWERUP,		PW_INVIS },
	{ "item_regen",					"models/powerups/instant/regen.md3",			"icons/regen",				"Regeneration",			30,		IT_POWERUP,		PW_REGEN },
	{ "item_flight",				"models/powerups/instant/flight.md3",			"icons/flight",				"Flight",				60,		IT_POWERUP,		PW_FLIGHT },
	{ "item_scout",					"models/powerups/scout.md3",					"icons/scout",				"Scout",				0,		IT_RUNE,		PW_SCOUT },
	{ "item_guard",					"models/powerups/guard.md3",					"icons/guard",				"Guard",				0,		IT_RUNE,		PW_GUARD },
	{ "item_doubler",				"models/powerups/doubler.md3",					"icons/doubler",			"Doubler",				0,		IT_RUNE,		PW_DOUBLER },
	{ "item_ammoregen",				"models/powerups/ammo.md3",						"icons/ammo_regen",			"Ammo Regen",			0,		IT_RUNE,		PW_AMMOREGEN },
	{ "team_CTF_redflag",			"models/flags/r_flag.md3",						"icons/iconf_red1",			"Red Flag",				0,		IT_TEAM,		PW_REDFLAG },
	{ "team_CTF_blueflag",			"models/flags/b_flag.md3",						"icons/iconf_blu1",			"Blue Flag",			0,		IT_TEAM,		PW_BLUEFLAG },
	{ "team_CTF_neutralflag",		"models/flags/n_flag.md3",						"icons/iconf_neutral1",		"Neutral Flag",			0,		IT_TEAM,		PW_NEUTRALFLAG },
	{ "item_redcube",				"models/powerups/orb/r_orb.md3",				"icons/iconh_rorb",			"Red Cube",				0,		IT_TEAM,		0 },
	{ "item_bluecube",				"models/powerups/orb/b_orb.md3",				"icons/iconh_borb",			"Blue Cube",			0,		IT_TEAM,		0 },
	{NULL}
};

//look for this to add new ones - WEAPONS_HYPER

int		bg_numItems = sizeof(bg_itemlist) / sizeof(bg_itemlist[0]) - 1;


/*
==============
BG_FindItemForPowerup
==============
*/
gitem_t	*BG_FindItemForPowerup( powerup_t pw ) {
	int		i;

	for ( i = 0 ; i < bg_numItems ; i++ ) {
		if ( (bg_itemlist[i].giType == IT_POWERUP || 
					bg_itemlist[i].giType == IT_TEAM ||
					bg_itemlist[i].giType == IT_RUNE) && 
			bg_itemlist[i].giTag == pw ) {
			return &bg_itemlist[i];
		}
	}

	return NULL;
}


/*
==============
BG_FindItemForHoldable
==============
*/
gitem_t	*BG_FindItemForHoldable( holdable_t pw ) {
	int		i;

	for ( i = 0 ; i < bg_numItems ; i++ ) {
		if ( bg_itemlist[i].giType == IT_HOLDABLE && bg_itemlist[i].giTag == pw ) {
			return &bg_itemlist[i];
		}
	}

	Com_Error( ERR_DROP, "HoldableItem not found" );

	return NULL;
}


/*
===============
BG_FindItemForWeapon

===============
*/
gitem_t	*BG_FindItemForWeapon( weapon_t weapon ) {
	gitem_t	*it;
	
	for ( it = bg_itemlist + 1 ; it->classname ; it++) {
		if ( it->giType == IT_WEAPON && it->giTag == weapon ) {
			return it;
		}
	}

	//Com_Error( ERR_DROP, "Couldn't find item for weapon %i", weapon);
	return NULL;
}

/*
===============
BG_FindItem

===============
*/
gitem_t	*BG_FindItem( const char *pickupName ) {
	gitem_t	*it;
	
	for ( it = bg_itemlist + 1 ; it->classname ; it++ ) {
		if ( !Q_stricmp( it->pickup_name, pickupName ) )
			return it;
	}

	return NULL;
}

/*
===============
BG_FindClassname
===============
*/
gitem_t		*BG_FindClassname( const char *classname ) {
	gitem_t	*it;
	
	for ( it = bg_itemlist + 1 ; it->classname ; it++ ) {
		if ( !Q_stricmp( it->classname, classname ) )
			return it;
	}

	return NULL;
}

/*
===============
BG_CheckClassname
===============
*/
qboolean	BG_CheckClassname( const char *classname ) {
	gitem_t	*it;
	
	for ( it = bg_itemlist + 1 ; it->classname ; it++ ) {
		if ( !Q_stricmp( it->classname, classname ) )
			return qtrue;
	}

	return qfalse;
}

/*
===============
BG_FindSwep

===============
*/
gitem_t	*BG_FindSwep( int id ) {
	gitem_t	*it;
	
	for ( it = bg_itemlist + 1 ; it->classname ; it++ ) {
		if ( it->giType == IT_WEAPON && it->giTag == id ) {
			return it;
		}
	}
		

	return NULL;
}

/*
===============
BG_FindSwepAmmo

===============
*/
gitem_t	*BG_FindSwepAmmo( int id ) {
	gitem_t	*it;
	
	for ( it = bg_itemlist + 1 ; it->classname ; it++ ) {
		if ( it->giType == IT_AMMO && it->giTag == id ) {
			return it;
		}
	}
		

	return NULL;
}

/*
============
BG_PlayerTouchesItem

Items can be picked up without actually touching their physical bounds to make
grabbing them easier
============
*/
qboolean	BG_PlayerTouchesItem( playerState_t *ps, entityState_t *item, int atTime ) {
	vec3_t		origin;

	BG_EvaluateTrajectory( &item->pos, atTime, origin );

	// we are ignoring ducked differences here
	if ( ps->origin[0] - origin[0] > 44
		|| ps->origin[0] - origin[0] < -50
		|| ps->origin[1] - origin[1] > 36
		|| ps->origin[1] - origin[1] < -36
		|| ps->origin[2] - origin[2] > 36
		|| ps->origin[2] - origin[2] < -36 ) {
		return qfalse;
	}

	return qtrue;
}

/*
================
BG_CanItemBeGrabbed

Returns false if the item should not be picked up.
This needs to be the same for client side prediction and server use.
================
*/
qboolean BG_CanItemBeGrabbed( int gametype, const entityState_t *ent, const playerState_t *ps ) {
	gitem_t	*item;
	int		upperBound;

	if ( ent->modelindex < 1 || ent->modelindex >= bg_numItems ) {
		Com_Error( ERR_DROP, "BG_CanItemBeGrabbed: index out of range" );
	}

	item = &bg_itemlist[ent->modelindex];

	switch( item->giType ) {
	case IT_WEAPON:
		return qtrue;	// weapons are always picked up

	case IT_AMMO:
		if ( ps->stats[STAT_SWEPAMMO] >= 9000 ) {
			return qfalse;		// can't hold any more
		}
		return qtrue;

	case IT_ARMOR:
		if( bg_itemlist[ps->stats[STAT_PERSISTANT_POWERUP]].giTag == PW_SCOUT ) {
			return qfalse;
		}

		// we also clamp armor to the maxhealth for handicapping
		if( bg_itemlist[ps->stats[STAT_PERSISTANT_POWERUP]].giTag == PW_GUARD ) {
			upperBound = ps->stats[STAT_MAX_HEALTH];
		}
		else {
			upperBound = ps->stats[STAT_MAX_HEALTH] * 2;
		}

		if ( ps->stats[STAT_ARMOR] >= upperBound ) {
			return qfalse;
		}
		return qtrue;

	case IT_HEALTH:
		if( bg_itemlist[ps->stats[STAT_PERSISTANT_POWERUP]].giTag == PW_GUARD ) {
			upperBound = ps->stats[STAT_MAX_HEALTH];
		} else if ( item->quantity == 5 || item->quantity == 100 ) {
			if ( ps->stats[STAT_HEALTH] >= ps->stats[STAT_MAX_HEALTH] * 2 ) {
				return qfalse;
			}
			return qtrue;
		}

		if ( ps->stats[STAT_HEALTH] >= ps->stats[STAT_MAX_HEALTH] ) {
			return qfalse;
		}
		return qtrue;

	case IT_POWERUP:
		return qtrue;	// powerups are always picked up

	case IT_RUNE:
		// can only hold one item at a time
		if ( ps->stats[STAT_PERSISTANT_POWERUP] ) {
			return qfalse;
		}

		// check team only
		if( ( ent->generic1 & 2 ) && ( ps->persistant[PERS_TEAM] != TEAM_RED ) ) {
			return qfalse;
		}
		if( ( ent->generic1 & 4 ) && ( ps->persistant[PERS_TEAM] != TEAM_BLUE ) ) {
			return qfalse;
		}

		return qtrue;

	case IT_TEAM: // team items, such as flags	
		if( gametype == GT_1FCTF ) {
			// neutral flag can always be picked up
			if( item->giTag == PW_NEUTRALFLAG ) {
				return qtrue;
			}
			if (ps->persistant[PERS_TEAM] == TEAM_RED) {
				if (item->giTag == PW_BLUEFLAG  && ps->powerups[PW_NEUTRALFLAG] ) {
					return qtrue;
				}
			} else if (ps->persistant[PERS_TEAM] == TEAM_BLUE) {
				if (item->giTag == PW_REDFLAG  && ps->powerups[PW_NEUTRALFLAG] ) {
					return qtrue;
				}
			}
		}
		if( gametype == GT_CTF ) {
			if (ps->persistant[PERS_TEAM] == TEAM_RED) {
				if (item->giTag == PW_BLUEFLAG ||
					(item->giTag == PW_REDFLAG && ent->modelindex2) ||
					(item->giTag == PW_REDFLAG && ps->powerups[PW_BLUEFLAG]) )
					return qtrue;
			} else if (ps->persistant[PERS_TEAM] == TEAM_BLUE) {
				if (item->giTag == PW_REDFLAG ||
					(item->giTag == PW_BLUEFLAG && ent->modelindex2) ||
					(item->giTag == PW_BLUEFLAG && ps->powerups[PW_REDFLAG]) )
					return qtrue;
			}
		}
		if( gametype == GT_HARVESTER ) {
			return qtrue;
		}
		return qfalse;

	case IT_HOLDABLE:
		if ( ps->stats[STAT_HOLDABLE_ITEM] ) {
			return qfalse;
		}
		return qtrue;

        case IT_BAD:
            Com_Error( ERR_DROP, "BG_CanItemBeGrabbed: IT_BAD" );
        default:
    		Com_Printf("BG_CanItemBeGrabbed: unknown enum %d\n", item->giType );
        	break;
	}

	return qfalse;
}

/*
================
BG_EvaluateTrajectory

================
*/
void BG_EvaluateTrajectory( const trajectory_t *tr, int atTime, vec3_t result ) {
	float		deltaTime;
	float		phase;

	switch( tr->trType ) {
	case TR_STATIONARY:
	case TR_INTERPOLATE:
		VectorCopy( tr->trBase, result );
		break;
	case TR_LINEAR:
		deltaTime = ( atTime - tr->trTime ) * 0.001;	// milliseconds to seconds
		VectorMA( tr->trBase, deltaTime, tr->trDelta, result );
		break;
	case TR_SINE:
		deltaTime = ( atTime - tr->trTime ) / (float) tr->trDuration;
		phase = sin( deltaTime * M_PI * 2 );
		VectorMA( tr->trBase, phase, tr->trDelta, result );
		break;
	case TR_LINEAR_STOP:
		if ( atTime > tr->trTime + tr->trDuration ) {
			atTime = tr->trTime + tr->trDuration;
		}
		deltaTime = ( atTime - tr->trTime ) * 0.001;	// milliseconds to seconds
		if ( deltaTime < 0 ) {
			deltaTime = 0;
		}
		VectorMA( tr->trBase, deltaTime, tr->trDelta, result );
		break;
	case TR_GRAVITY:
		deltaTime = ( atTime - tr->trTime ) * 0.001;	// milliseconds to seconds
		VectorMA( tr->trBase, deltaTime, tr->trDelta, result );
		result[2] -= 0.5 * mod_gravity * deltaTime * deltaTime;
		break;
	case TR_ROTATING:
		if ( tr->trTime > 0 )
			deltaTime = tr->trTime * 0.001;	// milliseconds to seconds
		else if ( tr->trTime < 0 )
			deltaTime = ( atTime + tr->trTime ) * 0.001;
		else
			deltaTime = ( atTime - tr->trTime ) * 0.001;
		VectorMA( tr->trBase, deltaTime, tr->trDelta, result );
		break;
	case TR_GRAVITY_WATER:
		deltaTime = ( atTime - tr->trTime ) * 0.001;	// milliseconds to seconds
		VectorMA( tr->trBase, deltaTime, tr->trDelta, result );
		result[2] -= 0.5 * (mod_gravity*0.50) * deltaTime * deltaTime;
		break;
	default:
		//Com_Error( ERR_DROP, "BG_EvaluateTrajectory: unknown trType: %i", tr->trTime );
		break;
	}
}

/*
================
BG_EvaluateTrajectoryDelta

For determining velocity at a given time
================
*/
void BG_EvaluateTrajectoryDelta( const trajectory_t *tr, int atTime, vec3_t result ) {
	float	deltaTime;
	float	phase;

	switch( tr->trType ) {
	case TR_STATIONARY:
	case TR_INTERPOLATE:
		VectorClear( result );
		break;
	case TR_ROTATING:
	case TR_LINEAR:
		VectorCopy( tr->trDelta, result );
		break;
	case TR_SINE:
		deltaTime = ( atTime - tr->trTime ) / (float) tr->trDuration;
		phase = cos( deltaTime * M_PI * 2 );	// derivative of sin = cos
		phase *= 0.5;
		VectorScale( tr->trDelta, phase, result );
		break;
	case TR_LINEAR_STOP:
		if ( atTime > tr->trTime + tr->trDuration ) {
			VectorClear( result );
			return;
		}
		VectorCopy( tr->trDelta, result );
		break;
	case TR_GRAVITY:
		deltaTime = ( atTime - tr->trTime ) * 0.001;	// milliseconds to seconds
		VectorCopy( tr->trDelta, result );
		result[2] -= mod_gravity * deltaTime;
		break;
	case TR_GRAVITY_WATER:
		deltaTime = ( atTime - tr->trTime ) * 0.001;	// milliseconds to seconds
		VectorCopy( tr->trDelta, result );
		result[2] -= (mod_gravity*0.50) * deltaTime;
		break;
	default:
		//Com_Error( ERR_DROP, "BG_EvaluateTrajectoryDelta: unknown trType: %i", tr->trTime );
		break;
	}
}

/*
================
ST_EvaluateTrajectory

================
*/
void ST_EvaluateTrajectory( const trajectory_t *tr, int atTime, vec3_t result, float mass ) {
	float		deltaTime;
	float		phase;

	switch( tr->trType ) {
	case TR_STATIONARY:
	case TR_INTERPOLATE:
		VectorCopy( tr->trBase, result );
		break;
	case TR_LINEAR:
		deltaTime = ( atTime - tr->trTime ) * 0.001;	// milliseconds to seconds
		VectorMA( tr->trBase, deltaTime, tr->trDelta, result );
		break;
	case TR_SINE:
		deltaTime = ( atTime - tr->trTime ) / (float) tr->trDuration;
		phase = sin( deltaTime * M_PI * 2 );
		VectorMA( tr->trBase, phase, tr->trDelta, result );
		break;
	case TR_LINEAR_STOP:
		if ( atTime > tr->trTime + tr->trDuration ) {
			atTime = tr->trTime + tr->trDuration;
		}
		deltaTime = ( atTime - tr->trTime ) * 0.001;	// milliseconds to seconds
		if ( deltaTime < 0 ) {
			deltaTime = 0;
		}
		VectorMA( tr->trBase, deltaTime, tr->trDelta, result );
		break;
	case TR_GRAVITY:
		deltaTime = ( atTime - tr->trTime ) * 0.001;	// milliseconds to seconds
		VectorMA( tr->trBase, deltaTime, tr->trDelta, result );
		result[2] -= 0.5 * (mod_gravity * mass) * deltaTime * deltaTime;
		break;
	case TR_GRAVITY_WATER:
		deltaTime = ( atTime - tr->trTime ) * 0.001;	// milliseconds to seconds
		VectorMA( tr->trBase, deltaTime, tr->trDelta, result );
		result[2] -= 0.5 * (mod_gravity * (mass*0.50)) * deltaTime * deltaTime;
		break;
	case TR_ROTATING:
		if ( tr->trTime > 0 )
			deltaTime = tr->trTime * 0.001;	// milliseconds to seconds
		else if ( tr->trTime < 0 )
			deltaTime = ( atTime + tr->trTime ) * 0.001;
		else
			deltaTime = ( atTime - tr->trTime ) * 0.001;
		VectorMA( tr->trBase, deltaTime, tr->trDelta, result );
		break;
	default:
		//Com_Error( ERR_DROP, "ST_EvaluateTrajectory: unknown trType: %i", tr->trTime );
		break;
	}
}

/*
================
ST_EvaluateTrajectoryDelta

For determining velocity at a given time
================
*/
void ST_EvaluateTrajectoryDelta( const trajectory_t *tr, int atTime, vec3_t result, float mass ) {
	float	deltaTime;
	float	phase;

	switch( tr->trType ) {
	case TR_STATIONARY:
	case TR_INTERPOLATE:
		VectorClear( result );
		break;
	case TR_ROTATING:
	case TR_LINEAR:
		VectorCopy( tr->trDelta, result );
		break;
	case TR_SINE:
		deltaTime = ( atTime - tr->trTime ) / (float) tr->trDuration;
		phase = cos( deltaTime * M_PI * 2 );	// derivative of sin = cos
		phase *= 0.5;
		VectorScale( tr->trDelta, phase, result );
		break;
	case TR_LINEAR_STOP:
		if ( atTime > tr->trTime + tr->trDuration ) {
			VectorClear( result );
			return;
		}
		VectorCopy( tr->trDelta, result );
		break;
	case TR_GRAVITY:
		deltaTime = ( atTime - tr->trTime ) * 0.001;	// milliseconds to seconds
		VectorCopy( tr->trDelta, result );
		result[2] -= (mod_gravity * mass) * deltaTime;
		break;
	case TR_GRAVITY_WATER:
		deltaTime = ( atTime - tr->trTime ) * 0.001;	// milliseconds to seconds
		VectorCopy( tr->trDelta, result );
		result[2] -= (mod_gravity * (mass*0.50)) * deltaTime;
		break;
	default:
		//Com_Error( ERR_DROP, "ST_EvaluateTrajectoryDelta: unknown trType: %i", tr->trTime );
		break;
	}
}

char *eventnames[] = {
	"EV_NONE",
	"EV_FOOTSTEP",
	"EV_FOOTSTEP_METAL",
	"EV_FOOTSPLASH",
	"EV_FOOTWADE",
	"EV_SWIM",
	"EV_STEP_4",
	"EV_STEP_8",
	"EV_STEP_12",
	"EV_STEP_16",
	"EV_FALL_SHORT",
	"EV_FALL_MEDIUM",
	"EV_FALL_FAR",
	"EV_JUMP_PAD",			// boing sound at origin, jump sound on player
	"EV_JUMP",                        //Event 14
	"EV_WATER_TOUCH",	// foot touches
	"EV_WATER_LEAVE",	// foot leaves
	"EV_WATER_UNDER",	// head touches
	"EV_WATER_CLEAR",	// head leaves
	"EV_ITEM_PICKUP",			// normal item pickups are predictable
	"EV_NOAMMO",
	"EV_CHANGE_WEAPON",
	"EV_FIRE_WEAPON",
	"EV_USE_ITEM0",                   //Event 24
	"EV_USE_ITEM1",
	"EV_USE_ITEM2",
	"EV_USE_ITEM3",
	"EV_USE_ITEM4",
	"EV_USE_ITEM5",
	"EV_USE_ITEM6",
	"EV_USE_ITEM7",
	"EV_USE_ITEM8",
	"EV_USE_ITEM9",
	"EV_USE_ITEM10",
	"EV_USE_ITEM11",
	"EV_USE_ITEM12",
	"EV_USE_ITEM13",
	"EV_USE_ITEM14",
	"EV_USE_ITEM15",
	"EV_ITEM_RESPAWN",                //Event 40
	"EV_ITEM_POP",
	"EV_PLAYER_TELEPORT_IN",
	"EV_PLAYER_TELEPORT_OUT",
	"EV_GRENADE_BOUNCE",		// eventParm will be the soundindex
	"EV_GENERAL_SOUND",
	"EV_GLOBAL_SOUND",		// no attenuation
	"EV_GLOBAL_TEAM_SOUND",
	"EV_BULLET_HIT_FLESH",
	"EV_BULLET_HIT_WALL",
	"EV_MISSILE_HIT",                 //Event 50
	"EV_MISSILE_MISS",
	"EV_MISSILE_MISS_METAL",
	"EV_RAILTRAIL",
	"EV_SHOTGUN",
	"EV_BULLET",				// otherEntity is the shooter
	"EV_PAIN",
	"EV_PAINVEHICLE",
	"EV_DEATH1",
	"EV_DEATH2",
	"EV_DEATH3",
	"EV_OBITUARY",                    //Event 60
	"EV_GIB_PLAYER",			// gib a previously living player
	"EV_PROXIMITY_MINE_STICK",
	"EV_PROXIMITY_MINE_TRIGGER",
	"EV_KAMIKAZE",			// kamikaze explodes
	"EV_OBELISKEXPLODE",		// obelisk explodes
	"EV_OBELISKPAIN",			// obelisk is in pain
	"EV_INVUL_IMPACT",		// invulnerability sphere impact
	"EV_JUICED",				// invulnerability juiced effect
	"EV_LIGHTNINGBOLT",		// lightning bolt bounced of invulnerability sphere
	"EV_DEBUG_LINE",
	"EV_STOPLOOPINGSOUND",
	"EV_TAUNT",
	"EV_TAUNT_YES",
	"EV_TAUNT_NO",
	"EV_TAUNT_FOLLOWME",
	"EV_TAUNT_GETFLAG",
	"EV_TAUNT_GUARDBASE",
	"EV_TAUNT_PATROL",
	"EV_EXPLOSION",
	"EV_PARTICLES_GRAVITY",
	"EV_PARTICLES_LINEAR",
	"EV_PARTICLES_LINEAR_UP",
	"EV_PARTICLES_LINEAR_DOWN",
	"EV_SMOKEPUFF",
	"EV_WATERPUFF",
	"EV_FOOTSTEP_FLESH",
	"EV_HORN",		//VEHICLES
	"EV_CRASH25",
	"EV_OT1_IMPACT",
	"EV_GRAVITYSOUND"
};

/*
===============
BG_AddPredictableEventToPlayerstate

Handles the sequence numbers
===============
*/

void	trap_Cvar_VariableStringBuffer( const char *var_name, char *buffer, int bufsize );

void BG_AddPredictableEventToPlayerstate( int newEvent, int eventParm, playerState_t *ps ) {

	switch(newEvent) {
		case EV_FOOTSTEP:
		case EV_FOOTSTEP_METAL:
		case EV_FOOTSTEP_FLESH:
		case EV_FALL_MEDIUM:
		case EV_FALL_FAR:
		case EV_JUMP:
		case EV_TAUNT:
		case EV_HORN:
		case EV_NOAMMO:
		case EV_CHANGE_WEAPON:
		case EV_FIRE_WEAPON:
			ps->eFlags |= EF_HEARED;
			ps->pm_time = 5;
			break;
	}

#ifdef _DEBUG
	{
		char buf[256];
		trap_Cvar_VariableStringBuffer("showevents", buf, sizeof(buf));
		if ( atof(buf) != 0 ) {
#ifdef GAME
			Com_Printf(" game event svt %5d -> %5d: num = %20s parm %d\n", ps->pmove_framecount/*ps->commandTime*/, ps->eventSequence, eventnames[newEvent], eventParm);
#else
			Com_Printf("Cgame event svt %5d -> %5d: num = %20s parm %d\n", ps->pmove_framecount/*ps->commandTime*/, ps->eventSequence, eventnames[newEvent], eventParm);
#endif
		}
	}
#endif
	ps->events[ps->eventSequence & (MAX_PS_EVENTS-1)] = newEvent;
	ps->eventParms[ps->eventSequence & (MAX_PS_EVENTS-1)] = eventParm;
	ps->eventSequence++;
}

/*
========================
BG_TouchJumpPad
========================
*/
void BG_TouchJumpPad( playerState_t *ps, entityState_t *jumppad ) {
	vec3_t	angles;
	float p;
	int effectNum;

	// spectators don't use jump pads
	if ( ps->pm_type != PM_NORMAL ) {
		return;
	}

	// flying characters don't hit bounce pads
	if ( ps->powerups[PW_FLIGHT] ) {
		return;
	}

	// if we didn't hit this same jumppad the previous frame
	// then don't play the event sound again if we are in a fat trigger
	if ( ps->jumppad_ent != jumppad->number ) {

		vectoangles( jumppad->origin2, angles);
		p = fabs( AngleNormalize180( angles[PITCH] ) );
		if( p < 45 ) {
			effectNum = 0;
		} else {
			effectNum = 1;
		}
		BG_AddPredictableEventToPlayerstate( EV_JUMP_PAD, effectNum, ps );
	}
	// remember hitting this jumppad this frame
	ps->jumppad_ent = jumppad->number;
	ps->jumppad_frame = ps->pmove_framecount;
	// give the player the velocity from the jumppad
	VectorCopy( jumppad->origin2, ps->velocity );
}

/*
========================
BG_PlayerStateToEntityState

This is done after each set of usercmd_t on the server,
and after local prediction on the client
========================
*/
void BG_PlayerStateToEntityState( playerState_t *ps, entityState_t *s, qboolean snap ) {
	int		i;

	if ( ps->pm_type == PM_INTERMISSION || ps->pm_type == PM_SPECTATOR ) {
		s->eType = ET_INVISIBLE;
	} else if ( ps->stats[STAT_HEALTH] <= GIB_HEALTH ) {
		s->eType = ET_INVISIBLE;
	} else {
		s->eType = ET_PLAYER;
	}

	s->number = ps->clientNum;

	s->pos.trType = TR_INTERPOLATE;
	VectorCopy( ps->origin, s->pos.trBase );
	if ( snap ) {
		SnapVector( s->pos.trBase );
	}
	// set the trDelta for flag direction
	VectorCopy( ps->velocity, s->pos.trDelta );

	s->apos.trType = TR_INTERPOLATE;
	VectorCopy( ps->viewangles, s->apos.trBase );
	if ( snap ) {
		SnapVector( s->apos.trBase );
	}

	s->angles2[YAW] = ps->movementDir;
	s->legsAnim = ps->legsAnim;
	s->torsoAnim = ps->torsoAnim;
	s->clientNum = ps->clientNum;		// ET_PLAYER looks here instead of at number
										// so corpses can also reference the proper config
	s->eFlags = ps->eFlags;
	if ( ps->stats[STAT_HEALTH] <= 0 ) {
		s->eFlags |= EF_DEAD;
	} else {
		s->eFlags &= ~EF_DEAD;
	}

	if ( ps->externalEvent ) {
		s->event = ps->externalEvent;
		s->eventParm = ps->externalEventParm;
	} else if ( ps->entityEventSequence < ps->eventSequence ) {
		int		seq;

		if ( ps->entityEventSequence < ps->eventSequence - MAX_PS_EVENTS) {
			ps->entityEventSequence = ps->eventSequence - MAX_PS_EVENTS;
		}
		seq = ps->entityEventSequence & (MAX_PS_EVENTS-1);
		s->event = ps->events[ seq ] | ( ( ps->entityEventSequence & 3 ) << 8 );
		s->eventParm = ps->eventParms[ seq ];
		ps->entityEventSequence++;
	}

	s->weapon = ps->weapon;
	s->groundEntityNum = ps->groundEntityNum;

	s->powerups = 0;
	for ( i = 0 ; i < MAX_POWERUPS ; i++ ) {
		if ( ps->powerups[ i ] ) {
			s->powerups |= 1 << i;
		}
	}

	s->loopSound = ps->loopSound;
	s->generic1 = ps->generic1;
}

/*
========================
BG_PlayerStateToEntityStateExtraPolate

This is done after each set of usercmd_t on the server,
and after local prediction on the client
========================
*/
void BG_PlayerStateToEntityStateExtraPolate( playerState_t *ps, entityState_t *s, int time, qboolean snap ) {
	int		i;

	if ( ps->pm_type == PM_INTERMISSION || ps->pm_type == PM_SPECTATOR ) {
		s->eType = ET_INVISIBLE;
	} else if ( ps->stats[STAT_HEALTH] <= GIB_HEALTH ) {
		s->eType = ET_INVISIBLE;
	} else {
		s->eType = ET_PLAYER;
	}

	s->number = ps->clientNum;

	s->pos.trType = TR_LINEAR_STOP;
	VectorCopy( ps->origin, s->pos.trBase );
	if ( snap ) {
		SnapVector( s->pos.trBase );
	}
	// set the trDelta for flag direction and linear prediction
	VectorCopy( ps->velocity, s->pos.trDelta );
	// set the time for linear prediction
	s->pos.trTime = time;
	// set maximum extra polation time
	s->pos.trDuration = 50; // 1000 / sv_fps (default = 20)

	s->apos.trType = TR_INTERPOLATE;
	VectorCopy( ps->viewangles, s->apos.trBase );
	if ( snap ) {
		SnapVector( s->apos.trBase );
	}

	s->angles2[YAW] = ps->movementDir;
	s->legsAnim = ps->legsAnim;
	s->torsoAnim = ps->torsoAnim;
	s->clientNum = ps->clientNum;		// ET_PLAYER looks here instead of at number
										// so corpses can also reference the proper config
	s->eFlags = ps->eFlags;
	if ( ps->stats[STAT_HEALTH] <= 0 ) {
		s->eFlags |= EF_DEAD;
	} else {
		s->eFlags &= ~EF_DEAD;
	}

	if ( ps->externalEvent ) {
		s->event = ps->externalEvent;
		s->eventParm = ps->externalEventParm;
	} else if ( ps->entityEventSequence < ps->eventSequence ) {
		int		seq;

		if ( ps->entityEventSequence < ps->eventSequence - MAX_PS_EVENTS) {
			ps->entityEventSequence = ps->eventSequence - MAX_PS_EVENTS;
		}
		seq = ps->entityEventSequence & (MAX_PS_EVENTS-1);
		s->event = ps->events[ seq ] | ( ( ps->entityEventSequence & 3 ) << 8 );
		s->eventParm = ps->eventParms[ seq ];
		ps->entityEventSequence++;
	}

	s->weapon = ps->weapon;
	s->groundEntityNum = ps->groundEntityNum;

	s->powerups = 0;
	for ( i = 0 ; i < MAX_POWERUPS ; i++ ) {
		if ( ps->powerups[ i ] ) {
			s->powerups |= 1 << i;
		}
	}

	s->loopSound = ps->loopSound;
	s->generic1 = ps->generic1;
}

/*
============
BG_TeamName
KK-OAX Copied from Tremulous
============
*/
char *BG_TeamName( team_t team )
{
  if( team == TEAM_NONE )
    return "spectator";
  if( team == TEAM_RED )
    return "Red";
  if( team == TEAM_BLUE )
    return "Blue";
  if( team == TEAM_FREE )
    return "Free";
  return "<team>";
}

int rq3_random(int min, int max) {
	int number;
	number = (rand() % (max - min + 1)) + (min);
	return number;
}

/*
==================
GetPlayerHoldable( int stat_holdable )
Determines what unique holdable item the player is carrying.
==================
*/
int GetPlayerHoldable( int stat_holdable ) {

	if ( stat_holdable & (1 << HI_TELEPORTER) )
		return HI_TELEPORTER;

	if ( stat_holdable & (1 << HI_MEDKIT) )
		return HI_MEDKIT;

	if ( stat_holdable & (1 << HI_KAMIKAZE) )
		return HI_KAMIKAZE;

	if ( stat_holdable & (1 << HI_PORTAL) )
		return HI_PORTAL;

	if ( stat_holdable & (1 << HI_INVULNERABILITY) )
		return HI_INVULNERABILITY;
	
	return HI_NONE;
}

/*
==================
GetHoldableListIndex( int giTag )
Determines what the index is of a holdable in the bg_itemlist
==================
*/
int GetHoldableListIndex( int giTag ) {
	int i;

	if ( giTag <= HI_NONE || giTag >= HI_NUM_HOLDABLE || giTag == HI_HOLDABLE_SPLIT )
		return 0;

	for (i = 0; i < bg_numItems; i++ ){
		if ( bg_itemlist[i].giType == IT_HOLDABLE && bg_itemlist[i].giTag == giTag )
			return i;
	}

	return 0;
}

int BG_VehicleCheckClass (int id){
	if(id == 1){ return VCLASS_CAR; }
	if(id == 2){ return VCLASS_CAR; }
	
	return 0;
}

float BG_GetVehicleSettings (int id, int set){
	if(id == 1){
		if(set==VSET_SPEED){ return 900; }
		if(set==VSET_GRAVITY){ return 0.4; }
	}
	if(id == 2){
		if(set==VSET_SPEED){ return 900; }
		if(set==VSET_GRAVITY){ return 0.4; }
	}
	
	return 0;
}
