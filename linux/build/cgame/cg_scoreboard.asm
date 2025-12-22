code
proc CG_DrawClientScore 84 24
ADDRFP4 0
ADDRFP4 0
INDIRI4
ASGNI4
ADDRFP4 4
ADDRFP4 4
INDIRP4
ASGNP4
ADDRLP4 48
ADDRFP4 4
INDIRP4
INDIRI4
ASGNI4
ADDRLP4 48
INDIRI4
CNSTI4 0
LTI4 $79
ADDRLP4 48
INDIRI4
ADDRGP4 cgs+71984
INDIRI4
LTI4 $76
LABELV $79
ADDRGP4 $75
JUMPV
LABELV $76
ADDRFP4 4
INDIRP4
INDIRI4
ADDRGP4 cg+28
INDIRP4
CNSTI4 184
ADDP4
INDIRI4
NEI4 $80
ADDRGP4 cg+28
INDIRP4
CNSTI4 304
ADDP4
INDIRI4
CNSTI4 3
EQI4 $87
ADDRGP4 cgs+71964
INDIRI4
CNSTI4 2
LTI4 $83
LABELV $87
ADDRLP4 68
CNSTI4 -1
ASGNI4
ADDRGP4 $84
JUMPV
LABELV $83
ADDRLP4 68
ADDRGP4 cg+28
INDIRP4
CNSTI4 300
ADDP4
INDIRI4
CNSTI4 -16385
BANDI4
ASGNI4
LABELV $84
ADDRGP4 $89
ARGP4
ADDRLP4 72
ADDRGP4 cvarFloat
CALLF4
ASGNF4
ADDRLP4 52
ADDRLP4 72
INDIRF4
ASGNF4
ADDRGP4 $91
ARGP4
ADDRLP4 76
ADDRGP4 cvarFloat
CALLF4
ASGNF4
ADDRLP4 52+4
ADDRLP4 76
INDIRF4
ASGNF4
ADDRGP4 $93
ARGP4
ADDRLP4 80
ADDRGP4 cvarFloat
CALLF4
ASGNF4
ADDRLP4 52+8
ADDRLP4 80
INDIRF4
ASGNF4
ADDRLP4 52+12
CNSTF4 1053609165
ASGNF4
CNSTF4 1126583763
ARGF4
ADDRFP4 0
INDIRI4
CVIF4 4
ARGF4
CNSTF4 1134146093
ARGF4
CNSTF4 1098907648
ARGF4
CNSTF4 0
ARGF4
ADDRLP4 52
ARGP4
ADDRGP4 CG_DrawRoundedRect
CALLV
pop
LABELV $80
ADDRLP4 44
CNSTI4 2880
ADDRFP4 4
INDIRP4
INDIRI4
MULI4
ADDRGP4 cgs+93776
ADDP4
ASGNP4
CNSTF4 1126583763
ARGF4
ADDRFP4 0
INDIRI4
CVIF4 4
ARGF4
ADDRLP4 56
CNSTF4 1098907648
ASGNF4
ADDRLP4 56
INDIRF4
ARGF4
ADDRLP4 56
INDIRF4
ARGF4
ADDRFP4 4
INDIRP4
INDIRI4
ARGI4
ADDRGP4 CG_DrawHead
CALLV
pop
ADDRLP4 44
INDIRP4
CNSTI4 260
ADDP4
INDIRI4
CNSTI4 3
NEI4 $96
ADDRLP4 0
ARGP4
CNSTI4 36
ARGI4
ADDRGP4 $98
ARGP4
ADDRLP4 44
INDIRP4
CNSTI4 4
ADDP4
ARGP4
ADDRGP4 Com_sprintf
CALLV
pop
ADDRLP4 36
ARGP4
CNSTI4 8
ARGI4
ADDRGP4 $99
ARGP4
ADDRGP4 Com_sprintf
CALLV
pop
ADDRGP4 $97
JUMPV
LABELV $96
ADDRLP4 0
ARGP4
CNSTI4 36
ARGI4
ADDRGP4 $98
ARGP4
ADDRLP4 44
INDIRP4
CNSTI4 4
ADDP4
ARGP4
ADDRGP4 Com_sprintf
CALLV
pop
ADDRLP4 36
ARGP4
CNSTI4 8
ARGI4
ADDRGP4 $100
ARGP4
ADDRFP4 4
INDIRP4
CNSTI4 4
ADDP4
INDIRI4
ARGI4
ADDRGP4 Com_sprintf
CALLV
pop
LABELV $97
CNSTF4 1127894483
ARGF4
ADDRFP4 0
INDIRI4
CNSTI4 2
ADDI4
CVIF4 4
ARGF4
ADDRLP4 0
ARGP4
CNSTI4 16
ARGI4
ADDRGP4 color_white
ARGP4
CNSTF4 1067030938
ARGF4
ADDRGP4 ST_DrawString
CALLV
pop
CNSTF4 1138105516
ARGF4
ADDRFP4 0
INDIRI4
CNSTI4 2
ADDI4
CVIF4 4
ARGF4
ADDRLP4 36
ARGP4
CNSTI4 16
ARGI4
ADDRGP4 color_white
ARGP4
CNSTF4 1067030938
ARGF4
ADDRGP4 ST_DrawString
CALLV
pop
LABELV $75
endproc CG_DrawClientScore 84 24
proc CG_TeamScoreboard 24 8
ADDRFP4 0
ADDRFP4 0
INDIRI4
ASGNI4
ADDRFP4 4
ADDRFP4 4
INDIRI4
ASGNI4
ADDRFP4 8
ADDRFP4 8
INDIRI4
ASGNI4
ADDRFP4 12
ADDRFP4 12
INDIRI4
ASGNI4
ADDRLP4 20
CNSTI4 0
ASGNI4
ADDRLP4 4
ADDRLP4 20
INDIRI4
ASGNI4
ADDRLP4 16
ADDRLP4 20
INDIRI4
ASGNI4
ADDRLP4 8
ADDRLP4 20
INDIRI4
ASGNI4
ADDRGP4 $105
JUMPV
LABELV $102
ADDRLP4 0
ADDRLP4 8
INDIRI4
CNSTI4 3
LSHI4
ADDRGP4 cg+1839048
ADDP4
ASGNP4
ADDRLP4 12
CNSTI4 2880
ADDRLP4 0
INDIRP4
INDIRI4
MULI4
ADDRGP4 cgs+93776
ADDP4
ASGNP4
ADDRFP4 4
INDIRI4
ADDRLP4 12
INDIRP4
CNSTI4 260
ADDP4
INDIRI4
EQI4 $109
ADDRGP4 $103
JUMPV
LABELV $109
ADDRLP4 0
INDIRP4
INDIRI4
CNSTI4 0
NEI4 $111
ADDRLP4 16
INDIRI4
CNSTI4 0
EQI4 $113
ADDRGP4 $103
JUMPV
LABELV $113
ADDRLP4 16
CNSTI4 1
ASGNI4
LABELV $111
ADDRFP4 0
INDIRI4
ADDRFP4 12
INDIRI4
ADDRLP4 4
INDIRI4
MULI4
ADDI4
ARGI4
ADDRLP4 0
INDIRP4
ARGP4
ADDRGP4 CG_DrawClientScore
CALLV
pop
ADDRLP4 4
ADDRLP4 4
INDIRI4
CNSTI4 1
ADDI4
ASGNI4
LABELV $103
ADDRLP4 8
ADDRLP4 8
INDIRI4
CNSTI4 1
ADDI4
ASGNI4
LABELV $105
ADDRLP4 8
INDIRI4
ADDRGP4 cg+1839036
INDIRI4
GEI4 $115
ADDRLP4 4
INDIRI4
ADDRFP4 8
INDIRI4
LTI4 $102
LABELV $115
ADDRLP4 4
INDIRI4
RETI4
LABELV $101
endproc CG_TeamScoreboard 24 8
export CG_DrawScoreboard
proc CG_DrawScoreboard 40 24
ADDRGP4 cg+1840072
INDIRI4
CNSTI4 0
NEI4 $117
ADDRGP4 cg+1836004+4
INDIRI4
CNSTI4 3
EQI4 $117
ADDRGP4 cg+1836004+4
INDIRI4
CNSTI4 5
EQI4 $117
CNSTI4 0
RETI4
ADDRGP4 $116
JUMPV
LABELV $117
CNSTF4 1126321619
ARGF4
CNSTF4 1109917696
ARGF4
CNSTF4 1134408237
ARGF4
CNSTF4 1137704960
ARGF4
CNSTF4 1086324736
ARGF4
ADDRGP4 color_dim
ARGP4
ADDRGP4 CG_DrawRoundedRect
CALLV
pop
ADDRLP4 0
CNSTI4 50
ASGNI4
ADDRLP4 4
CNSTI4 16
ASGNI4
ADDRGP4 cgs+71964
INDIRI4
CNSTI4 2
LTI4 $124
ADDRLP4 8
CNSTI4 24
ASGNI4
ADDRLP4 0
ADDRLP4 0
INDIRI4
CNSTI4 5
SUBI4
ASGNI4
ADDRLP4 0
ADDRLP4 0
INDIRI4
ADDRLP4 4
INDIRI4
CNSTI4 2
DIVI4
ADDI4
ASGNI4
ADDRGP4 cg+1839040
INDIRI4
ADDRGP4 cg+1839040+4
INDIRI4
LTI4 $127
ADDRLP4 0
INDIRI4
ARGI4
CNSTI4 1
ARGI4
ADDRLP4 8
INDIRI4
ARGI4
ADDRLP4 4
INDIRI4
ARGI4
ADDRLP4 20
ADDRGP4 CG_TeamScoreboard
CALLI4
ASGNI4
ADDRLP4 12
ADDRLP4 20
INDIRI4
ASGNI4
ADDRLP4 0
ADDRLP4 0
INDIRI4
ADDRLP4 12
INDIRI4
ADDRLP4 4
INDIRI4
MULI4
CNSTI4 12
ADDI4
ADDI4
ASGNI4
ADDRLP4 8
ADDRLP4 8
INDIRI4
ADDRLP4 12
INDIRI4
SUBI4
ASGNI4
ADDRLP4 0
INDIRI4
ARGI4
CNSTI4 2
ARGI4
ADDRLP4 8
INDIRI4
ARGI4
ADDRLP4 4
INDIRI4
ARGI4
ADDRLP4 32
ADDRGP4 CG_TeamScoreboard
CALLI4
ASGNI4
ADDRLP4 16
ADDRLP4 32
INDIRI4
ASGNI4
ADDRLP4 36
ADDRLP4 16
INDIRI4
ASGNI4
ADDRLP4 0
ADDRLP4 0
INDIRI4
ADDRLP4 36
INDIRI4
ADDRLP4 4
INDIRI4
MULI4
CNSTI4 12
ADDI4
ADDI4
ASGNI4
ADDRLP4 8
ADDRLP4 8
INDIRI4
ADDRLP4 36
INDIRI4
SUBI4
ASGNI4
ADDRGP4 $128
JUMPV
LABELV $127
ADDRLP4 0
INDIRI4
ARGI4
CNSTI4 2
ARGI4
ADDRLP4 8
INDIRI4
ARGI4
ADDRLP4 4
INDIRI4
ARGI4
ADDRLP4 20
ADDRGP4 CG_TeamScoreboard
CALLI4
ASGNI4
ADDRLP4 12
ADDRLP4 20
INDIRI4
ASGNI4
ADDRLP4 0
ADDRLP4 0
INDIRI4
ADDRLP4 12
INDIRI4
ADDRLP4 4
INDIRI4
MULI4
CNSTI4 12
ADDI4
ADDI4
ASGNI4
ADDRLP4 8
ADDRLP4 8
INDIRI4
ADDRLP4 12
INDIRI4
SUBI4
ASGNI4
ADDRLP4 0
INDIRI4
ARGI4
CNSTI4 1
ARGI4
ADDRLP4 8
INDIRI4
ARGI4
ADDRLP4 4
INDIRI4
ARGI4
ADDRLP4 32
ADDRGP4 CG_TeamScoreboard
CALLI4
ASGNI4
ADDRLP4 16
ADDRLP4 32
INDIRI4
ASGNI4
ADDRLP4 36
ADDRLP4 16
INDIRI4
ASGNI4
ADDRLP4 0
ADDRLP4 0
INDIRI4
ADDRLP4 36
INDIRI4
ADDRLP4 4
INDIRI4
MULI4
CNSTI4 12
ADDI4
ADDI4
ASGNI4
ADDRLP4 8
ADDRLP4 8
INDIRI4
ADDRLP4 36
INDIRI4
SUBI4
ASGNI4
LABELV $128
ADDRLP4 0
INDIRI4
ARGI4
CNSTI4 3
ARGI4
ADDRLP4 8
INDIRI4
ARGI4
ADDRLP4 4
INDIRI4
ARGI4
ADDRLP4 20
ADDRGP4 CG_TeamScoreboard
CALLI4
ASGNI4
ADDRLP4 12
ADDRLP4 20
INDIRI4
ASGNI4
ADDRLP4 0
ADDRLP4 0
INDIRI4
ADDRLP4 12
INDIRI4
ADDRLP4 4
INDIRI4
MULI4
CNSTI4 12
ADDI4
ADDI4
ASGNI4
ADDRGP4 $125
JUMPV
LABELV $124
ADDRLP4 8
CNSTI4 25
ASGNI4
ADDRLP4 0
INDIRI4
ARGI4
CNSTI4 0
ARGI4
ADDRLP4 8
INDIRI4
ARGI4
ADDRLP4 4
INDIRI4
ARGI4
ADDRLP4 20
ADDRGP4 CG_TeamScoreboard
CALLI4
ASGNI4
ADDRLP4 12
ADDRLP4 20
INDIRI4
ASGNI4
ADDRLP4 0
ADDRLP4 0
INDIRI4
ADDRLP4 12
INDIRI4
ADDRLP4 4
INDIRI4
MULI4
CNSTI4 12
ADDI4
ADDI4
ASGNI4
ADDRLP4 0
INDIRI4
ARGI4
CNSTI4 3
ARGI4
ADDRLP4 8
INDIRI4
ADDRLP4 12
INDIRI4
SUBI4
ARGI4
ADDRLP4 4
INDIRI4
ARGI4
ADDRLP4 32
ADDRGP4 CG_TeamScoreboard
CALLI4
ASGNI4
ADDRLP4 16
ADDRLP4 32
INDIRI4
ASGNI4
ADDRLP4 0
ADDRLP4 0
INDIRI4
ADDRLP4 16
INDIRI4
ADDRLP4 4
INDIRI4
MULI4
CNSTI4 12
ADDI4
ADDI4
ASGNI4
LABELV $125
CNSTI4 1
RETI4
LABELV $116
endproc CG_DrawScoreboard 40 24
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
LABELV $100
byte 1 37
byte 1 105
byte 1 0
align 1
LABELV $99
byte 1 83
byte 1 80
byte 1 69
byte 1 67
byte 1 84
byte 1 0
align 1
LABELV $98
byte 1 37
byte 1 115
byte 1 0
align 1
LABELV $93
byte 1 99
byte 1 103
byte 1 95
byte 1 99
byte 1 114
byte 1 111
byte 1 115
byte 1 115
byte 1 104
byte 1 97
byte 1 105
byte 1 114
byte 1 67
byte 1 111
byte 1 108
byte 1 111
byte 1 114
byte 1 66
byte 1 108
byte 1 117
byte 1 101
byte 1 0
align 1
LABELV $91
byte 1 99
byte 1 103
byte 1 95
byte 1 99
byte 1 114
byte 1 111
byte 1 115
byte 1 115
byte 1 104
byte 1 97
byte 1 105
byte 1 114
byte 1 67
byte 1 111
byte 1 108
byte 1 111
byte 1 114
byte 1 71
byte 1 114
byte 1 101
byte 1 101
byte 1 110
byte 1 0
align 1
LABELV $89
byte 1 99
byte 1 103
byte 1 95
byte 1 99
byte 1 114
byte 1 111
byte 1 115
byte 1 115
byte 1 104
byte 1 97
byte 1 105
byte 1 114
byte 1 67
byte 1 111
byte 1 108
byte 1 111
byte 1 114
byte 1 82
byte 1 101
byte 1 100
byte 1 0
