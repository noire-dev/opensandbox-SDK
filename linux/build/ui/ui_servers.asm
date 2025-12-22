data
align 4
LABELV master_items
address $59
address $60
byte 4 0
align 4
LABELV netnames
address $61
address $62
address $63
byte 4 0
code
proc ArenaServers_MaxPing 8 4
ADDRGP4 $68
ARGP4
ADDRLP4 4
ADDRGP4 cvarInt
CALLI4
ASGNI4
ADDRLP4 0
ADDRLP4 4
INDIRI4
ASGNI4
ADDRLP4 0
INDIRI4
CNSTI4 100
GEI4 $69
ADDRLP4 0
CNSTI4 100
ASGNI4
LABELV $69
ADDRLP4 0
INDIRI4
RETI4
LABELV $67
endproc ArenaServers_MaxPing 8 4
proc ArenaServers_Go 12 8
ADDRLP4 0
CNSTI4 116
ADDRGP4 g_arenaservers+2488+156
INDIRI4
MULI4
ADDRGP4 g_arenaservers+6128+112
ADDP4
INDIRP4
ASGNP4
ADDRLP4 0
INDIRP4
CVPU4 4
CNSTU4 0
EQU4 $76
ADDRGP4 $78
ARGP4
ADDRLP4 0
INDIRP4
CNSTI4 89
ADDP4
ARGP4
ADDRLP4 4
ADDRGP4 va
CALLP4
ASGNP4
CNSTI4 2
ARGI4
ADDRLP4 4
INDIRP4
ARGP4
ADDRGP4 trap_Cmd
CALLV
pop
ADDRGP4 $79
ARGP4
ADDRLP4 0
INDIRP4
ARGP4
ADDRLP4 8
ADDRGP4 va
CALLP4
ASGNP4
CNSTI4 2
ARGI4
ADDRLP4 8
INDIRP4
ARGP4
ADDRGP4 trap_Cmd
CALLV
pop
LABELV $76
LABELV $71
endproc ArenaServers_Go 12 8
bss
align 1
LABELV $81
skip 64
code
proc ArenaServers_UpdatePicture 4 16
ADDRGP4 g_arenaservers+2488+164
INDIRI4
CNSTI4 0
NEI4 $82
ADDRGP4 g_arenaservers+2976+108
CNSTP4 0
ASGNP4
ADDRGP4 $83
JUMPV
LABELV $82
ADDRLP4 0
CNSTI4 116
ADDRGP4 g_arenaservers+2488+156
INDIRI4
MULI4
ADDRGP4 g_arenaservers+6128+112
ADDP4
INDIRP4
ASGNP4
ADDRGP4 $81
ARGP4
CNSTI4 64
ARGI4
ADDRGP4 $92
ARGP4
ADDRLP4 0
INDIRP4
CNSTI4 111
ADDP4
ARGP4
ADDRGP4 Com_sprintf
CALLV
pop
ADDRGP4 g_arenaservers+2976+108
ADDRGP4 $81
ASGNP4
LABELV $83
ADDRGP4 g_arenaservers+2976+140
CNSTI4 0
ASGNI4
LABELV $80
endproc ArenaServers_UpdatePicture 4 16
proc ArenaServers_UpdateMenu 40 44
ADDRGP4 g_arenaservers+21488
INDIRI4
CNSTI4 0
LEI4 $98
ADDRGP4 g_arenaservers+21504
INDIRI4
CNSTI4 0
EQI4 $101
ADDRGP4 g_arenaservers+21500
INDIRI4
ADDRGP4 g_arenaservers+21488
INDIRI4
GTI4 $101
ADDRGP4 g_arenaservers+536+108
INDIRP4
ARGP4
CNSTI4 64
ARGI4
ADDRGP4 $108
ARGP4
ADDRGP4 g_arenaservers+21500
INDIRI4
ARGI4
ADDRGP4 g_arenaservers+21488
INDIRI4
ARGI4
ADDRGP4 Com_sprintf
CALLV
pop
ADDRGP4 $99
JUMPV
LABELV $101
ADDRLP4 24
ADDRGP4 g_arenaservers+1024+80
ASGNP4
ADDRLP4 24
INDIRP4
ADDRLP4 24
INDIRP4
INDIRU4
CNSTU4 4294967039
BANDU4
ASGNU4
ADDRLP4 28
ADDRGP4 g_arenaservers+2488+80
ASGNP4
ADDRLP4 28
INDIRP4
ADDRLP4 28
INDIRP4
INDIRU4
CNSTU4 4294967039
BANDU4
ASGNU4
ADDRLP4 32
ADDRGP4 g_arenaservers+3464+80
ASGNP4
ADDRLP4 32
INDIRP4
ADDRLP4 32
INDIRP4
INDIRU4
CNSTU4 4294967039
BANDU4
ASGNU4
ADDRGP4 $99
JUMPV
LABELV $98
ADDRGP4 g_arenaservers+21504
INDIRI4
CNSTI4 0
EQI4 $117
ADDRGP4 g_arenaservers+536+108
INDIRP4
ARGP4
ADDRGP4 $122
ARGP4
ADDRGP4 strcpy
CALLP4
pop
ADDRLP4 24
ADDRGP4 g_arenaservers+1024+80
ASGNP4
ADDRLP4 24
INDIRP4
ADDRLP4 24
INDIRP4
INDIRU4
CNSTU4 256
BORU4
ASGNU4
ADDRLP4 28
ADDRGP4 g_arenaservers+2488+80
ASGNP4
ADDRLP4 28
INDIRP4
ADDRLP4 28
INDIRP4
INDIRU4
CNSTU4 256
BORU4
ASGNU4
ADDRLP4 32
ADDRGP4 g_arenaservers+3464+80
ASGNP4
ADDRLP4 32
INDIRP4
ADDRLP4 32
INDIRP4
INDIRU4
CNSTU4 256
BORU4
ASGNU4
ADDRGP4 $118
JUMPV
LABELV $117
ADDRGP4 g_arenaservers+21488
INDIRI4
CNSTI4 0
GEI4 $129
ADDRGP4 g_arenaservers+536+108
INDIRP4
ARGP4
ADDRGP4 $134
ARGP4
ADDRGP4 strcpy
CALLP4
pop
ADDRGP4 $130
JUMPV
LABELV $129
ADDRGP4 g_arenaservers+536+108
INDIRP4
ARGP4
ADDRGP4 $137
ARGP4
ADDRGP4 strcpy
CALLP4
pop
LABELV $130
ADDRLP4 24
ADDRGP4 g_arenaservers+1024+80
ASGNP4
ADDRLP4 24
INDIRP4
ADDRLP4 24
INDIRP4
INDIRU4
CNSTU4 4294967039
BANDU4
ASGNU4
ADDRLP4 28
ADDRGP4 g_arenaservers+2488+80
ASGNP4
ADDRLP4 28
INDIRP4
ADDRLP4 28
INDIRP4
INDIRU4
CNSTU4 256
BORU4
ASGNU4
ADDRLP4 32
ADDRGP4 g_arenaservers+3464+80
ASGNP4
ADDRLP4 32
INDIRP4
ADDRLP4 32
INDIRP4
INDIRU4
CNSTU4 4294967039
BANDU4
ASGNU4
LABELV $118
ADDRGP4 g_arenaservers+2488+164
CNSTI4 0
ASGNI4
ADDRGP4 g_arenaservers+2488+156
CNSTI4 0
ASGNI4
ADDRGP4 g_arenaservers+2488+168
CNSTI4 0
ASGNI4
ADDRGP4 ArenaServers_UpdatePicture
CALLV
pop
ADDRGP4 $97
JUMPV
LABELV $99
ADDRLP4 0
ADDRGP4 g_arenaservers+21496
INDIRP4
ASGNP4
ADDRLP4 20
ADDRGP4 g_arenaservers+21492
INDIRP4
INDIRI4
ASGNI4
ADDRLP4 24
CNSTI4 0
ASGNI4
ADDRLP4 12
ADDRLP4 24
INDIRI4
ASGNI4
ADDRLP4 8
ADDRLP4 24
INDIRI4
ASGNI4
ADDRGP4 $155
JUMPV
LABELV $152
ADDRLP4 4
CNSTI4 116
ADDRLP4 8
INDIRI4
MULI4
ADDRGP4 g_arenaservers+6128
ADDP4
ASGNP4
ADDRLP4 4
INDIRP4
CNSTI4 112
ADDP4
ADDRLP4 0
INDIRP4
ASGNP4
ADDRLP4 16
ADDRLP4 4
INDIRP4
ASGNP4
ADDRLP4 0
INDIRP4
CNSTI4 136
ADDP4
INDIRI4
CNSTI4 0
NEI4 $157
ADDRGP4 $153
JUMPV
LABELV $157
ADDRLP4 0
INDIRP4
CNSTI4 136
ADDP4
INDIRI4
ADDRLP4 0
INDIRP4
CNSTI4 140
ADDP4
INDIRI4
NEI4 $159
ADDRGP4 $153
JUMPV
LABELV $159
ADDRLP4 16
INDIRP4
ARGP4
ADDRLP4 32
CNSTI4 111
ASGNI4
ADDRLP4 32
INDIRI4
ARGI4
ADDRGP4 $161
ARGP4
ADDRLP4 0
INDIRP4
CNSTI4 64
ADDP4
ARGP4
ADDRLP4 0
INDIRP4
CNSTI4 89
ADDP4
ARGP4
ADDRLP4 0
INDIRP4
ADDRLP4 32
INDIRI4
ADDP4
ARGP4
ADDRLP4 0
INDIRP4
CNSTI4 136
ADDP4
INDIRI4
ARGI4
ADDRLP4 0
INDIRP4
CNSTI4 140
ADDP4
INDIRI4
ARGI4
ADDRLP4 0
INDIRP4
CNSTI4 152
ADDP4
ARGP4
ADDRLP4 0
INDIRP4
CNSTI4 176
ADDP4
INDIRI4
CNSTI4 2
LSHI4
ADDRGP4 netnames
ADDP4
INDIRP4
ARGP4
ADDRLP4 0
INDIRP4
CNSTI4 144
ADDP4
INDIRI4
ARGI4
ADDRGP4 Com_sprintf
CALLV
pop
ADDRLP4 8
ADDRLP4 8
INDIRI4
CNSTI4 1
ADDI4
ASGNI4
LABELV $153
ADDRLP4 12
ADDRLP4 12
INDIRI4
CNSTI4 1
ADDI4
ASGNI4
ADDRLP4 0
ADDRLP4 0
INDIRP4
CNSTI4 188
ADDP4
ASGNP4
LABELV $155
ADDRLP4 12
INDIRI4
ADDRLP4 20
INDIRI4
LTI4 $152
ADDRGP4 g_arenaservers+2488+164
ADDRLP4 8
INDIRI4
ASGNI4
ADDRGP4 g_arenaservers+2488+156
CNSTI4 0
ASGNI4
ADDRGP4 g_arenaservers+2488+168
CNSTI4 0
ASGNI4
ADDRGP4 ArenaServers_UpdatePicture
CALLV
pop
LABELV $97
endproc ArenaServers_UpdateMenu 40 44
proc ArenaServers_Insert 72 12
ADDRFP4 4
ADDRFP4 4
INDIRP4
ASGNP4
ADDRLP4 8
ADDRGP4 ArenaServers_MaxPing
CALLI4
ASGNI4
ADDRFP4 8
INDIRI4
ADDRLP4 8
INDIRI4
LTI4 $169
ADDRGP4 $168
JUMPV
LABELV $169
ADDRGP4 g_arenaservers+21492
INDIRP4
INDIRI4
ADDRGP4 g_arenaservers+21512
INDIRI4
LTI4 $171
ADDRLP4 0
CNSTI4 188
ADDRGP4 g_arenaservers+21492
INDIRP4
INDIRI4
MULI4
ADDRGP4 g_arenaservers+21496
INDIRP4
ADDP4
CNSTI4 -188
ADDP4
ASGNP4
ADDRGP4 $172
JUMPV
LABELV $171
ADDRLP4 0
CNSTI4 188
ADDRGP4 g_arenaservers+21492
INDIRP4
INDIRI4
MULI4
ADDRGP4 g_arenaservers+21496
INDIRP4
ADDP4
ASGNP4
ADDRLP4 12
ADDRGP4 g_arenaservers+21492
INDIRP4
ASGNP4
ADDRLP4 12
INDIRP4
ADDRLP4 12
INDIRP4
INDIRI4
CNSTI4 1
ADDI4
ASGNI4
LABELV $172
ADDRLP4 0
INDIRP4
ARGP4
ADDRFP4 0
INDIRP4
ARGP4
CNSTI4 64
ARGI4
ADDRGP4 Q_strncpyz
CALLV
pop
ADDRFP4 4
INDIRP4
ARGP4
ADDRGP4 $180
ARGP4
ADDRLP4 12
ADDRGP4 Info_ValueForKey
CALLP4
ASGNP4
ADDRLP4 0
INDIRP4
CNSTI4 64
ADDP4
ARGP4
ADDRLP4 12
INDIRP4
ARGP4
CNSTI4 22
ARGI4
ADDRGP4 Q_strncpyz
CALLV
pop
ADDRLP4 0
INDIRP4
CNSTI4 64
ADDP4
ARGP4
ADDRGP4 Q_CleanStr
CALLP4
pop
ADDRLP4 0
INDIRP4
CNSTI4 64
ADDP4
ARGP4
ADDRGP4 Q_strupr
CALLP4
pop
ADDRFP4 4
INDIRP4
ARGP4
ADDRGP4 $181
ARGP4
ADDRLP4 16
ADDRGP4 Info_ValueForKey
CALLP4
ASGNP4
ADDRLP4 0
INDIRP4
CNSTI4 111
ADDP4
ARGP4
ADDRLP4 16
INDIRP4
ARGP4
CNSTI4 22
ARGI4
ADDRGP4 Q_strncpyz
CALLV
pop
ADDRLP4 0
INDIRP4
CNSTI4 111
ADDP4
ARGP4
ADDRGP4 Q_CleanStr
CALLP4
pop
ADDRLP4 0
INDIRP4
CNSTI4 111
ADDP4
ARGP4
ADDRGP4 Q_strupr
CALLP4
pop
ADDRFP4 4
INDIRP4
ARGP4
ADDRGP4 $182
ARGP4
ADDRLP4 20
ADDRGP4 Info_ValueForKey
CALLP4
ASGNP4
ADDRLP4 0
INDIRP4
CNSTI4 89
ADDP4
ARGP4
ADDRLP4 20
INDIRP4
ARGP4
CNSTI4 22
ARGI4
ADDRGP4 Q_strncpyz
CALLV
pop
ADDRLP4 0
INDIRP4
CNSTI4 89
ADDP4
ARGP4
ADDRGP4 Q_CleanStr
CALLP4
pop
ADDRLP4 0
INDIRP4
CNSTI4 89
ADDP4
ARGP4
ADDRGP4 Q_strupr
CALLP4
pop
ADDRFP4 4
INDIRP4
ARGP4
ADDRGP4 $183
ARGP4
ADDRLP4 24
ADDRGP4 Info_ValueForKey
CALLP4
ASGNP4
ADDRLP4 24
INDIRP4
ARGP4
ADDRLP4 28
ADDRGP4 atoi
CALLI4
ASGNI4
ADDRLP4 0
INDIRP4
CNSTI4 136
ADDP4
ADDRLP4 28
INDIRI4
ASGNI4
ADDRFP4 4
INDIRP4
ARGP4
ADDRGP4 $184
ARGP4
ADDRLP4 32
ADDRGP4 Info_ValueForKey
CALLP4
ASGNP4
ADDRLP4 32
INDIRP4
ARGP4
ADDRLP4 36
ADDRGP4 atoi
CALLI4
ASGNI4
ADDRLP4 0
INDIRP4
CNSTI4 140
ADDP4
ADDRLP4 36
INDIRI4
ASGNI4
ADDRLP4 0
INDIRP4
CNSTI4 144
ADDP4
ADDRFP4 8
INDIRI4
ASGNI4
ADDRFP4 4
INDIRP4
ARGP4
ADDRGP4 $185
ARGP4
ADDRLP4 40
ADDRGP4 Info_ValueForKey
CALLP4
ASGNP4
ADDRLP4 40
INDIRP4
ARGP4
ADDRLP4 44
ADDRGP4 atoi
CALLI4
ASGNI4
ADDRLP4 0
INDIRP4
CNSTI4 180
ADDP4
ADDRLP4 44
INDIRI4
ASGNI4
ADDRFP4 4
INDIRP4
ARGP4
ADDRGP4 $186
ARGP4
ADDRLP4 48
ADDRGP4 Info_ValueForKey
CALLP4
ASGNP4
ADDRLP4 48
INDIRP4
ARGP4
ADDRLP4 52
ADDRGP4 atoi
CALLI4
ASGNI4
ADDRLP4 0
INDIRP4
CNSTI4 184
ADDP4
ADDRLP4 52
INDIRI4
ASGNI4
ADDRFP4 4
INDIRP4
ARGP4
ADDRGP4 $187
ARGP4
ADDRLP4 56
ADDRGP4 Info_ValueForKey
CALLP4
ASGNP4
ADDRLP4 56
INDIRP4
ARGP4
ADDRLP4 60
ADDRGP4 atoi
CALLI4
ASGNI4
ADDRLP4 0
INDIRP4
CNSTI4 176
ADDP4
ADDRLP4 60
INDIRI4
ASGNI4
ADDRFP4 4
INDIRP4
ARGP4
ADDRGP4 $188
ARGP4
ADDRLP4 64
ADDRGP4 Info_ValueForKey
CALLP4
ASGNP4
ADDRLP4 64
INDIRP4
ARGP4
ADDRLP4 68
ADDRGP4 atoi
CALLI4
ASGNI4
ADDRLP4 4
ADDRLP4 68
INDIRI4
ASGNI4
ADDRLP4 0
INDIRP4
CNSTI4 148
ADDP4
ADDRLP4 4
INDIRI4
ASGNI4
ADDRLP4 0
INDIRP4
CNSTI4 152
ADDP4
ARGP4
ADDRLP4 4
INDIRI4
CNSTI4 2
LSHI4
ADDRGP4 gametype_items
ADDP4
INDIRP4
ARGP4
CNSTI4 22
ARGI4
ADDRGP4 Q_strncpyz
CALLV
pop
LABELV $168
endproc ArenaServers_Insert 72 12
proc ArenaServers_StopRefresh 0 0
ADDRGP4 g_arenaservers+21504
INDIRI4
CNSTI4 0
NEI4 $190
ADDRGP4 $189
JUMPV
LABELV $190
ADDRGP4 g_arenaservers+21504
CNSTI4 0
ASGNI4
ADDRGP4 g_arenaservers+21488
INDIRI4
CNSTI4 0
LTI4 $194
ADDRGP4 g_arenaservers+21500
ADDRGP4 g_arenaservers+21492
INDIRP4
INDIRI4
ASGNI4
ADDRGP4 g_arenaservers+21488
ADDRGP4 g_arenaservers+21492
INDIRP4
INDIRI4
ASGNI4
LABELV $194
ADDRGP4 ArenaServers_UpdateMenu
CALLV
pop
LABELV $189
endproc ArenaServers_StopRefresh 0 0
proc ArenaServers_DoRefresh 1124 16
ADDRGP4 uis+4
INDIRI4
ADDRGP4 g_arenaservers+21516
INDIRI4
GEI4 $202
ADDRGP4 g_servertype
INDIRI4
CNSTI4 0
NEI4 $206
ADDRGP4 g_servertype
INDIRI4
ARGI4
ADDRLP4 1104
ADDRGP4 trap_LAN_GetServerCount
CALLI4
ASGNI4
ADDRLP4 1104
INDIRI4
CNSTI4 0
NEI4 $208
ADDRGP4 $201
JUMPV
LABELV $208
LABELV $206
ADDRGP4 g_servertype
INDIRI4
ARGI4
ADDRLP4 1104
ADDRGP4 trap_LAN_GetServerCount
CALLI4
ASGNI4
ADDRLP4 1104
INDIRI4
CNSTI4 0
GEI4 $210
ADDRGP4 $201
JUMPV
LABELV $210
LABELV $202
ADDRGP4 uis+4
INDIRI4
ADDRGP4 g_arenaservers+21508
INDIRI4
GEI4 $212
ADDRGP4 $201
JUMPV
LABELV $212
ADDRGP4 g_arenaservers+21508
ADDRGP4 uis+4
INDIRI4
CNSTI4 10
ADDI4
ASGNI4
ADDRLP4 1104
ADDRGP4 ArenaServers_MaxPing
CALLI4
ASGNI4
ADDRLP4 76
ADDRLP4 1104
INDIRI4
ASGNI4
ADDRLP4 68
CNSTI4 0
ASGNI4
LABELV $218
ADDRLP4 68
INDIRI4
ARGI4
ADDRLP4 4
ARGP4
CNSTI4 64
ARGI4
ADDRLP4 72
ARGP4
ADDRGP4 trap_LAN_GetPing
CALLV
pop
ADDRLP4 4
INDIRI1
CVII4 1
CNSTI4 0
NEI4 $222
ADDRGP4 $219
JUMPV
LABELV $222
ADDRLP4 0
CNSTI4 0
ASGNI4
LABELV $224
ADDRLP4 4
ARGP4
CNSTI4 68
ADDRLP4 0
INDIRI4
MULI4
ADDRGP4 g_arenaservers+3952
ADDP4
ARGP4
ADDRLP4 1108
ADDRGP4 Q_stricmp
CALLI4
ASGNI4
ADDRLP4 1108
INDIRI4
CNSTI4 0
NEI4 $228
ADDRGP4 $226
JUMPV
LABELV $228
LABELV $225
ADDRLP4 0
ADDRLP4 0
INDIRI4
CNSTI4 1
ADDI4
ASGNI4
ADDRLP4 0
INDIRI4
CNSTI4 32
LTI4 $224
LABELV $226
ADDRLP4 0
INDIRI4
CNSTI4 32
GEI4 $231
ADDRLP4 72
INDIRI4
CNSTI4 0
NEI4 $233
ADDRLP4 72
ADDRGP4 uis+4
INDIRI4
CNSTI4 68
ADDRLP4 0
INDIRI4
MULI4
ADDRGP4 g_arenaservers+3952+64
ADDP4
INDIRI4
SUBI4
ASGNI4
ADDRLP4 72
INDIRI4
ADDRLP4 76
INDIRI4
GEI4 $238
ADDRGP4 $219
JUMPV
LABELV $238
LABELV $233
ADDRLP4 72
INDIRI4
ADDRLP4 76
INDIRI4
LEI4 $240
ADDRLP4 80
CNSTI1 0
ASGNI1
ADDRLP4 72
ADDRLP4 76
INDIRI4
ASGNI4
ADDRGP4 $241
JUMPV
LABELV $240
ADDRLP4 68
INDIRI4
ARGI4
ADDRLP4 80
ARGP4
CNSTI4 1024
ARGI4
ADDRGP4 trap_LAN_GetPingInfo
CALLV
pop
LABELV $241
ADDRLP4 4
ARGP4
ADDRLP4 80
ARGP4
ADDRLP4 72
INDIRI4
ARGI4
ADDRGP4 ArenaServers_Insert
CALLV
pop
CNSTI4 68
ADDRLP4 0
INDIRI4
MULI4
ADDRGP4 g_arenaservers+3952
ADDP4
CNSTI1 0
ASGNI1
LABELV $231
ADDRLP4 68
INDIRI4
ARGI4
ADDRGP4 trap_LAN_ClearPing
CALLV
pop
LABELV $219
ADDRLP4 68
ADDRLP4 68
INDIRI4
CNSTI4 1
ADDI4
ASGNI4
ADDRLP4 68
INDIRI4
CNSTI4 32
LTI4 $218
ADDRGP4 g_servertype
INDIRI4
ARGI4
ADDRLP4 1108
ADDRGP4 trap_LAN_GetServerCount
CALLI4
ASGNI4
ADDRGP4 g_arenaservers+21488
ADDRLP4 1108
INDIRI4
ASGNI4
ADDRLP4 68
CNSTI4 0
ASGNI4
ADDRGP4 $247
JUMPV
LABELV $244
ADDRLP4 1112
ADDRGP4 trap_LAN_GetPingQueueCount
CALLI4
ASGNI4
ADDRLP4 1112
INDIRI4
CNSTI4 32
LTI4 $250
ADDRGP4 $246
JUMPV
LABELV $250
ADDRLP4 0
CNSTI4 0
ASGNI4
LABELV $252
CNSTI4 68
ADDRLP4 0
INDIRI4
MULI4
ADDRGP4 g_arenaservers+3952
ADDP4
INDIRI1
CVII4 1
CNSTI4 0
NEI4 $256
ADDRGP4 $254
JUMPV
LABELV $256
LABELV $253
ADDRLP4 0
ADDRLP4 0
INDIRI4
CNSTI4 1
ADDI4
ASGNI4
ADDRLP4 0
INDIRI4
CNSTI4 32
LTI4 $252
LABELV $254
ADDRLP4 0
INDIRI4
CNSTI4 32
LTI4 $259
ADDRGP4 $246
JUMPV
LABELV $259
ADDRGP4 g_servertype
INDIRI4
ARGI4
ADDRGP4 g_arenaservers+21500
INDIRI4
ARGI4
ADDRLP4 4
ARGP4
CNSTI4 64
ARGI4
ADDRGP4 trap_LAN_GetServerAddressString
CALLV
pop
CNSTI4 68
ADDRLP4 0
INDIRI4
MULI4
ADDRGP4 g_arenaservers+3952
ADDP4
ARGP4
ADDRLP4 4
ARGP4
ADDRGP4 strcpy
CALLP4
pop
CNSTI4 68
ADDRLP4 0
INDIRI4
MULI4
ADDRGP4 g_arenaservers+3952+64
ADDP4
ADDRGP4 uis+4
INDIRI4
ASGNI4
ADDRGP4 $266
ARGP4
ADDRLP4 4
ARGP4
ADDRLP4 1116
ADDRGP4 va
CALLP4
ASGNP4
CNSTI4 0
ARGI4
ADDRLP4 1116
INDIRP4
ARGP4
ADDRGP4 trap_Cmd
CALLV
pop
ADDRLP4 1120
ADDRGP4 g_arenaservers+21500
ASGNP4
ADDRLP4 1120
INDIRP4
ADDRLP4 1120
INDIRP4
INDIRI4
CNSTI4 1
ADDI4
ASGNI4
LABELV $245
ADDRLP4 68
ADDRLP4 68
INDIRI4
CNSTI4 1
ADDI4
ASGNI4
LABELV $247
ADDRLP4 68
INDIRI4
CNSTI4 32
GEI4 $268
ADDRGP4 g_arenaservers+21500
INDIRI4
ADDRGP4 g_arenaservers+21488
INDIRI4
LTI4 $244
LABELV $268
LABELV $246
ADDRLP4 1112
ADDRGP4 trap_LAN_GetPingQueueCount
CALLI4
ASGNI4
ADDRLP4 1112
INDIRI4
CNSTI4 0
NEI4 $269
ADDRGP4 ArenaServers_StopRefresh
CALLV
pop
ADDRGP4 $201
JUMPV
LABELV $269
ADDRGP4 ArenaServers_UpdateMenu
CALLV
pop
LABELV $201
endproc ArenaServers_DoRefresh 1124 16
proc ArenaServers_StartRefresh 44 16
ADDRGP4 g_arenaservers+21496
INDIRP4
ARGP4
CNSTI4 0
ARGI4
CNSTU4 116
ADDRGP4 g_arenaservers+21512
INDIRI4
CVIU4 4
MULU4
CVUI4 4
ARGI4
ADDRGP4 memset
CALLP4
pop
ADDRLP4 0
CNSTI4 0
ASGNI4
LABELV $274
CNSTI4 68
ADDRLP4 0
INDIRI4
MULI4
ADDRGP4 g_arenaservers+3952
ADDP4
CNSTI1 0
ASGNI1
ADDRLP4 0
INDIRI4
ARGI4
ADDRGP4 trap_LAN_ClearPing
CALLV
pop
LABELV $275
ADDRLP4 0
ADDRLP4 0
INDIRI4
CNSTI4 1
ADDI4
ASGNI4
ADDRLP4 0
INDIRI4
CNSTI4 32
LTI4 $274
ADDRGP4 g_arenaservers+21504
CNSTI4 1
ASGNI4
ADDRGP4 g_arenaservers+21500
CNSTI4 0
ASGNI4
ADDRGP4 g_arenaservers+21508
CNSTI4 0
ASGNI4
ADDRGP4 g_arenaservers+21492
INDIRP4
CNSTI4 0
ASGNI4
ADDRGP4 g_arenaservers+21488
CNSTI4 0
ASGNI4
ADDRGP4 g_arenaservers+21516
ADDRGP4 uis+4
INDIRI4
CNSTI4 5000
ADDI4
ASGNI4
ADDRGP4 ArenaServers_UpdateMenu
CALLV
pop
ADDRGP4 g_servertype
INDIRI4
CNSTI4 0
NEI4 $286
CNSTI4 2
ARGI4
ADDRGP4 $288
ARGP4
ADDRGP4 trap_Cmd
CALLV
pop
ADDRGP4 $271
JUMPV
LABELV $286
ADDRGP4 g_servertype
INDIRI4
CNSTI4 1
NEI4 $289
ADDRLP4 0
CNSTI4 0
ASGNI4
ADDRGP4 $292
ARGP4
ADDRLP4 36
ADDRGP4 cvarInt
CALLI4
ASGNI4
ADDRGP4 $291
ARGP4
ADDRLP4 0
INDIRI4
ARGI4
ADDRLP4 36
INDIRI4
ARGI4
ADDRLP4 4
ARGP4
ADDRLP4 40
ADDRGP4 va
CALLP4
ASGNP4
CNSTI4 2
ARGI4
ADDRLP4 40
INDIRP4
ARGP4
ADDRGP4 trap_Cmd
CALLV
pop
LABELV $289
LABELV $271
endproc ArenaServers_StartRefresh 44 16
proc ArenaServers_SetType 12 8
ADDRFP4 0
ADDRFP4 0
INDIRI4
ASGNI4
ADDRGP4 g_servertype
INDIRI4
ADDRFP4 0
INDIRI4
NEI4 $294
ADDRGP4 $293
JUMPV
LABELV $294
ADDRGP4 g_servertype
ADDRFP4 0
INDIRI4
ASGNI4
ADDRGP4 $296
ARGP4
ADDRFP4 0
INDIRI4
ARGI4
ADDRLP4 4
ADDRGP4 va
CALLP4
ASGNP4
ADDRLP4 4
INDIRP4
ARGP4
ADDRGP4 trap_Print
CALLV
pop
ADDRFP4 0
INDIRI4
CNSTI4 0
EQI4 $299
ADDRFP4 0
INDIRI4
CNSTI4 1
EQI4 $303
ADDRGP4 $297
JUMPV
LABELV $297
LABELV $299
ADDRGP4 g_arenaservers+21496
ADDRGP4 g_localserverlist
ASGNP4
ADDRGP4 g_arenaservers+21492
ADDRGP4 g_numlocalservers
ASGNP4
ADDRGP4 g_arenaservers+21512
CNSTI4 128
ASGNI4
ADDRGP4 $298
JUMPV
LABELV $303
ADDRGP4 g_arenaservers+21496
ADDRGP4 g_globalserverlist
ASGNP4
ADDRGP4 g_arenaservers+21492
ADDRGP4 g_numglobalservers
ASGNP4
ADDRGP4 g_arenaservers+21512
CNSTI4 128
ASGNI4
LABELV $298
ADDRGP4 g_arenaservers+21492
INDIRP4
INDIRI4
CNSTI4 0
NEI4 $307
ADDRGP4 ArenaServers_StartRefresh
CALLV
pop
ADDRGP4 $308
JUMPV
LABELV $307
ADDRGP4 g_arenaservers+21500
ADDRGP4 g_arenaservers+21492
INDIRP4
INDIRI4
ASGNI4
ADDRGP4 g_arenaservers+21488
ADDRGP4 g_arenaservers+21492
INDIRP4
INDIRI4
ASGNI4
ADDRGP4 ArenaServers_UpdateMenu
CALLV
pop
LABELV $308
ADDRGP4 g_arenaservers+536+108
INDIRP4
ARGP4
ADDRGP4 $316
ARGP4
ADDRGP4 strcpy
CALLP4
pop
LABELV $293
endproc ArenaServers_SetType 12 8
proc ArenaServers_Event 20 8
ADDRLP4 0
ADDRFP4 0
INDIRP4
CNSTI4 40
ADDP4
INDIRI4
ASGNI4
ADDRFP4 4
INDIRI4
CNSTI4 3
EQI4 $318
ADDRLP4 0
INDIRI4
CNSTI4 12
EQI4 $318
ADDRGP4 $317
JUMPV
LABELV $318
ADDRLP4 0
INDIRI4
CNSTI4 10
LTI4 $320
ADDRLP4 0
INDIRI4
CNSTI4 13
GTI4 $320
ADDRLP4 0
INDIRI4
CNSTI4 2
LSHI4
ADDRGP4 $337-40
ADDP4
INDIRP4
JUMPV
lit
align 4
LABELV $337
address $322
address $327
address $331
address $336
code
LABELV $322
ADDRLP4 4
ADDRGP4 g_arenaservers+1024+156
INDIRI4
ASGNI4
ADDRGP4 $326
ARGP4
ADDRLP4 4
INDIRI4
ARGI4
ADDRLP4 12
ADDRGP4 va
CALLP4
ASGNP4
ADDRGP4 $325
ARGP4
ADDRLP4 12
INDIRP4
ARGP4
ADDRGP4 cvarSet
CALLV
pop
ADDRLP4 4
INDIRI4
ARGI4
ADDRGP4 ArenaServers_SetType
CALLV
pop
ADDRGP4 $321
JUMPV
LABELV $327
ADDRGP4 $79
ARGP4
ADDRGP4 g_arenaservers+1512+200+12
ARGP4
ADDRLP4 16
ADDRGP4 va
CALLP4
ASGNP4
CNSTI4 1
ARGI4
ADDRLP4 16
INDIRP4
ARGP4
ADDRGP4 trap_Cmd
CALLV
pop
ADDRGP4 $321
JUMPV
LABELV $331
ADDRFP4 4
INDIRI4
CNSTI4 1
NEI4 $332
ADDRGP4 ArenaServers_UpdatePicture
CALLV
pop
LABELV $332
ADDRFP4 4
INDIRI4
CNSTI4 3
NEI4 $321
ADDRGP4 ArenaServers_Go
CALLV
pop
ADDRGP4 $321
JUMPV
LABELV $336
ADDRGP4 ArenaServers_StartRefresh
CALLV
pop
LABELV $320
LABELV $321
LABELV $317
endproc ArenaServers_Event 20 8
proc ArenaServers_MenuDraw 0 4
ADDRGP4 g_arenaservers+21504
INDIRI4
CNSTI4 0
EQI4 $340
ADDRGP4 ArenaServers_DoRefresh
CALLV
pop
LABELV $340
ADDRGP4 g_arenaservers
ARGP4
ADDRGP4 Menu_Draw
CALLV
pop
LABELV $339
endproc ArenaServers_MenuDraw 0 4
bss
align 1
LABELV $344
skip 64
code
proc ArenaServers_MenuInit 24 12
ADDRGP4 g_arenaservers
ARGP4
CNSTI4 0
ARGI4
CNSTI4 21520
ARGI4
ADDRGP4 memset
CALLP4
pop
ADDRGP4 $345
ARGP4
ADDRGP4 trap_R_RegisterShaderNoMip
CALLI4
pop
ADDRGP4 g_arenaservers+532
CNSTI4 1
ASGNI4
ADDRGP4 g_arenaservers+524
ADDRGP4 ArenaServers_MenuDraw
ASGNP4
ADDRGP4 g_arenaservers+536
CNSTI4 9
ASGNI4
ADDRGP4 g_arenaservers+536+48
CNSTI4 320
ASGNI4
ADDRGP4 g_arenaservers+536+52
CNSTI4 15
ASGNI4
ADDRGP4 g_arenaservers+536+108
ADDRGP4 $344
ASGNP4
ADDRGP4 g_arenaservers+536+112
CNSTI4 1
ASGNI4
ADDRGP4 g_arenaservers+536+124
ADDRGP4 color_white
ASGNP4
ADDRGP4 g_arenaservers+1024
CNSTI4 3
ASGNI4
ADDRGP4 g_arenaservers+1024+80
CNSTU4 32
ASGNU4
ADDRGP4 g_arenaservers+1024+96
ADDRGP4 ArenaServers_Event
ASGNP4
ADDRGP4 g_arenaservers+1024+40
CNSTI4 10
ASGNI4
ADDRGP4 g_arenaservers+1024+108
ADDRGP4 $368
ASGNP4
ADDRGP4 g_arenaservers+1024+172
ADDRGP4 master_items
ASGNP4
ADDRGP4 g_arenaservers+1024+48
CNSTI4 160
ASGNI4
ADDRGP4 g_arenaservers+1024+52
CNSTI4 32
ASGNI4
ADDRGP4 g_arenaservers+1512
CNSTI4 4
ASGNI4
ADDRGP4 g_arenaservers+1512+108
ADDRGP4 $378
ASGNP4
ADDRGP4 g_arenaservers+1512+80
CNSTU4 32
ASGNU4
ADDRGP4 g_arenaservers+1512+48
CNSTI4 160
ASGNI4
ADDRGP4 g_arenaservers+1512+52
CNSTI4 44
ASGNI4
ADDRGP4 g_arenaservers+1512+200+8
CNSTI4 38
ASGNI4
ADDRGP4 g_arenaservers+1512+200+268
CNSTI4 80
ASGNI4
ADDRGP4 g_arenaservers+2000
CNSTI4 8
ASGNI4
ADDRGP4 g_arenaservers+2000+108
ADDRGP4 $394
ASGNP4
ADDRGP4 g_arenaservers+2000+80
CNSTU4 32
ASGNU4
ADDRGP4 g_arenaservers+2000+96
ADDRGP4 ArenaServers_Event
ASGNP4
ADDRGP4 g_arenaservers+2000+40
CNSTI4 11
ASGNI4
ADDRGP4 g_arenaservers+2000+48
CNSTI4 160
ASGNI4
ADDRGP4 g_arenaservers+2000+52
CNSTI4 56
ASGNI4
ADDRGP4 g_arenaservers+2000+116
CNSTF4 1065353216
ASGNF4
ADDRGP4 g_arenaservers+2000+124
ADDRGP4 color_white
ASGNP4
ADDRLP4 12
ADDRLP4 12
INDIRI4
CNSTI4 33
ADDI4
ASGNI4
ADDRGP4 g_arenaservers+2488
CNSTI4 7
ASGNI4
ADDRGP4 g_arenaservers+2488+80
CNSTU4 16
ASGNU4
ADDRGP4 g_arenaservers+2488+40
CNSTI4 12
ASGNI4
ADDRGP4 g_arenaservers+2488+96
ADDRGP4 ArenaServers_Event
ASGNP4
ADDRGP4 g_arenaservers+2488+48
CNSTI4 0
ASGNI4
ADDRGP4 g_arenaservers+2488+52
CNSTI4 88
ASGNI4
ADDRGP4 g_arenaservers+2488+148
CNSTI4 111
ASGNI4
ADDRGP4 g_arenaservers+2488+152
CNSTI4 30
ASGNI4
ADDRGP4 g_arenaservers+2488+172
ADDRGP4 g_arenaservers+20976
ASGNP4
ADDRLP4 0
CNSTI4 0
ASGNI4
LABELV $427
ADDRLP4 0
INDIRI4
CNSTI4 2
LSHI4
ADDRGP4 g_arenaservers+20976
ADDP4
CNSTI4 116
ADDRLP4 0
INDIRI4
MULI4
ADDRGP4 g_arenaservers+6128
ADDP4
ASGNP4
LABELV $428
ADDRLP4 0
ADDRLP4 0
INDIRI4
CNSTI4 1
ADDI4
ASGNI4
ADDRLP4 0
INDIRI4
CNSTI4 128
LTI4 $427
ADDRGP4 g_arenaservers+2976
CNSTI4 6
ASGNI4
ADDRGP4 g_arenaservers+2976+80
CNSTU4 513
ASGNU4
ADDRGP4 g_arenaservers+2976+48
CNSTI4 0
ASGNI4
ADDRGP4 g_arenaservers+2976+52
CNSTI4 8
ASGNI4
ADDRGP4 g_arenaservers+2976+148
CNSTI4 100
ASGNI4
ADDRGP4 g_arenaservers+2976+152
CNSTI4 75
ASGNI4
ADDRGP4 g_arenaservers+2976+136
ADDRGP4 $345
ASGNP4
ADDRGP4 g_arenaservers+3464
CNSTI4 8
ASGNI4
ADDRGP4 g_arenaservers+3464+108
ADDRGP4 $449
ASGNP4
ADDRGP4 g_arenaservers+3464+80
CNSTU4 34
ASGNU4
ADDRGP4 g_arenaservers+3464+96
ADDRGP4 ArenaServers_Event
ASGNP4
ADDRGP4 g_arenaservers+3464+40
CNSTI4 13
ASGNI4
ADDRGP4 g_arenaservers+3464+48
CNSTI4 320
ASGNI4
ADDRGP4 g_arenaservers+3464+52
CNSTI4 450
ASGNI4
ADDRGP4 g_arenaservers+3464+116
CNSTF4 1069547520
ASGNF4
ADDRGP4 g_arenaservers+3464+112
CNSTI4 1
ASGNI4
ADDRGP4 g_arenaservers+3464+124
ADDRGP4 color_white
ASGNP4
ADDRGP4 g_arenaservers
ARGP4
ADDRGP4 g_arenaservers+536
ARGP4
ADDRGP4 Menu_AddItem
CALLV
pop
ADDRGP4 g_arenaservers
ARGP4
ADDRGP4 g_arenaservers+1024
ARGP4
ADDRGP4 Menu_AddItem
CALLV
pop
ADDRGP4 g_arenaservers
ARGP4
ADDRGP4 g_arenaservers+1512
ARGP4
ADDRGP4 Menu_AddItem
CALLV
pop
ADDRGP4 g_arenaservers
ARGP4
ADDRGP4 g_arenaservers+2000
ARGP4
ADDRGP4 Menu_AddItem
CALLV
pop
ADDRGP4 g_arenaservers
ARGP4
ADDRGP4 g_arenaservers+2488
ARGP4
ADDRGP4 Menu_AddItem
CALLV
pop
ADDRGP4 g_arenaservers
ARGP4
ADDRGP4 g_arenaservers+2976
ARGP4
ADDRGP4 Menu_AddItem
CALLV
pop
ADDRGP4 g_arenaservers
ARGP4
ADDRGP4 g_arenaservers+3464
ARGP4
ADDRGP4 Menu_AddItem
CALLV
pop
ADDRLP4 16
ADDRGP4 g_servertype
ASGNP4
ADDRLP4 16
INDIRP4
CNSTI4 0
ASGNI4
ADDRLP4 4
ADDRLP4 16
INDIRP4
INDIRI4
ASGNI4
ADDRGP4 g_arenaservers+1024+156
ADDRLP4 4
INDIRI4
ASGNI4
ADDRLP4 20
ADDRGP4 g_servertype
ASGNP4
ADDRLP4 8
ADDRLP4 20
INDIRP4
INDIRI4
ASGNI4
ADDRLP4 20
INDIRP4
CNSTI4 -1
ASGNI4
ADDRLP4 8
INDIRI4
ARGI4
ADDRGP4 ArenaServers_SetType
CALLV
pop
LABELV $343
endproc ArenaServers_MenuInit 24 12
export UI_ArenaServersMenu
proc UI_ArenaServersMenu 0 4
ADDRGP4 ArenaServers_MenuInit
CALLV
pop
ADDRGP4 g_arenaservers
ARGP4
ADDRGP4 UI_PushMenu
CALLV
pop
LABELV $475
endproc UI_ArenaServersMenu 0 4
bss
align 4
LABELV g_servertype
skip 4
align 4
LABELV g_numlocalservers
skip 4
align 4
LABELV g_localserverlist
skip 24064
align 4
LABELV g_numglobalservers
skip 4
align 4
LABELV g_globalserverlist
skip 24064
align 4
LABELV g_arenaservers
skip 21520
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
LABELV $449
byte 1 82
byte 1 101
byte 1 102
byte 1 114
byte 1 101
byte 1 115
byte 1 104
byte 1 0
align 1
LABELV $394
byte 1 67
byte 1 111
byte 1 110
byte 1 110
byte 1 101
byte 1 99
byte 1 116
byte 1 0
align 1
LABELV $378
byte 1 65
byte 1 100
byte 1 100
byte 1 114
byte 1 101
byte 1 115
byte 1 115
byte 1 58
byte 1 0
align 1
LABELV $368
byte 1 83
byte 1 101
byte 1 114
byte 1 118
byte 1 101
byte 1 114
byte 1 115
byte 1 58
byte 1 0
align 1
LABELV $345
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
LABELV $326
byte 1 37
byte 1 102
byte 1 0
align 1
LABELV $325
byte 1 117
byte 1 105
byte 1 95
byte 1 98
byte 1 114
byte 1 111
byte 1 119
byte 1 115
byte 1 101
byte 1 114
byte 1 77
byte 1 97
byte 1 115
byte 1 116
byte 1 101
byte 1 114
byte 1 0
align 1
LABELV $316
byte 1 104
byte 1 105
byte 1 116
byte 1 32
byte 1 82
byte 1 101
byte 1 102
byte 1 114
byte 1 101
byte 1 115
byte 1 104
byte 1 32
byte 1 116
byte 1 111
byte 1 32
byte 1 117
byte 1 112
byte 1 100
byte 1 97
byte 1 116
byte 1 101
byte 1 0
align 1
LABELV $296
byte 1 37
byte 1 105
byte 1 0
align 1
LABELV $292
byte 1 112
byte 1 114
byte 1 111
byte 1 116
byte 1 111
byte 1 99
byte 1 111
byte 1 108
byte 1 0
align 1
LABELV $291
byte 1 103
byte 1 108
byte 1 111
byte 1 98
byte 1 97
byte 1 108
byte 1 115
byte 1 101
byte 1 114
byte 1 118
byte 1 101
byte 1 114
byte 1 115
byte 1 32
byte 1 37
byte 1 100
byte 1 32
byte 1 37
byte 1 100
byte 1 37
byte 1 115
byte 1 10
byte 1 0
align 1
LABELV $288
byte 1 108
byte 1 111
byte 1 99
byte 1 97
byte 1 108
byte 1 115
byte 1 101
byte 1 114
byte 1 118
byte 1 101
byte 1 114
byte 1 115
byte 1 10
byte 1 0
align 1
LABELV $266
byte 1 112
byte 1 105
byte 1 110
byte 1 103
byte 1 32
byte 1 37
byte 1 115
byte 1 10
byte 1 0
align 1
LABELV $188
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
LABELV $187
byte 1 110
byte 1 101
byte 1 116
byte 1 116
byte 1 121
byte 1 112
byte 1 101
byte 1 0
align 1
LABELV $186
byte 1 109
byte 1 97
byte 1 120
byte 1 80
byte 1 105
byte 1 110
byte 1 103
byte 1 0
align 1
LABELV $185
byte 1 109
byte 1 105
byte 1 110
byte 1 80
byte 1 105
byte 1 110
byte 1 103
byte 1 0
align 1
LABELV $184
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
LABELV $183
byte 1 99
byte 1 108
byte 1 105
byte 1 101
byte 1 110
byte 1 116
byte 1 115
byte 1 0
align 1
LABELV $182
byte 1 97
byte 1 100
byte 1 100
byte 1 111
byte 1 110
byte 1 110
byte 1 97
byte 1 109
byte 1 101
byte 1 0
align 1
LABELV $181
byte 1 109
byte 1 97
byte 1 112
byte 1 110
byte 1 97
byte 1 109
byte 1 101
byte 1 0
align 1
LABELV $180
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
LABELV $161
byte 1 37
byte 1 45
byte 1 50
byte 1 48
byte 1 46
byte 1 50
byte 1 48
byte 1 115
byte 1 32
byte 1 37
byte 1 45
byte 1 50
byte 1 48
byte 1 46
byte 1 50
byte 1 48
byte 1 115
byte 1 32
byte 1 37
byte 1 45
byte 1 50
byte 1 48
byte 1 46
byte 1 50
byte 1 48
byte 1 115
byte 1 32
byte 1 37
byte 1 50
byte 1 100
byte 1 47
byte 1 37
byte 1 50
byte 1 100
byte 1 32
byte 1 37
byte 1 45
byte 1 50
byte 1 48
byte 1 46
byte 1 50
byte 1 48
byte 1 115
byte 1 32
byte 1 37
byte 1 52
byte 1 115
byte 1 32
byte 1 37
byte 1 49
byte 1 48
byte 1 100
byte 1 0
align 1
LABELV $137
byte 1 78
byte 1 111
byte 1 32
byte 1 83
byte 1 101
byte 1 114
byte 1 118
byte 1 101
byte 1 114
byte 1 115
byte 1 32
byte 1 70
byte 1 111
byte 1 117
byte 1 110
byte 1 100
byte 1 46
byte 1 0
align 1
LABELV $134
byte 1 78
byte 1 111
byte 1 32
byte 1 82
byte 1 101
byte 1 115
byte 1 112
byte 1 111
byte 1 110
byte 1 115
byte 1 101
byte 1 32
byte 1 70
byte 1 114
byte 1 111
byte 1 109
byte 1 32
byte 1 77
byte 1 97
byte 1 115
byte 1 116
byte 1 101
byte 1 114
byte 1 32
byte 1 83
byte 1 101
byte 1 114
byte 1 118
byte 1 101
byte 1 114
byte 1 46
byte 1 0
align 1
LABELV $122
byte 1 83
byte 1 99
byte 1 97
byte 1 110
byte 1 110
byte 1 105
byte 1 110
byte 1 103
byte 1 32
byte 1 70
byte 1 111
byte 1 114
byte 1 32
byte 1 83
byte 1 101
byte 1 114
byte 1 118
byte 1 101
byte 1 114
byte 1 115
byte 1 46
byte 1 46
byte 1 46
byte 1 0
align 1
LABELV $108
byte 1 37
byte 1 100
byte 1 32
byte 1 111
byte 1 102
byte 1 32
byte 1 37
byte 1 100
byte 1 32
byte 1 83
byte 1 101
byte 1 114
byte 1 118
byte 1 101
byte 1 114
byte 1 115
byte 1 46
byte 1 0
align 1
LABELV $92
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
LABELV $79
byte 1 99
byte 1 111
byte 1 110
byte 1 110
byte 1 101
byte 1 99
byte 1 116
byte 1 32
byte 1 37
byte 1 115
byte 1 10
byte 1 0
align 1
LABELV $78
byte 1 99
byte 1 108
byte 1 95
byte 1 115
byte 1 101
byte 1 108
byte 1 101
byte 1 99
byte 1 116
byte 1 101
byte 1 100
byte 1 109
byte 1 111
byte 1 100
byte 1 32
byte 1 61
byte 1 32
byte 1 37
byte 1 115
byte 1 10
byte 1 0
align 1
LABELV $68
byte 1 99
byte 1 108
byte 1 95
byte 1 109
byte 1 97
byte 1 120
byte 1 80
byte 1 105
byte 1 110
byte 1 103
byte 1 0
align 1
LABELV $63
byte 1 73
byte 1 80
byte 1 118
byte 1 54
byte 1 0
align 1
LABELV $62
byte 1 73
byte 1 80
byte 1 118
byte 1 52
byte 1 0
align 1
LABELV $61
byte 1 63
byte 1 63
byte 1 63
byte 1 63
byte 1 0
align 1
LABELV $60
byte 1 73
byte 1 110
byte 1 116
byte 1 101
byte 1 114
byte 1 110
byte 1 101
byte 1 116
byte 1 0
align 1
LABELV $59
byte 1 76
byte 1 111
byte 1 99
byte 1 97
byte 1 108
byte 1 0
