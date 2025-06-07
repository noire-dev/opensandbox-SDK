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

#include "../qcommon/ns_local.h"

/*
============
Svcmd_NS_OpenScript_f
Opens Noire.Script file
============
*/
void Svcmd_NS_OpenScript_f( void ) {
	char   filename[64];
	if( trap_Argc( ) == 1 ){
		G_Printf( "usage: ns_openscript <filename>\n" );
		return;
	}
  
	trap_Argv( 1, filename, sizeof( filename ) );
  
	NS_OpenScript(filename, NULL, 0);

}

/*
============
Svcmd_NS_Interpret_f
Show Noire.Script variables
============
*/
void Svcmd_NS_Interpret_f( void ) {
	if( trap_Argc( ) == 1 ){
		G_Printf( "usage: ns_interpret <code>\n" );
		return;
	}
  
	Interpret(ConcatArgs( 1 ));

}

/*
============
Svcmd_NS_VariableList_f
Show Noire.Script variables
============
*/
void Svcmd_NS_VariableList_f( void ) {
  
	print_variables();

}

/*
============
Svcmd_NS_ThreadList_f
Show Noire.Script threads
============
*/
void Svcmd_NS_ThreadList_f( void ) {
  
	print_threads();

}

/*
============
Svcmd_NS_SendVariable_f
Sends variable to Noire.Script vm
============
*/
void Svcmd_NS_SendVariable_f( void ) {
	char   varName[MAX_VAR_NAME];
	char   varValue[MAX_VAR_CHAR_BUF];
	char   varType[8];
  
	trap_Argv( 1, varName, sizeof( varName ) );
	trap_Argv( 2, varValue, sizeof( varValue ) );
	trap_Argv( 3, varType, sizeof( varType ) );
  
  	if(!variable_exists(varName)){
		create_variable(varName, varValue, atoi(varType));
	}

	set_variable_value(varName, varValue, atoi(varType));
}
