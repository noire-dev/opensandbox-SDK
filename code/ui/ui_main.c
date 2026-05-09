// Copyright (C) 2023-2026 Noire's Mod — GPLv2

// TODO: не включать модуль ui.qvm при старте карты
//       никогда не перезагружать ui.qvm, возможно потребуется переделка hunk
//       удалить loading screen, вместо него, рисовать прогресс загрузки прямо в окне запуска карты

// TODO: минимизировать использование q3cfg, перейти на JS
//       оставить только обработчик команд через JS, остальное вырезать

// TODO: добавить элемент Picture, а также добавить элемент List (для файлов, с
//       возможностью выделить файл, клик по выделеному файлу это запуск)

#include "../shared/javascript.h"

shell_s shell;
console_s console;

int vmMain(int command, int arg0, int arg1, int arg2) { // Точка входа движка (функция должна быть первой)
	switch (command) {
	case UI_INIT: return UI_Init();
	case UI_SHUTDOWN: return UI_Shutdown();
	case UI_KEY_EVENT: return UI_KeyEvent(arg0, arg1, arg2);
	case UI_MOUSE_EVENT: return UI_MouseEvent(arg0, arg1);
	case UI_REFRESH: return UI_Refresh();
	case UI_IS_FULLSCREEN: return UI_IsFullscreen();
	case UI_SET_ACTIVE_MENU: return UI_SetActiveMenu(arg0);
	case UI_CONSOLE_COMMAND: return UI_ConsoleCommand();
	case UI_DRAW_CONNECT_SCREEN: return UI_DrawConnectScreen();
	
	case GETVMCONTEXT: VMContext(&vmargs, &vmresult); return 0;
	case VMCALL: VMCall(arg0); return 0;
	default: err("ui.qvm: unknown command"); break;
	}

	return -1;
}

static void UI_OnMapStatus(void) { // Проверка нахождения на карте
	char svinfo[MAX_INFO_STRING];
	uiClientState_t cstate;
	
	trap_GetClientState(&cstate);

	trap_GetConfigString(CS_SERVERINFO, svinfo, MAX_INFO_STRING);
	shell.onMap = (strlen(svinfo) <= 0 && cstate.connState < CA_LOADING) ? qfalse : qtrue;
}

static void UI_ShellInit(void) { // Инициализация Shell
    int i;
    
    memset(&shell, 0, sizeof(shell));
    shell.scale = cvarFloat("shell.global.scale");
    shell.corner = cvarFloat("shell.window.corner");
    UI_OnMapStatus();
    JS_ShellInit();
	for (i = 0; i < SHELL_MAX_WINDOWS; i++) shell.zOrder[i] = i;
}

static void UI_LoadApps(void) { // Кэширует список приложений из JS
    int i;
    
    for(i = 0; i < SHELL_MAX_APPS; i++) {
        shell.appCount = i;
        if(JS_GetApp(i) == NONE) break;
        StringCopy(shell.app[i].name, JS_GetAppName(i), MAX_JS_STRINGSIZE);
        StringCopy(shell.app[i].icon, JS_GetAppIcon(i), MAX_JS_STRINGSIZE);
    }
}

static void UI_HotReload(void) { // Перезапуск JS контекста и повторная инициализация Shell
	trap_Cmd(EXEC_NOW, "js.restart \n");
	UI_Shutdown();
	UI_Init();
}

int UI_Init(void) { // Инициализация UI и загрузка состояния
    fileHandle_t stateFile = 0;
    int stateLen = 0;
    int i;
    
	print("Shell: init... \n");
	VM_RegisterCvars();
	CL_Init();
	UI_ShellInit();
	UI_LoadApps();
	
	stateLen = FS_Open(SHELL_STATE_FILE, &stateFile, FS_READ);
	if(stateFile && stateLen > 0) {
	    print("Shell: loading state... \n");
	    FS_Read(shell.saveState, sizeof(shell.saveState), stateFile);
	    
	    for (i = 0; i < SHELL_MAX_WINDOWS; i++) {
    	    window_s *window = &shell.window[i];
    	    saveState_s *savedWindow = &shell.saveState[i];
    	    
    	    shell.zOrder[i] = savedWindow->zOrder;
    	    JS_LaunchApp(savedWindow->nameID);
    	    
    		window->minimized = savedWindow->minimized;
    		if(i != 1) window->x = savedWindow->x;
    		window->y = savedWindow->y;
    		window->scaleFactor = savedWindow->scaleFactor;
        }
        FS_Close(stateFile);
        print("Shell: state loaded! \n");
	}
	
	return 0;
}

int UI_Shutdown(void) { // Выключение UI и сохранение состояния
    fileHandle_t stateFile = 0;
    int i;
    
    print("Shell: shutdown... \n");
    print("Shell: saving state... \n");
    
    memset(&shell.saveState, 0, sizeof(shell.saveState));
    for (i = 0; i < SHELL_MAX_WINDOWS; i++) {
	    window_s *window = &shell.window[i];
	    saveState_s *savedWindow = &shell.saveState[i];
	    
	    savedWindow->zOrder = shell.zOrder[i];
	    
		if(!window->created) continue;
		savedWindow->minimized = window->minimized;
		StringCopy(savedWindow->nameID, window->nameID, MAX_JS_STRINGSIZE);
		savedWindow->x = window->x;
		savedWindow->y = window->y;
		savedWindow->scaleFactor = window->scaleFactor;
    }
    FS_Open(SHELL_STATE_FILE, &stateFile, FS_WRITE);
    FS_Write(&shell.saveState, sizeof(shell.saveState), stateFile);
    FS_Close(stateFile);
    print("Shell: state saved! \n");
    return 0;
}

static void UI_AdjustToWindow(element_s* element) { // Локальная позиция внутри окна к глобальной позиции
    window_s *window = (window_s *)element->parentWindow;
    int i;
    
	element->x = (element->baseX * window->scaleFactor) + window->x;
	element->y = (element->baseY * window->scaleFactor) + window->y;
	element->w = element->baseW * window->scaleFactor;
	element->h = element->baseH * window->scaleFactor;
	element->scale = element->baseScale * window->scaleFactor;
	element->corner = element->baseCorner * window->scaleFactor;
	element->margin = element->baseMargin * window->scaleFactor;
    if(element->type == ETYPE_LIST || element->type == ETYPE_LISTFILES) {
        element->w = (element->baseW * element->col) * window->scaleFactor;
	    element->h = (element->baseH * element->row) * window->scaleFactor;
	    element->itemW = element->baseW * window->scaleFactor;
	    element->itemH = element->baseH * window->scaleFactor;
	    for(i = 0; i < 4; i++) element->margin4[i] = element->baseMargin4[i] * window->scaleFactor;
    }
	if(element->type == ETYPE_WINDOWBUTTON) {
	    float posX = window->x+window->w;
        float marginX = 6 * shell.scale;
        float buttonX = posX-((element->baseH+marginX)*(element->baseX + 1));

	    element->x = buttonX;
	    element->y = element->baseY + window->y;
	    element->w = element->baseW;
	    element->h = element->baseH;
	    element->scale = element->baseScale * shell.scale;
	    element->corner = element->baseCorner;
    }
}

static void UI_ScaleToWindow(window_s* window) { // Применение масштабирования к окну
	if(window->style & UI_NOSCALE) window->scaleFactor = 1.00;
	window->w = window->baseW * window->scaleFactor;
	window->h = window->baseH * window->scaleFactor;
}

static qboolean UI_ElementIsVisible(element_s* element) { // Проверка видимости элемента
    if(element->x > glconfig.width) return qfalse; // проверка +X
    if(element->y > glconfig.height) return qfalse; // проверка +Y
    if((element->x + element->w) < 0) return qfalse; // проверка -X
    if((element->y + element->h) < 0) return qfalse; // проверка -Y
    return qtrue;
}

static qboolean UI_CursorOnItem(element_s* element) { // Проверка мыши на элементе
    window_s *window = (window_s *)element->parentWindow;
    
	if (shell.cursorX < element->x || shell.cursorY < element->y || shell.cursorX > element->x + element->w || shell.cursorY > element->y + element->h) return qfalse;
	return qtrue;
}

static qboolean UI_ItemFocused(element_s* element) { // Проверка мыши на элементе в окне
    window_s *window = (window_s *)element->parentWindow;
    
    if(shell.focusedWindow != window->id) return qfalse;
    if(window->focusedElement != element->id) return qfalse;
    
	return qtrue;
}

static qboolean UI_CursorInRect(float x, float y, float w, float h) { // Проверка мыши в области
	if (shell.cursorX < x || shell.cursorY < y || shell.cursorX > x + w || shell.cursorY > y + h) return qfalse;
	return qtrue;
}

static float UI_SliderValue(float min, float max, float x, float w) { // Значение слайдера из позиции мыши
	float local_x = (float)shell.cursorX - x;

	if (local_x < 0.0f) local_x = 0.0f;
	if (local_x > w) local_x = w;

	return min + (local_x / w) * (max - min);
}

static void UI_GeneralCallback(element_s* e) { // Обработка активации элемента
	switch (e->type) {
	case ETYPE_CHECKBOX:
		e->value = (e->value > 0) ? 0 : 1;
		if(strlen(e->cvar)) cvarSet(e->cvar, va("%f", e->value));
		break;
	case ETYPE_SLIDER:
		if (UI_CursorOnItem(e)) {
			if (e->mode == EMODE_INT) e->value = (int)(UI_SliderValue(e->min, e->max, e->x + (e->w * 0.5), (e->w * 0.5) - e->margin));
			if (e->mode == EMODE_FLOAT) e->value = UI_SliderValue(e->min, e->max, e->x + (e->w * 0.5), (e->w * 0.5) - e->margin);
		}
		if (strlen(e->cvar)) cvarSet(e->cvar, va("%f", e->value * e->valueMod));
		break;
	case ETYPE_ACTION:
		shell.window[shell.activeWindow].keyCapture = e->id;
		break;
	case ETYPE_SPIN:
		e->value = (e->value < e->optionsCount - 1) ? e->value + 1 : 0;
		if (e->mode == EMODE_NUMBER) cvarSet(e->cvar, va("%f", e->value * e->valueMod));
		else if (e->mode == EMODE_STRING) cvarSet(e->cvar, va("%s", e->options[(int)e->value]));
		break;
	case ETYPE_FIELD:
		shell.window[shell.activeWindow].keyCapture = e->id;
		break;
	case ETYPE_LIST: {
	    int i, j;
    	int currentItem = e->scroll;
    
    	for (i = 0; i < e->row; i++) {
    		for (j = 0; j < e->col; j++) {
                if(currentItem >= shell.appCount) break;
                if(UI_CursorInRect(e->x + (e->itemW * j), e->y + (e->itemH * i), e->itemW, e->itemH)) {
                    e->value = currentItem;
                    break;
                }
    			currentItem += 1;
    		}
    	}
    	break;}
    case ETYPE_LISTFILES: {
        window_s *window = (window_s *)e->parentWindow;
	    int i, j, currentItem = 0;
	    char *file = window->fileList[e->listID].list;
	    char nameWithoutExt[MAX_JS_STRINGSIZE];
	
    	for(i = 0; i < e->scroll; i++) {
    	    if(i >= window->fileList[e->listID].count) break;
    	    file += strlen(file) + 1;
    	}
    
    	for (i = 0; i < e->row; i++) {
    		for (j = 0; j < e->col; j++) {
                if(currentItem >= window->fileList[e->listID].count) break;
                if(UI_CursorInRect(e->x + (e->itemW * j), e->y + (e->itemH * i), e->itemW, e->itemH)) {
                    Q_snprintf(nameWithoutExt, sizeof(nameWithoutExt), "%.*s", (int)(strlen(file) - strlen(window->fileList[e->listID].ext)), file);
                    e->value = currentItem;
                    StringCopy(e->field, nameWithoutExt, MAX_JS_STRINGSIZE);
                    break;
                }
    			currentItem += 1;
        	    file += strlen(file) + 1;
    		}
    	}
    	break;}
	default: break;
	}
}

static void UI_SecondaryCallback(element_s* e) { // Обработка альтернативной активации элемента
    int i;
    
	switch (e->type) {
	case ETYPE_CHECKBOX:
	    if(strlen(e->cvar)) cvarSet(e->cvar, NULL);
		e->value = cvarFloat(e->cvar);
		break;
	case ETYPE_SLIDER:
		if(strlen(e->cvar)) cvarSet(e->cvar, NULL);
		e->value = cvarFloat(e->cvar);
		break;
	case ETYPE_SPIN:
        if(strlen(e->cvar)) cvarSet(e->cvar, NULL);
        if (e->mode == EMODE_NUMBER) {
    		e->value = cvarFloat(e->cvar);
    	} else if (e->mode == EMODE_STRING) {
    		for (i = 0; i < e->optionsCount; i++) {
    			if (!strcmp(cvarString(e->cvar), e->options[i])) e->value = i;
    		}
    	}
		break;
	case ETYPE_FIELD:
	    if(strlen(e->cvar)) cvarSet(e->cvar, NULL);
	    StringCopy(e->field, cvarString(e->cvar), MAX_JS_STRINGSIZE);
		break;
	default: break;
	}
}

static void UI_KeyCallback(element_s* e, int key) { // Обработка клавиш для элемента
    window_s *window = (window_s *)e->parentWindow;
    
	switch (e->type) {
	case ETYPE_LIST:
	    if(key == K_MWHEELUP && e->scroll - e->col >= 0) e->scroll -= e->col;
	    if(key == K_MWHEELDOWN && e->scroll + (e->col * e->row) < shell.appCount) e->scroll += e->col;
    	break;
	case ETYPE_LISTFILES:
	    if(key == K_MWHEELUP && e->scroll - e->col >= 0) e->scroll -= e->col;
	    if(key == K_MWHEELDOWN && e->scroll + (e->col * e->row) < window->fileList[e->listID].count) e->scroll += e->col;
    	break;
	default: break;
	}
}

static void UI_WindowToTop(int windowID) { // Перенос окна на пережний план
    int pos, temp, i;

    if(shell.window[windowID].style & UI_NOZORDER) return;
    for (pos = 0; pos < SHELL_MAX_WINDOWS; pos++) {
        if (shell.zOrder[pos] == windowID) break;
    }
    if (pos == SHELL_MAX_WINDOWS) return;

    temp = shell.zOrder[pos];
    for (i = pos; i < SHELL_MAX_WINDOWS - 1; i++) shell.zOrder[i] = shell.zOrder[i + 1];
    shell.zOrder[SHELL_MAX_WINDOWS - 1] = temp;
}

int UI_KeyEvent(int key, int isDown, int isChar) { // [SAFE] Обработка событий ввода UI
	element_s* element = NULL;
	window_s* activeWindow = NULL;
	window_s* focusedWindow = NULL;
	qboolean keyCapture = qfalse;
	int i;

	if (!isDown) { // Обработка поднятия клавиш
	    if (key == K_MOUSE1) shell.cursorIsDragging = qfalse;
	    return 0;
	}
	
	if (key == K_MOUSE1) { // Активация окна при нажатии мыши
	    shell.cursorIsDragging = qtrue;
	    shell.activeWindow = shell.focusedWindow;
	    if(shell.activeWindow != NONE) UI_WindowToTop(shell.activeWindow);
	}
	
	if(shell.activeWindow != NONE) activeWindow = &shell.window[shell.activeWindow];
	if(shell.focusedWindow != NONE) focusedWindow = &shell.window[shell.focusedWindow];
	
	if(focusedWindow != NULL && !(focusedWindow->style & UI_NOSCALE)) { // Изменения размера окна колесиком мыши
	    if(key == K_MWHEELDOWN && shell.cursorY < focusedWindow->y) {
	        focusedWindow->scaleFactor = clamp(focusedWindow->scaleFactor - 0.050000, 0.25, 4.00);
	        return 0;
	    }
	    if(key == K_MWHEELUP && shell.cursorY < focusedWindow->y) {
	        focusedWindow->scaleFactor = clamp(focusedWindow->scaleFactor + 0.050000, 0.25, 4.00);
	        return 0;
	    }
	}
	
	if (activeWindow != NULL && activeWindow->keyCapture != NONE && key != K_MOUSE1) { // Выбор элемента
		element = &activeWindow->element[activeWindow->keyCapture];
		keyCapture = qtrue;
	} else if(activeWindow != NULL) {
		if(activeWindow->focusedElement != NONE) element = &activeWindow->element[activeWindow->focusedElement];
	}

	if (keyCapture && element->type == ETYPE_ACTION) {
		if (key != K_ESCAPE) {
			for (i = 0; i < MAX_KEYS; i++) {
				if (!strcmp(element->cvar, cgui.binds[i])) trap_Key_SetBinding(i, "");
			}
			trap_Key_SetBinding(key, element->cvar);
			for (i = 0; i < MAX_KEYS; i++) {
			    trap_Key_GetBindingBuf(i, cgui.binds[i], 256);
			    if (!strcmp(element->cvar, cgui.binds[i])) trap_Key_KeynumToStringBuf(i, element->bind, 256);
			}
			activeWindow->keyCapture = NONE;
		}
		return 0;
	} else if (keyCapture && element->type == ETYPE_FIELD) {
		int len;
		len = strlen(element->field);

		if (isChar && (key < 0 || key >= 32) && key != 127 && len < MAX_JS_STRINGSIZE - 1) {
			char utf8[5] = {0};
			int bytes = 0;

			if (key < 0x80) {
				utf8[0] = (char)key;
				bytes = 1;
			} else if (key < 0x800) {
				utf8[0] = 0xC0 | (key >> 6);
				utf8[1] = 0x80 | (key & 0x3F);
				bytes = 2;
			} else if (key < 0x10000) {
				utf8[0] = 0xE0 | (key >> 12);
				utf8[1] = 0x80 | ((key >> 6) & 0x3F);
				utf8[2] = 0x80 | (key & 0x3F);
				bytes = 3;
			} else if (key < 0x110000) {
				utf8[0] = 0xF0 | (key >> 18);
				utf8[1] = 0x80 | ((key >> 12) & 0x3F);
				utf8[2] = 0x80 | ((key >> 6) & 0x3F);
				utf8[3] = 0x80 | (key & 0x3F);
				bytes = 4;
			}

			if (element->fieldPosition < 0) element->fieldPosition = len;
			if (element->fieldPosition <= len && len + bytes <= MAX_JS_STRINGSIZE - 1) {
				memmove(element->field + element->fieldPosition + bytes, element->field + element->fieldPosition, len - element->fieldPosition + 1);
				memcpy(element->field + element->fieldPosition, utf8, bytes);

				element->fieldPosition += bytes;
				element->field[len + bytes] = '\0';
			}
		} else if (!isChar) {
			len = strlen(element->field);
			if (element->fieldPosition < 0) element->fieldPosition = len;

			if (key == K_LEFTARROW) {
				if (element->fieldPosition > 0) {
					element->fieldPosition--;
					while (element->fieldPosition > 0 && (element->field[element->fieldPosition] & 0xC0) == 0x80) {
						element->fieldPosition--;
					}
				}
			} else if (key == K_RIGHTARROW) {
				if (element->fieldPosition < len) {
					element->fieldPosition++;
					while (element->fieldPosition < len && (element->field[element->fieldPosition] & 0xC0) == 0x80) {
						element->fieldPosition++;
					}
				}
			} else if (key == K_BACKSPACE) {
				if (element->fieldPosition > 0) {
					int start = element->fieldPosition - 1;
					int bytesToDelete;
					while (start > 0 && (element->field[start] & 0xC0) == 0x80) {
						start--;
					}
					bytesToDelete = element->fieldPosition - start;
					memmove(element->field + start, element->field + element->fieldPosition, len - element->fieldPosition + 1);
					element->fieldPosition = start;
				}
			} else if (key == K_HOME) {
				element->fieldPosition = 0;
			} else if (key == K_END) {
				element->fieldPosition = len;
			} else if (key == K_DEL) {
				UI_KeyEvent(K_RIGHTARROW, qtrue, qfalse);
				UI_KeyEvent(K_BACKSPACE, qtrue, qfalse);
			} else if (key == K_ENTER) {
				if(strlen(element->cvar)) cvarSet(element->cvar, element->field);
				JS_ShellCallback(activeWindow->id, element->id, key);
			} else if (key == K_ESCAPE) {
				if(strlen(element->cvar)) cvarSet(element->cvar, element->field);
			}
		}
	}
	
	if(activeWindow != NULL && key == K_ESCAPE) activeWindow->keyCapture = NONE;

    if(!keyCapture) {
    	switch (key) { // Обработка глобальных клавиш
    	case K_ESCAPE: UI_SetActiveMenu(UI_DISABLE); break;
    	case K_F5: UI_HotReload(); break;
    	case K_F12: shell.debug = !shell.debug; break;
    	}
    }
	
    if (activeWindow != NULL) {
        if ((key == K_SPACE && !isChar) || (key != K_SPACE)) {
            JS_ShellKey(key, activeWindow->id);
        }
    }

    if(element != NULL) { // Обработка нажатий по элементам
    	switch (key) {
    	case K_MOUSE1:
    		if (UI_CursorOnItem(element)) UI_GeneralCallback(element);
    		break;
    	case K_MOUSE2:
    		if (UI_CursorOnItem(element)) UI_SecondaryCallback(element);
    		break;
    	default: UI_KeyCallback(element, key); break;
    	}
    	if(UI_CursorOnItem(element) && element->type != ETYPE_FIELD) JS_ShellCallback(activeWindow->id, element->id, key);
    }
    
	return 0;
}

int UI_MouseEvent(int dx, int dy) { // Обработка событий мыши UI
	int i;

	for (i = SHELL_MAX_WINDOWS-1; i >= 0; i--) { // Проверка и обработка перетаскивания окна
	    window_s *window = &shell.window[shell.zOrder[i]];
	    
	    if(!shell.cursorIsDragging) break;
	    if(!window->minimized && UI_CursorInRect(window->x, window->y, window->w, window->h)) break;
	    if(window->style & UI_NOTITLE) continue;
	    
        if(UI_CursorInRect(window->x, window->y-UI_WINDOW_TITLE_HEIGHT, window->w, UI_WINDOW_TITLE_HEIGHT)){
            window->x = clamp(window->x + dx, 0-(window->w*0.5), glconfig.width-(window->w*0.5));
	        window->y = clamp(window->y + dy, 0+UI_WINDOW_TITLE_HEIGHT, glconfig.height);
            break;
        }
	}
	
	shell.cursorX = clamp(shell.cursorX + dx, 0, glconfig.width);
	shell.cursorY = clamp(shell.cursorY + dy, 0, glconfig.height);

	shell.focusedWindow = NONE;
	for (i = SHELL_MAX_WINDOWS-1; i >= 0; i--) { // Поиск окна под курсором
	    window_s *window = &shell.window[shell.zOrder[i]];
	    
	    if(!window->created) continue;
	    
        if(UI_CursorInRect(window->x, window->y-UI_WINDOW_TITLE_HEIGHT, window->w, UI_WINDOW_TITLE_HEIGHT)){
            shell.focusedWindow = window->id;
            break;
        }
        if(!window->minimized && UI_CursorInRect(window->x, window->y, window->w, window->h)){
            shell.focusedWindow = window->id;
            break;
        }
	}

    shell.window[shell.focusedWindow].focusedElement = NONE;
	for (i = WINDOW_MAX_ELEMENTS-1; i >= 0; i--) { // Поиск элемента под курсором
	    window_s *window = &shell.window[shell.focusedWindow];
	    
	    if(shell.focusedWindow == NONE) break;
	    
	    if(UI_CursorOnItem(&window->element[i])) {
	        window->focusedElement = i;
	        break;
	    }
	}
	
	return 0;
}

static void UI_DrawTip(const char* text) { // Отрисовка подсказки
	drawRoundedRect((shell.cursorX - 5) + 18, shell.cursorY - 20, stringWidth(text, 1.00, 0) + 10, BASEFONT_HEIGHT + 4, 0, cgui.colors[JSC_CONTEXTMENU], 0);
	drawString(shell.cursorX + 18, shell.cursorY - 18, text, 0, cgui.colors[JSC_WHITE], 1.00);
}

#define UISTYLE_Y e->y + (e->h - (BASEFONT_HEIGHT * e->scale)) * 0.5
#define UISTYLE_HOVER UI_ItemFocused(e) ? e->hoverStyle : 0
#define UISTYLE_BACKGROUND e->x, e->y, e->w, e->h, e->corner, cgui.colors[e->colorBackground], e->style | hoverStyle
#define UISTYLE_CENTER_TEXT(text) e->x + (e->w * 0.5), text_y, text, UI_CENTER | e->style | hoverStyle, cgui.colors[e->colorText], e->scale
#define UISTYLE_RIGHT_TEXT(text) (e->x + e->w) - e->margin, text_y, text, UI_RIGHT | e->style | hoverStyle, cgui.colors[e->colorText], e->scale
#define UISTYLE_LEFT_TEXT(text) e->x + e->margin, text_y, text, e->style | hoverStyle, cgui.colors[e->colorText], e->scale

static void UI_Picture_Draw(element_s* e) {
	int hoverStyle = UISTYLE_HOVER;

    trap_R_SetColor(cgui.colors[e->colorBackground]);
	drawShader(e->x, e->y, e->w, e->h, e->text);
	trap_R_SetColor(NULL);
}

static void UI_Button_Draw(element_s* e) {
	float text_y = UISTYLE_Y;
	int hoverStyle = UISTYLE_HOVER;

	drawRoundedRect(UISTYLE_BACKGROUND);
	if (e->style & UI_CENTER) drawString(UISTYLE_CENTER_TEXT(e->text));
	else if (e->style & UI_RIGHT) drawString(UISTYLE_RIGHT_TEXT(e->text));
	else drawString(UISTYLE_LEFT_TEXT(e->text));
}

static void UI_Checkbox_Draw(element_s* e) {
    float text_y = UISTYLE_Y;
	int hoverStyle = UISTYLE_HOVER;
	
	drawRoundedRect(UISTYLE_BACKGROUND);
	if(e->value) {
    	drawRoundedRect(((e->x + e->w) - e->h * 1.38) - e->margin, (e->y + (e->h * 0.5)) - e->h * 0.37, e->h * 1.38, e->h * 0.74, 999999, cgui.colors[JSC_ENABLED], 0);
    	drawRoundedRect(((e->x + e->w) - e->h * 0.66) - e->margin, (e->y + (e->h * 0.5)) - e->h * 0.27, e->h * 0.54, e->h * 0.54, 999999, cgui.colors[JSC_WHITE], 0);
	} else {
    	drawRoundedRect(((e->x + e->w) - e->h * 1.38) - e->margin, (e->y + (e->h * 0.5)) - e->h * 0.37, e->h * 1.38, e->h * 0.74, 999999, cgui.colors[JSC_DISABLED], 0);
    	drawRoundedRect(((e->x + e->w) - e->h * 1.26) - e->margin, (e->y + (e->h * 0.5)) - e->h * 0.27, e->h * 0.54, e->h * 0.54, 999999, cgui.colors[JSC_WHITE], 0);
	}
	drawString(UISTYLE_LEFT_TEXT(e->text));
}

static float UI_SliderProgress(float value, float min, float max) { // Расчет положения slider
	float c = clamp(value, min, max);
	float result = (c - min) / (max - min);

	return result;
}

static void UI_Slider_Draw(element_s* e) {
    float text_y = UISTYLE_Y;
	int hoverStyle = UISTYLE_HOVER;

	drawRoundedRect(UISTYLE_BACKGROUND);
	drawRoundedRect(e->x + (e->w * 0.5), (e->y + (e->h * 0.5)) - e->h * 0.15, (e->w * 0.5) - e->margin, e->h * 0.30, 999999, cgui.colors[JSC_DISABLED], 0);
	drawRoundedRect(e->x + (e->w * 0.5), (e->y + (e->h * 0.5)) - e->h * 0.15, ((e->w * 0.5) - e->margin) * UI_SliderProgress(e->value, e->min, e->max), e->h * 0.30, 999999, cgui.colors[JSC_ENABLED], 0);
	drawRoundedRect(((e->x + (e->w * 0.5)) - (e->h * 0.32)) + (((e->w * 0.5) - e->margin) * UI_SliderProgress(e->value, e->min, e->max)), (e->y + (e->h * 0.5)) - e->h * 0.32, e->h * 0.64, e->h * 0.64, 999999, cgui.colors[JSC_WHITE], 0);
	drawString(UISTYLE_LEFT_TEXT(e->text));
	if (UI_CursorOnItem(e)) UI_DrawTip(va("%f", e->value));
}

static void UI_Action_Draw(element_s* e) {
    float text_y = UISTYLE_Y;
	int hoverStyle = UISTYLE_HOVER;
	char* bind = (shell.window[shell.activeWindow].keyCapture == e->id) ? ">_" : e->bind;

	drawRoundedRect(UISTYLE_BACKGROUND);
	drawString(UISTYLE_LEFT_TEXT(e->text));
	drawString(UISTYLE_RIGHT_TEXT(bind));
}

static void UI_Spin_Draw(element_s* e) {
    float text_y = UISTYLE_Y;
	int hoverStyle = UISTYLE_HOVER;

	drawRoundedRect(UISTYLE_BACKGROUND);
	drawString(UISTYLE_LEFT_TEXT(e->text));
	drawString(UISTYLE_RIGHT_TEXT(e->options[(int)e->value]));
}

static void UI_Field_Draw(element_s* e) {
    window_s *window = (window_s *)e->parentWindow;
    float text_y = UISTYLE_Y;
	int hoverStyle = UISTYLE_HOVER;
	qboolean keyCaptureActive = (shell.window[shell.activeWindow].keyCapture == e->id && window->id == shell.window[shell.activeWindow].id);

	if(keyCaptureActive) hoverStyle = 0;

	drawRoundedRect(UISTYLE_BACKGROUND);
	if (keyCaptureActive) {
		drawStringField(UISTYLE_LEFT_TEXT(e->field), e->fieldPosition);
	} else {
		drawString(UISTYLE_LEFT_TEXT(e->text));
		drawStringField(UISTYLE_RIGHT_TEXT(e->field), NONE);
	}
}

static int UI_ListGetCount(int listType, int listSubtype) {
    if(listType == LTYPE_APPS) return shell.appCount;
    return 0;
}

static char *UI_ListGetName(int listType, int listSubtype, int index) {
    if(listType == LTYPE_APPS) return shell.app[index].name;
    return "";
}

static char *UI_ListGetIcon(int listType, int listSubtype, int index) {
    if(listType == LTYPE_APPS) return shell.app[index].icon;
    return "";
}

static void UI_List_Draw(element_s* e) {
	int i, j, totalItems, currentItem = e->scroll;

    totalItems = UI_ListGetCount(e->listType, e->listSubtype);
	for (i = 0; i < e->row && currentItem < totalItems; i++) {
		for (j = 0; j < e->col && currentItem < totalItems; j++) {
			drawShader((e->x+e->margin4[0]) + (e->itemW * j), (e->y+e->margin4[1]) + (e->itemH * i), e->itemW-(e->margin4[2]+e->margin4[0]), e->itemH-(e->margin4[3]+e->margin4[1]), UI_ListGetIcon(e->listType, e->listSubtype, currentItem));
			drawString((e->x + (e->itemW*0.50)) + (e->itemW * j), e->y + (e->itemH * i)+(e->itemH*0.85), UI_ListGetName(e->listType, e->listSubtype, currentItem), UI_CENTER|UI_BOLD, cgui.colors[JSC_WHITE], e->scale);
			currentItem++;
		}
	}
}

static void UI_ListFiles_Draw(element_s* e) {
    window_s *window = (window_s *)e->parentWindow;
	int i, j, currentItem = 0;
	char *file = window->fileList[e->listID].list;
	char nameWithoutExt[MAX_JS_STRINGSIZE];
	
	for(i = 0; i < e->scroll; i++) {
	    if(i >= window->fileList[e->listID].count) break;
	    file += strlen(file) + 1;
	}

	for (i = 0; i < e->row; i++) {
		for (j = 0; j < e->col; j++) {
            if(currentItem >= window->fileList[e->listID].count) break;
            Q_snprintf(nameWithoutExt, sizeof(nameWithoutExt), "%.*s", (int)(strlen(file) - strlen(window->fileList[e->listID].ext)), file);
			drawShader((e->x+e->margin4[0]) + (e->itemW * j), (e->y+e->margin4[1]) + (e->itemH * i), e->itemW-(e->margin4[2]+e->margin4[0]), e->itemH-(e->margin4[3]+e->margin4[1]), va("%s/%s", window->fileList[e->listID].drawDir, nameWithoutExt));
			drawString((e->x + (e->itemW*0.50)) + (e->itemW * j), e->y + (e->itemH * i)+(e->itemH*0.85), nameWithoutExt, UI_CENTER|UI_BOLD, cgui.colors[JSC_WHITE], e->scale);
			currentItem++;
			file += strlen(file) + 1;
		}
	}
}

static void UI_WindowButton_Draw(element_s* e) {
    window_s *window = (window_s *)e->parentWindow;

    drawRoundedRect(e->x, e->y, e->w, e->h, e->corner, cgui.colors[e->colorBackground], e->style);
    drawString(e->x + (e->w * 0.545), (e->y + (e->h * 0.525)) - ((BASEFONT_HEIGHT*0.50) * e->scale), e->text, UI_CENTER | UI_BOLD, cgui.colors[e->colorText], e->scale);
}

static void UI_ElementRender(int windowID, int elementID) { // Отрисовка элемента
	element_s* element = &shell.window[windowID].element[elementID];
    if(!element->type) return;
    UI_AdjustToWindow(element);
    if(!UI_ElementIsVisible(element)) return;
    if(shell.window[windowID].minimized && element->type != ETYPE_WINDOWBUTTON) return;

	if(element->style & UI_NODRAW) return;
	switch (element->type) {
	case ETYPE_PICTURE: UI_Picture_Draw(element); break;
	case ETYPE_BUTTON: UI_Button_Draw(element); break;
	case ETYPE_CHECKBOX: UI_Checkbox_Draw(element); break;
	case ETYPE_SLIDER: UI_Slider_Draw(element); break;
	case ETYPE_ACTION: UI_Action_Draw(element); break;
	case ETYPE_SPIN: UI_Spin_Draw(element); break;
	case ETYPE_FIELD: UI_Field_Draw(element); break;
	case ETYPE_LIST: UI_List_Draw(element); break;
	case ETYPE_LISTFILES: UI_ListFiles_Draw(element); break;
	case ETYPE_WINDOWBUTTON: UI_WindowButton_Draw(element); break;
	}
    if(shell.debug) drawOutline(element->x, element->y, element->w, element->h, 2, cgui.colors[JSC_DEBUG1]);
}

static void UI_ShellDraw(void) { // Отрисовка всех окон и подсказки
    window_s *focusedWindow = &shell.window[shell.focusedWindow];
    window_s *activeWindow = &shell.window[shell.activeWindow];
	int i, j;
	float tc = shell.corner * shell.scale;
    float th = UI_WINDOW_TITLE_HEIGHT * shell.scale;

	for (i = 0; i < SHELL_MAX_WINDOWS; i++) {
	    window_s *window = &shell.window[shell.zOrder[i]];
	    
		if(!window->created) continue;
		UI_ScaleToWindow(window);
		if(!(window->style & UI_NOTITLE)) {
            drawRoundedRect(window->x, window->y - th, window->w, th, tc, cgui.colors[window->colorTitle], NO_BOTTOM_LEFT|NO_BOTTOM_RIGHT);
            drawString(window->x + (window->w * 0.5), (window->y - th) + (th - BASEFONT_HEIGHT * (1.00*shell.scale)) * 0.5, window->name, UI_CENTER | UI_BOLD, cgui.colors[window->colorText], 1.00*shell.scale);
            drawShader(window->x + (8*shell.scale), window->y - (th * 0.85), th*0.70, th*0.70, window->icon);
        }
        if(!window->minimized) {
		    drawRoundedRect(window->x, window->y, window->w, window->h, tc, cgui.colors[window->colorBackground], NO_TOP_LEFT|NO_TOP_RIGHT);
		    if(shell.debug) drawOutline(window->x, window->y, window->w, window->h, 2, cgui.colors[JSC_DEBUG2]);
        }
		for (j = 0; j < WINDOW_MAX_ELEMENTS; j++) UI_ElementRender(shell.zOrder[i], j);
	}

	if (activeWindow->keyCapture >= 0) UI_DrawTip("Press any key, ESC for cancel");
}

static void UI_ShellRender(void) { // Отрисовка UI, а также fallback если JS упал
	int i;

	JS_ShellDraw();
	for(i = 0; i < SHELL_MAX_WINDOWS; i++) {
	   if(!shell.window[i].created || !strlen(shell.window[i].nameID)) continue; 
	   JS_ShellUpdate(i);
	}
	UI_ShellDraw();
}

int UI_Refresh(void) { // Обновление UI (отрисовка кадра)
	consoleSync(&console, console.linesCount);
	UI_ShellRender();
	drawShader(shell.cursorX - (16*cgui.scale), shell.cursorY - (16*cgui.scale), 32*cgui.scale, 32*cgui.scale, "menu/cursor");
	
    return 0;
}

int UI_IsFullscreen(void) { // Статус полноэкранного режима UI (влияет на рендеринг карты)
	return ((trap_Key_GetCatcher() & KEYCATCH_UI) && !shell.onMap);
}

static void UI_OpenShell(void) { // Открытие UI
    UI_OnMapStatus();
	if (trap_Key_GetCatcher() & KEYCATCH_UI) return;
	trap_Key_SetCatcher(KEYCATCH_UI);
}

static void UI_CloseShell(void) { // Закрытие UI (состояние сохраняется)
	trap_Key_SetCatcher(KEYCATCH_CGAME);
	trap_Key_ClearStates();
}

int UI_SetActiveMenu(uiCommand_t cmd) { // Управление UI из движка
	(cmd ? UI_OpenShell : UI_CloseShell)();
	return 0;
}

static char* UI_Argv(int arg) { // Буфер аргумента
	static char buffer[MAX_STRING_CHARS];

	trap_Argv(arg, buffer, sizeof(buffer));
	return buffer;
}

qboolean UI_ConsoleCommand(void) { // Команда консоли из движка
	char* cmd;
	cmd = UI_Argv(0);

	if (Q_stricmp(cmd, "shell.open") == 0) {
		UI_OpenShell();
		return qtrue;
	}
	
	if (Q_stricmp(cmd, "shell.close") == 0) {
		UI_CloseShell();
		return qtrue;
	}

	return qfalse;
}

int UI_DrawConnectScreen(void) { // Экран подключение к серверу (до фактического подключения)
    uiClientState_t cstate;
	char* s;

	trap_GetClientState(&cstate);

	if (cstate.connState < CA_CONNECTED) drawString(0, 0, cstate.messageString, UI_CENTER, color_white, 1.00);

	switch (cstate.connState) {
	case CA_CONNECTING: drawString(glconfig.width * 0.5, glconfig.height * 0.15, va("Awaiting challenge...%i", cstate.connectPacketCount), UI_CENTER, color_white, 1.00); break;
	case CA_CHALLENGING: drawString(glconfig.width * 0.5, glconfig.height * 0.15, va("Awaiting connection...%i", cstate.connectPacketCount), UI_CENTER, color_white, 1.00); break;
	case CA_CONNECTED: drawString(glconfig.width * 0.5, glconfig.height * 0.15, "Awaiting gamestate...", UI_CENTER, color_white, 1.00); break;
	}
	
	UI_OnMapStatus();
	UI_Refresh();
	drawString(glconfig.width * 0.5, glconfig.height * 0.15, s, UI_CENTER, color_white, 1.00);
	
	return 0;
}

void UI_CloseWindow(int windowID) { // Закрытие окна
    if(shell.activeWindow == shell.window[windowID].id) shell.activeWindow = 0;
	memset(&shell.window[windowID], 0, sizeof(shell.window[windowID]));
}

void UI_ClearWindow(int windowID, int min, int max) { // Очистка окна
    int i;
    
    for(i = min; i <= max; i++) memset(&shell.window[windowID].element[i], 0, sizeof(shell.window[windowID].element[i]));
}

int UI_GetElementFree(int windowID) {
	int id = -1;
	int i;

	for (i = 0; i < WINDOW_MAX_ELEMENTS; i++) {
		if (!shell.window[windowID].element[i].type) {
			id = i;
			break;
		}
	}
	
	return id;
}

int UI_Window(int windowID, char* nameID, char* name, char* icon, int style, float x, float y, float w, float h, int colorText, int colorTitle, int colorBackground) {
	int id = windowID;
	int i;

	if (windowID == NONE) {
		for (i = 0; i < SHELL_MAX_WINDOWS; i++) {
			if (!shell.window[i].created) {
				id = i;
				break;
			}
		}
	}

	if (id == NONE) {
		print("#f55Shell: window limit exceeded \n");
		return id;
	}
	
	// TODO: Сделать верификатор функцию которая как assert.
	//       Принимает значение индекса, а также min, max 
	//       если индекс некорректен, выдает ошибку сразу же!
	//       Всплывающую чтобы мешала, но не убивала игру!

	shell.window[id].created = qtrue;
	shell.window[id].minimized = qfalse;
	shell.window[id].id = id;
	StringCopy(shell.window[id].nameID, nameID, MAX_JS_STRINGSIZE);
	StringCopy(shell.window[id].name, name, MAX_JS_STRINGSIZE);
	StringCopy(shell.window[id].icon, icon, MAX_JS_STRINGSIZE);
	shell.window[id].style = style;
	shell.window[id].x = x;
	shell.window[id].y = y;
	shell.window[id].baseW = w;
	shell.window[id].baseH = h;
	shell.window[id].colorText = colorText;
	shell.window[id].colorTitle = colorTitle;
	shell.window[id].colorBackground = colorBackground;
	shell.window[id].scaleFactor = shell.scale;
	shell.window[id].keyCapture = NONE;
	UI_WindowToTop(id);
	return id;
}

static int UI_GenericItem(int windowID, int elementID, int type, float x, float y, float w, float h, char* text, int style, int color, float scale) {
	int id = elementID;
	int i;

	if (elementID == NONE) {
		for (i = 0; i < WINDOW_MAX_ELEMENTS; i++) {
			if (!shell.window[windowID].element[i].type) {
				id = i;
				break;
			}
		}
	}

	if (id == NONE) {
		print("#f55Shell: element limit exceeded \n");
		return id;
	}
	
	shell.window[windowID].element[id].type = type;
	shell.window[windowID].element[id].id = id;
	shell.window[windowID].element[id].parentWindow = &shell.window[windowID];
	shell.window[windowID].element[id].baseX = x;
	shell.window[windowID].element[id].baseY = y;
	shell.window[windowID].element[id].baseW = w;
	shell.window[windowID].element[id].baseH = h;
	StringCopy(shell.window[windowID].element[id].text, text, MAX_JS_STRINGSIZE);
	shell.window[windowID].element[id].style = style;
	shell.window[windowID].element[id].hoverStyle = UI_UNDERLINE;
	shell.window[windowID].element[id].baseScale = scale;
	shell.window[windowID].element[id].colorText = JSC_WHITE;
	shell.window[windowID].element[id].colorBackground = color;
	shell.window[windowID].element[id].baseCorner = cvarFloat("shell.generic.element.corner");
	shell.window[windowID].element[id].baseMargin = cvarFloat("shell.generic.element.corner") + cvarFloat("shell.generic.element.margin");
	shell.window[windowID].element[id].scroll = 0;
	if(!shell.window[windowID].element[id].valueMod) shell.window[windowID].element[id].valueMod = 1.00;
	return id;
}

int UI_WindowButton(int windowID, int elementID, float x, char* text, int style, int color, float scale) {
	int id = UI_GenericItem(windowID, elementID, ETYPE_WINDOWBUTTON, x, 0, 0, 0, text, style, color, scale);
	shell.window[windowID].element[id].baseY = 0 - ((UI_WINDOW_TITLE_HEIGHT * shell.scale) * 0.85);
	shell.window[windowID].element[id].baseW = UI_WINDOW_TITLE_HEIGHT * shell.scale * 0.70;
	shell.window[windowID].element[id].baseH = UI_WINDOW_TITLE_HEIGHT * shell.scale * 0.70;
	shell.window[windowID].element[id].baseCorner = 999999;
	return id;
}

int UI_Picture(int windowID, int elementID, float x, float y, float w, float h, char* picture, int style, int color) {
	int id = UI_GenericItem(windowID, elementID, ETYPE_PICTURE, x, y, w, h, picture, style, color, 1.00);
	return id;
}

int UI_Button(int windowID, int elementID, float x, float y, float w, float h, char* text, int style, int color, float scale) {
	int id = UI_GenericItem(windowID, elementID, ETYPE_BUTTON, x, y, w, h, text, style, color, scale);
	return id;
}

int UI_Checkbox(int windowID, int elementID, float x, float y, float w, float h, char* text, int style, int color, float scale, char* cvar) {
	int id = UI_GenericItem(windowID, elementID, ETYPE_CHECKBOX, x, y, w, h, text, style, color, scale);
	StringCopy(shell.window[windowID].element[id].cvar, cvar, MAX_JS_STRINGSIZE);
	shell.window[windowID].element[id].value = cvarFloat(shell.window[windowID].element[id].cvar);
	return id;
}

int UI_Slider(int windowID, int elementID, float x, float y, float w, float h, char* text, int style, int color, float scale, char* cvar, float min, float max, int mode) {
	int id = UI_GenericItem(windowID, elementID, ETYPE_SLIDER, x, y, w, h, text, style, color, scale);
	StringCopy(shell.window[windowID].element[id].cvar, cvar, MAX_JS_STRINGSIZE);
	shell.window[windowID].element[id].value = cvarFloat(shell.window[windowID].element[id].cvar);
	shell.window[windowID].element[id].min = min;
	shell.window[windowID].element[id].max = max;
	shell.window[windowID].element[id].mode = mode;
	return id;
}

int UI_Action(int windowID, int elementID, float x, float y, float w, float h, char* text, int style, int color, float scale, char* cvar) {
	int i;
	int id = UI_GenericItem(windowID, elementID, ETYPE_ACTION, x, y, w, h, text, style, color, scale);
	StringCopy(shell.window[windowID].element[id].cvar, cvar, MAX_JS_STRINGSIZE);
	for (i = 0; i < MAX_KEYS; i++) {
		if (!strcmp(cvar, cgui.binds[i])) trap_Key_KeynumToStringBuf(i, shell.window[windowID].element[id].bind, 256);
	}
	return id;
}

int UI_Spin(int windowID, int elementID, float x, float y, float w, float h, char* text, int style, int color, float scale, char* cvar, int mode) {
	int i;
	int id = UI_GenericItem(windowID, elementID, ETYPE_SPIN, x, y, w, h, text, style, color, scale);
	StringCopy(shell.window[windowID].element[id].cvar, cvar, MAX_JS_STRINGSIZE);
	shell.window[windowID].element[id].mode = mode;
	if (mode == EMODE_NUMBER) {
		shell.window[windowID].element[id].value = cvarFloat(shell.window[windowID].element[id].cvar);
	} else if (mode == EMODE_STRING) {
		for (i = 0; i < shell.window[windowID].element[id].optionsCount; i++) {
			if (!strcmp(cvarString(shell.window[windowID].element[id].cvar), shell.window[windowID].element[id].options[i])) shell.window[windowID].element[id].value = i;
		}
	}
	return id;
}

int UI_Field(int windowID, int elementID, float x, float y, float w, float h, char* text, int style, int color, float scale, char* cvar) {
	int id = UI_GenericItem(windowID, elementID, ETYPE_FIELD, x, y, w, h, text, style, color, scale);
	StringCopy(shell.window[windowID].element[id].cvar, cvar, MAX_JS_STRINGSIZE);
	StringCopy(shell.window[windowID].element[id].field, cvarString(cvar), MAX_JS_STRINGSIZE);
	shell.window[windowID].element[id].fieldPosition = strlen(cvarString(cvar));
	return id;
}

int UI_List(int windowID, int elementID, float x, float y, float w, float h, float scale, int col, int row, int listType, int listSubtype) {
	int id = UI_GenericItem(windowID, elementID, ETYPE_LIST, x, y, w, h, "", 0, 0, scale);
	shell.window[windowID].element[id].listType = listType;
	shell.window[windowID].element[id].listSubtype = listSubtype;
	shell.window[windowID].element[id].col = col;
	shell.window[windowID].element[id].row = row;
	return id;
}

int UI_ListFiles(int windowID, int elementID, float x, float y, float w, float h, float scale, int col, int row, int listID) {
	int id = UI_GenericItem(windowID, elementID, ETYPE_LISTFILES, x, y, w, h, "", 0, 0, scale);
	shell.window[windowID].element[id].col = col;
	shell.window[windowID].element[id].row = row;
	shell.window[windowID].element[id].listID = listID;
	return id;
}

void UI_FillListFiles(int windowID, char* folder, char* ext, char *drawDir, int listID){
	shell.window[windowID].fileList[listID].count = FS_List(folder, ext, shell.window[windowID].fileList[listID].list, FILELIST_MAX_LENGTH);
	StringCopy(shell.window[windowID].fileList[listID].folder, folder, MAX_JS_STRINGSIZE);
	StringCopy(shell.window[windowID].fileList[listID].ext, ext, MAX_JS_STRINGSIZE);
	StringCopy(shell.window[windowID].fileList[listID].drawDir, drawDir, MAX_JS_STRINGSIZE);
}

void UI_SetMargin(int windowID, int elementID, float x, float y, float w, float h) {
    shell.window[windowID].element[elementID].baseMargin4[0] = x;
    shell.window[windowID].element[elementID].baseMargin4[1] = y;
    shell.window[windowID].element[elementID].baseMargin4[2] = w;
    shell.window[windowID].element[elementID].baseMargin4[3] = h;
}

// TODO: Добавить margin4 массив для списков и ахуенную функцию настройки все0 четырех свойств
//       Тоже самое для углов сделать, внедрить типы контента, такие как 
//       [Шейдер, картинка, 3д модель, фоллбек на символы, значки предметов и оружия, игроки итд]
//       UI_SetMargin, UI_SetRect итд.
