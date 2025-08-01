// Copyright (C) 1999-2005 ID Software, Inc.
// Copyright (C) 2023-2025 Noire.dev
// Copyright (C) 2025 OpenSandbox Team
// OpenSandbox — GPLv2; see LICENSE for details.

#include "ui_local.h"

#define MODEL_ARROWSL "menu/assets/arrow-left"
#define MODEL_ARROWSR "menu/assets/arrow-right"
#define MODEL_ARROWSB "menu/assets/arrow-down"
#define MODEL_ARROWST "menu/assets/arrow-up"

#define MODELDIR "models/players"

// common string names for all dm/team model changing
const char* drawmodel_list[] = {
	"DM",    // DRAWMODEL_DM
    "Team",  // DRAWMODEL_TEAM
    0
};

#define PLAYERGRID_COLS 5
#define PLAYERGRID_ROWS 4
#define MAX_MODELSPERPAGE (PLAYERGRID_ROWS * PLAYERGRID_COLS)
#define MAX_SKINSPERPAGE 10

#define PLAYERMODEL_X 385
#define PLAYERMODEL_Y 32
#define PLAYERMODEL_WIDTH 32 * 8
#define PLAYERMODEL_HEIGHT 56 * 8

#define MAX_PLAYERMODELS 8192
#define MAX_PLAYERSKINS 4096

#define MODELARRAY_X 68
#define MODELARRAY_Y 140
#define PICTURE_GAP 8
#define ICON_SIZE 48

#define MODELARROWS_X 16
#define MODELARROWS_Y 220
#define SKINARROWS_X 180

// each id starts a list of controls
#define ID_PLAYERPIC0 0
#define ID_SKINPIC0 32

// the rest of the controls
#define ID_PREVPAGE 100
#define ID_NEXTPAGE 101
#define ID_PREVSKINPAGE 102
#define ID_NEXTSKINPAGE 103
#define ID_MODELCHANGE 104
#define ID_TEAMMODEL 105
#define ID_FORCEHEADMATCH 106

#define BODYPART_COLUMN (128 + 64 + 128 + 32 + 8)
#define TEAMANDSFX_COLUMN (128 + 64)

// defines used for drawing player model names (OR flags)
#define MODELNAME_ALLBOLD 0
#define MODELNAME_HEADWEAK 1
#define MODELNAME_BODYWEAK 2
#define MODELNAME_LEGSWEAK 4

#define PLAYERMODELNAME_X 497
#define PLAYERMODELNAME_Y 64

typedef struct {
	menuframework_s menu;
	menuelement_s e[OSUI_MAX_ELEMENTS];
	menuelement_s models[MAX_MODELSPERPAGE];
	menuelement_s skins[MAX_SKINSPERPAGE];

	menuelement_s modelup;
	menuelement_s modeldown;
	menuelement_s skinleft;
	menuelement_s skinright;

	menuelement_s forceHeadMatch;
	menuelement_s modelChange;
	menuelement_s teamModel;

	int nummodels;
	char modelnames[MAX_PLAYERMODELS][MODELNAME_BUFFER];
	int numskins;
	char modelskins[MAX_PLAYERSKINS][MODELNAME_BUFFER];

	char other_head[MODELNAME_BUFFER];
	char prevother_head[MODELNAME_BUFFER];
	qboolean otherheadstate;

	int skinpage;
	int numskinpages;
	int selectedskin;

	int modelpage;
	int numpages;
	int selectedmodel;

	modelAnim_t model;
} playermodel_t;

static playermodel_t s_playermodel;

enum { MODELCHANGE_BODY, MODELCHANGE_HEAD, MODELCHANGE_LEGS };

static const char* model_partchange[] = {
	"Choose Body",  // MODELCHANGE_BODY
	"Choose Head",  // MODELCHANGE_HEAD
	"Choose Legs",  // MODELCHANGE_LEGS
	0
};

static const char* model_dmteam[] = {
	"DM model",    // DRAWMODEL_DM
    "Team model",  // DRAWMODEL_TEAM
    0
};

static const char* model_simplesplit[] = {
	"Split model",
    "Simple model",
    0
};

static void PlayerModel_UpdateGrid(void) {
	int i;
	int j;

	j = s_playermodel.modelpage * MAX_MODELSPERPAGE;
	for(i = 0; i < PLAYERGRID_ROWS * PLAYERGRID_COLS; i++, j++) {
		if(j < s_playermodel.nummodels) {
			// model/skin portrait
			s_playermodel.models[i].string = s_playermodel.modelnames[j];
			s_playermodel.models[i].generic.flags &= ~QMF_INACTIVE;
		} else {
			// dead slot
			s_playermodel.models[i].string = NULL;
			s_playermodel.models[i].generic.flags |= QMF_INACTIVE;
		}

		s_playermodel.models[i].generic.flags &= ~QMF_HIGHLIGHT;
		s_playermodel.models[i].shader = 0;
		s_playermodel.models[i].generic.flags |= QMF_PULSEIFFOCUS;
	}

	if(s_playermodel.selectedmodel == -1) return;

	if(s_playermodel.selectedmodel / MAX_MODELSPERPAGE == s_playermodel.modelpage) {
		// set selected model
		i = s_playermodel.selectedmodel % MAX_MODELSPERPAGE;
		s_playermodel.models[i].generic.flags |= QMF_HIGHLIGHT;
		s_playermodel.models[i].generic.flags &= ~QMF_PULSEIFFOCUS;
	}
}

static void PlayerModel_UpdateSkinGrid(void) {
	int i;
	int j;

	j = s_playermodel.skinpage * MAX_SKINSPERPAGE;
	for(i = 0; i < MAX_SKINSPERPAGE; i++, j++) {
		if(j < s_playermodel.numskins) {
			// portrait
			s_playermodel.skins[i].string = s_playermodel.modelskins[j];
			s_playermodel.skins[i].generic.flags &= ~QMF_INACTIVE;
		} else {
			s_playermodel.skins[i].string = NULL;
			s_playermodel.skins[i].generic.flags |= QMF_INACTIVE;
		}

		s_playermodel.skins[i].generic.flags &= ~QMF_HIGHLIGHT;
		s_playermodel.skins[i].shader = 0;
		s_playermodel.skins[i].generic.flags |= QMF_PULSEIFFOCUS;
	}

	if(s_playermodel.selectedskin == -1) return;

	if(s_playermodel.selectedskin / MAX_SKINSPERPAGE == s_playermodel.skinpage) {
		// set selected model
		i = s_playermodel.selectedskin % MAX_SKINSPERPAGE;
		s_playermodel.skins[i].generic.flags |= QMF_HIGHLIGHT;
		s_playermodel.skins[i].generic.flags &= ~QMF_PULSEIFFOCUS;
	}
}

char pm_dirlist[131072];
char pm_filelist[131072];
static void PlayerModel_BuildList(void) {
	int numdirs;
	int numfiles;
	char skinname[512];
	char* dirptr;
	char* fileptr;
	int i;
	int j;
	int dirlen;
	int filelen;
	char* defaultskin;

	s_playermodel.modelpage = 0;
	s_playermodel.nummodels = 0;

	// iterate directory of all player models
	numdirs = trap_FS_GetFileList(MODELDIR, "/", pm_dirlist, 131072);
	dirptr = pm_dirlist;
	for(i = 0; i < numdirs && s_playermodel.nummodels < MAX_PLAYERMODELS; i++, dirptr += dirlen + 1) {
		dirlen = strlen(dirptr);

		if(dirlen && dirptr[dirlen - 1] == '/') dirptr[dirlen - 1] = '\0';

		if(!strcmp(dirptr, ".") || !strcmp(dirptr, "..")) continue;

		// iterate all skin tga files in directory
		defaultskin = NULL;
		numfiles = trap_FS_GetFileList(va(MODELDIR "/%s", dirptr), "", pm_filelist, 131072);
		fileptr = pm_filelist;
		for(j = 0; j < numfiles && s_playermodel.nummodels < MAX_PLAYERMODELS; j++, fileptr += filelen + 1) {
			filelen = strlen(fileptr);

			COM_StripExtension(fileptr, skinname, sizeof(skinname));

			// look for icon_????
			if(!Q_stricmpn(skinname, "icon_", 5)) {
				if(!defaultskin) defaultskin = fileptr;

				if(!Q_stricmp(skinname, "icon_default")) defaultskin = fileptr;
			}
		}

		// save the default model/skin combo
		if(defaultskin) {
			COM_StripExtension(defaultskin, skinname, sizeof(skinname));
			Com_sprintf(s_playermodel.modelnames[s_playermodel.nummodels++], sizeof(s_playermodel.modelnames[s_playermodel.nummodels]), MODELDIR "/%s/%s", dirptr, skinname);
		}
	}

	s_playermodel.numpages = s_playermodel.nummodels / MAX_MODELSPERPAGE;
	if(s_playermodel.nummodels % MAX_MODELSPERPAGE) s_playermodel.numpages++;
}

static void PlayerModel_LoadSkins(int modelnum) {
	int numfiles;
	char* fullmodelname;
	char skinname[MODELNAME_BUFFER];
	char* fileptr;
	int i;
	int filelen;

	s_playermodel.skinpage = 0;
	s_playermodel.numskins = 0;

	fullmodelname = s_playermodel.modelnames[modelnum] + strlen(MODELDIR) + 1;

	// iterate all skin files in directory
	numfiles = trap_FS_GetFileList(va(MODELDIR "/%s", GUI_ModelName(fullmodelname)), "", pm_filelist, 131072);
	fileptr = pm_filelist;

	for(i = 0; i < numfiles && s_playermodel.numskins < MAX_PLAYERSKINS; i++, fileptr += filelen + 1) {
		filelen = strlen(fileptr);

		COM_StripExtension(fileptr, skinname, sizeof(skinname));

		// look for icon_????
		if(!Q_stricmpn(skinname, "icon_", 5)) {
			Com_sprintf(s_playermodel.modelskins[s_playermodel.numskins], sizeof(s_playermodel.modelskins[s_playermodel.numskins]), MODELDIR "/%s/%s", GUI_ModelName(fullmodelname), skinname);

			s_playermodel.numskins++;
		}
	}

	s_playermodel.numskinpages = s_playermodel.numskins / MAX_SKINSPERPAGE;
	if(s_playermodel.numskins % MAX_SKINSPERPAGE) s_playermodel.numskinpages++;
}

static void PlayerModel_SaveChanges(void) {
	char model[64];
	char* legs;

	strcpy(model, s_playermodel.model.team_modelskin);
	Q_strlwr(model);
	trap_Cvar_Set("team_model", model);

	strcpy(model, s_playermodel.model.team_headskin);
	Q_strlwr(model);
	trap_Cvar_Set("team_headmodel", model);

	strcpy(model, s_playermodel.model.team_legsskin);
	Q_strlwr(model);
	legs = strchr(model, '/');
	trap_Cvar_Set("team_legsmodel", model);

	strcpy(model, s_playermodel.model.modelskin);
	Q_strlwr(model);
	trap_Cvar_Set("model", model);

	strcpy(model, s_playermodel.model.headskin);
	Q_strlwr(model);
	trap_Cvar_Set("headmodel", model);

	strcpy(model, s_playermodel.model.legsskin);
	Q_strlwr(model);
	legs = strchr(model, '/');
	trap_Cvar_Set("legsmodel", model);
}

static void PlayerModel_ShowControls(void) {
	int i;

	s_playermodel.modelChange.generic.flags &= ~(QMF_HIDDEN | QMF_INACTIVE);
	s_playermodel.forceHeadMatch.generic.flags &= ~(QMF_HIDDEN | QMF_INACTIVE);

	// model icons
	for(i = 0; i < MAX_MODELSPERPAGE; i++) s_playermodel.models[i].generic.flags &= ~QMF_HIDDEN;
	for(i = 0; i < MAX_SKINSPERPAGE; i++) s_playermodel.skins[i].generic.flags &= ~QMF_HIDDEN;

	PlayerModel_UpdateGrid();
	PlayerModel_UpdateSkinGrid();

	// head/body update
	if(s_playermodel.forceHeadMatch.curvalue)
		s_playermodel.modelChange.generic.flags |= (QMF_HIDDEN | QMF_INACTIVE);
	else
		s_playermodel.modelChange.generic.flags &= ~(QMF_HIDDEN | QMF_INACTIVE);
}

/*
=================
PlayerModel_SetModelIconSelection

Returns qfalse if no update to the model and skin
selection grids needs to be made
=================
*/
static qboolean PlayerModel_SetModelIconSelection(qboolean samepage) {
	int i;
	int maxlen;
	char modelname[64];
	char skinname[64];
	char search[64];
	char* buffptr;
	char* pdest;
	qboolean found;
	qboolean teamModel;
	int modelBody;
	int oldmodel_page;

	teamModel = drawTeamModel;
	modelBody = qfalse;

	if(s_playermodel.forceHeadMatch.curvalue || s_playermodel.modelChange.curvalue == MODELCHANGE_BODY) {
		modelBody = 1;
	}
	if(s_playermodel.forceHeadMatch.curvalue || s_playermodel.modelChange.curvalue == MODELCHANGE_LEGS) {
		modelBody = 2;
	}

	// grab the correct initial string
	if(modelBody == 1) {
		if(teamModel)
			buffptr = s_playermodel.model.team_modelskin;
		else
			buffptr = s_playermodel.model.modelskin;
	} else if(modelBody == 2) {
		if(teamModel)
			buffptr = s_playermodel.model.team_legsskin;
		else
			buffptr = s_playermodel.model.legsskin;
	} else {
		if(teamModel)
			buffptr = s_playermodel.model.team_headskin;
		else
			buffptr = s_playermodel.model.headskin;
	}

	pdest = strchr(buffptr, '/');
	if(pdest) {
		Q_strncpyz(modelname, buffptr, pdest - buffptr + 1);
		strcpy(skinname, pdest + 1);
	} else {
		strcpy(modelname, buffptr);
		strcpy(skinname, "default");
	}

	// save parameters
	oldmodel_page = s_playermodel.modelpage;

	// reset to default
	s_playermodel.selectedskin = -1;
	s_playermodel.selectedmodel = -1;
	s_playermodel.modelpage = 0;
	s_playermodel.skinpage = 0;

	if(modelBody && s_playermodel.model.bUnknownModel) return qfalse;

	if(!modelBody && s_playermodel.model.bUnknownHeadModel) return qfalse;

	if(!modelBody && s_playermodel.model.bUnknownLegsModel) return qfalse;

	found = qfalse;
	strcpy(search, va(MODELDIR "/%s/", modelname));
	maxlen = strlen(search);
	for(i = 0; i < s_playermodel.nummodels; i++) {
		if(!Q_stricmpn(s_playermodel.modelnames[i], search, maxlen)) {
			found = qtrue;
			break;
		}
	}

	if(!found) return qfalse;

	// found pic, set selection here
	s_playermodel.selectedmodel = i;
	s_playermodel.modelpage = i / MAX_MODELSPERPAGE;

	// now make the skin selection
	PlayerModel_LoadSkins(s_playermodel.selectedmodel);
	found = qfalse;
	for(i = 0; i < s_playermodel.numskins; i++) {
		// strip icon_
		pdest = strstr(s_playermodel.modelskins[i], "icon_");
		if(pdest && Q_stricmp(pdest + 5, skinname) == 0) {
			found = qtrue;
			break;
		}
	}

	if(!found) return qfalse;

	if(samepage && oldmodel_page != s_playermodel.modelpage) {
		s_playermodel.modelpage = oldmodel_page;
	}

	s_playermodel.selectedskin = i;
	s_playermodel.skinpage = i / MAX_SKINSPERPAGE;

	return qtrue;
}

static void PlayerModel_SwapOtherHeads(int modelchange) {
	char tmp[MODELNAME_BUFFER];
	char* thishead;

	// select set we are changing
	if(drawTeamModel) {
		thishead = s_playermodel.model.team_headskin;
	} else {
		thishead = s_playermodel.model.headskin;
	}

	if(modelchange) {
		Q_strncpyz(tmp, s_playermodel.other_head, MODELNAME_BUFFER);
		Q_strncpyz(s_playermodel.other_head, s_playermodel.prevother_head, MODELNAME_BUFFER);
		Q_strncpyz(s_playermodel.prevother_head, tmp, MODELNAME_BUFFER);
	} else {
		Q_strncpyz(tmp, thishead, MODELNAME_BUFFER);
		Q_strncpyz(thishead, s_playermodel.other_head, MODELNAME_BUFFER);
		Q_strncpyz(s_playermodel.other_head, tmp, MODELNAME_BUFFER);
	}
}

static void PlayerModel_MenuEvent(void* ptr, int event) {
	static qboolean tmp;

	if(event != QM_ACTIVATED) return;

	switch(((menucommon_s*)ptr)->id) {
		case ID_PREVPAGE:
			if(s_playermodel.modelpage > 0) {
				s_playermodel.modelpage--;
				PlayerModel_UpdateGrid();
			}
			break;

		case ID_NEXTPAGE:
			if(s_playermodel.modelpage < s_playermodel.numpages - 1) {
				s_playermodel.modelpage++;
				PlayerModel_UpdateGrid();
			}
			break;

		case ID_PREVSKINPAGE:
			if(s_playermodel.skinpage > 0) {
				s_playermodel.skinpage--;
				PlayerModel_UpdateSkinGrid();
			}
			break;

		case ID_NEXTSKINPAGE:
			if(s_playermodel.skinpage < s_playermodel.numskinpages - 1) {
				s_playermodel.skinpage++;
				PlayerModel_UpdateSkinGrid();
			}
			break;

		case ID_TEAMMODEL:
			PlayerModel_SwapOtherHeads(1);

			tmp = s_playermodel.otherheadstate;
			s_playermodel.otherheadstate = s_playermodel.forceHeadMatch.curvalue;
			s_playermodel.forceHeadMatch.curvalue = tmp;

			GUI_PlayerInfo_DrawTeamModel(&s_playermodel.model, s_playermodel.teamModel.curvalue);

			PlayerModel_SetModelIconSelection(qfalse);
			PlayerModel_UpdateGrid();
			PlayerModel_UpdateSkinGrid();
			PlayerModel_ShowControls();
			break;

		case ID_MODELCHANGE:
			PlayerModel_SetModelIconSelection(qtrue);
			PlayerModel_UpdateGrid();
			PlayerModel_UpdateSkinGrid();
			break;

		case ID_FORCEHEADMATCH:
			PlayerModel_SwapOtherHeads(0);
			s_playermodel.model.bForceUpdate = qtrue;

			// update only if model on same page
			PlayerModel_SetModelIconSelection(qtrue);
			PlayerModel_UpdateGrid();
			PlayerModel_UpdateSkinGrid();

			PlayerModel_ShowControls();
			break;
	}
}

static sfxHandle_t PlayerModel_MenuKey(int key) {
	switch(key) {
		case K_MOUSE2:
		case K_ESCAPE: PlayerModel_SaveChanges(); break;
	}
	return (Menu_DefaultKey(&s_playermodel.menu, key));
}

static void PlayerModel_PicInit(menuelement_s* b) {
	int x;
	int y;
	int w;
	int h;

	x = b->generic.x;
	y = b->generic.y;
	w = b->width;
	h = b->height;
	if(w < 0) {
		w = -w;
	}
	if(h < 0) {
		h = -h;
	}

	if(b->generic.flags & QMF_RIGHT_JUSTIFY) {
		x = x - w;
	} else if(b->generic.flags & QMF_CENTER_JUSTIFY) {
		x = x - w / 2;
	}

	b->generic.left = x - PICTURE_GAP / 2;
	b->generic.right = b->generic.left + w + PICTURE_GAP;
	b->generic.top = y - PICTURE_GAP / 2;
	b->generic.bottom = b->generic.top + h + PICTURE_GAP;

	b->shader = 0;
	b->focusshader = 0;
}

static void PlayerModel_PicDraw(void* self) {
	float x;
	float y;
	float w;
	float h;
	menuelement_s* b;

	b = (menuelement_s*)self;

	x = b->generic.x;
	y = b->generic.y;
	w = b->width;
	h = b->height;

	// used to refresh shader
	if(b->string && !b->shader) {
		b->shader = trap_R_RegisterShaderNoMip(b->string);
		if(!b->shader && b->errorpic) b->shader = trap_R_RegisterShaderNoMip(b->errorpic);
	}

	if(b->shader) {
		UI_DrawHandlePic(x, y, w, h, b->shader);
	}
	if((b->generic.flags & QMF_HIGHLIGHT) || ((b->generic.flags & QMF_HIGHLIGHT_IF_FOCUS) && (Menu_ItemAtCursor(b->generic.parent) == b))) {
		UI_DrawHandlePic(x, y, w, h, trap_R_RegisterShaderNoMip("menu/assets/ui_selected"));
	}
}

static qboolean PlayerModel_SetModelFromSelection(char* model) {
	char* buffptr;
	char* pdest;
	char* headmodel;
	char* bodymodel;
	char* legsmodel;

	// get model and strip icon_
	buffptr = model + strlen(MODELDIR) + 1;
	pdest = strstr(buffptr, "icon_");
	if(pdest) {
		// select set we are changing
		if(drawTeamModel) {
			bodymodel = s_playermodel.model.team_modelskin;
			headmodel = s_playermodel.model.team_headskin;
			legsmodel = s_playermodel.model.team_legsskin;
		} else {
			bodymodel = s_playermodel.model.modelskin;
			headmodel = s_playermodel.model.headskin;
			legsmodel = s_playermodel.model.legsskin;
		}

		// track the whole model/skin name
		if(s_playermodel.forceHeadMatch.curvalue || s_playermodel.modelChange.curvalue == MODELCHANGE_LEGS) {
			Q_strncpyz(legsmodel, buffptr, pdest - buffptr + 1);
			strcat(legsmodel, pdest + 5);
		}

		if(s_playermodel.forceHeadMatch.curvalue || s_playermodel.modelChange.curvalue == MODELCHANGE_HEAD) {
			Q_strncpyz(headmodel, buffptr, pdest - buffptr + 1);
			strcat(headmodel, pdest + 5);
		}

		if(s_playermodel.forceHeadMatch.curvalue || s_playermodel.modelChange.curvalue == MODELCHANGE_BODY) {
			Q_strncpyz(bodymodel, buffptr, pdest - buffptr + 1);
			strcat(bodymodel, pdest + 5);
		}

		// update the other_head value so toggling forceHeadMatch
		// switches in the correct model
		if(!s_playermodel.forceHeadMatch.curvalue && s_playermodel.modelChange.curvalue == MODELCHANGE_BODY) {
			Q_strncpyz(s_playermodel.other_head, bodymodel, MODELNAME_BUFFER);
		}

		if(trap_MemoryRemaining() > LOW_MEMORY) {
			s_playermodel.model.bForceUpdate = qtrue;
		}

		return qtrue;
	}
	return qfalse;
}

static void PlayerModel_PicEvent(void* ptr, int event) {
	int i;
	int modelnum;

	if(event != QM_ACTIVATED) return;

	for(i = 0; i < PLAYERGRID_ROWS * PLAYERGRID_COLS; i++) {
		// reset
		s_playermodel.models[i].generic.flags &= ~QMF_HIGHLIGHT;
		s_playermodel.models[i].generic.flags |= QMF_PULSEIFFOCUS;
	}

	// set selected
	i = ((menucommon_s*)ptr)->id - ID_PLAYERPIC0;
	s_playermodel.models[i].generic.flags |= QMF_HIGHLIGHT;
	s_playermodel.models[i].generic.flags &= ~QMF_PULSEIFFOCUS;

	modelnum = s_playermodel.modelpage * MAX_MODELSPERPAGE + i;
	if(PlayerModel_SetModelFromSelection(s_playermodel.modelnames[modelnum])) {
		s_playermodel.selectedmodel = modelnum;
		PlayerModel_LoadSkins(modelnum);
		for(i = 0; i < s_playermodel.numskins; i++) {
			if(!Q_stricmp(s_playermodel.modelnames[modelnum], s_playermodel.modelskins[i])) {
				s_playermodel.selectedskin = i;
				s_playermodel.skinpage = i / MAX_SKINSPERPAGE;
				break;
			}
		}

		PlayerModel_UpdateSkinGrid();
	}
}

static void PlayerModel_SkinPicEvent(void* ptr, int event) {
	int i;
	int skinnum;

	if(event != QM_ACTIVATED) return;

	for(i = 0; i < MAX_SKINSPERPAGE; i++) {
		// reset
		s_playermodel.skins[i].generic.flags &= ~QMF_HIGHLIGHT;
		s_playermodel.skins[i].generic.flags |= QMF_PULSEIFFOCUS;
	}

	// set selected
	i = ((menucommon_s*)ptr)->id - ID_SKINPIC0;
	s_playermodel.skins[i].generic.flags |= QMF_HIGHLIGHT;
	s_playermodel.skins[i].generic.flags &= ~QMF_PULSEIFFOCUS;

	skinnum = s_playermodel.skinpage * MAX_SKINSPERPAGE + i;
	if(PlayerModel_SetModelFromSelection(s_playermodel.modelskins[skinnum])) {
		s_playermodel.selectedskin = skinnum;
	}
}

static void PlayerModel_DrawPlayer(void* self) {
	int style;

	style = MODELNAME_ALLBOLD;
	if(!s_playermodel.forceHeadMatch.curvalue) {
		if(s_playermodel.modelChange.curvalue == MODELCHANGE_HEAD) {
			style |= MODELNAME_BODYWEAK;
		} else if(s_playermodel.modelChange.curvalue == MODELCHANGE_LEGS) {
			style |= MODELNAME_LEGSWEAK;
		} else {
			style |= MODELNAME_HEADWEAK;
		}
	}

	GUI_PlayerInfo_AnimateModel(&s_playermodel.model);
}

static void PlayerModel_SetMenuItems(void) {
	char* thismodel;
	char* thisheadmodel;
	char* thislegsmodel;
	char* othermodel;
	char* otherheadmodel;
	char* otherlegsmodel;
	modelAnim_t* m;

	// Hypo: 3d model already initialized
	m = &s_playermodel.model;

	// team or dm model
	if(drawTeamModel) {
		s_playermodel.teamModel.curvalue = DRAWMODEL_TEAM;
		thismodel = m->team_modelskin;
		thisheadmodel = m->team_headskin;
		thislegsmodel = m->team_legsskin;

		othermodel = m->modelskin;
		otherheadmodel = m->headskin;
		otherlegsmodel = m->legsskin;
	} else {
		s_playermodel.teamModel.curvalue = DRAWMODEL_DM;
		thismodel = m->modelskin;
		thisheadmodel = m->headskin;
		thislegsmodel = m->legsskin;

		othermodel = m->team_modelskin;
		otherheadmodel = m->team_headskin;
		otherlegsmodel = m->team_legsskin;
	}

	// enable "force head" from initial string values
	if(!Q_stricmp(thismodel, thisheadmodel)) {
		s_playermodel.forceHeadMatch.curvalue = qtrue;
	} else {
		s_playermodel.forceHeadMatch.curvalue = qfalse;
	}

	if(!Q_stricmp(othermodel, otherheadmodel)) {
		s_playermodel.otherheadstate = qtrue;
	} else {
		s_playermodel.otherheadstate = qfalse;
	}

	// We need to copy current head selections into history buffers
	// Copy over the body model/skin, this is a good default
	Q_strncpyz(s_playermodel.other_head, thismodel, MODELNAME_BUFFER);
	Q_strncpyz(s_playermodel.prevother_head, othermodel, MODELNAME_BUFFER);

	// find model in our list
	PlayerModel_SetModelIconSelection(qfalse);
}

static void PlayerModel_MenuInit(void) {
	int i, j, k, x, y;

	memset(&s_playermodel, 0, sizeof(playermodel_t));
	s_playermodel.menu.fullscreen = qtrue;
	s_playermodel.menu.key = PlayerModel_MenuKey;

	PlayerModel_BuildList();
	for(i = 0; i < s_playermodel.nummodels; i++)
		trap_R_RegisterShaderNoMip(s_playermodel.modelnames[i]);

	UI_CText(&s_playermodel.e[0], 320, 12, "PLAYER MODEL", UI_CENTER, 1.00);

	UI_CField(&s_playermodel.e[100], 60, 48, "Name:", 24, 24, color_white, "name", NULL, 0); y += 12;

	y = 68;
	UI_CSlider(&s_playermodel.e[1], 60, y, "^1Head:", "headR", 0, 255, 1, NULL, 0); y += 18;
	UI_CSlider(&s_playermodel.e[2], 60, y, "^2Head:", "headG", 0, 255, 1, NULL, 0); y += 18;
	UI_CSlider(&s_playermodel.e[3], 60, y, "^4Head:", "headB", 0, 255, 1, NULL, 0); y = 68;

	UI_CSlider(&s_playermodel.e[4], 185, y, "^1Torso:", "modelR", 0, 255, 1, NULL, 0); y += 18;
	UI_CSlider(&s_playermodel.e[5], 185, y, "^2Torso:", "modelG", 0, 255, 1, NULL, 0); y += 18;
	UI_CSlider(&s_playermodel.e[6], 185, y, "^4Torso:", "modelB", 0, 255, 1, NULL, 0); y = 68;

	UI_CSlider(&s_playermodel.e[7], 310, y, "^1Legs:", "legsR", 0, 255, 1, NULL, 0); y += 18;
	UI_CSlider(&s_playermodel.e[8], 310, y, "^2Legs:", "legsG", 0, 255, 1, NULL, 0); y += 18;
	UI_CSlider(&s_playermodel.e[9], 310, y, "^4Legs:", "legsB", 0, 255, 1, NULL, 0); y = 68;

	UI_CSlider(&s_playermodel.e[10], 435, y, "^1Phys:", "physR", 0, 255, 1, NULL, 0); y += 18;
	UI_CSlider(&s_playermodel.e[11], 435, y, "^2Phys:", "physG", 0, 255, 1, NULL, 0); y += 18;
	UI_CSlider(&s_playermodel.e[12], 435, y, "^4Phys:", "physB", 0, 255, 1, NULL, 0); y = 68;

	y = MODELARRAY_Y;
	for(i = 0, k = 0; i < PLAYERGRID_ROWS; i++) {
		x = MODELARRAY_X;
		for(j = 0; j < PLAYERGRID_COLS; j++, k++) {
			s_playermodel.models[k].generic.type = MTYPE_BITMAP;
			s_playermodel.models[k].generic.flags = QMF_LEFT_JUSTIFY | QMF_NODEFAULTINIT | QMF_PULSEIFFOCUS;
			s_playermodel.models[k].generic.id = ID_PLAYERPIC0 + k;
			s_playermodel.models[k].generic.ownerdraw = PlayerModel_PicDraw;
			s_playermodel.models[k].generic.callback = PlayerModel_PicEvent;
			s_playermodel.models[k].generic.x = x;
			s_playermodel.models[k].generic.y = y;
			s_playermodel.models[k].width = ICON_SIZE;
			s_playermodel.models[k].height = ICON_SIZE;
			PlayerModel_PicInit(&s_playermodel.models[k]);

			x += ICON_SIZE + PICTURE_GAP;
		}
		y += ICON_SIZE + PICTURE_GAP;
	}

	y = MODELARRAY_Y + 4 * (ICON_SIZE + PICTURE_GAP);
	x = MODELARRAY_X;
	for(i = 0; i < MAX_SKINSPERPAGE; i++) {
		s_playermodel.skins[i].generic.type = MTYPE_BITMAP;
		s_playermodel.skins[i].generic.flags = QMF_LEFT_JUSTIFY | QMF_NODEFAULTINIT | QMF_PULSEIFFOCUS;
		s_playermodel.skins[i].generic.id = ID_SKINPIC0 + i;
		s_playermodel.skins[i].generic.ownerdraw = PlayerModel_PicDraw;
		s_playermodel.skins[i].generic.callback = PlayerModel_SkinPicEvent;
		s_playermodel.skins[i].generic.x = x;
		s_playermodel.skins[i].generic.y = y;
		s_playermodel.skins[i].width = ICON_SIZE * 0.90;
		s_playermodel.skins[i].height = ICON_SIZE * 0.90;
		PlayerModel_PicInit(&s_playermodel.skins[i]);

		x += ICON_SIZE * 0.90 + PICTURE_GAP;
	}

	y += ICON_SIZE + PICTURE_GAP;
	s_playermodel.skinleft.generic.type = MTYPE_BITMAP;
	s_playermodel.skinleft.generic.flags = QMF_LEFT_JUSTIFY | QMF_PULSEIFFOCUS;
	s_playermodel.skinleft.generic.callback = PlayerModel_MenuEvent;
	s_playermodel.skinleft.generic.id = ID_PREVSKINPAGE;
	s_playermodel.skinleft.generic.x = SKINARROWS_X;
	s_playermodel.skinleft.generic.y = y;
	s_playermodel.skinleft.width = 32;
	s_playermodel.skinleft.height = 32;
	s_playermodel.skinleft.string = MODEL_ARROWSL;
	s_playermodel.skinleft.focuspic = MODEL_ARROWSL;

	s_playermodel.skinright.generic.type = MTYPE_BITMAP;
	s_playermodel.skinright.generic.flags = QMF_LEFT_JUSTIFY | QMF_PULSEIFFOCUS;
	s_playermodel.skinright.generic.callback = PlayerModel_MenuEvent;
	s_playermodel.skinright.generic.id = ID_NEXTSKINPAGE;
	s_playermodel.skinright.generic.x = SKINARROWS_X + 32;
	s_playermodel.skinright.generic.y = y;
	s_playermodel.skinright.width = 32;
	s_playermodel.skinright.height = 32;
	s_playermodel.skinright.string = MODEL_ARROWSR;
	s_playermodel.skinright.focuspic = MODEL_ARROWSR;

	s_playermodel.model.bitmap.generic.type = MTYPE_BITMAP;
	s_playermodel.model.bitmap.generic.flags = QMF_INACTIVE;
	s_playermodel.model.bitmap.generic.ownerdraw = PlayerModel_DrawPlayer;
	s_playermodel.model.bitmap.generic.x = PLAYERMODEL_X;
	s_playermodel.model.bitmap.generic.y = PLAYERMODEL_Y;
	s_playermodel.model.bitmap.width = PLAYERMODEL_WIDTH;
	s_playermodel.model.bitmap.height = PLAYERMODEL_HEIGHT;

	s_playermodel.modelup.generic.type = MTYPE_BITMAP;
	s_playermodel.modelup.generic.flags = QMF_LEFT_JUSTIFY | QMF_PULSEIFFOCUS;
	s_playermodel.modelup.generic.callback = PlayerModel_MenuEvent;
	s_playermodel.modelup.generic.id = ID_PREVPAGE;
	s_playermodel.modelup.generic.x = MODELARROWS_X;
	s_playermodel.modelup.generic.y = MODELARROWS_Y;
	s_playermodel.modelup.width = 32;
	s_playermodel.modelup.height = 32;
	s_playermodel.modelup.string = MODEL_ARROWST;
	s_playermodel.modelup.focuspic = MODEL_ARROWST;

	s_playermodel.modeldown.generic.type = MTYPE_BITMAP;
	s_playermodel.modeldown.generic.flags = QMF_LEFT_JUSTIFY | QMF_PULSEIFFOCUS;
	s_playermodel.modeldown.generic.callback = PlayerModel_MenuEvent;
	s_playermodel.modeldown.generic.id = ID_NEXTPAGE;
	s_playermodel.modeldown.generic.x = MODELARROWS_X;
	s_playermodel.modeldown.generic.y = MODELARROWS_Y + 32;
	s_playermodel.modeldown.width = 32;
	s_playermodel.modeldown.height = 32;
	s_playermodel.modeldown.string = MODEL_ARROWSB;
	s_playermodel.modeldown.focuspic = MODEL_ARROWSB;

	s_playermodel.teamModel.generic.type = MTYPE_SPINCONTROL;
	s_playermodel.teamModel.string = "";
	s_playermodel.teamModel.generic.flags = QMF_PULSEIFFOCUS | QMF_CENTER_JUSTIFY;
	s_playermodel.teamModel.generic.callback = PlayerModel_MenuEvent;
	s_playermodel.teamModel.generic.id = ID_TEAMMODEL;
	s_playermodel.teamModel.generic.x = TEAMANDSFX_COLUMN - 50;
	s_playermodel.teamModel.generic.y = 480 - 50 + 16 + BASEFONT_HEIGHT + 2;
	s_playermodel.teamModel.itemnames = model_dmteam;

	s_playermodel.forceHeadMatch.generic.type = MTYPE_SPINCONTROL;
	s_playermodel.forceHeadMatch.string = "";
	s_playermodel.forceHeadMatch.generic.flags = QMF_PULSEIFFOCUS | QMF_CENTER_JUSTIFY;
	s_playermodel.forceHeadMatch.generic.callback = PlayerModel_MenuEvent;
	s_playermodel.forceHeadMatch.generic.id = ID_FORCEHEADMATCH;
	s_playermodel.forceHeadMatch.generic.x = BODYPART_COLUMN - 50;
	s_playermodel.forceHeadMatch.generic.y = 480 - 50 + 16 + BASEFONT_HEIGHT + 2;
	s_playermodel.forceHeadMatch.itemnames = model_simplesplit;

	s_playermodel.modelChange.generic.type = MTYPE_SPINCONTROL;
	s_playermodel.modelChange.string = "";
	s_playermodel.modelChange.generic.flags = QMF_PULSEIFFOCUS | QMF_CENTER_JUSTIFY;
	s_playermodel.modelChange.generic.callback = PlayerModel_MenuEvent;
	s_playermodel.modelChange.generic.id = ID_MODELCHANGE;
	s_playermodel.modelChange.generic.x = BODYPART_COLUMN - 50;
	s_playermodel.modelChange.generic.y = 480 - 50 + 16;
	s_playermodel.modelChange.itemnames = model_partchange;

	UI_CreateUI(&s_playermodel.menu, s_playermodel.e);

	for(i = 0; i < MAX_MODELSPERPAGE; i++)
		Menu_AddItem(&s_playermodel.menu, &s_playermodel.models[i]);

	for(i = 0; i < MAX_SKINSPERPAGE; i++)
		Menu_AddItem(&s_playermodel.menu, &s_playermodel.skins[i]);

	Menu_AddItem(&s_playermodel.menu, &s_playermodel.model.bitmap);
	Menu_AddItem(&s_playermodel.menu, &s_playermodel.modelup);
	Menu_AddItem(&s_playermodel.menu, &s_playermodel.modeldown);
	Menu_AddItem(&s_playermodel.menu, &s_playermodel.skinleft);
	Menu_AddItem(&s_playermodel.menu, &s_playermodel.skinright);
	Menu_AddItem(&s_playermodel.menu, &s_playermodel.forceHeadMatch);
	Menu_AddItem(&s_playermodel.menu, &s_playermodel.modelChange);
	Menu_AddItem(&s_playermodel.menu, &s_playermodel.teamModel);

	// set initial states
	GUI_PlayerInfo_InitModel(&s_playermodel.model);

	PlayerModel_SetMenuItems();
	PlayerModel_ShowControls();
}

void UI_PlayerModelMenu(void) {
	PlayerModel_MenuInit();

	UI_PushMenu(&s_playermodel.menu);
}
