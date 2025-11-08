// Copyright (C) 1999-2005 ID Software, Inc.
// Copyright (C) 2023-2025 Noire.dev
// Copyright (C) 2025 OpenSandbox Team
// OpenSandbox — GPLv2; see LICENSE for details.

#include "ui_local.h"

sfxHandle_t menu_move_sound;
sfxHandle_t menu_out_sound;
sfxHandle_t menu_buzz_sound;
sfxHandle_t menu_null_sound;

static qhandle_t sliderBar;
static qhandle_t sliderButton_0;
static qhandle_t sliderButton_1;

static void MField_CharEvent(mfield_t *edit, int ch);

static item_t *UI_FindItem(const char *pickupName) {
	item_t *it;

	for(it = gameInfoItems + 1; it->classname; it++) {
		if(!Q_stricmp(it->pickup_name, pickupName)) return it;
	}

	return NULL;
}

static item_t *UI_FindItemClassname(const char *classname) {
	item_t *it;

	for(it = gameInfoItems + 1; it->classname; it++) {
		if(!Q_stricmp(it->classname, classname)) return it;
	}

	return NULL;
}

static sfxHandle_t Menu_ActivateItem(menuframework_s *s, menucommon_s *item) {
	if(item->callback){
		item->callback(item, QM_ACTIVATED);
		return menu_move_sound;
	}
	if(item->excallback){
		item->excallback(item, QM_ACTIVATED);
		return menu_move_sound;
	}

	return 0;
}

static void BText_Init(menuelement_s *t) { t->generic.flags |= QMF_INACTIVE; }

static void Text_Draw(menuelement_s *t) {
	int x;
	int y;
	float *color;

	x = t->generic.x;
	y = t->generic.y;

	if(t->generic.flags & QMF_GRAYED)
		color = color_disabled;
	else
		color = t->color;

	ST_DrawString(x, y, t->string, t->style, color, t->size);
}

static void PText_Init(menuelement_s *t) {
	int x;
	int y;
	int w;
	int h;

	x = t->generic.x;
	y = t->generic.y;
	w = ST_StringWidth(t->string, t->size);
	h = BASEFONT_HEIGHT * t->size;

	if(t->generic.flags & QMF_RIGHT_JUSTIFY) {
		x -= w;
	} else if(t->generic.flags & QMF_CENTER_JUSTIFY) {
		x -= w / 2;
	}

	t->generic.left = x;
	t->generic.right = x + w;
	t->generic.top = y;
	t->generic.bottom = y + h;
}

static void PText_Draw(menuelement_s *t) {
	int x;
	int y;
	int style;

	x = t->generic.x;
	y = t->generic.y;

	if(t->generic.flags & QMF_GRAYED)
		t->curColor = color_disabled;
	else
		t->curColor = t->color;

	if(t->generic.flags & QMF_HIGHLIGHT_IF_FOCUS) {
		if(Menu_ItemAtCursor(t->generic.parent) == t) {
			t->curColor = color_highlight;
		} else {
			t->curColor = t->color;
		}
	}

	if(t->generic.flags & QMF_HIGHLIGHT) {
		t->curColor = color_grey;
	}

	style = t->style;
	if(t->generic.flags & QMF_PULSEIFFOCUS) {
		t->curColor = t->color;
		if(Menu_ItemAtCursor(t->generic.parent) == t) {
			style |= UI_PULSE;
		}
	}

	ST_DrawString(x, y, t->string, style, t->curColor, t->size);
}

static void Bitmap_Init(menuelement_s *b) {
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

	b->generic.left = x;
	b->generic.right = x + w;
	b->generic.top = y;
	b->generic.bottom = y + h;

	b->shader = 0;
	b->focusshader = 0;
}

static void Bitmap_Draw(menuelement_s *b) {
	float x;
	float y;
	float w;
	float h;
	vec4_t tempcolor;
	float *color;

	tempcolor[0] = tempcolor[1] = tempcolor[2] = tempcolor[3] = 1.00;

	x = b->generic.x;
	y = b->generic.y;
	w = b->width;
	h = b->height;

	if(b->generic.flags & QMF_RIGHT_JUSTIFY) {
		x = x - w;
	} else if(b->generic.flags & QMF_CENTER_JUSTIFY) {
		x = x - w / 2;
	}

	// used to refresh shader
	if(b->string && !b->shader) {
		b->shader = trap_R_RegisterShaderNoMip(b->string);
		if(!b->shader && b->errorpic) b->shader = trap_R_RegisterShaderNoMip(b->errorpic);
	}

	if(b->focuspic && !b->focusshader) b->focusshader = trap_R_RegisterShaderNoMip(b->focuspic);

	if(b->generic.flags & QMF_GRAYED) {
		if(b->shader) {
			trap_R_SetColor(color_disabled);
			UI_DrawHandlePic(x, y, w, h, b->shader);
			trap_R_SetColor(NULL);
		}
	} else {
		if(b->shader) UI_DrawHandlePic(x, y, w, h, b->shader);

		if((b->generic.flags & QMF_PULSE) || (b->generic.flags & QMF_PULSEIFFOCUS) && (Menu_ItemAtCursor(b->generic.parent) == b)) {
			color = tempcolor;
			color[3] = 0.5 + 0.5 * sin(uis.realtime / PULSE_DIVISOR);

			trap_R_SetColor(color);
			UI_DrawHandlePic(x, y, w, h, b->focusshader);
			trap_R_SetColor(NULL);
		} else if((b->generic.flags & QMF_HIGHLIGHT) || ((b->generic.flags & QMF_HIGHLIGHT_IF_FOCUS) && (Menu_ItemAtCursor(b->generic.parent) == b))) {
			UI_DrawHandlePic(x, y, w, h, b->focusshader);
		}
	}
}

static void Action_Init(menuelement_s *a) {
	int len;

	// calculate bounds
	if(a->string)
		len = strlenru(a->string);
	else
		len = 0;

	// left justify text
	a->generic.left = a->generic.x;
	a->generic.right = a->generic.x + len * BIGCHAR_WIDTH;
	a->generic.top = a->generic.y;
	a->generic.bottom = a->generic.y + BIGCHAR_HEIGHT;
}

static void Action_Draw(menuelement_s *a) {
	int x, y;
	int style;
	float *color;

	style = 0;
	color = color_white;
	if(a->generic.flags & QMF_GRAYED) {
		color = color_disabled;
	} else if((a->generic.flags & QMF_PULSEIFFOCUS) && (a->generic.parent->cursor == a->generic.menuPosition)) {
		color = color_highlight;
		style = UI_PULSE;
	} else if((a->generic.flags & QMF_HIGHLIGHT_IF_FOCUS) && (a->generic.parent->cursor == a->generic.menuPosition)) {
		color = color_highlight;
	}

	x = a->generic.x;
	y = a->generic.y;

	ST_DrawString(x, y, a->string, UI_LEFT | style, color, 1.00);

	if(a->generic.parent->cursor == a->generic.menuPosition) {
		// draw cursor
		ST_DrawChar(x - BASEFONT_INDENT*2, y, 13, UI_LEFT, color, 1.00);
	}
}

static void RadioButton_Init(menuelement_s *rb) {
	int len;
	// calculate bounds
	if(rb->string)
		len = strlenru(rb->string);
	else
		len = 0;

	rb->generic.left = rb->generic.x - (len + 1) * BASEFONT_INDENT;
	rb->generic.right = rb->generic.x + 6 * BASEFONT_INDENT;
	rb->generic.top = rb->generic.y;
	rb->generic.bottom = rb->generic.y + BASEFONT_HEIGHT;
}

static sfxHandle_t RadioButton_Key(menuelement_s *rb, int key) {
	switch(key) {
		case K_MOUSE1:
			if(!(rb->generic.flags & QMF_HASMOUSEFOCUS)) break;

		case K_ENTER:
			rb->curvalue = !rb->curvalue;
			if(rb->generic.callback) rb->generic.callback(rb, QM_ACTIVATED);
			if(rb->generic.excallback) rb->generic.excallback(rb, QM_ACTIVATED);

			return (menu_move_sound);
	}

	// key not handled
	return 0;
}

static void RadioButton_Draw(menuelement_s *rb) {
	int x;
	int y;
	float *color;
	int style;
	qboolean focus;

	x = rb->generic.x;
	y = rb->generic.y;

	focus = (rb->generic.parent->cursor == rb->generic.menuPosition);

	if(rb->generic.flags & QMF_GRAYED) {
		color = color_disabled;
		style = UI_LEFT;
	} else if(focus) {
		color = color_highlight;
		style = UI_LEFT | UI_PULSE;
	} else {
		if(!rb->color) {
			color = color_white;
		} else {
			color = rb->color;
		}
		style = UI_LEFT;
	}

	if(rb->string) ST_DrawString(x - BASEFONT_INDENT*2, y, rb->string, UI_RIGHT, color, 1.00);

	if(!rb->curvalue) {
		UI_DrawHandlePic(x, y + 1, 10, 10, uis.rb_off);
		ST_DrawString(x + 16, y, "off", style, color, 1.00);
	} else {
		UI_DrawHandlePic(x, y + 1, 10, 10, uis.rb_on);
		ST_DrawString(x + 16, y, "on", style, color, 1.00);
	}
}

static void Slider_Init(menuelement_s *s) {
	int len;

	// calculate bounds
	if(s->string)
		len = strlenru(s->string);
	else
		len = 0;

	s->generic.left = (s->generic.x - 4);
	s->generic.right = (s->generic.x + 4) + (SLIDER_RANGE * BASEFONT_INDENT);
	s->generic.top = s->generic.y;
	s->generic.bottom = s->generic.y + BASEFONT_HEIGHT;
}

static sfxHandle_t Slider_Key(menuelement_s *s, int key) {
	sfxHandle_t sound;
	int x;
	int oldvalue;

	switch(key) {
		case K_MOUSE1:
			x = uis.cursorx - s->generic.x;
			oldvalue = s->curvalue;
			s->curvalue = (x / (float)(SLIDER_RANGE * BASEFONT_INDENT)) * (s->maxvalue - s->minvalue) + s->minvalue;

			if(s->curvalue < s->minvalue)
				s->curvalue = s->minvalue;
			else if(s->curvalue > s->maxvalue)
				s->curvalue = s->maxvalue;
			if(s->curvalue != oldvalue)
				sound = menu_move_sound;
			else
				sound = 0;
			break;

		case K_LEFTARROW:
			if(s->curvalue > s->minvalue) {
				s->curvalue--;
				sound = menu_move_sound;
			} else
				sound = menu_buzz_sound;
			break;

		case K_RIGHTARROW:
			if(s->curvalue < s->maxvalue) {
				s->curvalue++;
				sound = menu_move_sound;
			} else
				sound = menu_buzz_sound;
			break;

		default:
			// key not handled
			sound = 0;
			break;
	}

	if(sound && s->generic.callback) s->generic.callback(s, QM_ACTIVATED);
	if(sound && s->generic.excallback) s->generic.excallback(s, QM_ACTIVATED);

	return (sound);
}

static void Slider_Draw(menuelement_s *s) {
	int x, y;
	int val;
	int style;
	float *color;
	int button;
	qboolean focus;

	x = s->generic.x;
	y = s->generic.y;
	val = s->curvalue;
	focus = (s->generic.parent->cursor == s->generic.menuPosition);

	if(s->generic.flags & QMF_GRAYED) {
		color = color_disabled;
		style = 0;
	} else if(focus) {
		color = color_highlight;
		style = UI_PULSE;
	} else {
		color = color_white;
		style = 0;
	}

	// draw label
	ST_DrawString(x - BASEFONT_INDENT*2, y, s->string, UI_RIGHT | style, color, 1.00);
	ST_DrawString(x + (SLIDER_RANGE * BASEFONT_INDENT) + BASEFONT_INDENT, y, va("%i", val), UI_LEFT | style, color, 1.00);

	// draw slider
	trap_R_SetColor(color);
	UI_DrawHandlePic(x, y + 2, (SLIDER_RANGE * BASEFONT_INDENT), SLIDER_RANGE, sliderBar);
	trap_R_SetColor(NULL);

	// clamp thumb
	if(s->maxvalue > s->minvalue) {
		s->range = (s->curvalue - s->minvalue) / (float)(s->maxvalue - s->minvalue);
		if(s->range < 0) {
			s->range = 0;
		} else if(s->range > 1) {
			s->range = 1;
		}
	} else {
		s->range = 0;
	}

	// draw thumb
	if(focus) {
		button = sliderButton_1;
	} else {
		button = sliderButton_0;
	}

	UI_DrawHandlePic((int)(x + (SLIDER_RANGE * BASEFONT_INDENT) * s->range) - (s->range * SLIDER_RANGE), y + 2, SLIDER_RANGE, SLIDER_RANGE, button);
}

static void SpinControl_Init(menuelement_s *s) {
	int len;
	int l;
	const char *str;

	if(s->string)
		len = strlenru(s->string) * BASEFONT_INDENT;
	else
		len = 0;

	s->generic.left = s->generic.x - BASEFONT_INDENT - len;

	len = s->numitems = 0;
	while((str = s->itemnames[s->numitems]) != 0) {
		l = strlenru(str);

		if(l > len) len = l;

		s->numitems++;
	}

	s->generic.top = s->generic.y;
	s->generic.right = s->generic.x + (len + 1) * BASEFONT_INDENT;
	s->generic.bottom = s->generic.y + BASEFONT_HEIGHT;
}

static sfxHandle_t SpinControl_Key(menuelement_s *s, int key) {
	sfxHandle_t sound;

	sound = 0;
	switch(key) {
		case K_MOUSE1:
			s->curvalue++;
			if(s->curvalue >= s->numitems) s->curvalue = 0;
			sound = menu_move_sound;
			break;

		case K_LEFTARROW:
			if(s->curvalue > 0) {
				s->curvalue--;
				sound = menu_move_sound;
			} else
				sound = menu_buzz_sound;
			break;

		case K_RIGHTARROW:
			if(s->curvalue < s->numitems - 1) {
				s->curvalue++;
				sound = menu_move_sound;
			} else
				sound = menu_buzz_sound;
			break;
	}

	if(sound && s->generic.callback) s->generic.callback(s, QM_ACTIVATED);
	if(sound && s->generic.excallback) s->generic.excallback(s, QM_ACTIVATED);

	return (sound);
}

static void SpinControl_Draw(menuelement_s *s) {
	float *color;
	int x, y;
	qboolean focus;
	int style;

	style = 0;

	x = s->generic.x;
	y = s->generic.y;

	focus = (s->generic.parent->cursor == s->generic.menuPosition);

	if(s->generic.flags & QMF_GRAYED) {
		color = color_disabled;
	} else if(focus) {
		color = color_highlight;
		style |= UI_PULSE;
	} else {
		color = color_white;
	}

	ST_DrawString(x - BASEFONT_INDENT*2, y, s->string, style | UI_RIGHT, color, 1.00);
	ST_DrawString(x, y, s->itemnames[s->curvalue], style | UI_LEFT, color, 1.00);
}

static void ScrollList_Init(menuelement_s *l) {
	int w;

	l->oldvalue = 0;
	l->curvalue = 0;
	l->top = 0;

	if(!l->columns) l->columns = 1;

	if(l->generic.style <= LST_ICONS) w = (l->width * l->columns) * (BASEFONT_INDENT * l->size);
	if(l->generic.style == LST_GRID) w = l->width * l->columns;

	l->generic.left = l->generic.x;
	l->generic.top = l->generic.y;
	l->generic.right = l->generic.x + w;
	if(l->generic.style <= LST_ICONS) l->generic.bottom = l->generic.y + l->height * (BASEFONT_HEIGHT * l->size);
	if(l->generic.style == LST_GRID) l->generic.bottom = l->generic.y + l->width * l->height;

	if(l->generic.flags & QMF_CENTER_JUSTIFY) {
		l->generic.left -= w / 2;
		l->generic.right -= w / 2;
	}
}

sfxHandle_t ScrollList_Key(menuelement_s *l, int key) {
	static int clicktime = 0;
	int x, y, cursorx, cursory, column, index, clickdelay;

	switch(key) {
		case K_MOUSE1:
		case K_ENTER:
			if(l->generic.flags & QMF_HASMOUSEFOCUS) {
				int item_w, item_h;
				int total_w;

				x = l->generic.x;
				y = l->generic.y;

				if(l->generic.style == LST_GRID) {
					item_w = l->width;
					item_h = l->width;
					total_w = item_w * l->columns;
				} else {
					item_w = (BASEFONT_INDENT * l->size) * l->width;
					item_h = BASEFONT_HEIGHT * l->size;
					total_w = l->width * l->columns * item_w;
				}

				if(UI_CursorInRect(x, y, total_w, l->height * item_h)) {
					cursorx = (uis.cursorx - x) / item_w;
					cursory = (uis.cursory - y) / item_h;

					column = cursorx;
					index = cursory * l->columns + column;

					if(l->top + index < l->numitems) {
						l->oldvalue = l->curvalue;
						l->curvalue = l->top + index;

						clickdelay = uis.realtime - clicktime;
						clicktime = uis.realtime;

						if(l->oldvalue != l->curvalue) {
							if(l->generic.callback) {
								l->generic.callback(l, QM_GOTFOCUS);
							}
							return menu_move_sound;
						} else {
							if((clickdelay < 350) && !(l->generic.flags & (QMF_GRAYED | QMF_INACTIVE))) {
								return Menu_ActivateItem(l->generic.parent, &l->generic);
							}
						}
					}
					return menu_null_sound;
				}
			}
			break;

		case K_MWHEELUP:
			if(l->columns <= 1) {
				if(l->top <= 0) {
					l->top = 0;
				} else {
					l->top -= 1;
				}
				return menu_null_sound;
			}
			if(l->top - l->columns >= 0) {
				l->top -= l->columns;
			} else {
				return menu_buzz_sound;
			}

			if(l->generic.callback) {
				l->generic.callback(l, QM_GOTFOCUS);
			}

			return menu_move_sound;

		case K_MWHEELDOWN:
			if(l->columns <= 1) {
				if(l->top + l->height >= l->numitems) {
					int new_top = l->numitems - l->height;
					l->top = new_top > 0 ? new_top : 0;
				} else {
					l->top += 1;
				}
				return menu_null_sound;
			}
			if(l->top + (l->columns * l->height) < l->numitems) {
				l->top += l->columns;
			} else {
				return menu_buzz_sound;
			}

			if(l->generic.callback) {
				l->generic.callback(l, QM_GOTFOCUS);
			}

			return menu_move_sound;
	}

	return (menu_buzz_sound);
}

static void UI_ServerPlayerIcon(const char *modelAndSkin, char *iconName, int iconNameMaxSize) {
	char *skin;
	char model[MAX_QPATH];

	Q_strncpyz(model, modelAndSkin, sizeof(model));
	skin = strrchr(model, '/');
	if(skin) {
		*skin++ = '\0';
	} else {
		skin = "default";
	}

	Com_sprintf(iconName, iconNameMaxSize, "models/players/%s/icon_%s.tga", model, skin);

	if(!trap_R_RegisterShaderNoMip(iconName) && Q_stricmp(skin, "default") != 0) {
		Com_sprintf(iconName, iconNameMaxSize, "models/players/%s/icon_default.tga", model);
	}
}

static void DrawListItemImage(int x, int y, int w, int h, const char *path, menuelement_s *l, const char *itemname) {
	const char *info;
	char pic[MAX_QPATH];
	item_t *it;

	l->generic.shader = trap_R_RegisterShaderNoMip(path);
	if(l->generic.shader) {
		UI_DrawHandlePic(x, y, w, h, l->generic.shader);
		return;
	}

	l->generic.model = trap_R_RegisterModel(path);
	if(l->generic.model) {
		UI_DrawModelElement(x, y, w, h, path, l->range);
		return;
	}

	info = UI_GetBotInfoByName(itemname);
	UI_ServerPlayerIcon(Info_ValueForKey(info, "model"), pic, MAX_QPATH);
	l->generic.shader = trap_R_RegisterShaderNoMip(pic);
	if(l->generic.shader) {
		UI_DrawHandlePic(x, y, w, h, l->generic.shader);
		return;
	}

	it = UI_FindItem(itemname);
	if(it && it->icon && it->classname) {
		UI_DrawHandlePic(x, y, w, h, trap_R_RegisterShaderNoMip(it->icon));
		return;
	}

	it = UI_FindItemClassname(itemname);
	if(it && it->world_model && it->classname) {
		l->generic.model = trap_R_RegisterModel(it->world_model);
		if(l->generic.model) {
			UI_DrawModelElement(x, y, w, h, it->world_model, l->range);
			return;
		}
	}

	UI_DrawPictureElement(x, y, w, h, path);
}

static void UI_DrawListItemSelection(menuelement_s *l, int i, int x, int y, int item_h, int grid_w) {
	int u = x;

	if(i != l->curvalue) return;

	if(l->generic.flags & QMF_CENTER_JUSTIFY) {
		u -= ((l->width * BASEFONT_INDENT * l->size) / 2 + 1);
	}

	if(l->generic.style <= LST_ICONS) {
		UI_DrawRoundedRect(u, y, (l->width * BASEFONT_INDENT) * l->size, item_h, 0, color_select);
	} else if(l->generic.style == LST_GRID) {
		UI_DrawRoundedRect(u, y, grid_w, grid_w, l->corner, color_select);
	}
}

static void ScrollList_Draw(menuelement_s *l) {
	const int item_h = BASEFONT_HEIGHT * l->size;
	const int grid_w = l->width;
	const int icon_w = BASEFONT_HEIGHT * l->size;
	int column, base, i, style;
	float *color;
	const char *item;

	int x = l->generic.x;
	qboolean hasfocus = (l->generic.parent->cursor == l->generic.menuPosition);
	float select_x = 0, select_y = 0;
	int select_i = -1;

	for(column = 0; column < l->columns; column++) {
		int y = l->generic.y;

		for(base = 0; base < l->height; base++) {
			i = (base * l->columns + column) + l->top;
			if(i >= l->numitems) break;

			item = l->itemnames[i];
			style = UI_LEFT;
			if(l->generic.flags & QMF_CENTER_JUSTIFY) style |= UI_CENTER;
			color = l->color;

			switch(l->generic.style) {
				case LST_SIMPLE:
					UI_DrawListItemSelection(l, i, x, y, item_h, grid_w);
					ST_DrawString(x, y, item, style, color, l->size);
					break;

				case LST_ICONS:
					UI_DrawListItemSelection(l, i, x, y, item_h, grid_w);
					ST_DrawString(x + item_h, y, item, style, color, l->size);
					DrawListItemImage(x, y, item_h, item_h, va("%s/%s", l->string, item), l, item);
					break;

				case LST_GRID:
					if(l->padding_x || l->padding_y) {
						if(!l->drawText) UI_DrawListItemSelection(l, i, x, y, item_h, grid_w);
						if(l->drawText) UI_DrawListItemSelection(l, i, x, y + l->padding_y * 0.50, item_h, grid_w);
					}
					DrawListItemImage(x + l->padding_x, y + l->padding_y, grid_w - (l->padding_x * 2), grid_w - (l->padding_y * 2), va("%s/%s", l->string, item), l, item);
					if(l->drawText) ST_DrawString(x + (grid_w * 0.5), (y + grid_w) - (l->padding_y * 0.80), item, UI_CENTER, color, 0.75);

					if(UI_CursorInRect(x, y, grid_w, grid_w) && hasfocus) {
						select_x = x;
						select_y = y;
						select_i = i;
					}
					if(!l->padding_x && !l->padding_y) UI_DrawListItemSelection(l, i, x, y, item_h, grid_w);
					break;
			}

			y += (l->generic.style == LST_GRID) ? grid_w : item_h;
		}

		x += (l->generic.style == LST_GRID) ? grid_w : (l->width * BASEFONT_INDENT * l->size);
	}

	if(l->generic.style == LST_GRID && hasfocus && select_i >= 0 && l->itemnames[select_i]) {
		const char *item = l->itemnames[select_i];
		float wordsize = BASEFONT_HEIGHT * l->size;

		UI_DrawRoundedRect((uis.cursorx - BASEFONT_INDENT) + 18, uis.cursory - (4 + 18), ((ST_StringCount(item) + 1) * BASEFONT_INDENT) + BASEFONT_WIDTH, wordsize + 8, 5, color_dim);
		ST_DrawString(uis.cursorx + 18, uis.cursory - 18, item, style | UI_DROPSHADOW, color_white, l->size);
	}
}

static void MField_Draw(mfield_t *edit, int x, int y, qboolean focus, vec4_t color, float size) {
	int len, esc;
	int drawLen;
	int prestep;
	char str[MAX_STRING_CHARS];

	drawLen = edit->widthInChars;
	len = strlen(edit->buffer) + 1;
	esc = ST_ColorEscapes(edit->buffer)*2;

	// guarantee that cursor will be visible
	if(len <= drawLen) {
		prestep = 0;
	} else {
		if(edit->scroll + drawLen > len) {
			edit->scroll = len - drawLen;
			if(edit->scroll < 0) {
				edit->scroll = 0;
			}
		}
		prestep = edit->scroll;
	}

	if(prestep + drawLen > len) drawLen = len - prestep;

	if(drawLen >= MAX_STRING_CHARS) trap_Error("drawLen >= MAX_STRING_CHARS");
	memcpy(str, edit->buffer + prestep, drawLen);
	str[drawLen] = 0;

	ST_DrawString(x, y, str, UI_LEFT, color, size);

	// draw the cursor
	if(!focus) return;

	ST_DrawChar(x + (edit->cursor - prestep-esc) * BASEFONT_INDENT, y, 10, UI_LEFT, color, 1.00);
}

static void MField_Paste(mfield_t *edit) {
	char pasteBuffer[64];
	int pasteLen, i;

	trap_GetClipboardData(pasteBuffer, 64);

	// send as if typed, so insert / overstrike works properly
	pasteLen = strlen(pasteBuffer);
	for(i = 0; i < pasteLen; i++) {
		MField_CharEvent(edit, pasteBuffer[i]);
	}
}

static void MField_Clear(mfield_t *edit) {
	edit->buffer[0] = 0;
	edit->cursor = 0;
	edit->scroll = 0;
}

static void MField_CharEvent(mfield_t *edit, int ch) {
	int len;

	if(ch == 'v' - 'a' + 1) {  // ctrl-v is paste
		MField_Paste(edit);
		return;
	}

	if(ch == 'c' - 'a' + 1) {  // ctrl-c clears the field
		MField_Clear(edit);
		return;
	}

	len = strlen(edit->buffer);

	if(ch == 'h' - 'a' + 1) {  // ctrl-h is backspace
		if(edit->cursor > 0) {
			memmove(edit->buffer + edit->cursor - 1, edit->buffer + edit->cursor, len + 1 - edit->cursor);
			edit->cursor--;
			if(edit->cursor < edit->scroll) {
				edit->scroll--;
			}
		}
		return;
	}

	if(ch == 'a' - 'a' + 1) {  // ctrl-a is home
		edit->cursor = 0;
		edit->scroll = 0;
		return;
	}

	if(ch == 'e' - 'a' + 1) {  // ctrl-e is end
		edit->cursor = len;
		edit->scroll = edit->cursor - edit->widthInChars + 1;
		if(edit->scroll < 0) edit->scroll = 0;
		return;
	}

	//
	// ignore any other non printable chars
	//
	if(ch == -48) {
		return;
	}

	if(!trap_Key_GetOverstrikeMode()) {
		if((edit->cursor == MAX_EDIT_LINE - 1) || (edit->maxchars && edit->cursor >= edit->maxchars)) return;
	} else {
		// insert mode
		if((len == MAX_EDIT_LINE - 1) || (edit->maxchars && len >= edit->maxchars)) return;
		memmove(edit->buffer + edit->cursor + 1, edit->buffer + edit->cursor, len + 1 - edit->cursor);
	}

	edit->buffer[edit->cursor] = ch;
	if(!edit->maxchars || edit->cursor < edit->maxchars - 1) edit->cursor++;

	if(edit->cursor >= edit->widthInChars) {
		edit->scroll++;
	}

	if(edit->cursor == len + 1) {
		edit->buffer[edit->cursor] = 0;
	}
}

static void MField_KeyDownEvent(mfield_t *edit, int key) {
	int len;

	// shift-insert is paste
	if(((key == K_INS) || (key == K_KP_INS)) && trap_Key_IsDown(K_SHIFT)) {
		MField_Paste(edit);
		return;
	}

	len = strlen(edit->buffer);

	if(key == K_DEL || key == K_KP_DEL) {
		if(edit->cursor < len) {
			memmove(edit->buffer + edit->cursor, edit->buffer + edit->cursor + 1, len - edit->cursor);
		}
		return;
	}

	if(key == K_RIGHTARROW) {
		if(edit->cursor < len) {
			edit->cursor++;
		}
		if(edit->cursor >= edit->scroll + edit->widthInChars && edit->cursor <= len) {
			edit->scroll++;
		}
		return;
	}

	if(key == K_LEFTARROW) {
		if(edit->cursor > 0) {
			edit->cursor--;
		}
		if(edit->cursor < edit->scroll) {
			edit->scroll--;
		}
		return;
	}

	if(key == K_INS || key == K_KP_INS) {
		trap_Key_SetOverstrikeMode(!trap_Key_GetOverstrikeMode());
		return;
	}
}

static void MenuField_Init(menuelement_s *m) {
	int w, h;

	w = BASEFONT_INDENT * m->size;
	h = BASEFONT_HEIGHT * m->size;

	m->generic.left = m->generic.x - (strlen(m->generic.name) + 1) * w;
	m->generic.top = m->generic.y;
	m->generic.right = m->generic.x + w + m->field.widthInChars * w;
	m->generic.bottom = m->generic.y + h;
}

static void MenuField_Draw(menuelement_s *f) {
	int x;
	int y;
	float *color;
	qboolean focus = qfalse;

	x = f->generic.x;
	y = f->generic.y;

	if(Menu_ItemAtCursor(f->generic.parent) == f) {
		focus = qtrue;
	}

	if(f->generic.flags & QMF_GRAYED) {
		color = color_disabled;
	} else {
		color = f->color;
	}

	if(f->string) ST_DrawString(x - BASEFONT_INDENT*2, y, f->string, UI_RIGHT, color, 1.00);

	MField_Draw(&f->field, x, y, focus, color, 1.00);
}

static sfxHandle_t MenuField_Key(menuelement_s *m, int *key) {
	int keycode;
	static int lastKeypress = 0;

	keycode = *key;

	switch(keycode) {
		case K_ENTER:
		case K_MOUSE1: break;

		case K_TAB:
		case K_DOWNARROW:
		case K_UPARROW: break;

		default:
			if(keycode & K_CHAR_FLAG) {
				keycode &= ~K_CHAR_FLAG;
				MField_CharEvent(&m->field, keycode);
			} else {
				MField_KeyDownEvent(&m->field, keycode);
			}
			break;
	}
	lastKeypress = uis.realtime;

	return (0);
}

void Menu_AddItem(menuframework_s *menu, menuelement_s *item) {
	menucommon_s *itemptr;

	if(menu->nitems >= MAX_MENUITEMS) trap_Error("Menu_AddItem: excessive items");

	menu->items[menu->nitems] = item;
	((menucommon_s *)menu->items[menu->nitems])->parent = menu;
	((menucommon_s *)menu->items[menu->nitems])->menuPosition = menu->nitems;
	((menucommon_s *)menu->items[menu->nitems])->flags &= ~QMF_HASMOUSEFOCUS;

	if(!item->size) item->size = 1.00;

	// perform any item specific initializations
	itemptr = (menucommon_s *)item;
	if(!(itemptr->flags & QMF_NODEFAULTINIT)) {
		switch(itemptr->type) {
			case MTYPE_ACTION: Action_Init((menuelement_s *)item); break;

			case MTYPE_FIELD: MenuField_Init((menuelement_s *)item); break;

			case MTYPE_SPINCONTROL: SpinControl_Init((menuelement_s *)item); break;

			case MTYPE_RADIOBUTTON: RadioButton_Init((menuelement_s *)item); break;

			case MTYPE_SLIDER: Slider_Init((menuelement_s *)item); break;

			case MTYPE_BITMAP: Bitmap_Init((menuelement_s *)item); break;

			case MTYPE_SCROLLLIST: ScrollList_Init((menuelement_s *)item); break;

			case MTYPE_PTEXT: PText_Init((menuelement_s *)item); break;

			case MTYPE_TEXT: BText_Init((menuelement_s *)item); break;

			default: trap_Error(va("Menu_Init: unknown type %d", itemptr->type));
		}
	}

	menu->nitems++;
}

static void Menu_CursorMoved(menuframework_s *m) {
	void (*callback)(void *self, int notification);

	if(m->cursor_prev == m->cursor) return;

	if(m->cursor_prev >= 0 && m->cursor_prev < m->nitems) {
		callback = ((menucommon_s *)(m->items[m->cursor_prev]))->callback;
		if(callback) callback(m->items[m->cursor_prev], QM_LOSTFOCUS);
	}

	if(m->cursor >= 0 && m->cursor < m->nitems) {
		callback = ((menucommon_s *)(m->items[m->cursor]))->callback;
		if(callback) callback(m->items[m->cursor], QM_GOTFOCUS);
	}
}

void Menu_SetCursor(menuframework_s *m, int cursor) {
	if(((menucommon_s *)(m->items[cursor]))->flags & (QMF_GRAYED | QMF_INACTIVE)) {
		// cursor can't go there
		return;
	}

	m->cursor_prev = m->cursor;
	m->cursor = cursor;

	Menu_CursorMoved(m);
}

static void Menu_AdjustCursor(menuframework_s *m, int dir) {
	menucommon_s *item = NULL;
	qboolean wrapped = qfalse;

wrap:
	while(m->cursor >= 0 && m->cursor < m->nitems) {
		item = (menucommon_s *)m->items[m->cursor];
		if((item->flags & (QMF_GRAYED | QMF_INACTIVE))) {
			m->cursor += dir;
		} else {
			break;
		}
	}

	if(dir == 1) {
		if(m->cursor >= m->nitems) {
			if(wrapped) {
				m->cursor = m->cursor_prev;
				return;
			}
			m->cursor = 0;
			wrapped = qtrue;
			goto wrap;
		}
	} else {
		if(m->cursor < 0) {
			if(wrapped) {
				m->cursor = m->cursor_prev;
				return;
			}
			m->cursor = m->nitems - 1;
			wrapped = qtrue;
			goto wrap;
		}
	}
}

void Menu_Draw(menuframework_s *menu) {
	int i;
	menucommon_s *itemptr;

	// draw menu
	for(i = 0; i < menu->nitems; i++) {
		itemptr = (menucommon_s *)menu->items[i];

		if(itemptr->flags & QMF_HIDDEN) continue;

		if(itemptr->ownerdraw) {
			// total subclassing, owner draws everything
			itemptr->ownerdraw(itemptr);
		} else {
			switch(itemptr->type) {
				case MTYPE_RADIOBUTTON: RadioButton_Draw((menuelement_s *)itemptr); break;

				case MTYPE_FIELD: MenuField_Draw((menuelement_s *)itemptr); break;

				case MTYPE_SLIDER: Slider_Draw((menuelement_s *)itemptr); break;

				case MTYPE_SPINCONTROL: SpinControl_Draw((menuelement_s *)itemptr); break;

				case MTYPE_ACTION: Action_Draw((menuelement_s *)itemptr); break;

				case MTYPE_BITMAP: Bitmap_Draw((menuelement_s *)itemptr); break;

				case MTYPE_SCROLLLIST: ScrollList_Draw((menuelement_s *)itemptr); break;

				case MTYPE_PTEXT: PText_Draw((menuelement_s *)itemptr); break;

				case MTYPE_TEXT: Text_Draw((menuelement_s *)itemptr); break;

				default: trap_Error(va("Menu_Draw: unknown type %d", itemptr->type));
			}
		}
		if(uis.debug) {
			int x, y, w, h;

			if(!(itemptr->flags & QMF_INACTIVE)) {
				x = itemptr->left;
				y = itemptr->top;
				w = itemptr->right - itemptr->left + 1;
				h = itemptr->bottom - itemptr->top + 1;
				UI_DrawRoundedRect(x, y, w, h, 0, color_dim);
			}
		}
	}

	itemptr = Menu_ItemAtCursor(menu);
	if(itemptr && itemptr->statusbar) itemptr->statusbar((void *)itemptr);
}

void *Menu_ItemAtCursor(menuframework_s *m) {
	if(m->cursor < 0 || m->cursor >= m->nitems) return 0;

	return m->items[m->cursor];
}

sfxHandle_t Menu_DefaultKey(menuframework_s *m, int key) {
	sfxHandle_t sound = 0;
	menucommon_s *item;
	int cursor_prev;
	menuelement_s *b;

	// menu system keys
	switch(key) {
		case K_MOUSE2:
		case K_ESCAPE: UI_PopMenu(); return menu_out_sound;
	}

	if(!m || !m->nitems) return 0;

	// route key stimulus to widget
	item = Menu_ItemAtCursor(m);
	b = (menuelement_s *)item;
	if(item && !(item->flags & (QMF_GRAYED | QMF_INACTIVE))) {
		switch(item->type) {
			case MTYPE_SPINCONTROL: sound = SpinControl_Key((menuelement_s *)item, key); break;

			case MTYPE_RADIOBUTTON: sound = RadioButton_Key((menuelement_s *)item, key); break;

			case MTYPE_SLIDER: sound = Slider_Key((menuelement_s *)item, key); break;

			case MTYPE_SCROLLLIST: sound = ScrollList_Key((menuelement_s *)item, key); break;

			case MTYPE_FIELD:
				sound = MenuField_Key((menuelement_s *)item, &key);
				item->callback(item, QM_ACTIVATED);
				if(item->excallback) {
					item->excallback(item, QM_ACTIVATED);
				}
				break;
		}

		if(sound) {
			// key was handled
			return sound;
		}
	}

	// default handling
	switch(key) {
		case K_F11: uis.debug ^= 1; break;
		case K_F12: trap_Cmd(EXEC_APPEND, "screenshotJPEG\n"); break;

		case K_UPARROW:
			cursor_prev = m->cursor;
			m->cursor_prev = m->cursor;
			m->cursor--;
			Menu_AdjustCursor(m, -1);
			if(cursor_prev != m->cursor) {
				Menu_CursorMoved(m);
				sound = menu_move_sound;
			}
			break;

		case K_TAB:
		case K_DOWNARROW:
			cursor_prev = m->cursor;
			m->cursor_prev = m->cursor;
			m->cursor++;
			Menu_AdjustCursor(m, 1);
			if(cursor_prev != m->cursor) {
				Menu_CursorMoved(m);
				sound = menu_move_sound;
			}
			break;

		case K_MOUSE1:
			if(item)
				if((item->flags & QMF_HASMOUSEFOCUS) && !(item->flags & (QMF_GRAYED | QMF_INACTIVE))) return (Menu_ActivateItem(m, item));
			break;

		case K_ENTER:
			if(item)
				if(!(item->flags & (QMF_GRAYED | QMF_INACTIVE))) return (Menu_ActivateItem(m, item));
			break;
	}

	return sound;
}

void Menu_Cache(void) {
	uis.cursor = trap_R_RegisterShaderNoMip("menu/assets/3_cursor2");
	uis.corner = trap_R_RegisterShaderNoMip("menu/corner");
	uis.rb_on = trap_R_RegisterShaderNoMip("menu/assets/switch_on");
	uis.rb_off = trap_R_RegisterShaderNoMip("menu/assets/switch_off");
	uis.whiteShader = trap_R_RegisterShaderNoMip("white");
	uis.menuWallpapers = trap_R_RegisterShaderNoMip("menu/animbg");
	menu_move_sound = trap_S_RegisterSound("sound/misc/menu2.wav", qfalse);
	menu_out_sound = trap_S_RegisterSound("sound/misc/menu3.wav", qfalse);
	menu_buzz_sound = trap_S_RegisterSound("sound/misc/menu4.wav", qfalse);
	menu_null_sound = -1;
	sliderBar = trap_R_RegisterShaderNoMip("menu/assets/slider2");
	sliderButton_0 = trap_R_RegisterShaderNoMip("menu/assets/sliderbutt_0");
	sliderButton_1 = trap_R_RegisterShaderNoMip("menu/assets/sliderbutt_1");
}

#define CB_COMMAND 0
#define CB_VARIABLE 1
#define CB_FUNC 2

static void UI_FindButtonPic(menuelement_s *e, int pic) {
	switch(pic) {
		case AST_OSLOGO:
			e->string = "menu/sandbox_logo";
			e->focuspic = "menu/sandbox_logo";
			break;
		case AST_MOD:
			e->string = "menu/gamemode_default";
			e->focuspic = "menu/gamemode_default";
			break;
		case AST_LINK:
			e->string = "menu/officialsite";
			e->focuspic = "menu/officialsite";
			break;
	}
}

void UI_FillList(menuelement_s *e, char *location, char *itemsLocation, char *extension, char *names, int namesSize, char **configlist) {
	int i, len, validItems = 0;
	char *configname;
	qboolean skip = qfalse;

	e->string = itemsLocation;
	if(!strcmp(extension, "$image") || !strcmp(extension, "$sound")) {
		e->numitems = FS_List(location, "", names, namesSize);
	} else {
		e->numitems = FS_List(location, extension, names, namesSize);
	}
	e->itemnames = (const char **)configlist;

	if(e->numitems == 0) {  // Empty folder
		strcpy(names, "Empty");
		e->numitems = 1;
	} else if(e->numitems > 65536) {  // Limit
		e->numitems = 65536;
	}

	configname = names;
	for(i = 0; i < e->numitems; i++) {
		len = strlen(configname);

		skip = qfalse;

		if(!strcmp(extension, "$image")) {
			if(!(Q_stricmp(configname + len - 4, ".png") == 0 || Q_stricmp(configname + len - 4, ".jpg") == 0 || Q_stricmp(configname + len - 4, ".tga") == 0 || Q_stricmp(configname + len - 4, ".bmp") == 0))
				skip = qtrue;
		} else if(!strcmp(extension, "$sound")) {
			if(!(Q_stricmp(configname + len - 4, ".wav") == 0 || Q_stricmp(configname + len - 4, ".ogg") == 0 || Q_stricmp(configname + len - 4, ".mp3") == 0))
				skip = qtrue;
		}

		if(skip) {
			configname += len + 1;
			continue;
		}

		if(!strcmp(extension, "$image")) {
			if(Q_stricmp(configname + len - 4, ".png") == 0) configname[len - 4] = '\0';
			if(Q_stricmp(configname + len - 4, ".jpg") == 0) configname[len - 4] = '\0';
			if(Q_stricmp(configname + len - 4, ".tga") == 0) configname[len - 4] = '\0';
			if(Q_stricmp(configname + len - 4, ".bmp") == 0) configname[len - 4] = '\0';
		} else if(!strcmp(extension, "$sound")) {
			if(Q_stricmp(configname + len - 4, ".wav") == 0) configname[len - 4] = '\0';
			if(Q_stricmp(configname + len - 4, ".ogg") == 0) configname[len - 4] = '\0';
			if(Q_stricmp(configname + len - 4, ".mp3") == 0) configname[len - 4] = '\0';
		} else {
			if(Q_stricmp(configname + len - strlen(extension), extension) == 0)
				configname[len - strlen(extension)] = '\0';
		}

		e->itemnames[validItems++] = configname;
		configname += len + 1;
	}

	e->numitems = UI_CountFiles(location, extension);
}

int UI_CountFiles(const char *location, const char *extension) {
	char tempNames[32000];
	int count = 0;

	if(!strcmp(extension, "$image")){
		count += FS_List(location, ".png", tempNames, sizeof(tempNames));
		count += FS_List(location, ".jpg", tempNames, sizeof(tempNames));
		count += FS_List(location, ".tga", tempNames, sizeof(tempNames));
		count += FS_List(location, ".bmp", tempNames, sizeof(tempNames));
		return count;
	}
	if(!strcmp(extension, "$sound")){
		count += FS_List(location, ".wav", tempNames, sizeof(tempNames));
		count += FS_List(location, ".ogg", tempNames, sizeof(tempNames));
		count += FS_List(location, ".mp3", tempNames, sizeof(tempNames));
		return count;
	}

	return FS_List(location, extension, tempNames, sizeof(tempNames));
}

void UI_FillListFromArray(menuelement_s *e, char **configlist, char **items, int maxItems) {
	int i;

	e->itemnames = (const char **)configlist;
	e->numitems = 0;

	if(maxItems == 0 || !items) {
		static const char *emptyItem = "Empty";
		e->itemnames[0] = emptyItem;
		e->numitems = 1;
		return;
	}

	if(maxItems > 65536) maxItems = 65536;

	for(i = 0; i < maxItems; i++) {
		if(!items[i]) break;

		configlist[i] = (char *)items[i];
		e->itemnames[i] = configlist[i];
		e->numitems++;
	}
}

void UI_FillListOfItems(menuelement_s *e, char *names, int namesSize, char **configlist) {
	int i, len;
	char *itemName;
	char *out = names;

	e->string = "";
	e->itemnames = (const char **)configlist;

	for(i = 0; i < gameInfoItemsNum - 1; i++) {
		itemName = gameInfoItems[i + 1].pickup_name;

		if(!itemName[0]) continue;

		len = strlen(itemName);
		strcpy(out, itemName);
		e->itemnames[i] = out;
		out += len + 1;
	}
	e->numitems = gameInfoItemsNum - 1;
}

void UI_FillListPlayers(menuelement_s *e, char **configlist, char *names, int namesSize) {
	int i, count = 0;
	char *out = names;
	const char *name;
	int len;

	e->string = "";

	for(i = 0; i < MAX_CLIENTS; i++) {
		char info[MAX_INFO_STRING];
		trap_GetConfigString(CS_PLAYERS + i, info, sizeof(info));

		if(!info[0]) continue;

		name = Info_ValueForKey(info, "n");
		if(!name || !name[0]) continue;

		len = strlen(name);
		if(out + len + 1 >= names + namesSize) break;

		strcpy(out, name);
		Q_CleanStr(out);
		configlist[count] = out;

		out += len + 1;
		count++;
	}

	if(count == 0) {
		static const char *emptyItem = "Empty";
		e->itemnames = &emptyItem;
		e->numitems = 1;
	} else {
		e->itemnames = (const char **)configlist;
		e->numitems = count;
	}
}

int UI_ListPlayerCount(void) {
	int i, count = 0;
	const char *name;

	for(i = 0; i < MAX_CLIENTS; i++) {
		char info[MAX_INFO_STRING];
		trap_GetConfigString(CS_PLAYERS + i, info, sizeof(info));

		if(!info[0]) continue;

		name = Info_ValueForKey(info, "n");
		if(!name || !name[0]) continue;

		count++;
	}
	return count;
}

void UI_SetHitbox(menuelement_s *e, float x, float y, float w, float h) {
	e->generic.left = x;
	e->generic.right = x + w;
	e->generic.top = y;
	e->generic.bottom = y + h;
}

static void UI_GeneralCallback(void *ptr, int event) {
	if(event != QM_ACTIVATED) {
		return;
	}

	switch(((menucommon_s *)ptr)->excallbacktype) {
		case CB_COMMAND: trap_Cmd(EXEC_INSERT, ((menucommon_s *)ptr)->cmd); break;

		case CB_VARIABLE:
			if(((menucommon_s *)ptr)->type == MTYPE_SLIDER) {
				cvarSetValue(((menucommon_s *)ptr)->var, (float)*((menucommon_s *)ptr)->value / (float)((menucommon_s *)ptr)->mode);
			}
			if(((menucommon_s *)ptr)->type == MTYPE_FIELD) {
				cvarSet(((menucommon_s *)ptr)->var, ((menucommon_s *)ptr)->buffer);
			}
			if(((menucommon_s *)ptr)->type == MTYPE_RADIOBUTTON) {
				if(((menucommon_s *)ptr)->mode == RBT_NORMAL) {
					cvarSetValue(((menucommon_s *)ptr)->var, (float)*((menucommon_s *)ptr)->value);
				}
				if(((menucommon_s *)ptr)->mode == RBT_INVERSE) {
					cvarSetValue(((menucommon_s *)ptr)->var, -(float)*((menucommon_s *)ptr)->value);
				}
			}
			break;

		case CB_FUNC: ((menucommon_s *)ptr)->func(); break;
	}
}

void UI_CreateUI(menuframework_s *menu, menuelement_s *e) {
	int i;

	for(i = 0; i <= OSUI_MAX_ELEMENTS - 1; i++) {
		if(e[i].generic.type) {
			// ID
			e[i].generic.id = i;

			// Pointers
			e[i].generic.buffer = e[i].field.buffer;
			e[i].generic.value = &e[i].curvalue;

			// Get value
			if(e[i].generic.var) {
				if(e[i].generic.type == MTYPE_SLIDER) {
					e[i].curvalue = cvarInt(e[i].generic.var) * (float)e[i].generic.mode;
				}
				if(e[i].generic.type == MTYPE_FIELD) {
					Q_strncpyz(e[i].field.buffer, UI_Cvar_VariableString(e[i].generic.var), MAX_EDIT_LINE);
				}
				if(e[i].generic.type == MTYPE_RADIOBUTTON) {
					if(e[i].generic.mode == RBT_NORMAL) {
						e[i].curvalue = cvarInt(e[i].generic.var);
					}
					if(e[i].generic.mode == RBT_INVERSE) {
						e[i].curvalue = -cvarInt(e[i].generic.var);
					}
				}
			}

			e[i].generic.flags |= (QMF_PULSEIFFOCUS);
			Menu_AddItem(menu, &e[i]);
		}
	}
}

void UI_CButton(menuelement_s *e, float x, float y, char *text, int style, float size, float *color, char *cmd, char *var, void (*func)(void), void (*callback)(void *self, int event), int callid) {
	e->generic.type = MTYPE_PTEXT;
	e->generic.x = x;
	e->generic.y = y;
	e->size = size;
	e->string = text;
	e->generic.callback = callback;
	e->generic.callid = callid;
	e->generic.excallback = UI_GeneralCallback;
	if(cmd) {
		e->generic.excallbacktype = CB_COMMAND;
		e->generic.cmd = cmd;
	} else if(var) {
		e->generic.excallbacktype = CB_VARIABLE;
		e->generic.var = var;
	} else if(func) {
		e->generic.excallbacktype = CB_FUNC;
		e->generic.func = func;
	}
	e->color = color;

	if(style == UI_LEFT) {
		e->generic.flags = QMF_LEFT_JUSTIFY;
		e->style = UI_LEFT;
	}
	if(style == UI_CENTER) {
		e->generic.flags = QMF_CENTER_JUSTIFY;
		e->style = UI_CENTER;
	}
	if(style == UI_RIGHT) {
		e->generic.flags = QMF_RIGHT_JUSTIFY;
		e->style = UI_RIGHT;
	}
}

void UI_CSlider(menuelement_s *e, float x, float y, char *text, char *var, float min, float max, float mod, void (*callback)(void *self, int event), int callid) {
	e->generic.type = MTYPE_SLIDER;
	e->generic.x = x;
	e->generic.y = y;
	e->string = text;
	e->generic.callback = callback;
	e->generic.callid = callid;
	e->generic.excallback = UI_GeneralCallback;
	e->minvalue = min;
	e->maxvalue = max;
	e->generic.mode = mod;
	if(var) {
		e->generic.excallbacktype = CB_VARIABLE;
		e->generic.var = var;
	}
	e->color = color_white;
}

void UI_CRadioButton(menuelement_s *e, float x, float y, char *text, char *var, int mod, void (*callback)(void *self, int event), int callid) {
	e->generic.type = MTYPE_RADIOBUTTON;
	e->generic.x = x;
	e->generic.y = y;
	e->string = text;
	e->generic.callback = callback;
	e->generic.callid = callid;
	e->generic.excallback = UI_GeneralCallback;
	e->generic.mode = mod;
	if(var) {
		e->generic.excallbacktype = CB_VARIABLE;
		e->generic.var = var;
	}
	e->color = color_white;
}

void UI_CSpinControl(menuelement_s *e, float x, float y, char *text, const char **list, void (*callback)(void *self, int event), int callid) {
	e->generic.type = MTYPE_SPINCONTROL;
	e->generic.x = x;
	e->generic.y = y;
	e->string = text;
	e->generic.callback = callback;
	e->generic.callid = callid;
	e->itemnames = list;
	e->color = color_white;
}

void UI_CList(menuelement_s *e, float x, float y, float size, int h, int w, float pad_x, float pad_y, int style, qboolean drawText, int corner, float *color, void (*callback)(void *self, int event), int callid) {
	e->generic.type = MTYPE_SCROLLLIST;
	e->generic.style = style;
	e->generic.x = x;
	e->generic.y = y;
	e->padding_x = pad_x;
	e->padding_y = pad_y;
	e->width = size;
	e->height = h;
	e->columns = w;
	e->generic.callback = callback;
	e->generic.callid = callid;
	e->color = color;
	e->corner = corner;
	e->drawText = drawText;
	e->range = 80.0;
}

void UI_CField(menuelement_s *e, float x, float y, char *text, int w, int maxchars, float *color, char *var, void (*callback)(void *self, int event), int callid) {
	e->generic.type = MTYPE_FIELD;
	e->generic.x = x;
	e->generic.y = y;
	e->string = text;
	e->field.widthInChars = w;
	e->field.maxchars = maxchars;
	e->generic.callback = callback;
	e->generic.callid = callid;
	e->generic.excallback = UI_GeneralCallback;
	e->color = color;
	if(var) {
		e->generic.excallbacktype = CB_VARIABLE;
		e->generic.var = var;
	}
}

void UI_CText(menuelement_s *e, float x, float y, char *text, int style, float size) {
	e->generic.type = MTYPE_TEXT;
	e->generic.x = x;
	e->generic.y = y;
	e->size = size;
	e->string = text;
	e->color = color_white;

	if(style == UI_LEFT) {
		e->style = UI_LEFT;
	}
	if(style == UI_CENTER) {
		e->style = UI_CENTER;
	}
	if(style == UI_RIGHT) {
		e->style = UI_RIGHT;
	}
}

void UI_CPicture(menuelement_s *e, float x, float y, float w, float h, int pic, int flags, char *cmd, char *var, void (*func)(void), void (*callback)(void *self, int event), int callid) {
	UI_FindButtonPic(e, pic);

	e->generic.type = MTYPE_BITMAP;
	e->generic.flags = flags;
	e->generic.callback = callback;
	e->generic.callid = callid;
	e->generic.excallback = UI_GeneralCallback;
	if(cmd) {
		e->generic.excallbacktype = CB_COMMAND;
		e->generic.cmd = cmd;
	} else if(var) {
		e->generic.excallbacktype = CB_VARIABLE;
		e->generic.var = var;
	} else if(func) {
		e->generic.excallbacktype = CB_FUNC;
		e->generic.func = func;
	}
	e->generic.x = x;
	e->generic.y = y;
	e->width = w;
	e->height = h;
}
