code
proc CG_DamageFeedback 84 16
ADDRLP4 16
ADDRGP4 cg+28
INDIRP4
CNSTI4 228
ADDP4
INDIRI4
ASGNI4
ADDRLP4 16
INDIRI4
CNSTI4 40
GEI4 $77
ADDRLP4 24
CNSTF4 1065353216
ASGNF4
ADDRGP4 $78
JUMPV
LABELV $77
ADDRLP4 24
CNSTF4 1109393408
ADDRLP4 16
INDIRI4
CVIF4 4
DIVF4
ASGNF4
LABELV $78
ADDRLP4 12
ADDRFP4 8
INDIRI4
CVIF4 4
ADDRLP4 24
INDIRF4
MULF4
ASGNF4
ADDRLP4 12
INDIRF4
CNSTF4 1084227584
GEF4 $79
ADDRLP4 12
CNSTF4 1084227584
ASGNF4
LABELV $79
ADDRLP4 12
INDIRF4
CNSTF4 1092616192
LEF4 $81
ADDRLP4 12
CNSTF4 1092616192
ASGNF4
LABELV $81
ADDRLP4 60
CNSTI4 255
ASGNI4
ADDRFP4 0
INDIRI4
ADDRLP4 60
INDIRI4
NEI4 $83
ADDRFP4 4
INDIRI4
ADDRLP4 60
INDIRI4
NEI4 $83
ADDRGP4 cg+1892784
CNSTF4 0
ASGNF4
ADDRGP4 cg+1892788
CNSTF4 0
ASGNF4
ADDRGP4 $84
JUMPV
LABELV $83
ADDRLP4 64
CNSTF4 1135869952
ASGNF4
ADDRLP4 68
CNSTF4 1132396544
ASGNF4
ADDRLP4 56
ADDRLP4 64
INDIRF4
ADDRFP4 4
INDIRI4
CVIF4 4
ADDRLP4 68
INDIRF4
DIVF4
MULF4
ASGNF4
ADDRLP4 52
ADDRLP4 64
INDIRF4
ADDRFP4 0
INDIRI4
CVIF4 4
ADDRLP4 68
INDIRF4
DIVF4
MULF4
ASGNF4
ADDRLP4 28
ADDRLP4 56
INDIRF4
ASGNF4
ADDRLP4 28+4
ADDRLP4 52
INDIRF4
ASGNF4
ADDRLP4 28+8
CNSTF4 0
ASGNF4
ADDRLP4 28
ARGP4
ADDRLP4 0
ARGP4
ADDRLP4 72
CNSTP4 0
ASGNP4
ADDRLP4 72
INDIRP4
ARGP4
ADDRLP4 72
INDIRP4
ARGP4
ADDRGP4 AngleVectors
CALLV
pop
ADDRLP4 0
ADDRGP4 vec3_origin
INDIRF4
ADDRLP4 0
INDIRF4
SUBF4
ASGNF4
ADDRLP4 0+4
ADDRGP4 vec3_origin+4
INDIRF4
ADDRLP4 0+4
INDIRF4
SUBF4
ASGNF4
ADDRLP4 0+8
ADDRGP4 vec3_origin+8
INDIRF4
ADDRLP4 0+8
INDIRF4
SUBF4
ASGNF4
ADDRLP4 76
ADDRLP4 0
INDIRF4
ASGNF4
ADDRLP4 20
ADDRLP4 76
INDIRF4
ADDRGP4 cg+1837612+36
INDIRF4
MULF4
ADDRLP4 0+4
INDIRF4
ADDRGP4 cg+1837612+36+4
INDIRF4
MULF4
ADDF4
ADDRLP4 0+8
INDIRF4
ADDRGP4 cg+1837612+36+8
INDIRF4
MULF4
ADDF4
ASGNF4
ADDRLP4 44
ADDRLP4 76
INDIRF4
ADDRGP4 cg+1837612+36+12
INDIRF4
MULF4
ADDRLP4 0+4
INDIRF4
ADDRGP4 cg+1837612+36+12+4
INDIRF4
MULF4
ADDF4
ADDRLP4 0+8
INDIRF4
ADDRGP4 cg+1837612+36+12+8
INDIRF4
MULF4
ADDF4
ASGNF4
ADDRLP4 48
ADDRLP4 76
INDIRF4
ADDRGP4 cg+1837612+36+24
INDIRF4
MULF4
ADDRLP4 0+4
INDIRF4
ADDRGP4 cg+1837612+36+24+4
INDIRF4
MULF4
ADDF4
ADDRLP4 0+8
INDIRF4
ADDRGP4 cg+1837612+36+24+8
INDIRF4
MULF4
ADDF4
ASGNF4
ADDRLP4 0
ADDRLP4 20
INDIRF4
ASGNF4
ADDRLP4 0+4
ADDRLP4 44
INDIRF4
ASGNF4
ADDRLP4 0+8
CNSTF4 0
ASGNF4
ADDRLP4 0
ARGP4
ADDRLP4 80
ADDRGP4 VectorLength
CALLF4
ASGNF4
ADDRLP4 40
ADDRLP4 80
INDIRF4
ASGNF4
ADDRLP4 40
INDIRF4
CNSTF4 1036831949
GEF4 $133
ADDRLP4 40
CNSTF4 1036831949
ASGNF4
LABELV $133
ADDRLP4 20
INDIRF4
CNSTF4 1036831949
GTF4 $135
ADDRLP4 20
CNSTF4 1036831949
ASGNF4
LABELV $135
ADDRGP4 cg+1892784
ADDRLP4 44
INDIRF4
NEGF4
ADDRLP4 20
INDIRF4
DIVF4
ASGNF4
ADDRGP4 cg+1892788
ADDRLP4 48
INDIRF4
ADDRLP4 40
INDIRF4
DIVF4
ASGNF4
LABELV $84
ADDRGP4 cg+1892784
INDIRF4
CNSTF4 1065353216
LEF4 $139
ADDRGP4 cg+1892784
CNSTF4 1065353216
ASGNF4
LABELV $139
ADDRGP4 cg+1892784
INDIRF4
CNSTF4 3212836864
GEF4 $143
ADDRGP4 cg+1892784
CNSTF4 3212836864
ASGNF4
LABELV $143
ADDRGP4 cg+1892788
INDIRF4
CNSTF4 1065353216
LEF4 $147
ADDRGP4 cg+1892788
CNSTF4 1065353216
ASGNF4
LABELV $147
ADDRGP4 cg+1892788
INDIRF4
CNSTF4 3212836864
GEF4 $151
ADDRGP4 cg+1892788
CNSTF4 3212836864
ASGNF4
LABELV $151
ADDRLP4 12
INDIRF4
CNSTF4 1092616192
LEF4 $155
ADDRLP4 12
CNSTF4 1092616192
ASGNF4
LABELV $155
ADDRGP4 cg+1892792
ADDRLP4 12
INDIRF4
ASGNF4
ADDRGP4 cg+1892780
ADDRGP4 cg+28
INDIRP4
CNSTI4 8
ADDP4
INDIRI4
CVIF4 4
ASGNF4
LABELV $75
endproc CG_DamageFeedback 84 16
export CG_Respawn
proc CG_Respawn 8 0
ADDRGP4 cg+1835968
CNSTI4 1
ASGNI4
ADDRLP4 0
CNSTI4 1
ASGNI4
LABELV $162
ADDRLP4 4
ADDRLP4 0
INDIRI4
CNSTI4 2
LSHI4
ASGNI4
ADDRLP4 4
INDIRI4
ADDRGP4 cg+1837296
ADDP4
ADDRLP4 4
INDIRI4
ADDRGP4 cg+1837404
ADDP4
INDIRI4
ASGNI4
LABELV $163
ADDRLP4 0
ADDRLP4 0
INDIRI4
CNSTI4 1
ADDI4
ASGNI4
ADDRLP4 0
INDIRI4
CNSTI4 27
LTI4 $162
ADDRGP4 cg+1837292
CNSTI4 0
ASGNI4
ADDRGP4 cg+1837992
CNSTI4 0
ASGNI4
LABELV $160
endproc CG_Respawn 8 0
proc CG_CheckPlayerstateEvents 52 8
ADDRFP4 0
ADDRFP4 0
INDIRP4
ASGNP4
ADDRFP4 4
ADDRFP4 4
INDIRP4
ASGNP4
ADDRLP4 12
CNSTI4 128
ASGNI4
ADDRLP4 16
ADDRFP4 0
INDIRP4
ADDRLP4 12
INDIRI4
ADDP4
INDIRI4
ASGNI4
ADDRLP4 16
INDIRI4
CNSTI4 0
EQI4 $171
ADDRLP4 16
INDIRI4
ADDRFP4 4
INDIRP4
ADDRLP4 12
INDIRI4
ADDP4
INDIRI4
EQI4 $171
ADDRLP4 4
CNSTI4 772
ADDRFP4 0
INDIRP4
CNSTI4 140
ADDP4
INDIRI4
MULI4
ADDRGP4 cg_entities
ADDP4
ASGNP4
ADDRLP4 4
INDIRP4
CNSTI4 192
ADDP4
ADDRFP4 0
INDIRP4
CNSTI4 128
ADDP4
INDIRI4
ASGNI4
ADDRLP4 4
INDIRP4
CNSTI4 196
ADDP4
ADDRFP4 0
INDIRP4
CNSTI4 132
ADDP4
INDIRI4
ASGNI4
ADDRLP4 4
INDIRP4
ARGP4
ADDRLP4 4
INDIRP4
CNSTI4 724
ADDP4
ARGP4
ADDRGP4 CG_EntityEvent
CALLV
pop
LABELV $171
ADDRLP4 4
ADDRGP4 cg+1836408
ASGNP4
ADDRLP4 0
ADDRFP4 0
INDIRP4
CNSTI4 108
ADDP4
INDIRI4
CNSTI4 2
SUBI4
ASGNI4
ADDRGP4 $177
JUMPV
LABELV $174
ADDRLP4 28
ADDRFP4 4
INDIRP4
CNSTI4 108
ADDP4
INDIRI4
ASGNI4
ADDRLP4 0
INDIRI4
ADDRLP4 28
INDIRI4
GEI4 $180
ADDRLP4 32
CNSTI4 2
ASGNI4
ADDRLP4 0
INDIRI4
ADDRLP4 28
INDIRI4
ADDRLP4 32
INDIRI4
SUBI4
LEI4 $178
ADDRLP4 36
ADDRLP4 0
INDIRI4
CNSTI4 1
BANDI4
ADDRLP4 32
INDIRI4
LSHI4
ASGNI4
ADDRLP4 40
CNSTI4 112
ASGNI4
ADDRLP4 36
INDIRI4
ADDRFP4 0
INDIRP4
ADDRLP4 40
INDIRI4
ADDP4
ADDP4
INDIRI4
ADDRLP4 36
INDIRI4
ADDRFP4 4
INDIRP4
ADDRLP4 40
INDIRI4
ADDP4
ADDP4
INDIRI4
EQI4 $178
LABELV $180
ADDRLP4 8
ADDRLP4 0
INDIRI4
CNSTI4 1
BANDI4
CNSTI4 2
LSHI4
ADDRFP4 0
INDIRP4
CNSTI4 112
ADDP4
ADDP4
INDIRI4
ASGNI4
ADDRLP4 4
INDIRP4
CNSTI4 192
ADDP4
ADDRLP4 8
INDIRI4
ASGNI4
ADDRLP4 4
INDIRP4
CNSTI4 196
ADDP4
ADDRLP4 0
INDIRI4
CNSTI4 1
BANDI4
CNSTI4 2
LSHI4
ADDRFP4 0
INDIRP4
CNSTI4 120
ADDP4
ADDP4
INDIRI4
ASGNI4
ADDRLP4 4
INDIRP4
ARGP4
ADDRLP4 4
INDIRP4
CNSTI4 724
ADDP4
ARGP4
ADDRGP4 CG_EntityEvent
CALLV
pop
ADDRLP4 0
INDIRI4
CNSTI4 15
BANDI4
CNSTI4 2
LSHI4
ADDRGP4 cg+1837204
ADDP4
ADDRLP4 8
INDIRI4
ASGNI4
ADDRLP4 48
ADDRGP4 cg+1837200
ASGNP4
ADDRLP4 48
INDIRP4
ADDRLP4 48
INDIRP4
INDIRI4
CNSTI4 1
ADDI4
ASGNI4
LABELV $178
LABELV $175
ADDRLP4 0
ADDRLP4 0
INDIRI4
CNSTI4 1
ADDI4
ASGNI4
LABELV $177
ADDRLP4 0
INDIRI4
ADDRFP4 0
INDIRP4
CNSTI4 108
ADDP4
INDIRI4
LTI4 $174
LABELV $170
endproc CG_CheckPlayerstateEvents 52 8
proc CG_CheckLocalSounds 32 16
ADDRFP4 0
ADDRFP4 0
INDIRP4
ASGNP4
ADDRFP4 4
ADDRFP4 4
INDIRP4
ASGNP4
ADDRLP4 8
CNSTI4 260
ASGNI4
ADDRFP4 0
INDIRP4
ADDRLP4 8
INDIRI4
ADDP4
INDIRI4
ADDRFP4 4
INDIRP4
ADDRLP4 8
INDIRI4
ADDP4
INDIRI4
EQI4 $184
ADDRGP4 $183
JUMPV
LABELV $184
ADDRLP4 12
CNSTI4 252
ASGNI4
ADDRFP4 0
INDIRP4
ADDRLP4 12
INDIRI4
ADDP4
INDIRI4
ADDRFP4 4
INDIRP4
ADDRLP4 12
INDIRI4
ADDP4
INDIRI4
LEI4 $186
ADDRLP4 16
ADDRFP4 0
INDIRP4
CNSTI4 268
ADDP4
ASGNP4
ADDRLP4 4
ADDRLP4 16
INDIRP4
INDIRI4
CNSTI4 255
BANDI4
ASGNI4
ADDRLP4 0
ADDRLP4 16
INDIRP4
INDIRI4
CNSTI4 8
RSHI4
ASGNI4
ADDRLP4 20
CNSTI4 0
ASGNI4
ADDRFP4 8
INDIRI4
ADDRLP4 20
INDIRI4
NEI4 $187
ADDRGP4 cgs+71964
INDIRI4
ADDRLP4 20
INDIRI4
EQI4 $187
ADDRLP4 0
INDIRI4
CNSTI4 100
LEI4 $191
ADDRGP4 cgs+474200+1164
INDIRI4
ARGI4
CNSTI4 6
ARGI4
ADDRGP4 trap_S_StartLocalSound
CALLV
pop
ADDRGP4 $187
JUMPV
LABELV $191
ADDRLP4 0
INDIRI4
CNSTI4 50
LEI4 $195
ADDRGP4 cgs+474200+1160
INDIRI4
ARGI4
CNSTI4 6
ARGI4
ADDRGP4 trap_S_StartLocalSound
CALLV
pop
ADDRGP4 $187
JUMPV
LABELV $195
ADDRGP4 cgs+474200+1168
INDIRI4
ARGI4
CNSTI4 6
ARGI4
ADDRGP4 trap_S_StartLocalSound
CALLV
pop
ADDRGP4 $187
JUMPV
LABELV $186
ADDRLP4 16
CNSTI4 252
ASGNI4
ADDRFP4 0
INDIRP4
ADDRLP4 16
INDIRI4
ADDP4
INDIRI4
ADDRFP4 4
INDIRP4
ADDRLP4 16
INDIRI4
ADDP4
INDIRI4
GEI4 $201
ADDRGP4 cgs+474200+1172
INDIRI4
ARGI4
CNSTI4 6
ARGI4
ADDRGP4 trap_S_StartLocalSound
CALLV
pop
LABELV $201
LABELV $187
ADDRFP4 8
INDIRI4
CNSTI4 0
EQI4 $205
ADDRLP4 20
CNSTI4 212
ASGNI4
ADDRLP4 24
ADDRFP4 0
INDIRP4
ADDRLP4 20
INDIRI4
ADDP4
INDIRI4
ASGNI4
ADDRLP4 24
INDIRI4
ADDRFP4 4
INDIRP4
ADDRLP4 20
INDIRI4
ADDP4
INDIRI4
CNSTI4 1
SUBI4
GEI4 $206
ADDRLP4 24
INDIRI4
CNSTI4 0
LEI4 $206
ADDRGP4 cg+1836408
INDIRI4
ARGI4
ADDRGP4 $211
ARGP4
ADDRLP4 28
ADDRGP4 CG_CustomSound
CALLI4
ASGNI4
CNSTP4 0
ARGP4
ADDRGP4 cg+1836408
INDIRI4
ARGI4
CNSTI4 3
ARGI4
ADDRLP4 28
INDIRI4
ARGI4
ADDRGP4 trap_S_StartSound
CALLV
pop
ADDRGP4 $206
JUMPV
LABELV $205
ADDRLP4 20
CNSTI4 184
ASGNI4
ADDRLP4 24
ADDRFP4 0
INDIRP4
ADDRLP4 20
INDIRI4
ADDP4
INDIRI4
ASGNI4
ADDRLP4 24
INDIRI4
ADDRFP4 4
INDIRP4
ADDRLP4 20
INDIRI4
ADDP4
INDIRI4
CNSTI4 1
SUBI4
GEI4 $212
ADDRLP4 24
INDIRI4
CNSTI4 0
LEI4 $212
ADDRGP4 cg+1836408
ARGP4
ADDRFP4 0
INDIRP4
CNSTI4 184
ADDP4
INDIRI4
ARGI4
ADDRGP4 CG_PainEvent
CALLV
pop
LABELV $212
LABELV $206
ADDRGP4 cg+16
INDIRI4
CNSTI4 0
EQI4 $215
LABELV $215
LABELV $183
endproc CG_CheckLocalSounds 32 16
export CG_TransitionPlayerState
proc CG_TransitionPlayerState 28 12
ADDRFP4 0
ADDRFP4 0
INDIRP4
ASGNP4
ADDRFP4 4
ADDRFP4 4
INDIRP4
ASGNP4
ADDRLP4 0
CNSTI4 140
ASGNI4
ADDRFP4 0
INDIRP4
ADDRLP4 0
INDIRI4
ADDP4
INDIRI4
ADDRFP4 4
INDIRP4
ADDRLP4 0
INDIRI4
ADDP4
INDIRI4
EQI4 $219
ADDRGP4 cg+1835968
CNSTI4 1
ASGNI4
ADDRFP4 4
INDIRP4
ADDRFP4 0
INDIRP4
INDIRB
ASGNB 404
LABELV $219
ADDRLP4 8
CNSTI4 168
ASGNI4
ADDRFP4 0
INDIRP4
ADDRLP4 8
INDIRI4
ADDP4
INDIRI4
ADDRFP4 4
INDIRP4
ADDRLP4 8
INDIRI4
ADDP4
INDIRI4
EQI4 $222
ADDRFP4 0
INDIRP4
CNSTI4 180
ADDP4
INDIRI4
CNSTI4 0
EQI4 $222
ADDRFP4 0
INDIRP4
CNSTI4 172
ADDP4
INDIRI4
ARGI4
ADDRFP4 0
INDIRP4
CNSTI4 176
ADDP4
INDIRI4
ARGI4
ADDRFP4 0
INDIRP4
CNSTI4 180
ADDP4
INDIRI4
ARGI4
ADDRGP4 CG_DamageFeedback
CALLV
pop
LABELV $222
ADDRLP4 16
CNSTI4 264
ASGNI4
ADDRFP4 0
INDIRP4
ADDRLP4 16
INDIRI4
ADDP4
INDIRI4
ADDRFP4 4
INDIRP4
ADDRLP4 16
INDIRI4
ADDP4
INDIRI4
EQI4 $224
ADDRGP4 CG_Respawn
CALLV
pop
LABELV $224
ADDRGP4 cg+1835992
INDIRI4
CNSTI4 0
EQI4 $226
ADDRGP4 CG_Respawn
CALLV
pop
ADDRGP4 cg+1835992
CNSTI4 0
ASGNI4
LABELV $226
ADDRGP4 cg+28
INDIRP4
CNSTI4 252
ADDP4
INDIRI4
CNSTI4 0
NEI4 $230
ADDRGP4 cg+28
INDIRP4
CNSTI4 48
ADDP4
INDIRI4
CNSTI4 5
EQI4 $231
ADDRFP4 0
INDIRP4
CNSTI4 260
ADDP4
INDIRI4
CNSTI4 3
EQI4 $231
ADDRFP4 0
INDIRP4
ARGP4
ADDRFP4 4
INDIRP4
ARGP4
CNSTI4 0
ARGI4
ADDRGP4 CG_CheckLocalSounds
CALLV
pop
ADDRGP4 $231
JUMPV
LABELV $230
ADDRGP4 cg+28
INDIRP4
CNSTI4 48
ADDP4
INDIRI4
CNSTI4 5
EQI4 $236
ADDRFP4 0
INDIRP4
CNSTI4 260
ADDP4
INDIRI4
CNSTI4 3
EQI4 $236
ADDRFP4 0
INDIRP4
ARGP4
ADDRFP4 4
INDIRP4
ARGP4
CNSTI4 1
ARGI4
ADDRGP4 CG_CheckLocalSounds
CALLV
pop
LABELV $236
LABELV $231
ADDRFP4 0
INDIRP4
ARGP4
ADDRFP4 4
INDIRP4
ARGP4
ADDRGP4 CG_CheckPlayerstateEvents
CALLV
pop
ADDRLP4 20
CNSTI4 164
ASGNI4
ADDRFP4 0
INDIRP4
ADDRLP4 20
INDIRI4
ADDP4
INDIRI4
ADDRFP4 4
INDIRP4
ADDRLP4 20
INDIRI4
ADDP4
INDIRI4
EQI4 $239
ADDRLP4 24
CNSTI4 164
ASGNI4
ADDRGP4 cg+1837276
ADDRFP4 0
INDIRP4
ADDRLP4 24
INDIRI4
ADDP4
INDIRI4
ADDRFP4 4
INDIRP4
ADDRLP4 24
INDIRI4
ADDP4
INDIRI4
SUBI4
CVIF4 4
ASGNF4
ADDRGP4 cg+1837280
ADDRGP4 cg+1835980
INDIRI4
ASGNI4
LABELV $239
LABELV $218
endproc CG_TransitionPlayerState 28 12
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
import CG_ProcessSnapshots
import CG_TransitionEntity
import CG_ExecuteNewServerCommands
import CG_SetConfigValues
import CG_ParseServerinfo
import CG_DrawScoreboard
import CG_PredictPlayerState
import CG_PointContents
import CG_Trace
import CG_BuildSolidList
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
LABELV $211
byte 1 115
byte 1 111
byte 1 117
byte 1 110
byte 1 100
byte 1 47
byte 1 118
byte 1 101
byte 1 104
byte 1 105
byte 1 99
byte 1 108
byte 1 101
byte 1 47
byte 1 100
byte 1 97
byte 1 109
byte 1 97
byte 1 103
byte 1 101
byte 1 53
byte 1 48
byte 1 46
byte 1 111
byte 1 103
byte 1 103
byte 1 0
