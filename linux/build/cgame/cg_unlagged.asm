export CG_PredictWeaponEffects
code
proc CG_PredictWeaponEffects 184 48
ADDRLP4 24
ADDRFP4 0
INDIRP4
ASGNP4
ADDRFP4 0
INDIRP4
INDIRI4
ADDRGP4 cg+1836004+140
INDIRI4
EQI4 $76
ADDRGP4 $75
JUMPV
LABELV $76
ADDRLP4 0
ADDRGP4 cg+1836004+20
INDIRF4
ASGNF4
ADDRLP4 0+4
ADDRGP4 cg+1836004+20+4
INDIRF4
ASGNF4
ADDRLP4 0+8
ADDRGP4 cg+1836004+20+8
INDIRF4
ASGNF4
ADDRLP4 0+8
ADDRLP4 0+8
INDIRF4
ADDRGP4 cg+1836004+164
INDIRI4
CVIF4 4
ADDF4
ASGNF4
ADDRGP4 cg+1836004+152
ARGP4
ADDRLP4 12
ARGP4
ADDRLP4 28
ARGP4
ADDRLP4 40
ARGP4
ADDRGP4 AngleVectors
CALLV
pop
ADDRLP4 52
CNSTF4 1096810496
ASGNF4
ADDRLP4 0
ADDRLP4 0
INDIRF4
ADDRLP4 52
INDIRF4
ADDRLP4 12
INDIRF4
MULF4
ADDF4
ASGNF4
ADDRLP4 0+4
ADDRLP4 0+4
INDIRF4
ADDRLP4 52
INDIRF4
ADDRLP4 12+4
INDIRF4
MULF4
ADDF4
ASGNF4
ADDRLP4 0+8
ADDRLP4 0+8
INDIRF4
CNSTF4 1096810496
ADDRLP4 12+8
INDIRF4
MULF4
ADDF4
ASGNF4
CNSTI4 76
ADDRLP4 24
INDIRP4
CNSTI4 204
ADDP4
INDIRI4
MULI4
ADDRGP4 gameInfoWeapons
ADDP4
INDIRI4
CNSTI4 5
NEI4 $101
ADDRLP4 124
CNSTI4 76
ASGNI4
ADDRLP4 128
ADDRLP4 24
INDIRP4
CNSTI4 204
ADDP4
ASGNP4
ADDRLP4 112
ADDRLP4 0
INDIRF4
ADDRLP4 12
INDIRF4
ADDRLP4 124
INDIRI4
ADDRLP4 128
INDIRP4
INDIRI4
MULI4
ADDRGP4 gameInfoWeapons+36
ADDP4
INDIRI4
CVIF4 4
MULF4
ADDF4
ASGNF4
ADDRLP4 112+4
ADDRLP4 0+4
INDIRF4
ADDRLP4 12+4
INDIRF4
ADDRLP4 124
INDIRI4
ADDRLP4 128
INDIRP4
INDIRI4
MULI4
ADDRGP4 gameInfoWeapons+36
ADDP4
INDIRI4
CVIF4 4
MULF4
ADDF4
ASGNF4
ADDRLP4 112+8
ADDRLP4 0+8
INDIRF4
ADDRLP4 12+8
INDIRF4
CNSTI4 76
ADDRLP4 24
INDIRP4
CNSTI4 204
ADDP4
INDIRI4
MULI4
ADDRGP4 gameInfoWeapons+36
ADDP4
INDIRI4
CVIF4 4
MULF4
ADDF4
ASGNF4
ADDRLP4 56
ARGP4
ADDRLP4 0
ARGP4
ADDRLP4 132
ADDRGP4 vec3_origin
ASGNP4
ADDRLP4 132
INDIRP4
ARGP4
ADDRLP4 132
INDIRP4
ARGP4
ADDRLP4 112
ARGP4
ADDRGP4 cg+1836004+140
INDIRI4
ARGI4
CNSTI4 1
ARGI4
ADDRGP4 CG_Trace
CALLV
pop
ADDRLP4 136
CNSTF4 1082130432
ASGNF4
ADDRLP4 0
ADDRLP4 0
INDIRF4
ADDRLP4 136
INDIRF4
ADDRLP4 28
INDIRF4
MULF4
ADDF4
ASGNF4
ADDRLP4 0+4
ADDRLP4 0+4
INDIRF4
ADDRLP4 136
INDIRF4
ADDRLP4 28+4
INDIRF4
MULF4
ADDF4
ASGNF4
ADDRLP4 0+8
ADDRLP4 0+8
INDIRF4
CNSTF4 1082130432
ADDRLP4 28+8
INDIRF4
MULF4
ADDF4
ASGNF4
ADDRLP4 140
CNSTF4 3212836864
ASGNF4
ADDRLP4 0
ADDRLP4 0
INDIRF4
ADDRLP4 140
INDIRF4
ADDRLP4 40
INDIRF4
MULF4
ADDF4
ASGNF4
ADDRLP4 0+4
ADDRLP4 0+4
INDIRF4
ADDRLP4 140
INDIRF4
ADDRLP4 40+4
INDIRF4
MULF4
ADDF4
ASGNF4
ADDRLP4 0+8
ADDRLP4 0+8
INDIRF4
CNSTF4 3212836864
ADDRLP4 40+8
INDIRF4
MULF4
ADDF4
ASGNF4
ADDRGP4 cg+1835996
INDIRI4
CNSTI4 0
NEI4 $126
ADDRGP4 $131
ARGP4
ADDRLP4 144
ADDRGP4 cvarInt
CALLI4
ASGNI4
ADDRLP4 144
INDIRI4
CNSTI4 2
NEI4 $129
ADDRLP4 148
CNSTF4 1090519040
ASGNF4
ADDRLP4 0
ADDRLP4 0
INDIRF4
ADDRLP4 148
INDIRF4
ADDRGP4 cg+1837612+36+12
INDIRF4
MULF4
ADDF4
ASGNF4
ADDRLP4 0+4
ADDRLP4 0+4
INDIRF4
ADDRLP4 148
INDIRF4
ADDRGP4 cg+1837612+36+12+4
INDIRF4
MULF4
ADDF4
ASGNF4
ADDRLP4 0+8
ADDRLP4 0+8
INDIRF4
CNSTF4 1090519040
ADDRGP4 cg+1837612+36+12+8
INDIRF4
MULF4
ADDF4
ASGNF4
ADDRGP4 $130
JUMPV
LABELV $129
ADDRGP4 $131
ARGP4
ADDRLP4 152
ADDRGP4 cvarInt
CALLI4
ASGNI4
ADDRLP4 152
INDIRI4
CNSTI4 3
NEI4 $147
ADDRLP4 156
CNSTF4 1082130432
ASGNF4
ADDRLP4 0
ADDRLP4 0
INDIRF4
ADDRLP4 156
INDIRF4
ADDRGP4 cg+1837612+36+12
INDIRF4
MULF4
ADDF4
ASGNF4
ADDRLP4 0+4
ADDRLP4 0+4
INDIRF4
ADDRLP4 156
INDIRF4
ADDRGP4 cg+1837612+36+12+4
INDIRF4
MULF4
ADDF4
ASGNF4
ADDRLP4 0+8
ADDRLP4 0+8
INDIRF4
CNSTF4 1082130432
ADDRGP4 cg+1837612+36+12+8
INDIRF4
MULF4
ADDF4
ASGNF4
LABELV $147
LABELV $130
LABELV $126
CNSTI4 2880
ADDRFP4 0
INDIRP4
INDIRI4
MULI4
ADDRGP4 cgs+93776
ADDP4
ARGP4
ADDRLP4 0
ARGP4
ADDRLP4 56+12
ARGP4
ADDRLP4 24
INDIRP4
CNSTI4 204
ADDP4
INDIRI4
ARGI4
ADDRGP4 CG_RailTrail
CALLV
pop
ADDRLP4 56+44
INDIRI4
CNSTI4 16
BANDI4
CNSTI4 0
NEI4 $102
ADDRLP4 24
INDIRP4
CNSTI4 204
ADDP4
INDIRI4
ARGI4
ADDRGP4 cg+1836004+140
INDIRI4
ARGI4
ADDRLP4 56+12
ARGP4
ADDRLP4 56+24
ARGP4
CNSTI4 0
ARGI4
ADDRGP4 CG_MissileHitWall
CALLV
pop
ADDRGP4 $102
JUMPV
LABELV $101
CNSTI4 76
ADDRLP4 24
INDIRP4
CNSTI4 204
ADDP4
INDIRI4
MULI4
ADDRGP4 gameInfoWeapons
ADDP4
INDIRI4
CNSTI4 3
NEI4 $173
ADDRLP4 0
ADDRLP4 0
INDIRF4
CVFI4 4
CVIF4 4
ASGNF4
ADDRLP4 0+4
ADDRLP4 0+4
INDIRF4
CVFI4 4
CVIF4 4
ASGNF4
ADDRLP4 0+8
ADDRLP4 0+8
INDIRF4
CVFI4 4
CVIF4 4
ASGNF4
ADDRLP4 96
CNSTI4 76
ASGNI4
ADDRLP4 100
ADDRLP4 24
INDIRP4
CNSTI4 204
ADDP4
ASGNP4
ADDRLP4 68
ADDRLP4 12
INDIRF4
ADDRLP4 96
INDIRI4
ADDRLP4 100
INDIRP4
INDIRI4
MULI4
ADDRGP4 gameInfoWeapons+36
ADDP4
INDIRI4
CVIF4 4
MULF4
ASGNF4
ADDRLP4 68+4
ADDRLP4 12+4
INDIRF4
ADDRLP4 96
INDIRI4
ADDRLP4 100
INDIRP4
INDIRI4
MULI4
ADDRGP4 gameInfoWeapons+36
ADDP4
INDIRI4
CVIF4 4
MULF4
ASGNF4
ADDRLP4 68+8
ADDRLP4 12+8
INDIRF4
CNSTI4 76
ADDRLP4 24
INDIRP4
CNSTI4 204
ADDP4
INDIRI4
MULI4
ADDRGP4 gameInfoWeapons+36
ADDP4
INDIRI4
CVIF4 4
MULF4
ASGNF4
ADDRLP4 68
ADDRLP4 68
INDIRF4
CVFI4 4
CVIF4 4
ASGNF4
ADDRLP4 68+4
ADDRLP4 68+4
INDIRF4
CVFI4 4
CVIF4 4
ASGNF4
ADDRLP4 68+8
ADDRLP4 68+8
INDIRF4
CVFI4 4
CVIF4 4
ASGNF4
ADDRLP4 56
ADDRLP4 68
INDIRF4
ADDRLP4 0
INDIRF4
SUBF4
ASGNF4
ADDRLP4 56+4
ADDRLP4 68+4
INDIRF4
ADDRLP4 0+4
INDIRF4
SUBF4
ASGNF4
ADDRLP4 56+8
ADDRLP4 68+8
INDIRF4
ADDRLP4 0+8
INDIRF4
SUBF4
ASGNF4
ADDRLP4 56
ARGP4
ADDRGP4 VectorNormalize
CALLF4
pop
ADDRLP4 104
CNSTF4 1107296256
ASGNF4
ADDRLP4 56
ADDRLP4 104
INDIRF4
ADDRLP4 56
INDIRF4
MULF4
ASGNF4
ADDRLP4 56+4
ADDRLP4 104
INDIRF4
ADDRLP4 56+4
INDIRF4
MULF4
ASGNF4
ADDRLP4 56+8
CNSTF4 1107296256
ADDRLP4 56+8
INDIRF4
MULF4
ASGNF4
ADDRLP4 56
ADDRLP4 0
INDIRF4
ADDRLP4 56
INDIRF4
ADDF4
ASGNF4
ADDRLP4 56+4
ADDRLP4 0+4
INDIRF4
ADDRLP4 56+4
INDIRF4
ADDF4
ASGNF4
ADDRLP4 56+8
ADDRLP4 0+8
INDIRF4
ADDRLP4 56+8
INDIRF4
ADDF4
ASGNF4
ADDRLP4 0
ARGP4
CNSTI4 0
ARGI4
ADDRLP4 108
ADDRGP4 trap_CM_PointContents
CALLI4
ASGNI4
ADDRLP4 80
ADDRLP4 108
INDIRI4
ASGNI4
ADDRLP4 80
INDIRI4
CNSTI4 32
BANDI4
CNSTI4 0
NEI4 $206
ADDRLP4 112
CNSTF4 0
ASGNF4
ADDRLP4 84
ADDRLP4 112
INDIRF4
ASGNF4
ADDRLP4 84+4
ADDRLP4 112
INDIRF4
ASGNF4
ADDRLP4 84+8
CNSTF4 1090519040
ASGNF4
ADDRLP4 56
ARGP4
ADDRLP4 84
ARGP4
CNSTF4 1107296256
ARGF4
ADDRLP4 116
CNSTF4 1065353216
ASGNF4
ADDRLP4 116
INDIRF4
ARGF4
ADDRLP4 116
INDIRF4
ARGF4
ADDRLP4 116
INDIRF4
ARGF4
CNSTF4 1051260355
ARGF4
CNSTF4 1147207680
ARGF4
ADDRGP4 cg+1835980
INDIRI4
ARGI4
CNSTI4 0
ARGI4
CNSTI4 1
ARGI4
ADDRGP4 cgs+474200+592
INDIRI4
ARGI4
ADDRGP4 CG_SmokePuff
CALLP4
pop
LABELV $206
ADDRLP4 0
ARGP4
ADDRLP4 68
ARGP4
ADDRGP4 cg+1835984
INDIRI4
CNSTI4 256
MODI4
ARGI4
ADDRGP4 cg+1836004+140
INDIRI4
ARGI4
ADDRLP4 24
INDIRP4
CNSTI4 204
ADDP4
INDIRI4
ARGI4
ADDRGP4 CG_ShotgunPattern
CALLV
pop
ADDRGP4 $174
JUMPV
LABELV $173
CNSTI4 76
ADDRLP4 24
INDIRP4
CNSTI4 204
ADDP4
INDIRI4
MULI4
ADDRGP4 gameInfoWeapons
ADDP4
INDIRI4
CNSTI4 2
NEI4 $216
ADDRLP4 68
ADDRGP4 cg+1835984
INDIRI4
CNSTI4 256
MODI4
ASGNI4
ADDRLP4 72
CNSTI4 0
ASGNI4
ADDRLP4 68
ARGP4
ADDRLP4 144
ADDRGP4 Q_random
CALLF4
ASGNF4
ADDRLP4 76
CNSTF4 1073741824
CNSTF4 1078530011
ADDRLP4 144
INDIRF4
MULF4
MULF4
ASGNF4
ADDRLP4 76
INDIRF4
ARGF4
ADDRLP4 148
ADDRGP4 sin
CALLF4
ASGNF4
ADDRLP4 68
ARGP4
ADDRLP4 152
ADDRGP4 Q_crandom
CALLF4
ASGNF4
ADDRLP4 136
CNSTF4 1098907648
ADDRLP4 148
INDIRF4
ADDRLP4 152
INDIRF4
MULF4
CNSTI4 76
ADDRLP4 24
INDIRP4
CNSTI4 204
ADDP4
INDIRI4
MULI4
ADDRGP4 gameInfoWeapons+52
ADDP4
INDIRI4
CVIF4 4
MULF4
MULF4
ASGNF4
ADDRLP4 76
INDIRF4
ARGF4
ADDRLP4 156
ADDRGP4 cos
CALLF4
ASGNF4
ADDRLP4 68
ARGP4
ADDRLP4 160
ADDRGP4 Q_crandom
CALLF4
ASGNF4
ADDRLP4 164
CNSTI4 76
ASGNI4
ADDRLP4 168
ADDRLP4 24
INDIRP4
CNSTI4 204
ADDP4
ASGNP4
ADDRLP4 76
CNSTF4 1098907648
ADDRLP4 156
INDIRF4
ADDRLP4 160
INDIRF4
MULF4
ADDRLP4 164
INDIRI4
ADDRLP4 168
INDIRP4
INDIRI4
MULI4
ADDRGP4 gameInfoWeapons+52
ADDP4
INDIRI4
CVIF4 4
MULF4
MULF4
ASGNF4
ADDRLP4 56
ADDRLP4 0
INDIRF4
ADDRLP4 12
INDIRF4
ADDRLP4 164
INDIRI4
ADDRLP4 168
INDIRP4
INDIRI4
MULI4
ADDRGP4 gameInfoWeapons+36
ADDP4
INDIRI4
CVIF4 4
MULF4
ADDF4
ASGNF4
ADDRLP4 56+4
ADDRLP4 0+4
INDIRF4
ADDRLP4 12+4
INDIRF4
ADDRLP4 164
INDIRI4
ADDRLP4 168
INDIRP4
INDIRI4
MULI4
ADDRGP4 gameInfoWeapons+36
ADDP4
INDIRI4
CVIF4 4
MULF4
ADDF4
ASGNF4
ADDRLP4 56+8
ADDRLP4 0+8
INDIRF4
ADDRLP4 12+8
INDIRF4
CNSTI4 76
ADDRLP4 24
INDIRP4
CNSTI4 204
ADDP4
INDIRI4
MULI4
ADDRGP4 gameInfoWeapons+36
ADDP4
INDIRI4
CVIF4 4
MULF4
ADDF4
ASGNF4
ADDRLP4 172
ADDRLP4 76
INDIRF4
ASGNF4
ADDRLP4 56
ADDRLP4 56
INDIRF4
ADDRLP4 28
INDIRF4
ADDRLP4 172
INDIRF4
MULF4
ADDF4
ASGNF4
ADDRLP4 56+4
ADDRLP4 56+4
INDIRF4
ADDRLP4 28+4
INDIRF4
ADDRLP4 172
INDIRF4
MULF4
ADDF4
ASGNF4
ADDRLP4 56+8
ADDRLP4 56+8
INDIRF4
ADDRLP4 28+8
INDIRF4
ADDRLP4 76
INDIRF4
MULF4
ADDF4
ASGNF4
ADDRLP4 176
ADDRLP4 136
INDIRF4
ASGNF4
ADDRLP4 56
ADDRLP4 56
INDIRF4
ADDRLP4 40
INDIRF4
ADDRLP4 176
INDIRF4
MULF4
ADDF4
ASGNF4
ADDRLP4 56+4
ADDRLP4 56+4
INDIRF4
ADDRLP4 40+4
INDIRF4
ADDRLP4 176
INDIRF4
MULF4
ADDF4
ASGNF4
ADDRLP4 56+8
ADDRLP4 56+8
INDIRF4
ADDRLP4 40+8
INDIRF4
ADDRLP4 136
INDIRF4
MULF4
ADDF4
ASGNF4
ADDRLP4 80
ARGP4
ADDRLP4 0
ARGP4
ADDRLP4 180
CNSTP4 0
ASGNP4
ADDRLP4 180
INDIRP4
ARGP4
ADDRLP4 180
INDIRP4
ARGP4
ADDRLP4 56
ARGP4
ADDRGP4 cg+1836004+140
INDIRI4
ARGI4
CNSTI4 100663297
ARGI4
ADDRGP4 CG_Trace
CALLV
pop
ADDRLP4 80+44
INDIRI4
CNSTI4 16
BANDI4
CNSTI4 0
EQI4 $244
ADDRGP4 $75
JUMPV
LABELV $244
ADDRLP4 80+12
ARGP4
ADDRLP4 0
ARGP4
ADDRGP4 SnapVectorTowards
CALLV
pop
ADDRLP4 80+52
INDIRI4
CNSTI4 128
GEI4 $248
ADDRLP4 140
CNSTI4 1
ASGNI4
ADDRLP4 72
ADDRLP4 80+52
INDIRI4
ASGNI4
ADDRGP4 $249
JUMPV
LABELV $248
ADDRLP4 140
CNSTI4 0
ASGNI4
LABELV $249
ADDRLP4 80+12
ARGP4
ADDRGP4 cg+1836004+140
INDIRI4
ARGI4
ADDRLP4 80+24
ARGP4
ADDRLP4 140
INDIRI4
ARGI4
ADDRLP4 72
INDIRI4
ARGI4
ADDRGP4 CG_Bullet
CALLV
pop
LABELV $216
LABELV $174
LABELV $102
LABELV $75
endproc CG_PredictWeaponEffects 184 48
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
LABELV $131
byte 1 99
byte 1 103
byte 1 95
byte 1 100
byte 1 114
byte 1 97
byte 1 119
byte 1 71
byte 1 117
byte 1 110
byte 1 0
