// Copyright (C) 2023-2026 Noire's Mod — GPLv2

#include "javascript.h"

js_args_t vmargs;
js_result_t vmresult;
js_args_t jsargs;
js_result_t jsresult;

void JS_StackClean(void) { memset(&jsargs, 0, sizeof(jsargs)); }
void JS_Function(int func_id) {
	memset(&jsresult, 0, sizeof(jsresult));
	JSCall(func_id, &jsargs, &jsresult);
}

static int api_foundEntry = qfalse;
static int api_valueIndex = 0;
static int api_counter = 0;

void JSE_Int(int* number) {
    api_foundEntry = qtrue;
    if(vmargs.t[api_valueIndex] != JS_TYPE_NONE) *number = vmargs.v[api_valueIndex].i;
    vmresult.t = JS_TYPE_INT;
    vmresult.v.i = *number;
}

void JSE_Float(float* number) {
    api_foundEntry = qtrue;
    if(vmargs.t[api_valueIndex] != JS_TYPE_NONE) *number = vmargs.v[api_valueIndex].f;
    vmresult.t = JS_TYPE_FLOAT;
    vmresult.v.f = *number;
}

void JSE_String(char* string, size_t maxSize) {
    api_foundEntry = qtrue;
    if(vmargs.t[api_valueIndex] != JS_TYPE_NONE) StringCopy(string, vmargs.v[api_valueIndex].s, maxSize);
    vmresult.t = JS_TYPE_STRING;
    StringCopy(vmresult.v.s, string, MAX_JS_STRINGSIZE);
}

#define API_FIELD(expression) if (fieldID == api_counter++) { expression; return; }

#ifndef GAME
static void API_CGUI(int fieldID, int i1, int i2, int i3, int i4) {
    api_foundEntry = qfalse;
    api_valueIndex = 1;
    api_counter = 0;
    
    API_FIELD( JSE_Int(&cgui.whiteShader) )
    API_FIELD( JSE_Int(&cgui.corner) )
    API_FIELD( JSE_Float(&cgui.scale) )
    API_FIELD( JSE_Float(&cgui.wideoffset) )
    API_FIELD( bounds(i1, CGUI_COLORCOUNT, "CGUI Colors"); bounds(i2, 4, "Color Components"); JSE_Float(&cgui.colors[i1][i2]) )
    API_FIELD( bounds(i1, MAX_KEYS, "CGUI Binds"); JSE_String(cgui.binds[i1], MAX_JS_STRINGSIZE) )
}

static void API_GLConfig(int fieldID, int i1, int i2, int i3, int i4) {
    api_foundEntry = qfalse;
    api_valueIndex = 1;
    api_counter = 0;
    
    API_FIELD( JSE_Int(&glconfig.width) )
    API_FIELD( JSE_Int(&glconfig.height) )
    API_FIELD( JSE_Int(&glconfig.isFullscreen) )
}
#endif

#ifdef UI
static void API_Shell(int fieldID, int i1, int i2, int i3, int i4) {
    api_foundEntry = qfalse;
    api_valueIndex = 1;
    api_counter = 0;
    
    API_FIELD( JSE_Int(&shell.cursorX) )
    API_FIELD( JSE_Int(&shell.cursorY) )
    API_FIELD( JSE_Int(&shell.cursorIsDragging) )
    API_FIELD( JSE_Int(&shell.debug) )
    API_FIELD( JSE_Float(&shell.scale) )
    API_FIELD( JSE_Int(&shell.corner) )
    API_FIELD( JSE_Int(&shell.onMap) )
    API_FIELD( JSE_Int(&shell.appCount) )
    API_FIELD( JSE_Int(&shell.focusedWindow) )
    API_FIELD( JSE_Int(&shell.activeWindow) )
    API_FIELD( bounds(i1, SHELL_MAX_WINDOWS, "Shell ZOrder"); JSE_Int(&shell.zOrder[i1]) )
}

static void API_Window(int windowID, int fieldID, int i1, int i2, int i3, int i4) {
    api_foundEntry = qfalse;
    api_valueIndex = 2;
    api_counter = 0;
    
    bounds(windowID, SHELL_MAX_WINDOWS, "Shell Windows");
    
    API_FIELD( JSE_Int(&shell.window[windowID].created) )
    API_FIELD( JSE_Int(&shell.window[windowID].minimized) )
    API_FIELD( JSE_Int(&shell.window[windowID].id) )
    API_FIELD( JSE_String(shell.window[windowID].nameID, MAX_JS_STRINGSIZE) )
    API_FIELD( JSE_String(shell.window[windowID].name, MAX_JS_STRINGSIZE) )
    API_FIELD( JSE_String(shell.window[windowID].icon, MAX_JS_STRINGSIZE) )
    API_FIELD( JSE_Int(&shell.window[windowID].style) )
    API_FIELD( JSE_Float(&shell.window[windowID].baseW) )
    API_FIELD( JSE_Float(&shell.window[windowID].baseH) )
    API_FIELD( JSE_Float(&shell.window[windowID].x) )
    API_FIELD( JSE_Float(&shell.window[windowID].y) )
    API_FIELD( JSE_Float(&shell.window[windowID].w) )
    API_FIELD( JSE_Float(&shell.window[windowID].h) )
    API_FIELD( JSE_Int(&shell.window[windowID].colorText) )
    API_FIELD( JSE_Int(&shell.window[windowID].colorTitle) )
    API_FIELD( JSE_Int(&shell.window[windowID].colorBackground) )
    API_FIELD( JSE_Float(&shell.window[windowID].scaleFactor) )
    API_FIELD( JSE_Int(&shell.window[windowID].focusedElement) )
    API_FIELD( JSE_Int(&shell.window[windowID].keyCapture) )
}

static void API_Element(int windowID, int elementID, int fieldID, int i1, int i2, int i3, int i4) {
    api_foundEntry = qfalse;
    api_valueIndex = 3;
    api_counter = 0;
    
    bounds(windowID, SHELL_MAX_WINDOWS, "Shell Windows");
    bounds(elementID, WINDOW_MAX_ELEMENTS, "Window Elements");
    
    API_FIELD( JSE_Int(&((window_s*)shell.window[windowID].element[elementID].parentWindow)->id) )
    API_FIELD( JSE_Int(&shell.window[windowID].element[elementID].type) )
    API_FIELD( JSE_Int(&shell.window[windowID].element[elementID].id) )
    API_FIELD( JSE_Float(&shell.window[windowID].element[elementID].baseX) )
    API_FIELD( JSE_Float(&shell.window[windowID].element[elementID].baseY) )
    API_FIELD( JSE_Float(&shell.window[windowID].element[elementID].baseW) )
    API_FIELD( JSE_Float(&shell.window[windowID].element[elementID].baseH) )
    API_FIELD( JSE_Float(&shell.window[windowID].element[elementID].x) )
    API_FIELD( JSE_Float(&shell.window[windowID].element[elementID].y) )
    API_FIELD( JSE_Float(&shell.window[windowID].element[elementID].w) )
    API_FIELD( JSE_Float(&shell.window[windowID].element[elementID].h) )
    API_FIELD( JSE_String(shell.window[windowID].element[elementID].text, MAX_JS_STRINGSIZE) )
    API_FIELD( JSE_Int(&shell.window[windowID].element[elementID].style) )
    API_FIELD( JSE_Int(&shell.window[windowID].element[elementID].hoverStyle) )
    API_FIELD( JSE_Float(&shell.window[windowID].element[elementID].baseScale) )
    API_FIELD( JSE_Float(&shell.window[windowID].element[elementID].scale) )
    API_FIELD( JSE_Int(&shell.window[windowID].element[elementID].colorText) )
    API_FIELD( JSE_Int(&shell.window[windowID].element[elementID].colorBackground) )
    API_FIELD( JSE_Int(&shell.window[windowID].element[elementID].baseCorner) )
    API_FIELD( JSE_Int(&shell.window[windowID].element[elementID].corner) )
    API_FIELD( JSE_Int(&shell.window[windowID].element[elementID].baseMargin) )
    API_FIELD( JSE_Int(&shell.window[windowID].element[elementID].margin) )
    API_FIELD( JSE_String(shell.window[windowID].element[elementID].cvar, MAX_JS_STRINGSIZE) )
    API_FIELD( JSE_Float(&shell.window[windowID].element[elementID].value) )
    API_FIELD( JSE_Float(&shell.window[windowID].element[elementID].valueMod) )
    API_FIELD( JSE_Float(&shell.window[windowID].element[elementID].min) )
    API_FIELD( JSE_Float(&shell.window[windowID].element[elementID].max) )
    API_FIELD( JSE_Int(&shell.window[windowID].element[elementID].mode) )
    API_FIELD( JSE_String(shell.window[windowID].element[elementID].bind, MAX_JS_STRINGSIZE) )
    API_FIELD( bounds(i1, ELEMENT_OPTIONSCOUNT, "Element Options"); JSE_String(shell.window[windowID].element[elementID].options[i1], ELEMENT_OPTIONLENGTH) )
    API_FIELD( JSE_Int(&shell.window[windowID].element[elementID].optionsCount) )
    API_FIELD( JSE_String(shell.window[windowID].element[elementID].field, MAX_JS_STRINGSIZE) )
    API_FIELD( JSE_Int(&shell.window[windowID].element[elementID].fieldPosition) )
    API_FIELD( JSE_Float(&shell.window[windowID].element[elementID].itemW) )
    API_FIELD( JSE_Float(&shell.window[windowID].element[elementID].itemH) )
    API_FIELD( JSE_Int(&shell.window[windowID].element[elementID].col) )
    API_FIELD( JSE_Int(&shell.window[windowID].element[elementID].row) )
    API_FIELD( JSE_Int(&shell.window[windowID].element[elementID].listID) )
}

static void API_Console(int fieldID, int i1, int i2, int i3, int i4) {
    api_foundEntry = qfalse;
    api_valueIndex = 1;
    api_counter = 0;
    
    API_FIELD( bounds(i1, CONSOLE_MAXLINES, "Console Lines"); JSE_String(console.lines[i1], MAX_JS_STRINGSIZE) )
    API_FIELD( JSE_Int(&console.linesCount) )
}
#endif

void VMCall(int func_id) {
    switch(func_id) {
    case VM_CMD: trap_Cmd(EXEC_INSERT, vmargs.v[0].s); break;
#ifndef GAME
    case VM_APICGUI: API_CGUI(vmargs.v[0].i, vmargs.v[2].i, vmargs.v[3].i, vmargs.v[4].i, vmargs.v[5].i); break;
    case VM_APIGLCONFIG: API_GLConfig(vmargs.v[0].i, vmargs.v[2].i, vmargs.v[3].i, vmargs.v[4].i, vmargs.v[5].i); break;
    case VM_DRAWSTRING: drawString(vmargs.v[0].f, vmargs.v[1].f, vmargs.v[2].s, vmargs.v[3].i, cgui.colors[vmargs.v[4].i], vmargs.v[5].f); break;
    case VM_DRAWRECTANGLE: drawRoundedRect(vmargs.v[0].f, vmargs.v[1].f, vmargs.v[2].f, vmargs.v[3].f, vmargs.v[4].f, cgui.colors[vmargs.v[5].i], vmargs.v[6].i); break;
    case VM_DRAWSHADER: drawShader(vmargs.v[0].f, vmargs.v[1].f, vmargs.v[2].f, vmargs.v[3].f, vmargs.v[4].s); break;
    case VM_DRAWCOLOREDSHADER: drawColoredShader(vmargs.v[0].f, vmargs.v[1].f, vmargs.v[2].f, vmargs.v[3].f, vmargs.v[4].s, cgui.colors[vmargs.v[5].i]); break;
#endif
#ifdef UI
    case VM_APISHELL: API_Shell(vmargs.v[0].i, vmargs.v[2].i, vmargs.v[3].i, vmargs.v[4].i, vmargs.v[5].i); break;
    case VM_APIWINDOW: API_Window(vmargs.v[0].i, vmargs.v[1].i, vmargs.v[3].i, vmargs.v[4].i, vmargs.v[5].i, vmargs.v[6].i); break;
    case VM_APIELEMENT: API_Element(vmargs.v[0].i, vmargs.v[1].i, vmargs.v[2].i, vmargs.v[4].i, vmargs.v[5].i, vmargs.v[6].i, vmargs.v[7].i); break;
    case VM_APICONSOLE: API_Console(vmargs.v[0].i, vmargs.v[2].i, vmargs.v[3].i, vmargs.v[4].i, vmargs.v[5].i); break;
    case VM_CLOSEWINDOW: UI_CloseWindow(vmargs.v[0].i); break;
    case VM_CLEARWINDOW: UI_ClearWindow(vmargs.v[0].i, vmargs.v[1].i, vmargs.v[2].i); break;
    case VM_GETFREEELEMENT: UI_GetElementFree(vmargs.v[0].i); break;
    case VM_CREATEWINDOW: vmresult.v.i = UI_Window(vmargs.v[0].i, vmargs.v[1].s, vmargs.v[2].s, vmargs.v[3].s, vmargs.v[4].i, vmargs.v[5].f, vmargs.v[6].f, vmargs.v[7].f, vmargs.v[8].f, vmargs.v[9].i, vmargs.v[10].i, vmargs.v[11].i); vmresult.t = JS_TYPE_INT; break;
    case VM_CREATEWINDOWBUTTON: vmresult.v.i = UI_WindowButton(vmargs.v[0].i, vmargs.v[1].i, vmargs.v[2].f, vmargs.v[3].s, vmargs.v[4].i, vmargs.v[5].i, vmargs.v[6].f); vmresult.t = JS_TYPE_INT; break;
    case VM_CREATEPICTURE: vmresult.v.i = UI_Picture(vmargs.v[0].i, vmargs.v[1].i, vmargs.v[2].f, vmargs.v[3].f, vmargs.v[4].f, vmargs.v[5].f, vmargs.v[6].s, vmargs.v[7].i, vmargs.v[8].i); vmresult.t = JS_TYPE_INT; break;
    case VM_CREATEBUTTON: vmresult.v.i = UI_Button(vmargs.v[0].i, vmargs.v[1].i, vmargs.v[2].f, vmargs.v[3].f, vmargs.v[4].f, vmargs.v[5].f, vmargs.v[6].s, vmargs.v[7].i, vmargs.v[8].i, vmargs.v[9].f); vmresult.t = JS_TYPE_INT; break;
    case VM_CREATECHECKBOX: vmresult.v.i = UI_Checkbox(vmargs.v[0].i, vmargs.v[1].i, vmargs.v[2].f, vmargs.v[3].f, vmargs.v[4].f, vmargs.v[5].f, vmargs.v[6].s, vmargs.v[7].i, vmargs.v[8].i, vmargs.v[9].f, vmargs.v[10].s); vmresult.t = JS_TYPE_INT; break;
    case VM_CREATESLIDER: vmresult.v.i = UI_Slider(vmargs.v[0].i, vmargs.v[1].i, vmargs.v[2].f, vmargs.v[3].f, vmargs.v[4].f, vmargs.v[5].f, vmargs.v[6].s, vmargs.v[7].i, vmargs.v[8].i, vmargs.v[9].f, vmargs.v[10].s, vmargs.v[11].f, vmargs.v[12].f, vmargs.v[13].i); vmresult.t = JS_TYPE_INT; break;
    case VM_CREATEACTION: vmresult.v.i = UI_Action(vmargs.v[0].i, vmargs.v[1].i, vmargs.v[2].f, vmargs.v[3].f, vmargs.v[4].f, vmargs.v[5].f, vmargs.v[6].s, vmargs.v[7].i, vmargs.v[8].i, vmargs.v[9].f, vmargs.v[10].s); vmresult.t = JS_TYPE_INT; break;
    case VM_CREATESPIN: vmresult.v.i = UI_Spin(vmargs.v[0].i, vmargs.v[1].i, vmargs.v[2].f, vmargs.v[3].f, vmargs.v[4].f, vmargs.v[5].f, vmargs.v[6].s, vmargs.v[7].i, vmargs.v[8].i, vmargs.v[9].f, vmargs.v[10].s, vmargs.v[11].i); vmresult.t = JS_TYPE_INT; break;
    case VM_CREATEFIELD: vmresult.v.i = UI_Field(vmargs.v[0].i, vmargs.v[1].i, vmargs.v[2].f, vmargs.v[3].f, vmargs.v[4].f, vmargs.v[5].f, vmargs.v[6].s, vmargs.v[7].i, vmargs.v[8].i, vmargs.v[9].f, vmargs.v[10].s); vmresult.t = JS_TYPE_INT; break;
    case VM_CREATELIST: vmresult.v.i = UI_List(vmargs.v[0].i, vmargs.v[1].i, vmargs.v[2].f, vmargs.v[3].f, vmargs.v[4].f, vmargs.v[5].f, vmargs.v[6].f, vmargs.v[7].i, vmargs.v[8].i, vmargs.v[9].i, vmargs.v[10].i); vmresult.t = JS_TYPE_INT; break;
    case VM_CREATELISTFILES: vmresult.v.i = UI_ListFiles(vmargs.v[0].i, vmargs.v[1].i, vmargs.v[2].f, vmargs.v[3].f, vmargs.v[4].f, vmargs.v[5].f, vmargs.v[6].f, vmargs.v[7].i, vmargs.v[8].i, vmargs.v[9].i); vmresult.t = JS_TYPE_INT; break;
    case VM_FILLLISTFILES: UI_FillListFiles(vmargs.v[0].i, vmargs.v[1].s, vmargs.v[2].s, vmargs.v[3].s, vmargs.v[4].i); break;
    case VM_SETMARGIN: UI_SetMargin(vmargs.v[0].i, vmargs.v[1].i, vmargs.v[2].f, vmargs.v[3].f, vmargs.v[4].f, vmargs.v[5].f); break;
#endif
	}
}
