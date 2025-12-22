data
export net_error_count
align 4
LABELV net_error_count
byte 4 0
code
proc CG_ResetEntity 24 4
ADDRFP4 0
ADDRFP4 0
INDIRP4
ASGNP4
ADDRFP4 0
INDIRP4
CNSTI4 476
ADDP4
INDIRI4
ADDRGP4 cg+1835980
INDIRI4
CNSTI4 300
SUBI4
GEI4 $76
ADDRFP4 0
INDIRP4
CNSTI4 460
ADDP4
CNSTI4 0
ASGNI4
LABELV $76
ADDRFP4 0
INDIRP4
CNSTI4 464
ADDP4
ADDRGP4 cg+28
INDIRP4
CNSTI4 8
ADDP4
INDIRI4
ASGNI4
ADDRFP4 0
INDIRP4
CNSTI4 724
ADDP4
ADDRFP4 0
INDIRP4
CNSTI4 92
ADDP4
INDIRF4
ASGNF4
ADDRFP4 0
INDIRP4
CNSTI4 728
ADDP4
ADDRFP4 0
INDIRP4
CNSTI4 96
ADDP4
INDIRF4
ASGNF4
ADDRFP4 0
INDIRP4
CNSTI4 732
ADDP4
ADDRFP4 0
INDIRP4
CNSTI4 100
ADDP4
INDIRF4
ASGNF4
ADDRFP4 0
INDIRP4
CNSTI4 736
ADDP4
ADDRFP4 0
INDIRP4
CNSTI4 128
ADDP4
INDIRF4
ASGNF4
ADDRFP4 0
INDIRP4
CNSTI4 740
ADDP4
ADDRFP4 0
INDIRP4
CNSTI4 132
ADDP4
INDIRF4
ASGNF4
ADDRFP4 0
INDIRP4
CNSTI4 744
ADDP4
ADDRFP4 0
INDIRP4
CNSTI4 136
ADDP4
INDIRF4
ASGNF4
ADDRFP4 0
INDIRP4
CNSTI4 4
ADDP4
INDIRI4
CNSTI4 1
NEI4 $80
ADDRFP4 0
INDIRP4
ARGP4
ADDRGP4 CG_ResetPlayerEntity
CALLV
pop
LABELV $80
LABELV $75
endproc CG_ResetEntity 24 4
export CG_TransitionEntity
proc CG_TransitionEntity 4 4
ADDRFP4 0
ADDRFP4 0
INDIRP4
ASGNP4
ADDRFP4 0
INDIRP4
ADDRFP4 0
INDIRP4
CNSTI4 224
ADDP4
INDIRB
ASGNB 224
ADDRFP4 0
INDIRP4
CNSTI4 452
ADDP4
CNSTI4 1
ASGNI4
ADDRFP4 0
INDIRP4
CNSTI4 448
ADDP4
INDIRI4
CNSTI4 0
NEI4 $83
ADDRFP4 0
INDIRP4
ARGP4
ADDRGP4 CG_ResetEntity
CALLV
pop
LABELV $83
ADDRFP4 0
INDIRP4
CNSTI4 448
ADDP4
CNSTI4 0
ASGNI4
ADDRFP4 0
INDIRP4
ARGP4
ADDRGP4 CG_CheckEvents
CALLV
pop
LABELV $82
endproc CG_TransitionEntity 4 4
proc CG_SetInitialSnapshot 24 12
ADDRFP4 0
ADDRFP4 0
INDIRP4
ASGNP4
ADDRGP4 cg+28
ADDRFP4 0
INDIRP4
ASGNP4
ADDRFP4 0
INDIRP4
CNSTI4 44
ADDP4
ARGP4
CNSTI4 772
ADDRFP4 0
INDIRP4
CNSTI4 184
ADDP4
INDIRI4
MULI4
ADDRGP4 cg_entities
ADDP4
ARGP4
CNSTI4 0
ARGI4
ADDRGP4 BG_PlayerStateToEntityState
CALLV
pop
ADDRGP4 CG_BuildSolidList
CALLV
pop
ADDRFP4 0
INDIRP4
CNSTI4 917960
ADDP4
INDIRI4
ARGI4
ADDRGP4 CG_ExecuteNewServerCommands
CALLV
pop
ADDRGP4 CG_Respawn
CALLV
pop
ADDRLP4 4
CNSTI4 0
ASGNI4
ADDRGP4 $90
JUMPV
LABELV $87
ADDRLP4 16
CNSTI4 224
ASGNI4
ADDRLP4 8
ADDRLP4 16
INDIRI4
ADDRLP4 4
INDIRI4
MULI4
ADDRGP4 cg+28
INDIRP4
CNSTI4 452
ADDP4
ADDP4
ASGNP4
ADDRLP4 0
CNSTI4 772
ADDRLP4 8
INDIRP4
INDIRI4
MULI4
ADDRGP4 cg_entities
ADDP4
ASGNP4
ADDRLP4 0
INDIRP4
ARGP4
ADDRLP4 8
INDIRP4
ARGP4
ADDRLP4 16
INDIRI4
ARGI4
ADDRGP4 memcpy
CALLP4
pop
ADDRLP4 0
INDIRP4
CNSTI4 448
ADDP4
CNSTI4 0
ASGNI4
ADDRLP4 0
INDIRP4
CNSTI4 452
ADDP4
CNSTI4 1
ASGNI4
ADDRLP4 0
INDIRP4
ARGP4
ADDRGP4 CG_ResetEntity
CALLV
pop
ADDRLP4 0
INDIRP4
ARGP4
ADDRGP4 CG_CheckEvents
CALLV
pop
LABELV $88
ADDRLP4 4
ADDRLP4 4
INDIRI4
CNSTI4 1
ADDI4
ASGNI4
LABELV $90
ADDRLP4 4
INDIRI4
ADDRGP4 cg+28
INDIRP4
CNSTI4 448
ADDP4
INDIRI4
LTI4 $87
LABELV $85
endproc CG_SetInitialSnapshot 24 12
proc CG_TransitionSnapshot 32 12
ADDRGP4 cg+28
INDIRP4
CVPU4 4
CNSTU4 0
NEU4 $94
ADDRGP4 $97
ARGP4
ADDRGP4 CG_Error
CALLV
pop
LABELV $94
ADDRGP4 cg+32
INDIRP4
CVPU4 4
CNSTU4 0
NEU4 $98
ADDRGP4 $101
ARGP4
ADDRGP4 CG_Error
CALLV
pop
LABELV $98
ADDRGP4 cg+32
INDIRP4
CNSTI4 917960
ADDP4
INDIRI4
ARGI4
ADDRGP4 CG_ExecuteNewServerCommands
CALLV
pop
ADDRLP4 0
CNSTI4 0
ASGNI4
ADDRGP4 $106
JUMPV
LABELV $103
ADDRLP4 12
CNSTI4 452
ASGNI4
ADDRLP4 4
CNSTI4 772
CNSTI4 224
ADDRLP4 0
INDIRI4
MULI4
ADDRGP4 cg+28
INDIRP4
ADDRLP4 12
INDIRI4
ADDP4
ADDP4
INDIRI4
MULI4
ADDRGP4 cg_entities
ADDP4
ASGNP4
ADDRLP4 4
INDIRP4
ADDRLP4 12
INDIRI4
ADDP4
CNSTI4 0
ASGNI4
LABELV $104
ADDRLP4 0
ADDRLP4 0
INDIRI4
CNSTI4 1
ADDI4
ASGNI4
LABELV $106
ADDRLP4 0
INDIRI4
ADDRGP4 cg+28
INDIRP4
CNSTI4 448
ADDP4
INDIRI4
LTI4 $103
ADDRLP4 8
ADDRGP4 cg+28
INDIRP4
ASGNP4
ADDRGP4 cg+28
ADDRGP4 cg+32
INDIRP4
ASGNP4
ADDRGP4 cg+28
INDIRP4
CNSTI4 44
ADDP4
ARGP4
CNSTI4 772
ADDRGP4 cg+28
INDIRP4
CNSTI4 184
ADDP4
INDIRI4
MULI4
ADDRGP4 cg_entities
ADDP4
ARGP4
CNSTI4 0
ARGI4
ADDRGP4 BG_PlayerStateToEntityState
CALLV
pop
CNSTI4 772
ADDRGP4 cg+28
INDIRP4
CNSTI4 184
ADDP4
INDIRI4
MULI4
ADDRGP4 cg_entities+448
ADDP4
CNSTI4 0
ASGNI4
ADDRLP4 0
CNSTI4 0
ASGNI4
ADDRGP4 $119
JUMPV
LABELV $116
ADDRLP4 4
CNSTI4 772
CNSTI4 224
ADDRLP4 0
INDIRI4
MULI4
ADDRGP4 cg+28
INDIRP4
CNSTI4 452
ADDP4
ADDP4
INDIRI4
MULI4
ADDRGP4 cg_entities
ADDP4
ASGNP4
ADDRLP4 4
INDIRP4
ARGP4
ADDRGP4 CG_TransitionEntity
CALLV
pop
ADDRLP4 4
INDIRP4
CNSTI4 476
ADDP4
ADDRGP4 cg+28
INDIRP4
CNSTI4 8
ADDP4
INDIRI4
ASGNI4
LABELV $117
ADDRLP4 0
ADDRLP4 0
INDIRI4
CNSTI4 1
ADDI4
ASGNI4
LABELV $119
ADDRLP4 0
INDIRI4
ADDRGP4 cg+28
INDIRP4
CNSTI4 448
ADDP4
INDIRI4
LTI4 $116
ADDRGP4 cg+32
CNSTP4 0
ASGNP4
ADDRLP4 8
INDIRP4
CVPU4 4
CNSTU4 0
EQU4 $124
ADDRLP4 20
CNSTI4 44
ASGNI4
ADDRLP4 12
ADDRLP4 8
INDIRP4
ADDRLP4 20
INDIRI4
ADDP4
ASGNP4
ADDRLP4 16
ADDRGP4 cg+28
INDIRP4
ADDRLP4 20
INDIRI4
ADDP4
ASGNP4
ADDRLP4 24
CNSTI4 104
ASGNI4
ADDRLP4 16
INDIRP4
ADDRLP4 24
INDIRI4
ADDP4
INDIRI4
ADDRLP4 12
INDIRP4
ADDRLP4 24
INDIRI4
ADDP4
INDIRI4
BXORI4
CNSTI4 4
BANDI4
CNSTI4 0
EQI4 $127
ADDRGP4 cg+1835968
CNSTI4 1
ASGNI4
LABELV $127
ADDRLP4 28
CNSTI4 0
ASGNI4
ADDRGP4 cg+8
INDIRI4
ADDRLP4 28
INDIRI4
NEI4 $134
ADDRGP4 cg+28
INDIRP4
CNSTI4 56
ADDP4
INDIRI4
CNSTI4 1024
BANDI4
ADDRLP4 28
INDIRI4
EQI4 $130
LABELV $134
ADDRLP4 16
INDIRP4
ARGP4
ADDRLP4 12
INDIRP4
ARGP4
ADDRGP4 CG_TransitionPlayerState
CALLV
pop
LABELV $130
LABELV $124
LABELV $93
endproc CG_TransitionSnapshot 32 12
proc CG_SetNextSnap 36 12
ADDRFP4 0
ADDRFP4 0
INDIRP4
ASGNP4
ADDRGP4 cg+32
ADDRFP4 0
INDIRP4
ASGNP4
ADDRFP4 0
INDIRP4
CNSTI4 44
ADDP4
ARGP4
CNSTI4 772
ADDRFP4 0
INDIRP4
CNSTI4 184
ADDP4
INDIRI4
MULI4
ADDRGP4 cg_entities+224
ADDP4
ARGP4
CNSTI4 0
ARGI4
ADDRGP4 BG_PlayerStateToEntityState
CALLV
pop
CNSTI4 772
ADDRGP4 cg+28
INDIRP4
CNSTI4 184
ADDP4
INDIRI4
MULI4
ADDRGP4 cg_entities+448
ADDP4
CNSTI4 1
ASGNI4
ADDRLP4 8
CNSTI4 0
ASGNI4
ADDRGP4 $143
JUMPV
LABELV $140
ADDRLP4 16
CNSTI4 224
ASGNI4
ADDRLP4 4
ADDRLP4 16
INDIRI4
ADDRLP4 8
INDIRI4
MULI4
ADDRFP4 0
INDIRP4
CNSTI4 452
ADDP4
ADDP4
ASGNP4
ADDRLP4 0
CNSTI4 772
ADDRLP4 4
INDIRP4
INDIRI4
MULI4
ADDRGP4 cg_entities
ADDP4
ASGNP4
ADDRLP4 0
INDIRP4
ADDRLP4 16
INDIRI4
ADDP4
ARGP4
ADDRLP4 4
INDIRP4
ARGP4
ADDRLP4 16
INDIRI4
ARGI4
ADDRGP4 memcpy
CALLP4
pop
ADDRLP4 28
CNSTI4 0
ASGNI4
ADDRLP4 0
INDIRP4
CNSTI4 452
ADDP4
INDIRI4
ADDRLP4 28
INDIRI4
EQI4 $146
ADDRLP4 32
CNSTI4 8
ASGNI4
ADDRLP4 0
INDIRP4
ADDRLP4 32
INDIRI4
ADDP4
INDIRI4
ADDRLP4 4
INDIRP4
ADDRLP4 32
INDIRI4
ADDP4
INDIRI4
BXORI4
CNSTI4 4
BANDI4
ADDRLP4 28
INDIRI4
EQI4 $144
LABELV $146
ADDRLP4 0
INDIRP4
CNSTI4 448
ADDP4
CNSTI4 0
ASGNI4
ADDRGP4 $145
JUMPV
LABELV $144
ADDRLP4 0
INDIRP4
CNSTI4 448
ADDP4
CNSTI4 1
ASGNI4
LABELV $145
LABELV $141
ADDRLP4 8
ADDRLP4 8
INDIRI4
CNSTI4 1
ADDI4
ASGNI4
LABELV $143
ADDRLP4 8
INDIRI4
ADDRFP4 0
INDIRP4
CNSTI4 448
ADDP4
INDIRI4
LTI4 $140
ADDRGP4 cg+28
INDIRP4
CVPU4 4
CNSTU4 0
EQU4 $147
ADDRLP4 16
CNSTI4 148
ASGNI4
ADDRFP4 0
INDIRP4
ADDRLP4 16
INDIRI4
ADDP4
INDIRI4
ADDRGP4 cg+28
INDIRP4
ADDRLP4 16
INDIRI4
ADDP4
INDIRI4
BXORI4
CNSTI4 4
BANDI4
CNSTI4 0
EQI4 $147
ADDRGP4 cg+1835972
CNSTI4 1
ASGNI4
ADDRGP4 $148
JUMPV
LABELV $147
ADDRGP4 cg+1835972
CNSTI4 0
ASGNI4
LABELV $148
ADDRLP4 20
CNSTI4 184
ASGNI4
ADDRGP4 cg+32
INDIRP4
ADDRLP4 20
INDIRI4
ADDP4
INDIRI4
ADDRGP4 cg+28
INDIRP4
ADDRLP4 20
INDIRI4
ADDP4
INDIRI4
EQI4 $153
ADDRGP4 cg+1835972
CNSTI4 1
ASGNI4
LABELV $153
ADDRGP4 cg+32
INDIRP4
INDIRI4
ADDRGP4 cg+28
INDIRP4
INDIRI4
BXORI4
CNSTI4 4
BANDI4
CNSTI4 0
EQI4 $158
ADDRGP4 cg+1835972
CNSTI4 1
ASGNI4
LABELV $158
ADDRGP4 CG_BuildSolidList
CALLV
pop
LABELV $135
endproc CG_SetNextSnap 36 12
proc CG_ReadNextSnapshot 16 12
ADDRGP4 cg+20
INDIRI4
ADDRGP4 cgs+71956
INDIRI4
CNSTI4 1000
ADDI4
LEI4 $172
ADDRGP4 $168
ARGP4
ADDRGP4 cg+20
INDIRI4
ARGI4
ADDRGP4 cgs+71956
INDIRI4
ARGI4
ADDRGP4 CG_Printf
CALLV
pop
ADDRGP4 $172
JUMPV
LABELV $171
ADDRGP4 cg+28
INDIRP4
CVPU4 4
ADDRGP4 cg+36
CVPU4 4
NEU4 $176
ADDRLP4 0
ADDRGP4 cg+36+917964
ASGNP4
ADDRGP4 $177
JUMPV
LABELV $176
ADDRLP4 0
ADDRGP4 cg+36
ASGNP4
LABELV $177
ADDRLP4 8
ADDRGP4 cgs+71956
ASGNP4
ADDRLP4 8
INDIRP4
ADDRLP4 8
INDIRP4
INDIRI4
CNSTI4 1
ADDI4
ASGNI4
ADDRGP4 cgs+71956
INDIRI4
ARGI4
ADDRLP4 0
INDIRP4
ARGP4
ADDRLP4 12
ADDRGP4 trap_GetSnapshot
CALLI4
ASGNI4
ADDRLP4 4
ADDRLP4 12
INDIRI4
ASGNI4
ADDRLP4 4
INDIRI4
CNSTI4 0
EQI4 $185
ADDRGP4 net_error_count
CNSTI4 0
ASGNI4
ADDRLP4 0
INDIRP4
RETP4
ADDRGP4 $163
JUMPV
LABELV $185
LABELV $172
ADDRGP4 cgs+71956
INDIRI4
ADDRGP4 cg+20
INDIRI4
LTI4 $171
ADDRLP4 8
ADDRGP4 net_error_count
ASGNP4
ADDRLP4 8
INDIRP4
ADDRLP4 8
INDIRP4
INDIRI4
CNSTI4 1
ADDI4
ASGNI4
ADDRLP4 8
INDIRP4
INDIRI4
CNSTI4 16
LTI4 $187
CNSTI4 1
ARGI4
ADDRGP4 $189
ARGP4
ADDRGP4 trap_Cmd
CALLV
pop
ADDRGP4 $190
ARGP4
ADDRGP4 $191
ARGP4
ADDRGP4 cvarSet
CALLV
pop
ADDRGP4 net_error_count
CNSTI4 0
ASGNI4
LABELV $187
CNSTP4 0
RETP4
LABELV $163
endproc CG_ReadNextSnapshot 16 12
export CG_ProcessSnapshots
proc CG_ProcessSnapshots 16 8
ADDRLP4 4
ARGP4
ADDRGP4 cg+24
ARGP4
ADDRGP4 trap_GetCurrentSnapshotNumber
CALLV
pop
ADDRLP4 4
INDIRI4
ADDRGP4 cg+20
INDIRI4
EQI4 $203
ADDRLP4 4
INDIRI4
ADDRGP4 cg+20
INDIRI4
GEI4 $197
ADDRGP4 $200
ARGP4
ADDRGP4 CG_Error
CALLV
pop
LABELV $197
ADDRGP4 cg+20
ADDRLP4 4
INDIRI4
ASGNI4
ADDRGP4 $203
JUMPV
LABELV $202
ADDRLP4 8
ADDRGP4 CG_ReadNextSnapshot
CALLP4
ASGNP4
ADDRLP4 0
ADDRLP4 8
INDIRP4
ASGNP4
ADDRLP4 0
INDIRP4
CVPU4 4
CNSTU4 0
NEU4 $206
ADDRGP4 $192
JUMPV
LABELV $206
ADDRLP4 0
INDIRP4
INDIRI4
CNSTI4 2
BANDI4
CNSTI4 0
NEI4 $208
ADDRLP4 0
INDIRP4
ARGP4
ADDRGP4 CG_SetInitialSnapshot
CALLV
pop
LABELV $208
LABELV $203
ADDRGP4 cg+28
INDIRP4
CVPU4 4
CNSTU4 0
EQU4 $202
LABELV $210
ADDRGP4 cg+32
INDIRP4
CVPU4 4
CNSTU4 0
NEU4 $213
ADDRLP4 8
ADDRGP4 CG_ReadNextSnapshot
CALLP4
ASGNP4
ADDRLP4 0
ADDRLP4 8
INDIRP4
ASGNP4
ADDRLP4 0
INDIRP4
CVPU4 4
CNSTU4 0
NEU4 $216
ADDRGP4 $212
JUMPV
LABELV $216
ADDRLP4 0
INDIRP4
ARGP4
ADDRGP4 CG_SetNextSnap
CALLV
pop
ADDRLP4 12
CNSTI4 8
ASGNI4
ADDRGP4 cg+32
INDIRP4
ADDRLP4 12
INDIRI4
ADDP4
INDIRI4
ADDRGP4 cg+28
INDIRP4
ADDRLP4 12
INDIRI4
ADDP4
INDIRI4
GEI4 $218
ADDRGP4 $222
ARGP4
ADDRGP4 CG_Error
CALLV
pop
LABELV $218
LABELV $213
ADDRLP4 8
CNSTI4 8
ASGNI4
ADDRGP4 cg+1835980
INDIRI4
ADDRGP4 cg+28
INDIRP4
ADDRLP4 8
INDIRI4
ADDP4
INDIRI4
LTI4 $223
ADDRGP4 cg+1835980
INDIRI4
ADDRGP4 cg+32
INDIRP4
ADDRLP4 8
INDIRI4
ADDP4
INDIRI4
GEI4 $223
ADDRGP4 $212
JUMPV
LABELV $223
ADDRGP4 CG_TransitionSnapshot
CALLV
pop
LABELV $211
ADDRGP4 $210
JUMPV
LABELV $212
ADDRGP4 cg+28
INDIRP4
CVPU4 4
CNSTU4 0
NEU4 $229
ADDRGP4 $232
ARGP4
ADDRGP4 CG_Error
CALLV
pop
LABELV $229
ADDRGP4 cg+1835980
INDIRI4
ADDRGP4 cg+28
INDIRP4
CNSTI4 8
ADDP4
INDIRI4
GEI4 $233
ADDRGP4 cg+1835980
ADDRGP4 cg+28
INDIRP4
CNSTI4 8
ADDP4
INDIRI4
ASGNI4
LABELV $233
ADDRGP4 cg+32
INDIRP4
CVPU4 4
CNSTU4 0
EQU4 $239
ADDRGP4 cg+32
INDIRP4
CNSTI4 8
ADDP4
INDIRI4
ADDRGP4 cg+1835980
INDIRI4
GTI4 $239
ADDRGP4 $244
ARGP4
ADDRGP4 CG_Error
CALLV
pop
LABELV $239
LABELV $192
endproc CG_ProcessSnapshots 16 8
import JSCall
import JSEval
import JSLoadScripts
import JSOpenFile
import VMContext
import jsresult
import jsargs
import vmresult
import vmargs
import JS_Test
import JS_Function
import JS_StackClean
import trap_ImportOBJ
import trap_S_StopLoopingSound
import trap_S_AddRealLoopingSound
import trap_Key_GetKey
import trap_SetUserCmdValue
import trap_GetUserCmd
import trap_GetCurrentCmdNumber
import trap_GetServerCommand
import trap_GetSnapshot
import trap_GetCurrentSnapshotNumber
import trap_GetGameState
import trap_R_ModelBounds
import trap_R_LoadWorldMap
import trap_S_StartBackgroundTrack
import trap_S_Respatialize
import trap_S_UpdateEntityPosition
import trap_S_AddLoopingSound
import trap_S_ClearLoopingSounds
import trap_S_StartSound
import trap_CM_MarkFragments
import trap_CM_TransformedBoxTrace
import trap_CM_BoxTrace
import trap_CM_TransformedPointContents
import trap_CM_PointContents
import trap_CM_TempBoxModel
import trap_CM_InlineModel
import trap_CM_NumInlineModels
import trap_CM_LoadMap
import trap_SendClientCommand
import trap_AddCommand
import CG_Bullet
import CG_ShotgunFire
import CG_ShotgunPattern
import CG_MissileHitPlayer
import CG_MissileHitWall
import CG_FireWeapon
import CG_Weapon_f
import CG_PrevWeapon_f
import CG_NextWeapon_f
import CG_AddViewWeapon
import CG_AddPlayerWeapon
import CG_RegisterItemVisuals
import CG_RegisterWeapon
import CG_GrappleTrail
import CG_RailTrail
import CG_DrawActiveFrame
import CG_AddBufferedSound
import CG_ZoomUp_f
import CG_ZoomDown_f
import CG_PredictWeaponEffects
import CG_ExecuteNewServerCommands
import CG_SetConfigValues
import CG_ParseServerinfo
import CG_DrawScoreboard
import CG_PredictPlayerState
import CG_PointContents
import CG_Trace
import CG_BuildSolidList
import CG_TransitionPlayerState
import CG_Respawn
import CG_ResetPlayerEntity
import CG_Player
import CG_ReloadPlayers
import CG_NewClientInfo
import CG_CustomSound
import CG_AddMarks
import CG_ImpactMark
import CG_InitMarkPolys
import CG_ConfigString
import CG_Argv
import CG_Error
import CG_Printf
import CG_PrintfChat
import CG_AddLocalEntities
import CG_AllocLocalEntity
import CG_InitLocalEntities
import CG_DrawInformation
import CG_LoadingClient
import CG_LoadingString
import CG_CheckEvents
import CG_EntityEvent
import CG_PainEvent
import CG_AddPacketEntities
import CG_SetEntitySoundPosition
import CG_PositionRotatedEntityOnTag
import CG_ParticlesFromEntityState
import CG_GibPlayer
import CG_Bleed
import CG_MakeExplosion
import CG_InvulnerabilityJuiced
import CG_InvulnerabilityImpact
import CG_ObeliskPain
import CG_ObeliskExplode
import CG_KamikazeEffect
import CG_LightningBoltBeam
import CG_SpawnEffect
import CG_SmokePuff
import CG_BubbleTrail
import CG_FadeColor
import CG_Draw3DString
import CG_DrawPic
import CG_DrawProgressBar
import CG_DrawRoundedRect
import CG_AdjustFrom640
import CG_DrawActive
import CG_AddNotify
import CG_CenterPrint
import CG_AddToGenericConsole
import CG_DrawHead
import CG_Add3DString
import CG_InitConsoleCommands
import CG_ConsoleCommand
import mod_gravity
import mod_jumpheight
import cg_markPolys
import cg_items
import cg_weapons
import cg_entities
import cg
import cgs
import Pmove
import PM_UpdateViewAngles
import BG_InVehicle
import BG_PlayerStateToEntityState
import BG_TouchJumpPad
import BG_AddPredictableEventToPlayerstate
import ST_EvaluateTrajectoryDelta
import ST_EvaluateTrajectory
import BG_EvaluateTrajectoryDelta
import BG_EvaluateTrajectory
import BG_CanItemBeGrabbed
import BG_PlayerTouchesItem
import BG_FindAmmo
import BG_FindWeapon
import BG_CheckClassname
import BG_FindItem
import BG_FindItemForWeapon
import BG_FindItemForHoldable
import BG_FindItemForPowerup
import BG_FactionShouldAttack
import BG_FindNPCTypeID
import G_InitMemory
import G_Alloc
import gameInfoSandboxSpawnsNum
import gameInfoSandboxSpawns
import gameInfoFactionsNum
import gameInfoFactions
import gameInfoNPCTypesNum
import gameInfoNPCTypes
import gameInfoWPropsNum
import gameInfoWProps
import gameInfoWeaponsNum
import gameInfoWeapons
import gameInfoItemsNum
import gameInfoItems
import gametypes_mapnames
import gametypes_names
import trap_R_RemapShader
import trap_Key_GetCatcher
import trap_GetGlconfig
import trap_R_LerpTag
import trap_R_DrawStretchPic
import trap_R_SetColor
import trap_R_RenderScene
import trap_R_AddLinearLightToScene
import trap_R_AddLightToScene
import trap_R_AddPolyToScene
import trap_R_AddRefEntityToScene
import trap_R_ClearScene
import trap_R_RegisterShaderNoMip
import trap_R_RegisterShader
import trap_R_RegisterSkin
import trap_R_RegisterModel
import trap_S_RegisterSound
import trap_S_StartLocalSound
import trap_UpdateScreen
import glconfig
import strncpy
import memcpy
import memset
import trap_System
import trap_RealTime
import trap_Cmd
import FS_List
import FS_Close
import FS_Write
import FS_Read
import FS_Open
import trap_Args
import trap_Argv
import trap_Argc
import cvarSet
import cvarReload
import cvarUpdate
import cvarID
import cvarRegister
import trap_Milliseconds
import trap_Error
import trap_Print
import ST_AnimValue
import ST_AnimStart
import ST_DrawString
import ST_StringWidth
import ST_DrawChar
import ST_StringCount
import ST_UpdateColors
import ST_RegisterFont
import ST_ColorEscapes
import defaultFont
import anim_weaponSelect
import weaponSelectOut
import weaponSelectIn
import customcolor_crosshair
import color_highlight
import color_select
import color_disabled
import color_dim
import color_grey
import color_white
import color_black
import VectorDistance
import Com_Printf
import Com_Error
import Info_NextPair
import Info_Validate
import Info_SetValueForKey_Big
import Info_SetValueForKey
import Info_RemoveKey_big
import Info_RemoveKey
import Info_ValueForKey
import VectorToYaw
import AngleDifference
import va
import Q_CleanStr
import Q_strcat
import Q_strncpyz
import Q_stristr
import Q_strupr
import Q_strlwr
import Q_stricmpn
import Q_strncmp
import Q_stricmp
import Q_isalpha
import Q_isupper
import Q_islower
import Q_isprint
import Com_sprintf
import COM_Compress
import COM_ParseExt
import COM_Parse
import COM_DefaultExtension
import COM_StripExtension
import COM_GetExtension
import COM_SkipPath
import Com_Clamp
import SnapVectorTowards
import PerpendicularVector
import AngleVectors
import MatrixMultiply
import RotateAroundDirection
import RotatePointAroundVector
import ProjectPointOnPlane
import PlaneFromPoints
import AngleDelta
import AngleNormalize180
import AngleNormalize360
import LerpAngles
import AngleMA
import AngleAdd
import AnglesSubtract
import AngleSubtract
import LerpAngle
import AngleMod
import AxisCopy
import AxisClear
import Lerp
import VelocityToAxis
import OrthogonalizeMatrix
import AxisToAngles
import AnglesToAxis
import vectoangles
import Q_crandom
import Q_random
import Q_rand
import Vector4Scale
import VectorNormalize2
import VectorNormalize
import CrossProduct
import VectorInverse
import VectorNormalizeFast
import DistanceSquared
import Distance
import VectorLengthSquared
import VectorLength
import VectorCompare
import AddPointToBounds
import RadiusFromBounds
import NormalizeColor
import _VectorMA
import _VectorScale
import _VectorCopy
import _VectorAdd
import _VectorSubtract
import _DotProduct
import ByteToDir
import DirToByte
import Q_rsqrt
import Q_fabs
import axisDefault
import vec3_origin
import g_color_table
import bytedirs
import cvarString
import cvarFloat
import cvarInt
import ST_UpdateCvars
import ST_RegisterCvars
import FloatSwap
import LongSwap
import ShortSwap
import sqrt
import atan2
import acos
import cos
import sin
import sscanf
import Q_snprintf
import Q_vsnprintf
import fabs
import abs
import atoi
import atof
import rand
import srand
import tan
import memmove
import toupper
import tolower
import strstr
import strrchr
import strchr
import strcmp
import strcpy
import strcat
import ifstrlenru
import strlenru
import strlen
import qsort
lit
align 1
LABELV $244
byte 1 67
byte 1 71
byte 1 95
byte 1 80
byte 1 114
byte 1 111
byte 1 99
byte 1 101
byte 1 115
byte 1 115
byte 1 83
byte 1 110
byte 1 97
byte 1 112
byte 1 115
byte 1 104
byte 1 111
byte 1 116
byte 1 115
byte 1 58
byte 1 32
byte 1 99
byte 1 103
byte 1 46
byte 1 110
byte 1 101
byte 1 120
byte 1 116
byte 1 83
byte 1 110
byte 1 97
byte 1 112
byte 1 45
byte 1 62
byte 1 115
byte 1 101
byte 1 114
byte 1 118
byte 1 101
byte 1 114
byte 1 84
byte 1 105
byte 1 109
byte 1 101
byte 1 32
byte 1 60
byte 1 61
byte 1 32
byte 1 99
byte 1 103
byte 1 46
byte 1 116
byte 1 105
byte 1 109
byte 1 101
byte 1 0
align 1
LABELV $232
byte 1 67
byte 1 71
byte 1 95
byte 1 80
byte 1 114
byte 1 111
byte 1 99
byte 1 101
byte 1 115
byte 1 115
byte 1 83
byte 1 110
byte 1 97
byte 1 112
byte 1 115
byte 1 104
byte 1 111
byte 1 116
byte 1 115
byte 1 58
byte 1 32
byte 1 99
byte 1 103
byte 1 46
byte 1 115
byte 1 110
byte 1 97
byte 1 112
byte 1 32
byte 1 61
byte 1 61
byte 1 32
byte 1 78
byte 1 85
byte 1 76
byte 1 76
byte 1 0
align 1
LABELV $222
byte 1 67
byte 1 71
byte 1 95
byte 1 80
byte 1 114
byte 1 111
byte 1 99
byte 1 101
byte 1 115
byte 1 115
byte 1 83
byte 1 110
byte 1 97
byte 1 112
byte 1 115
byte 1 104
byte 1 111
byte 1 116
byte 1 115
byte 1 58
byte 1 32
byte 1 83
byte 1 101
byte 1 114
byte 1 118
byte 1 101
byte 1 114
byte 1 32
byte 1 116
byte 1 105
byte 1 109
byte 1 101
byte 1 32
byte 1 119
byte 1 101
byte 1 110
byte 1 116
byte 1 32
byte 1 98
byte 1 97
byte 1 99
byte 1 107
byte 1 119
byte 1 97
byte 1 114
byte 1 100
byte 1 115
byte 1 0
align 1
LABELV $200
byte 1 67
byte 1 71
byte 1 95
byte 1 80
byte 1 114
byte 1 111
byte 1 99
byte 1 101
byte 1 115
byte 1 115
byte 1 83
byte 1 110
byte 1 97
byte 1 112
byte 1 115
byte 1 104
byte 1 111
byte 1 116
byte 1 115
byte 1 58
byte 1 32
byte 1 110
byte 1 32
byte 1 60
byte 1 32
byte 1 99
byte 1 103
byte 1 46
byte 1 108
byte 1 97
byte 1 116
byte 1 101
byte 1 115
byte 1 116
byte 1 83
byte 1 110
byte 1 97
byte 1 112
byte 1 115
byte 1 104
byte 1 111
byte 1 116
byte 1 78
byte 1 117
byte 1 109
byte 1 0
align 1
LABELV $191
byte 1 51
byte 1 0
align 1
LABELV $190
byte 1 115
byte 1 99
byte 1 114
byte 1 105
byte 1 112
byte 1 116
byte 1 46
byte 1 101
byte 1 114
byte 1 114
byte 1 111
byte 1 114
byte 1 0
align 1
LABELV $189
byte 1 110
byte 1 101
byte 1 116
byte 1 101
byte 1 114
byte 1 114
byte 1 111
byte 1 114
byte 1 0
align 1
LABELV $168
byte 1 87
byte 1 65
byte 1 82
byte 1 78
byte 1 73
byte 1 78
byte 1 71
byte 1 58
byte 1 32
byte 1 67
byte 1 71
byte 1 95
byte 1 82
byte 1 101
byte 1 97
byte 1 100
byte 1 78
byte 1 101
byte 1 120
byte 1 116
byte 1 83
byte 1 110
byte 1 97
byte 1 112
byte 1 115
byte 1 104
byte 1 111
byte 1 116
byte 1 58
byte 1 32
byte 1 119
byte 1 97
byte 1 121
byte 1 32
byte 1 111
byte 1 117
byte 1 116
byte 1 32
byte 1 111
byte 1 102
byte 1 32
byte 1 114
byte 1 97
byte 1 110
byte 1 103
byte 1 101
byte 1 44
byte 1 32
byte 1 37
byte 1 105
byte 1 32
byte 1 62
byte 1 32
byte 1 37
byte 1 105
byte 1 0
align 1
LABELV $101
byte 1 67
byte 1 71
byte 1 95
byte 1 84
byte 1 114
byte 1 97
byte 1 110
byte 1 115
byte 1 105
byte 1 116
byte 1 105
byte 1 111
byte 1 110
byte 1 83
byte 1 110
byte 1 97
byte 1 112
byte 1 115
byte 1 104
byte 1 111
byte 1 116
byte 1 58
byte 1 32
byte 1 78
byte 1 85
byte 1 76
byte 1 76
byte 1 32
byte 1 99
byte 1 103
byte 1 46
byte 1 110
byte 1 101
byte 1 120
byte 1 116
byte 1 83
byte 1 110
byte 1 97
byte 1 112
byte 1 0
align 1
LABELV $97
byte 1 67
byte 1 71
byte 1 95
byte 1 84
byte 1 114
byte 1 97
byte 1 110
byte 1 115
byte 1 105
byte 1 116
byte 1 105
byte 1 111
byte 1 110
byte 1 83
byte 1 110
byte 1 97
byte 1 112
byte 1 115
byte 1 104
byte 1 111
byte 1 116
byte 1 58
byte 1 32
byte 1 78
byte 1 85
byte 1 76
byte 1 76
byte 1 32
byte 1 99
byte 1 103
byte 1 46
byte 1 115
byte 1 110
byte 1 97
byte 1 112
byte 1 0
