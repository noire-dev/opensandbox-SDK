code

equ	trap_Printf				            -1
equ	trap_Error				            -2
equ	trap_Milliseconds		            -3
equ	trap_Cvar_Register		            -4
equ	trap_Cvar_Update		            -5
equ	trap_Cvar_Set			            -6
equ	trap_Cvar_VariableIntegerValue	    -7
equ	trap_Cvar_VariableStringBuffer	    -8
equ	trap_Argc				            -9
equ	trap_Argv				            -10
equ	trap_FS_FOpenFile		            -11
equ	trap_FS_Read			            -12
equ	trap_FS_Write			            -13
equ	trap_FS_FCloseFile		            -14
equ	trap_SendConsoleCommand	            -15
equ	trap_LocateGameData		            -16
equ	trap_DropClient			            -17
equ	trap_SendServerCommand	            -18
equ	trap_SetConfigstring	            -19
equ	trap_GetConfigstring	            -20
equ	trap_GetUserinfo		            -21
equ	trap_SetUserinfo		            -22
equ	trap_GetServerinfo		            -23
equ	trap_SetBrushModel		            -24
equ	trap_Trace				            -25
equ	trap_PointContents		            -26
equ trap_InPVS				            -27
equ	trap_AdjustAreaPortalState	        -28
equ	trap_LinkEntity			            -29
equ	trap_UnlinkEntity		            -30
equ	trap_EntitiesInBox		            -31
equ	trap_EntityContact		            -32
equ	trap_BotAllocateClient	            -33
equ	trap_GetUsercmd			            -34
equ	trap_GetEntityToken		            -35
equ	trap_FS_GetFileList		            -36
equ trap_RealTime			            -37
equ trap_System				            -38

equ	memset					            -1001
equ	memcpy					            -1002
equ	strncpy					            -1003
equ	sin						            -1004
equ	cos						            -1005
equ	atan2					            -1006
equ	sqrt					            -1007

equ trap_BotLibSetup					-2001
equ trap_BotLibShutdown					-2002
equ trap_BotLibVarSet					-2003
equ trap_BotLibDefine					-2004
equ trap_BotLibStartFrame				-2005
equ trap_BotLibLoadMap					-2006
equ trap_BotLibUpdateEntity				-2007
equ trap_BotGetSnapshotEntity			-2008
equ trap_BotGetServerCommand		    -2009
equ trap_BotUserCommand					-2010

equ trap_AAS_EnableRoutingArea		    -3001
equ trap_AAS_BBoxAreas				    -3002
equ trap_AAS_AreaInfo				    -3003
equ trap_AAS_EntityInfo					-3004
equ trap_AAS_Initialized				-3005
equ trap_AAS_PresenceTypeBoundingBox	-3006
equ trap_AAS_Time						-3007
equ trap_AAS_PointAreaNum				-3008
equ trap_AAS_TraceAreas					-3009
equ trap_AAS_PointContents				-3010
equ trap_AAS_NextBSPEntity				-3011
equ trap_AAS_ValueForBSPEpairKey		-3012
equ trap_AAS_VectorForBSPEpairKey		-3013
equ trap_AAS_FloatForBSPEpairKey		-3014
equ trap_AAS_IntForBSPEpairKey			-3015
equ trap_AAS_AreaReachability			-3016
equ trap_AAS_AreaTravelTimeToGoalArea	-3017
equ trap_AAS_Swimming					-3018
equ trap_AAS_PredictClientMovement		-3019

equ trap_EA_Say							-4001
equ trap_EA_SayTeam						-4002
equ trap_EA_Command						-4003
equ trap_EA_Action						-4004
equ trap_EA_Gesture						-4005
equ trap_EA_Talk						-4006
equ trap_EA_Attack						-4007
equ trap_EA_Use							-4008
equ trap_EA_Respawn						-4009
equ trap_EA_Crouch						-4010
equ trap_EA_SelectWeapon				-4011
equ trap_EA_View						-4012
equ trap_EA_GetInput					-4013
equ trap_EA_ResetInput					-4014

equ trap_BotLoadCharacter				-5001
equ trap_BotFreeCharacter				-5002
equ trap_Characteristic_BFloat			-5003
equ trap_Characteristic_BInteger		-5004
equ trap_Characteristic_String			-5005
equ trap_BotAllocChatState				-5006
equ trap_BotFreeChatState				-5007
equ trap_BotQueueConsoleMessage			-5008
equ trap_BotRemoveConsoleMessage		-5009
equ trap_BotNextConsoleMessage			-5010
equ trap_BotNumConsoleMessages			-5011
equ trap_BotInitialChat					-5012
equ trap_BotReplyChat					-5013
equ trap_BotEnterChat					-5014
equ trap_BotFindMatch					-5015
equ trap_BotMatchVariable				-5016
equ trap_UnifyWhiteSpaces				-5017
equ trap_BotReplaceSynonyms				-5018
equ trap_BotLoadChatFile				-5019
equ trap_BotSetChatGender				-5020
equ trap_BotSetChatName					-5021
equ trap_BotResetGoalState				-5022
equ trap_BotResetAvoidGoals				-5023
equ trap_BotPushGoal					-5024
equ trap_BotPopGoal						-5025
equ trap_BotEmptyGoalStack				-5026
equ trap_BotDumpAvoidGoals				-5027
equ trap_BotDumpGoalStack				-5028
equ trap_BotGoalName					-5029
equ trap_BotGetTopGoal					-5030
equ trap_BotGetSecondGoal				-5031
equ trap_BotChooseLTGItem				-5032
equ trap_BotChooseNBGItem				-5033
equ trap_BotTouchingGoal				-5034
equ trap_BotItemGoalInVisButNotVisible	-5035
equ trap_BotGetLevelItemGoal			-5036
equ trap_BotAvoidGoalTime				-5037
equ trap_BotUpdateEntityItems			-5038
equ trap_BotLoadItemWeights				-5039
equ trap_BotAllocGoalState				-5040
equ trap_BotFreeGoalState				-5041
equ trap_BotResetMoveState				-5042
equ trap_BotMoveToGoal					-5043
equ trap_BotMoveInDirection				-5044
equ trap_BotResetAvoidReach				-5045
equ trap_BotResetLastAvoidReach			-5046
equ trap_BotMovementViewTarget			-5047
equ trap_BotAllocMoveState				-5048
equ trap_BotFreeMoveState				-5049
equ trap_BotInitMoveState				-5050
equ trap_BotLoadWeaponWeights			-5051
equ trap_BotAllocWeaponState			-5052
equ trap_BotFreeWeaponState				-5053
equ trap_BotResetWeaponState			-5054
equ trap_BotGetNextCampSpotGoal			-5055
equ trap_BotNumInitialChats				-5056
equ trap_BotGetChatMessage				-5057
equ trap_BotRemoveFromAvoidGoals		-5058
equ trap_BotPredictVisiblePosition		-5059
equ trap_BotSetAvoidGoalTime			-5060
equ trap_BotAddAvoidSpot				-5061
equ trap_AAS_AlternativeRouteGoals		-5062
equ trap_AAS_PredictRoute				-5063
equ trap_AAS_PointReachabilityAreaIndex	-5064
 