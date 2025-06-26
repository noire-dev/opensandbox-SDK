// Copyright (C) 1999-2005 ID Software, Inc.
// Copyright (C) 2023-2025 Noire.dev
// Copyright (C) 2025 OpenSandbox Team
// OpenSandbox — GPLv2; see LICENSE for details.

#include "cg_local.h"

#define	SCOREBOARD_X		(0-cgs.wideoffset)

#define SB_HEADER			18
#define SB_TOP				(SB_HEADER+12)

#define SB_INTER_HEIGHT		16 // interleaved height

// Normal
#define SB_BOTICON_X		(SCOREBOARD_X+4)
#define SB_HEAD_X			(SCOREBOARD_X+20)

#define SB_SCORELINE_X		28-cgs.wideoffset

#define SB_RATING_WIDTH	    (6 * BIGCHAR_WIDTH)
#define SB_SCORE_X			(SB_SCORELINE_X + 2 * BIGCHAR_WIDTH)
#define SB_PING_X			(SB_SCORELINE_X + 8 * BIGCHAR_WIDTH)
#define SB_TIME_X			(SB_SCORELINE_X + 12 * BIGCHAR_WIDTH)
#define SB_NAME_X			(SB_SCORELINE_X + 18 * BIGCHAR_WIDTH)

static qboolean localClient; // true if local client has been displayed
/*
=================
CG_DrawScoreboard
=================
*/
static void CG_DrawClientScore( int y, score_t *score, float fade ) {
	char	string[1024];
	clientInfo_t	*ci;
	int iconx, headx;

	if (score->client < 0 || score->client >= cgs.maxclients) {
		Com_Printf( "Bad score->client: %i\n", score->client );
		return;
	}

	ci = &cgs.clientinfo[score->client];

	iconx = SB_BOTICON_X + (SB_RATING_WIDTH / 2);
	headx = SB_HEAD_X + (SB_RATING_WIDTH / 2);

	if ( ci->powerups & ( 1 << PW_NEUTRALFLAG ) ) {
		CG_DrawFlagModel( iconx-16, y, 16, 16, TEAM_FREE );
	} else if ( ci->powerups & ( 1 << PW_REDFLAG ) ) {
		CG_DrawFlagModel( iconx-16, y, 16, 16, TEAM_RED );
	} else if ( ci->powerups & ( 1 << PW_BLUEFLAG ) ) {
		CG_DrawFlagModel( iconx-16, y, 16, 16, TEAM_BLUE );
	}

	CG_DrawHead( headx, y, 16, 16, score->client );
	
	// draw the score line
	if ( score->ping == -1 ) {
		Com_sprintf(string, sizeof(string), " connecting    %s", ci->name);
	} else if ( ci->team == TEAM_SPECTATOR ) {
		Com_sprintf(string, sizeof(string), " SPECT %3i %4i %s", score->ping, score->time, ci->name);
	} else {
		Com_sprintf(string, sizeof(string), "%5i %4i %4i %s", score->score, score->ping, score->time, ci->name);
	}

	// highlight your position
	if ( score->client == cg.snap->ps.clientNum ) {
		float	hcolor[4];
		int		rank;

		localClient = qtrue;

		if (cg.snap->ps.persistant[PERS_TEAM] == TEAM_SPECTATOR || cgs.gametype >= GT_TEAM) {
			rank = -1;
		} else {
			rank = cg.snap->ps.persistant[PERS_RANK] & ~RANK_TIED_FLAG;
		}
		hcolor[0] = cg_crosshairColorRed.value;
		hcolor[1] = cg_crosshairColorGreen.value;
		hcolor[2] = cg_crosshairColorBlue.value;
		hcolor[3] = fade * 0.5;

		CG_FillRect( 90 - cgs.wideoffset, y, 550 + (cgs.wideoffset*2), BIGCHAR_HEIGHT+1, hcolor );
	}

	ST_DrawString( SB_SCORELINE_X + (SB_RATING_WIDTH / 2), y, string, UI_LEFT, color_white, 1.50);
}

/*
=================
CG_TeamScoreboard
=================
*/
static int CG_TeamScoreboard( int y, team_t team, float fade, int maxClients, int lineHeight ) {
	int		i;
	score_t	*score;
	int		count;
	clientInfo_t	*ci;

	count = 0;
	for ( i = 0 ; i < cg.numScores && count < maxClients ; i++ ) {
		score = &cg.scores[i];
		ci = &cgs.clientinfo[ score->client ];

		if ( team != ci->team ) {
			continue;
		}

		if ( score->ping == 0 ){ continue; }	// No draw with 0 ping

		CG_DrawClientScore( y + lineHeight * count, score, fade );

		count++;
	}

	return count;
}

/*
=================
CG_DrawScoreboard

Draw the normal in-game scoreboard
=================
*/
qboolean CG_DrawScoreboard( void ) {
	int		x, y, w, n1, n2;
	float	fade;
	float	*fadeColor;
	char	*s;
	int maxClients;
	int lineHeight;
	int topBorderSize, bottomBorderSize;
	vec4_t         colorblk;

	// don't draw amuthing if the menu or console is up
	if ( cg_paused.integer ) {
		cg.deferredPlayerLoading = 0;
		return qfalse;
	}

	if ( !cg.showScores ) {
		return qfalse;
	}

	if ( cg.showScores || cg.predictedPlayerState.pm_type == PM_DEAD ||
		 cg.predictedPlayerState.pm_type == PM_INTERMISSION ) {
		fade = 1.0;
		fadeColor = colorWhite;
	} else {
		fadeColor = CG_FadeColor( cg.scoreFadeTime, 0 );

		if ( !fadeColor ) {
			// next time scoreboard comes up, don't print killer
			cg.deferredPlayerLoading = 0;
			cg.killerName[0] = 0;
			return qfalse;
		}
		fade = *fadeColor;
	}

	colorblk[0]=0.0f;
	colorblk[1]=0.0f;
	colorblk[2]=0.0f;
	colorblk[3]=0.75f;
	
	CG_DrawPic( -1 - cgs.wideoffset, 0, 642+(cgs.wideoffset*2), 480, trap_R_RegisterShaderNoMip( "menu/assets/blacktrans" ) );

	// fragged by ... line
	if ( cg.killerName[0] ) {
		s = va("Fragged by %s", cg.killerName );
		w = CG_DrawStrlen( s ) * BIGCHAR_WIDTH;
		x = ( SCREEN_WIDTH - w ) / 2;
		y = 2;
		CG_DrawBigString( x, y, s, fade );
	}

	// scoreboard
	y = SB_HEADER;

	CG_DrawSmallString( SB_SCORE_X + (SB_RATING_WIDTH / 2), y, "Score", fade );
	CG_DrawSmallString( SB_PING_X + (SB_RATING_WIDTH / 2), y, "Ping", fade );
	CG_DrawSmallString( SB_TIME_X + (SB_RATING_WIDTH / 2), y, "Time", fade );
	CG_DrawSmallString( SB_NAME_X + (SB_RATING_WIDTH / 2), y, "Name", fade );

	y = SB_TOP;

	maxClients = 24;
	lineHeight = SB_INTER_HEIGHT;
	topBorderSize = 8;
	bottomBorderSize = 16;

	localClient = qfalse;

	if ( cgs.gametype >= GT_TEAM ) {
		//
		// teamplay scoreboard
		//
		y += lineHeight/2;

		if ( cg.teamScores[0] >= cg.teamScores[1] ) {
			n1 = CG_TeamScoreboard( y, TEAM_RED, fade, maxClients, lineHeight );
			y += (n1 * lineHeight) + BIGCHAR_HEIGHT;
			maxClients -= n1;
			n2 = CG_TeamScoreboard( y, TEAM_BLUE, fade, maxClients, lineHeight );
			y += (n2 * lineHeight) + BIGCHAR_HEIGHT;
			maxClients -= n2;
		} else {
			n1 = CG_TeamScoreboard( y, TEAM_BLUE, fade, maxClients, lineHeight );
			y += (n1 * lineHeight) + BIGCHAR_HEIGHT;
			maxClients -= n1;
			n2 = CG_TeamScoreboard( y, TEAM_RED, fade, maxClients, lineHeight );
			y += (n2 * lineHeight) + BIGCHAR_HEIGHT;
			maxClients -= n2;
		}
		n1 = CG_TeamScoreboard( y, TEAM_SPECTATOR, fade, maxClients, lineHeight );
		y += (n1 * lineHeight) + BIGCHAR_HEIGHT;

	} else {
		//
		// free for all scoreboard
		//
		n1 = CG_TeamScoreboard( y, TEAM_FREE, fade, maxClients, lineHeight );
		y += (n1 * lineHeight) + BIGCHAR_HEIGHT;
		n2 = CG_TeamScoreboard( y, TEAM_SPECTATOR, fade, maxClients - n1, lineHeight );
		y += (n2 * lineHeight) + BIGCHAR_HEIGHT;
	}

	return qtrue;
}
