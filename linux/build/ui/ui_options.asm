export UI_Options
code
proc UI_Options 36 48
ADDRGP4 options
ARGP4
CNSTI4 0
ARGI4
CNSTI4 49824
ARGI4
ADDRGP4 memset
CALLP4
pop
ADDRGP4 options+532
CNSTI4 1
ASGNI4
ADDRGP4 options+536
ARGP4
CNSTF4 1114112000
ADDRGP4 uis+92
INDIRF4
SUBF4
ARGF4
CNSTF4 1118830592
ARGF4
ADDRGP4 $64
ARGP4
CNSTI4 0
ARGI4
CNSTF4 1072064102
ARGF4
ADDRGP4 UI_CText
CALLV
pop
ADDRLP4 0
CNSTI4 145
ASGNI4
ADDRGP4 options+536+488
ARGP4
CNSTF4 1115684864
ADDRGP4 uis+92
INDIRF4
SUBF4
ARGF4
ADDRLP4 0
INDIRI4
CVIF4 4
ARGF4
ADDRGP4 $68
ARGP4
ADDRLP4 4
CNSTI4 0
ASGNI4
ADDRLP4 4
INDIRI4
ARGI4
CNSTF4 1065353216
ARGF4
ADDRGP4 color_white
ARGP4
ADDRLP4 8
CNSTP4 0
ASGNP4
ADDRLP4 8
INDIRP4
ARGP4
ADDRLP4 8
INDIRP4
ARGP4
ADDRGP4 UI_PlayerModelMenu
ARGP4
CNSTP4 0
ARGP4
ADDRLP4 4
INDIRI4
ARGI4
ADDRGP4 UI_CButton
CALLV
pop
ADDRLP4 0
ADDRLP4 0
INDIRI4
CNSTI4 18
ADDI4
ASGNI4
ADDRGP4 options+536+976
ARGP4
CNSTF4 1115684864
ADDRGP4 uis+92
INDIRF4
SUBF4
ARGF4
ADDRLP4 0
INDIRI4
CVIF4 4
ARGF4
ADDRGP4 $72
ARGP4
ADDRLP4 12
CNSTI4 0
ASGNI4
ADDRLP4 12
INDIRI4
ARGI4
CNSTF4 1065353216
ARGF4
ADDRGP4 color_white
ARGP4
ADDRLP4 16
CNSTP4 0
ASGNP4
ADDRLP4 16
INDIRP4
ARGP4
ADDRLP4 16
INDIRP4
ARGP4
ADDRGP4 UI_Controls
ARGP4
CNSTP4 0
ARGP4
ADDRLP4 12
INDIRI4
ARGI4
ADDRGP4 UI_CButton
CALLV
pop
ADDRLP4 0
ADDRLP4 0
INDIRI4
CNSTI4 18
ADDI4
ASGNI4
ADDRGP4 options+536+1464
ARGP4
CNSTF4 1115684864
ADDRGP4 uis+92
INDIRF4
SUBF4
ARGF4
ADDRLP4 0
INDIRI4
CVIF4 4
ARGF4
ADDRGP4 $76
ARGP4
ADDRLP4 20
CNSTI4 0
ASGNI4
ADDRLP4 20
INDIRI4
ARGI4
CNSTF4 1065353216
ARGF4
ADDRGP4 color_white
ARGP4
ADDRLP4 24
CNSTP4 0
ASGNP4
ADDRLP4 24
INDIRP4
ARGP4
ADDRLP4 24
INDIRP4
ARGP4
ADDRGP4 UI_Settings
ARGP4
CNSTP4 0
ARGP4
ADDRLP4 20
INDIRI4
ARGI4
ADDRGP4 UI_CButton
CALLV
pop
ADDRLP4 0
ADDRLP4 0
INDIRI4
CNSTI4 36
ADDI4
ASGNI4
ADDRGP4 options+536+2440
ARGP4
CNSTF4 1115684864
ADDRGP4 uis+92
INDIRF4
SUBF4
ARGF4
ADDRLP4 0
INDIRI4
CVIF4 4
ARGF4
ADDRGP4 $80
ARGP4
ADDRLP4 28
CNSTI4 0
ASGNI4
ADDRLP4 28
INDIRI4
ARGI4
CNSTF4 1065353216
ARGF4
ADDRGP4 color_white
ARGP4
ADDRLP4 32
CNSTP4 0
ASGNP4
ADDRLP4 32
INDIRP4
ARGP4
ADDRLP4 32
INDIRP4
ARGP4
ADDRGP4 UI_PopMenu
ARGP4
CNSTP4 0
ARGP4
ADDRLP4 28
INDIRI4
ARGI4
ADDRGP4 UI_CButton
CALLV
pop
ADDRGP4 options
ARGP4
ADDRGP4 options+536
ARGP4
ADDRGP4 UI_CreateUI
CALLV
pop
ADDRGP4 options
ARGP4
ADDRGP4 UI_PushMenu
CALLV
pop
LABELV $60
endproc UI_Options 36 48
bss
align 4
LABELV options
skip 49824
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
import UI_DrawConnectScreen
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
LABELV $80
byte 1 66
byte 1 97
byte 1 99
byte 1 107
byte 1 0
align 1
LABELV $76
byte 1 83
byte 1 101
byte 1 116
byte 1 116
byte 1 105
byte 1 110
byte 1 103
byte 1 115
byte 1 0
align 1
LABELV $72
byte 1 67
byte 1 111
byte 1 110
byte 1 116
byte 1 114
byte 1 111
byte 1 108
byte 1 115
byte 1 0
align 1
LABELV $68
byte 1 80
byte 1 108
byte 1 97
byte 1 121
byte 1 101
byte 1 114
byte 1 0
align 1
LABELV $64
byte 1 79
byte 1 80
byte 1 84
byte 1 73
byte 1 79
byte 1 78
byte 1 83
byte 1 0
