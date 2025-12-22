data
export passwordNeeded
align 4
LABELV passwordNeeded
byte 4 1
export UI_DrawConnectScreen
code
proc UI_DrawConnectScreen 4176 24
ADDRGP4 UI_UpdateState
CALLV
pop
ADDRGP4 console
ARGP4
ADDRGP4 console+4194304
INDIRI4
ARGI4
ADDRGP4 consoleSync
CALLV
pop
ADDRLP4 8
ARGP4
ADDRGP4 trap_GetClientState
CALLV
pop
ADDRLP4 3092
CNSTI1 0
ASGNI1
CNSTI4 0
ARGI4
ADDRLP4 3092
ARGP4
CNSTI4 1024
ARGI4
ADDRGP4 trap_GetConfigString
CALLI4
pop
ADDRGP4 Menu_Cache
CALLV
pop
ADDRGP4 $61
ARGP4
ADDRLP4 4132
ADDRGP4 trap_R_RegisterShaderNoMip
CALLI4
ASGNI4
ADDRLP4 4116
ADDRLP4 4132
INDIRI4
ASGNI4
ADDRGP4 $62
ARGP4
ADDRLP4 4136
ADDRGP4 trap_R_RegisterShaderNoMip
CALLI4
ASGNI4
ADDRLP4 4120
ADDRLP4 4136
INDIRI4
ASGNI4
ADDRGP4 $63
ARGP4
ADDRLP4 4140
ADDRGP4 trap_R_RegisterShaderNoMip
CALLI4
ASGNI4
ADDRLP4 4124
ADDRLP4 4140
INDIRI4
ASGNI4
ADDRLP4 8
INDIRI4
CNSTI4 4
GEI4 $64
ADDRLP4 4144
CNSTF4 1073741824
ASGNF4
ADDRLP4 4144
INDIRF4
ADDRGP4 uis+92
INDIRF4
SUBF4
ARGF4
ADDRLP4 4144
INDIRF4
ARGF4
ADDRLP4 8+2060
ARGP4
CNSTI4 17
ARGI4
ADDRGP4 color_white
ARGP4
CNSTF4 1065353216
ARGF4
ADDRGP4 ST_DrawString
CALLV
pop
LABELV $64
ADDRGP4 lastConnState
INDIRI4
ADDRLP4 8
INDIRI4
LEI4 $68
ADDRGP4 lastLoadingText
CNSTI1 0
ASGNI1
LABELV $68
ADDRGP4 lastConnState
ADDRLP4 8
INDIRI4
ASGNI4
ADDRLP4 4
CNSTI4 449
ASGNI4
ADDRLP4 0
ADDRGP4 console+4194304
INDIRI4
ASGNI4
ADDRGP4 $73
JUMPV
LABELV $70
ADDRLP4 0
INDIRI4
CNSTI4 0
GTI4 $76
ADDRGP4 $72
JUMPV
LABELV $76
CNSTF4 1097859072
ADDRGP4 uis+92
INDIRF4
SUBF4
ARGF4
ADDRLP4 4
INDIRI4
CVIF4 4
ARGF4
ADDRLP4 0
INDIRI4
CNSTI4 8
LSHI4
ADDRGP4 console
ADDP4
ARGP4
CNSTI4 16
ARGI4
ADDRGP4 color_white
ARGP4
CNSTF4 1063675494
ARGF4
ADDRGP4 ST_DrawString
CALLV
pop
ADDRLP4 4
ADDRLP4 4
INDIRI4
CNSTI4 11
SUBI4
ASGNI4
LABELV $71
ADDRLP4 0
ADDRLP4 0
INDIRI4
CNSTI4 1
SUBI4
ASGNI4
LABELV $73
ADDRLP4 0
INDIRI4
ADDRGP4 console+4194304
INDIRI4
CNSTI4 8
SUBI4
GTI4 $70
LABELV $72
ADDRLP4 4144
ADDRLP4 8
INDIRI4
ASGNI4
ADDRLP4 4152
CNSTI4 2
ASGNI4
ADDRLP4 4144
INDIRI4
ADDRLP4 4152
INDIRI4
LTI4 $59
ADDRLP4 4144
INDIRI4
CNSTI4 6
GTI4 $59
ADDRLP4 4144
INDIRI4
ADDRLP4 4152
INDIRI4
LSHI4
ADDRGP4 $92-8
ADDP4
INDIRP4
JUMPV
lit
align 4
LABELV $92
address $82
address $85
address $88
address $59
address $59
code
LABELV $82
ADDRGP4 $83
ARGP4
ADDRLP4 8+4
INDIRI4
ARGI4
ADDRLP4 4156
ADDRGP4 va
CALLP4
ASGNP4
ADDRLP4 4128
ADDRLP4 4156
INDIRP4
ASGNP4
ADDRGP4 $80
JUMPV
LABELV $85
ADDRGP4 $86
ARGP4
ADDRLP4 8+4
INDIRI4
ARGI4
ADDRLP4 4160
ADDRGP4 va
CALLP4
ASGNP4
ADDRLP4 4128
ADDRLP4 4160
INDIRP4
ASGNP4
ADDRGP4 $80
JUMPV
LABELV $88
ADDRLP4 4128
ADDRGP4 $89
ASGNP4
LABELV $80
ADDRLP4 4156
CNSTF4 0
ASGNF4
ADDRLP4 4156
INDIRF4
ADDRGP4 uis+92
INDIRF4
CNSTF4 1065353216
ADDF4
SUBF4
ARGF4
ADDRLP4 4156
INDIRF4
ARGF4
ADDRLP4 4160
CNSTF4 1073741824
ASGNF4
ADDRLP4 4160
INDIRF4
ADDRGP4 uis+92
INDIRF4
MULF4
CNSTF4 1142947840
ADDF4
ADDRLP4 4160
INDIRF4
ADDF4
ARGF4
CNSTF4 1152647168
ARGF4
ADDRLP4 4116
INDIRI4
ARGI4
ADDRGP4 UI_DrawHandlePic
CALLV
pop
CNSTF4 1132920832
ARGF4
CNSTF4 1126498304
ARGF4
ADDRLP4 4164
CNSTF4 1120403456
ASGNF4
ADDRLP4 4164
INDIRF4
ARGF4
ADDRLP4 4164
INDIRF4
ARGF4
ADDRLP4 4120
INDIRI4
ARGI4
ADDRGP4 UI_DrawHandlePic
CALLV
pop
CNSTF4 1133772800
ARGF4
CNSTF4 1132986368
ARGF4
ADDRLP4 4168
CNSTF4 1111490560
ASGNF4
ADDRLP4 4168
INDIRF4
ARGF4
ADDRLP4 4168
INDIRF4
ARGF4
ADDRLP4 4124
INDIRI4
ARGI4
ADDRGP4 UI_DrawHandlePic
CALLV
pop
ADDRLP4 4172
CNSTF4 1073741824
ASGNF4
ADDRLP4 4172
INDIRF4
ADDRGP4 uis+92
INDIRF4
SUBF4
ARGF4
ADDRLP4 4172
INDIRF4
ARGF4
ADDRLP4 4128
INDIRP4
ARGP4
CNSTI4 16
ARGI4
ADDRGP4 color_white
ARGP4
CNSTF4 1065353216
ARGF4
ADDRGP4 ST_DrawString
CALLV
pop
LABELV $59
endproc UI_DrawConnectScreen 4176 24
bss
align 1
LABELV lastLoadingText
skip 1024
align 4
LABELV lastConnState
skip 4
export passwordField
align 4
LABELV passwordField
skip 488
import consoleSync
import trap_LAN_GetServerAddressString
import trap_LAN_GetServerCount
import trap_MemoryRemaining
import trap_LAN_GetPingInfo
import trap_LAN_GetPing
import trap_LAN_ClearPing
import trap_LAN_GetPingQueueCount
import trap_GetConfigString
import trap_GetClientState
import trap_GetClipboardData
import trap_Key_SetCatcher
import trap_Key_ClearStates
import trap_Key_SetOverstrikeMode
import trap_Key_GetOverstrikeMode
import trap_Key_IsDown
import trap_Key_SetBinding
import trap_Key_GetBindingBuf
import trap_Key_KeynumToStringBuf
import consoleLines
import console
import gametype_items
import uis
import UI_SpawnMenu
import UI_Settings
import UI_ArenaServersMenu
import UI_CPicture
import UI_CBitmap
import UI_CText
import UI_CField
import UI_CList
import UI_CSpinControl
import UI_CRadioButton
import UI_CSlider
import UI_CButton
import UI_CreateUI
import UI_SetHitbox
import UI_ListPlayerCount
import UI_FillListPlayers
import UI_FillListOfItems
import UI_FillListFromArray
import UI_CountFiles
import UI_FillList
import Menu_Cache
import Menu_DefaultKey
import Menu_ItemAtCursor
import Menu_Draw
import Menu_SetCursor
import Menu_AddItem
import ScrollList_Key
import GUI_PlayerInfo_AnimateModel
import GUI_ModelName
import GUI_PlayerInfo_DrawTeamModel
import GUI_PlayerInfo_InitModel
import UI_PlayerModelMenu
import UI_Options
import UI_NewGame
import UI_MainMenu
import UI_CreateCvars
import UI_CountOfMaps
import UI_FillListOfBots
import UI_FillListOfMaps
import UI_GetNumBots
import UI_GetBotInfoByName
import UI_LoadBots
import UI_LoadArenas
import UI_InitMemory
import UI_Alloc
import UI_Controls
import UI_DrawRoundedRect
import UI_DrawModelElement
import UI_DrawPictureElement
import UI_DrawHandlePic
import UI_AdjustFrom640
import UI_Init
import UI_Shutdown
import UI_ConsoleCommand
import UI_MouseEvent
import UI_KeyEvent
import UI_SetActiveMenu
import UI_IsFullscreen
import UI_ForceMenuOff
import UI_PopMenu
import UI_PushMenu
import UI_UpdateState
import drawTeamModel
import menu_null_sound
import menu_buzz_sound
import menu_out_sound
import menu_move_sound
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
LABELV $89
byte 1 65
byte 1 119
byte 1 97
byte 1 105
byte 1 116
byte 1 105
byte 1 110
byte 1 103
byte 1 32
byte 1 103
byte 1 97
byte 1 109
byte 1 101
byte 1 115
byte 1 116
byte 1 97
byte 1 116
byte 1 101
byte 1 46
byte 1 46
byte 1 46
byte 1 0
align 1
LABELV $86
byte 1 65
byte 1 119
byte 1 97
byte 1 105
byte 1 116
byte 1 105
byte 1 110
byte 1 103
byte 1 32
byte 1 99
byte 1 111
byte 1 110
byte 1 110
byte 1 101
byte 1 99
byte 1 116
byte 1 105
byte 1 111
byte 1 110
byte 1 46
byte 1 46
byte 1 46
byte 1 37
byte 1 105
byte 1 0
align 1
LABELV $83
byte 1 65
byte 1 119
byte 1 97
byte 1 105
byte 1 116
byte 1 105
byte 1 110
byte 1 103
byte 1 32
byte 1 99
byte 1 104
byte 1 97
byte 1 108
byte 1 108
byte 1 101
byte 1 110
byte 1 103
byte 1 101
byte 1 46
byte 1 46
byte 1 46
byte 1 37
byte 1 105
byte 1 0
align 1
LABELV $63
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
LABELV $62
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
LABELV $61
byte 1 103
byte 1 102
byte 1 120
byte 1 47
byte 1 99
byte 1 111
byte 1 108
byte 1 111
byte 1 114
byte 1 115
byte 1 47
byte 1 98
byte 1 108
byte 1 97
byte 1 99
byte 1 107
byte 1 0
