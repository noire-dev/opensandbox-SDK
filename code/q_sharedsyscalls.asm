code

equ trap_Print                              -1001
equ trap_Error                              -1002
equ trap_Milliseconds                       -1003
equ cvarRegister                            -1004
equ cvarID                                  -1005
equ cvarUpdate                              -1006
equ cvarReload                              -1007
equ cvarSet                                 -1008
equ	trap_Argc								-1009
equ	trap_Argv								-1010
equ	trap_Args								-1011
equ	FS_Open								    -1012
equ	FS_Read								    -1013
equ	FS_Write								-1014
equ	FS_Close								-1015
equ	FS_List								    -1016
equ trap_Cmd                                -1017
equ trap_RealTime                           -1018
equ trap_System                             -1019
equ VMContext                               -1020
equ JSOpenFile                              -1021
equ JSLoadScripts                           -1022
equ JSEval                                  -1023
equ JSCall                                  -1024

equ	memset						            -3001
equ	memcpy						            -3002
equ	strncpy                                 -3003
equ	sin							            -3004
equ	cos							            -3005
equ acos                                    -3006
equ	atan2						            -3007
equ	sqrt						            -3008