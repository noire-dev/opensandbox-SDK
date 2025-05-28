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
// cg_main.c -- initialization and primary entry point for cgame
#include "../qcommon/ns_local.h"

void CG_Init( int serverMessageNum, int serverCommandSequence, int clientNum );
void CG_Shutdown( void );

int realVidWidth;
int realVidHeight;		// leilei - global video hack

/*
================
vmMain

This is the only way control passes into the module.
This must be the very first function compiled into the .q3vm file
================
*/
intptr_t vmMain( int command, int arg0, int arg1, int arg2, int arg3, int arg4, int arg5, int arg6, int arg7, int arg8, int arg9, int arg10, int arg11  ) {

	switch ( command ) {
	case CG_INIT:
		CG_Init( arg0, arg1, arg2 );
		return 0;
	case CG_SHUTDOWN:
		CG_Shutdown();
		return 0;
	case CG_CONSOLE_COMMAND:
		return CG_ConsoleCommand();
	case CG_DRAW_ACTIVE_FRAME:
		CG_DrawActiveFrame( arg0, arg1 );
		return 0;
	case CG_CROSSHAIR_PLAYER:
		return CG_CrosshairPlayer();
	case CG_LAST_ATTACKER:
		return CG_LastAttacker();
	default:
		CG_Error( "vmMain: unknown command %i", command );
		break;
	}
	return -1;
}

cg_t				cg;
cgs_t				cgs;
centity_t			cg_entities[MAX_GENTITIES];
weaponInfo_t		cg_weapons[WEAPONS_NUM];
itemInfo_t			cg_items[MAX_ITEMS];

int	mod_mgspread;
int	mod_sgspread;
int mod_sgcount;
int	mod_lgrange;
int	mod_cgspread;
int mod_jumpheight;
int	mod_gdelay;
int	mod_mgdelay;
int	mod_sgdelay;
int	mod_gldelay;
int	mod_rldelay;
int	mod_lgdelay;
int	mod_pgdelay;
int	mod_rgdelay;
int	mod_bfgdelay;
int	mod_ngdelay;
int	mod_pldelay;
int	mod_cgdelay;
int	mod_ftdelay;
int	mod_amdelay;
float mod_hastefirespeed;
float mod_ammoregenfirespeed;
float mod_scoutfirespeed;
float	mod_guardfirespeed;
float	mod_doublerfirespeed;
int mod_noplayerclip;
int	mod_ammolimit;
int mod_invulmove;
float mod_teamred_firespeed;
float mod_teamblue_firespeed;
int mod_medkitlimit;
int mod_medkitinf;
int mod_teleporterinf;
int mod_portalinf;
int mod_kamikazeinf;
int mod_invulinf;
int mod_accelerate;
int mod_movetype;
int mod_gravity;
int mod_fogModel;
int mod_fogShader;
int mod_fogDistance;
int mod_fogInterval;
int mod_fogColorR;
int mod_fogColorG;
int mod_fogColorB;
int mod_fogColorA;
int mod_skyShader;
int mod_skyColorR;
int mod_skyColorG;
int mod_skyColorB;
int mod_skyColorA;

/*
=================
CG_CreateCvars
=================
*/
void CG_CreateCvars( void ) {
	char		var[MAX_TOKEN_CHARS];

	// see if we are also running the server on this machine
	trap_Cvar_VariableStringBuffer( "sv_running", var, sizeof( var ) );
	cgs.localServer = atoi( var );

	trap_Cvar_Register(NULL, "model", "beret/default", CVAR_USERINFO | CVAR_ARCHIVE );
	trap_Cvar_Register(NULL, "headmodel", "beret/default", CVAR_USERINFO | CVAR_ARCHIVE );
	trap_Cvar_Register(NULL, "legsmodel", "beret/default", CVAR_USERINFO | CVAR_ARCHIVE );
	trap_Cvar_Register(NULL, "team_model", "beret/default", CVAR_USERINFO | CVAR_ARCHIVE );
	trap_Cvar_Register(NULL, "team_headmodel", "beret/default", CVAR_USERINFO | CVAR_ARCHIVE );
	trap_Cvar_Register(NULL, "team_legsmodel", "beret/default", CVAR_USERINFO | CVAR_ARCHIVE );
}

int CG_CrosshairPlayer( void ) {
	if ( cg.time > ( cg.crosshairClientTime + 1000 ) ) {
		return -1;
	}
	return cg.crosshairClientNum;
}

int CG_LastAttacker( void ) {
	if ( !cg.attackerTime ) {
		return -1;
	}
	return cg.snap->ps.persistant[PERS_ATTACKER];
}

void QDECL CG_PrintfChat( qboolean team, const char *msg, ... ) {
	va_list		argptr;
	char		text[1024];

	va_start (argptr, msg);
	Q_vsnprintf (text, sizeof(text), msg, argptr);
	va_end (argptr);

	if (team) {
		CG_AddToGenericConsole(text, &cgs.teamChat);
	} else {
		CG_AddToGenericConsole(text, &cgs.chat);
	}
	CG_AddToGenericConsole(text, &cgs.commonConsole);
	trap_Print( text );
}

void QDECL CG_Printf( const char *msg, ... ) {
	va_list		argptr;
	char		text[1024];

	va_start (argptr, msg);
	Q_vsnprintf (text, sizeof(text), msg, argptr);
	va_end (argptr);

	CG_AddToGenericConsole(text, &cgs.console);
	CG_AddToGenericConsole(text, &cgs.commonConsole);
	trap_Print( text );
}

void QDECL CG_Error( const char *msg, ... ) {
	va_list		argptr;
	char		text[1024];

	va_start (argptr, msg);
	Q_vsnprintf (text, sizeof(text), msg, argptr);
	va_end (argptr);

	trap_Error( text );
}

void QDECL Com_Error( int level, const char *error, ... ) {
	va_list		argptr;
	char		text[1024];

	va_start (argptr, error);
	Q_vsnprintf (text, sizeof(text), error, argptr);
	va_end (argptr);

	CG_Error( "%s", text);
}

void QDECL Com_Printf( const char *msg, ... ) {
	va_list		argptr;
	char		text[1024];

	va_start (argptr, msg);
	Q_vsnprintf (text, sizeof(text), msg, argptr);
	va_end (argptr);

	CG_Printf ("%s", text);
}

/*
================
CG_Argv
================
*/
char *CG_Argv( int arg ) {
	static char	buffer[MAX_STRING_CHARS];

	trap_Argv( arg, buffer, sizeof( buffer ) );

	return buffer;
}

/*
=================
CG_RegisterItemSounds

The server says this item is used on this level
=================
*/
static void CG_RegisterItemSounds( int itemNum ) {
	gitem_t			*item;
	char			data[MAX_QPATH];
	char			*s, *start;
	int				len;

	item = &bg_itemlist[ itemNum ];

	if( item->pickup_sound ) {
		trap_S_RegisterSound( item->pickup_sound, qfalse );
	}

	// parse the space seperated precache string for other media
	s = item->sounds;
	if (!s || !s[0])
		return;

	while (*s) {
		start = s;
		while (*s && *s != ' ') {
			s++;
		}

		len = s-start;
		if (len >= MAX_QPATH || len < 5) {
			CG_Error( "PrecacheItem: %s has bad precache string",
				item->classname);
			return;
		}
		memcpy (data, start, len);
		data[len] = 0;
		if ( *s ) {
			s++;
		}

		if ( !strcmp(data+len-3, "wav" )) {
			trap_S_RegisterSound( data, qfalse );
		}
	}
}

/*
=================
CG_RegisterSounds

called during a precache command
=================
*/
static void CG_RegisterSounds( void ) {
	int		i;
	char	items[MAX_ITEMS+1];
	char	name[MAX_QPATH];
	const char	*soundName;

	cgs.media.oneMinuteSound = trap_S_RegisterSound( "sound/feedback/1_minute.wav", qtrue );
	cgs.media.fiveMinuteSound = trap_S_RegisterSound( "sound/feedback/5_minute.wav", qtrue );
	cgs.media.suddenDeathSound = trap_S_RegisterSound( "sound/feedback/sudden_death.wav", qtrue );
	cgs.media.oneFragSound = trap_S_RegisterSound( "sound/feedback/1_frag.wav", qtrue );
	cgs.media.twoFragSound = trap_S_RegisterSound( "sound/feedback/2_frags.wav", qtrue );
	cgs.media.threeFragSound = trap_S_RegisterSound( "sound/feedback/3_frags.wav", qtrue );
	cgs.media.count3Sound = trap_S_RegisterSound( "sound/feedback/three.wav", qtrue );
	cgs.media.count2Sound = trap_S_RegisterSound( "sound/feedback/two.wav", qtrue );
	cgs.media.count1Sound = trap_S_RegisterSound( "sound/feedback/one.wav", qtrue );
	cgs.media.countFightSound = trap_S_RegisterSound( "sound/feedback/fight.wav", qtrue );
	cgs.media.countPrepareSound = trap_S_RegisterSound( "sound/feedback/prepare.wav", qtrue );

	// N_G: Another condition that makes no sense to me, see for
	// yourself if you really meant this
	// Sago: Makes perfect sense: Load team game stuff if the gametype is a teamgame and not an exception (like GT_LMS)
	if (cgs.gametype >= GT_TEAM && cgs.ffa_gt != 1) {
		cgs.media.captureAwardSound = trap_S_RegisterSound( "sound/teamplay/flagcapture_yourteam.wav", qtrue );
		cgs.media.redLeadsSound = trap_S_RegisterSound( "sound/feedback/redleads.wav", qtrue );
		cgs.media.blueLeadsSound = trap_S_RegisterSound( "sound/feedback/blueleads.wav", qtrue );
		cgs.media.teamsTiedSound = trap_S_RegisterSound( "sound/feedback/teamstied.wav", qtrue );
		cgs.media.hitTeamSound = trap_S_RegisterSound( "sound/feedback/hit_teammate.wav", qtrue );

		cgs.media.redScoredSound = trap_S_RegisterSound( "sound/teamplay/voc_red_scores.wav", qtrue );
		cgs.media.blueScoredSound = trap_S_RegisterSound( "sound/teamplay/voc_blue_scores.wav", qtrue );

		cgs.media.captureYourTeamSound = trap_S_RegisterSound( "sound/teamplay/flagcapture_yourteam.wav", qtrue );
		cgs.media.captureOpponentSound = trap_S_RegisterSound( "sound/teamplay/flagcapture_opponent.wav", qtrue );

		cgs.media.returnYourTeamSound = trap_S_RegisterSound( "sound/teamplay/flagreturn_yourteam.wav", qtrue );
		cgs.media.returnOpponentSound = trap_S_RegisterSound( "sound/teamplay/flagreturn_opponent.wav", qtrue );

		cgs.media.takenYourTeamSound = trap_S_RegisterSound( "sound/teamplay/flagtaken_yourteam.wav", qtrue );
		cgs.media.takenOpponentSound = trap_S_RegisterSound( "sound/teamplay/flagtaken_opponent.wav", qtrue );

		if ( cgs.gametype == GT_CTF || cgs.gametype == GT_CTF_ELIMINATION ) {
			cgs.media.redFlagReturnedSound = trap_S_RegisterSound( "sound/teamplay/voc_red_returned.wav", qtrue );
			cgs.media.blueFlagReturnedSound = trap_S_RegisterSound( "sound/teamplay/voc_blue_returned.wav", qtrue );
			cgs.media.enemyTookYourFlagSound = trap_S_RegisterSound( "sound/teamplay/voc_enemy_flag.wav", qtrue );
			cgs.media.yourTeamTookEnemyFlagSound = trap_S_RegisterSound( "sound/teamplay/voc_team_flag.wav", qtrue );
		}

		if ( cgs.gametype == GT_1FCTF ) {
			// FIXME: get a replacement for this sound ?
			cgs.media.neutralFlagReturnedSound = trap_S_RegisterSound( "sound/teamplay/flagreturn_opponent.wav", qtrue );
			cgs.media.yourTeamTookTheFlagSound = trap_S_RegisterSound( "sound/teamplay/voc_team_1flag.wav", qtrue );
			cgs.media.enemyTookTheFlagSound = trap_S_RegisterSound( "sound/teamplay/voc_enemy_1flag.wav", qtrue );
		}

		if ( cgs.gametype == GT_1FCTF || cgs.gametype == GT_CTF || cgs.gametype == GT_CTF_ELIMINATION ) {
			cgs.media.youHaveFlagSound = trap_S_RegisterSound( "sound/teamplay/voc_you_flag.wav", qtrue );
			cgs.media.holyShitSound = trap_S_RegisterSound("sound/feedback/voc_holyshit.wav", qtrue);
		}

        if ( cgs.gametype == GT_OBELISK ) {
			cgs.media.yourBaseIsUnderAttackSound = trap_S_RegisterSound( "sound/teamplay/voc_base_attack.wav", qtrue );
		}
	}

	cgs.media.tracerSound = trap_S_RegisterSound( "sound/weapons/machinegun/buletby1.wav", qfalse );
	cgs.media.selectSound = trap_S_RegisterSound( "sound/weapons/change.wav", qfalse );
	cgs.media.wearOffSound = trap_S_RegisterSound( "sound/items/wearoff.wav", qfalse );
	cgs.media.useNothingSound = trap_S_RegisterSound( "sound/items/use_nothing.wav", qfalse );
	cgs.media.gibSound = trap_S_RegisterSound( "sound/player/gibsplt1.wav", qfalse );
	cgs.media.gibBounce1Sound = trap_S_RegisterSound( "sound/player/gibimp1.wav", qfalse );
	cgs.media.gibBounce2Sound = trap_S_RegisterSound( "sound/player/gibimp2.wav", qfalse );
	cgs.media.gibBounce3Sound = trap_S_RegisterSound( "sound/player/gibimp3.wav", qfalse );

	cgs.media.lspl1Sound = trap_S_RegisterSound( "sound/le/splat1.wav", qfalse );
	cgs.media.lspl2Sound = trap_S_RegisterSound( "sound/le/splat2.wav", qfalse );
	cgs.media.lspl3Sound = trap_S_RegisterSound( "sound/le/splat3.wav", qfalse );

	cgs.media.lbul1Sound = trap_S_RegisterSound( "sound/le/bullet1.wav", qfalse );
	cgs.media.lbul2Sound = trap_S_RegisterSound( "sound/le/bullet2.wav", qfalse );
	cgs.media.lbul3Sound = trap_S_RegisterSound( "sound/le/bullet3.wav", qfalse );

	cgs.media.lshl1Sound = trap_S_RegisterSound( "sound/le/shell1.wav", qfalse );
	cgs.media.lshl2Sound = trap_S_RegisterSound( "sound/le/shell2.wav", qfalse );
	cgs.media.lshl3Sound = trap_S_RegisterSound( "sound/le/shell3.wav", qfalse );

	cgs.media.useInvulnerabilitySound = trap_S_RegisterSound( "sound/items/invul_activate.wav", qfalse );
	cgs.media.invulnerabilityImpactSound1 = trap_S_RegisterSound( "sound/items/invul_impact_01.wav", qfalse );
	cgs.media.invulnerabilityImpactSound2 = trap_S_RegisterSound( "sound/items/invul_impact_02.wav", qfalse );
	cgs.media.invulnerabilityImpactSound3 = trap_S_RegisterSound( "sound/items/invul_impact_03.wav", qfalse );
	cgs.media.invulnerabilityJuicedSound = trap_S_RegisterSound( "sound/items/invul_juiced.wav", qfalse );

	cgs.media.ammoregenSound = trap_S_RegisterSound("sound/items/cl_ammoregen.wav", qfalse);
	cgs.media.doublerSound = trap_S_RegisterSound("sound/items/cl_doubler.wav", qfalse);
	cgs.media.guardSound = trap_S_RegisterSound("sound/items/cl_guard.wav", qfalse);
	cgs.media.scoutSound = trap_S_RegisterSound("sound/items/cl_scout.wav", qfalse);
        cgs.media.obeliskHitSound1 = trap_S_RegisterSound( "sound/items/obelisk_hit_01.wav", qfalse );
	cgs.media.obeliskHitSound2 = trap_S_RegisterSound( "sound/items/obelisk_hit_02.wav", qfalse );
	cgs.media.obeliskHitSound3 = trap_S_RegisterSound( "sound/items/obelisk_hit_03.wav", qfalse );
	cgs.media.obeliskRespawnSound = trap_S_RegisterSound( "sound/items/obelisk_respawn.wav", qfalse );

	cgs.media.teleInSound = trap_S_RegisterSound( "sound/world/telein.wav", qfalse );
	cgs.media.teleOutSound = trap_S_RegisterSound( "sound/world/teleout.wav", qfalse );
	cgs.media.respawnSound = trap_S_RegisterSound( "sound/items/respawn1.wav", qfalse );

	cgs.media.noAmmoSound = trap_S_RegisterSound( "sound/weapons/noammo.wav", qfalse );

	cgs.media.talkSound = trap_S_RegisterSound( "sound/player/talk.wav", qfalse );
	cgs.media.landSound = trap_S_RegisterSound( "sound/player/land1.wav", qfalse);

	cgs.media.notifySound = trap_S_RegisterSound( "sound/notify.wav", qfalse );
	cgs.media.undoSound = trap_S_RegisterSound( "sound/undo.wav", qfalse );

    cgs.media.hitSound = trap_S_RegisterSound( "sound/feedback/hitde.wav", qfalse );
	cgs.media.hitSoundHighArmor = trap_S_RegisterSound( "sound/feedback/hithi.wav", qfalse );
	cgs.media.hitSoundLowArmor = trap_S_RegisterSound( "sound/feedback/hitlo.wav", qfalse );

	cgs.media.impressiveSound = trap_S_RegisterSound( "sound/feedback/impressive.wav", qtrue );
	cgs.media.excellentSound = trap_S_RegisterSound( "sound/feedback/excellent.wav", qtrue );
	cgs.media.deniedSound = trap_S_RegisterSound( "sound/feedback/denied.wav", qtrue );
	cgs.media.humiliationSound = trap_S_RegisterSound( "sound/feedback/humiliation.wav", qtrue );
	cgs.media.assistSound = trap_S_RegisterSound( "sound/feedback/assist.wav", qtrue );
	cgs.media.defendSound = trap_S_RegisterSound( "sound/feedback/defense.wav", qtrue );

	cgs.media.takenLeadSound = trap_S_RegisterSound( "sound/feedback/takenlead.wav", qtrue);
	cgs.media.tiedLeadSound = trap_S_RegisterSound( "sound/feedback/tiedlead.wav", qtrue);
	cgs.media.lostLeadSound = trap_S_RegisterSound( "sound/feedback/lostlead.wav", qtrue);

	cgs.media.voteNow = trap_S_RegisterSound( "sound/feedback/vote_now.wav", qtrue);
	cgs.media.votePassed = trap_S_RegisterSound( "sound/feedback/vote_passed.wav", qtrue);
	cgs.media.voteFailed = trap_S_RegisterSound( "sound/feedback/vote_failed.wav", qtrue);

	cgs.media.watrInSound = trap_S_RegisterSound( "sound/player/watr_in.wav", qfalse);
	cgs.media.watrOutSound = trap_S_RegisterSound( "sound/player/watr_out.wav", qfalse);
	cgs.media.watrUnSound = trap_S_RegisterSound( "sound/player/watr_un.wav", qfalse);

	cgs.media.jumpPadSound = trap_S_RegisterSound ("sound/world/jumppad.wav", qfalse );

	for (i=0 ; i<4 ; i++) {
		Com_sprintf (name, sizeof(name), "sound/player/footsteps/step%i.wav", i+1);
		cgs.media.footsteps[FOOTSTEP_NORMAL][i] = trap_S_RegisterSound (name, qfalse);

		Com_sprintf (name, sizeof(name), "sound/player/footsteps/boot%i.wav", i+1);
		cgs.media.footsteps[FOOTSTEP_BOOT][i] = trap_S_RegisterSound (name, qfalse);

		Com_sprintf (name, sizeof(name), "sound/player/footsteps/flesh%i.wav", i+1);
		cgs.media.footsteps[FOOTSTEP_FLESH][i] = trap_S_RegisterSound (name, qfalse);

		Com_sprintf (name, sizeof(name), "sound/player/footsteps/mech%i.wav", i+1);
		cgs.media.footsteps[FOOTSTEP_MECH][i] = trap_S_RegisterSound (name, qfalse);

		Com_sprintf (name, sizeof(name), "sound/player/footsteps/energy%i.wav", i+1);
		cgs.media.footsteps[FOOTSTEP_ENERGY][i] = trap_S_RegisterSound (name, qfalse);

		Com_sprintf (name, sizeof(name), "sound/player/footsteps/splash%i.wav", i+1);
		cgs.media.footsteps[FOOTSTEP_SPLASH][i] = trap_S_RegisterSound (name, qfalse);

		Com_sprintf (name, sizeof(name), "sound/player/footsteps/clank%i.wav", i+1);
		cgs.media.footsteps[FOOTSTEP_METAL][i] = trap_S_RegisterSound (name, qfalse);
	}
	
	for (i=0 ; i<=10 ; i++) {
		Com_sprintf (name, sizeof(name), "sound/vehicle/engine%i.ogg", i);
		cgs.media.carengine[i] = trap_S_RegisterSound (name, qfalse);
	}

	// only register the items that the server says we need
	Q_strncpyz(items, CG_ConfigString(CS_ITEMS), sizeof(items));

	for ( i = 1 ; i < bg_numItems ; i++ ) {
		CG_RegisterItemSounds( i );
	}

	for ( i = 1 ; i < MAX_SOUNDS ; i++ ) {
		soundName = CG_ConfigString( CS_SOUNDS+i );
		if ( !soundName[0] ) {
			break;
		}
		if ( soundName[0] == '*' ) {
			continue;	// custom sound
		}
		cgs.gameSounds[i] = trap_S_RegisterSound( soundName, qfalse );
	}

	// FIXME: only needed with item
	cgs.media.flightSound = trap_S_RegisterSound( "sound/items/flight.wav", qfalse );
	cgs.media.medkitSound = trap_S_RegisterSound ("sound/items/use_medkit.wav", qfalse);
	cgs.media.quadSound = trap_S_RegisterSound("sound/items/damage3.wav", qfalse);
	cgs.media.sfx_ric1 = trap_S_RegisterSound ("sound/weapons/machinegun/ric1.wav", qfalse);
	cgs.media.sfx_ric2 = trap_S_RegisterSound ("sound/weapons/machinegun/ric2.wav", qfalse);
	cgs.media.sfx_ric3 = trap_S_RegisterSound ("sound/weapons/machinegun/ric3.wav", qfalse);
	cgs.media.sfx_railg = trap_S_RegisterSound ("sound/weapons/railgun/railgf1a.wav", qfalse);
	cgs.media.sfx_rockexp = trap_S_RegisterSound ("sound/weapons/rocket/rocklx1a.wav", qfalse);
	cgs.media.sfx_plasmaexp = trap_S_RegisterSound ("sound/weapons/plasma/plasmx1a.wav", qfalse);
	cgs.media.sfx_proxexp = trap_S_RegisterSound( "sound/weapons/proxmine/wstbexpl.wav" , qfalse);
	cgs.media.sfx_nghit = trap_S_RegisterSound( "sound/weapons/nailgun/wnalimpd.wav" , qfalse);
	cgs.media.sfx_nghitflesh = trap_S_RegisterSound( "sound/weapons/nailgun/wnalimpl.wav" , qfalse);
	cgs.media.sfx_nghitmetal = trap_S_RegisterSound( "sound/weapons/nailgun/wnalimpm.wav", qfalse );
	cgs.media.sfx_chghit = trap_S_RegisterSound( "sound/weapons/vulcan/wvulimpd.wav", qfalse );
	cgs.media.sfx_chghitflesh = trap_S_RegisterSound( "sound/weapons/vulcan/wvulimpl.wav", qfalse );
	cgs.media.sfx_chghitmetal = trap_S_RegisterSound( "sound/weapons/vulcan/wvulimpm.wav", qfalse );
	cgs.media.weaponHoverSound = trap_S_RegisterSound( "sound/weapons/weapon_hover.wav", qfalse );
	cgs.media.kamikazeExplodeSound = trap_S_RegisterSound( "sound/items/kam_explode.wav", qfalse );
	cgs.media.kamikazeImplodeSound = trap_S_RegisterSound( "sound/items/kam_implode.wav", qfalse );
	cgs.media.kamikazeFarSound = trap_S_RegisterSound( "sound/items/kam_explode_far.wav", qfalse );
	cgs.media.winnerSound = trap_S_RegisterSound( "sound/feedback/voc_youwin.wav", qfalse );
	cgs.media.loserSound = trap_S_RegisterSound( "sound/feedback/voc_youlose.wav", qfalse );
	cgs.media.youSuckSound = trap_S_RegisterSound( "sound/misc/yousuck.wav", qfalse );

	cgs.media.wstbimplSound = trap_S_RegisterSound("sound/weapons/proxmine/wstbimpl.wav", qfalse);
	cgs.media.wstbimpmSound = trap_S_RegisterSound("sound/weapons/proxmine/wstbimpm.wav", qfalse);
	cgs.media.wstbimpdSound = trap_S_RegisterSound("sound/weapons/proxmine/wstbimpd.wav", qfalse);
	cgs.media.wstbactvSound = trap_S_RegisterSound("sound/weapons/proxmine/wstbactv.wav", qfalse);

	cgs.media.regenSound = trap_S_RegisterSound("sound/items/regen.wav", qfalse);
	cgs.media.protectSound = trap_S_RegisterSound("sound/items/protect3.wav", qfalse);
	cgs.media.n_healthSound = trap_S_RegisterSound("sound/items/n_health.wav", qfalse );
	cgs.media.hgrenb1aSound = trap_S_RegisterSound("sound/weapons/grenade/hgrenb1a.wav", qfalse);
	cgs.media.hgrenb2aSound = trap_S_RegisterSound("sound/weapons/grenade/hgrenb2a.wav", qfalse);
}

/*
=================
CG_RegisterOverlay

Registers the graphic for the target_effect overlay.
=================
*/
void CG_RegisterOverlay( void ) {
	const char *overlay;

	overlay = CG_ConfigString( CS_OVERLAY );
	if ( strlen(overlay) ) {
		cgs.media.effectOverlay = trap_R_RegisterShaderNoMip( overlay );
	} else {
		cgs.media.effectOverlay = 0;
	}
}

void CG_SetDefaultGameCvars(void) {
	mod_sgspread = 700;
	mod_sgcount = 11;
	mod_lgrange = 768;
	mod_mgspread = 200;
	mod_cgspread = 600;
	mod_jumpheight = 270;
	mod_gdelay = 400;
	mod_mgdelay = 100;
	mod_sgdelay = 1000;
	mod_gldelay = 800;
	mod_rldelay = 800;
	mod_lgdelay = 50;
	mod_pgdelay = 100;
	mod_rgdelay = 1500;
	mod_bfgdelay = 200;
	mod_ngdelay = 1000;
	mod_pldelay = 800;
	mod_cgdelay = 30;
	mod_ftdelay = 40;
	mod_amdelay = 40;
	mod_scoutfirespeed = 1.5;
	mod_doublerfirespeed = 1;
	mod_guardfirespeed = 1;
	mod_hastefirespeed = 1.3;
	mod_ammoregenfirespeed = 1.3;
	mod_noplayerclip = 0;
	mod_ammolimit = 200;
	mod_invulmove = 0;
	mod_teamred_firespeed = 1;
	mod_teamblue_firespeed = 1;
	mod_medkitlimit = 200;
	mod_medkitinf = 0;
	mod_teleporterinf = 0;
	mod_portalinf = 0;
	mod_kamikazeinf = 0;
	mod_invulinf = 0;
	mod_accelerate = 1;
	mod_movetype = 0;
	mod_gravity = 800;
    mod_fogModel = 0;
    mod_fogShader = 0;
    mod_fogDistance = 0;
    mod_fogInterval = 0;
    mod_fogColorR = 0;
    mod_fogColorG = 0;
    mod_fogColorB = 0;
    mod_fogColorA = 0;
    mod_skyShader = 0;
    mod_skyColorR = 0;
    mod_skyColorG = 0;
    mod_skyColorB = 0;
    mod_skyColorA = 0;
}

/*
=================
CG_RegisterGraphics

This function may execute for a couple of minutes with a slow disk.
=================
*/
static void CG_RegisterGraphics( void ) {
	int			i;
	char		items[MAX_ITEMS+1];
	static char		*sb_nums[11] = {
		"gfx/2d/numbers/zero_32b",
		"gfx/2d/numbers/one_32b",
		"gfx/2d/numbers/two_32b",
		"gfx/2d/numbers/three_32b",
		"gfx/2d/numbers/four_32b",
		"gfx/2d/numbers/five_32b",
		"gfx/2d/numbers/six_32b",
		"gfx/2d/numbers/seven_32b",
		"gfx/2d/numbers/eight_32b",
		"gfx/2d/numbers/nine_32b",
		"gfx/2d/numbers/minus_32b",
	};

	// clear any references to old media
	memset( &cg.refdef, 0, sizeof( cg.refdef ) );
	trap_R_ClearScene();

	CG_LoadingString( cgs.mapname, -1 );

	trap_R_LoadWorldMap( cgs.mapname );
	
	CG_SetDefaultGameCvars();

	// precache status bar pics
	CG_LoadingString( "game media", 0.70 );

	for ( i=0 ; i<11 ; i++) {
		cgs.media.numberShaders[i] = trap_R_RegisterShader( sb_nums[i] );
	}

	cgs.media.viewBloodShader = trap_R_RegisterShader( "viewBloodBlend" );

	cgs.media.objectivesOverlay = trap_R_RegisterShaderNoMip( CG_ConfigString(CS_OBJECTIVESOVERLAY) );
	cgs.media.objectivesUpdated = trap_R_RegisterShaderNoMip( "menu/objectives/updated.tga" );
	cgs.media.objectivesUpdatedSound = trap_S_RegisterSound( "sound/misc/objective_update_01.wav", qfalse );

	cgs.media.deathImage = trap_R_RegisterShaderNoMip( "menu/assets/level_complete5" );

	cgs.media.scoreShow = trap_S_RegisterSound( "sound/weapons/rocket/rocklx1a.wav", qfalse );
	cgs.media.finalScoreShow = trap_S_RegisterSound( "sound/weapons/rocket/rocklx1a.wav", qfalse );

	cgs.media.smokePuffShader = trap_R_RegisterShader( "smokePuff" );
	cgs.media.smokePuffRageProShader = trap_R_RegisterShader( "smokePuffRagePro" );
	cgs.media.shotgunSmokePuffShader = trap_R_RegisterShader( "shotgunSmokePuff" );
	cgs.media.nailPuffShader = trap_R_RegisterShader( "nailtrail" );
	cgs.media.blueProxMine = trap_R_RegisterModel( "models/weaphits/proxmineb.md3" );
	cgs.media.plasmaBallShader = trap_R_RegisterShader( "sprites/plasma1" );
	cgs.media.flameBallShader = trap_R_RegisterShader( "sprites/flameball" );
	cgs.media.antimatterBallShader = trap_R_RegisterShader( "sprites/antimatter" );
	cgs.media.bloodTrailShader = trap_R_RegisterShader( "bloodTrail" );
	cgs.media.lagometerShader = trap_R_RegisterShader("lagometer" );
	cgs.media.connectionShader = trap_R_RegisterShader( "disconnected" );

	cgs.media.waterBubbleShader = trap_R_RegisterShader( "waterBubble" );

	cgs.media.tracerShader = trap_R_RegisterShader( "gfx/misc/tracer" );
	cgs.media.selectShader = trap_R_RegisterShader( "gfx/2d/select" );

	for (i = 0; i < NUM_CROSSHAIRS; i++ ) {
		if (i < 10){
			cgs.media.crosshairShader[i] = trap_R_RegisterShader( va("gfx/2d/crosshair%c", 'a'+i) );
		    cgs.media.crosshairSh3d[i] = trap_R_RegisterShader( va("gfx/3d/crosshair%c", 'a'+i) );
		} else {
			cgs.media.crosshairShader[i] = trap_R_RegisterShader( va("gfx/2d/crosshair%02d", i - 10) );
			cgs.media.crosshairSh3d[i] = trap_R_RegisterShader( va("gfx/3d/crosshair%02d", i - 10) );
		}
 	}

	cgs.media.backTileShader = trap_R_RegisterShader( "gfx/2d/backtile" );
	cgs.media.noammoShader = trap_R_RegisterShader( "icons/noammo" );

	// powerup shaders
	cgs.media.quadShader = trap_R_RegisterShader("powerups/quad" );
	cgs.media.quadWeaponShader = trap_R_RegisterShader("powerups/quadWeapon" );
	cgs.media.battleSuitShader = trap_R_RegisterShader("powerups/battleSuit" );
	cgs.media.battleWeaponShader = trap_R_RegisterShader("powerups/battleWeapon" );
	cgs.media.invisShader = trap_R_RegisterShader("powerups/invisibility" );
	cgs.media.regenShader = trap_R_RegisterShader("powerups/regen" );
	cgs.media.hastePuffShader = trap_R_RegisterShader("hasteSmokePuff" );
	
	cgs.media.ptexShader[0]	= trap_R_RegisterShader( "trans" );
	cgs.media.ptexShader[1]	= trap_R_RegisterShader( "powerups/quad" );

	if ( cgs.gametype == GT_CTF || cgs.gametype == GT_CTF_ELIMINATION|| cgs.gametype == GT_1FCTF || cgs.gametype == GT_HARVESTER ) {
		cgs.media.redCubeModel = trap_R_RegisterModel( "models/powerups/orb/r_orb.md3" );
		cgs.media.blueCubeModel = trap_R_RegisterModel( "models/powerups/orb/b_orb.md3" );
		cgs.media.redCubeIcon = trap_R_RegisterShader( "icons/skull_red" );
		cgs.media.blueCubeIcon = trap_R_RegisterShader( "icons/skull_blue" );
	}

    if( ( cgs.gametype >= GT_TEAM ) && ( cgs.ffa_gt != 1 ) ) {
        cgs.media.redOverlay = trap_R_RegisterShader( "overlay/red");
        cgs.media.blueOverlay = trap_R_RegisterShader( "overlay/blue");
    } else {
        cgs.media.neutralOverlay = trap_R_RegisterShader( "overlay/neutral");
    }

//For Double Domination:
	if ( cgs.gametype == GT_DOUBLE_D ) {
		cgs.media.ddPointSkinA[TEAM_RED] = trap_R_RegisterShaderNoMip( "icons/icona_red" );
        cgs.media.ddPointSkinA[TEAM_BLUE] = trap_R_RegisterShaderNoMip( "icons/icona_blue" );
        cgs.media.ddPointSkinA[TEAM_FREE] = trap_R_RegisterShaderNoMip( "icons/icona_white" );
        cgs.media.ddPointSkinA[TEAM_NONE] = trap_R_RegisterShaderNoMip( "icons/noammo" );
        cgs.media.ddPointSkinB[TEAM_RED] = trap_R_RegisterShaderNoMip( "icons/iconb_red" );
        cgs.media.ddPointSkinB[TEAM_BLUE] = trap_R_RegisterShaderNoMip( "icons/iconb_blue" );
        cgs.media.ddPointSkinB[TEAM_FREE] = trap_R_RegisterShaderNoMip( "icons/iconb_white" );
        cgs.media.ddPointSkinB[TEAM_NONE] = trap_R_RegisterShaderNoMip( "icons/noammo" );
	}

	if ( cgs.gametype == GT_CTF || cgs.gametype == GT_CTF_ELIMINATION || cgs.gametype == GT_1FCTF || cgs.gametype == GT_HARVESTER ) {
		cgs.media.redFlagModel = trap_R_RegisterModel( "models/flags/r_flag.md3" );
		cgs.media.blueFlagModel = trap_R_RegisterModel( "models/flags/b_flag.md3" );
        cgs.media.neutralFlagModel = trap_R_RegisterModel( "models/flags/n_flag.md3" );
		cgs.media.redFlagShader[0] = trap_R_RegisterShaderNoMip( "icons/iconf_red1" );
		cgs.media.redFlagShader[1] = trap_R_RegisterShaderNoMip( "icons/iconf_red2" );
		cgs.media.redFlagShader[2] = trap_R_RegisterShaderNoMip( "icons/iconf_red3" );
		cgs.media.blueFlagShader[0] = trap_R_RegisterShaderNoMip( "icons/iconf_blu1" );
		cgs.media.blueFlagShader[1] = trap_R_RegisterShaderNoMip( "icons/iconf_blu2" );
		cgs.media.blueFlagShader[2] = trap_R_RegisterShaderNoMip( "icons/iconf_blu3" );
		cgs.media.flagPoleModel = trap_R_RegisterModel( "models/flag2/flagpole.md3" );
		cgs.media.flagFlapModel = trap_R_RegisterModel( "models/flag2/flagflap3.md3" );

		cgs.media.redFlagFlapSkin = trap_R_RegisterSkin( "models/flag2/red.skin" );
		cgs.media.blueFlagFlapSkin = trap_R_RegisterSkin( "models/flag2/blue.skin" );
		cgs.media.neutralFlagFlapSkin = trap_R_RegisterSkin( "models/flag2/white.skin" );

		cgs.media.redFlagBaseModel = trap_R_RegisterModel( "models/mapobjects/flagbase/red_base.md3" );
		cgs.media.blueFlagBaseModel = trap_R_RegisterModel( "models/mapobjects/flagbase/blue_base.md3" );
		cgs.media.neutralFlagBaseModel = trap_R_RegisterModel( "models/mapobjects/flagbase/ntrl_base.md3" );
	}

	if ( cgs.gametype == GT_1FCTF ) {
		cgs.media.neutralFlagModel = trap_R_RegisterModel( "models/flags/n_flag.md3" );
		cgs.media.flagShader[0] = trap_R_RegisterShaderNoMip( "icons/iconf_neutral1" );
		cgs.media.flagShader[1] = trap_R_RegisterShaderNoMip( "icons/iconf_red2" );
		cgs.media.flagShader[2] = trap_R_RegisterShaderNoMip( "icons/iconf_blu2" );
		cgs.media.flagShader[3] = trap_R_RegisterShaderNoMip( "icons/iconf_neutral3" );
	}

	if ( cgs.gametype == GT_OBELISK ) {
		cgs.media.rocketExplosionShader = trap_R_RegisterShader("rocketExplosion");
		cgs.media.overloadBaseModel = trap_R_RegisterModel( "models/powerups/overload_base.md3" );
		cgs.media.overloadTargetModel = trap_R_RegisterModel( "models/powerups/overload_target.md3" );
		cgs.media.overloadLightsModel = trap_R_RegisterModel( "models/powerups/overload_lights.md3" );
		cgs.media.overloadEnergyModel = trap_R_RegisterModel( "models/powerups/overload_energy.md3" );
	}

	if ( cgs.gametype == GT_HARVESTER ) {
		cgs.media.harvesterModel = trap_R_RegisterModel( "models/powerups/harvester/harvester.md3" );
		cgs.media.harvesterRedSkin = trap_R_RegisterSkin( "models/powerups/harvester/red.skin" );
		cgs.media.harvesterBlueSkin = trap_R_RegisterSkin( "models/powerups/harvester/blue.skin" );
		cgs.media.harvesterNeutralModel = trap_R_RegisterModel( "models/powerups/obelisk/obelisk.md3" );
	}

	cgs.media.redKamikazeShader = trap_R_RegisterShader( "models/weaphits/kamikred" );
	cgs.media.dustPuffShader = trap_R_RegisterShader("hasteSmokePuff" );

	if ( cgs.gametype >= GT_TEAM && cgs.ffa_gt != 1 ) {
		cgs.media.friendShader = trap_R_RegisterShader( "sprites/foe3" );
		cgs.media.redQuadShader = trap_R_RegisterShader("powerups/blueflag" );
		cgs.media.blueKamikazeShader = trap_R_RegisterShader( "models/weaphits/kamikblu" );
	}
	cgs.media.teamStatusBar = trap_R_RegisterShader( "gfx/2d/colorbar.tga" );

	cgs.media.armorModel = trap_R_RegisterModel( "models/powerups/armor/armor_yel.md3" );
	cgs.media.armorIcon  = trap_R_RegisterShaderNoMip( "icons/iconr_yellow" );

	cgs.media.machinegunBrassModel = trap_R_RegisterModel( "models/weapons2/shells/m_shell.md3" );
	cgs.media.shotgunBrassModel = trap_R_RegisterModel( "models/weapons2/shells/s_shell.md3" );

	cgs.media.gibAbdomen = trap_R_RegisterModel( "models/gibs/abdomen.md3" );
	cgs.media.gibArm = trap_R_RegisterModel( "models/gibs/arm.md3" );
	cgs.media.gibChest = trap_R_RegisterModel( "models/gibs/chest.md3" );
	cgs.media.gibFist = trap_R_RegisterModel( "models/gibs/fist.md3" );
	cgs.media.gibFoot = trap_R_RegisterModel( "models/gibs/foot.md3" );
	cgs.media.gibForearm = trap_R_RegisterModel( "models/gibs/forearm.md3" );
	cgs.media.gibIntestine = trap_R_RegisterModel( "models/gibs/intestine.md3" );
	cgs.media.gibLeg = trap_R_RegisterModel( "models/gibs/leg.md3" );
	cgs.media.gibSkull = trap_R_RegisterModel( "models/gibs/skull.md3" );
	cgs.media.gibBrain = trap_R_RegisterModel( "models/gibs/brain.md3" );

	cgs.media.debrislight1 = trap_R_RegisterModel( "models/debris/concrete_b1.md3" );
	cgs.media.debrislight2 = trap_R_RegisterModel( "models/debris/concrete_b2.md3" );
	cgs.media.debrislight3 = trap_R_RegisterModel( "models/debris/concrete_b3.md3" );
	cgs.media.debrislight4 = trap_R_RegisterModel( "models/debris/concrete_b4.md3" );
	cgs.media.debrislight5 = trap_R_RegisterModel( "models/debris/concrete_b5.md3" );
	cgs.media.debrislight6 = trap_R_RegisterModel( "models/debris/concrete_b6.md3" );
	cgs.media.debrislight7 = trap_R_RegisterModel( "models/debris/concrete_b7.md3" );
	cgs.media.debrislight8 = trap_R_RegisterModel( "models/debris/concrete_b8.md3" );

	cgs.media.debrisdark1 = trap_R_RegisterModel( "models/debris/concrete_d1.md3" );
	cgs.media.debrisdark2 = trap_R_RegisterModel( "models/debris/concrete_d2.md3" );
	cgs.media.debrisdark3 = trap_R_RegisterModel( "models/debris/concrete_d3.md3" );
	cgs.media.debrisdark4 = trap_R_RegisterModel( "models/debris/concrete_d4.md3" );
	cgs.media.debrisdark5 = trap_R_RegisterModel( "models/debris/concrete_d5.md3" );
	cgs.media.debrisdark6 = trap_R_RegisterModel( "models/debris/concrete_d6.md3" );
	cgs.media.debrisdark7 = trap_R_RegisterModel( "models/debris/concrete_d7.md3" );
	cgs.media.debrisdark8 = trap_R_RegisterModel( "models/debris/concrete_d8.md3" );

	cgs.media.debrislightlarge1 = trap_R_RegisterModel( "models/debris/concrete_b1_large.md3" );
	cgs.media.debrislightlarge2 = trap_R_RegisterModel( "models/debris/concrete_b2_large.md3" );
	cgs.media.debrislightlarge3 = trap_R_RegisterModel( "models/debris/concrete_b3_large.md3" );

	cgs.media.debrisdarklarge1 = trap_R_RegisterModel( "models/debris/wood_b1.md3" );
	cgs.media.debrisdarklarge2 = trap_R_RegisterModel( "models/debris/wood_b1.md3" );
	cgs.media.debrisdarklarge3 = trap_R_RegisterModel( "models/debris/wood_b1.md3" );

	cgs.media.debriswood1 = trap_R_RegisterModel( "models/debris/wood_b1.md3" );
	cgs.media.debriswood2 = trap_R_RegisterModel( "models/debris/wood_b2.md3" );
	cgs.media.debriswood3 = trap_R_RegisterModel( "models/debris/wood_b3.md3" );
	cgs.media.debriswood4 = trap_R_RegisterModel( "models/debris/wood_b4.md3" );
	cgs.media.debriswood5 = trap_R_RegisterModel( "models/debris/wood_b5.md3" );

	cgs.media.debrisglass1 = trap_R_RegisterModel( "models/debris/glass_1.md3" );
	cgs.media.debrisglass2 = trap_R_RegisterModel( "models/debris/glass_2.md3" );
	cgs.media.debrisglass3 = trap_R_RegisterModel( "models/debris/glass_3.md3" );
	cgs.media.debrisglass4 = trap_R_RegisterModel( "models/debris/glass_4.md3" );
	cgs.media.debrisglass5 = trap_R_RegisterModel( "models/debris/glass_5.md3" );

	cgs.media.debrisglasslarge1 = trap_R_RegisterModel( "models/debris/glass_1_large.md3" );
	cgs.media.debrisglasslarge2 = trap_R_RegisterModel( "models/debris/glass_2_large.md3" );
	cgs.media.debrisglasslarge3 = trap_R_RegisterModel( "models/debris/glass_3_large.md3" );
	cgs.media.debrisglasslarge4 = trap_R_RegisterModel( "models/debris/glass_4_large.md3" );
	cgs.media.debrisglasslarge5 = trap_R_RegisterModel( "models/debris/glass_5_large.md3" );
	
	cgs.media.debrisstone1 = trap_R_RegisterModel( "models/debris/stone_1.md3" );
	cgs.media.debrisstone2 = trap_R_RegisterModel( "models/debris/stone_2.md3" );
	cgs.media.debrisstone3 = trap_R_RegisterModel( "models/debris/stone_3.md3" );
	cgs.media.debrisstone4 = trap_R_RegisterModel( "models/debris/stone_4.md3" );
	cgs.media.debrisstone5 = trap_R_RegisterModel( "models/debris/stone_5.md3" );

	cgs.media.sparkShader = trap_R_RegisterShaderNoMip("spark");
	
	cgs.media.smoke2 = trap_R_RegisterModel( "models/weapons2/shells/s_shell.md3" );

	cgs.media.balloonShader = trap_R_RegisterShader( "sprites/balloon3" );

	cgs.media.bloodExplosionShader = trap_R_RegisterShader( "bloodExplosion" );

	cgs.media.bulletFlashModel = trap_R_RegisterModel("models/weaphits/bullet.md3");
	cgs.media.ringFlashModel = trap_R_RegisterModel("models/weaphits/ring02.md3");
	cgs.media.dishFlashModel = trap_R_RegisterModel("models/weaphits/boom01.md3");
	cgs.media.teleportEffectModel = trap_R_RegisterModel( "models/powerups/pop.md3" );
	cgs.media.kamikazeEffectModel = trap_R_RegisterModel( "models/weaphits/kamboom2.md3" );
	cgs.media.kamikazeShockWave = trap_R_RegisterModel( "models/weaphits/kamwave.md3" );
	cgs.media.kamikazeHeadModel = trap_R_RegisterModel( "models/powerups/kamikazi.md3" );
	cgs.media.kamikazeHeadTrail = trap_R_RegisterModel( "models/powerups/trailtest.md3" );
	cgs.media.guardPowerupModel = trap_R_RegisterModel( "models/powerups/guard_player.md3" );
	cgs.media.scoutPowerupModel = trap_R_RegisterModel( "models/powerups/scout_player.md3" );
	cgs.media.doublerPowerupModel = trap_R_RegisterModel( "models/powerups/doubler_player.md3" );
	cgs.media.ammoRegenPowerupModel = trap_R_RegisterModel( "models/powerups/ammo_player.md3" );
	cgs.media.invulnerabilityImpactModel = trap_R_RegisterModel( "models/powerups/shield/impact.md3" );
	cgs.media.invulnerabilityJuicedModel = trap_R_RegisterModel( "models/powerups/shield/juicer.md3" );
	cgs.media.medkitUsageModel = trap_R_RegisterModel( "models/powerups/regen.md3" );
	cgs.media.heartShader = trap_R_RegisterShaderNoMip( "ui/assets/statusbar/selectedhealth.tga" );

	cgs.media.invulnerabilityPowerupModel = trap_R_RegisterModel( "models/powerups/shield/shield.md3" );
	cgs.media.medalImpressive = trap_R_RegisterShaderNoMip( "medal_impressive" );
	cgs.media.medalExcellent = trap_R_RegisterShaderNoMip( "medal_excellent" );
	cgs.media.medalGauntlet = trap_R_RegisterShaderNoMip( "medal_gauntlet" );
	cgs.media.medalDefend = trap_R_RegisterShaderNoMip( "medal_defend" );
	cgs.media.medalAssist = trap_R_RegisterShaderNoMip( "medal_assist" );
	cgs.media.medalCapture = trap_R_RegisterShaderNoMip( "medal_capture" );

	// LEILEI SHADERS
	cgs.media.lsmkShader1 = trap_R_RegisterShader("leismoke1" );
	cgs.media.lsmkShader2 = trap_R_RegisterShader("leismoke2" );
	cgs.media.lsmkShader3 = trap_R_RegisterShader("leismoke3" );
	cgs.media.lsmkShader4 = trap_R_RegisterShader("leismoke4" );

	cgs.media.lsplShader = trap_R_RegisterShader("leisplash" );

	//OpenSandbox UI
	cgs.media.errIcon = trap_R_RegisterShaderNoMip("menu/erricon");
    cgs.media.notifyIcon = trap_R_RegisterShaderNoMip("menu/notifyicon");
    cgs.media.undoIcon = trap_R_RegisterShaderNoMip("menu/undoicon");


	memset( cg_items, 0, sizeof( cg_items ) );
	memset( cg_weapons, 0, sizeof( cg_weapons ) );

	// only register the items that the server says we need
	Q_strncpyz(items, CG_ConfigString(CS_ITEMS), sizeof(items));

	for ( i = 1 ; i < bg_numItems ; i++ ) {
		if ( items[ i ] == '1' || 1 ) {
			CG_LoadingItem( i );
			CG_RegisterItemVisuals( i );
		}
	}

	// wall marks
	cgs.media.bulletMarkShader = trap_R_RegisterShader( "gfx/damage/bullet_mrk" );
	cgs.media.burnMarkShader = trap_R_RegisterShader( "gfx/damage/burn_med_mrk" );
	cgs.media.holeMarkShader = trap_R_RegisterShader( "gfx/damage/hole_lg_mrk" );
	cgs.media.energyMarkShader = trap_R_RegisterShader( "gfx/damage/plasma_mrk" );
	cgs.media.shadowMarkShader = trap_R_RegisterShader( "markShadow" );
	cgs.media.wakeMarkShader = trap_R_RegisterShader( "wake" );
	cgs.media.bloodMarkShader = trap_R_RegisterShader( "bloodMark" );

	// paintball mode marks
	cgs.media.bulletMarkPaintShader = trap_R_RegisterShader( "gfx/damage/bullet_mrk_paint" );
	cgs.media.burnMarkPaintShader = trap_R_RegisterShader( "gfx/damage/burn_med_mrk_paint" );
	cgs.media.holeMarkPaintShader = trap_R_RegisterShader( "gfx/damage/hole_lg_mrk_paint" );
	cgs.media.energyMarkPaintShader = trap_R_RegisterShader( "gfx/damage/plasma_mrk_paint" );

	//explosion effect
	cgs.media.rocketExplosionShader = trap_R_RegisterShader("rocketExplosion");

	// register the inline models
	cgs.numInlineModels = trap_CM_NumInlineModels();
	for ( i = 1 ; i < cgs.numInlineModels ; i++ ) {
		char	name[10];
		vec3_t			mins, maxs;
		int				j;

		Com_sprintf( name, sizeof(name), "*%i", i );
		cgs.inlineDrawModel[i] = trap_R_RegisterModel( name );
		trap_R_ModelBounds( cgs.inlineDrawModel[i], mins, maxs );
		for ( j = 0 ; j < 3 ; j++ ) {
			cgs.inlineModelMidpoints[i][j] = mins[j] + 0.5 * ( maxs[j] - mins[j] );
		}
	}

	// register all the server specified models
	for (i=1 ; i<MAX_MODELS ; i++) {
		const char		*modelName;

		modelName = CG_ConfigString( CS_MODELS+i );
		if ( !modelName[0] ) {
			break;
		}
		cgs.gameModels[i] = trap_R_RegisterModel( modelName );
	}

	cgs.media.railCoreShader = trap_R_RegisterShader("railCore");
}

/*																																			
=======================
CG_BuildSpectatorString

=======================
*/
void CG_BuildSpectatorString(void) {
	int i;
	cg.spectatorList[0] = 0;
	for (i = 0; i < MAX_CLIENTS; i++) {
		if (cgs.clientinfo[i].infoValid && cgs.clientinfo[i].team == TEAM_SPECTATOR ) {
			Q_strcat(cg.spectatorList, sizeof(cg.spectatorList), va("%s     ", cgs.clientinfo[i].name));
		}
	}
	i = strlen(cg.spectatorList);
	if (i != cg.spectatorLen) {
		cg.spectatorLen = i;
		cg.spectatorWidth = -1;
	}
}


/*																																			
===================
CG_RegisterClients
===================
*/
static void CG_RegisterClients( void ) {
	int		i;

	CG_LoadingClient(cg.clientNum);
	CG_NewClientInfo(cg.clientNum);

	for (i=0 ; i<MAX_CLIENTS ; i++) {
		const char		*clientInfo;

		if (cg.clientNum == i) {
			continue;
		}

		clientInfo = CG_ConfigString( CS_PLAYERS+i );
		if ( !clientInfo[0]) {
			continue;
		}
		CG_LoadingClient( i );
		CG_NewClientInfo( i );
	}
	CG_BuildSpectatorString();
}

/*
===================
CG_ImportModelsOBJ
===================
*/
static void CG_ImportModelsOBJ(void) {
    char dirlist[32000];
    int numfiles = trap_FS_GetFileList("props", ".obj", dirlist, sizeof(dirlist));
	int i;
	char* file;

    file = dirlist;
    for (i = 0; i < numfiles; i++) {
        char nameWithoutExt[128];
        Q_snprintf(nameWithoutExt, sizeof(nameWithoutExt), "props/%.*s", (int)(strlen(file) - 4), file);
        trap_ImportOBJ(nameWithoutExt);
		CG_LoadingString( va("import OBJ: %s", nameWithoutExt), 0.30 );
        file += strlen(file) + 1;
    }
}

/*
=================
CG_ConfigString
=================
*/
const char *CG_ConfigString( int index ) {
	if ( index < 0 || index >= MAX_CONFIGSTRINGS ) {
		CG_Error( "CG_ConfigString: bad index: %i", index );
	}
	return cgs.gameState.stringData + cgs.gameState.stringOffsets[ index ];
}

/*
======================
CG_StartMusic
======================
*/
void CG_StartMusic( void ) {
	char	*s;
	char	parm1[MAX_QPATH], parm2[MAX_QPATH];

	// start the background music
	if ( *cg_music.string && Q_stricmp( cg_music.string, "none" ) ) {
		s = (char *)cg_music.string;
	} else {
		s = (char *)CG_ConfigString( CS_MUSIC );
	Q_strncpyz( parm1, COM_Parse( &s ), sizeof( parm1 ) );
	Q_strncpyz( parm2, COM_Parse( &s ), sizeof( parm2 ) );

	trap_S_StartBackgroundTrack( parm1, parm2 );
        }
}

void CG_StartScoreboardMusic( void ) {
	char	var[MAX_TOKEN_CHARS];
	char	*s;
	char	parm1[MAX_QPATH], parm2[MAX_QPATH];

	//if music volume is 0, don't start the scoreboard music
	trap_Cvar_VariableStringBuffer( "s_musicvolume", var, sizeof( var ) );
	if ( !strcmp(var, "0") )
		return;

	// start the background music
	s = (char *)CG_ConfigString( CS_SCOREBOARDMUSIC );
	Q_strncpyz( parm1, COM_Parse( &s ), sizeof( parm1 ) );
	Q_strncpyz( parm2, COM_Parse( &s ), sizeof( parm2 ) );

	trap_S_StartBackgroundTrack( parm1, parm2 );
}

void CG_StartDeathMusic( void ) {
	char	var[MAX_TOKEN_CHARS];
	char	*s;
	char	parm1[MAX_QPATH], parm2[MAX_QPATH];

	//if music volume is 0, don't start the scoreboard music
	trap_Cvar_VariableStringBuffer( "s_musicvolume", var, sizeof( var ) );
	if ( !strcmp(var, "0") )
		return;

	// start the background music
	s = (char *)CG_ConfigString( CS_DEATHMUSIC );
	Q_strncpyz( parm1, COM_Parse( &s ), sizeof( parm1 ) );
	Q_strncpyz( parm2, COM_Parse( &s ), sizeof( parm2 ) );

	trap_S_StartBackgroundTrack( parm1, parm2 );
	cg.deathmusicStarted = qtrue;
}

void CG_StopDeathMusic( void ) {
	trap_S_StartBackgroundTrack( "", "" );
	cg.deathmusicStarted = qfalse;
	CG_StartMusic();
}

/*
=================
CG_Init

Called after every level change or subsystem restart
Will perform callbacks to make the loading info screen update.
=================
*/
void CG_Init( int serverMessageNum, int serverCommandSequence, int clientNum ) {
	const char	*s;
	float resbias, resbiasy;
	float rex, rey;
	int newresx, newresy;

	// init cvars and commands
	ST_RegisterCvars();
	CG_CreateCvars();
	CG_InitConsoleCommands();

	// cache glconfig
	trap_GetGlconfig( &glconfig );

	// clear state
	memset( &cgs, 0, sizeof( cgs ) );
	memset( &cg, 0, sizeof( cg ) );
	memset( cg_entities, 0, sizeof(cg_entities) );
	memset( cg_weapons, 0, sizeof(cg_weapons) );
	memset( cg_items, 0, sizeof(cg_items) );

	CG_LoadingString( "game core", 0.10 );

	cg.clientNum = clientNum;
	cgs.processedSnapshotNum = serverMessageNum;
	cgs.serverCommandSequence = serverCommandSequence;

	// load a few needed things before we do any screen updates
	ST_RegisterFont("default");
	cgs.media.defaultFont[0]		= trap_R_RegisterShader( "default_font" );  //256
	cgs.media.defaultFont[1]		= trap_R_RegisterShader( "default_font1" ); //512
	cgs.media.defaultFont[2]		= trap_R_RegisterShader( "default_font2" ); //1024
	cgs.media.whiteShader		= trap_R_RegisterShader( "white" );
	cgs.media.corner          	= trap_R_RegisterShader( "menu/corner" );

	CG_LoadingString( "noire.script", 0.20 );

	trap_Cvar_Set("ns_haveerror", "0");
	NS_OpenScript("nscript/cgame/init.ns", NULL, 0);		//Noire.Script Init in cgame.qvm

	cgs.redflag = cgs.blueflag = -1; // For compatibily, default to unset for
	cgs.flagStatus = -1;

	// setup screen
	cgs.scale = (glconfig.vidWidth * (1.0 / 640.0) < glconfig.vidHeight * (1.0 / 480.0)) ? glconfig.vidWidth * (1.0 / 640.0) : glconfig.vidHeight * (1.0 / 480.0);

	realVidWidth = glconfig.vidWidth;
	realVidHeight = glconfig.vidHeight;

	rex = 640.0f / realVidWidth;
	rey = 480.0f / realVidHeight;
	newresx = 640.0f * (rex);
	newresy = 480.0f * (rey);
	newresx = realVidWidth * rey;
	newresy = realVidHeight * rey;
	resbias  = 0.5 * ( newresx -  ( newresy * (640.0/480.0) ) );
	resbiasy = 0.5 * ( newresy -  ( newresx * (640.0/480.0) ) );

	if ( glconfig.vidWidth * 480 > glconfig.vidHeight * 640 ) {
		// wide screen
		cgs.bias = 0.5 * ( glconfig.vidWidth - ( glconfig.vidHeight * (640.0/480.0) ) );
	}
	else {
		// no wide screen
		cgs.bias = 0;
	}

	// get the gamestate from the client system
	trap_GetGameState( &cgs.gameState );

	// check version
	s = CG_ConfigString( CS_GAME_VERSION );

	s = CG_ConfigString( CS_LEVEL_START_TIME );
	cgs.levelStartTime = atoi( s );

	CG_ImportModelsOBJ();

	CG_ParseServerinfo();

	// load the new map
	CG_LoadingString( "collision map", 0.40 );

	trap_CM_LoadMap( cgs.mapname );

	cg.loading = qtrue;

	CG_LoadingString( "sounds", 0.50 );

	CG_RegisterSounds();

	CG_LoadingString( "graphics", 0.60 );

	CG_RegisterGraphics();

	CG_LoadingString( "clients", 0.80 );

	CG_RegisterClients();		// if low on memory, some clients will be deferred

	cg.loading = qfalse;	// future players will be deferred

	CG_LoadingString( "entities", 0.90 );

	CG_InitLocalEntities();

	CG_InitMarkPolys();

	// Make sure we have update values (scores)
	CG_SetConfigValues();

	CG_LoadingString( "Loaded!", 1.00 );
	
	CG_StartMusic();

	CG_LoadingString( "", 0.0 );

	CG_ShaderStateChanged();

	trap_S_ClearLoopingSounds( qtrue );

	// remove the last loading update
	cg.infoScreenText[0] = 0;
	cg.infoScreenValue = 0.0;
	
	trap_SendConsoleCommand("ns_openscript_ui tools/create.ns\n");
	trap_SendConsoleCommand( va("weapon %i\n", WP_TOOLGUN) );
}

/*
=================
CG_Shutdown

Called before every level change or subsystem restart
=================
*/
void CG_Shutdown( void ) {
	// some mods may need to do cleanup work here,
	// like closing files or archiving session data
}

/*
======================
SnapVectorTowards

Round a vector to integers for more efficient network
transmission, but make sure that it rounds towards a given point
rather than blindly truncating.  This prevents it from truncating
into a wall.
======================
*/
void SnapVectorTowards( vec3_t v, vec3_t to ) {
	int		i;

	for ( i = 0 ; i < 3 ; i++ ) {
		if ( to[i] <= v[i] ) {
			v[i] = (int)v[i];
		} else {
			v[i] = (int)v[i] + 1;
		}
	}
}
