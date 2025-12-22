code
proc CG_ScoresDown_f 0 4
ADDRGP4 cg+1839032
INDIRI4
CNSTI4 2000
ADDI4
ADDRGP4 cg+1835980
INDIRI4
GEI4 $76
ADDRGP4 cg+1839032
ADDRGP4 cg+1835980
INDIRI4
ASGNI4
ADDRGP4 $82
ARGP4
ADDRGP4 trap_SendClientCommand
CALLV
pop
ADDRGP4 cg+1840072
INDIRI4
CNSTI4 0
NEI4 $77
ADDRGP4 cg+1840072
CNSTI4 1
ASGNI4
ADDRGP4 cg+1839036
CNSTI4 0
ASGNI4
ADDRGP4 $77
JUMPV
LABELV $76
ADDRGP4 cg+1840072
CNSTI4 1
ASGNI4
LABELV $77
LABELV $75
endproc CG_ScoresDown_f 0 4
proc CG_ScoresUp_f 0 0
ADDRGP4 cg+1840072
INDIRI4
CNSTI4 0
EQI4 $90
ADDRGP4 cg+1840072
CNSTI4 0
ASGNI4
LABELV $90
LABELV $89
endproc CG_ScoresUp_f 0 0
proc CG_ReplaceTexture_f 2048 12
CNSTI4 1
ARGI4
ADDRLP4 0
ARGP4
CNSTI4 1024
ARGI4
ADDRGP4 trap_Argv
CALLV
pop
CNSTI4 2
ARGI4
ADDRLP4 1024
ARGP4
CNSTI4 1024
ARGI4
ADDRGP4 trap_Argv
CALLV
pop
ADDRLP4 0
ARGP4
ADDRLP4 1024
ARGP4
ADDRGP4 $95
ARGP4
ADDRGP4 trap_R_RemapShader
CALLV
pop
LABELV $94
endproc CG_ReplaceTexture_f 2048 12
data
align 4
LABELV commands
address $97
address CG_ScoresDown_f
address $98
address CG_ScoresUp_f
address $99
address CG_ZoomDown_f
address $100
address CG_ZoomUp_f
address $101
address CG_NextWeapon_f
address $102
address CG_PrevWeapon_f
address $103
address CG_Weapon_f
address $104
address CG_ReplaceTexture_f
export CG_ConsoleCommand
code
proc CG_ConsoleCommand 16 8
CNSTI4 0
ARGI4
ADDRLP4 8
ADDRGP4 CG_Argv
CALLP4
ASGNP4
ADDRLP4 4
ADDRLP4 8
INDIRP4
ASGNP4
ADDRLP4 0
CNSTI4 0
ASGNI4
ADDRGP4 $109
JUMPV
LABELV $106
ADDRLP4 4
INDIRP4
ARGP4
ADDRLP4 0
INDIRI4
CNSTI4 3
LSHI4
ADDRGP4 commands
ADDP4
INDIRP4
ARGP4
ADDRLP4 12
ADDRGP4 Q_stricmp
CALLI4
ASGNI4
ADDRLP4 12
INDIRI4
CNSTI4 0
NEI4 $110
ADDRLP4 0
INDIRI4
CNSTI4 3
LSHI4
ADDRGP4 commands+4
ADDP4
INDIRP4
CALLV
pop
CNSTI4 1
RETI4
ADDRGP4 $105
JUMPV
LABELV $110
LABELV $107
ADDRLP4 0
ADDRLP4 0
INDIRI4
CNSTI4 1
ADDI4
ASGNI4
LABELV $109
ADDRLP4 0
INDIRI4
CVIU4 4
CNSTU4 8
LTU4 $106
CNSTI4 0
RETI4
LABELV $105
endproc CG_ConsoleCommand 16 8
export CG_InitConsoleCommands
proc CG_InitConsoleCommands 4 4
ADDRLP4 0
CNSTI4 0
ASGNI4
ADDRGP4 $117
JUMPV
LABELV $114
ADDRLP4 0
INDIRI4
CNSTI4 3
LSHI4
ADDRGP4 commands
ADDP4
INDIRP4
ARGP4
ADDRGP4 trap_AddCommand
CALLV
pop
LABELV $115
ADDRLP4 0
ADDRLP4 0
INDIRI4
CNSTI4 1
ADDI4
ASGNI4
LABELV $117
ADDRLP4 0
INDIRI4
CVIU4 4
CNSTU4 8
LTU4 $114
ADDRGP4 $118
ARGP4
ADDRGP4 trap_AddCommand
CALLV
pop
ADDRGP4 $119
ARGP4
ADDRGP4 trap_AddCommand
CALLV
pop
ADDRGP4 $120
ARGP4
ADDRGP4 trap_AddCommand
CALLV
pop
ADDRGP4 $121
ARGP4
ADDRGP4 trap_AddCommand
CALLV
pop
ADDRGP4 $82
ARGP4
ADDRGP4 trap_AddCommand
CALLV
pop
ADDRGP4 $122
ARGP4
ADDRGP4 trap_AddCommand
CALLV
pop
ADDRGP4 $123
ARGP4
ADDRGP4 trap_AddCommand
CALLV
pop
ADDRGP4 $124
ARGP4
ADDRGP4 trap_AddCommand
CALLV
pop
ADDRGP4 $125
ARGP4
ADDRGP4 trap_AddCommand
CALLV
pop
ADDRGP4 $126
ARGP4
ADDRGP4 trap_AddCommand
CALLV
pop
ADDRGP4 $127
ARGP4
ADDRGP4 trap_AddCommand
CALLV
pop
ADDRGP4 $128
ARGP4
ADDRGP4 trap_AddCommand
CALLV
pop
ADDRGP4 $129
ARGP4
ADDRGP4 trap_AddCommand
CALLV
pop
ADDRGP4 $130
ARGP4
ADDRGP4 trap_AddCommand
CALLV
pop
ADDRGP4 $131
ARGP4
ADDRGP4 trap_AddCommand
CALLV
pop
ADDRGP4 $132
ARGP4
ADDRGP4 trap_AddCommand
CALLV
pop
ADDRGP4 $133
ARGP4
ADDRGP4 trap_AddCommand
CALLV
pop
ADDRGP4 $134
ARGP4
ADDRGP4 trap_AddCommand
CALLV
pop
ADDRGP4 $135
ARGP4
ADDRGP4 trap_AddCommand
CALLV
pop
ADDRGP4 $136
ARGP4
ADDRGP4 trap_AddCommand
CALLV
pop
ADDRGP4 $137
ARGP4
ADDRGP4 trap_AddCommand
CALLV
pop
ADDRGP4 $138
ARGP4
ADDRGP4 trap_AddCommand
CALLV
pop
ADDRGP4 $139
ARGP4
ADDRGP4 trap_AddCommand
CALLV
pop
ADDRGP4 $140
ARGP4
ADDRGP4 trap_AddCommand
CALLV
pop
LABELV $113
endproc CG_InitConsoleCommands 4 4
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
LABELV $140
byte 1 108
byte 1 111
byte 1 97
byte 1 100
byte 1 109
byte 1 97
byte 1 112
byte 1 0
align 1
LABELV $139
byte 1 99
byte 1 108
byte 1 101
byte 1 97
byte 1 114
byte 1 109
byte 1 97
byte 1 112
byte 1 0
align 1
LABELV $138
byte 1 100
byte 1 101
byte 1 108
byte 1 101
byte 1 116
byte 1 101
byte 1 109
byte 1 97
byte 1 112
byte 1 0
align 1
LABELV $137
byte 1 115
byte 1 97
byte 1 118
byte 1 101
byte 1 109
byte 1 97
byte 1 112
byte 1 0
align 1
LABELV $136
byte 1 97
byte 1 100
byte 1 100
byte 1 98
byte 1 111
byte 1 116
byte 1 0
align 1
LABELV $135
byte 1 102
byte 1 111
byte 1 114
byte 1 99
byte 1 101
byte 1 84
byte 1 101
byte 1 97
byte 1 109
byte 1 0
align 1
LABELV $134
byte 1 101
byte 1 110
byte 1 116
byte 1 105
byte 1 116
byte 1 121
byte 1 76
byte 1 105
byte 1 115
byte 1 116
byte 1 0
align 1
LABELV $133
byte 1 102
byte 1 114
byte 1 101
byte 1 101
byte 1 115
byte 1 112
byte 1 101
byte 1 99
byte 1 116
byte 1 97
byte 1 116
byte 1 111
byte 1 114
byte 1 0
align 1
LABELV $132
byte 1 102
byte 1 111
byte 1 108
byte 1 108
byte 1 111
byte 1 119
byte 1 112
byte 1 114
byte 1 101
byte 1 118
byte 1 0
align 1
LABELV $131
byte 1 102
byte 1 111
byte 1 108
byte 1 108
byte 1 111
byte 1 119
byte 1 110
byte 1 101
byte 1 120
byte 1 116
byte 1 0
align 1
LABELV $130
byte 1 97
byte 1 99
byte 1 116
byte 1 105
byte 1 118
byte 1 97
byte 1 116
byte 1 101
byte 1 0
align 1
LABELV $129
byte 1 117
byte 1 110
byte 1 100
byte 1 111
byte 1 0
align 1
LABELV $128
byte 1 102
byte 1 108
byte 1 97
byte 1 115
byte 1 104
byte 1 108
byte 1 105
byte 1 103
byte 1 104
byte 1 116
byte 1 0
align 1
LABELV $127
byte 1 107
byte 1 105
byte 1 108
byte 1 108
byte 1 0
align 1
LABELV $126
byte 1 101
byte 1 120
byte 1 105
byte 1 116
byte 1 118
byte 1 101
byte 1 104
byte 1 105
byte 1 99
byte 1 108
byte 1 101
byte 1 0
align 1
LABELV $125
byte 1 110
byte 1 111
byte 1 99
byte 1 108
byte 1 105
byte 1 112
byte 1 0
align 1
LABELV $124
byte 1 116
byte 1 112
byte 1 0
align 1
LABELV $123
byte 1 103
byte 1 111
byte 1 100
byte 1 0
align 1
LABELV $122
byte 1 103
byte 1 105
byte 1 118
byte 1 101
byte 1 0
align 1
LABELV $121
byte 1 116
byte 1 101
byte 1 97
byte 1 109
byte 1 0
align 1
LABELV $120
byte 1 116
byte 1 101
byte 1 108
byte 1 108
byte 1 0
align 1
LABELV $119
byte 1 115
byte 1 97
byte 1 121
byte 1 95
byte 1 116
byte 1 101
byte 1 97
byte 1 109
byte 1 0
align 1
LABELV $118
byte 1 115
byte 1 97
byte 1 121
byte 1 0
align 1
LABELV $104
byte 1 99
byte 1 104
byte 1 97
byte 1 110
byte 1 103
byte 1 101
byte 1 116
byte 1 101
byte 1 120
byte 1 116
byte 1 117
byte 1 114
byte 1 101
byte 1 0
align 1
LABELV $103
byte 1 119
byte 1 101
byte 1 97
byte 1 112
byte 1 111
byte 1 110
byte 1 0
align 1
LABELV $102
byte 1 119
byte 1 101
byte 1 97
byte 1 112
byte 1 112
byte 1 114
byte 1 101
byte 1 118
byte 1 0
align 1
LABELV $101
byte 1 119
byte 1 101
byte 1 97
byte 1 112
byte 1 110
byte 1 101
byte 1 120
byte 1 116
byte 1 0
align 1
LABELV $100
byte 1 45
byte 1 122
byte 1 111
byte 1 111
byte 1 109
byte 1 0
align 1
LABELV $99
byte 1 43
byte 1 122
byte 1 111
byte 1 111
byte 1 109
byte 1 0
align 1
LABELV $98
byte 1 45
byte 1 115
byte 1 99
byte 1 111
byte 1 114
byte 1 101
byte 1 115
byte 1 0
align 1
LABELV $97
byte 1 43
byte 1 115
byte 1 99
byte 1 111
byte 1 114
byte 1 101
byte 1 115
byte 1 0
align 1
LABELV $95
byte 1 48
byte 1 46
byte 1 48
byte 1 48
byte 1 53
byte 1 0
align 1
LABELV $82
byte 1 115
byte 1 99
byte 1 111
byte 1 114
byte 1 101
byte 1 0
