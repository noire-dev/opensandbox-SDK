data
align 4
LABELV modeList
address $60
address $61
address $62
address $63
byte 4 0
align 4
LABELV textureList
address $64
address $65
address $66
address $67
address $68
byte 4 0
align 4
LABELV aaList
address $69
address $70
address $71
address $72
address $73
byte 4 0
align 4
LABELV bloomList
address $69
address $66
address $65
address $64
address $74
byte 4 0
align 4
LABELV sdriverList
address $75
address $76
byte 4 0
align 4
LABELV resolutions
byte 4 0
align 4
LABELV resolutionsDetected
byte 4 0
code
proc Settings_MenuEvent 16 8
ADDRFP4 0
ADDRFP4 0
INDIRP4
ASGNP4
ADDRFP4 4
INDIRI4
CNSTI4 3
EQI4 $78
ADDRGP4 $77
JUMPV
LABELV $78
ADDRFP4 0
INDIRP4
CNSTI4 40
ADDP4
INDIRI4
CNSTI4 3
NEI4 $80
ADDRGP4 $82
ARGP4
ADDRGP4 settings+536+1464+156
INDIRI4
CNSTI4 2
LSHI4
ADDRGP4 resolutions
INDIRP4
ADDP4
INDIRP4
ARGP4
ADDRGP4 cvarSet
CALLV
pop
LABELV $80
ADDRFP4 0
INDIRP4
CNSTI4 40
ADDP4
INDIRI4
CNSTI4 4
NEI4 $86
ADDRGP4 $89
ARGP4
ADDRGP4 settings+536+1952+156
INDIRI4
ARGI4
ADDRLP4 0
ADDRGP4 va
CALLP4
ASGNP4
ADDRGP4 $88
ARGP4
ADDRLP4 0
INDIRP4
ARGP4
ADDRGP4 cvarSet
CALLV
pop
LABELV $86
ADDRFP4 0
INDIRP4
CNSTI4 40
ADDP4
INDIRI4
CNSTI4 8
NEI4 $93
ADDRGP4 $89
ARGP4
ADDRGP4 settings+536+3904+156
INDIRI4
ARGI4
ADDRLP4 4
ADDRGP4 va
CALLP4
ASGNP4
ADDRGP4 $95
ARGP4
ADDRLP4 4
INDIRP4
ARGP4
ADDRGP4 cvarSet
CALLV
pop
LABELV $93
ADDRFP4 0
INDIRP4
CNSTI4 40
ADDP4
INDIRI4
CNSTI4 9
NEI4 $99
ADDRGP4 $89
ARGP4
ADDRGP4 settings+536+4392+156
INDIRI4
CNSTI4 1
LSHI4
ARGI4
ADDRLP4 8
ADDRGP4 va
CALLP4
ASGNP4
ADDRGP4 $101
ARGP4
ADDRLP4 8
INDIRP4
ARGP4
ADDRGP4 cvarSet
CALLV
pop
LABELV $99
ADDRFP4 0
INDIRP4
CNSTI4 40
ADDP4
INDIRI4
CNSTI4 10
NEI4 $105
ADDRGP4 $108
ARGP4
CNSTF4 1028443341
ADDRGP4 settings+536+4880+156
INDIRI4
CVIF4 4
MULF4
ARGF4
ADDRLP4 12
ADDRGP4 va
CALLP4
ASGNP4
ADDRGP4 $107
ARGP4
ADDRLP4 12
INDIRP4
ARGP4
ADDRGP4 cvarSet
CALLV
pop
LABELV $105
ADDRFP4 0
INDIRP4
CNSTI4 40
ADDP4
INDIRI4
CNSTI4 32
NEI4 $112
ADDRGP4 $114
ARGP4
ADDRGP4 settings+536+15616+156
INDIRI4
CNSTI4 2
LSHI4
ADDRGP4 sdriverList
ADDP4
INDIRP4
ARGP4
ADDRGP4 cvarSet
CALLV
pop
LABELV $112
LABELV $77
endproc Settings_MenuEvent 16 8
proc Settings_GetResolutions 24 20
ADDRGP4 $119
ARGP4
ADDRLP4 0
ADDRGP4 cvarString
CALLP4
ASGNP4
ADDRGP4 resbuf
ADDRLP4 0
INDIRP4
ASGNP4
ADDRGP4 resbuf
INDIRP4
INDIRI1
CVII4 1
CNSTI4 0
EQI4 $120
ADDRLP4 4
ADDRGP4 resbuf
INDIRP4
ASGNP4
ADDRLP4 8
CNSTU4 0
ASGNU4
ADDRLP4 12
CNSTU4 0
ASGNU4
ADDRGP4 $123
JUMPV
LABELV $122
ADDRLP4 16
ADDRLP4 8
INDIRU4
ASGNU4
ADDRLP4 8
ADDRLP4 16
INDIRU4
CNSTU4 1
ADDU4
ASGNU4
ADDRLP4 16
INDIRU4
CNSTI4 2
LSHU4
ADDRGP4 detectedResolutions
ADDP4
ADDRLP4 4
INDIRP4
ASGNP4
ADDRLP4 4
INDIRP4
ARGP4
CNSTI4 32
ARGI4
ADDRLP4 20
ADDRGP4 strchr
CALLP4
ASGNP4
ADDRLP4 4
ADDRLP4 20
INDIRP4
ASGNP4
ADDRLP4 4
INDIRP4
CVPU4 4
CNSTU4 0
EQU4 $125
ADDRLP4 4
INDIRP4
CNSTI1 0
ASGNI1
ADDRLP4 4
ADDRLP4 4
INDIRP4
CNSTI4 1
ADDP4
ASGNP4
LABELV $125
LABELV $123
ADDRLP4 4
INDIRP4
CVPU4 4
CNSTU4 0
EQU4 $127
ADDRLP4 8
INDIRU4
CNSTU4 63
LTU4 $122
LABELV $127
ADDRLP4 8
INDIRU4
CNSTI4 2
LSHU4
ADDRGP4 detectedResolutions
ADDP4
CNSTP4 0
ASGNP4
ADDRLP4 8
INDIRU4
CNSTU4 63
GEU4 $128
ADDRGP4 currentResolution
ARGP4
CNSTI4 32
ARGI4
ADDRGP4 $130
ARGP4
ADDRGP4 glconfig+11296
INDIRI4
ARGI4
ADDRGP4 glconfig+11300
INDIRI4
ARGI4
ADDRGP4 Com_sprintf
CALLV
pop
ADDRLP4 12
CNSTU4 0
ASGNU4
ADDRGP4 $136
JUMPV
LABELV $133
ADDRLP4 12
INDIRU4
CNSTI4 2
LSHU4
ADDRGP4 detectedResolutions
ADDP4
INDIRP4
ARGP4
ADDRGP4 currentResolution
ARGP4
ADDRLP4 16
ADDRGP4 strcmp
CALLI4
ASGNI4
ADDRLP4 16
INDIRI4
CNSTI4 0
NEI4 $137
ADDRGP4 $139
JUMPV
LABELV $137
LABELV $134
ADDRLP4 12
ADDRLP4 12
INDIRU4
CNSTU4 1
ADDU4
ASGNU4
LABELV $136
ADDRLP4 12
INDIRU4
ADDRLP4 8
INDIRU4
LTU4 $133
ADDRLP4 16
ADDRLP4 8
INDIRU4
ASGNU4
ADDRLP4 8
ADDRLP4 16
INDIRU4
CNSTU4 1
ADDU4
ASGNU4
ADDRLP4 16
INDIRU4
CNSTI4 2
LSHU4
ADDRGP4 detectedResolutions
ADDP4
ADDRGP4 currentResolution
ASGNP4
ADDRLP4 8
INDIRU4
CNSTI4 2
LSHU4
ADDRGP4 detectedResolutions
ADDP4
CNSTP4 0
ASGNP4
LABELV $128
LABELV $139
ADDRGP4 resolutions
ADDRGP4 detectedResolutions
ASGNP4
ADDRGP4 resolutionsDetected
CNSTI4 1
ASGNI4
LABELV $120
LABELV $118
endproc Settings_GetResolutions 24 20
export UI_Settings
proc UI_Settings 88 48
ADDRGP4 settings
ARGP4
CNSTI4 0
ARGI4
CNSTI4 49824
ARGI4
ADDRGP4 memset
CALLP4
pop
ADDRGP4 settings+532
CNSTI4 1
ASGNI4
ADDRGP4 Settings_GetResolutions
CALLV
pop
ADDRGP4 settings+536
ARGP4
CNSTF4 1092616192
ARGF4
CNSTF4 1094713344
ARGF4
ADDRGP4 $143
ARGP4
CNSTI4 0
ARGI4
CNSTF4 1065353216
ARGF4
ADDRGP4 UI_CText
CALLV
pop
ADDRGP4 settings+536+488
ARGP4
CNSTF4 1134559232
ARGF4
CNSTF4 1094713344
ARGF4
ADDRGP4 $146
ARGP4
CNSTI4 1
ARGI4
CNSTF4 1065353216
ARGF4
ADDRGP4 UI_CText
CALLV
pop
ADDRGP4 settings+536+976
ARGP4
CNSTF4 1142784000
ARGF4
CNSTF4 1094713344
ARGF4
ADDRGP4 $149
ARGP4
CNSTI4 2
ARGI4
CNSTF4 1065353216
ARGF4
ADDRGP4 UI_CText
CALLV
pop
ADDRLP4 0
CNSTI4 30
ASGNI4
ADDRLP4 8
CNSTI4 120
ASGNI4
ADDRGP4 settings+536+1464
ARGP4
ADDRLP4 8
INDIRI4
CVIF4 4
ARGF4
ADDRLP4 0
INDIRI4
CVIF4 4
ARGF4
ADDRGP4 $152
ARGP4
ADDRGP4 resolutions
INDIRP4
ARGP4
CNSTP4 0
ARGP4
ADDRGP4 Settings_MenuEvent
ARGP4
CNSTI4 0
ARGI4
ADDRGP4 UI_CSpinControl
CALLV
pop
ADDRLP4 0
ADDRLP4 0
INDIRI4
CNSTI4 12
ADDI4
ASGNI4
ADDRLP4 4
CNSTI4 0
ASGNI4
ADDRGP4 $156
JUMPV
LABELV $153
ADDRGP4 $82
ARGP4
ADDRLP4 12
ADDRGP4 cvarString
CALLP4
ASGNP4
ADDRLP4 4
INDIRI4
CNSTI4 2
LSHI4
ADDRGP4 resolutions
INDIRP4
ADDP4
INDIRP4
ARGP4
ADDRLP4 12
INDIRP4
ARGP4
ADDRLP4 16
ADDRGP4 strcmp
CALLI4
ASGNI4
ADDRLP4 16
INDIRI4
CNSTI4 0
NEI4 $157
ADDRGP4 settings+536+1464+156
ADDRLP4 4
INDIRI4
ASGNI4
ADDRGP4 $155
JUMPV
LABELV $157
LABELV $154
ADDRLP4 4
ADDRLP4 4
INDIRI4
CNSTI4 1
ADDI4
ASGNI4
LABELV $156
ADDRLP4 4
INDIRI4
CNSTI4 2
LSHI4
ADDRGP4 resolutions
INDIRP4
ADDP4
INDIRP4
CVPU4 4
CNSTU4 0
NEU4 $153
LABELV $155
ADDRGP4 settings+536+1952
ARGP4
ADDRLP4 8
INDIRI4
CVIF4 4
ARGF4
ADDRLP4 0
INDIRI4
CVIF4 4
ARGF4
ADDRGP4 $164
ARGP4
ADDRGP4 modeList
ARGP4
CNSTP4 0
ARGP4
ADDRGP4 Settings_MenuEvent
ARGP4
CNSTI4 0
ARGI4
ADDRGP4 UI_CSpinControl
CALLV
pop
ADDRLP4 0
ADDRLP4 0
INDIRI4
CNSTI4 12
ADDI4
ASGNI4
ADDRGP4 $88
ARGP4
ADDRLP4 12
ADDRGP4 cvarInt
CALLI4
ASGNI4
ADDRGP4 settings+536+1952+156
ADDRLP4 12
INDIRI4
ASGNI4
ADDRGP4 settings+536+2440
ARGP4
ADDRLP4 8
INDIRI4
CVIF4 4
ARGF4
ADDRLP4 0
INDIRI4
CVIF4 4
ARGF4
ADDRGP4 $170
ARGP4
ADDRGP4 $171
ARGP4
CNSTI4 1
ARGI4
CNSTP4 0
ARGP4
CNSTI4 0
ARGI4
ADDRGP4 UI_CRadioButton
CALLV
pop
ADDRLP4 0
ADDRLP4 0
INDIRI4
CNSTI4 12
ADDI4
ASGNI4
ADDRGP4 settings+536+2928
ARGP4
ADDRLP4 8
INDIRI4
CVIF4 4
ARGF4
ADDRLP4 0
INDIRI4
CVIF4 4
ARGF4
ADDRGP4 $174
ARGP4
ADDRGP4 $175
ARGP4
CNSTI4 1
ARGI4
CNSTP4 0
ARGP4
CNSTI4 0
ARGI4
ADDRGP4 UI_CRadioButton
CALLV
pop
ADDRLP4 0
ADDRLP4 0
INDIRI4
CNSTI4 12
ADDI4
ASGNI4
ADDRGP4 settings+536+3416
ARGP4
ADDRLP4 8
INDIRI4
CVIF4 4
ARGF4
ADDRLP4 0
INDIRI4
CVIF4 4
ARGF4
ADDRGP4 $178
ARGP4
ADDRGP4 $179
ARGP4
CNSTI4 1
ARGI4
CNSTP4 0
ARGP4
CNSTI4 0
ARGI4
ADDRGP4 UI_CRadioButton
CALLV
pop
ADDRLP4 0
ADDRLP4 0
INDIRI4
CNSTI4 12
ADDI4
ASGNI4
ADDRGP4 settings+536+3904
ARGP4
ADDRLP4 8
INDIRI4
CVIF4 4
ARGF4
ADDRLP4 0
INDIRI4
CVIF4 4
ARGF4
ADDRGP4 $182
ARGP4
ADDRGP4 textureList
ARGP4
CNSTP4 0
ARGP4
ADDRGP4 Settings_MenuEvent
ARGP4
CNSTI4 0
ARGI4
ADDRGP4 UI_CSpinControl
CALLV
pop
ADDRLP4 0
ADDRLP4 0
INDIRI4
CNSTI4 12
ADDI4
ASGNI4
ADDRGP4 $95
ARGP4
ADDRLP4 16
ADDRGP4 cvarInt
CALLI4
ASGNI4
ADDRGP4 settings+536+3904+156
ADDRLP4 16
INDIRI4
ASGNI4
ADDRGP4 settings+536+4392
ARGP4
ADDRLP4 8
INDIRI4
CVIF4 4
ARGF4
ADDRLP4 0
INDIRI4
CVIF4 4
ARGF4
ADDRGP4 $188
ARGP4
ADDRGP4 aaList
ARGP4
CNSTP4 0
ARGP4
ADDRGP4 Settings_MenuEvent
ARGP4
CNSTI4 0
ARGI4
ADDRGP4 UI_CSpinControl
CALLV
pop
ADDRLP4 0
ADDRLP4 0
INDIRI4
CNSTI4 12
ADDI4
ASGNI4
ADDRGP4 $101
ARGP4
ADDRLP4 20
ADDRGP4 cvarInt
CALLI4
ASGNI4
ADDRGP4 settings+536+4392+156
ADDRLP4 20
INDIRI4
CNSTI4 2
DIVI4
ASGNI4
ADDRGP4 settings+536+4880
ARGP4
ADDRLP4 8
INDIRI4
CVIF4 4
ARGF4
ADDRLP4 0
INDIRI4
CVIF4 4
ARGF4
ADDRGP4 $194
ARGP4
ADDRGP4 bloomList
ARGP4
CNSTP4 0
ARGP4
ADDRGP4 Settings_MenuEvent
ARGP4
CNSTI4 0
ARGI4
ADDRGP4 UI_CSpinControl
CALLV
pop
ADDRLP4 0
ADDRLP4 0
INDIRI4
CNSTI4 12
ADDI4
ASGNI4
ADDRGP4 $107
ARGP4
ADDRLP4 24
ADDRGP4 cvarFloat
CALLF4
ASGNF4
ADDRGP4 settings+536+4880+156
ADDRLP4 24
INDIRF4
CNSTF4 1028443341
DIVF4
CVFI4 4
ASGNI4
ADDRGP4 settings+536+5368
ARGP4
ADDRLP4 8
INDIRI4
CVIF4 4
ARGF4
ADDRLP4 0
INDIRI4
CVIF4 4
ARGF4
ADDRGP4 $200
ARGP4
ADDRGP4 $201
ARGP4
CNSTF4 1112014848
ARGF4
CNSTF4 1133903872
ARGF4
CNSTF4 1120403456
ARGF4
CNSTP4 0
ARGP4
CNSTI4 0
ARGI4
ADDRGP4 UI_CSlider
CALLV
pop
ADDRLP4 28
CNSTI4 12
ASGNI4
ADDRLP4 0
ADDRLP4 0
INDIRI4
ADDRLP4 28
INDIRI4
ADDI4
ASGNI4
ADDRLP4 0
ADDRLP4 0
INDIRI4
ADDRLP4 28
INDIRI4
ADDI4
ASGNI4
ADDRGP4 settings+536+5856
ARGP4
ADDRLP4 8
INDIRI4
CVIF4 4
ARGF4
ADDRLP4 0
INDIRI4
CVIF4 4
ARGF4
ADDRGP4 $204
ARGP4
ADDRGP4 $205
ARGP4
CNSTI4 1
ARGI4
CNSTP4 0
ARGP4
CNSTI4 0
ARGI4
ADDRGP4 UI_CRadioButton
CALLV
pop
ADDRLP4 0
ADDRLP4 0
INDIRI4
CNSTI4 12
ADDI4
ASGNI4
ADDRGP4 settings+536+6344
ARGP4
ADDRLP4 8
INDIRI4
CVIF4 4
ARGF4
ADDRLP4 0
INDIRI4
CVIF4 4
ARGF4
ADDRGP4 $208
ARGP4
ADDRLP4 32
CNSTI4 4
ASGNI4
ADDRLP4 32
INDIRI4
ARGI4
ADDRLP4 32
INDIRI4
ARGI4
ADDRGP4 color_white
ARGP4
ADDRGP4 $209
ARGP4
CNSTP4 0
ARGP4
CNSTI4 0
ARGI4
ADDRGP4 UI_CField
CALLV
pop
ADDRLP4 0
ADDRLP4 0
INDIRI4
CNSTI4 12
ADDI4
ASGNI4
ADDRLP4 0
CNSTI4 30
ASGNI4
ADDRLP4 8
CNSTI4 330
ASGNI4
ADDRGP4 settings+536+14640
ARGP4
ADDRLP4 8
INDIRI4
CVIF4 4
ARGF4
ADDRLP4 0
INDIRI4
CVIF4 4
ARGF4
ADDRGP4 $212
ARGP4
ADDRGP4 $213
ARGP4
CNSTF4 0
ARGF4
ADDRLP4 36
CNSTF4 1120403456
ASGNF4
ADDRLP4 36
INDIRF4
ARGF4
ADDRLP4 36
INDIRF4
ARGF4
CNSTP4 0
ARGP4
CNSTI4 0
ARGI4
ADDRGP4 UI_CSlider
CALLV
pop
ADDRLP4 0
ADDRLP4 0
INDIRI4
CNSTI4 12
ADDI4
ASGNI4
ADDRGP4 settings+536+15128
ARGP4
ADDRLP4 8
INDIRI4
CVIF4 4
ARGF4
ADDRLP4 0
INDIRI4
CVIF4 4
ARGF4
ADDRGP4 $216
ARGP4
ADDRGP4 $217
ARGP4
CNSTF4 0
ARGF4
ADDRLP4 40
CNSTF4 1120403456
ASGNF4
ADDRLP4 40
INDIRF4
ARGF4
ADDRLP4 40
INDIRF4
ARGF4
CNSTP4 0
ARGP4
CNSTI4 0
ARGI4
ADDRGP4 UI_CSlider
CALLV
pop
ADDRLP4 0
ADDRLP4 0
INDIRI4
CNSTI4 12
ADDI4
ASGNI4
ADDRGP4 settings+536+15616
ARGP4
ADDRLP4 8
INDIRI4
CVIF4 4
ARGF4
ADDRLP4 0
INDIRI4
CVIF4 4
ARGF4
ADDRGP4 $220
ARGP4
ADDRGP4 sdriverList
ARGP4
CNSTP4 0
ARGP4
ADDRGP4 Settings_MenuEvent
ARGP4
CNSTI4 0
ARGI4
ADDRGP4 UI_CSpinControl
CALLV
pop
ADDRLP4 0
ADDRLP4 0
INDIRI4
CNSTI4 12
ADDI4
ASGNI4
ADDRLP4 4
CNSTI4 0
ASGNI4
ADDRGP4 $224
JUMPV
LABELV $221
ADDRGP4 $114
ARGP4
ADDRLP4 44
ADDRGP4 cvarString
CALLP4
ASGNP4
ADDRLP4 4
INDIRI4
CNSTI4 2
LSHI4
ADDRGP4 sdriverList
ADDP4
INDIRP4
ARGP4
ADDRLP4 44
INDIRP4
ARGP4
ADDRLP4 48
ADDRGP4 strcmp
CALLI4
ASGNI4
ADDRLP4 48
INDIRI4
CNSTI4 0
NEI4 $225
ADDRGP4 settings+536+15616+156
ADDRLP4 4
INDIRI4
ASGNI4
ADDRGP4 $223
JUMPV
LABELV $225
LABELV $222
ADDRLP4 4
ADDRLP4 4
INDIRI4
CNSTI4 1
ADDI4
ASGNI4
LABELV $224
ADDRLP4 4
INDIRI4
CNSTI4 2
LSHI4
ADDRGP4 sdriverList
ADDP4
INDIRP4
CVPU4 4
CNSTU4 0
NEU4 $221
LABELV $223
ADDRLP4 0
CNSTI4 30
ASGNI4
ADDRLP4 8
CNSTI4 560
ASGNI4
ADDRGP4 settings+536+29280
ARGP4
ADDRLP4 8
INDIRI4
CVIF4 4
ARGF4
ADDRLP4 0
INDIRI4
CVIF4 4
ARGF4
ADDRGP4 $232
ARGP4
ADDRGP4 $233
ARGP4
CNSTI4 1
ARGI4
CNSTP4 0
ARGP4
CNSTI4 0
ARGI4
ADDRGP4 UI_CRadioButton
CALLV
pop
ADDRLP4 0
ADDRLP4 0
INDIRI4
CNSTI4 12
ADDI4
ASGNI4
ADDRGP4 settings+536+29768
ARGP4
ADDRLP4 8
INDIRI4
CVIF4 4
ARGF4
ADDRLP4 0
INDIRI4
CVIF4 4
ARGF4
ADDRGP4 $236
ARGP4
ADDRGP4 $237
ARGP4
CNSTI4 1
ARGI4
CNSTP4 0
ARGP4
CNSTI4 0
ARGI4
ADDRGP4 UI_CRadioButton
CALLV
pop
ADDRLP4 0
ADDRLP4 0
INDIRI4
CNSTI4 12
ADDI4
ASGNI4
ADDRGP4 settings+536+30256
ARGP4
ADDRLP4 8
INDIRI4
CVIF4 4
ARGF4
ADDRLP4 0
INDIRI4
CVIF4 4
ARGF4
ADDRGP4 $240
ARGP4
ADDRGP4 $241
ARGP4
CNSTI4 1
ARGI4
CNSTP4 0
ARGP4
CNSTI4 0
ARGI4
ADDRGP4 UI_CRadioButton
CALLV
pop
ADDRLP4 44
CNSTI4 12
ASGNI4
ADDRLP4 0
ADDRLP4 0
INDIRI4
ADDRLP4 44
INDIRI4
ADDI4
ASGNI4
ADDRLP4 0
ADDRLP4 0
INDIRI4
ADDRLP4 44
INDIRI4
ADDI4
ASGNI4
ADDRGP4 settings+536+30744
ARGP4
ADDRLP4 8
INDIRI4
CVIF4 4
ARGF4
ADDRLP4 0
INDIRI4
CVIF4 4
ARGF4
ADDRGP4 $244
ARGP4
ADDRGP4 $245
ARGP4
CNSTF4 0
ARGF4
ADDRLP4 48
CNSTF4 1120403456
ASGNF4
ADDRLP4 48
INDIRF4
ARGF4
ADDRLP4 48
INDIRF4
ARGF4
CNSTP4 0
ARGP4
CNSTI4 0
ARGI4
ADDRGP4 UI_CSlider
CALLV
pop
ADDRLP4 0
ADDRLP4 0
INDIRI4
CNSTI4 12
ADDI4
ASGNI4
ADDRGP4 settings+536+31232
ARGP4
ADDRLP4 8
INDIRI4
CVIF4 4
ARGF4
ADDRLP4 0
INDIRI4
CVIF4 4
ARGF4
ADDRGP4 $248
ARGP4
ADDRGP4 $249
ARGP4
CNSTF4 0
ARGF4
ADDRLP4 52
CNSTF4 1120403456
ASGNF4
ADDRLP4 52
INDIRF4
ARGF4
ADDRLP4 52
INDIRF4
ARGF4
CNSTP4 0
ARGP4
CNSTI4 0
ARGI4
ADDRGP4 UI_CSlider
CALLV
pop
ADDRLP4 0
ADDRLP4 0
INDIRI4
CNSTI4 12
ADDI4
ASGNI4
ADDRGP4 settings+536+31720
ARGP4
ADDRLP4 8
INDIRI4
CVIF4 4
ARGF4
ADDRLP4 0
INDIRI4
CVIF4 4
ARGF4
ADDRGP4 $252
ARGP4
ADDRGP4 $253
ARGP4
CNSTF4 0
ARGF4
ADDRLP4 56
CNSTF4 1120403456
ASGNF4
ADDRLP4 56
INDIRF4
ARGF4
ADDRLP4 56
INDIRF4
ARGF4
CNSTP4 0
ARGP4
CNSTI4 0
ARGI4
ADDRGP4 UI_CSlider
CALLV
pop
ADDRLP4 60
CNSTI4 12
ASGNI4
ADDRLP4 0
ADDRLP4 0
INDIRI4
ADDRLP4 60
INDIRI4
ADDI4
ASGNI4
ADDRLP4 0
ADDRLP4 0
INDIRI4
ADDRLP4 60
INDIRI4
ADDI4
ASGNI4
ADDRGP4 settings+536+32208
ARGP4
ADDRLP4 8
INDIRI4
CVIF4 4
ARGF4
ADDRLP4 0
INDIRI4
CVIF4 4
ARGF4
ADDRGP4 $256
ARGP4
ADDRGP4 $257
ARGP4
CNSTF4 1092616192
ARGF4
CNSTF4 1120403456
ARGF4
CNSTF4 1065353216
ARGF4
CNSTP4 0
ARGP4
CNSTI4 0
ARGI4
ADDRGP4 UI_CSlider
CALLV
pop
ADDRLP4 0
ADDRLP4 0
INDIRI4
CNSTI4 12
ADDI4
ASGNI4
ADDRGP4 settings+536+32696
ARGP4
ADDRLP4 8
INDIRI4
CVIF4 4
ARGF4
ADDRLP4 0
INDIRI4
CVIF4 4
ARGF4
ADDRGP4 $260
ARGP4
ADDRGP4 $261
ARGP4
CNSTF4 0
ARGF4
CNSTF4 1091567616
ARGF4
CNSTF4 1065353216
ARGF4
CNSTP4 0
ARGP4
CNSTI4 0
ARGI4
ADDRGP4 UI_CSlider
CALLV
pop
ADDRLP4 64
CNSTI4 12
ASGNI4
ADDRLP4 0
ADDRLP4 0
INDIRI4
ADDRLP4 64
INDIRI4
ADDI4
ASGNI4
ADDRLP4 0
ADDRLP4 0
INDIRI4
ADDRLP4 64
INDIRI4
ADDI4
ASGNI4
ADDRGP4 settings+536+33184
ARGP4
ADDRLP4 8
INDIRI4
CVIF4 4
ARGF4
ADDRLP4 0
INDIRI4
CVIF4 4
ARGF4
ADDRGP4 $264
ARGP4
ADDRGP4 $265
ARGP4
CNSTI4 1
ARGI4
CNSTP4 0
ARGP4
CNSTI4 0
ARGI4
ADDRGP4 UI_CRadioButton
CALLV
pop
ADDRLP4 0
ADDRLP4 0
INDIRI4
CNSTI4 12
ADDI4
ASGNI4
ADDRGP4 settings+536+33672
ARGP4
ADDRLP4 8
INDIRI4
CVIF4 4
ARGF4
ADDRLP4 0
INDIRI4
CVIF4 4
ARGF4
ADDRGP4 $268
ARGP4
ADDRGP4 $269
ARGP4
CNSTF4 3259498496
ARGF4
CNSTF4 1112014848
ARGF4
CNSTF4 1065353216
ARGF4
CNSTP4 0
ARGP4
CNSTI4 0
ARGI4
ADDRGP4 UI_CSlider
CALLV
pop
ADDRLP4 0
ADDRLP4 0
INDIRI4
CNSTI4 12
ADDI4
ASGNI4
ADDRGP4 settings+536+34160
ARGP4
ADDRLP4 8
INDIRI4
CVIF4 4
ARGF4
ADDRLP4 0
INDIRI4
CVIF4 4
ARGF4
ADDRGP4 $272
ARGP4
ADDRGP4 $273
ARGP4
CNSTF4 1101004800
ARGF4
CNSTF4 1120403456
ARGF4
CNSTF4 1065353216
ARGF4
CNSTP4 0
ARGP4
CNSTI4 0
ARGI4
ADDRGP4 UI_CSlider
CALLV
pop
ADDRLP4 68
CNSTI4 12
ASGNI4
ADDRLP4 0
ADDRLP4 0
INDIRI4
ADDRLP4 68
INDIRI4
ADDI4
ASGNI4
ADDRLP4 0
ADDRLP4 0
INDIRI4
ADDRLP4 68
INDIRI4
ADDI4
ASGNI4
ADDRGP4 settings+536+34648
ARGP4
ADDRLP4 8
INDIRI4
CVIF4 4
ARGF4
ADDRLP4 0
INDIRI4
CVIF4 4
ARGF4
ADDRGP4 $276
ARGP4
ADDRGP4 $277
ARGP4
CNSTI4 1
ARGI4
CNSTP4 0
ARGP4
CNSTI4 0
ARGI4
ADDRGP4 UI_CRadioButton
CALLV
pop
ADDRLP4 72
CNSTI4 12
ASGNI4
ADDRLP4 0
ADDRLP4 0
INDIRI4
ADDRLP4 72
INDIRI4
ADDI4
ASGNI4
ADDRLP4 0
ADDRLP4 0
INDIRI4
ADDRLP4 72
INDIRI4
ADDI4
ASGNI4
ADDRGP4 settings+536+35136
ARGP4
ADDRLP4 8
INDIRI4
CVIF4 4
ARGF4
ADDRLP4 0
INDIRI4
CVIF4 4
ARGF4
ADDRGP4 $280
ARGP4
ADDRGP4 $281
ARGP4
CNSTI4 1
ARGI4
CNSTP4 0
ARGP4
CNSTI4 0
ARGI4
ADDRGP4 UI_CRadioButton
CALLV
pop
ADDRLP4 0
ADDRLP4 0
INDIRI4
CNSTI4 12
ADDI4
ASGNI4
ADDRGP4 settings+536+35624
ARGP4
ADDRLP4 8
INDIRI4
CVIF4 4
ARGF4
ADDRLP4 0
INDIRI4
CVIF4 4
ARGF4
ADDRGP4 $284
ARGP4
ADDRGP4 $285
ARGP4
CNSTI4 1
ARGI4
CNSTP4 0
ARGP4
CNSTI4 0
ARGI4
ADDRGP4 UI_CRadioButton
CALLV
pop
ADDRLP4 0
ADDRLP4 0
INDIRI4
CNSTI4 12
ADDI4
ASGNI4
ADDRGP4 settings+536+36112
ARGP4
ADDRLP4 8
INDIRI4
CVIF4 4
ARGF4
ADDRLP4 0
INDIRI4
CVIF4 4
ARGF4
ADDRGP4 $288
ARGP4
ADDRGP4 $289
ARGP4
CNSTI4 1
ARGI4
CNSTP4 0
ARGP4
CNSTI4 0
ARGI4
ADDRGP4 UI_CRadioButton
CALLV
pop
ADDRLP4 76
CNSTI4 12
ASGNI4
ADDRLP4 0
ADDRLP4 0
INDIRI4
ADDRLP4 76
INDIRI4
ADDI4
ASGNI4
ADDRLP4 0
ADDRLP4 0
INDIRI4
ADDRLP4 76
INDIRI4
ADDI4
ASGNI4
ADDRGP4 settings+536+36600
ARGP4
ADDRLP4 8
INDIRI4
CVIF4 4
ARGF4
ADDRLP4 0
INDIRI4
CVIF4 4
ARGF4
ADDRGP4 $292
ARGP4
ADDRLP4 80
CNSTI4 4
ASGNI4
ADDRLP4 80
INDIRI4
ARGI4
ADDRLP4 80
INDIRI4
ARGI4
ADDRGP4 color_white
ARGP4
ADDRGP4 $293
ARGP4
CNSTP4 0
ARGP4
CNSTI4 0
ARGI4
ADDRGP4 UI_CField
CALLV
pop
ADDRLP4 0
ADDRLP4 0
INDIRI4
CNSTI4 12
ADDI4
ASGNI4
ADDRGP4 settings+536+37088
ARGP4
ADDRLP4 8
INDIRI4
CVIF4 4
ARGF4
ADDRLP4 0
INDIRI4
CVIF4 4
ARGF4
ADDRGP4 $296
ARGP4
ADDRLP4 84
CNSTI4 4
ASGNI4
ADDRLP4 84
INDIRI4
ARGI4
ADDRLP4 84
INDIRI4
ARGI4
ADDRGP4 color_white
ARGP4
ADDRGP4 $297
ARGP4
CNSTP4 0
ARGP4
CNSTI4 0
ARGI4
ADDRGP4 UI_CField
CALLV
pop
ADDRLP4 0
ADDRLP4 0
INDIRI4
CNSTI4 12
ADDI4
ASGNI4
ADDRGP4 settings+536+48800
ARGP4
CNSTF4 1134559232
ARGF4
CNSTF4 1138819072
ARGF4
ADDRGP4 $300
ARGP4
CNSTI4 1
ARGI4
CNSTF4 1069547520
ARGF4
ADDRGP4 color_white
ARGP4
ADDRGP4 $301
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
ADDRGP4 settings
ARGP4
ADDRGP4 settings+536
ARGP4
ADDRGP4 UI_CreateUI
CALLV
pop
ADDRGP4 settings
ARGP4
ADDRGP4 UI_PushMenu
CALLV
pop
LABELV $140
endproc UI_Settings 88 48
bss
align 1
LABELV currentResolution
skip 32
align 4
LABELV detectedResolutions
skip 256
align 4
LABELV resbuf
skip 4
align 4
LABELV settings
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
LABELV $301
byte 1 118
byte 1 105
byte 1 100
byte 1 95
byte 1 114
byte 1 101
byte 1 115
byte 1 116
byte 1 97
byte 1 114
byte 1 116
byte 1 59
byte 1 0
align 1
LABELV $300
byte 1 65
byte 1 112
byte 1 112
byte 1 108
byte 1 121
byte 1 0
align 1
LABELV $297
byte 1 99
byte 1 103
byte 1 95
byte 1 122
byte 1 111
byte 1 111
byte 1 109
byte 1 102
byte 1 111
byte 1 118
byte 1 0
align 1
LABELV $296
byte 1 90
byte 1 111
byte 1 111
byte 1 109
byte 1 32
byte 1 70
byte 1 111
byte 1 86
byte 1 58
byte 1 0
align 1
LABELV $293
byte 1 99
byte 1 103
byte 1 95
byte 1 102
byte 1 111
byte 1 118
byte 1 0
align 1
LABELV $292
byte 1 70
byte 1 111
byte 1 86
byte 1 58
byte 1 0
align 1
LABELV $289
byte 1 99
byte 1 111
byte 1 109
byte 1 95
byte 1 98
byte 1 108
byte 1 111
byte 1 111
byte 1 100
byte 1 0
align 1
LABELV $288
byte 1 66
byte 1 108
byte 1 111
byte 1 111
byte 1 100
byte 1 58
byte 1 0
align 1
LABELV $285
byte 1 99
byte 1 103
byte 1 95
byte 1 97
byte 1 100
byte 1 100
byte 1 77
byte 1 97
byte 1 114
byte 1 107
byte 1 115
byte 1 0
align 1
LABELV $284
byte 1 77
byte 1 97
byte 1 114
byte 1 107
byte 1 115
byte 1 58
byte 1 0
align 1
LABELV $281
byte 1 99
byte 1 103
byte 1 95
byte 1 101
byte 1 110
byte 1 97
byte 1 98
byte 1 108
byte 1 101
byte 1 66
byte 1 111
byte 1 98
byte 1 98
byte 1 105
byte 1 110
byte 1 103
byte 1 0
align 1
LABELV $280
byte 1 67
byte 1 97
byte 1 109
byte 1 101
byte 1 114
byte 1 97
byte 1 32
byte 1 115
byte 1 104
byte 1 97
byte 1 107
byte 1 101
byte 1 58
byte 1 0
align 1
LABELV $277
byte 1 99
byte 1 103
byte 1 95
byte 1 99
byte 1 97
byte 1 109
byte 1 101
byte 1 114
byte 1 97
byte 1 69
byte 1 121
byte 1 101
byte 1 115
byte 1 0
align 1
LABELV $276
byte 1 67
byte 1 97
byte 1 109
byte 1 101
byte 1 114
byte 1 97
byte 1 32
byte 1 101
byte 1 121
byte 1 101
byte 1 115
byte 1 58
byte 1 0
align 1
LABELV $273
byte 1 99
byte 1 103
byte 1 95
byte 1 116
byte 1 104
byte 1 105
byte 1 114
byte 1 100
byte 1 80
byte 1 101
byte 1 114
byte 1 115
byte 1 111
byte 1 110
byte 1 82
byte 1 97
byte 1 110
byte 1 103
byte 1 101
byte 1 0
align 1
LABELV $272
byte 1 51
byte 1 114
byte 1 100
byte 1 32
byte 1 112
byte 1 101
byte 1 114
byte 1 115
byte 1 111
byte 1 110
byte 1 32
byte 1 114
byte 1 97
byte 1 110
byte 1 103
byte 1 101
byte 1 58
byte 1 0
align 1
LABELV $269
byte 1 99
byte 1 103
byte 1 95
byte 1 116
byte 1 104
byte 1 105
byte 1 114
byte 1 100
byte 1 80
byte 1 101
byte 1 114
byte 1 115
byte 1 111
byte 1 110
byte 1 79
byte 1 102
byte 1 102
byte 1 115
byte 1 101
byte 1 116
byte 1 0
align 1
LABELV $268
byte 1 51
byte 1 114
byte 1 100
byte 1 32
byte 1 112
byte 1 101
byte 1 114
byte 1 115
byte 1 111
byte 1 110
byte 1 32
byte 1 111
byte 1 102
byte 1 102
byte 1 115
byte 1 101
byte 1 116
byte 1 58
byte 1 0
align 1
LABELV $265
byte 1 99
byte 1 103
byte 1 95
byte 1 116
byte 1 104
byte 1 105
byte 1 114
byte 1 100
byte 1 80
byte 1 101
byte 1 114
byte 1 115
byte 1 111
byte 1 110
byte 1 0
align 1
LABELV $264
byte 1 51
byte 1 114
byte 1 100
byte 1 32
byte 1 112
byte 1 101
byte 1 114
byte 1 115
byte 1 111
byte 1 110
byte 1 58
byte 1 0
align 1
LABELV $261
byte 1 99
byte 1 103
byte 1 95
byte 1 100
byte 1 114
byte 1 97
byte 1 119
byte 1 67
byte 1 114
byte 1 111
byte 1 115
byte 1 115
byte 1 104
byte 1 97
byte 1 105
byte 1 114
byte 1 0
align 1
LABELV $260
byte 1 67
byte 1 114
byte 1 111
byte 1 115
byte 1 115
byte 1 104
byte 1 97
byte 1 105
byte 1 114
byte 1 32
byte 1 116
byte 1 121
byte 1 112
byte 1 101
byte 1 58
byte 1 0
align 1
LABELV $257
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
byte 1 83
byte 1 99
byte 1 97
byte 1 108
byte 1 101
byte 1 0
align 1
LABELV $256
byte 1 67
byte 1 114
byte 1 111
byte 1 115
byte 1 115
byte 1 104
byte 1 97
byte 1 105
byte 1 114
byte 1 32
byte 1 115
byte 1 105
byte 1 122
byte 1 101
byte 1 58
byte 1 0
align 1
LABELV $253
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
LABELV $252
byte 1 94
byte 1 52
byte 1 72
byte 1 117
byte 1 100
byte 1 58
byte 1 0
align 1
LABELV $249
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
LABELV $248
byte 1 94
byte 1 50
byte 1 72
byte 1 117
byte 1 100
byte 1 58
byte 1 0
align 1
LABELV $245
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
align 1
LABELV $244
byte 1 94
byte 1 49
byte 1 72
byte 1 117
byte 1 100
byte 1 58
byte 1 0
align 1
LABELV $241
byte 1 99
byte 1 103
byte 1 95
byte 1 100
byte 1 114
byte 1 97
byte 1 119
byte 1 84
byte 1 105
byte 1 109
byte 1 101
byte 1 114
byte 1 0
align 1
LABELV $240
byte 1 68
byte 1 114
byte 1 97
byte 1 119
byte 1 32
byte 1 84
byte 1 105
byte 1 109
byte 1 101
byte 1 114
byte 1 58
byte 1 0
align 1
LABELV $237
byte 1 99
byte 1 103
byte 1 95
byte 1 100
byte 1 114
byte 1 97
byte 1 119
byte 1 70
byte 1 80
byte 1 83
byte 1 0
align 1
LABELV $236
byte 1 68
byte 1 114
byte 1 97
byte 1 119
byte 1 32
byte 1 70
byte 1 80
byte 1 83
byte 1 58
byte 1 0
align 1
LABELV $233
byte 1 99
byte 1 103
byte 1 95
byte 1 100
byte 1 114
byte 1 97
byte 1 119
byte 1 83
byte 1 112
byte 1 101
byte 1 101
byte 1 100
byte 1 0
align 1
LABELV $232
byte 1 68
byte 1 114
byte 1 97
byte 1 119
byte 1 32
byte 1 83
byte 1 112
byte 1 101
byte 1 101
byte 1 100
byte 1 58
byte 1 0
align 1
LABELV $220
byte 1 83
byte 1 111
byte 1 117
byte 1 110
byte 1 100
byte 1 32
byte 1 100
byte 1 114
byte 1 105
byte 1 118
byte 1 101
byte 1 114
byte 1 58
byte 1 0
align 1
LABELV $217
byte 1 115
byte 1 95
byte 1 109
byte 1 117
byte 1 115
byte 1 105
byte 1 99
byte 1 86
byte 1 111
byte 1 108
byte 1 117
byte 1 109
byte 1 101
byte 1 0
align 1
LABELV $216
byte 1 77
byte 1 117
byte 1 115
byte 1 105
byte 1 99
byte 1 32
byte 1 118
byte 1 111
byte 1 108
byte 1 117
byte 1 109
byte 1 101
byte 1 58
byte 1 0
align 1
LABELV $213
byte 1 115
byte 1 95
byte 1 118
byte 1 111
byte 1 108
byte 1 117
byte 1 109
byte 1 101
byte 1 0
align 1
LABELV $212
byte 1 83
byte 1 111
byte 1 117
byte 1 110
byte 1 100
byte 1 32
byte 1 118
byte 1 111
byte 1 108
byte 1 117
byte 1 109
byte 1 101
byte 1 58
byte 1 0
align 1
LABELV $209
byte 1 99
byte 1 111
byte 1 109
byte 1 95
byte 1 109
byte 1 97
byte 1 120
byte 1 102
byte 1 112
byte 1 115
byte 1 0
align 1
LABELV $208
byte 1 77
byte 1 97
byte 1 120
byte 1 32
byte 1 70
byte 1 80
byte 1 83
byte 1 58
byte 1 0
align 1
LABELV $205
byte 1 114
byte 1 95
byte 1 115
byte 1 119
byte 1 97
byte 1 112
byte 1 73
byte 1 110
byte 1 116
byte 1 101
byte 1 114
byte 1 118
byte 1 97
byte 1 108
byte 1 0
align 1
LABELV $204
byte 1 86
byte 1 45
byte 1 83
byte 1 121
byte 1 110
byte 1 99
byte 1 58
byte 1 0
align 1
LABELV $201
byte 1 114
byte 1 95
byte 1 103
byte 1 97
byte 1 109
byte 1 109
byte 1 97
byte 1 0
align 1
LABELV $200
byte 1 71
byte 1 97
byte 1 109
byte 1 109
byte 1 97
byte 1 58
byte 1 0
align 1
LABELV $194
byte 1 66
byte 1 108
byte 1 111
byte 1 111
byte 1 109
byte 1 32
byte 1 108
byte 1 101
byte 1 118
byte 1 101
byte 1 108
byte 1 58
byte 1 0
align 1
LABELV $188
byte 1 65
byte 1 110
byte 1 116
byte 1 105
byte 1 45
byte 1 97
byte 1 108
byte 1 105
byte 1 97
byte 1 115
byte 1 105
byte 1 110
byte 1 103
byte 1 58
byte 1 0
align 1
LABELV $182
byte 1 84
byte 1 101
byte 1 120
byte 1 116
byte 1 117
byte 1 114
byte 1 101
byte 1 32
byte 1 113
byte 1 117
byte 1 97
byte 1 108
byte 1 105
byte 1 116
byte 1 121
byte 1 58
byte 1 0
align 1
LABELV $179
byte 1 114
byte 1 95
byte 1 104
byte 1 100
byte 1 114
byte 1 0
align 1
LABELV $178
byte 1 72
byte 1 68
byte 1 82
byte 1 58
byte 1 0
align 1
LABELV $175
byte 1 114
byte 1 95
byte 1 112
byte 1 111
byte 1 115
byte 1 116
byte 1 102
byte 1 120
byte 1 0
align 1
LABELV $174
byte 1 80
byte 1 111
byte 1 115
byte 1 116
byte 1 45
byte 1 112
byte 1 114
byte 1 111
byte 1 99
byte 1 101
byte 1 115
byte 1 115
byte 1 105
byte 1 110
byte 1 103
byte 1 58
byte 1 0
align 1
LABELV $171
byte 1 114
byte 1 95
byte 1 101
byte 1 120
byte 1 116
byte 1 95
byte 1 116
byte 1 101
byte 1 120
byte 1 116
byte 1 117
byte 1 114
byte 1 101
byte 1 95
byte 1 102
byte 1 105
byte 1 108
byte 1 116
byte 1 101
byte 1 114
byte 1 95
byte 1 97
byte 1 110
byte 1 105
byte 1 115
byte 1 111
byte 1 116
byte 1 114
byte 1 111
byte 1 112
byte 1 105
byte 1 99
byte 1 0
align 1
LABELV $170
byte 1 65
byte 1 110
byte 1 105
byte 1 115
byte 1 111
byte 1 116
byte 1 114
byte 1 111
byte 1 112
byte 1 121
byte 1 58
byte 1 0
align 1
LABELV $164
byte 1 77
byte 1 111
byte 1 100
byte 1 101
byte 1 58
byte 1 0
align 1
LABELV $152
byte 1 82
byte 1 101
byte 1 115
byte 1 111
byte 1 108
byte 1 117
byte 1 116
byte 1 105
byte 1 111
byte 1 110
byte 1 58
byte 1 0
align 1
LABELV $149
byte 1 79
byte 1 116
byte 1 104
byte 1 101
byte 1 114
byte 1 0
align 1
LABELV $146
byte 1 83
byte 1 111
byte 1 117
byte 1 110
byte 1 100
byte 1 0
align 1
LABELV $143
byte 1 71
byte 1 114
byte 1 97
byte 1 112
byte 1 104
byte 1 105
byte 1 99
byte 1 115
byte 1 0
align 1
LABELV $130
byte 1 37
byte 1 100
byte 1 120
byte 1 37
byte 1 100
byte 1 0
align 1
LABELV $119
byte 1 114
byte 1 95
byte 1 97
byte 1 118
byte 1 97
byte 1 105
byte 1 108
byte 1 97
byte 1 98
byte 1 108
byte 1 101
byte 1 77
byte 1 111
byte 1 100
byte 1 101
byte 1 115
byte 1 0
align 1
LABELV $114
byte 1 115
byte 1 95
byte 1 100
byte 1 114
byte 1 105
byte 1 118
byte 1 101
byte 1 114
byte 1 0
align 1
LABELV $108
byte 1 37
byte 1 102
byte 1 0
align 1
LABELV $107
byte 1 114
byte 1 95
byte 1 98
byte 1 108
byte 1 111
byte 1 111
byte 1 109
byte 1 95
byte 1 105
byte 1 110
byte 1 116
byte 1 101
byte 1 110
byte 1 115
byte 1 105
byte 1 116
byte 1 121
byte 1 0
align 1
LABELV $101
byte 1 114
byte 1 95
byte 1 101
byte 1 120
byte 1 116
byte 1 95
byte 1 109
byte 1 117
byte 1 108
byte 1 116
byte 1 105
byte 1 115
byte 1 97
byte 1 109
byte 1 112
byte 1 108
byte 1 101
byte 1 0
align 1
LABELV $95
byte 1 114
byte 1 95
byte 1 112
byte 1 105
byte 1 99
byte 1 109
byte 1 105
byte 1 112
byte 1 0
align 1
LABELV $89
byte 1 37
byte 1 105
byte 1 0
align 1
LABELV $88
byte 1 114
byte 1 95
byte 1 102
byte 1 117
byte 1 108
byte 1 108
byte 1 115
byte 1 99
byte 1 114
byte 1 101
byte 1 101
byte 1 110
byte 1 0
align 1
LABELV $82
byte 1 114
byte 1 95
byte 1 114
byte 1 101
byte 1 115
byte 1 111
byte 1 108
byte 1 117
byte 1 116
byte 1 105
byte 1 111
byte 1 110
byte 1 0
align 1
LABELV $76
byte 1 119
byte 1 97
byte 1 115
byte 1 97
byte 1 112
byte 1 105
byte 1 0
align 1
LABELV $75
byte 1 100
byte 1 115
byte 1 111
byte 1 117
byte 1 110
byte 1 100
byte 1 0
align 1
LABELV $74
byte 1 85
byte 1 108
byte 1 116
byte 1 114
byte 1 97
byte 1 0
align 1
LABELV $73
byte 1 56
byte 1 120
byte 1 0
align 1
LABELV $72
byte 1 54
byte 1 120
byte 1 0
align 1
LABELV $71
byte 1 52
byte 1 120
byte 1 0
align 1
LABELV $70
byte 1 50
byte 1 120
byte 1 0
align 1
LABELV $69
byte 1 79
byte 1 102
byte 1 102
byte 1 0
align 1
LABELV $68
byte 1 85
byte 1 108
byte 1 116
byte 1 114
byte 1 97
byte 1 32
byte 1 108
byte 1 111
byte 1 119
byte 1 0
align 1
LABELV $67
byte 1 86
byte 1 101
byte 1 114
byte 1 121
byte 1 32
byte 1 108
byte 1 111
byte 1 119
byte 1 0
align 1
LABELV $66
byte 1 76
byte 1 111
byte 1 119
byte 1 0
align 1
LABELV $65
byte 1 77
byte 1 101
byte 1 100
byte 1 105
byte 1 117
byte 1 109
byte 1 0
align 1
LABELV $64
byte 1 72
byte 1 105
byte 1 103
byte 1 104
byte 1 0
align 1
LABELV $63
byte 1 70
byte 1 117
byte 1 108
byte 1 108
byte 1 32
byte 1 100
byte 1 101
byte 1 115
byte 1 107
byte 1 116
byte 1 111
byte 1 112
byte 1 0
align 1
LABELV $62
byte 1 70
byte 1 117
byte 1 108
byte 1 108
byte 1 115
byte 1 99
byte 1 114
byte 1 101
byte 1 101
byte 1 110
byte 1 0
align 1
LABELV $61
byte 1 66
byte 1 111
byte 1 114
byte 1 100
byte 1 101
byte 1 114
byte 1 108
byte 1 101
byte 1 115
byte 1 115
byte 1 32
byte 1 119
byte 1 105
byte 1 110
byte 1 100
byte 1 111
byte 1 119
byte 1 0
align 1
LABELV $60
byte 1 87
byte 1 105
byte 1 110
byte 1 100
byte 1 111
byte 1 119
byte 1 0
