code
proc Use_Target_Give 68 12
ADDRFP4 0
ADDRFP4 0
INDIRP4
ASGNP4
ADDRFP4 8
ADDRFP4 8
INDIRP4
ASGNP4
ADDRLP4 60
CNSTU4 0
ASGNU4
ADDRFP4 8
INDIRP4
CNSTI4 548
ADDP4
INDIRP4
CVPU4 4
ADDRLP4 60
INDIRU4
EQU4 $57
ADDRFP4 0
INDIRP4
CNSTI4 676
ADDP4
INDIRP4
CVPU4 4
ADDRLP4 60
INDIRU4
NEU4 $55
LABELV $57
ADDRGP4 $54
JUMPV
LABELV $55
ADDRLP4 4
ARGP4
CNSTI4 0
ARGI4
CNSTI4 56
ARGI4
ADDRGP4 memset
CALLP4
pop
ADDRLP4 0
CNSTP4 0
ASGNP4
ADDRGP4 $59
JUMPV
LABELV $58
ADDRLP4 0
INDIRP4
CNSTI4 828
ADDP4
INDIRP4
CVPU4 4
CNSTU4 0
NEU4 $61
ADDRGP4 $59
JUMPV
LABELV $61
ADDRLP4 0
INDIRP4
ARGP4
ADDRFP4 8
INDIRP4
ARGP4
ADDRLP4 4
ARGP4
ADDRGP4 Touch_Item
CALLV
pop
ADDRLP4 0
INDIRP4
CNSTI4 708
ADDP4
CNSTI4 0
ASGNI4
ADDRLP4 0
INDIRP4
ARGP4
ADDRGP4 trap_UnlinkEntity
CALLV
pop
LABELV $59
ADDRLP4 0
INDIRP4
ARGP4
CNSTI4 680
ARGI4
ADDRFP4 0
INDIRP4
CNSTI4 676
ADDP4
INDIRP4
ARGP4
ADDRLP4 64
ADDRGP4 G_Find
CALLP4
ASGNP4
ADDRLP4 0
ADDRLP4 64
INDIRP4
ASGNP4
ADDRLP4 64
INDIRP4
CVPU4 4
CNSTU4 0
NEU4 $58
LABELV $54
endproc Use_Target_Give 68 12
export SP_target_give
proc SP_target_give 0 0
ADDRFP4 0
INDIRP4
CNSTI4 728
ADDP4
ADDRGP4 Use_Target_Give
ASGNP4
LABELV $63
endproc SP_target_give 0 0
proc Think_Target_Delay 4 8
ADDRLP4 0
ADDRFP4 0
INDIRP4
ASGNP4
ADDRLP4 0
INDIRP4
ARGP4
ADDRLP4 0
INDIRP4
CNSTI4 788
ADDP4
INDIRP4
ARGP4
ADDRGP4 G_UseTargets
CALLV
pop
LABELV $64
endproc Think_Target_Delay 4 8
proc Use_Target_Delay 8 0
ADDRFP4 0
ADDRFP4 0
INDIRP4
ASGNP4
ADDRLP4 0
ADDRGP4 rand
CALLI4
ASGNI4
ADDRFP4 0
INDIRP4
CNSTI4 708
ADDP4
ADDRGP4 level+24
INDIRI4
CVIF4 4
CNSTF4 1148846080
ADDRFP4 0
INDIRP4
CNSTI4 820
ADDP4
INDIRF4
ADDRFP4 0
INDIRP4
CNSTI4 824
ADDP4
INDIRF4
CNSTF4 1073741824
ADDRLP4 0
INDIRI4
CNSTI4 32767
BANDI4
CVIF4 4
CNSTF4 1191181824
DIVF4
CNSTF4 1056964608
SUBF4
MULF4
MULF4
ADDF4
MULF4
ADDF4
CVFI4 4
ASGNI4
ADDRFP4 0
INDIRP4
CNSTI4 712
ADDP4
ADDRGP4 Think_Target_Delay
ASGNP4
ADDRFP4 0
INDIRP4
CNSTI4 788
ADDP4
ADDRFP4 8
INDIRP4
ASGNP4
LABELV $65
endproc Use_Target_Delay 8 0
export SP_target_delay
proc SP_target_delay 4 12
ADDRFP4 0
ADDRFP4 0
INDIRP4
ASGNP4
ADDRGP4 $70
ARGP4
ADDRGP4 $71
ARGP4
ADDRFP4 0
INDIRP4
CNSTI4 820
ADDP4
ARGP4
ADDRLP4 0
ADDRGP4 G_SpawnFloat
CALLI4
ASGNI4
ADDRLP4 0
INDIRI4
CNSTI4 0
NEI4 $68
ADDRGP4 $72
ARGP4
ADDRGP4 $73
ARGP4
ADDRFP4 0
INDIRP4
CNSTI4 820
ADDP4
ARGP4
ADDRGP4 G_SpawnFloat
CALLI4
pop
LABELV $68
ADDRFP4 0
INDIRP4
CNSTI4 820
ADDP4
INDIRF4
CNSTF4 0
NEF4 $74
ADDRFP4 0
INDIRP4
CNSTI4 820
ADDP4
CNSTF4 1065353216
ASGNF4
LABELV $74
ADDRFP4 0
INDIRP4
CNSTI4 728
ADDP4
ADDRGP4 Use_Target_Delay
ASGNP4
LABELV $67
endproc SP_target_delay 4 12
proc Use_Target_Print 4 8
ADDRFP4 0
ADDRFP4 0
INDIRP4
ASGNP4
ADDRFP4 8
INDIRP4
CNSTI4 548
ADDP4
INDIRP4
CVPU4 4
CNSTU4 0
EQU4 $77
ADDRFP4 0
INDIRP4
CNSTI4 560
ADDP4
INDIRI4
CNSTI4 4
BANDI4
CNSTI4 0
EQI4 $77
ADDRGP4 $79
ARGP4
ADDRFP4 0
INDIRP4
CNSTI4 664
ADDP4
INDIRP4
ARGP4
ADDRLP4 0
ADDRGP4 va
CALLP4
ASGNP4
ADDRFP4 8
INDIRP4
CVPU4 4
ADDRGP4 g_entities
CVPU4 4
SUBU4
CVUI4 4
CNSTI4 1288
DIVI4
ARGI4
ADDRLP4 0
INDIRP4
ARGP4
ADDRGP4 trap_SendServerCommand
CALLV
pop
ADDRGP4 $76
JUMPV
LABELV $77
ADDRFP4 0
INDIRP4
CNSTI4 560
ADDP4
INDIRI4
CNSTI4 3
BANDI4
CNSTI4 0
EQI4 $80
ADDRFP4 0
INDIRP4
CNSTI4 560
ADDP4
INDIRI4
CNSTI4 1
BANDI4
CNSTI4 0
EQI4 $82
ADDRGP4 $79
ARGP4
ADDRFP4 0
INDIRP4
CNSTI4 664
ADDP4
INDIRP4
ARGP4
ADDRLP4 0
ADDRGP4 va
CALLP4
ASGNP4
CNSTI4 1
ARGI4
ADDRLP4 0
INDIRP4
ARGP4
ADDRGP4 G_TeamCommand
CALLV
pop
LABELV $82
ADDRFP4 0
INDIRP4
CNSTI4 560
ADDP4
INDIRI4
CNSTI4 2
BANDI4
CNSTI4 0
EQI4 $76
ADDRGP4 $79
ARGP4
ADDRFP4 0
INDIRP4
CNSTI4 664
ADDP4
INDIRP4
ARGP4
ADDRLP4 0
ADDRGP4 va
CALLP4
ASGNP4
CNSTI4 2
ARGI4
ADDRLP4 0
INDIRP4
ARGP4
ADDRGP4 G_TeamCommand
CALLV
pop
ADDRGP4 $76
JUMPV
LABELV $80
ADDRGP4 $79
ARGP4
ADDRFP4 0
INDIRP4
CNSTI4 664
ADDP4
INDIRP4
ARGP4
ADDRLP4 0
ADDRGP4 va
CALLP4
ASGNP4
CNSTI4 -1
ARGI4
ADDRLP4 0
INDIRP4
ARGP4
ADDRGP4 trap_SendServerCommand
CALLV
pop
LABELV $76
endproc Use_Target_Print 4 8
export SP_target_print
proc SP_target_print 0 0
ADDRFP4 0
INDIRP4
CNSTI4 728
ADDP4
ADDRGP4 Use_Target_Print
ASGNP4
LABELV $86
endproc SP_target_print 0 0
proc Use_Target_Speaker 4 12
ADDRFP4 0
ADDRFP4 0
INDIRP4
ASGNP4
ADDRFP4 0
INDIRP4
CNSTI4 560
ADDP4
INDIRI4
CNSTI4 3
BANDI4
CNSTI4 0
EQI4 $88
ADDRFP4 0
INDIRP4
CNSTI4 168
ADDP4
INDIRI4
CNSTI4 0
EQI4 $90
ADDRFP4 0
INDIRP4
CNSTI4 168
ADDP4
CNSTI4 0
ASGNI4
ADDRGP4 $89
JUMPV
LABELV $90
ADDRFP4 0
INDIRP4
CNSTI4 168
ADDP4
ADDRFP4 0
INDIRP4
CNSTI4 816
ADDP4
INDIRI4
ASGNI4
ADDRGP4 $89
JUMPV
LABELV $88
ADDRFP4 0
INDIRP4
CNSTI4 560
ADDP4
INDIRI4
CNSTI4 8
BANDI4
CNSTI4 0
EQI4 $92
ADDRFP4 8
INDIRP4
ARGP4
CNSTI4 44
ARGI4
ADDRFP4 0
INDIRP4
CNSTI4 816
ADDP4
INDIRI4
ARGI4
ADDRGP4 G_AddEvent
CALLV
pop
ADDRGP4 $93
JUMPV
LABELV $92
ADDRFP4 0
INDIRP4
CNSTI4 560
ADDP4
INDIRI4
CNSTI4 4
BANDI4
CNSTI4 0
EQI4 $94
ADDRFP4 0
INDIRP4
ARGP4
CNSTI4 45
ARGI4
ADDRFP4 0
INDIRP4
CNSTI4 816
ADDP4
INDIRI4
ARGI4
ADDRGP4 G_AddEvent
CALLV
pop
ADDRGP4 $95
JUMPV
LABELV $94
ADDRFP4 0
INDIRP4
ARGP4
CNSTI4 44
ARGI4
ADDRFP4 0
INDIRP4
CNSTI4 816
ADDP4
INDIRI4
ARGI4
ADDRGP4 G_AddEvent
CALLV
pop
LABELV $95
LABELV $93
LABELV $89
LABELV $87
endproc Use_Target_Speaker 4 12
export SP_target_speaker
proc SP_target_speaker 308 16
ADDRFP4 0
ADDRFP4 0
INDIRP4
ASGNP4
ADDRGP4 $72
ARGP4
ADDRGP4 $71
ARGP4
ADDRFP4 0
INDIRP4
CNSTI4 820
ADDP4
ARGP4
ADDRGP4 G_SpawnFloat
CALLI4
pop
ADDRGP4 $97
ARGP4
ADDRGP4 $71
ARGP4
ADDRFP4 0
INDIRP4
CNSTI4 824
ADDP4
ARGP4
ADDRGP4 G_SpawnFloat
CALLI4
pop
ADDRGP4 $100
ARGP4
ADDRGP4 $101
ARGP4
ADDRLP4 0
ARGP4
ADDRLP4 260
ADDRGP4 G_SpawnString
CALLI4
ASGNI4
ADDRLP4 260
INDIRI4
CNSTI4 0
NEI4 $98
ADDRFP4 0
INDIRP4
CNSTI4 92
ADDP4
ARGP4
ADDRLP4 264
ADDRGP4 vtos
CALLP4
ASGNP4
ADDRGP4 $102
ARGP4
ADDRLP4 264
INDIRP4
ARGP4
ADDRGP4 G_Error
CALLV
pop
LABELV $98
ADDRLP4 0
INDIRP4
INDIRI1
CVII4 1
CNSTI4 42
NEI4 $103
ADDRLP4 264
ADDRFP4 0
INDIRP4
CNSTI4 560
ADDP4
ASGNP4
ADDRLP4 264
INDIRP4
ADDRLP4 264
INDIRP4
INDIRI4
CNSTI4 8
BORI4
ASGNI4
LABELV $103
ADDRLP4 0
INDIRP4
ARGP4
ADDRGP4 $107
ARGP4
ADDRLP4 268
ADDRGP4 strstr
CALLP4
ASGNP4
ADDRLP4 268
INDIRP4
CVPU4 4
CNSTU4 0
NEU4 $105
ADDRLP4 4
ARGP4
CNSTI4 256
ARGI4
ADDRGP4 $108
ARGP4
ADDRLP4 0
INDIRP4
ARGP4
ADDRGP4 Com_sprintf
CALLV
pop
ADDRGP4 $106
JUMPV
LABELV $105
ADDRLP4 4
ARGP4
ADDRLP4 0
INDIRP4
ARGP4
CNSTI4 256
ARGI4
ADDRGP4 Q_strncpyz
CALLV
pop
LABELV $106
ADDRLP4 4
ARGP4
ADDRLP4 272
ADDRGP4 G_SoundIndex
CALLI4
ASGNI4
ADDRFP4 0
INDIRP4
CNSTI4 816
ADDP4
ADDRLP4 272
INDIRI4
ASGNI4
ADDRFP4 0
INDIRP4
CNSTI4 4
ADDP4
CNSTI4 6
ASGNI4
ADDRFP4 0
INDIRP4
CNSTI4 196
ADDP4
ADDRFP4 0
INDIRP4
CNSTI4 816
ADDP4
INDIRI4
ASGNI4
ADDRFP4 0
INDIRP4
CNSTI4 184
ADDP4
CNSTF4 1092616192
ADDRFP4 0
INDIRP4
CNSTI4 820
ADDP4
INDIRF4
MULF4
CVFI4 4
ASGNI4
ADDRFP4 0
INDIRP4
CNSTI4 180
ADDP4
CNSTF4 1092616192
ADDRFP4 0
INDIRP4
CNSTI4 824
ADDP4
INDIRF4
MULF4
CVFI4 4
ASGNI4
ADDRFP4 0
INDIRP4
CNSTI4 560
ADDP4
INDIRI4
CNSTI4 1
BANDI4
CNSTI4 0
EQI4 $109
ADDRFP4 0
INDIRP4
CNSTI4 168
ADDP4
ADDRFP4 0
INDIRP4
CNSTI4 816
ADDP4
INDIRI4
ASGNI4
LABELV $109
ADDRFP4 0
INDIRP4
CNSTI4 728
ADDP4
ADDRGP4 Use_Target_Speaker
ASGNP4
ADDRFP4 0
INDIRP4
CNSTI4 560
ADDP4
INDIRI4
CNSTI4 4
BANDI4
CNSTI4 0
EQI4 $111
ADDRLP4 292
ADDRFP4 0
INDIRP4
CNSTI4 456
ADDP4
ASGNP4
ADDRLP4 292
INDIRP4
ADDRLP4 292
INDIRP4
INDIRI4
CNSTI4 4
BORI4
ASGNI4
LABELV $111
ADDRFP4 0
INDIRP4
CNSTI4 24
ADDP4
ADDRFP4 0
INDIRP4
CNSTI4 92
ADDP4
INDIRF4
ASGNF4
ADDRFP4 0
INDIRP4
CNSTI4 28
ADDP4
ADDRFP4 0
INDIRP4
CNSTI4 96
ADDP4
INDIRF4
ASGNF4
ADDRFP4 0
INDIRP4
CNSTI4 32
ADDP4
ADDRFP4 0
INDIRP4
CNSTI4 100
ADDP4
INDIRF4
ASGNF4
ADDRFP4 0
INDIRP4
ARGP4
ADDRGP4 trap_LinkEntity
CALLV
pop
LABELV $96
endproc SP_target_speaker 308 16
proc target_teleporter_use 12 16
ADDRFP4 8
INDIRP4
CNSTI4 548
ADDP4
INDIRP4
CVPU4 4
CNSTU4 0
NEU4 $114
ADDRGP4 $113
JUMPV
LABELV $114
ADDRFP4 0
INDIRP4
CNSTI4 676
ADDP4
INDIRP4
ARGP4
ADDRLP4 4
ADDRGP4 G_PickTarget
CALLP4
ASGNP4
ADDRLP4 0
ADDRLP4 4
INDIRP4
ASGNP4
ADDRLP4 0
INDIRP4
CVPU4 4
CNSTU4 0
NEU4 $116
ADDRGP4 $118
ARGP4
ADDRGP4 G_Printf
CALLV
pop
ADDRGP4 $113
JUMPV
LABELV $116
ADDRFP4 8
INDIRP4
ARGP4
ADDRLP4 0
INDIRP4
CNSTI4 92
ADDP4
ARGP4
ADDRLP4 0
INDIRP4
CNSTI4 128
ADDP4
ARGP4
CNSTI4 0
ARGI4
ADDRGP4 TeleportPlayer
CALLV
pop
LABELV $113
endproc target_teleporter_use 12 16
export SP_target_teleporter
proc SP_target_teleporter 4 12
ADDRFP4 0
ADDRFP4 0
INDIRP4
ASGNP4
ADDRFP4 0
INDIRP4
CNSTI4 680
ADDP4
INDIRP4
CVPU4 4
CNSTU4 0
NEU4 $120
ADDRFP4 0
INDIRP4
CNSTI4 92
ADDP4
ARGP4
ADDRLP4 0
ADDRGP4 vtos
CALLP4
ASGNP4
ADDRGP4 $122
ARGP4
ADDRFP4 0
INDIRP4
CNSTI4 556
ADDP4
INDIRP4
ARGP4
ADDRLP4 0
INDIRP4
ARGP4
ADDRGP4 G_Printf
CALLV
pop
LABELV $120
ADDRFP4 0
INDIRP4
CNSTI4 728
ADDP4
ADDRGP4 target_teleporter_use
ASGNP4
LABELV $119
endproc SP_target_teleporter 4 12
proc target_relay_use 36 12
ADDRFP4 0
ADDRFP4 0
INDIRP4
ASGNP4
ADDRFP4 8
ADDRFP4 8
INDIRP4
ASGNP4
ADDRLP4 0
CNSTI4 1
ASGNI4
ADDRFP4 0
INDIRP4
CNSTI4 560
ADDP4
INDIRI4
ADDRLP4 0
INDIRI4
BANDI4
CNSTI4 0
EQI4 $124
ADDRLP4 4
ADDRFP4 8
INDIRP4
CNSTI4 548
ADDP4
INDIRP4
ASGNP4
ADDRLP4 4
INDIRP4
CVPU4 4
CNSTU4 0
EQU4 $124
ADDRLP4 4
INDIRP4
CNSTI4 1512
ADDP4
INDIRI4
ADDRLP4 0
INDIRI4
EQI4 $124
ADDRGP4 $123
JUMPV
LABELV $124
ADDRLP4 8
CNSTI4 2
ASGNI4
ADDRFP4 0
INDIRP4
CNSTI4 560
ADDP4
INDIRI4
ADDRLP4 8
INDIRI4
BANDI4
CNSTI4 0
EQI4 $126
ADDRLP4 12
ADDRFP4 8
INDIRP4
CNSTI4 548
ADDP4
INDIRP4
ASGNP4
ADDRLP4 12
INDIRP4
CVPU4 4
CNSTU4 0
EQU4 $126
ADDRLP4 12
INDIRP4
CNSTI4 1512
ADDP4
INDIRI4
ADDRLP4 8
INDIRI4
EQI4 $126
ADDRGP4 $123
JUMPV
LABELV $126
ADDRFP4 0
INDIRP4
CNSTI4 560
ADDP4
INDIRI4
CNSTI4 4
BANDI4
CNSTI4 0
EQI4 $128
ADDRFP4 0
INDIRP4
CNSTI4 676
ADDP4
INDIRP4
ARGP4
ADDRLP4 20
ADDRGP4 G_PickTarget
CALLP4
ASGNP4
ADDRLP4 16
ADDRLP4 20
INDIRP4
ASGNP4
ADDRLP4 24
ADDRLP4 16
INDIRP4
ASGNP4
ADDRLP4 28
CNSTU4 0
ASGNU4
ADDRLP4 24
INDIRP4
CVPU4 4
ADDRLP4 28
INDIRU4
EQU4 $123
ADDRLP4 24
INDIRP4
CNSTI4 728
ADDP4
INDIRP4
CVPU4 4
ADDRLP4 28
INDIRU4
EQU4 $123
ADDRLP4 32
ADDRLP4 16
INDIRP4
ASGNP4
ADDRLP4 32
INDIRP4
ARGP4
ADDRFP4 0
INDIRP4
ARGP4
ADDRFP4 8
INDIRP4
ARGP4
ADDRLP4 32
INDIRP4
CNSTI4 728
ADDP4
INDIRP4
CALLV
pop
ADDRGP4 $123
JUMPV
LABELV $128
ADDRFP4 0
INDIRP4
ARGP4
ADDRFP4 8
INDIRP4
ARGP4
ADDRGP4 G_UseTargets
CALLV
pop
LABELV $123
endproc target_relay_use 36 12
export SP_target_relay
proc SP_target_relay 0 0
ADDRFP4 0
INDIRP4
CNSTI4 728
ADDP4
ADDRGP4 target_relay_use
ASGNP4
LABELV $132
endproc SP_target_relay 0 0
proc target_kill_use 8 32
ADDRFP4 8
INDIRP4
ARGP4
ADDRLP4 0
CNSTP4 0
ASGNP4
ADDRLP4 0
INDIRP4
ARGP4
ADDRLP4 0
INDIRP4
ARGP4
ADDRLP4 4
CNSTP4 0
ASGNP4
ADDRLP4 4
INDIRP4
ARGP4
ADDRLP4 4
INDIRP4
ARGP4
CNSTI4 100000
ARGI4
CNSTI4 8
ARGI4
CNSTI4 32
ARGI4
ADDRGP4 G_Damage
CALLV
pop
LABELV $133
endproc target_kill_use 8 32
export SP_target_kill
proc SP_target_kill 0 0
ADDRFP4 0
INDIRP4
CNSTI4 728
ADDP4
ADDRGP4 target_kill_use
ASGNP4
LABELV $134
endproc SP_target_kill 0 0
export SP_target_position
proc SP_target_position 4 8
ADDRLP4 0
ADDRFP4 0
INDIRP4
ASGNP4
ADDRLP4 0
INDIRP4
ARGP4
ADDRLP4 0
INDIRP4
CNSTI4 92
ADDP4
ARGP4
ADDRGP4 G_SetOrigin
CALLV
pop
LABELV $135
endproc SP_target_position 4 8
proc target_location_linkup 16 8
ADDRFP4 0
ADDRFP4 0
INDIRP4
ASGNP4
ADDRGP4 level+5260
INDIRI4
CNSTI4 0
EQI4 $137
ADDRGP4 $136
JUMPV
LABELV $137
ADDRGP4 level+5260
CNSTI4 1
ASGNI4
ADDRGP4 level+5264
CNSTP4 0
ASGNP4
CNSTI4 1536
ARGI4
ADDRGP4 $142
ARGP4
ADDRGP4 trap_SetConfigstring
CALLV
pop
ADDRLP4 0
CNSTI4 0
ASGNI4
ADDRFP4 0
ADDRGP4 g_entities
ASGNP4
ADDRLP4 4
CNSTI4 1
ASGNI4
ADDRGP4 $146
JUMPV
LABELV $143
ADDRLP4 8
ADDRFP4 0
INDIRP4
CNSTI4 556
ADDP4
INDIRP4
ASGNP4
ADDRLP4 8
INDIRP4
CVPU4 4
CNSTU4 0
EQU4 $148
ADDRLP4 8
INDIRP4
ARGP4
ADDRGP4 $150
ARGP4
ADDRLP4 12
ADDRGP4 Q_stricmp
CALLI4
ASGNI4
ADDRLP4 12
INDIRI4
CNSTI4 0
NEI4 $148
ADDRLP4 4
INDIRI4
CNSTI4 1536
ADDI4
CNSTI4 1600
LTI4 $151
ADDRGP4 $136
JUMPV
LABELV $151
ADDRFP4 0
INDIRP4
CNSTI4 752
ADDP4
ADDRLP4 4
INDIRI4
ASGNI4
ADDRLP4 4
INDIRI4
CNSTI4 1536
ADDI4
ARGI4
ADDRFP4 0
INDIRP4
CNSTI4 664
ADDP4
INDIRP4
ARGP4
ADDRGP4 trap_SetConfigstring
CALLV
pop
ADDRLP4 4
ADDRLP4 4
INDIRI4
CNSTI4 1
ADDI4
ASGNI4
ADDRFP4 0
INDIRP4
CNSTI4 632
ADDP4
ADDRGP4 level+5264
INDIRP4
ASGNP4
ADDRGP4 level+5264
ADDRFP4 0
INDIRP4
ASGNP4
LABELV $148
LABELV $144
ADDRLP4 0
ADDRLP4 0
INDIRI4
CNSTI4 1
ADDI4
ASGNI4
ADDRFP4 0
ADDRFP4 0
INDIRP4
CNSTI4 1288
ADDP4
ASGNP4
LABELV $146
ADDRLP4 0
INDIRI4
ADDRGP4 level+12
INDIRI4
LTI4 $143
LABELV $136
endproc target_location_linkup 16 8
export SP_target_location
proc SP_target_location 4 8
ADDRFP4 0
ADDRFP4 0
INDIRP4
ASGNP4
ADDRFP4 0
INDIRP4
CNSTI4 712
ADDP4
ADDRGP4 target_location_linkup
ASGNP4
ADDRFP4 0
INDIRP4
CNSTI4 708
ADDP4
ADDRGP4 level+24
INDIRI4
CNSTI4 200
ADDI4
ASGNI4
ADDRFP4 0
INDIRP4
ARGP4
ADDRFP4 0
INDIRP4
CNSTI4 92
ADDP4
ARGP4
ADDRGP4 G_SetOrigin
CALLV
pop
LABELV $155
endproc SP_target_location 4 8
proc script_variable_use 48 12
ADDRFP4 0
ADDRFP4 0
INDIRP4
ASGNP4
ADDRLP4 4
ADDRFP4 0
INDIRP4
CNSTI4 560
ADDP4
INDIRI4
ASGNI4
ADDRLP4 8
CNSTI4 0
ASGNI4
ADDRLP4 4
INDIRI4
CNSTI4 1
BANDI4
ADDRLP4 8
INDIRI4
NEI4 $160
ADDRLP4 4
INDIRI4
CNSTI4 2
BANDI4
ADDRLP4 8
INDIRI4
EQI4 $158
LABELV $160
ADDRFP4 0
INDIRP4
CNSTI4 924
ADDP4
INDIRP4
ARGP4
ADDRLP4 12
ADDRGP4 cvarString
CALLP4
ASGNP4
ADDRLP4 0
ADDRLP4 12
INDIRP4
ASGNP4
ADDRFP4 0
INDIRP4
CNSTI4 560
ADDP4
INDIRI4
CNSTI4 1
BANDI4
CNSTI4 0
EQI4 $161
ADDRLP4 0
INDIRP4
ARGP4
ADDRFP4 0
INDIRP4
CNSTI4 928
ADDP4
INDIRP4
ARGP4
ADDRLP4 20
ADDRGP4 strcmp
CALLI4
ASGNI4
ADDRLP4 20
INDIRI4
CNSTI4 0
NEI4 $161
ADDRFP4 0
INDIRP4
ARGP4
ADDRFP4 8
INDIRP4
ARGP4
ADDRGP4 G_UseTargets
CALLV
pop
LABELV $161
ADDRFP4 0
INDIRP4
CNSTI4 560
ADDP4
INDIRI4
CNSTI4 2
BANDI4
CNSTI4 0
EQI4 $163
ADDRLP4 0
INDIRP4
ARGP4
ADDRFP4 0
INDIRP4
CNSTI4 928
ADDP4
INDIRP4
ARGP4
ADDRLP4 28
ADDRGP4 strcmp
CALLI4
ASGNI4
ADDRLP4 28
INDIRI4
CNSTI4 0
EQI4 $163
ADDRFP4 0
INDIRP4
ARGP4
ADDRFP4 8
INDIRP4
ARGP4
ADDRGP4 G_UseTargets
CALLV
pop
LABELV $163
ADDRFP4 0
INDIRP4
CNSTI4 560
ADDP4
INDIRI4
CNSTI4 4
BANDI4
CNSTI4 0
EQI4 $165
ADDRLP4 0
INDIRP4
ARGP4
ADDRLP4 32
ADDRGP4 atoi
CALLI4
ASGNI4
ADDRFP4 0
INDIRP4
CNSTI4 928
ADDP4
INDIRP4
ARGP4
ADDRLP4 36
ADDRGP4 atoi
CALLI4
ASGNI4
ADDRLP4 32
INDIRI4
ADDRLP4 36
INDIRI4
GTI4 $165
ADDRFP4 0
INDIRP4
ARGP4
ADDRFP4 8
INDIRP4
ARGP4
ADDRGP4 G_UseTargets
CALLV
pop
LABELV $165
ADDRFP4 0
INDIRP4
CNSTI4 560
ADDP4
INDIRI4
CNSTI4 8
BANDI4
CNSTI4 0
EQI4 $157
ADDRLP4 0
INDIRP4
ARGP4
ADDRLP4 40
ADDRGP4 atoi
CALLI4
ASGNI4
ADDRFP4 0
INDIRP4
CNSTI4 928
ADDP4
INDIRP4
ARGP4
ADDRLP4 44
ADDRGP4 atoi
CALLI4
ASGNI4
ADDRLP4 40
INDIRI4
ADDRLP4 44
INDIRI4
LTI4 $157
ADDRFP4 0
INDIRP4
ARGP4
ADDRFP4 8
INDIRP4
ARGP4
ADDRGP4 G_UseTargets
CALLV
pop
ADDRGP4 $157
JUMPV
LABELV $158
ADDRFP4 0
INDIRP4
CNSTI4 560
ADDP4
INDIRI4
CNSTI4 8192
BANDI4
CNSTI4 0
EQI4 $169
ADDRGP4 $171
ARGP4
ADDRFP4 0
INDIRP4
CNSTI4 924
ADDP4
INDIRP4
ARGP4
ADDRFP4 0
INDIRP4
CNSTI4 928
ADDP4
INDIRP4
ARGP4
ADDRLP4 16
ADDRGP4 va
CALLP4
ASGNP4
CNSTI4 2
ARGI4
ADDRLP4 16
INDIRP4
ARGP4
ADDRGP4 trap_Cmd
CALLV
pop
ADDRGP4 $170
JUMPV
LABELV $169
ADDRFP4 0
INDIRP4
CNSTI4 924
ADDP4
INDIRP4
ARGP4
ADDRFP4 0
INDIRP4
CNSTI4 928
ADDP4
INDIRP4
ARGP4
ADDRGP4 cvarSet
CALLV
pop
LABELV $170
LABELV $157
endproc script_variable_use 48 12
proc script_variable_think 4 12
ADDRFP4 0
ADDRFP4 0
INDIRP4
ASGNP4
ADDRFP4 0
INDIRP4
CNSTI4 708
ADDP4
CNSTI4 0
ASGNI4
ADDRFP4 0
INDIRP4
ARGP4
CNSTP4 0
ARGP4
ADDRFP4 0
INDIRP4
ARGP4
ADDRGP4 script_variable_use
CALLV
pop
LABELV $172
endproc script_variable_think 4 12
export SP_script_variable
proc SP_script_variable 4 8
ADDRFP4 0
ADDRFP4 0
INDIRP4
ASGNP4
ADDRFP4 0
INDIRP4
CNSTI4 924
ADDP4
INDIRP4
CVPU4 4
CNSTU4 0
NEU4 $174
ADDRFP4 0
INDIRP4
CNSTI4 92
ADDP4
ARGP4
ADDRLP4 0
ADDRGP4 vtos
CALLP4
ASGNP4
ADDRGP4 $176
ARGP4
ADDRLP4 0
INDIRP4
ARGP4
ADDRGP4 G_Printf
CALLV
pop
ADDRFP4 0
INDIRP4
ARGP4
ADDRGP4 G_FreeEntity
CALLV
pop
ADDRGP4 $173
JUMPV
LABELV $174
ADDRFP4 0
INDIRP4
CNSTI4 924
ADDP4
INDIRP4
CVPU4 4
CNSTU4 0
NEU4 $177
ADDRFP4 0
INDIRP4
CNSTI4 92
ADDP4
ARGP4
ADDRLP4 0
ADDRGP4 vtos
CALLP4
ASGNP4
ADDRGP4 $179
ARGP4
ADDRLP4 0
INDIRP4
ARGP4
ADDRGP4 G_Printf
CALLV
pop
ADDRFP4 0
INDIRP4
ARGP4
ADDRGP4 G_FreeEntity
CALLV
pop
ADDRGP4 $173
JUMPV
LABELV $177
ADDRFP4 0
INDIRP4
CNSTI4 728
ADDP4
ADDRGP4 script_variable_use
ASGNP4
ADDRFP4 0
INDIRP4
CNSTI4 560
ADDP4
INDIRI4
CNSTI4 4096
BANDI4
CNSTI4 0
EQI4 $180
ADDRFP4 0
INDIRP4
CNSTI4 708
ADDP4
ADDRGP4 level+24
INDIRI4
CNSTI4 300
ADDI4
ASGNI4
ADDRFP4 0
INDIRP4
CNSTI4 712
ADDP4
ADDRGP4 script_variable_think
ASGNP4
LABELV $180
LABELV $173
endproc SP_script_variable 4 8
proc use_script_cmd 4 8
ADDRFP4 0
ADDRFP4 0
INDIRP4
ASGNP4
ADDRFP4 0
INDIRP4
CNSTI4 560
ADDP4
INDIRI4
CNSTI4 1
BANDI4
CNSTI4 0
EQI4 $184
ADDRGP4 $186
ARGP4
ADDRFP4 0
INDIRP4
CNSTI4 676
ADDP4
INDIRP4
ARGP4
ADDRLP4 0
ADDRGP4 va
CALLP4
ASGNP4
CNSTI4 2
ARGI4
ADDRLP4 0
INDIRP4
ARGP4
ADDRGP4 trap_Cmd
CALLV
pop
LABELV $184
ADDRFP4 0
INDIRP4
CNSTI4 560
ADDP4
INDIRI4
CNSTI4 2
BANDI4
CNSTI4 0
EQI4 $187
ADDRGP4 $189
ARGP4
ADDRFP4 0
INDIRP4
CNSTI4 676
ADDP4
INDIRP4
ARGP4
ADDRLP4 0
ADDRGP4 va
CALLP4
ASGNP4
ADDRFP4 8
INDIRP4
CVPU4 4
ADDRGP4 g_entities
CVPU4 4
SUBU4
CVUI4 4
CNSTI4 1288
DIVI4
ARGI4
ADDRLP4 0
INDIRP4
ARGP4
ADDRGP4 trap_SendServerCommand
CALLV
pop
LABELV $187
ADDRFP4 0
INDIRP4
CNSTI4 560
ADDP4
INDIRI4
CNSTI4 4
BANDI4
CNSTI4 0
EQI4 $190
ADDRGP4 $189
ARGP4
ADDRFP4 0
INDIRP4
CNSTI4 676
ADDP4
INDIRP4
ARGP4
ADDRLP4 0
ADDRGP4 va
CALLP4
ASGNP4
CNSTI4 -1
ARGI4
ADDRLP4 0
INDIRP4
ARGP4
ADDRGP4 trap_SendServerCommand
CALLV
pop
LABELV $190
LABELV $183
endproc use_script_cmd 4 8
proc script_cmd_think 4 12
ADDRFP4 0
ADDRFP4 0
INDIRP4
ASGNP4
ADDRFP4 0
INDIRP4
CNSTI4 708
ADDP4
CNSTI4 0
ASGNI4
ADDRFP4 0
INDIRP4
ARGP4
CNSTP4 0
ARGP4
ADDRFP4 0
INDIRP4
ARGP4
ADDRGP4 use_script_cmd
CALLV
pop
LABELV $192
endproc script_cmd_think 4 12
export SP_script_cmd
proc SP_script_cmd 0 4
ADDRFP4 0
ADDRFP4 0
INDIRP4
ASGNP4
ADDRFP4 0
INDIRP4
CNSTI4 676
ADDP4
INDIRP4
CVPU4 4
CNSTU4 0
NEU4 $194
ADDRGP4 $196
ARGP4
ADDRGP4 G_Printf
CALLV
pop
LABELV $194
ADDRFP4 0
INDIRP4
CNSTI4 560
ADDP4
INDIRI4
CNSTI4 4096
BANDI4
CNSTI4 0
EQI4 $197
ADDRFP4 0
INDIRP4
CNSTI4 708
ADDP4
ADDRGP4 level+24
INDIRI4
CNSTI4 1000
ADDI4
ASGNI4
ADDRFP4 0
INDIRP4
CNSTI4 712
ADDP4
ADDRGP4 script_cmd_think
ASGNP4
LABELV $197
ADDRFP4 0
INDIRP4
CNSTI4 728
ADDP4
ADDRGP4 use_script_cmd
ASGNP4
LABELV $193
endproc SP_script_cmd 0 4
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
import SandboxBotSpawn
import Svcmd_AddBot_f
import G_AddBot
import G_BotConnect
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
LABELV $196
byte 1 78
byte 1 111
byte 1 32
byte 1 116
byte 1 97
byte 1 114
byte 1 103
byte 1 101
byte 1 116
byte 1 32
byte 1 105
byte 1 110
byte 1 32
byte 1 115
byte 1 99
byte 1 114
byte 1 105
byte 1 112
byte 1 116
byte 1 95
byte 1 99
byte 1 109
byte 1 100
byte 1 10
byte 1 0
align 1
LABELV $189
byte 1 99
byte 1 108
byte 1 99
byte 1 109
byte 1 100
byte 1 32
byte 1 34
byte 1 37
byte 1 115
byte 1 34
byte 1 0
align 1
LABELV $186
byte 1 37
byte 1 115
byte 1 10
byte 1 0
align 1
LABELV $179
byte 1 87
byte 1 65
byte 1 82
byte 1 78
byte 1 73
byte 1 78
byte 1 71
byte 1 58
byte 1 32
byte 1 115
byte 1 99
byte 1 114
byte 1 105
byte 1 112
byte 1 116
byte 1 95
byte 1 118
byte 1 97
byte 1 114
byte 1 105
byte 1 97
byte 1 98
byte 1 108
byte 1 101
byte 1 32
byte 1 119
byte 1 105
byte 1 116
byte 1 104
byte 1 111
byte 1 117
byte 1 116
byte 1 32
byte 1 118
byte 1 97
byte 1 108
byte 1 117
byte 1 101
byte 1 32
byte 1 97
byte 1 116
byte 1 32
byte 1 37
byte 1 115
byte 1 10
byte 1 0
align 1
LABELV $176
byte 1 87
byte 1 65
byte 1 82
byte 1 78
byte 1 73
byte 1 78
byte 1 71
byte 1 58
byte 1 32
byte 1 115
byte 1 99
byte 1 114
byte 1 105
byte 1 112
byte 1 116
byte 1 95
byte 1 118
byte 1 97
byte 1 114
byte 1 105
byte 1 97
byte 1 98
byte 1 108
byte 1 101
byte 1 32
byte 1 119
byte 1 105
byte 1 116
byte 1 104
byte 1 111
byte 1 117
byte 1 116
byte 1 32
byte 1 107
byte 1 101
byte 1 121
byte 1 32
byte 1 97
byte 1 116
byte 1 32
byte 1 37
byte 1 115
byte 1 10
byte 1 0
align 1
LABELV $171
byte 1 37
byte 1 115
byte 1 32
byte 1 45
byte 1 32
byte 1 37
byte 1 115
byte 1 10
byte 1 0
align 1
LABELV $150
byte 1 116
byte 1 97
byte 1 114
byte 1 103
byte 1 101
byte 1 116
byte 1 95
byte 1 108
byte 1 111
byte 1 99
byte 1 97
byte 1 116
byte 1 105
byte 1 111
byte 1 110
byte 1 0
align 1
LABELV $142
byte 1 117
byte 1 110
byte 1 107
byte 1 110
byte 1 111
byte 1 119
byte 1 110
byte 1 0
align 1
LABELV $122
byte 1 117
byte 1 110
byte 1 116
byte 1 97
byte 1 114
byte 1 103
byte 1 101
byte 1 116
byte 1 101
byte 1 100
byte 1 32
byte 1 37
byte 1 115
byte 1 32
byte 1 97
byte 1 116
byte 1 32
byte 1 37
byte 1 115
byte 1 10
byte 1 0
align 1
LABELV $118
byte 1 67
byte 1 111
byte 1 117
byte 1 108
byte 1 100
byte 1 110
byte 1 39
byte 1 116
byte 1 32
byte 1 102
byte 1 105
byte 1 110
byte 1 100
byte 1 32
byte 1 116
byte 1 101
byte 1 108
byte 1 101
byte 1 112
byte 1 111
byte 1 114
byte 1 116
byte 1 101
byte 1 114
byte 1 32
byte 1 100
byte 1 101
byte 1 115
byte 1 116
byte 1 105
byte 1 110
byte 1 97
byte 1 116
byte 1 105
byte 1 111
byte 1 110
byte 1 10
byte 1 0
align 1
LABELV $108
byte 1 37
byte 1 115
byte 1 46
byte 1 119
byte 1 97
byte 1 118
byte 1 0
align 1
LABELV $107
byte 1 46
byte 1 119
byte 1 97
byte 1 118
byte 1 0
align 1
LABELV $102
byte 1 116
byte 1 97
byte 1 114
byte 1 103
byte 1 101
byte 1 116
byte 1 95
byte 1 115
byte 1 112
byte 1 101
byte 1 97
byte 1 107
byte 1 101
byte 1 114
byte 1 32
byte 1 119
byte 1 105
byte 1 116
byte 1 104
byte 1 111
byte 1 117
byte 1 116
byte 1 32
byte 1 97
byte 1 32
byte 1 110
byte 1 111
byte 1 105
byte 1 115
byte 1 101
byte 1 32
byte 1 107
byte 1 101
byte 1 121
byte 1 32
byte 1 97
byte 1 116
byte 1 32
byte 1 37
byte 1 115
byte 1 0
align 1
LABELV $101
byte 1 78
byte 1 79
byte 1 83
byte 1 79
byte 1 85
byte 1 78
byte 1 68
byte 1 0
align 1
LABELV $100
byte 1 110
byte 1 111
byte 1 105
byte 1 115
byte 1 101
byte 1 0
align 1
LABELV $97
byte 1 114
byte 1 97
byte 1 110
byte 1 100
byte 1 111
byte 1 109
byte 1 0
align 1
LABELV $79
byte 1 99
byte 1 112
byte 1 32
byte 1 34
byte 1 37
byte 1 115
byte 1 34
byte 1 0
align 1
LABELV $73
byte 1 49
byte 1 0
align 1
LABELV $72
byte 1 119
byte 1 97
byte 1 105
byte 1 116
byte 1 0
align 1
LABELV $71
byte 1 48
byte 1 0
align 1
LABELV $70
byte 1 100
byte 1 101
byte 1 108
byte 1 97
byte 1 121
byte 1 0
