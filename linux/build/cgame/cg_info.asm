export CG_LoadingString
code
proc CG_LoadingString 4 12
ADDRGP4 $77
ARGP4
ADDRFP4 0
INDIRP4
ARGP4
ADDRLP4 0
ADDRGP4 va
CALLP4
ASGNP4
ADDRGP4 cg+1838004
ARGP4
ADDRLP4 0
INDIRP4
ARGP4
CNSTI4 1024
ARGI4
ADDRGP4 Q_strncpyz
CALLV
pop
ADDRFP4 4
INDIRF4
CNSTF4 3212836864
EQF4 $79
ADDRGP4 cg+1839028
ADDRFP4 4
INDIRF4
ASGNF4
LABELV $79
ADDRGP4 trap_UpdateScreen
CALLV
pop
LABELV $75
endproc CG_LoadingString 4 12
export CG_LoadingClient
proc CG_LoadingClient 268 12
ADDRFP4 0
INDIRI4
CNSTI4 1408
ADDI4
ARGI4
ADDRLP4 260
ADDRGP4 CG_ConfigString
CALLP4
ASGNP4
ADDRLP4 256
ADDRLP4 260
INDIRP4
ASGNP4
ADDRLP4 256
INDIRP4
ARGP4
ADDRGP4 $83
ARGP4
ADDRLP4 264
ADDRGP4 Info_ValueForKey
CALLP4
ASGNP4
ADDRLP4 0
ARGP4
ADDRLP4 264
INDIRP4
ARGP4
CNSTI4 256
ARGI4
ADDRGP4 Q_strncpyz
CALLV
pop
ADDRLP4 0
ARGP4
ADDRGP4 Q_CleanStr
CALLP4
pop
ADDRLP4 0
ARGP4
CNSTF4 3212836864
ARGF4
ADDRGP4 CG_LoadingString
CALLV
pop
LABELV $82
endproc CG_LoadingClient 268 12
lit
align 4
LABELV $85
byte 4 1065353216
byte 4 1065353216
byte 4 1065353216
byte 4 1065353216
align 4
LABELV $86
byte 4 1063675494
byte 4 1063675494
byte 4 1063675494
byte 4 1065353216
align 4
LABELV $87
byte 4 1050253722
byte 4 1050253722
byte 4 1050253722
byte 4 1065353216
align 4
LABELV $88
byte 4 1056964608
byte 4 1056964608
byte 4 1056964608
byte 4 1065353216
export CG_DrawInformation
code
proc CG_DrawInformation 236 32
ADDRLP4 68
ADDRGP4 $85
INDIRB
ASGNB 16
ADDRLP4 112
ADDRGP4 $86
INDIRB
ASGNB 16
ADDRLP4 128
ADDRGP4 $87
INDIRB
ASGNB 16
ADDRLP4 144
ADDRGP4 $88
INDIRB
ASGNB 16
CNSTI4 0
ARGI4
ADDRLP4 160
ADDRGP4 CG_ConfigString
CALLP4
ASGNP4
ADDRLP4 88
ADDRLP4 160
INDIRP4
ASGNP4
ADDRLP4 88
INDIRP4
ARGP4
ADDRGP4 $89
ARGP4
ADDRLP4 164
ADDRGP4 Info_ValueForKey
CALLP4
ASGNP4
ADDRLP4 92
ADDRLP4 164
INDIRP4
ASGNP4
ADDRGP4 $90
ARGP4
ADDRLP4 92
INDIRP4
ARGP4
ADDRLP4 168
ADDRGP4 va
CALLP4
ASGNP4
ADDRLP4 168
INDIRP4
ARGP4
ADDRLP4 172
ADDRGP4 trap_R_RegisterShaderNoMip
CALLI4
ASGNI4
ADDRLP4 84
ADDRLP4 172
INDIRI4
ASGNI4
ADDRLP4 84
INDIRI4
CNSTI4 0
NEI4 $91
ADDRGP4 $93
ARGP4
ADDRLP4 176
ADDRGP4 trap_R_RegisterShaderNoMip
CALLI4
ASGNI4
ADDRLP4 84
ADDRLP4 176
INDIRI4
ASGNI4
LABELV $91
ADDRGP4 $94
ARGP4
ADDRLP4 180
ADDRGP4 trap_R_RegisterShaderNoMip
CALLI4
ASGNI4
ADDRLP4 96
ADDRLP4 180
INDIRI4
ASGNI4
ADDRGP4 $95
ARGP4
ADDRLP4 184
ADDRGP4 trap_R_RegisterShaderNoMip
CALLI4
ASGNI4
ADDRLP4 100
ADDRLP4 184
INDIRI4
ASGNI4
ADDRGP4 $96
ARGP4
ADDRLP4 188
ADDRGP4 trap_R_RegisterShaderNoMip
CALLI4
ASGNI4
ADDRLP4 104
ADDRLP4 188
INDIRI4
ASGNI4
ADDRGP4 $97
ARGP4
ADDRLP4 192
ADDRGP4 trap_R_RegisterShaderNoMip
CALLI4
ASGNI4
ADDRLP4 108
ADDRLP4 192
INDIRI4
ASGNI4
CNSTP4 0
ARGP4
ADDRGP4 trap_R_SetColor
CALLV
pop
ADDRLP4 196
CNSTF4 0
ASGNF4
ADDRLP4 196
INDIRF4
ADDRGP4 cgs+71948
INDIRF4
SUBF4
ARGF4
ADDRLP4 196
INDIRF4
ARGF4
CNSTF4 1073741824
ADDRGP4 cgs+71948
INDIRF4
MULF4
CNSTF4 1142947840
ADDF4
ARGF4
CNSTF4 1139802112
ARGF4
ADDRLP4 84
INDIRI4
ARGI4
ADDRGP4 CG_DrawPic
CALLV
pop
ADDRLP4 200
CNSTF4 0
ASGNF4
ADDRLP4 200
INDIRF4
ADDRGP4 cgs+71948
INDIRF4
SUBF4
ARGF4
ADDRLP4 200
INDIRF4
ARGF4
CNSTF4 1073741824
ADDRGP4 cgs+71948
INDIRF4
MULF4
CNSTF4 1142947840
ADDF4
ARGF4
CNSTF4 1139802112
ARGF4
ADDRLP4 96
INDIRI4
ARGI4
ADDRGP4 CG_DrawPic
CALLV
pop
ADDRGP4 cgs+71948
INDIRF4
CNSTF4 1137508352
ADDF4
ARGF4
CNSTF4 1138655232
ARGF4
CNSTF4 1130102784
ARGF4
CNSTF4 1106247680
ARGF4
CNSTF4 1065353216
ARGF4
ADDRLP4 144
ARGP4
ADDRGP4 CG_DrawRoundedRect
CALLV
pop
ADDRGP4 cgs+71948
INDIRF4
CNSTF4 1137672192
ADDF4
ARGF4
CNSTF4 1139113984
ARGF4
CNSTF4 1129447424
ARGF4
CNSTF4 1094713344
ARGF4
ADDRGP4 cg+1839028
INDIRF4
ARGF4
CNSTF4 1090519040
ARGF4
ADDRLP4 68
ARGP4
ADDRLP4 128
ARGP4
ADDRGP4 CG_DrawProgressBar
CALLV
pop
ADDRGP4 cgs+71948
INDIRF4
CNSTF4 1137672192
ADDF4
ARGF4
CNSTF4 1138786304
ARGF4
ADDRGP4 cg+1838004
ARGP4
CNSTI4 0
ARGI4
ADDRLP4 112
ARGP4
CNSTF4 1061997773
ARGF4
ADDRGP4 ST_DrawString
CALLV
pop
CNSTF4 1132920832
ARGF4
CNSTF4 1126498304
ARGF4
ADDRLP4 204
CNSTF4 1120403456
ASGNF4
ADDRLP4 204
INDIRF4
ARGF4
ADDRLP4 204
INDIRF4
ARGF4
ADDRLP4 104
INDIRI4
ARGI4
ADDRGP4 CG_DrawPic
CALLV
pop
CNSTF4 1133772800
ARGF4
CNSTF4 1132986368
ARGF4
ADDRLP4 208
CNSTF4 1111490560
ASGNF4
ADDRLP4 208
INDIRF4
ARGF4
ADDRLP4 208
INDIRF4
ARGF4
ADDRLP4 108
INDIRI4
ARGI4
ADDRGP4 CG_DrawPic
CALLV
pop
ADDRLP4 212
CNSTF4 0
ASGNF4
ADDRLP4 212
INDIRF4
ADDRGP4 cgs+71948
INDIRF4
SUBF4
ARGF4
ADDRLP4 212
INDIRF4
ARGF4
CNSTF4 1133903872
ARGF4
CNSTF4 1118437376
ARGF4
ADDRLP4 100
INDIRI4
ARGI4
ADDRGP4 CG_DrawPic
CALLV
pop
ADDRLP4 216
CNSTF4 1084227584
ASGNF4
ADDRLP4 216
INDIRF4
ADDRGP4 cgs+71948
INDIRF4
SUBF4
ARGF4
ADDRLP4 216
INDIRF4
ARGF4
CNSTF4 1120403456
ARGF4
CNSTF4 1117126656
ARGF4
ADDRLP4 84
INDIRI4
ARGI4
ADDRGP4 CG_DrawPic
CALLV
pop
ADDRLP4 0
CNSTI4 10
ASGNI4
ADDRLP4 88
INDIRP4
ARGP4
ADDRGP4 $109
ARGP4
ADDRLP4 220
ADDRGP4 Info_ValueForKey
CALLP4
ASGNP4
ADDRLP4 4
ARGP4
ADDRLP4 220
INDIRP4
ARGP4
CNSTI4 64
ARGI4
ADDRGP4 Q_strncpyz
CALLV
pop
ADDRLP4 4
ARGP4
ADDRGP4 Q_CleanStr
CALLP4
pop
CNSTF4 1121714176
ADDRGP4 cgs+71948
INDIRF4
SUBF4
ARGF4
ADDRLP4 0
INDIRI4
CVIF4 4
ARGF4
ADDRLP4 4
ARGP4
CNSTI4 0
ARGI4
ADDRLP4 68
ARGP4
CNSTF4 1069547520
ARGF4
ADDRGP4 ST_DrawString
CALLV
pop
ADDRLP4 0
ADDRLP4 0
INDIRI4
CNSTI4 25
ADDI4
ASGNI4
ADDRLP4 88
INDIRP4
ARGP4
ADDRGP4 $89
ARGP4
ADDRLP4 224
ADDRGP4 Info_ValueForKey
CALLP4
ASGNP4
ADDRLP4 4
ARGP4
ADDRLP4 224
INDIRP4
ARGP4
CNSTI4 64
ARGI4
ADDRGP4 Q_strncpyz
CALLV
pop
ADDRLP4 4
ARGP4
ADDRGP4 Q_CleanStr
CALLP4
pop
CNSTF4 1121714176
ADDRGP4 cgs+71948
INDIRF4
SUBF4
ARGF4
ADDRLP4 0
INDIRI4
CVIF4 4
ARGF4
ADDRLP4 4
ARGP4
CNSTI4 0
ARGI4
ADDRLP4 68
ARGP4
CNSTF4 1069547520
ARGF4
ADDRGP4 ST_DrawString
CALLV
pop
ADDRLP4 0
ADDRLP4 0
INDIRI4
CNSTI4 25
ADDI4
ASGNI4
ADDRLP4 228
ADDRGP4 cgs+71964
INDIRI4
ASGNI4
ADDRLP4 228
INDIRI4
CNSTI4 0
LTI4 $112
ADDRLP4 228
INDIRI4
CNSTI4 6
GTI4 $112
ADDRLP4 228
INDIRI4
CNSTI4 2
LSHI4
ADDRGP4 $131
ADDP4
INDIRP4
JUMPV
lit
align 4
LABELV $131
address $116
address $118
address $120
address $122
address $124
address $126
address $128
code
LABELV $116
ADDRLP4 92
ADDRGP4 $117
ASGNP4
ADDRGP4 $113
JUMPV
LABELV $118
ADDRLP4 92
ADDRGP4 $119
ASGNP4
ADDRGP4 $113
JUMPV
LABELV $120
ADDRLP4 92
ADDRGP4 $121
ASGNP4
ADDRGP4 $113
JUMPV
LABELV $122
ADDRLP4 92
ADDRGP4 $123
ASGNP4
ADDRGP4 $113
JUMPV
LABELV $124
ADDRLP4 92
ADDRGP4 $125
ASGNP4
ADDRGP4 $113
JUMPV
LABELV $126
ADDRLP4 92
ADDRGP4 $127
ASGNP4
ADDRGP4 $113
JUMPV
LABELV $128
ADDRLP4 92
ADDRGP4 $129
ASGNP4
ADDRGP4 $113
JUMPV
LABELV $112
ADDRLP4 92
ADDRGP4 $130
ASGNP4
LABELV $113
CNSTF4 1121714176
ADDRGP4 cgs+71948
INDIRF4
SUBF4
ARGF4
ADDRLP4 0
INDIRI4
CVIF4 4
ARGF4
ADDRLP4 92
INDIRP4
ARGP4
CNSTI4 0
ARGI4
ADDRLP4 68
ARGP4
CNSTF4 1069547520
ARGF4
ADDRGP4 ST_DrawString
CALLV
pop
LABELV $84
endproc CG_DrawInformation 236 32
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
LABELV $130
byte 1 85
byte 1 110
byte 1 107
byte 1 110
byte 1 111
byte 1 119
byte 1 110
byte 1 32
byte 1 71
byte 1 97
byte 1 109
byte 1 101
byte 1 116
byte 1 121
byte 1 112
byte 1 101
byte 1 0
align 1
LABELV $129
byte 1 72
byte 1 97
byte 1 114
byte 1 118
byte 1 101
byte 1 115
byte 1 116
byte 1 101
byte 1 114
byte 1 0
align 1
LABELV $127
byte 1 79
byte 1 118
byte 1 101
byte 1 114
byte 1 108
byte 1 111
byte 1 97
byte 1 100
byte 1 0
align 1
LABELV $125
byte 1 79
byte 1 110
byte 1 101
byte 1 32
byte 1 70
byte 1 108
byte 1 97
byte 1 103
byte 1 32
byte 1 67
byte 1 84
byte 1 70
byte 1 0
align 1
LABELV $123
byte 1 67
byte 1 97
byte 1 112
byte 1 116
byte 1 117
byte 1 114
byte 1 101
byte 1 32
byte 1 84
byte 1 104
byte 1 101
byte 1 32
byte 1 70
byte 1 108
byte 1 97
byte 1 103
byte 1 0
align 1
LABELV $121
byte 1 84
byte 1 101
byte 1 97
byte 1 109
byte 1 32
byte 1 68
byte 1 101
byte 1 97
byte 1 116
byte 1 104
byte 1 109
byte 1 97
byte 1 116
byte 1 99
byte 1 104
byte 1 0
align 1
LABELV $119
byte 1 70
byte 1 114
byte 1 101
byte 1 101
byte 1 32
byte 1 70
byte 1 111
byte 1 114
byte 1 32
byte 1 65
byte 1 108
byte 1 108
byte 1 0
align 1
LABELV $117
byte 1 83
byte 1 97
byte 1 110
byte 1 100
byte 1 98
byte 1 111
byte 1 120
byte 1 0
align 1
LABELV $109
byte 1 115
byte 1 118
byte 1 95
byte 1 104
byte 1 111
byte 1 115
byte 1 116
byte 1 110
byte 1 97
byte 1 109
byte 1 101
byte 1 0
align 1
LABELV $97
byte 1 109
byte 1 101
byte 1 110
byte 1 117
byte 1 47
byte 1 97
byte 1 115
byte 1 115
byte 1 101
byte 1 116
byte 1 115
byte 1 47
byte 1 108
byte 1 111
byte 1 97
byte 1 100
byte 1 105
byte 1 110
byte 1 103
byte 1 0
align 1
LABELV $96
byte 1 109
byte 1 101
byte 1 110
byte 1 117
byte 1 47
byte 1 108
byte 1 111
byte 1 103
byte 1 111
byte 1 0
align 1
LABELV $95
byte 1 109
byte 1 101
byte 1 110
byte 1 117
byte 1 47
byte 1 97
byte 1 115
byte 1 115
byte 1 101
byte 1 116
byte 1 115
byte 1 47
byte 1 98
byte 1 108
byte 1 97
byte 1 99
byte 1 107
byte 1 116
byte 1 114
byte 1 97
byte 1 110
byte 1 115
byte 1 0
align 1
LABELV $94
byte 1 109
byte 1 101
byte 1 110
byte 1 117
byte 1 47
byte 1 97
byte 1 115
byte 1 115
byte 1 101
byte 1 116
byte 1 115
byte 1 47
byte 1 108
byte 1 111
byte 1 97
byte 1 100
byte 1 105
byte 1 110
byte 1 103
byte 1 111
byte 1 118
byte 1 101
byte 1 114
byte 1 108
byte 1 97
byte 1 121
byte 1 0
align 1
LABELV $93
byte 1 109
byte 1 101
byte 1 110
byte 1 117
byte 1 47
byte 1 97
byte 1 115
byte 1 115
byte 1 101
byte 1 116
byte 1 115
byte 1 47
byte 1 117
byte 1 110
byte 1 107
byte 1 110
byte 1 111
byte 1 119
byte 1 110
byte 1 109
byte 1 97
byte 1 112
byte 1 0
align 1
LABELV $90
byte 1 108
byte 1 101
byte 1 118
byte 1 101
byte 1 108
byte 1 115
byte 1 104
byte 1 111
byte 1 116
byte 1 115
byte 1 47
byte 1 37
byte 1 115
byte 1 0
align 1
LABELV $89
byte 1 115
byte 1 118
byte 1 95
byte 1 109
byte 1 97
byte 1 112
byte 1 110
byte 1 97
byte 1 109
byte 1 101
byte 1 0
align 1
LABELV $83
byte 1 110
byte 1 0
align 1
LABELV $77
byte 1 76
byte 1 111
byte 1 97
byte 1 100
byte 1 105
byte 1 110
byte 1 103
byte 1 46
byte 1 46
byte 1 46
byte 1 32
byte 1 37
byte 1 115
byte 1 0
