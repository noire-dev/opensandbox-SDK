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
/**********************************************************************
	UI_QMENU.C

	Quake's menu framework system.
**********************************************************************/

#include "ui_local.h"

sfxHandle_t menu_in_sound;
sfxHandle_t menu_move_sound;
sfxHandle_t menu_out_sound;
sfxHandle_t menu_buzz_sound;
sfxHandle_t menu_null_sound;
sfxHandle_t weaponChangeSound;

static qhandle_t	sliderBar;
static qhandle_t	sliderButton_0;
static qhandle_t	sliderButton_1;

// action widget
static void	Action_Init( menuelement_s *a );
static void	Action_Draw( menuelement_s *a );

// radio button widget
static void	RadioButton_Draw( menuelement_s *rb );
static sfxHandle_t RadioButton_Key( menuelement_s *rb, int key );

// slider widget
static void Slider_Init( menuelement_s *s );
static sfxHandle_t Slider_Key( menuelement_s *s, int key );
static void	Slider_Draw( menuelement_s *s );

// spin control widget
static void	SpinControl_Draw( menuelement_s *s );
static sfxHandle_t SpinControl_Key( menuelement_s *l, int key );

// scrolllist widget
sfxHandle_t ScrollList_Key( menuelement_s *l, int key );

// proportional text widget
static void PText_Draw( menuelement_s *b );

//text widget
static void Text_Init( menuelement_s *b );
static void Text_Draw( menuelement_s *b );

/*
===============
UI_FindItem

===============
*/
item_t	*UI_FindItem( const char *pickupName ) {
	item_t	*it;
	
	for ( it = gameInfoItems + 1 ; it->classname ; it++ ) {
		if ( !Q_stricmp( it->pickup_name, pickupName ) )
			return it;
	}

	return NULL;
}

item_t	*UI_FindItemClassname( const char *classname ) {
	item_t	*it;
	
	for ( it = gameInfoItems + 1 ; it->classname ; it++ ) {
		if ( !Q_stricmp( it->classname, classname ) )
			return it;
	}

	return NULL;
}

/*
=================
BText_Init
=================
*/
static void BText_Init( menuelement_s *t )
{
	t->generic.flags |= QMF_INACTIVE;
}

/*
=================
Text_Draw
=================
*/
static void Text_Draw( menuelement_s *t )
{
	int		x;
	int		y;
	float*	color;

	x = t->generic.x;
	y = t->generic.y;

	if (t->generic.flags & QMF_GRAYED)
		color = color_disabled;
	else
		color = t->color;

	ST_DrawString( x, y, t->string, t->style, color, t->size );
}

/*
=================
PText_Init
=================
*/
void PText_Init( menuelement_s *t )
{
	int	x;
	int	y;
	int	w;
	int	h;

	x = t->generic.x;
	y = t->generic.y;
	w = ST_StringWidth( t->string, t->size );
	h =	BASEFONT_HEIGHT * t->size;

	if( t->generic.flags & QMF_RIGHT_JUSTIFY ) {
		x -= w;
	}
	else if( t->generic.flags & QMF_CENTER_JUSTIFY ) {
		x -= w / 2;
	}

	t->generic.left   = x;
	t->generic.right  = x + w;
	if(t->generic.heightmod){
	t->generic.top    = y - (t->generic.heightmod*h);
	t->generic.bottom = y + (t->generic.heightmod*h);
	} else {
	t->generic.top    = y;
	t->generic.bottom = y + h;	
	}
}

/*
=================
PText_Draw
=================
*/
static void PText_Draw( menuelement_s *t )
{
	int		x;
	int		y;
	float *	color;
	int		style;

	x = t->generic.x;
	y = t->generic.y;

	if (t->generic.flags & QMF_GRAYED)
		color = color_disabled;
	else
		color = t->color;
	
	if( t->generic.flags & QMF_HIGHLIGHT_IF_FOCUS ) {
	if( Menu_ItemAtCursor( t->generic.parent ) == t ) {
		t->color = color_highlight;
	}
	else {
		t->color = color_white;
	}
	}

	if( t->generic.flags & QMF_HIGHLIGHT ) {
		t->color = color_grey;
	}

	style = t->style;
	if( t->generic.flags & QMF_PULSEIFFOCUS ) {
		t->color = color_white;
		if( Menu_ItemAtCursor( t->generic.parent ) == t ) {
			style |= UI_PULSE;
		}
		else {
			style |= UI_INVERSE;
		}
	}

	ST_DrawString( x, y, t->string, style, color, t->size );
}

/*
=================
Bitmap_Init
=================
*/
void Bitmap_Init( menuelement_s *b )
{
	int	x;
	int	y;
	int	w;
	int	h;

	x = b->generic.x;
	y = b->generic.y;
	w = b->width;
	h =	b->height;
	if( w < 0 ) {
		w = -w;
	}
	if( h < 0 ) {
		h = -h;
	}

	if (b->generic.flags & QMF_RIGHT_JUSTIFY)
	{
		x = x - w;
	}
	else if (b->generic.flags & QMF_CENTER_JUSTIFY)
	{
		x = x - w/2;
	}

	b->generic.left   = x;
	b->generic.right  = x + w;
	b->generic.top    = y;
	b->generic.bottom = y + h;

	b->shader      = 0;
	b->focusshader = 0;
}

/*
=================
Bitmap_Draw
=================
*/
void Bitmap_Draw( menuelement_s *b )
{
	float	x;
	float	y;
	float	w;
	float	h;
	vec4_t	tempcolor;
	float*	color;

	tempcolor[0] = tempcolor[1] = tempcolor[2] = tempcolor[3] = 1.00;

	x = b->generic.x;
	y = b->generic.y;
	w = b->width;
	h =	b->height;

	if (b->generic.flags & QMF_RIGHT_JUSTIFY)
	{
		x = x - w;
	}
	else if (b->generic.flags & QMF_CENTER_JUSTIFY)
	{
		x = x - w/2;
	}

	// used to refresh shader
	if (b->string && !b->shader)
	{
		b->shader = trap_R_RegisterShaderNoMip( b->string );
		if (!b->shader && b->errorpic)
			b->shader = trap_R_RegisterShaderNoMip( b->errorpic );
	}

	if (b->focuspic && !b->focusshader)
		b->focusshader = trap_R_RegisterShaderNoMip( b->focuspic );

	if (b->generic.flags & QMF_GRAYED)
	{
		if (b->shader)
		{
			trap_R_SetColor( colorMdGrey );
			UI_DrawHandlePic( x, y, w, h, b->shader );
			trap_R_SetColor( NULL );
		}
	}
	else
	{
		if (b->shader)
			UI_DrawHandlePic( x, y, w, h, b->shader );

		if ((b->generic.flags & QMF_PULSE) || (b->generic.flags & QMF_PULSEIFFOCUS) && (Menu_ItemAtCursor( b->generic.parent ) == b))
		{
			color = tempcolor;
			color[3] = 0.5+0.5*sin(uis.realtime/PULSE_DIVISOR);

			trap_R_SetColor( color );
			UI_DrawHandlePic( x, y, w, h, b->focusshader );
			trap_R_SetColor( NULL );
		}
		else if ((b->generic.flags & QMF_HIGHLIGHT) || ((b->generic.flags & QMF_HIGHLIGHT_IF_FOCUS) && (Menu_ItemAtCursor( b->generic.parent ) == b)))
		{	
			UI_DrawHandlePic( x, y, w, h, b->focusshader );
		}
	}
}

/*
=================
Action_Init
=================
*/
static void Action_Init( menuelement_s *a )
{
	int	len;

	// calculate bounds
	if (a->string)
		len = strlenru(a->string);
	else
		len = 0;

	// left justify text
	a->generic.left   = a->generic.x; 
	a->generic.right  = a->generic.x + len*BIGCHAR_WIDTH;
	a->generic.top    = a->generic.y;
	a->generic.bottom = a->generic.y + BIGCHAR_HEIGHT;
}

/*
=================
Action_Draw
=================
*/
static void Action_Draw( menuelement_s *a )
{
	int		x, y;
	int		style;
	float*	color;

	style = 0;
	color = color_white;
	if ( a->generic.flags & QMF_GRAYED )
	{
		color = color_disabled;
	}
	else if (( a->generic.flags & QMF_PULSEIFFOCUS ) && ( a->generic.parent->cursor == a->generic.menuPosition ))
	{
		color = color_highlight;
		style = UI_PULSE;
	}
	else if (( a->generic.flags & QMF_HIGHLIGHT_IF_FOCUS ) && ( a->generic.parent->cursor == a->generic.menuPosition ))
	{
		color = color_highlight;
	}
	else if ( a->generic.flags & QMF_BLINK )
	{
		style = UI_BLINK;
		color = color_highlight;
	}

	x = a->generic.x;
	y = a->generic.y;

	ST_DrawString( x, y, a->string, UI_LEFT|style, color, 1.00 );

	if ( a->generic.parent->cursor == a->generic.menuPosition )
	{
		// draw cursor
		ST_DrawChar( x - BIGCHAR_WIDTH, y, 13, UI_LEFT|UI_BLINK, color, 1.00);
	}
}

/*
=================
RadioButton_Init
=================
*/
void RadioButton_Init( menuelement_s *rb ) {
	int	len;
	// calculate bounds
	if (rb->string)
		len = strlenru(rb->string);
	else
		len = 0;
	
	rb->generic.left   = rb->generic.x - (len+1)*BASEFONT_INDENT;
	rb->generic.right  = rb->generic.x + 6*BASEFONT_INDENT;
	rb->generic.top    = rb->generic.y;
	rb->generic.bottom = rb->generic.y + BASEFONT_HEIGHT;
}

/*
=================
RadioButton_Key
=================
*/
static sfxHandle_t RadioButton_Key( menuelement_s *rb, int key ) {
	switch (key) {
		case K_MOUSE1:
			if (!(rb->generic.flags & QMF_HASMOUSEFOCUS))
				break;

		case K_ENTER:
			rb->curvalue = !rb->curvalue;
			if ( rb->generic.callback )
				rb->generic.callback( rb, QM_ACTIVATED );
			if ( rb->generic.excallback )
				rb->generic.excallback( rb, QM_ACTIVATED );

			return (menu_move_sound);
	}

	// key not handled
	return 0;
}

/*
=================
RadioButton_Draw
=================
*/
static void RadioButton_Draw( menuelement_s *rb ) {
	int			x;
	int 		y;
	float 		*color;
	int			style;
	qboolean 	focus;

	x = rb->generic.x;
	y = rb->generic.y;

	focus = (rb->generic.parent->cursor == rb->generic.menuPosition);

	if ( rb->generic.flags & QMF_GRAYED ) {
		color = color_disabled;
		style = UI_LEFT|UI_SMALLFONT;
	} else if ( focus ) {
		color = color_highlight;
		style = UI_LEFT|UI_PULSE|UI_SMALLFONT;
	} else {
		if(!rb->color){
			color = color_white;
		} else {
			color = rb->color;
		}
		style = UI_LEFT|UI_SMALLFONT;
	}

	if ( rb->string )
		ST_DrawString( x - BASEFONT_INDENT, y, rb->string, UI_RIGHT|UI_SMALLFONT, color, 1.00 );

	if ( !rb->curvalue ) {
		UI_DrawHandlePic( x, y+1, 10, 10, uis.rb_off);
		ST_DrawString( x + 16, y, "off", style, color, 1.00 );
	} else {
		UI_DrawHandlePic( x, y+1, 10, 10, uis.rb_on );
		ST_DrawString( x + 16, y, "on", style, color, 1.00 );
	}
}

/*
=================
Slider_Init
=================
*/
static void Slider_Init( menuelement_s *s ) {
	int len;

	// calculate bounds
	if (s->string)
		len = strlenru(s->string);
	else
		len = 0;

	s->generic.left   = (s->generic.x-4); 
	s->generic.right  = (s->generic.x+4) + (SLIDER_RANGE*BASEFONT_INDENT);
	s->generic.top    = s->generic.y;
	s->generic.bottom = s->generic.y + BASEFONT_HEIGHT;
}

/*
=================
Slider_Key
=================
*/
static sfxHandle_t Slider_Key( menuelement_s *s, int key )
{
	sfxHandle_t	sound;
	int			x;
	int			oldvalue;

	switch (key)
	{
		case K_MOUSE1:
			x           = uis.cursorx - s->generic.x;
			oldvalue    = s->curvalue;
			s->curvalue = (x/(float)(SLIDER_RANGE*BASEFONT_INDENT)) * (s->maxvalue-s->minvalue) + s->minvalue;

			if (s->curvalue < s->minvalue)
				s->curvalue = s->minvalue;
			else if (s->curvalue > s->maxvalue)
				s->curvalue = s->maxvalue;
			if (s->curvalue != oldvalue)
				sound = menu_move_sound;
			else
				sound = 0;
			break;

		case K_LEFTARROW:
			if (s->curvalue > s->minvalue)
			{
				s->curvalue--;
				sound = menu_move_sound;
			}
			else
				sound = menu_buzz_sound;
			break;			

		case K_RIGHTARROW:
			if (s->curvalue < s->maxvalue)
			{
				s->curvalue++;
				sound = menu_move_sound;
			}
			else
				sound = menu_buzz_sound;
			break;			

		default:
			// key not handled
			sound = 0;
			break;
	}

	if ( sound && s->generic.callback )
		s->generic.callback( s, QM_ACTIVATED );
	if ( sound && s->generic.excallback )
		s->generic.excallback( s, QM_ACTIVATED );

	return (sound);
}

/*
=================
Slider_Draw
=================
*/
static void Slider_Draw( menuelement_s *s ) {
	int			x, y;
	int			val;
	int			style;
	float		*color;
	int			button;
	qboolean	focus;
	
	x =	s->generic.x;
	y = s->generic.y;
	val = s->curvalue;
	focus = (s->generic.parent->cursor == s->generic.menuPosition);

	if( s->generic.flags & QMF_GRAYED ) {
		color = color_disabled;
		style = UI_SMALLFONT;
	}
	else if( focus ) {
		color  = color_highlight;
		style = UI_SMALLFONT | UI_PULSE;
	}
	else {
		color = color_white;
		style = UI_SMALLFONT;
	}

	// draw label
	ST_DrawString( x - BASEFONT_INDENT, y, s->string, UI_RIGHT|style, color, 1.00 );
	ST_DrawString( x + (SLIDER_RANGE*BASEFONT_INDENT)+BASEFONT_INDENT, y, va("%i", val), UI_LEFT|style, color, 1.00 );

	// draw slider
	trap_R_SetColor( color );
	UI_DrawHandlePic( x, y+2, (SLIDER_RANGE*BASEFONT_INDENT), SLIDER_RANGE, sliderBar );
	trap_R_SetColor( NULL );

	// clamp thumb
	if( s->maxvalue > s->minvalue )	{
		s->range = ( s->curvalue - s->minvalue ) / ( float ) ( s->maxvalue - s->minvalue );
		if( s->range < 0 ) {
			s->range = 0;
		}
		else if( s->range > 1) {
			s->range = 1;
		}
	}
	else {
		s->range = 0;
	}

	// draw thumb
	if( style & UI_PULSE) {
		button = sliderButton_1;
	}
	else {
		button = sliderButton_0;
	}

	UI_DrawHandlePic( (int)(x+(SLIDER_RANGE*BASEFONT_INDENT)*s->range)-(s->range*SLIDER_RANGE), y+2, SLIDER_RANGE, SLIDER_RANGE, button );
}

/*
=================
SpinControl_Init
=================
*/
void SpinControl_Init( menuelement_s *s ) {
	int	len;
	int	l;
	const char* str;

	if (s->string)
		len = strlenru(s->string) * BASEFONT_INDENT;
	else
		len = 0;

	s->generic.left	= s->generic.x - BASEFONT_INDENT - len;

	len = s->numitems = 0;
	while ( (str = s->itemnames[s->numitems]) != 0 )
	{
		l = strlenru(str);

		if (l > len)
			len = l;

		s->numitems++;
	}		

	s->generic.top	  =	s->generic.y;
	s->generic.right  =	s->generic.x + (len+1)*BASEFONT_INDENT;
	s->generic.bottom =	s->generic.y + BASEFONT_HEIGHT;
}

/*
=================
SpinControl_Key
=================
*/
static sfxHandle_t SpinControl_Key( menuelement_s *s, int key )
{
	sfxHandle_t	sound;

	sound = 0;
	switch (key)
	{
		case K_MOUSE1:
			s->curvalue++;
			if (s->curvalue >= s->numitems)
				s->curvalue = 0;
			sound = menu_move_sound;
			break;
		
		case K_LEFTARROW:
			if (s->curvalue > 0)
			{
				s->curvalue--;
				sound = menu_move_sound;
			}
			else
				sound = menu_buzz_sound;
			break;

		case K_RIGHTARROW:
			if (s->curvalue < s->numitems-1)
			{
				s->curvalue++;
				sound = menu_move_sound;
			}
			else
				sound = menu_buzz_sound;
			break;
	}

	if ( sound && s->generic.callback )
		s->generic.callback( s, QM_ACTIVATED );
	if ( sound && s->generic.excallback )
		s->generic.excallback( s, QM_ACTIVATED );

	return (sound);
}

/*
=================
SpinControl_Draw
=================
*/
static void SpinControl_Draw( menuelement_s *s )
{
	float *color;
	int	x,y;
	int	style;
	qboolean focus;

	x = s->generic.x;
	y =	s->generic.y;

	style = UI_SMALLFONT;
	focus = (s->generic.parent->cursor == s->generic.menuPosition);

	if ( s->generic.flags & QMF_GRAYED )
		color = color_disabled;
	else if ( focus )
	{
		color = color_highlight;
		style |= UI_PULSE;
	}
	else if ( s->generic.flags & QMF_BLINK )
	{
		color = color_highlight;
		style |= UI_BLINK;
	}
	else
		color = color_white;

	if ( focus )
	{
		// draw cursor
		ST_DrawChar( x, y, 13, UI_CENTER|UI_BLINK|UI_SMALLFONT, color, 1.00);
	}

	ST_DrawString( x - BASEFONT_INDENT, y, s->string, style|UI_RIGHT, color, 1.00 );
	ST_DrawString( x + BASEFONT_INDENT, y, s->itemnames[s->curvalue], style|UI_LEFT, color, 1.00 );
}

/*
=================
ScrollList_Init
=================
*/
void ScrollList_Init( menuelement_s *l )
{
	int		w;

	l->oldvalue = 0;
	l->curvalue = 0;
	l->top      = 0;

	if( !l->columns ) {
		l->columns = 1;
	}

	if(l->generic.style <= 1){
		w = (l->width * l->columns) * (BASEFONT_INDENT*l->size);
		}
	if(l->generic.style == 2){
		w = (l->width * l->columns) * (BASEFONT_INDENT);
	}

	l->generic.left   =	l->generic.x;
	l->generic.top    = l->generic.y;	
	l->generic.right  =	l->generic.x + w;
	if(l->generic.style <= 1){
		l->generic.bottom =	l->generic.y + l->height * (BASEFONT_HEIGHT*l->size);
		}
		if(l->generic.style == 2){
		l->generic.bottom =	l->generic.y + l->height * (BASEFONT_INDENT*l->width);
	}

	if( l->generic.flags & QMF_CENTER_JUSTIFY ) {
		l->generic.left -= w / 2;
		l->generic.right -= w / 2;
	}
}

/*
=================
ScrollList_Key
=================
*/
sfxHandle_t ScrollList_Key( menuelement_s *l, int key ) {
	static int clicktime = 0;
	int	x;
	int	y;
	int	w;
	int	i;
	int	j;
	int	c;
	int	cursorx;
	int	cursory;
	int	column;
	int	index;
	int clickdelay;
	int modifier;

	if(l->generic.style <= 1){
		modifier = l->height*(BASEFONT_HEIGHT*l->size);
	} else if(l->generic.style == 2){
		modifier = l->height * (BASEFONT_INDENT*l->width);
	}

	switch (key) {
		case K_MOUSE1:
			if (l->generic.flags & QMF_HASMOUSEFOCUS) {
				// check scroll region
				x = l->generic.x;
				y = l->generic.y;

				if(l->generic.style <= 1){
					w = ( l->width * l->columns) * (BASEFONT_INDENT*l->size);
				} else if(l->generic.style == 2){
					w = ( l->width * l->columns) * (BASEFONT_INDENT);
				}
				
				if (UI_CursorInRect( x, y, w, modifier )) {
					if(l->generic.style <= 1){
						cursorx = (uis.cursorx - x)/(BASEFONT_INDENT*l->size);
						cursory = (uis.cursory - y)/(BASEFONT_HEIGHT*l->size);
					} else if(l->generic.style == 2){
						cursorx = (uis.cursorx - x)/(BASEFONT_INDENT);
						cursory = (uis.cursory - y)/(BASEFONT_INDENT*l->width);
					}
					column = cursorx / l->width;
					index = (cursory * l->columns) + column;
					if (l->top + index < l->numitems) {
						l->oldvalue = l->curvalue;
						l->curvalue = l->top + index;

						clickdelay = uis.realtime - clicktime;
						clicktime = uis.realtime;
						if (l->oldvalue != l->curvalue) {
							if (l->generic.callback) {
								l->generic.callback( l, QM_GOTFOCUS );
							}
							return (menu_move_sound);
						} else {
							// double click
							if ((clickdelay < 350) && !(l->generic.flags & (QMF_GRAYED|QMF_INACTIVE))) {
								return (Menu_ActivateItem( l->generic.parent, &l->generic ));
							}
						}
					}
				}

				// absorbed, silent sound effect
				return (menu_null_sound);
			}
			break;

		case K_MWHEELUP:
			if(l->columns <= 1){
				return menu_null_sound;
			}
			if(l->top - l->columns >= 0){
				l->top -= l->columns;
			} else {
				return menu_buzz_sound;
			}

			if( l->generic.callback ) {
				l->generic.callback( l, QM_GOTFOCUS );
			}

			return menu_move_sound;

		case K_MWHEELDOWN:
			if(l->columns <= 1){
				return menu_null_sound;
			}
			if(l->top + (l->columns * l->height) < l->numitems){
				l->top += l->columns;
			} else {
				return menu_buzz_sound;
			}

			if( l->generic.callback ) {
				l->generic.callback( l, QM_GOTFOCUS );
			}

			return menu_move_sound;
	}

	// cycle look for ascii key inside list items
	if ( !Q_isprint( key ) )
		return (0);

	// force to lower for case insensitive compare
	if ( Q_isupper( key ) ) {
		key -= 'A' - 'a';
	}

	// iterate list items
	for (i=1; i<=l->numitems; i++) {
		j = (l->curvalue + i) % l->numitems;
		c = l->itemnames[j][0];
		if (Q_isupper( c )) {
			c -= 'A' - 'a';
		}

		if (c == key) {
			// set current item, mimic windows listbox scroll behavior
			if (j < l->top) {
				l->top = j;
			} else if (j > l->top+l->height-1) {
				l->top = (j+1) - l->height;
			}
			
			if (l->curvalue != j) {
				l->oldvalue = l->curvalue;
				l->curvalue = j;
				if (l->generic.callback)
					l->generic.callback( l, QM_GOTFOCUS );
				return ( menu_move_sound );			
			}

			return (menu_buzz_sound);
		}
	}

	return (menu_buzz_sound);
}

/*
=================
ScrollList_Draw
=================
*/
void ScrollList_Draw( menuelement_s *l ) {
	int			x;
	int			u;
	int			y;
	int			i;
	int			base;
	int			column;
	float*		color;
	qboolean	hasfocus;
	int			style;
	item_t		*it;
	const char	*info;
	char		pic[MAX_QPATH];
	float		select_x = 0;
	float		select_y = 0;
	int			select_i = 0;

	hasfocus = (l->generic.parent->cursor == l->generic.menuPosition);

	x =	l->generic.x;
	
	for (column = 0; column < l->columns; column++) {
	    y = l->generic.y;
	    // Calculate the base index using the top variable
	    for (base = 0; base < l->height; base++) {
	        // Calculate the index based on the column and row, offset by the top variable
	        i = (base * l->columns + column) + l->top;
	        // Check if the calculated index is within the number of items
	        if (i >= l->numitems)
	            break;
			
			color = l->color;
	        style = UI_LEFT | UI_SMALLFONT;
	        if (l->generic.flags & QMF_CENTER_JUSTIFY) {
	            style |= UI_CENTER;
	        }

			if(l->generic.style <= 0){
				ST_DrawString(x,y,l->itemnames[i],style,color, l->size );
			}

			if(l->generic.style == 1){
				ST_DrawString(x+BASEFONT_HEIGHT*l->size,y,l->itemnames[i],style,color, l->size );
				l->generic.shader = trap_R_RegisterShaderNoMip( va("%s/%s", l->string, l->itemnames[i]) );
				if(l->generic.shader){
					UI_DrawHandlePic( x, y, BASEFONT_HEIGHT*l->size, BASEFONT_HEIGHT*l->size, trap_R_RegisterShaderNoMip( va("%s/%s", l->string, l->itemnames[i]) ) );
				}
				l->generic.model = trap_R_RegisterModel( va("%s/%s", l->string, l->itemnames[i]) );
				if(l->generic.model){
					UI_DrawModelElement( x, y, BASEFONT_HEIGHT*l->size, BASEFONT_HEIGHT*l->size, va("%s/%s", l->string, l->itemnames[i]), l->corner );
				}
				if(!l->generic.shader && !l->generic.model){
					info = UI_GetBotInfoByName( l->itemnames[i] );
					UI_ServerPlayerIcon( Info_ValueForKey( info, "model" ), pic, MAX_QPATH );
					l->generic.shader = trap_R_RegisterShaderNoMip( pic );
					if(l->generic.shader){
						UI_DrawHandlePic( x, y, BASEFONT_HEIGHT*l->size, BASEFONT_HEIGHT*l->size, trap_R_RegisterShaderNoMip( pic ));
					}
				}
				it = UI_FindItem(l->itemnames[i]);
				if(it->classname && it->icon && !l->generic.model && !l->generic.shader){
					UI_DrawHandlePic( x, y, BASEFONT_HEIGHT*l->size, BASEFONT_HEIGHT*l->size, trap_R_RegisterShaderNoMip( it->icon ) );
				}
				if(!it->classname){
					it = UI_FindItemClassname(l->itemnames[i]);
					if(it->classname && !l->generic.model && !l->generic.shader){
						l->generic.model = trap_R_RegisterModel( it->world_model );
						if(l->generic.model){
							UI_DrawModelElement( x, y, BASEFONT_HEIGHT*l->size, BASEFONT_HEIGHT*l->size, it->world_model, l->corner );
						}
					}
				}
				if(!l->generic.shader && !l->generic.model && !it){
					UI_DrawPictureElement( x, y, BASEFONT_HEIGHT*l->size, BASEFONT_HEIGHT*l->size, va("%s/%s", l->string, l->itemnames[i]) );
				}
			}

			if(l->generic.style == 2){
				l->generic.shader = trap_R_RegisterShaderNoMip( va("%s/%s", l->string, l->itemnames[i]) );
				if(l->generic.shader){
					UI_DrawHandlePic( x, y, BASEFONT_INDENT*l->width, BASEFONT_INDENT*l->width, trap_R_RegisterShaderNoMip( va("%s/%s", l->string, l->itemnames[i]) ) );
				}
				l->generic.model = trap_R_RegisterModel( va("%s/%s", l->string, l->itemnames[i]) );
				if(l->generic.model){
					UI_DrawModelElement( x, y, (float)(BASEFONT_INDENT*l->width), (float)(BASEFONT_INDENT*l->width), va("%s/%s", l->string, l->itemnames[i]), l->corner );
				}
				if(!l->generic.shader && !l->generic.model){
					info = UI_GetBotInfoByName( l->itemnames[i] );
					UI_ServerPlayerIcon( Info_ValueForKey( info, "model" ), pic, MAX_QPATH );
					l->generic.shader = trap_R_RegisterShaderNoMip( pic );
					if(l->generic.shader){
						UI_DrawHandlePic( x, y, BASEFONT_INDENT*l->width, BASEFONT_INDENT*l->width, trap_R_RegisterShaderNoMip( pic ));
					}
				}
				it = UI_FindItem(l->itemnames[i]);
				if(it->classname && it->icon && !l->generic.model && !l->generic.shader){
					UI_DrawHandlePic( x, y, BASEFONT_INDENT*l->width, BASEFONT_INDENT*l->width, trap_R_RegisterShaderNoMip( it->icon ) );
				}
				if(!it->classname){
					it = UI_FindItemClassname(l->itemnames[i]);
					if(it->classname && !l->generic.model && !l->generic.shader){
						l->generic.model = trap_R_RegisterModel( it->world_model );
						if(l->generic.model){
							UI_DrawModelElement( x, y, (float)(BASEFONT_INDENT*l->width), (float)(BASEFONT_INDENT*l->width), it->world_model, l->corner );
						}
					}
				}
				if(!l->generic.shader && !l->generic.model && !it){
					UI_DrawPictureElement( x, y, (float)(BASEFONT_INDENT*l->width), (float)(BASEFONT_INDENT*l->width), va("%s/%s", l->string, l->itemnames[i]) );
				}
				if(UI_CursorInRect( x, y, BASEFONT_INDENT*l->width, BASEFONT_INDENT*l->width) && hasfocus){
					select_x = x;
					select_y = y;
					select_i = i;
				}
			}

	        if (i == l->curvalue) {
	            u = x;
	            if (l->generic.flags & QMF_CENTER_JUSTIFY) {
	                if (l->generic.style <= 1) {
	                    u -= (l->width * (BASEFONT_INDENT * l->size)) / 2 + 1;
	                }
	                if (l->generic.style == 2) {
	                    u -= (l->width * (BASEFONT_INDENT)) / 2 + 1;
	                }
	            }
	            if (l->generic.style <= 1) {
	                UI_FillRect(u, y, (l->width * BASEFONT_INDENT) * l->size, (BASEFONT_HEIGHT) * l->size, color_select_bluo);
	            }
	            if (l->generic.style == 2) {
	                UI_FillRect(u, y, (l->width * BASEFONT_INDENT), (l->width * BASEFONT_INDENT), color_select_bluo);
					UI_FillRect(u, y+(l->width * BASEFONT_INDENT)-2, (l->width * BASEFONT_INDENT), 2, color_bluo);
	            }
	        }

			if(l->generic.style <= 1){
				y += (BASEFONT_HEIGHT*l->size);
			}

			if(l->generic.style == 2){
				y += (l->width*BASEFONT_INDENT);
			}
		}

		if(l->generic.style <= 1){
			x += l->width * (BASEFONT_INDENT*l->size);
		}

		if(l->generic.style == 2){
			x += l->width * (BASEFONT_INDENT);
		}
	}

	if(l->generic.style == 2 && hasfocus && UI_CursorInRect( select_x, select_y, (BASEFONT_INDENT*l->width), BASEFONT_INDENT*l->width)){
		if(l->itemnames[select_i]){
			float wordsize = (BASEFONT_HEIGHT*l->size);
			select_x += wordsize;
			select_y -= wordsize*1.45;
			UI_DrawRoundedRect(select_x-wordsize, select_y-4, (strlen(l->itemnames[select_i])*(BASEFONT_HEIGHT*l->size)+wordsize*2), (BASEFONT_HEIGHT*l->size)+8, 5, color_dim);
			ST_DrawString( select_x, select_y, l->itemnames[select_i], style|UI_DROPSHADOW, color_bluo, l->size );
		}
	}
}

/*
=================
Menu_AddItem
=================
*/
void Menu_AddItem( menuframework_s *menu, menuelement_s *item ) {
	menucommon_s	*itemptr;

	if (menu->nitems >= MAX_MENUITEMS)
		trap_Error ("Menu_AddItem: excessive items");

	menu->items[menu->nitems] = item;
	((menucommon_s*)menu->items[menu->nitems])->parent        = menu;
	((menucommon_s*)menu->items[menu->nitems])->menuPosition  = menu->nitems;
	((menucommon_s*)menu->items[menu->nitems])->flags        &= ~QMF_HASMOUSEFOCUS;

	if(!item->size){
		item->size = 1.00;
	}

	// perform any item specific initializations
	itemptr = (menucommon_s*)item;
	if (!(itemptr->flags & QMF_NODEFAULTINIT)) {
		switch (itemptr->type)  {
			case MTYPE_ACTION:
				Action_Init((menuelement_s*)item);
				break;

			case MTYPE_FIELD:
				MenuField_Init((menuelement_s*)item);
				break;

			case MTYPE_SPINCONTROL:
				SpinControl_Init((menuelement_s*)item);
				break;

			case MTYPE_RADIOBUTTON:
				RadioButton_Init((menuelement_s*)item);
				break;

			case MTYPE_SLIDER:
				Slider_Init((menuelement_s*)item);
				break;

			case MTYPE_BITMAP:
				Bitmap_Init((menuelement_s*)item);
				break;

			case MTYPE_SCROLLLIST:
				ScrollList_Init((menuelement_s*)item);
				break;

			case MTYPE_PTEXT:
				PText_Init((menuelement_s*)item);
				break;

			case MTYPE_TEXT:
				BText_Init((menuelement_s*)item);
				break;

			default:
				trap_Error( va("Menu_Init: unknown type %d", itemptr->type) );
		}
	}

	menu->nitems++;
}

/*
=================
Menu_CursorMoved
=================
*/
void Menu_CursorMoved( menuframework_s *m )
{
	void (*callback)( void *self, int notification );
	
	if (m->cursor_prev == m->cursor)
		return;

	if (m->cursor_prev >= 0 && m->cursor_prev < m->nitems)
	{
		callback = ((menucommon_s*)(m->items[m->cursor_prev]))->callback;
		if (callback)
			callback(m->items[m->cursor_prev],QM_LOSTFOCUS);
	}
	
	if (m->cursor >= 0 && m->cursor < m->nitems)
	{
		callback = ((menucommon_s*)(m->items[m->cursor]))->callback;
		if (callback)
			callback(m->items[m->cursor],QM_GOTFOCUS);
	}
}

/*
=================
Menu_SetCursor
=================
*/
void Menu_SetCursor( menuframework_s *m, int cursor )
{
	if (((menucommon_s*)(m->items[cursor]))->flags & (QMF_GRAYED|QMF_INACTIVE))
	{
		// cursor can't go there
		return;
	}

	m->cursor_prev = m->cursor;
	m->cursor      = cursor;

	Menu_CursorMoved( m );
}

/*
=================
Menu_SetCursorToItem
=================
*/
void Menu_SetCursorToItem( menuframework_s *m, void* ptr )
{
	int	i;

	for (i=0; i<m->nitems; i++)
	{
		if (m->items[i] == ptr)
		{
			Menu_SetCursor( m, i );
			return;
		}
	}
}

/*
** Menu_AdjustCursor
**
** This function takes the given menu, the direction, and attempts
** to adjust the menu's cursor so that it's at the next available
** slot.
*/
void Menu_AdjustCursor( menuframework_s *m, int dir ) {
	menucommon_s	*item = NULL;
	qboolean		wrapped = qfalse;

wrap:
	while ( m->cursor >= 0 && m->cursor < m->nitems ) {
		item = ( menucommon_s * ) m->items[m->cursor];
		if (( item->flags & (QMF_GRAYED|QMF_MOUSEONLY|QMF_INACTIVE) ) ) {
			m->cursor += dir;
		}
		else {
			break;
		}
	}

	if ( dir == 1 ) {
		if ( m->cursor >= m->nitems ) {
			if ( wrapped ) {
				m->cursor = m->cursor_prev;
				return;
			}
			m->cursor = 0;
			wrapped = qtrue;
			goto wrap;
			m->cursor = m->cursor_prev;
		}
	}
	else {
		if ( m->cursor < 0 ) {
			if ( wrapped ) {
				m->cursor = m->cursor_prev;
				return;
			}
			m->cursor = m->nitems - 1;
			wrapped = qtrue;
			goto wrap;
			m->cursor = m->cursor_prev;
		}
	}
}

/*
=================
Menu_Draw
=================
*/
void Menu_Draw( menuframework_s *menu ) {
	int				i;
	menucommon_s	*itemptr;

	// draw menu
	for (i=0; i<menu->nitems; i++) {
		itemptr = (menucommon_s*)menu->items[i];

		if (itemptr->flags & QMF_HIDDEN)
			continue;

		if (itemptr->ownerdraw) {
			// total subclassing, owner draws everything
			itemptr->ownerdraw( itemptr );
		} else {
			switch (itemptr->type) {	
				case MTYPE_RADIOBUTTON:
					RadioButton_Draw( (menuelement_s*)itemptr );
					break;

				case MTYPE_FIELD:
					MenuField_Draw( (menuelement_s*)itemptr );
					break;
		
				case MTYPE_SLIDER:
					Slider_Draw( (menuelement_s*)itemptr );
					break;
 
				case MTYPE_SPINCONTROL:
					SpinControl_Draw( (menuelement_s*)itemptr );
					break;
		
				case MTYPE_ACTION:
					Action_Draw( (menuelement_s*)itemptr );
					break;
		
				case MTYPE_BITMAP:
					Bitmap_Draw( (menuelement_s*)itemptr );
					break;

				case MTYPE_SCROLLLIST:
					ScrollList_Draw( (menuelement_s*)itemptr );
					break;
				
				case MTYPE_PTEXT:
					PText_Draw( (menuelement_s*)itemptr );
					break;

				case MTYPE_TEXT:
					Text_Draw( (menuelement_s*)itemptr );
					break;

				default:
					trap_Error( va("Menu_Draw: unknown type %d", itemptr->type) );
			}
		}
		if( uis.debug ) {
			int	x;
			int	y;
			int	w;
			int	h;

			if( !( itemptr->flags & QMF_INACTIVE ) ) {
				x = itemptr->left;
				y = itemptr->top;
				w = itemptr->right - itemptr->left + 1;
				h =	itemptr->bottom - itemptr->top + 1;

				if (itemptr->flags & QMF_HASMOUSEFOCUS) {
					UI_DrawRect(x, y, w, h, colorYellow );
				}
				else {
					UI_DrawRect(x, y, w, h, colorWhite );
				}
			}
		}
	}

	itemptr = Menu_ItemAtCursor( menu );
	if ( itemptr && itemptr->statusbar)
		itemptr->statusbar( ( void * ) itemptr );
}

/*
=================
Menu_ItemAtCursor
=================
*/
void *Menu_ItemAtCursor( menuframework_s *m )
{
	if ( m->cursor < 0 || m->cursor >= m->nitems )
		return 0;

	return m->items[m->cursor];
}

/*
=================
Menu_ActivateItem
=================
*/
sfxHandle_t Menu_ActivateItem( menuframework_s *s, menucommon_s* item ) {
	if ( item->callback ) {
		item->callback( item, QM_ACTIVATED );
		if( !( item->flags & QMF_SILENT ) ) {
			return menu_move_sound;
		}
	}

	if ( item->excallback ) {
		item->excallback( item, QM_ACTIVATED );
		if( !( item->flags & QMF_SILENT ) ) {
			return menu_move_sound;
		}
	}

	return 0;
}

/*
=================
Menu_DefaultKey
=================
*/
sfxHandle_t Menu_DefaultKey( menuframework_s *m, int key )
{
	sfxHandle_t		sound = 0;
	menucommon_s	*item;
	int				cursor_prev;
	menuelement_s* b;
	
	// menu system keys
	switch ( key )
	{
		case K_MOUSE2:
		case K_ESCAPE:
			UI_PopMenu();
			return menu_out_sound;
	}

	if (!m || !m->nitems)
		return 0;

	// route key stimulus to widget
	item = Menu_ItemAtCursor( m );
	b = (menuelement_s*)item;
	if (item && (item->flags & (QMF_HASMOUSEFOCUS)) && !(item->flags & (QMF_GRAYED|QMF_INACTIVE))) {
		switch (item->type) {
			case MTYPE_SPINCONTROL:
				sound = SpinControl_Key( (menuelement_s*)item, key );
				break;

			case MTYPE_RADIOBUTTON:
				sound = RadioButton_Key( (menuelement_s*)item, key );
				break;

			case MTYPE_SLIDER:
				sound = Slider_Key( (menuelement_s*)item, key );
				break;

			case MTYPE_SCROLLLIST:
				sound = ScrollList_Key( (menuelement_s*)item, key );
				break;

			case MTYPE_FIELD:
				sound = MenuField_Key( (menuelement_s*)item, &key );
				item->callback( item, QM_ACTIVATED );
				if(item->excallback){
					item->excallback( item, QM_ACTIVATED );
				}
				break;
		}

		if (sound) {
			// key was handled
			return sound;		
		}
	}

	// default handling
	switch ( key ) {
		case K_F11:
			uis.debug ^= 1;
			break;

		case K_F12:
			trap_Cmd_ExecuteText(EXEC_APPEND, "screenshotJPEG\n");
			break;

		case K_UPARROW:
			cursor_prev    = m->cursor;
			m->cursor_prev = m->cursor;
			m->cursor--;
			Menu_AdjustCursor( m, -1 );
			if ( cursor_prev != m->cursor ) {
				Menu_CursorMoved( m );
				sound = menu_move_sound;
			}
			break;

		case K_TAB:
		case K_DOWNARROW:
			cursor_prev    = m->cursor;
			m->cursor_prev = m->cursor;
			m->cursor++;
			Menu_AdjustCursor( m, 1 );
			if ( cursor_prev != m->cursor ) {
				Menu_CursorMoved( m );
				sound = menu_move_sound;
			}
			break;

		case K_MOUSE1:
			if (item)
				if ((item->flags & QMF_HASMOUSEFOCUS) && !(item->flags & (QMF_GRAYED|QMF_INACTIVE)))
					return (Menu_ActivateItem( m, item ));
			break;

		case K_ENTER:
			if (item)
				if (!(item->flags & (QMF_MOUSEONLY|QMF_GRAYED|QMF_INACTIVE)))
					return (Menu_ActivateItem( m, item ));
			break;
	}

	return sound;
}

/*
=================
Menu_Cache
=================
*/
void Menu_Cache( void ) {
	int i;
	uis.cursor          = trap_R_RegisterShaderNoMip( "menu/assets/3_cursor2" );
	uis.corner          = trap_R_RegisterShaderNoMip( "menu/corner" );
	uis.rb_on           = trap_R_RegisterShaderNoMip( "menu/assets/switch_on" );
	uis.rb_off          = trap_R_RegisterShaderNoMip( "menu/assets/switch_off" );
	uis.whiteShader 	= trap_R_RegisterShaderNoMip( "white" );
	uis.menuBlack		= trap_R_RegisterShaderNoMip( "menu/assets/blacktrans" );
	uis.menuWallpapers 	= trap_R_RegisterShaderNoMip( "menu/animbg" );
	uis.menuLoadingIcon = trap_R_RegisterShaderNoMip( "menu/assets/loading" );
	menu_in_sound		= trap_S_RegisterSound( "sound/misc/menu1.wav", qfalse );
	menu_move_sound		= trap_S_RegisterSound( "sound/misc/menu2.wav", qfalse );
	menu_out_sound		= trap_S_RegisterSound( "sound/misc/menu3.wav", qfalse );
	menu_buzz_sound		= trap_S_RegisterSound( "sound/misc/menu4.wav", qfalse );
	weaponChangeSound	= trap_S_RegisterSound( "sound/weapons/change.wav", qfalse );
	menu_null_sound 	= -1;
	sliderBar 			= trap_R_RegisterShaderNoMip( "menu/assets/slider2" );
	sliderButton_0 		= trap_R_RegisterShaderNoMip( "menu/assets/sliderbutt_0" );
	sliderButton_1 		= trap_R_RegisterShaderNoMip( "menu/assets/sliderbutt_1" );
}

/*
===================
MField_Draw

Handles horizontal scrolling and cursor blinking
x, y, are in pixels
===================
*/
void MField_Draw( mfield_t *edit, int x, int y, int style, vec4_t color, float size ) {
	int		len;
	int		drawLen;
	int		prestep;
	char	str[MAX_STRING_CHARS];

	drawLen = edit->widthInChars;
	len     = strlen( edit->buffer ) + 1;

	// guarantee that cursor will be visible
	if ( len <= drawLen ) {
		prestep = 0;
	} else {
		if ( edit->scroll + drawLen > len ) {
			edit->scroll = len - drawLen;
			if ( edit->scroll < 0 ) {
				edit->scroll = 0;
			}
		}
		prestep = edit->scroll;
	}

	if ( prestep + drawLen > len ) {
		drawLen = len - prestep;
	}

	// extract <drawLen> characters from the field at <prestep>
	if ( drawLen >= MAX_STRING_CHARS )
		trap_Error( "drawLen >= MAX_STRING_CHARS" );
	memcpy( str, edit->buffer + prestep, drawLen );
	str[ drawLen ] = 0;

	ST_DrawString( x, y, str, style, color, size );

	// draw the cursor
	if (!(style & UI_PULSE)) {
		return;
	}

	style &= ~UI_PULSE;
	style |= UI_BLINK;

	if (style & UI_CENTER) {
		len = strlen(str);
		x = x - len*BASEFONT_INDENT/2;
	} else if (style & UI_RIGHT) {
		len = strlen(str);
		x = x - len*BASEFONT_INDENT;
	}
	
	ST_DrawChar( x + ( edit->cursor - prestep ) * BASEFONT_INDENT, y, 10, style & ~(UI_CENTER|UI_RIGHT), color, 1.00 );
}

/*
================
MField_Paste
================
*/
void MField_Paste( mfield_t *edit ) {
	char	pasteBuffer[64];
	int		pasteLen, i;

	trap_GetClipboardData( pasteBuffer, 64 );

	// send as if typed, so insert / overstrike works properly
	pasteLen = strlen( pasteBuffer );
	for ( i = 0 ; i < pasteLen ; i++ ) {
		MField_CharEvent( edit, pasteBuffer[i] );
	}
}

/*
=================
MField_KeyDownEvent

Performs the basic line editing functions for the console,
in-game talk, and menu fields

Key events are used for non-printable characters, others are gotten from char events.
=================
*/
void MField_KeyDownEvent( mfield_t *edit, int key ) {
	int		len;

	// shift-insert is paste
	if ( ( ( key == K_INS ) || ( key == K_KP_INS ) ) && trap_Key_IsDown( K_SHIFT ) ) {
		MField_Paste( edit );
		return;
	}

	len = strlen( edit->buffer );

	if ( key == K_DEL || key == K_KP_DEL ) {
		if ( edit->cursor < len ) {
			memmove( edit->buffer + edit->cursor, 
				edit->buffer + edit->cursor + 1, len - edit->cursor );
		}
		return;
	}

	if ( key == K_RIGHTARROW ) {
		if ( edit->cursor < len ) {
			edit->cursor++;
		}
		if ( edit->cursor >= edit->scroll + edit->widthInChars && edit->cursor <= len ) {
			edit->scroll++;
		}
		return;
	}

	if ( key == K_LEFTARROW ) {
		if ( edit->cursor > 0 ) {
			edit->cursor--;
		}
		if ( edit->cursor < edit->scroll ) {
			edit->scroll--;
		}
		return;
	}

	if ( key == K_INS || key == K_KP_INS ) {
		trap_Key_SetOverstrikeMode( !trap_Key_GetOverstrikeMode() );
		return;
	}
}

/*
==================
MField_CharEvent
==================
*/
void MField_CharEvent( mfield_t *edit, int ch ) {
	int		len;

	if ( ch == 'v' - 'a' + 1 ) {	// ctrl-v is paste
		MField_Paste( edit );
		return;
	}

	if ( ch == 'c' - 'a' + 1 ) {	// ctrl-c clears the field
		MField_Clear( edit );
		return;
	}

	len = strlen( edit->buffer );

	if ( ch == 'h' - 'a' + 1 )	{	// ctrl-h is backspace
		if ( edit->cursor > 0 ) {
			memmove( edit->buffer + edit->cursor - 1, 
				edit->buffer + edit->cursor, len + 1 - edit->cursor );
			edit->cursor--;
			if ( edit->cursor < edit->scroll )
			{
				edit->scroll--;
			}
		}
		return;
	}

	if ( ch == 'a' - 'a' + 1 ) {	// ctrl-a is home
		edit->cursor = 0;
		edit->scroll = 0;
		return;
	}

	if ( ch == 'e' - 'a' + 1 ) {	// ctrl-e is end
		edit->cursor = len;
		edit->scroll = edit->cursor - edit->widthInChars + 1;
		if (edit->scroll < 0)
			edit->scroll = 0;
		return;
	}

	//
	// ignore any other non printable chars
	//
	if ( ch == -48 ) {
		return;
	}

	if ( !trap_Key_GetOverstrikeMode() ) {	
		if ((edit->cursor == MAX_EDIT_LINE - 1) || (edit->maxchars && edit->cursor >= edit->maxchars))
			return;
	} else {
		// insert mode
		if (( len == MAX_EDIT_LINE - 1 ) || (edit->maxchars && len >= edit->maxchars))
			return;
		memmove( edit->buffer + edit->cursor + 1, edit->buffer + edit->cursor, len + 1 - edit->cursor );
	}

	edit->buffer[edit->cursor] = ch;
	if (!edit->maxchars || edit->cursor < edit->maxchars-1)
		edit->cursor++;

	if ( edit->cursor >= edit->widthInChars )
	{
		edit->scroll++;
	}

	if ( edit->cursor == len + 1) {
		edit->buffer[edit->cursor] = 0;
	}
}

/*
==================
MField_Clear
==================
*/
void MField_Clear( mfield_t *edit ) {
	edit->buffer[0] = 0;
	edit->cursor = 0;
	edit->scroll = 0;
}

/*
==================
MenuField_Init
==================
*/
void MenuField_Init( menuelement_s* m ) {
	int	l;
	int	w;
	int	h;

	MField_Clear( &m->field );

	w = BASEFONT_INDENT*m->size;
	h = BASEFONT_HEIGHT*m->size;

	if (m->string) {
		l = (strlenru( m->string )+1) * w;		
	} else {
		l = 0;
	}

	m->generic.left   = m->generic.x - l;
	m->generic.top    = m->generic.y;
	m->generic.right  = m->generic.x + w + m->field.widthInChars*w;
	m->generic.bottom = m->generic.y + h;
}

/*
==================
MenuField_Draw
==================
*/
void MenuField_Draw( menuelement_s *f ) {
	int		x;
	int		y;
	int		w;
	int		h;
	int		style;
	qboolean focus;
	float	*color;

	x =	f->generic.x;
	y =	f->generic.y;
	w = BASEFONT_INDENT * f->size;
	h = BASEFONT_HEIGHT * f->size;
	style = UI_SMALLFONT;

	if (Menu_ItemAtCursor( f->generic.parent ) == f) {
		focus = qtrue;
		style |= UI_PULSE;
	} else {
		focus = qfalse;
	}

	if (f->generic.flags & QMF_GRAYED) {
		color = color_disabled;
	} else if (focus) {
		color = color_highlight;
	} else if (!f->color) {
		color = color_white;
	} else {
		color = f->color;
	}

	if (focus){
		// draw cursor
		ST_DrawChar( x, y, 13, UI_CENTER|UI_BLINK|style, color, 1.00);
		f->generic.flags |= QMF_HASMOUSEFOCUS;
	}

	if (f->string) {
		ST_DrawString( x - w, y, f->string, style|UI_RIGHT, color, f->size );
	}

	MField_Draw( &f->field, x + w, y, style, color, f->size );
}

/*
==================
MenuField_Key
==================
*/
sfxHandle_t MenuField_Key( menuelement_s* m, int* key ) {
	int keycode;
	static int lastKeypress = 0;

	keycode = *key;

	switch ( keycode ) {
		case K_ENTER:
		case K_MOUSE1:
			break;

		case K_TAB:
		case K_DOWNARROW:
		case K_UPARROW:
			break;

		default:
			if ( keycode & K_CHAR_FLAG ) {
				keycode &= ~K_CHAR_FLAG;

				if ((m->generic.flags & QMF_UPPERCASE) && Q_islower( keycode ))
					keycode -= 'a' - 'A';
				else if ((m->generic.flags & QMF_LOWERCASE) && Q_isupper( keycode ))
					keycode -= 'A' - 'a';
				else if ((m->generic.flags & QMF_NUMBERSONLY) && Q_isalpha( keycode ))
					return (menu_buzz_sound);

				MField_CharEvent( &m->field, keycode);
			} else {
				MField_KeyDownEvent( &m->field, keycode );
			}
			break;
	}
	lastKeypress = uis.realtime;

	return (0);
}

/*
=================
UI_ServerPlayerIcon
=================
*/
void UI_ServerPlayerIcon( const char *modelAndSkin, char *iconName, int iconNameMaxSize ) {
	char	*skin;
	char	model[MAX_QPATH];

	Q_strncpyz( model, modelAndSkin, sizeof(model));
	skin = strrchr( model, '/' );
	if ( skin ) {
		*skin++ = '\0';
	}
	else {
		skin = "default";
	}

	Com_sprintf(iconName, iconNameMaxSize, "models/players/%s/icon_%s.tga", model, skin );

	if( !trap_R_RegisterShaderNoMip( iconName ) && Q_stricmp( skin, "default" ) != 0 ) {
		Com_sprintf(iconName, iconNameMaxSize, "models/players/%s/icon_default.tga", model );
	}
}

/*
=================
UI_CurrentPlayerTeam
=================
*/
int UI_CurrentPlayerTeam(void) {
	static uiClientState_t	cs;
	static char	info[MAX_INFO_STRING];

	trap_GetClientState(&cs);
	trap_GetConfigString(CS_PLAYERS + cs.clientNum, info, MAX_INFO_STRING);
	return atoi(Info_ValueForKey(info, "t"));
}

/*
=================
UI_ServerGametype
=================
*/
int UI_ServerGametype(void)
{
	char	info[MAX_INFO_STRING];

	trap_GetConfigString( CS_SERVERINFO, info, sizeof(info) );
	return atoi( Info_ValueForKey( info, "g_gametype" ) );
}

#define	CB_COMMAND 		0
#define CB_VARIABLE 	1
#define	CB_FUNC 		2

void UI_FindButtonPic( menuelement_s* e, int pic ) {
	switch( pic ) {
	case AST_BACK:
		e->string = "menu/assets/back_0";
		e->focuspic = "menu/assets/back_1";
		break;
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

void UI_FillList(menuelement_s* e, char* location, char* extension, char* names, int namesSize, char** configlist) {
	int 	i, len;
	char* 	configname;

    e->string = va("%s/icons", location);
    e->numitems = trap_FS_GetFileList(location, extension, names, namesSize);
	e->itemnames = (const char **)configlist;

    if (e->numitems == 0) { 			//Empty folder
        strcpy(names, "Empty");
        e->numitems = 1;
    } else if (e->numitems > 65536) { 	//Limit
        e->numitems = 65536;
    }

    configname = names;
    for (i = 0; i < e->numitems; i++) {
        e->itemnames[i] = configname;

        len = strlen(configname);
        if (!Q_stricmp(configname + len - strlen(extension), extension)) { 	//remove extension
            configname[len - strlen(extension)] = '\0';
        }

        configname += len + 1;
    }
}

void UI_GeneralCallback( void *ptr, int event ) {
	if( event != QM_ACTIVATED ) {
		return;
	}

	switch( ((menucommon_s*)ptr)->excallbacktype ) {
		case CB_COMMAND:
			trap_Cmd_ExecuteText( EXEC_APPEND, ((menucommon_s*)ptr)->cmd );
			break;
	
		case CB_VARIABLE:
			if(((menucommon_s*)ptr)->type == MTYPE_SLIDER){
				trap_Cvar_SetValue(((menucommon_s*)ptr)->var, (float)*((menucommon_s*)ptr)->value / (float)((menucommon_s*)ptr)->mode);
			}
			if(((menucommon_s*)ptr)->type == MTYPE_FIELD){
				trap_Cvar_Set(((menucommon_s*)ptr)->var, ((menucommon_s*)ptr)->buffer);
			}
			if(((menucommon_s*)ptr)->type == MTYPE_RADIOBUTTON){
				if(((menucommon_s*)ptr)->mode == RBT_NORMAL){
					trap_Cvar_SetValue(((menucommon_s*)ptr)->var, (float)*((menucommon_s*)ptr)->value);
				}
				if(((menucommon_s*)ptr)->mode == RBT_INVERSE){
					trap_Cvar_SetValue(((menucommon_s*)ptr)->var, -(float)*((menucommon_s*)ptr)->value);
				}
			}
			break;
	
		case CB_FUNC:
			((menucommon_s*)ptr)->func();
			break;
	}
}

void UI_CreateUI(menuframework_s* menu, menuelement_s* e) {
	int i;

	for(i = 0; i <= OSUI_MAX_ELEMENTS-1; i++){
		if(e[i].generic.type != MTYPE_NULL){
			//ID
			e[i].generic.id = i;

			//Pointers
			e[i].generic.buffer = e[i].field.buffer;
			e[i].generic.value = &e[i].curvalue;

			//Get value
			if(e[i].generic.var){
				if(e[i].generic.type == MTYPE_SLIDER){
					e[i].curvalue = trap_Cvar_VariableValue( e[i].generic.var ) * (float)e[i].generic.mode;
				}
				if(e[i].generic.type == MTYPE_FIELD){
					trap_Cvar_VariableStringBuffer( e[i].generic.var, e[i].field.buffer, sizeof(e[i].field.buffer) );
				}
				if(e[i].generic.type == MTYPE_RADIOBUTTON){
					if(e[i].generic.mode == RBT_NORMAL){
						e[i].curvalue = trap_Cvar_VariableValue( e[i].generic.var );
					}
					if(e[i].generic.mode == RBT_INVERSE){
						e[i].curvalue = -trap_Cvar_VariableValue( e[i].generic.var );
					}
				}
			}

			e[i].generic.flags |= (QMF_PULSEIFFOCUS);
			Menu_AddItem( menu, &e[i] );
		}
	}
}

void UI_CButton(menuelement_s* e, float x, float y, char* text, int style, float size, char* cmd, char* var, void (*func)(void), void (*callback)( void *self, int event ), int callid) {
	e->generic.type					= MTYPE_PTEXT;
	e->generic.x					= x;
	e->generic.y					= y;
	e->size							= size;
	e->string						= text;
	e->generic.callback				= callback;
	e->generic.callid				= callid;
	e->generic.excallback			= UI_GeneralCallback;
	if(cmd){
		e->generic.excallbacktype	= CB_COMMAND;
		e->generic.cmd				= cmd;
	} else if(var){
		e->generic.excallbacktype	= CB_VARIABLE;
		e->generic.var				= var;
	} else if(func){
		e->generic.excallbacktype	= CB_FUNC;
		e->generic.func				= func;
	}
	e->color						= color_white;
	
	if(style == UI_LEFT){
		e->generic.flags			= QMF_LEFT_JUSTIFY;
		e->style					= UI_LEFT;
	}
	if(style == UI_CENTER){
		e->generic.flags			= QMF_CENTER_JUSTIFY;
		e->style					= UI_CENTER;
	}
	if(style == UI_RIGHT){
		e->generic.flags			= QMF_RIGHT_JUSTIFY;
		e->style					= UI_RIGHT;
	}
}

void UI_CSlider(menuelement_s* e, float x, float y, char* text, char* var, float min, float max, float mod, void (*callback)( void *self, int event ), int callid) {
	e->generic.type					= MTYPE_SLIDER;
	e->generic.x					= x;
	e->generic.y					= y;
	e->string						= text;
	e->generic.callback				= callback;
	e->generic.callid				= callid;
	e->generic.excallback			= UI_GeneralCallback;
	e->minvalue		    			= min;
	e->maxvalue		    			= max;
	e->generic.mode		    		= mod;
	if(var){
		e->generic.excallbacktype	= CB_VARIABLE;
		e->generic.var				= var;
	}
	e->color						= color_white;
}

void UI_CRadioButton(menuelement_s* e, float x, float y, char* text, char* var, float mod, void (*callback)( void *self, int event ), int callid) {
	e->generic.type					= MTYPE_RADIOBUTTON;
	e->generic.x					= x;
	e->generic.y					= y;
	e->string						= text;
	e->generic.callback				= callback;
	e->generic.callid				= callid;
	e->generic.excallback			= UI_GeneralCallback;
	e->generic.mode		    		= mod;
	if(var){
		e->generic.excallbacktype	= CB_VARIABLE;
		e->generic.var				= var;
	}
	e->color						= color_white;
}

void UI_CSpinControl(menuelement_s* e, float x, float y, char* text, const char **list, void (*callback)( void *self, int event ), int callid) {
	e->generic.type					= MTYPE_SPINCONTROL;
	e->generic.x					= x;
	e->generic.y					= y;
	e->string						= text;
	e->generic.callback				= callback;
	e->generic.callid				= callid;
	e->itemnames		    		= list;
	e->color						= color_white;
}

void UI_CList(menuelement_s* e, float x, float y, float w, float h, int style, float size, int col, void (*callback)( void *self, int event ), int callid) {
	e->generic.type					= MTYPE_SCROLLLIST;
	e->generic.style				= style;
	e->generic.x					= x;
	e->generic.y					= y;
	e->width						= w;
	e->height						= h;
	e->size							= size;
	e->columns						= col;
	e->generic.callback				= callback;
	e->generic.callid				= callid;
	e->color						= color_white;
}

void UI_CField(menuelement_s* e, float x, float y, char* text, int w, int maxchars, int style, void (*callback)( void *self, int event ), int callid) {
	e->generic.type					= MTYPE_FIELD;
	e->generic.style				= style;
	e->generic.x					= x;
	e->generic.y					= y;
	e->string						= text;
	e->field.widthInChars			= w;
	e->field.maxchars				= maxchars;
	e->generic.callback				= callback;
	e->generic.callid				= callid;
	e->color						= color_white;

	if(style == UI_LEFT){
		e->generic.flags			= QMF_LEFT_JUSTIFY;
		e->style					= UI_LEFT;
	}
	if(style == UI_CENTER){
		e->generic.flags			= QMF_CENTER_JUSTIFY;
		e->style					= UI_CENTER;
	}
	if(style == UI_RIGHT){
		e->generic.flags			= QMF_RIGHT_JUSTIFY;
		e->style					= UI_RIGHT;
	}
}

void UI_CText(menuelement_s* e, float x, float y, char* text, int style, float size) {
	e->generic.type				= MTYPE_TEXT;
	e->generic.x				= x;
	e->generic.y				= y;
	e->size						= size;
	e->string					= text;
	e->color					= color_white;

	if(style == UI_LEFT){
		e->style				= UI_LEFT;
	}
	if(style == UI_CENTER){
		e->style				= UI_CENTER;
	}
	if(style == UI_RIGHT){
		e->style				= UI_RIGHT;
	}
}

void UI_CBitmap(menuelement_s* e, float x, float y, float w, float h, int pic, int flags, char* cmd, char* var, void (*func)(void), void (*callback)( void *self, int event ), int callid) {
	UI_FindButtonPic(e, pic);

	e->generic.type					= MTYPE_BITMAP;
	e->generic.flags				= flags;
	e->generic.callback				= callback;
	e->generic.callid				= callid;
	e->generic.excallback			= UI_GeneralCallback;
	if(cmd){
		e->generic.excallbacktype	= CB_COMMAND;
		e->generic.cmd				= cmd;
	} else if(var){
		e->generic.excallbacktype	= CB_VARIABLE;
		e->generic.var				= var;
	} else if(func){
		e->generic.excallbacktype	= CB_FUNC;
		e->generic.func				= func;
	}
	e->generic.x					= x;
	e->generic.y					= y;
	e->width						= w;
	e->height						= h;
}

void UI_CPicture(menuelement_s* e, float x, float y, float w, float h, int pic, int flags, char* cmd, char* var, void (*func)(void), void (*callback)( void *self, int event ), int callid) {
	UI_FindButtonPic(e, pic);

	e->generic.type					= MTYPE_BITMAP;
	e->generic.flags				= flags;
	e->generic.callback				= callback;
	e->generic.callid				= callid;
	e->generic.excallback			= UI_GeneralCallback;
	if(cmd){
		e->generic.excallbacktype	= CB_COMMAND;
		e->generic.cmd				= cmd;
	} else if(var){
		e->generic.excallbacktype	= CB_VARIABLE;
		e->generic.var				= var;
	} else if(func){
		e->generic.excallbacktype	= CB_FUNC;
		e->generic.func				= func;
	}
	e->generic.flags				|= QMF_INACTIVE;
	e->generic.x					= x;
	e->generic.y					= y;
	e->width						= w;
	e->height						= h;
}
