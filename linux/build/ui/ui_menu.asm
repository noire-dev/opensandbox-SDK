code
proc Main_MenuEvent 16 4
ADDRFP4 4
INDIRI4
CNSTI4 3
EQI4 $61
ADDRGP4 $60
JUMPV
LABELV $61
ADDRLP4 0
ADDRFP4 0
INDIRP4
CNSTI4 44
ADDP4
INDIRI4
ASGNI4
ADDRLP4 0
INDIRI4
CNSTI4 100
EQI4 $66
ADDRGP4 $63
JUMPV
LABELV $66
ADDRGP4 $69
ARGP4
ADDRLP4 4
ADDRGP4 cvarInt
CALLI4
ASGNI4
ADDRLP4 4
INDIRI4
CNSTI4 0
EQI4 $67
ADDRGP4 $70
ARGP4
ADDRGP4 trap_System
CALLV
pop
LABELV $67
ADDRGP4 $73
ARGP4
ADDRLP4 8
ADDRGP4 cvarInt
CALLI4
ASGNI4
ADDRLP4 8
INDIRI4
CNSTI4 0
EQI4 $71
ADDRGP4 $74
ARGP4
ADDRGP4 trap_System
CALLV
pop
LABELV $71
ADDRGP4 $77
ARGP4
ADDRLP4 12
ADDRGP4 cvarInt
CALLI4
ASGNI4
ADDRLP4 12
INDIRI4
CNSTI4 0
EQI4 $64
ADDRGP4 $78
ARGP4
ADDRGP4 trap_System
CALLV
pop
LABELV $63
LABELV $64
LABELV $60
endproc Main_MenuEvent 16 4
lit
align 4
LABELV $80
byte 4 1062836634
byte 4 1063675494
byte 4 1065353216
byte 4 1065353216
align 4
LABELV $81
byte 4 1062836634
byte 4 1063675494
byte 4 1065353216
byte 4 1045220557
code
proc Main_MenuDraw 52 24
ADDRLP4 24
ADDRGP4 $80
INDIRB
ASGNB 16
ADDRLP4 8
ADDRGP4 $81
INDIRB
ASGNB 16
ADDRGP4 main
ARGP4
ADDRGP4 Menu_Draw
CALLV
pop
ADDRGP4 main+49824
INDIRP4
ARGP4
ADDRLP4 40
ADDRGP4 strlen
CALLI4
ASGNI4
ADDRLP4 40
INDIRI4
CNSTI4 0
EQI4 $82
ADDRLP4 44
CNSTF4 0
ASGNF4
ADDRLP4 44
INDIRF4
ADDRGP4 uis+92
INDIRF4
SUBF4
ARGF4
ADDRLP4 44
INDIRF4
ARGF4
ADDRGP4 main+49824
INDIRP4
ARGP4
CNSTI4 16
ARGI4
ADDRLP4 24
ARGP4
CNSTF4 1065353216
ARGF4
ADDRGP4 ST_DrawString
CALLV
pop
LABELV $82
ADDRGP4 uis+92
INDIRF4
CNSTF4 1142865920
ADDF4
ARGF4
CNSTF4 1073741824
ARGF4
ADDRGP4 $88
ARGP4
CNSTI4 2
ARGI4
ADDRLP4 24
ARGP4
CNSTF4 1065353216
ARGF4
ADDRGP4 ST_DrawString
CALLV
pop
ADDRLP4 48
CNSTF4 1092616192
ASGNF4
ADDRLP4 48
INDIRF4
ADDRGP4 uis+92
INDIRF4
SUBF4
ARGF4
ADDRLP4 48
INDIRF4
ARGF4
CNSTF4 1124532224
ARGF4
CNSTI4 20
ADDRGP4 mainmenu_items
INDIRI4
MULI4
CNSTI4 6
ADDI4
CVIF4 4
ARGF4
CNSTF4 1082130432
ARGF4
ADDRGP4 color_dim
ARGP4
ADDRGP4 UI_DrawRoundedRect
CALLV
pop
ADDRLP4 0
CNSTI4 15
ASGNI4
ADDRLP4 4
CNSTI4 0
ASGNI4
ADDRGP4 $93
JUMPV
LABELV $90
CNSTF4 1097859072
ADDRGP4 uis+92
INDIRF4
SUBF4
ARGF4
ADDRLP4 0
INDIRI4
CVIF4 4
ARGF4
CNSTF4 1123680256
ARGF4
CNSTF4 1098907648
ARGF4
CNSTF4 1077936128
ARGF4
ADDRLP4 8
ARGP4
ADDRGP4 UI_DrawRoundedRect
CALLV
pop
ADDRLP4 0
ADDRLP4 0
INDIRI4
CNSTI4 20
ADDI4
ASGNI4
LABELV $91
ADDRLP4 4
ADDRLP4 4
INDIRI4
CNSTI4 1
ADDI4
ASGNI4
LABELV $93
ADDRLP4 4
INDIRI4
ADDRGP4 mainmenu_items
INDIRI4
LTI4 $90
ADDRGP4 main
ARGP4
ADDRGP4 Menu_Draw
CALLV
pop
LABELV $79
endproc Main_MenuDraw 52 24
export UI_MainMenu
proc UI_MainMenu 96 48
ADDRGP4 main
ARGP4
CNSTI4 0
ARGI4
CNSTI4 49828
ARGI4
ADDRGP4 memset
CALLP4
pop
ADDRGP4 $97
ARGP4
ADDRLP4 8
ADDRGP4 cvarString
CALLP4
ASGNP4
ADDRGP4 main+49824
ADDRLP4 8
INDIRP4
ASGNP4
ADDRGP4 main+524
ADDRGP4 Main_MenuDraw
ASGNP4
ADDRGP4 main+532
CNSTI4 1
ASGNI4
ADDRLP4 4
CNSTI4 15
ASGNI4
ADDRLP4 12
CNSTI4 0
ASGNI4
ADDRGP4 mainmenu_items
ADDRLP4 12
INDIRI4
ASGNI4
ADDRGP4 uis+100
INDIRI4
ADDRLP4 12
INDIRI4
EQI4 $100
ADDRGP4 main+536
ARGP4
CNSTF4 1099956224
ADDRGP4 uis+92
INDIRF4
SUBF4
ARGF4
ADDRLP4 4
INDIRI4
CNSTI4 4
ADDI4
CVIF4 4
ARGF4
ADDRGP4 $105
ARGP4
ADDRLP4 16
CNSTI4 0
ASGNI4
ADDRLP4 16
INDIRI4
ARGI4
CNSTF4 1063675494
ARGF4
ADDRGP4 color_white
ARGP4
ADDRLP4 20
CNSTP4 0
ASGNP4
ADDRLP4 20
INDIRP4
ARGP4
ADDRLP4 20
INDIRP4
ARGP4
ADDRGP4 UI_ForceMenuOff
ARGP4
CNSTP4 0
ARGP4
ADDRLP4 16
INDIRI4
ARGI4
ADDRGP4 UI_CButton
CALLV
pop
ADDRLP4 4
ADDRLP4 4
INDIRI4
CNSTI4 20
ADDI4
ASGNI4
ADDRLP4 24
ADDRGP4 mainmenu_items
ASGNP4
ADDRLP4 24
INDIRP4
ADDRLP4 24
INDIRP4
INDIRI4
CNSTI4 1
ADDI4
ASGNI4
LABELV $100
ADDRGP4 main+536+488
ARGP4
CNSTF4 1099956224
ADDRGP4 uis+92
INDIRF4
SUBF4
ARGF4
ADDRLP4 4
INDIRI4
CNSTI4 4
ADDI4
CVIF4 4
ARGF4
ADDRGP4 $109
ARGP4
ADDRLP4 16
CNSTI4 0
ASGNI4
ADDRLP4 16
INDIRI4
ARGI4
CNSTF4 1063675494
ARGF4
ADDRGP4 color_white
ARGP4
ADDRLP4 20
CNSTP4 0
ASGNP4
ADDRLP4 20
INDIRP4
ARGP4
ADDRLP4 20
INDIRP4
ARGP4
ADDRGP4 UI_NewGame
ARGP4
CNSTP4 0
ARGP4
ADDRLP4 16
INDIRI4
ARGI4
ADDRGP4 UI_CButton
CALLV
pop
ADDRLP4 4
ADDRLP4 4
INDIRI4
CNSTI4 20
ADDI4
ASGNI4
ADDRLP4 24
ADDRGP4 mainmenu_items
ASGNP4
ADDRLP4 24
INDIRP4
ADDRLP4 24
INDIRP4
INDIRI4
CNSTI4 1
ADDI4
ASGNI4
ADDRGP4 main+536+976
ARGP4
CNSTF4 1099956224
ADDRGP4 uis+92
INDIRF4
SUBF4
ARGF4
ADDRLP4 4
INDIRI4
CNSTI4 4
ADDI4
CVIF4 4
ARGF4
ADDRGP4 $113
ARGP4
ADDRLP4 28
CNSTI4 0
ASGNI4
ADDRLP4 28
INDIRI4
ARGI4
CNSTF4 1063675494
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
ADDRGP4 UI_ArenaServersMenu
ARGP4
CNSTP4 0
ARGP4
ADDRLP4 28
INDIRI4
ARGI4
ADDRGP4 UI_CButton
CALLV
pop
ADDRLP4 4
ADDRLP4 4
INDIRI4
CNSTI4 20
ADDI4
ASGNI4
ADDRLP4 36
ADDRGP4 mainmenu_items
ASGNP4
ADDRLP4 36
INDIRP4
ADDRLP4 36
INDIRP4
INDIRI4
CNSTI4 1
ADDI4
ASGNI4
ADDRGP4 main+536+1464
ARGP4
CNSTF4 1099956224
ADDRGP4 uis+92
INDIRF4
SUBF4
ARGF4
ADDRLP4 4
INDIRI4
CNSTI4 4
ADDI4
CVIF4 4
ARGF4
ADDRGP4 $117
ARGP4
ADDRLP4 40
CNSTI4 0
ASGNI4
ADDRLP4 40
INDIRI4
ARGI4
CNSTF4 1063675494
ARGF4
ADDRGP4 color_white
ARGP4
ADDRLP4 44
CNSTP4 0
ASGNP4
ADDRLP4 44
INDIRP4
ARGP4
ADDRLP4 44
INDIRP4
ARGP4
ADDRGP4 UI_PlayerModelMenu
ARGP4
CNSTP4 0
ARGP4
ADDRLP4 40
INDIRI4
ARGI4
ADDRGP4 UI_CButton
CALLV
pop
ADDRLP4 4
ADDRLP4 4
INDIRI4
CNSTI4 20
ADDI4
ASGNI4
ADDRLP4 48
ADDRGP4 mainmenu_items
ASGNP4
ADDRLP4 48
INDIRP4
ADDRLP4 48
INDIRP4
INDIRI4
CNSTI4 1
ADDI4
ASGNI4
ADDRGP4 main+536+1952
ARGP4
CNSTF4 1099956224
ADDRGP4 uis+92
INDIRF4
SUBF4
ARGF4
ADDRLP4 4
INDIRI4
CNSTI4 4
ADDI4
CVIF4 4
ARGF4
ADDRGP4 $121
ARGP4
ADDRLP4 52
CNSTI4 0
ASGNI4
ADDRLP4 52
INDIRI4
ARGI4
CNSTF4 1063675494
ARGF4
ADDRGP4 color_white
ARGP4
ADDRLP4 56
CNSTP4 0
ASGNP4
ADDRLP4 56
INDIRP4
ARGP4
ADDRLP4 56
INDIRP4
ARGP4
ADDRGP4 UI_Options
ARGP4
CNSTP4 0
ARGP4
ADDRLP4 52
INDIRI4
ARGI4
ADDRGP4 UI_CButton
CALLV
pop
ADDRLP4 4
ADDRLP4 4
INDIRI4
CNSTI4 20
ADDI4
ASGNI4
ADDRLP4 60
ADDRGP4 mainmenu_items
ASGNP4
ADDRLP4 60
INDIRP4
ADDRLP4 60
INDIRP4
INDIRI4
CNSTI4 1
ADDI4
ASGNI4
ADDRGP4 uis+100
INDIRI4
CNSTI4 0
EQI4 $122
ADDRGP4 main+536+2440
ARGP4
CNSTF4 1099956224
ADDRGP4 uis+92
INDIRF4
SUBF4
ARGF4
ADDRLP4 4
INDIRI4
CNSTI4 4
ADDI4
CVIF4 4
ARGF4
ADDRGP4 $128
ARGP4
ADDRLP4 64
CNSTI4 0
ASGNI4
ADDRLP4 64
INDIRI4
ARGI4
CNSTF4 1063675494
ARGF4
ADDRGP4 color_white
ARGP4
ADDRGP4 $129
ARGP4
CNSTP4 0
ARGP4
CNSTP4 0
ARGP4
CNSTP4 0
ARGP4
ADDRLP4 64
INDIRI4
ARGI4
ADDRGP4 UI_CButton
CALLV
pop
ADDRLP4 4
ADDRLP4 4
INDIRI4
CNSTI4 20
ADDI4
ASGNI4
ADDRLP4 68
ADDRGP4 mainmenu_items
ASGNP4
ADDRLP4 68
INDIRP4
ADDRLP4 68
INDIRP4
INDIRI4
CNSTI4 1
ADDI4
ASGNI4
LABELV $122
ADDRGP4 main+536+2928
ARGP4
CNSTF4 1099956224
ADDRGP4 uis+92
INDIRF4
SUBF4
ARGF4
ADDRLP4 4
INDIRI4
CNSTI4 4
ADDI4
CVIF4 4
ARGF4
ADDRGP4 $133
ARGP4
ADDRLP4 64
CNSTI4 0
ASGNI4
ADDRLP4 64
INDIRI4
ARGI4
CNSTF4 1063675494
ARGF4
ADDRGP4 color_white
ARGP4
ADDRGP4 $134
ARGP4
CNSTP4 0
ARGP4
CNSTP4 0
ARGP4
CNSTP4 0
ARGP4
ADDRLP4 64
INDIRI4
ARGI4
ADDRGP4 UI_CButton
CALLV
pop
ADDRLP4 68
ADDRGP4 mainmenu_items
ASGNP4
ADDRLP4 68
INDIRP4
ADDRLP4 68
INDIRP4
INDIRI4
CNSTI4 1
ADDI4
ASGNI4
ADDRGP4 uis+100
INDIRI4
CNSTI4 0
EQI4 $135
ADDRLP4 4
CNSTI4 81
ASGNI4
ADDRGP4 main+536+9760
ARGP4
ADDRGP4 uis+92
INDIRF4
CNSTF4 1142784000
ADDF4
ARGF4
ADDRLP4 4
INDIRI4
CVIF4 4
ARGF4
ADDRGP4 $141
ARGP4
CNSTI4 2
ARGI4
CNSTF4 1065353216
ARGF4
ADDRGP4 color_white
ARGP4
ADDRGP4 $142
ARGP4
CNSTP4 0
ARGP4
CNSTP4 0
ARGP4
CNSTP4 0
ARGP4
CNSTI4 0
ARGI4
ADDRGP4 UI_CButton
CALLV
pop
ADDRLP4 4
ADDRLP4 4
INDIRI4
CNSTI4 18
ADDI4
ASGNI4
ADDRGP4 main+536+10248
ARGP4
ADDRGP4 uis+92
INDIRF4
CNSTF4 1142784000
ADDF4
ARGF4
ADDRLP4 4
INDIRI4
CVIF4 4
ARGF4
ADDRGP4 $146
ARGP4
CNSTI4 2
ARGI4
CNSTF4 1065353216
ARGF4
ADDRGP4 color_white
ARGP4
ADDRGP4 $147
ARGP4
CNSTP4 0
ARGP4
CNSTP4 0
ARGP4
CNSTP4 0
ARGP4
CNSTI4 0
ARGI4
ADDRGP4 UI_CButton
CALLV
pop
ADDRLP4 4
ADDRLP4 4
INDIRI4
CNSTI4 18
ADDI4
ASGNI4
ADDRGP4 main+536+10736
ARGP4
ADDRGP4 uis+92
INDIRF4
CNSTF4 1142784000
ADDF4
ARGF4
ADDRLP4 4
INDIRI4
CVIF4 4
ARGF4
ADDRGP4 $151
ARGP4
CNSTI4 2
ARGI4
CNSTF4 1065353216
ARGF4
ADDRGP4 color_white
ARGP4
ADDRGP4 $152
ARGP4
CNSTP4 0
ARGP4
CNSTP4 0
ARGP4
CNSTP4 0
ARGP4
CNSTI4 0
ARGI4
ADDRGP4 UI_CButton
CALLV
pop
ADDRLP4 4
ADDRLP4 4
INDIRI4
CNSTI4 18
ADDI4
ASGNI4
ADDRGP4 main+536+11224
ARGP4
ADDRGP4 uis+92
INDIRF4
CNSTF4 1142784000
ADDF4
ARGF4
ADDRLP4 4
INDIRI4
CVIF4 4
ARGF4
ADDRGP4 $156
ARGP4
CNSTI4 2
ARGI4
CNSTF4 1065353216
ARGF4
ADDRGP4 color_white
ARGP4
ADDRGP4 $157
ARGP4
CNSTP4 0
ARGP4
CNSTP4 0
ARGP4
CNSTP4 0
ARGP4
CNSTI4 0
ARGI4
ADDRGP4 UI_CButton
CALLV
pop
ADDRLP4 4
ADDRLP4 4
INDIRI4
CNSTI4 36
ADDI4
ASGNI4
ADDRGP4 main+536+11712
ARGP4
ADDRGP4 uis+92
INDIRF4
CNSTF4 1142784000
ADDF4
ARGF4
ADDRLP4 4
INDIRI4
CVIF4 4
ARGF4
ADDRGP4 $161
ARGP4
CNSTI4 2
ARGI4
CNSTF4 1065353216
ARGF4
ADDRGP4 color_white
ARGP4
ADDRGP4 $162
ARGP4
CNSTP4 0
ARGP4
CNSTP4 0
ARGP4
CNSTP4 0
ARGP4
CNSTI4 0
ARGI4
ADDRGP4 UI_CButton
CALLV
pop
ADDRLP4 4
ADDRLP4 4
INDIRI4
CNSTI4 36
ADDI4
ASGNI4
ADDRGP4 $168
ARGP4
ADDRLP4 72
ADDRGP4 cvarString
CALLP4
ASGNP4
ADDRGP4 $169
ARGP4
ADDRLP4 76
ADDRGP4 cvarString
CALLP4
ASGNP4
ADDRGP4 $167
ARGP4
ADDRLP4 72
INDIRP4
ARGP4
ADDRLP4 76
INDIRP4
ARGP4
ADDRLP4 80
ADDRGP4 va
CALLP4
ASGNP4
ADDRGP4 main+536+12688
ARGP4
ADDRGP4 uis+92
INDIRF4
CNSTF4 1142784000
ADDF4
ARGF4
ADDRLP4 4
INDIRI4
CVIF4 4
ARGF4
ADDRGP4 $166
ARGP4
CNSTI4 2
ARGI4
CNSTF4 1065353216
ARGF4
ADDRGP4 color_white
ARGP4
ADDRLP4 80
INDIRP4
ARGP4
CNSTP4 0
ARGP4
CNSTP4 0
ARGP4
CNSTP4 0
ARGP4
CNSTI4 0
ARGI4
ADDRGP4 UI_CButton
CALLV
pop
ADDRLP4 4
ADDRLP4 4
INDIRI4
CNSTI4 18
ADDI4
ASGNI4
ADDRGP4 $168
ARGP4
ADDRLP4 84
ADDRGP4 cvarString
CALLP4
ASGNP4
ADDRGP4 $169
ARGP4
ADDRLP4 88
ADDRGP4 cvarString
CALLP4
ASGNP4
ADDRGP4 $174
ARGP4
ADDRLP4 84
INDIRP4
ARGP4
ADDRLP4 88
INDIRP4
ARGP4
ADDRLP4 92
ADDRGP4 va
CALLP4
ASGNP4
ADDRGP4 main+536+13176
ARGP4
ADDRGP4 uis+92
INDIRF4
CNSTF4 1142784000
ADDF4
ARGF4
ADDRLP4 4
INDIRI4
CVIF4 4
ARGF4
ADDRGP4 $173
ARGP4
CNSTI4 2
ARGI4
CNSTF4 1065353216
ARGF4
ADDRGP4 color_white
ARGP4
ADDRLP4 92
INDIRP4
ARGP4
CNSTP4 0
ARGP4
CNSTP4 0
ARGP4
CNSTP4 0
ARGP4
CNSTI4 0
ARGI4
ADDRGP4 UI_CButton
CALLV
pop
ADDRLP4 4
ADDRLP4 4
INDIRI4
CNSTI4 18
ADDI4
ASGNI4
ADDRGP4 main+536+13664
ARGP4
ADDRGP4 uis+92
INDIRF4
CNSTF4 1142784000
ADDF4
ARGF4
ADDRLP4 4
INDIRI4
CVIF4 4
ARGF4
ADDRGP4 $178
ARGP4
CNSTI4 2
ARGI4
CNSTF4 1065353216
ARGF4
ADDRGP4 color_white
ARGP4
ADDRGP4 $179
ARGP4
CNSTP4 0
ARGP4
CNSTP4 0
ARGP4
CNSTP4 0
ARGP4
CNSTI4 0
ARGI4
ADDRGP4 UI_CButton
CALLV
pop
LABELV $135
ADDRGP4 main+536+48800
ARGP4
ADDRGP4 uis+92
INDIRF4
CNSTF4 1139310592
ADDF4
ARGF4
CNSTF4 1137508352
ARGF4
CNSTF4 1126039552
ARGF4
CNSTF4 1113325568
ARGF4
CNSTI4 1
ARGI4
CNSTI4 0
ARGI4
ADDRLP4 72
CNSTP4 0
ASGNP4
ADDRLP4 72
INDIRP4
ARGP4
ADDRLP4 72
INDIRP4
ARGP4
CNSTP4 0
ARGP4
ADDRGP4 Main_MenuEvent
ARGP4
CNSTI4 100
ARGI4
ADDRGP4 UI_CPicture
CALLV
pop
ADDRGP4 main
ARGP4
ADDRGP4 main+536
ARGP4
ADDRGP4 UI_CreateUI
CALLV
pop
ADDRLP4 4
CNSTI4 15
ASGNI4
ADDRLP4 0
CNSTI4 0
ASGNI4
LABELV $184
CNSTI4 488
ADDRLP4 0
INDIRI4
MULI4
ADDRGP4 main+536
ADDP4
ARGP4
CNSTF4 1097859072
ADDRGP4 uis+92
INDIRF4
SUBF4
ARGF4
ADDRLP4 4
INDIRI4
CVIF4 4
ARGF4
CNSTF4 1123680256
ARGF4
CNSTF4 1098907648
ARGF4
ADDRGP4 UI_SetHitbox
CALLV
pop
CNSTI4 488
ADDRLP4 0
INDIRI4
MULI4
ADDRGP4 main+536
ADDP4
INDIRI4
CNSTI4 0
EQI4 $190
ADDRLP4 4
ADDRLP4 4
INDIRI4
CNSTI4 20
ADDI4
ASGNI4
LABELV $190
LABELV $185
ADDRLP4 0
ADDRLP4 0
INDIRI4
CNSTI4 1
ADDI4
ASGNI4
ADDRLP4 0
INDIRI4
CNSTI4 6
LEI4 $184
ADDRGP4 uis+16
CNSTI4 0
ASGNI4
CNSTI4 2
ARGI4
ADDRGP4 trap_Key_SetCatcher
CALLV
pop
ADDRGP4 main
ARGP4
ADDRGP4 UI_PushMenu
CALLV
pop
LABELV $95
endproc UI_MainMenu 96 48
bss
export mainmenu_items
align 4
LABELV mainmenu_items
skip 4
align 4
LABELV main
skip 49828
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
LABELV $179
byte 1 99
byte 1 108
byte 1 101
byte 1 97
byte 1 114
byte 1 109
byte 1 97
byte 1 112
byte 1 59
byte 1 32
byte 1 109
byte 1 101
byte 1 110
byte 1 117
byte 1 98
byte 1 97
byte 1 99
byte 1 107
byte 1 10
byte 1 0
align 1
LABELV $178
byte 1 67
byte 1 108
byte 1 101
byte 1 97
byte 1 114
byte 1 32
byte 1 109
byte 1 97
byte 1 112
byte 1 0
align 1
LABELV $174
byte 1 100
byte 1 101
byte 1 108
byte 1 101
byte 1 116
byte 1 101
byte 1 109
byte 1 97
byte 1 112
byte 1 32
byte 1 109
byte 1 97
byte 1 112
byte 1 115
byte 1 47
byte 1 37
byte 1 115
byte 1 47
byte 1 37
byte 1 115
byte 1 46
byte 1 101
byte 1 110
byte 1 116
byte 1 59
byte 1 32
byte 1 109
byte 1 101
byte 1 110
byte 1 117
byte 1 98
byte 1 97
byte 1 99
byte 1 107
byte 1 59
byte 1 32
byte 1 119
byte 1 97
byte 1 105
byte 1 116
byte 1 32
byte 1 50
byte 1 53
byte 1 59
byte 1 32
byte 1 109
byte 1 97
byte 1 112
byte 1 95
byte 1 114
byte 1 101
byte 1 115
byte 1 116
byte 1 97
byte 1 114
byte 1 116
byte 1 10
byte 1 0
align 1
LABELV $173
byte 1 68
byte 1 101
byte 1 108
byte 1 101
byte 1 116
byte 1 101
byte 1 32
byte 1 109
byte 1 97
byte 1 112
byte 1 0
align 1
LABELV $169
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
LABELV $168
byte 1 103
byte 1 95
byte 1 101
byte 1 110
byte 1 116
byte 1 105
byte 1 116
byte 1 121
byte 1 112
byte 1 97
byte 1 99
byte 1 107
byte 1 0
align 1
LABELV $167
byte 1 115
byte 1 97
byte 1 118
byte 1 101
byte 1 109
byte 1 97
byte 1 112
byte 1 32
byte 1 109
byte 1 97
byte 1 112
byte 1 115
byte 1 47
byte 1 37
byte 1 115
byte 1 47
byte 1 37
byte 1 115
byte 1 46
byte 1 101
byte 1 110
byte 1 116
byte 1 59
byte 1 32
byte 1 109
byte 1 101
byte 1 110
byte 1 117
byte 1 98
byte 1 97
byte 1 99
byte 1 107
byte 1 10
byte 1 0
align 1
LABELV $166
byte 1 83
byte 1 97
byte 1 118
byte 1 101
byte 1 32
byte 1 109
byte 1 97
byte 1 112
byte 1 0
align 1
LABELV $162
byte 1 109
byte 1 97
byte 1 112
byte 1 95
byte 1 114
byte 1 101
byte 1 115
byte 1 116
byte 1 97
byte 1 114
byte 1 116
byte 1 59
byte 1 32
byte 1 109
byte 1 101
byte 1 110
byte 1 117
byte 1 98
byte 1 97
byte 1 99
byte 1 107
byte 1 10
byte 1 0
align 1
LABELV $161
byte 1 82
byte 1 101
byte 1 115
byte 1 116
byte 1 97
byte 1 114
byte 1 116
byte 1 0
align 1
LABELV $157
byte 1 116
byte 1 101
byte 1 97
byte 1 109
byte 1 32
byte 1 115
byte 1 59
byte 1 32
byte 1 109
byte 1 101
byte 1 110
byte 1 117
byte 1 98
byte 1 97
byte 1 99
byte 1 107
byte 1 10
byte 1 0
align 1
LABELV $156
byte 1 94
byte 1 50
byte 1 83
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
LABELV $152
byte 1 116
byte 1 101
byte 1 97
byte 1 109
byte 1 32
byte 1 102
byte 1 59
byte 1 32
byte 1 109
byte 1 101
byte 1 110
byte 1 117
byte 1 98
byte 1 97
byte 1 99
byte 1 107
byte 1 10
byte 1 0
align 1
LABELV $151
byte 1 94
byte 1 51
byte 1 70
byte 1 114
byte 1 101
byte 1 101
byte 1 32
byte 1 116
byte 1 101
byte 1 97
byte 1 109
byte 1 0
align 1
LABELV $147
byte 1 116
byte 1 101
byte 1 97
byte 1 109
byte 1 32
byte 1 98
byte 1 59
byte 1 32
byte 1 109
byte 1 101
byte 1 110
byte 1 117
byte 1 98
byte 1 97
byte 1 99
byte 1 107
byte 1 10
byte 1 0
align 1
LABELV $146
byte 1 94
byte 1 52
byte 1 66
byte 1 108
byte 1 117
byte 1 101
byte 1 32
byte 1 116
byte 1 101
byte 1 97
byte 1 109
byte 1 0
align 1
LABELV $142
byte 1 116
byte 1 101
byte 1 97
byte 1 109
byte 1 32
byte 1 114
byte 1 59
byte 1 32
byte 1 109
byte 1 101
byte 1 110
byte 1 117
byte 1 98
byte 1 97
byte 1 99
byte 1 107
byte 1 10
byte 1 0
align 1
LABELV $141
byte 1 94
byte 1 49
byte 1 82
byte 1 101
byte 1 100
byte 1 32
byte 1 116
byte 1 101
byte 1 97
byte 1 109
byte 1 0
align 1
LABELV $134
byte 1 113
byte 1 117
byte 1 105
byte 1 116
byte 1 32
byte 1 10
byte 1 0
align 1
LABELV $133
byte 1 81
byte 1 117
byte 1 105
byte 1 116
byte 1 0
align 1
LABELV $129
byte 1 100
byte 1 105
byte 1 115
byte 1 99
byte 1 111
byte 1 110
byte 1 110
byte 1 101
byte 1 99
byte 1 116
byte 1 32
byte 1 10
byte 1 0
align 1
LABELV $128
byte 1 68
byte 1 105
byte 1 115
byte 1 99
byte 1 111
byte 1 110
byte 1 110
byte 1 101
byte 1 99
byte 1 116
byte 1 0
align 1
LABELV $121
byte 1 79
byte 1 112
byte 1 116
byte 1 105
byte 1 111
byte 1 110
byte 1 115
byte 1 0
align 1
LABELV $117
byte 1 80
byte 1 114
byte 1 111
byte 1 102
byte 1 105
byte 1 108
byte 1 101
byte 1 0
align 1
LABELV $113
byte 1 70
byte 1 105
byte 1 110
byte 1 100
byte 1 32
byte 1 77
byte 1 117
byte 1 108
byte 1 116
byte 1 105
byte 1 112
byte 1 108
byte 1 97
byte 1 121
byte 1 101
byte 1 114
byte 1 32
byte 1 71
byte 1 97
byte 1 109
byte 1 101
byte 1 0
align 1
LABELV $109
byte 1 83
byte 1 116
byte 1 97
byte 1 114
byte 1 116
byte 1 32
byte 1 78
byte 1 101
byte 1 119
byte 1 32
byte 1 71
byte 1 97
byte 1 109
byte 1 101
byte 1 0
align 1
LABELV $105
byte 1 82
byte 1 101
byte 1 115
byte 1 117
byte 1 109
byte 1 101
byte 1 32
byte 1 103
byte 1 97
byte 1 109
byte 1 101
byte 1 0
align 1
LABELV $97
byte 1 99
byte 1 111
byte 1 109
byte 1 95
byte 1 101
byte 1 114
byte 1 114
byte 1 111
byte 1 114
byte 1 77
byte 1 101
byte 1 115
byte 1 115
byte 1 97
byte 1 103
byte 1 101
byte 1 0
align 1
LABELV $88
byte 1 50
byte 1 48
byte 1 50
byte 1 53
byte 1 46
byte 1 49
byte 1 48
byte 1 46
byte 1 50
byte 1 54
byte 1 0
align 1
LABELV $78
byte 1 111
byte 1 112
byte 1 101
byte 1 110
byte 1 32
byte 1 104
byte 1 116
byte 1 116
byte 1 112
byte 1 115
byte 1 58
byte 1 47
byte 1 47
byte 1 111
byte 1 112
byte 1 101
byte 1 110
byte 1 115
byte 1 97
byte 1 110
byte 1 100
byte 1 98
byte 1 111
byte 1 120
byte 1 46
byte 1 110
byte 1 101
byte 1 111
byte 1 99
byte 1 105
byte 1 116
byte 1 105
byte 1 101
byte 1 115
byte 1 46
byte 1 111
byte 1 114
byte 1 103
byte 1 47
byte 1 0
align 1
LABELV $77
byte 1 111
byte 1 115
byte 1 95
byte 1 109
byte 1 97
byte 1 99
byte 1 111
byte 1 115
byte 1 0
align 1
LABELV $74
byte 1 120
byte 1 100
byte 1 103
byte 1 45
byte 1 111
byte 1 112
byte 1 101
byte 1 110
byte 1 32
byte 1 104
byte 1 116
byte 1 116
byte 1 112
byte 1 115
byte 1 58
byte 1 47
byte 1 47
byte 1 111
byte 1 112
byte 1 101
byte 1 110
byte 1 115
byte 1 97
byte 1 110
byte 1 100
byte 1 98
byte 1 111
byte 1 120
byte 1 46
byte 1 110
byte 1 101
byte 1 111
byte 1 99
byte 1 105
byte 1 116
byte 1 105
byte 1 101
byte 1 115
byte 1 46
byte 1 111
byte 1 114
byte 1 103
byte 1 47
byte 1 0
align 1
LABELV $73
byte 1 111
byte 1 115
byte 1 95
byte 1 108
byte 1 105
byte 1 110
byte 1 117
byte 1 120
byte 1 0
align 1
LABELV $70
byte 1 115
byte 1 116
byte 1 97
byte 1 114
byte 1 116
byte 1 32
byte 1 104
byte 1 116
byte 1 116
byte 1 112
byte 1 115
byte 1 58
byte 1 47
byte 1 47
byte 1 111
byte 1 112
byte 1 101
byte 1 110
byte 1 115
byte 1 97
byte 1 110
byte 1 100
byte 1 98
byte 1 111
byte 1 120
byte 1 46
byte 1 110
byte 1 101
byte 1 111
byte 1 99
byte 1 105
byte 1 116
byte 1 105
byte 1 101
byte 1 115
byte 1 46
byte 1 111
byte 1 114
byte 1 103
byte 1 47
byte 1 0
align 1
LABELV $69
byte 1 111
byte 1 115
byte 1 95
byte 1 119
byte 1 105
byte 1 110
byte 1 100
byte 1 111
byte 1 119
byte 1 115
byte 1 0
