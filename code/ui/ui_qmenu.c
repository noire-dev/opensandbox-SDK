// Copyright (C) 1999-2005 ID Software, Inc.
// OpenSandbox by noire.dev — GPLv2

#include "../shared/javascript.h"

qboolean Menu_CursorOnItem(int id) {
	if(uis.cursorx < uis.items[id].x || uis.cursory < uis.items[id].y || uis.cursorx > uis.items[id].x + uis.items[id].w || uis.cursory > uis.items[id].y + uis.items[id].h) return qfalse;
	return qtrue;
}

#define CB_NONE 0
#define CB_COMMAND 1
#define CB_VARIABLE 2
#define CB_FUNC 3

static void DrawTip(const char *text) {
	drawRoundedRect((uis.cursorx - BASEFONT_INDENT) + 17, uis.cursory - 21, stringWidth(text, 1.00)+BASEFONT_INDENT*2, BASEFONT_HEIGHT + 4, 0, cgui.colors[JSCOLOR_TIPSHADOW]);
	drawRoundedRect((uis.cursorx - BASEFONT_INDENT) + 18, uis.cursory - 20, stringWidth(text, 1.00)+BASEFONT_INDENT*2, BASEFONT_HEIGHT + 4, 0, cgui.colors[JSCOLOR_TIP]);
	drawString(uis.cursorx + 18, uis.cursory - 18, text, 0, cgui.colors[JSCOLOR_TIPTEXT], 1.00);
}

static float SliderValue(float min, float max, float x, float w) {
    float local_x = (float)uis.cursorx - x;
    
    if(local_x < 0.0f) local_x = 0.0f;
    if(local_x > w) local_x = w;
    
    return min + (local_x / w) * (max - min);
}

static void UI_GeneralCallback(menuelement_s *item, int key) {
	switch(item->type) {
    case MTYPE_BUTTON:
        if(strlen(item->action)) trap_Cmd(EXEC_INSERT, (item->action));
        break;
    case MTYPE_CHECKBOX:
        if(item->value > 0) item->value = 0;
        else item->value = 1;
        if(strlen(item->action)) cvarSet(item->action, va("%f", item->value));
        break;
	case MTYPE_SLIDER:
        switch(key) {
	    case K_MOUSE1: if(Menu_CursorOnItem(item->id)) 
	        if(item->mode == EMODE_INT) item->value = (int)(SliderValue(item->min, item->max, item->x+item->corner, item->w-(item->corner*2)));
	        if(item->mode == EMODE_FLOAT) item->value = SliderValue(item->min, item->max, item->x+item->corner, item->w-(item->corner*2)); 
	        break;
	    }
        if(strlen(item->action)) cvarSet(item->action, va("%f", item->value));
        break;
	}
	JS_MenuCallback(item->id);
}

static void Button_Draw(menuelement_s *e) {
    int tcolor = Menu_CursorOnItem(e->id) ? JSCOLOR_SELECTED : e->colortext;
	drawButton(e->x, e->y, e->w, e->h, e->corner, tcolor, e->colorbg, e->size, e->margin, e->style, e->text);
}

static void Checkbox_Draw(menuelement_s *e) {
    int tcolor = Menu_CursorOnItem(e->id) ? JSCOLOR_SELECTED : e->colortext;
    int scolor = e->value ? JSCOLOR_ENABLED : JSCOLOR_DISABLED;
    drawCheckbox(e->x, e->y, e->w, e->h, e->corner, tcolor, scolor, e->colorbg, e->size, e->margin, e->style, e->text);
}

static void Slider_Draw(menuelement_s *e) {
    int tcolor = Menu_CursorOnItem(e->id) ? JSCOLOR_SELECTED : e->colortext;
	drawSlider(e->x, e->y, e->w, e->h, e->corner, tcolor, e->colorbg, e->size, e->margin, e->style, e->text, e->value, e->min, e->max);
    if(Menu_CursorOnItem(e->id)) DrawTip(va("%f", e->value));
}

static void Action_Draw(menuelement_s *e) {
    int tcolor = Menu_CursorOnItem(e->id) ? JSCOLOR_SELECTED : e->colortext;
    drawAction(e->x, e->y, e->w, e->h, e->corner, tcolor, e->colorbg, e->size, e->margin, e->style, e->text, e->bind);
}

void Menu_ElementsDraw(void) {
	int i;

	for(i = 0; i < MAX_MENUITEMS; i++) {
		switch(uis.items[i].type) {
		    case MTYPE_BUTTON: Button_Draw(&uis.items[i]); break;
		    case MTYPE_CHECKBOX: Checkbox_Draw(&uis.items[i]); break;
		    case MTYPE_SLIDER: Slider_Draw(&uis.items[i]); break;
		}
	}
}

static void *Menu_CurrentItem(void) {
	if(uis.currentItem < 0 || uis.currentItem >= MAX_MENUITEMS) return 0;
	return &uis.items[uis.currentItem];
}

void Menu_DefaultKey(int key) {
	menuelement_s *item;

	switch(key) {
	case K_ESCAPE: UI_SetActiveMenu(UI_DISABLE); break;
    case K_F5: UI_HotReloadJS(); break;
	}

	item = Menu_CurrentItem();

	switch(key) {
	case K_MOUSE1: if(Menu_CursorOnItem(item->id)) UI_GeneralCallback(item, key); break;
	case K_ENTER: UI_GeneralCallback(item, key); break;
	}
	
	JS_MenuKey(key);
}

static qboolean UI_GenericItem(int type, int id, float x, float y, float w, float h, char *text, int style, float size, int colortext, int colorbg, int corner, int margin, char *action) {
    if(id >= MAX_MENUITEMS) {
        print("^3Menu item limit exceeded");
        return qfalse;
    }
    uis.items[id].type = type;
    uis.items[id].id = id;
	uis.items[id].x = x;
	uis.items[id].y = y;
	uis.items[id].w = w;
	uis.items[id].h = h;
	StringCopy(uis.items[id].text, text, UI_STRINGLENGTH);
	uis.items[id].style = style;
	uis.items[id].size = size;
	uis.items[id].colortext = colortext;
	uis.items[id].colorbg = colorbg;
	uis.items[id].corner = corner;
	uis.items[id].margin = margin;
	StringCopy(uis.items[id].action, action, UI_ACTIONLENGTH);
	return qtrue;
}

int UI_CButton(int id, float x, float y, float w, float h, char *text, int style, float size, int colortext, int colorbg, int corner, int margin, char *action) {
    if(!UI_GenericItem(MTYPE_BUTTON, id, x, y, w, h, text, style, size, colortext, colorbg, corner, margin, action)) return -1;
    return id;
}

int UI_CCheckbox(int id, float x, float y, float w, float h, char *text, int style, float size, int colortext, int colorbg, int corner, int margin, char *action) {
    if(!UI_GenericItem(MTYPE_CHECKBOX, id, x, y, w, h, text, style, size, colortext, colorbg, corner, margin, action)) return -1;
	uis.items[id].value = cvarFloat(uis.items[id].action);
	return id;
}

int UI_CSlider(int id, float x, float y, float w, float h, char *text, int style, float size, int colortext, int colorbg, int corner, int margin, char *action, float min, float max, int mode) {
    if(!UI_GenericItem(MTYPE_SLIDER, id, x, y, w, h, text, style, size, colortext, colorbg, corner, margin, action)) return -1;
	uis.items[id].value = cvarFloat(uis.items[id].action);
	uis.items[id].min = min;
	uis.items[id].max = max;
	uis.items[id].mode = mode;
	return id;
}

int UI_CAction(int id, float x, float y, float w, float h, char *text, int style, float size, int colortext, int colorbg, int corner, int margin, char *action) {
    if(!UI_GenericItem(MTYPE_CHECKBOX, id, x, y, w, h, text, style, size, colortext, colorbg, corner, margin, action)) return -1;
	// write bindkey string to bindkey, check cgui.binds strings for our action and save id, and load bindkey string from engine by id
	return id;
}
