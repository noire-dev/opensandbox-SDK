export G_BotConnect
code
proc G_BotConnect 1176 12
ADDRFP4 0
INDIRI4
ARGI4
ADDRLP4 144
ARGP4
CNSTI4 1024
ARGI4
ADDRGP4 trap_GetUserinfo
CALLV
pop
ADDRLP4 144
ARGP4
ADDRGP4 $55
ARGP4
ADDRLP4 1168
ADDRGP4 Info_ValueForKey
CALLP4
ASGNP4
ADDRLP4 0
ARGP4
ADDRLP4 1168
INDIRP4
ARGP4
CNSTI4 144
ARGI4
ADDRGP4 Q_strncpyz
CALLV
pop
ADDRFP4 0
INDIRI4
ARGI4
ADDRLP4 0
ARGP4
ADDRLP4 1172
ADDRGP4 BotAISetupClient
CALLI4
ASGNI4
ADDRLP4 1172
INDIRI4
CNSTI4 0
NEI4 $56
ADDRFP4 0
INDIRI4
ARGI4
ADDRGP4 $58
ARGP4
ADDRGP4 trap_DropClient
CALLV
pop
CNSTI4 0
RETI4
ADDRGP4 $54
JUMPV
LABELV $56
CNSTI4 1
RETI4
LABELV $54
endproc G_BotConnect 1176 12
proc G_BotSandboxCheck 24 4
ADDRFP4 0
ADDRFP4 0
INDIRP4
ASGNP4
ADDRLP4 0
ADDRFP4 0
INDIRP4
CNSTI4 628
ADDP4
INDIRP4
ASGNP4
ADDRLP4 4
CNSTU4 0
ASGNU4
ADDRLP4 0
INDIRP4
CVPU4 4
ADDRLP4 4
INDIRU4
EQU4 $63
ADDRLP4 8
ADDRLP4 0
INDIRP4
CNSTI4 548
ADDP4
INDIRP4
ASGNP4
ADDRLP4 8
INDIRP4
CVPU4 4
ADDRLP4 4
INDIRU4
EQU4 $63
ADDRLP4 8
INDIRP4
CNSTI4 404
ADDP4
INDIRI4
CNSTI4 0
NEI4 $60
LABELV $63
ADDRFP4 0
INDIRP4
ARGP4
ADDRGP4 G_FreeEntity
CALLV
pop
ADDRGP4 $59
JUMPV
LABELV $60
ADDRFP4 0
INDIRP4
CNSTI4 92
ADDP4
ADDRFP4 0
INDIRP4
CNSTI4 628
ADDP4
INDIRP4
CNSTI4 24
ADDP4
INDIRF4
ASGNF4
ADDRFP4 0
INDIRP4
CNSTI4 96
ADDP4
ADDRFP4 0
INDIRP4
CNSTI4 628
ADDP4
INDIRP4
CNSTI4 28
ADDP4
INDIRF4
ASGNF4
ADDRFP4 0
INDIRP4
CNSTI4 100
ADDP4
ADDRFP4 0
INDIRP4
CNSTI4 628
ADDP4
INDIRP4
CNSTI4 32
ADDP4
INDIRF4
ASGNF4
ADDRFP4 0
INDIRP4
CNSTI4 712
ADDP4
ADDRGP4 G_BotSandboxCheck
ASGNP4
ADDRFP4 0
INDIRP4
CNSTI4 708
ADDP4
ADDRGP4 level+24
INDIRI4
CNSTI4 1
ADDI4
ASGNI4
LABELV $59
endproc G_BotSandboxCheck 24 4
export G_AddBot
proc G_AddBot 1060 12
ADDRFP4 0
ADDRFP4 0
INDIRP4
ASGNP4
ADDRFP4 8
ADDRFP4 8
INDIRP4
ASGNP4
ADDRFP4 12
ADDRFP4 12
INDIRP4
ASGNP4
ADDRLP4 0
CNSTI1 0
ASGNI1
ADDRFP4 4
INDIRP4
ARGP4
ADDRLP4 1032
ADDRGP4 strlen
CALLI4
ASGNI4
ADDRLP4 1032
INDIRI4
CNSTI4 0
NEI4 $66
ADDRFP4 4
ADDRFP4 0
INDIRP4
ASGNP4
LABELV $66
ADDRLP4 0
ARGP4
ADDRGP4 $68
ARGP4
ADDRFP4 4
INDIRP4
ARGP4
ADDRGP4 Info_SetValueForKey
CALLV
pop
ADDRLP4 0
ARGP4
ADDRGP4 $69
ARGP4
ADDRGP4 $70
ARGP4
ADDRGP4 Info_SetValueForKey
CALLV
pop
ADDRLP4 0
ARGP4
ADDRGP4 $71
ARGP4
ADDRGP4 $72
ARGP4
ADDRGP4 Info_SetValueForKey
CALLV
pop
ADDRFP4 0
INDIRP4
INDIRI1
CVII4 1
CNSTI4 0
NEI4 $73
ADDRFP4 0
ADDRGP4 $75
ASGNP4
LABELV $73
ADDRLP4 0
ARGP4
ADDRGP4 $76
ARGP4
ADDRFP4 0
INDIRP4
ARGP4
ADDRGP4 Info_SetValueForKey
CALLV
pop
ADDRGP4 $80
ARGP4
ADDRLP4 1036
ADDRGP4 cvarInt
CALLI4
ASGNI4
ADDRGP4 level+64
INDIRI4
ADDRLP4 1036
INDIRI4
LTI4 $77
ADDRGP4 $81
ARGP4
ADDRGP4 G_Printf
CALLV
pop
ADDRGP4 $65
JUMPV
LABELV $77
ADDRLP4 1040
ADDRGP4 trap_BotAllocateClient
CALLI4
ASGNI4
ADDRLP4 1024
ADDRLP4 1040
INDIRI4
ASGNI4
ADDRLP4 1024
INDIRI4
CNSTI4 -1
NEI4 $82
ADDRGP4 $84
ARGP4
ADDRGP4 G_Printf
CALLV
pop
ADDRGP4 $65
JUMPV
LABELV $82
ADDRFP4 8
INDIRP4
CVPU4 4
CNSTU4 0
EQU4 $87
ADDRFP4 8
INDIRP4
INDIRI1
CVII4 1
CNSTI4 0
NEI4 $85
LABELV $87
ADDRGP4 $90
ARGP4
ADDRLP4 1048
ADDRGP4 cvarInt
CALLI4
ASGNI4
ADDRLP4 1048
INDIRI4
CNSTI4 2
LTI4 $88
ADDRLP4 1024
INDIRI4
ARGI4
ADDRLP4 1052
ADDRGP4 PickTeam
CALLI4
ASGNI4
ADDRLP4 1052
INDIRI4
CNSTI4 1
NEI4 $91
ADDRFP4 8
ADDRGP4 $93
ASGNP4
ADDRGP4 $89
JUMPV
LABELV $91
ADDRFP4 8
ADDRGP4 $94
ASGNP4
ADDRGP4 $89
JUMPV
LABELV $88
ADDRFP4 8
ADDRGP4 $93
ASGNP4
LABELV $89
LABELV $85
ADDRLP4 0
ARGP4
ADDRGP4 $55
ARGP4
ADDRFP4 8
INDIRP4
ARGP4
ADDRGP4 Info_SetValueForKey
CALLV
pop
ADDRLP4 1028
CNSTI4 1288
ADDRLP4 1024
INDIRI4
MULI4
ADDRGP4 g_entities
ADDP4
ASGNP4
ADDRLP4 1048
ADDRLP4 1028
INDIRP4
CNSTI4 456
ADDP4
ASGNP4
ADDRLP4 1048
INDIRP4
ADDRLP4 1048
INDIRP4
INDIRI4
CNSTI4 2
BORI4
ASGNI4
ADDRLP4 1028
INDIRP4
CNSTI4 552
ADDP4
CNSTI4 1
ASGNI4
ADDRFP4 12
INDIRP4
CVPU4 4
CNSTU4 0
EQU4 $95
ADDRGP4 $98
ARGP4
ADDRFP4 12
INDIRP4
INDIRI4
ARGI4
ADDRLP4 1052
ADDRGP4 va
CALLP4
ASGNP4
ADDRLP4 0
ARGP4
ADDRGP4 $97
ARGP4
ADDRLP4 1052
INDIRP4
ARGP4
ADDRGP4 Info_SetValueForKey
CALLV
pop
ADDRGP4 $98
ARGP4
ADDRFP4 12
INDIRP4
CNSTI4 908
ADDP4
INDIRI4
ARGI4
ADDRLP4 1056
ADDRGP4 va
CALLP4
ASGNP4
ADDRLP4 0
ARGP4
ADDRGP4 $99
ARGP4
ADDRLP4 1056
INDIRP4
ARGP4
ADDRGP4 Info_SetValueForKey
CALLV
pop
ADDRFP4 12
INDIRP4
CNSTI4 628
ADDP4
ADDRLP4 1028
INDIRP4
ASGNP4
ADDRFP4 12
INDIRP4
CNSTI4 712
ADDP4
ADDRGP4 G_BotSandboxCheck
ASGNP4
ADDRFP4 12
INDIRP4
CNSTI4 708
ADDP4
ADDRGP4 level+24
INDIRI4
ASGNI4
ADDRGP4 $96
JUMPV
LABELV $95
ADDRLP4 0
ARGP4
ADDRGP4 $97
ARGP4
ADDRGP4 $101
ARGP4
ADDRGP4 Info_SetValueForKey
CALLV
pop
ADDRLP4 0
ARGP4
ADDRGP4 $99
ARGP4
ADDRGP4 $101
ARGP4
ADDRGP4 Info_SetValueForKey
CALLV
pop
LABELV $96
ADDRLP4 1024
INDIRI4
ARGI4
ADDRLP4 0
ARGP4
ADDRGP4 trap_SetUserinfo
CALLV
pop
ADDRLP4 1024
INDIRI4
ARGI4
ADDRLP4 1052
CNSTI4 1
ASGNI4
ADDRLP4 1052
INDIRI4
ARGI4
ADDRLP4 1052
INDIRI4
ARGI4
ADDRLP4 1056
ADDRGP4 ClientConnect
CALLP4
ASGNP4
ADDRLP4 1056
INDIRP4
CVPU4 4
CNSTU4 0
EQU4 $102
ADDRGP4 $65
JUMPV
LABELV $102
ADDRLP4 1024
INDIRI4
ARGI4
ADDRGP4 ClientBegin
CALLV
pop
LABELV $65
endproc G_AddBot 1060 12
export Svcmd_AddBot_f
proc Svcmd_AddBot_f 3072 16
CNSTI4 1
ARGI4
ADDRLP4 0
ARGP4
CNSTI4 1024
ARGI4
ADDRGP4 trap_Argv
CALLV
pop
ADDRLP4 0
INDIRI1
CVII4 1
CNSTI4 0
NEI4 $105
ADDRGP4 $107
ARGP4
ADDRGP4 trap_Print
CALLV
pop
ADDRGP4 $104
JUMPV
LABELV $105
CNSTI4 2
ARGI4
ADDRLP4 1024
ARGP4
CNSTI4 1024
ARGI4
ADDRGP4 trap_Argv
CALLV
pop
CNSTI4 3
ARGI4
ADDRLP4 2048
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
ADDRLP4 2048
ARGP4
CNSTP4 0
ARGP4
ADDRGP4 G_AddBot
CALLV
pop
LABELV $104
endproc Svcmd_AddBot_f 3072 16
export SandboxBotSpawn
proc SandboxBotSpawn 28 12
ADDRFP4 0
ADDRFP4 0
INDIRP4
ASGNP4
ADDRFP4 4
ADDRFP4 4
INDIRP4
ASGNP4
ADDRLP4 8
CNSTU4 0
ASGNU4
ADDRFP4 0
INDIRP4
CVPU4 4
ADDRLP4 8
INDIRU4
EQU4 $111
ADDRFP4 4
INDIRP4
CVPU4 4
ADDRLP4 8
INDIRU4
NEU4 $109
LABELV $111
ADDRGP4 $108
JUMPV
LABELV $109
ADDRFP4 4
INDIRP4
ARGP4
ADDRGP4 $101
ARGP4
ADDRLP4 12
ADDRGP4 strcmp
CALLI4
ASGNI4
ADDRLP4 12
INDIRI4
CNSTI4 0
NEI4 $112
ADDRFP4 0
INDIRP4
CNSTI4 916
ADDP4
CNSTP4 0
ASGNP4
LABELV $112
ADDRFP4 4
INDIRP4
ARGP4
ADDRLP4 16
ADDRGP4 atoi
CALLI4
ASGNI4
ADDRLP4 4
ADDRLP4 16
INDIRI4
ASGNI4
ADDRLP4 20
CNSTP4 0
ASGNP4
ADDRLP4 0
ADDRLP4 20
INDIRP4
ASGNP4
ADDRFP4 0
INDIRP4
CNSTI4 916
ADDP4
ADDRLP4 20
INDIRP4
ASGNP4
ADDRGP4 $115
JUMPV
LABELV $114
ADDRLP4 0
INDIRP4
INDIRI4
ADDRLP4 4
INDIRI4
NEI4 $118
ADDRFP4 0
INDIRP4
CNSTI4 916
ADDP4
ADDRLP4 0
INDIRP4
ASGNP4
LABELV $118
LABELV $115
ADDRLP4 0
INDIRP4
ARGP4
CNSTI4 556
ARGI4
ADDRGP4 $117
ARGP4
ADDRLP4 24
ADDRGP4 G_Find
CALLP4
ASGNP4
ADDRLP4 0
ADDRLP4 24
INDIRP4
ASGNP4
ADDRLP4 24
INDIRP4
CVPU4 4
CNSTU4 0
NEU4 $114
LABELV $108
endproc SandboxBotSpawn 28 12
import trap_BotInitMoveState
import trap_BotFreeMoveState
import trap_BotAllocMoveState
import trap_BotResetMoveState
import trap_BotMoveToGoal
import trap_EA_ResetInput
import trap_EA_GetInput
import trap_EA_View
import trap_EA_SelectWeapon
import trap_EA_Use
import trap_EA_Attack
import trap_EA_Gesture
import trap_EA_Command
import trap_AAS_TraceAreas
import trap_AAS_PointAreaNum
import trap_AAS_Time
import trap_AAS_Initialized
import trap_BotUpdateEntity
import trap_BotUserCommand
import trap_BotGetServerCommand
import trap_BotLibLoadMap
import trap_BotLibStartFrame
import trap_BotLibShutdown
import trap_BotLibSetup
import trap_GetEntityToken
import trap_GetUsercmd
import trap_BotAllocateClient
import trap_EntityContact
import trap_EntitiesInBox
import trap_UnlinkEntity
import trap_LinkEntity
import trap_AdjustAreaPortalState
import trap_InPVS
import trap_PointContents
import trap_Trace
import trap_SetBrushModel
import trap_GetServerinfo
import trap_SetUserinfo
import trap_GetUserinfo
import trap_GetConfigstring
import trap_SetConfigstring
import trap_SendServerCommand
import trap_DropClient
import trap_LocateGameData
import mod_gravity
import mod_jumpheight
import g_entities
import level
import G_StartNukeExplode
import G_StartCarExplode
import G_StartKamikaze
import FireWeapon
import fire_missile
import G_RunMissile
import ProximityMine_Trigger
import Weapon_Toolgun_Info
import Weapon_HookFree
import Melee_Fire
import G_ExplodeMissile
import CalcMuzzlePoint
import FindRandomSpawn
import FindRandomItem
import G_FindWeldEntity
import G_PlayerIsOwner
import G_FindEntityForEntityNum
import CrosshairPointGravity
import CrosshairPointPhys
import FindEntityForGravitygun
import FindEntityForPhysgun
import G_SetOrigin
import G_Sound
import G_AddEvent
import G_AddPredictableEvent
import G_KillBox
import G_TempEntity
import G_FreeEntity
import G_Spawn
import G_InitGentity
import G_SetMovedir
import vtos
import G_UseTargets
import G_PickAllTargets
import G_PickTarget
import G_Find
import G_TeamCommand
import G_SoundIndex
import G_ModelIndex
import SP_trigger_hurt
import SP_trigger_teleport
import SP_target_push
import SP_trigger_push
import SP_trigger_always
import SP_trigger_multiple
import CheckObeliskAttack
import SP_team_neutralobelisk
import SP_team_blueobelisk
import SP_team_redobelisk
import SelectCTFSpawnPoint
import Team_GetLocationMsg
import Pickup_Team
import Team_DroppedFlagThink
import Team_FreeEntity
import Team_ReturnFlag
import Team_CheckDroppedItem
import OnSameTeam
import Team_InitGame
import SP_script_cmd
import SP_script_variable
import SP_target_location
import SP_target_position
import SP_target_kill
import SP_target_relay
import SP_target_teleporter
import SP_target_speaker
import SP_target_print
import SP_target_delay
import SP_target_give
import ConsoleCommand
import G_ClearMap_f
import G_DeleteMapfile_f
import G_WriteMapfile_f
import G_LoadMapfile_f
import G_SpawnEntitiesFromString
import G_SpawnVector
import G_SpawnInt
import G_SpawnFloat
import G_SpawnString
import gameInfoEntities
import G_WriteSessionData
import G_InitWorldSession
import G_InitSessionData
import G_ReadSessionData
import Undo_RemoveElement
import Undo_LastElement
import Undo_AddElement
import G_ModProp
import G_BuildProp
import SP_sandbox_prop
import SP_sandbox_npc
import G_DieProp
import Phys_Frame
import Phys_Unweld
import Phys_Enable
import Phys_Disable
import Phys_HoldFrame
import Phys_HoldSetup
import Phys_HoldDropDynamic
import Phys_HoldDropStatic
import Phys_CheckCarCollisions
import Phys_VehiclePlayer
import SP_func_pendulum
import SP_func_bobbing
import SP_func_rotating
import SP_func_static
import SP_func_train
import SP_path_corner
import SP_func_button
import SP_func_plat
import SP_func_door
import Touch_DoorTrigger
import G_RunMover
import DropPortalSource
import DropPortalDestination
import SP_shooter_grenade
import SP_shooter_plasma
import SP_shooter_rocket
import SP_misc_portal_camera
import SP_misc_portal_surface
import TeleportPlayer
import G_RunThink
import SetLeader
import FindIntermissionPoint
import MoveClientToIntermission
import CalculateRanks
import G_Error
import G_Printf
import G_RunItem
import G_SpawnItem
import G_CheckTeamItems
import FinishSpawningItem
import Drop_Item
import LaunchItem
import Touch_Item
import RespawnItem
import Set_Ammo
import Set_Weapon
import G_RadiusDamage
import CanDamage
import G_Damage
import G_InvulnerabilityEffect
import player_die
import body_die
import TossClientPersistantPowerups
import TossClientCubes
import TossClientItems
import AddScore
import ClientCommand
import Cmd_FollowCycle_f
import StopFollowing
import SetTeam
import ConcatArgs
import Cmd_Score_f
import ObeliskHealthMessage
import RespawnTimeMessage
import G_SendSpawnSwepWeapons
import G_SendSwepWeapons
import G_SendGameCvars
import DeathmatchScoreboardMessage
import SetUnlimitedWeapons
import DropClientSilently
import ClientDisconnect
import ClientSpawn
import ClientBegin
import ClientConnect
import ClientUserinfoChanged
import PickTeam
import ClientRespawn
import SetClientViewAngle
import CopyToBodyQue
import InitBodyQue
import SelectSpawnPoint
import SpotWouldTelefrag
import SP_info_player_start
import SP_info_player_deathmatch
import ClientEndFrame
import PM_Add_SwepAmmo
import G_CheckWeaponAmmo
import G_CheckWeapon
import G_RunClient
import ClientThink
import AI_Frame
import BotAIShutdownClient
import BotAISetupClient
import BotAILoadMap
import BotAIShutdown
import BotAISetup
import SourceTechEntityList
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
LABELV $117
byte 1 115
byte 1 97
byte 1 110
byte 1 100
byte 1 98
byte 1 111
byte 1 120
byte 1 95
byte 1 110
byte 1 112
byte 1 99
byte 1 0
align 1
LABELV $107
byte 1 85
byte 1 115
byte 1 97
byte 1 103
byte 1 101
byte 1 58
byte 1 32
byte 1 97
byte 1 100
byte 1 100
byte 1 98
byte 1 111
byte 1 116
byte 1 32
byte 1 60
byte 1 109
byte 1 111
byte 1 100
byte 1 101
byte 1 108
byte 1 47
byte 1 115
byte 1 107
byte 1 105
byte 1 110
byte 1 62
byte 1 32
byte 1 91
byte 1 110
byte 1 97
byte 1 109
byte 1 101
byte 1 93
byte 1 32
byte 1 91
byte 1 116
byte 1 101
byte 1 97
byte 1 109
byte 1 93
byte 1 10
byte 1 0
align 1
LABELV $101
byte 1 0
align 1
LABELV $99
byte 1 110
byte 1 112
byte 1 99
byte 1 84
byte 1 121
byte 1 112
byte 1 101
byte 1 0
align 1
LABELV $98
byte 1 37
byte 1 105
byte 1 0
align 1
LABELV $97
byte 1 115
byte 1 112
byte 1 97
byte 1 119
byte 1 110
byte 1 105
byte 1 100
byte 1 0
align 1
LABELV $94
byte 1 98
byte 1 108
byte 1 117
byte 1 101
byte 1 0
align 1
LABELV $93
byte 1 114
byte 1 101
byte 1 100
byte 1 0
align 1
LABELV $90
byte 1 103
byte 1 95
byte 1 103
byte 1 97
byte 1 109
byte 1 101
byte 1 116
byte 1 121
byte 1 112
byte 1 101
byte 1 0
align 1
LABELV $84
byte 1 94
byte 1 51
byte 1 83
byte 1 101
byte 1 114
byte 1 118
byte 1 101
byte 1 114
byte 1 32
byte 1 105
byte 1 115
byte 1 32
byte 1 102
byte 1 117
byte 1 108
byte 1 108
byte 1 44
byte 1 32
byte 1 105
byte 1 110
byte 1 99
byte 1 114
byte 1 101
byte 1 97
byte 1 115
byte 1 101
byte 1 32
byte 1 101
byte 1 110
byte 1 103
byte 1 105
byte 1 110
byte 1 101
byte 1 32
byte 1 108
byte 1 105
byte 1 109
byte 1 105
byte 1 116
byte 1 46
byte 1 10
byte 1 0
align 1
LABELV $81
byte 1 94
byte 1 51
byte 1 83
byte 1 101
byte 1 114
byte 1 118
byte 1 101
byte 1 114
byte 1 32
byte 1 105
byte 1 115
byte 1 32
byte 1 102
byte 1 117
byte 1 108
byte 1 108
byte 1 44
byte 1 32
byte 1 105
byte 1 110
byte 1 99
byte 1 114
byte 1 101
byte 1 97
byte 1 115
byte 1 101
byte 1 32
byte 1 103
byte 1 95
byte 1 109
byte 1 97
byte 1 120
byte 1 67
byte 1 108
byte 1 105
byte 1 101
byte 1 110
byte 1 116
byte 1 115
byte 1 46
byte 1 10
byte 1 0
align 1
LABELV $80
byte 1 103
byte 1 95
byte 1 109
byte 1 97
byte 1 120
byte 1 67
byte 1 108
byte 1 105
byte 1 101
byte 1 110
byte 1 116
byte 1 115
byte 1 0
align 1
LABELV $76
byte 1 109
byte 1 111
byte 1 100
byte 1 101
byte 1 108
byte 1 0
align 1
LABELV $75
byte 1 115
byte 1 97
byte 1 114
byte 1 103
byte 1 101
byte 1 47
byte 1 100
byte 1 101
byte 1 102
byte 1 97
byte 1 117
byte 1 108
byte 1 116
byte 1 0
align 1
LABELV $72
byte 1 50
byte 1 48
byte 1 0
align 1
LABELV $71
byte 1 115
byte 1 110
byte 1 97
byte 1 112
byte 1 115
byte 1 0
align 1
LABELV $70
byte 1 50
byte 1 53
byte 1 48
byte 1 48
byte 1 48
byte 1 0
align 1
LABELV $69
byte 1 114
byte 1 97
byte 1 116
byte 1 101
byte 1 0
align 1
LABELV $68
byte 1 110
byte 1 97
byte 1 109
byte 1 101
byte 1 0
align 1
LABELV $58
byte 1 66
byte 1 111
byte 1 116
byte 1 65
byte 1 73
byte 1 83
byte 1 101
byte 1 116
byte 1 117
byte 1 112
byte 1 67
byte 1 108
byte 1 105
byte 1 101
byte 1 110
byte 1 116
byte 1 32
byte 1 102
byte 1 97
byte 1 105
byte 1 108
byte 1 101
byte 1 100
byte 1 0
align 1
LABELV $55
byte 1 116
byte 1 101
byte 1 97
byte 1 109
byte 1 0
