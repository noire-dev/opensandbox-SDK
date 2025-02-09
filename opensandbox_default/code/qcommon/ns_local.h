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

#ifdef QAGAME
#include "../game/g_local.h"
#endif
#ifdef CGAME
#include "../cgame/cg_local.h"
#endif
#ifdef Q3_UI
#include "../ui/ui_local.h"
#endif

/*
###############
Глобальное
###############
*/

#define MAX_NSSCRIPT_SIZE   1024*256      //Макс длина скрипта
#define MAX_TOKEN_LENGTH    1024          //Макс количество символов в токене
#define MAX_SCRIPTS         128           //Макс количество потоков скриптов
#define MAX_CYCLE_SIZE      1024*256      //Макс длина буфера кода
#define MAX_VARS            32768         //Макс переменных
#define MAX_VAR_NAME        32            //Макс имя переменной
#define MAX_NCVAR_NAME      64            //Макс имя консольной переменной
#define MAX_VAR_CHAR_BUF    1024          //Макс буфер char переменной
#define MAX_FUNCS           4096          //Макс количество функций
#define MAX_ARGS            64            //Количество аргументов
#define MAX_ARG_LENGTH      1024          //Длина аргументов
#define MAX_THREAD_NAME     32            //Макс длина названия потока

typedef struct {
    char threadName[MAX_THREAD_NAME];
    char code[MAX_NSSCRIPT_SIZE];
    int interval; // Интервал в миллисекундах
    int lastRunTime; // Время последнего запуска
} ScriptLoop;

/*
###############
Выражения
###############
*/

typedef enum {
    EQUAL,            // == 
    NOT_EQUAL,       // != 
    LESS_THAN,       // <  
    GREATER_THAN,    // >  
    LESS_OR_EQUAL,   // <= 
    GREATER_OR_EQUAL  // >=
} NSOperator;

/*
###############
Переменные
###############
*/

// Определяем union для разных типов
typedef union {
    char c[MAX_VAR_CHAR_BUF];
    int i;
    float f;
} VarValue;

// Перечисляем типы переменных
typedef enum {
    TYPE_CHAR,
    TYPE_INT,
    TYPE_FLOAT,
    TYPE_INVALID
} VarType;

// Структура для переменной с именем, типом и значением
typedef struct {
    char name[MAX_VAR_NAME];
    VarValue value;
    VarType type;
} Variable;

Variable* find_variable(const char *name);
int variable_exists(const char *name);
void print_variables();
int get_variable_int(const char *name);
float get_variable_float(const char *name);
char* get_variable_char(const char *name);
void set_variable_value(const char *name, const char *value, VarType type);
void create_variable(const char *name, const char *value, VarType type);

/*
###############
Выражения
###############
*/

float parse_expression(const char** expr);
float parse_term(const char** expr);
float parse_number(const char** expr);
float parse_factor(const char** expr);
void NS_ArgumentText(const char *input, char *result, int resultSize);

/*
###############
Функции
###############
*/

// Определяем для разных типов
typedef union {
    char c[MAX_VAR_CHAR_BUF];
    int i;
    float f;
} ArgValue;

/*
###############
Выполнение
###############
*/

void Interpret(char* script);

/*
###############
Потоки
###############
*/

extern ScriptLoop threadsLoops[MAX_SCRIPTS];
extern int threadsCount;
void print_threads();

/*
###############
Запуск
###############
*/

void NS_OpenScript(const char* filename, const char* threadName, int interval);

/*
###############
Noire.Script API
###############
*/

void NS_getCvar(VarValue *modify, VarType type, const char *cvarName);
int get_cvar_int(const char *name);
float get_cvar_float(const char *name);
char* get_cvar_char(const char *name);
void NS_setCvar(const char *cvarName, const char *cvarValue);
void NS_createCvar(const char *cvarName, const char *cvarValue);

/*
###############
NS Gui API
###############
*/

#define	 MAX_OBJECTS 99
