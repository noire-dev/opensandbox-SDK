export CG_AdjustFrom640
code
proc CG_AdjustFrom640 16 0
ADDRLP4 0
ADDRFP4 0
INDIRP4
ASGNP4
ADDRLP4 0
INDIRP4
ADDRLP4 0
INDIRP4
INDIRF4
ADDRGP4 cgs+71940
INDIRF4
MULF4
ADDRGP4 cgs+71944
INDIRF4
ADDF4
ASGNF4
ADDRLP4 4
ADDRFP4 4
INDIRP4
ASGNP4
ADDRLP4 4
INDIRP4
ADDRLP4 4
INDIRP4
INDIRF4
ADDRGP4 cgs+71940
INDIRF4
MULF4
ASGNF4
ADDRLP4 8
ADDRFP4 8
INDIRP4
ASGNP4
ADDRLP4 8
INDIRP4
ADDRLP4 8
INDIRP4
INDIRF4
ADDRGP4 cgs+71940
INDIRF4
MULF4
ASGNF4
ADDRLP4 12
ADDRFP4 12
INDIRP4
ASGNP4
ADDRLP4 12
INDIRP4
ADDRLP4 12
INDIRP4
INDIRF4
ADDRGP4 cgs+71940
INDIRF4
MULF4
ASGNF4
LABELV $75
endproc CG_AdjustFrom640 16 0
export CG_DrawRoundedRect
proc CG_DrawRoundedRect 72 36
ADDRFP4 16
ADDRFP4 16
INDIRF4
ASGNF4
ADDRFP4 0
ARGP4
ADDRFP4 4
ARGP4
ADDRFP4 8
ARGP4
ADDRFP4 12
ARGP4
ADDRGP4 CG_AdjustFrom640
CALLV
pop
CNSTF4 1073741824
ADDRFP4 16
INDIRF4
MULF4
ADDRFP4 12
INDIRF4
LEF4 $82
ADDRFP4 16
CNSTF4 1056964608
ADDRFP4 12
INDIRF4
MULF4
ASGNF4
LABELV $82
CNSTF4 1073741824
ADDRFP4 16
INDIRF4
MULF4
ADDRFP4 8
INDIRF4
LEF4 $84
ADDRFP4 16
CNSTF4 1056964608
ADDRFP4 8
INDIRF4
MULF4
ASGNF4
LABELV $84
ADDRFP4 16
ADDRFP4 16
INDIRF4
ADDRGP4 cgs+71940
INDIRF4
MULF4
ASGNF4
ADDRFP4 20
INDIRP4
ARGP4
ADDRGP4 trap_R_SetColor
CALLV
pop
ADDRFP4 0
INDIRF4
ARGF4
ADDRFP4 4
INDIRF4
ARGF4
ADDRFP4 16
INDIRF4
ARGF4
ADDRFP4 16
INDIRF4
ARGF4
ADDRLP4 4
CNSTF4 1065353216
ASGNF4
ADDRLP4 4
INDIRF4
ARGF4
ADDRLP4 8
CNSTF4 0
ASGNF4
ADDRLP4 8
INDIRF4
ARGF4
ADDRLP4 8
INDIRF4
ARGF4
ADDRLP4 4
INDIRF4
ARGF4
ADDRGP4 cgs+474200+16
INDIRI4
ARGI4
ADDRGP4 trap_R_DrawStretchPic
CALLV
pop
ADDRFP4 0
INDIRF4
ADDRFP4 8
INDIRF4
ADDF4
ADDRFP4 16
INDIRF4
SUBF4
ARGF4
ADDRFP4 4
INDIRF4
ARGF4
ADDRFP4 16
INDIRF4
ARGF4
ADDRFP4 16
INDIRF4
ARGF4
ADDRLP4 16
CNSTF4 0
ASGNF4
ADDRLP4 16
INDIRF4
ARGF4
ADDRLP4 16
INDIRF4
ARGF4
ADDRLP4 20
CNSTF4 1065353216
ASGNF4
ADDRLP4 20
INDIRF4
ARGF4
ADDRLP4 20
INDIRF4
ARGF4
ADDRGP4 cgs+474200+16
INDIRI4
ARGI4
ADDRGP4 trap_R_DrawStretchPic
CALLV
pop
ADDRFP4 0
INDIRF4
ARGF4
ADDRFP4 4
INDIRF4
ADDRFP4 12
INDIRF4
ADDF4
ADDRFP4 16
INDIRF4
SUBF4
ARGF4
ADDRFP4 16
INDIRF4
ARGF4
ADDRFP4 16
INDIRF4
ARGF4
ADDRLP4 28
CNSTF4 1065353216
ASGNF4
ADDRLP4 28
INDIRF4
ARGF4
ADDRLP4 28
INDIRF4
ARGF4
ADDRLP4 32
CNSTF4 0
ASGNF4
ADDRLP4 32
INDIRF4
ARGF4
ADDRLP4 32
INDIRF4
ARGF4
ADDRGP4 cgs+474200+16
INDIRI4
ARGI4
ADDRGP4 trap_R_DrawStretchPic
CALLV
pop
ADDRFP4 0
INDIRF4
ADDRFP4 8
INDIRF4
ADDF4
ADDRFP4 16
INDIRF4
SUBF4
ARGF4
ADDRFP4 4
INDIRF4
ADDRFP4 12
INDIRF4
ADDF4
ADDRFP4 16
INDIRF4
SUBF4
ARGF4
ADDRFP4 16
INDIRF4
ARGF4
ADDRFP4 16
INDIRF4
ARGF4
ADDRLP4 40
CNSTF4 0
ASGNF4
ADDRLP4 40
INDIRF4
ARGF4
ADDRLP4 44
CNSTF4 1065353216
ASGNF4
ADDRLP4 44
INDIRF4
ARGF4
ADDRLP4 44
INDIRF4
ARGF4
ADDRLP4 40
INDIRF4
ARGF4
ADDRGP4 cgs+474200+16
INDIRI4
ARGI4
ADDRGP4 trap_R_DrawStretchPic
CALLV
pop
ADDRFP4 0
INDIRF4
ARGF4
ADDRFP4 4
INDIRF4
ADDRFP4 16
INDIRF4
ADDF4
ARGF4
ADDRFP4 16
INDIRF4
ARGF4
ADDRFP4 12
INDIRF4
CNSTF4 1073741824
ADDRFP4 16
INDIRF4
MULF4
SUBF4
ARGF4
ADDRLP4 52
CNSTF4 0
ASGNF4
ADDRLP4 52
INDIRF4
ARGF4
ADDRLP4 52
INDIRF4
ARGF4
ADDRLP4 52
INDIRF4
ARGF4
ADDRLP4 52
INDIRF4
ARGF4
ADDRGP4 cgs+474200+12
INDIRI4
ARGI4
ADDRGP4 trap_R_DrawStretchPic
CALLV
pop
ADDRFP4 0
INDIRF4
ADDRFP4 8
INDIRF4
ADDF4
ADDRFP4 16
INDIRF4
SUBF4
ARGF4
ADDRFP4 4
INDIRF4
ADDRFP4 16
INDIRF4
ADDF4
ARGF4
ADDRFP4 16
INDIRF4
ARGF4
ADDRFP4 12
INDIRF4
CNSTF4 1073741824
ADDRFP4 16
INDIRF4
MULF4
SUBF4
ARGF4
ADDRLP4 60
CNSTF4 0
ASGNF4
ADDRLP4 60
INDIRF4
ARGF4
ADDRLP4 60
INDIRF4
ARGF4
ADDRLP4 60
INDIRF4
ARGF4
ADDRLP4 60
INDIRF4
ARGF4
ADDRGP4 cgs+474200+12
INDIRI4
ARGI4
ADDRGP4 trap_R_DrawStretchPic
CALLV
pop
ADDRFP4 0
INDIRF4
ADDRFP4 16
INDIRF4
ADDF4
ARGF4
ADDRFP4 4
INDIRF4
ARGF4
ADDRFP4 8
INDIRF4
CNSTF4 1073741824
ADDRFP4 16
INDIRF4
MULF4
SUBF4
ARGF4
ADDRFP4 12
INDIRF4
ARGF4
ADDRLP4 68
CNSTF4 0
ASGNF4
ADDRLP4 68
INDIRF4
ARGF4
ADDRLP4 68
INDIRF4
ARGF4
ADDRLP4 68
INDIRF4
ARGF4
ADDRLP4 68
INDIRF4
ARGF4
ADDRGP4 cgs+474200+12
INDIRI4
ARGI4
ADDRGP4 trap_R_DrawStretchPic
CALLV
pop
CNSTP4 0
ARGP4
ADDRGP4 trap_R_SetColor
CALLV
pop
LABELV $81
endproc CG_DrawRoundedRect 72 36
export CG_DrawProgressBar
proc CG_DrawProgressBar 52 36
ADDRFP4 16
ADDRFP4 16
INDIRF4
ASGNF4
ADDRFP4 20
ADDRFP4 20
INDIRF4
ASGNF4
ADDRLP4 4
CNSTF4 1073741824
ADDRGP4 cgs+71940
INDIRF4
MULF4
CVFI4 4
ASGNI4
ADDRLP4 16
CNSTF4 1077936128
ADDRGP4 cgs+71940
INDIRF4
MULF4
CVFI4 4
ASGNI4
ADDRLP4 20
CNSTF4 1081081856
ADDRGP4 cgs+71940
INDIRF4
MULF4
CVFI4 4
ASGNI4
ADDRFP4 16
INDIRF4
CNSTF4 0
GEF4 $105
ADDRFP4 16
CNSTF4 0
ASGNF4
LABELV $105
ADDRFP4 16
INDIRF4
CNSTF4 1065353216
LEF4 $107
ADDRFP4 16
CNSTF4 1065353216
ASGNF4
LABELV $107
ADDRFP4 0
ARGP4
ADDRFP4 4
ARGP4
ADDRFP4 8
ARGP4
ADDRFP4 12
ARGP4
ADDRGP4 CG_AdjustFrom640
CALLV
pop
ADDRFP4 20
ADDRFP4 20
INDIRF4
ADDRGP4 cgs+71940
INDIRF4
MULF4
ASGNF4
ADDRFP4 28
INDIRP4
ARGP4
ADDRGP4 trap_R_SetColor
CALLV
pop
ADDRFP4 0
INDIRF4
ARGF4
ADDRFP4 4
INDIRF4
ARGF4
ADDRFP4 8
INDIRF4
ARGF4
ADDRFP4 12
INDIRF4
ARGF4
ADDRLP4 28
CNSTF4 0
ASGNF4
ADDRLP4 28
INDIRF4
ARGF4
ADDRLP4 28
INDIRF4
ARGF4
ADDRLP4 32
CNSTF4 1065353216
ASGNF4
ADDRLP4 32
INDIRF4
ARGF4
ADDRLP4 32
INDIRF4
ARGF4
ADDRGP4 cgs+474200+12
INDIRI4
ARGI4
ADDRGP4 trap_R_DrawStretchPic
CALLV
pop
CNSTP4 0
ARGP4
ADDRGP4 trap_R_SetColor
CALLV
pop
ADDRLP4 24
ADDRFP4 8
INDIRF4
ADDRFP4 20
INDIRF4
DIVF4
CVFI4 4
ASGNI4
ADDRLP4 12
ADDRLP4 24
INDIRI4
CVIF4 4
ADDRFP4 16
INDIRF4
MULF4
CVFI4 4
ASGNI4
ADDRFP4 24
INDIRP4
ARGP4
ADDRGP4 trap_R_SetColor
CALLV
pop
ADDRLP4 0
CNSTI4 0
ASGNI4
ADDRGP4 $115
JUMPV
LABELV $112
ADDRLP4 8
ADDRFP4 0
INDIRF4
ADDRLP4 0
INDIRI4
CVIF4 4
ADDRFP4 20
INDIRF4
MULF4
ADDF4
ASGNF4
ADDRLP4 40
ADDRLP4 4
INDIRI4
CVIF4 4
ASGNF4
ADDRLP4 8
INDIRF4
ADDRLP4 40
INDIRF4
ADDF4
ARGF4
ADDRFP4 4
INDIRF4
ADDRLP4 40
INDIRF4
ADDF4
ARGF4
ADDRFP4 20
INDIRF4
ADDRLP4 16
INDIRI4
CVIF4 4
SUBF4
ARGF4
ADDRFP4 12
INDIRF4
ADDRLP4 20
INDIRI4
CVIF4 4
SUBF4
ARGF4
ADDRLP4 44
CNSTF4 0
ASGNF4
ADDRLP4 44
INDIRF4
ARGF4
ADDRLP4 44
INDIRF4
ARGF4
ADDRLP4 48
CNSTF4 1065353216
ASGNF4
ADDRLP4 48
INDIRF4
ARGF4
ADDRLP4 48
INDIRF4
ARGF4
ADDRGP4 cgs+474200+12
INDIRI4
ARGI4
ADDRGP4 trap_R_DrawStretchPic
CALLV
pop
LABELV $113
ADDRLP4 0
ADDRLP4 0
INDIRI4
CNSTI4 1
ADDI4
ASGNI4
LABELV $115
ADDRLP4 0
INDIRI4
ADDRLP4 12
INDIRI4
LTI4 $112
CNSTP4 0
ARGP4
ADDRGP4 trap_R_SetColor
CALLV
pop
LABELV $101
endproc CG_DrawProgressBar 52 36
export CG_DrawPic
proc CG_DrawPic 8 36
ADDRFP4 0
ARGP4
ADDRFP4 4
ARGP4
ADDRFP4 8
ARGP4
ADDRFP4 12
ARGP4
ADDRGP4 CG_AdjustFrom640
CALLV
pop
ADDRFP4 0
INDIRF4
ARGF4
ADDRFP4 4
INDIRF4
ARGF4
ADDRFP4 8
INDIRF4
ARGF4
ADDRFP4 12
INDIRF4
ARGF4
ADDRLP4 0
CNSTF4 0
ASGNF4
ADDRLP4 0
INDIRF4
ARGF4
ADDRLP4 0
INDIRF4
ARGF4
ADDRLP4 4
CNSTF4 1065353216
ASGNF4
ADDRLP4 4
INDIRF4
ARGF4
ADDRLP4 4
INDIRF4
ARGF4
ADDRFP4 16
INDIRI4
ARGI4
ADDRGP4 trap_R_DrawStretchPic
CALLV
pop
LABELV $118
endproc CG_DrawPic 8 36
export CG_Draw3DString
proc CG_Draw3DString 164 28
ADDRFP4 20
ADDRFP4 20
INDIRP4
ASGNP4
ADDRLP4 12
ADDRFP4 0
INDIRF4
ASGNF4
ADDRLP4 12+4
ADDRFP4 4
INDIRF4
ASGNF4
ADDRLP4 12+8
ADDRFP4 8
INDIRF4
ASGNF4
ADDRLP4 0
ADDRLP4 12
INDIRF4
ADDRGP4 cg+1837612+24
INDIRF4
SUBF4
ASGNF4
ADDRLP4 0+4
ADDRLP4 12+4
INDIRF4
ADDRGP4 cg+1837612+24+4
INDIRF4
SUBF4
ASGNF4
ADDRLP4 0+8
ADDRLP4 12+8
INDIRF4
ADDRGP4 cg+1837612+24+8
INDIRF4
SUBF4
ASGNF4
ADDRFP4 36
INDIRI4
CNSTI4 0
EQI4 $134
ADDRLP4 72
ARGP4
ADDRGP4 cg+1837612+24
ARGP4
ADDRLP4 128
ADDRGP4 vec3_origin
ASGNP4
ADDRLP4 128
INDIRP4
ARGP4
ADDRLP4 128
INDIRP4
ARGP4
ADDRLP4 12
ARGP4
ADDRGP4 cg+28
INDIRP4
CNSTI4 184
ADDP4
INDIRI4
ARGI4
CNSTI4 1
ARGI4
ADDRGP4 CG_Trace
CALLV
pop
ADDRLP4 72+8
INDIRF4
CNSTF4 1065353216
GEF4 $139
ADDRGP4 $119
JUMPV
LABELV $139
LABELV $134
ADDRLP4 128
ADDRLP4 0
INDIRF4
ASGNF4
ADDRLP4 48
ADDRLP4 128
INDIRF4
ADDRGP4 cg+1837612+36+12
INDIRF4
MULF4
ADDRLP4 0+4
INDIRF4
ADDRGP4 cg+1837612+36+12+4
INDIRF4
MULF4
ADDF4
ADDRLP4 0+8
INDIRF4
ADDRGP4 cg+1837612+36+12+8
INDIRF4
MULF4
ADDF4
NEGF4
ASGNF4
ADDRLP4 52
ADDRLP4 128
INDIRF4
ADDRGP4 cg+1837612+36+24
INDIRF4
MULF4
ADDRLP4 0+4
INDIRF4
ADDRGP4 cg+1837612+36+24+4
INDIRF4
MULF4
ADDF4
ADDRLP4 0+8
INDIRF4
ADDRGP4 cg+1837612+36+24+8
INDIRF4
MULF4
ADDF4
ASGNF4
ADDRLP4 40
ADDRLP4 128
INDIRF4
ADDRGP4 cg+1837612+36
INDIRF4
MULF4
ADDRLP4 0+4
INDIRF4
ADDRGP4 cg+1837612+36+4
INDIRF4
MULF4
ADDF4
ADDRLP4 0+8
INDIRF4
ADDRGP4 cg+1837612+36+8
INDIRF4
MULF4
ADDF4
ASGNF4
ADDRLP4 40
INDIRF4
CNSTF4 0
GTF4 $178
ADDRGP4 $119
JUMPV
LABELV $178
CNSTF4 1078530011
CNSTF4 1056964608
ADDRGP4 cg+1837612+16
INDIRF4
MULF4
MULF4
CNSTF4 1127481344
DIVF4
ARGF4
ADDRLP4 132
ADDRGP4 tan
CALLF4
ASGNF4
ADDRLP4 56
ADDRLP4 132
INDIRF4
ASGNF4
CNSTF4 1078530011
CNSTF4 1056964608
ADDRGP4 cg+1837612+20
INDIRF4
MULF4
MULF4
CNSTF4 1127481344
DIVF4
ARGF4
ADDRLP4 136
ADDRGP4 tan
CALLF4
ASGNF4
ADDRLP4 60
ADDRLP4 136
INDIRF4
ASGNF4
ADDRLP4 144
CNSTF4 1134559232
ASGNF4
ADDRLP4 64
ADDRLP4 48
INDIRF4
ADDRLP4 40
INDIRF4
ADDRLP4 56
INDIRF4
MULF4
DIVF4
ADDRGP4 cgs+71948
INDIRF4
ADDRLP4 144
INDIRF4
ADDF4
MULF4
ADDRLP4 144
INDIRF4
ADDF4
ASGNF4
ADDRLP4 148
CNSTF4 1131413504
ASGNF4
ADDRLP4 68
ADDRLP4 148
INDIRF4
ADDRLP4 52
INDIRF4
NEGF4
ADDRLP4 40
INDIRF4
ADDRLP4 60
INDIRF4
MULF4
DIVF4
MULF4
ADDRLP4 148
INDIRF4
ADDF4
ASGNF4
ADDRLP4 0
ARGP4
ADDRLP4 152
ADDRGP4 VectorLength
CALLF4
ASGNF4
ADDRLP4 44
ADDRLP4 152
INDIRF4
ASGNF4
ADDRLP4 24
ADDRFP4 20
INDIRP4
INDIRF4
ASGNF4
ADDRLP4 24+4
ADDRFP4 20
INDIRP4
CNSTI4 4
ADDP4
INDIRF4
ASGNF4
ADDRLP4 24+8
ADDRFP4 20
INDIRP4
CNSTI4 8
ADDP4
INDIRF4
ASGNF4
ADDRLP4 24+12
ADDRFP4 20
INDIRP4
CNSTI4 12
ADDP4
INDIRF4
ASGNF4
ADDRLP4 44
INDIRF4
ADDRFP4 28
INDIRF4
LEF4 $188
ADDRLP4 160
ADDRFP4 28
INDIRF4
ASGNF4
ADDRLP4 24+12
ADDRFP4 20
INDIRP4
CNSTI4 12
ADDP4
INDIRF4
CNSTF4 1065353216
ADDRLP4 44
INDIRF4
ADDRLP4 160
INDIRF4
SUBF4
ADDRFP4 32
INDIRF4
ADDRLP4 160
INDIRF4
SUBF4
DIVF4
SUBF4
MULF4
ASGNF4
ADDRLP4 24+12
INDIRF4
CNSTF4 0
GEF4 $191
ADDRLP4 24+12
CNSTF4 0
ASGNF4
LABELV $191
LABELV $188
ADDRLP4 64
INDIRF4
ARGF4
ADDRLP4 68
INDIRF4
ARGF4
ADDRFP4 12
INDIRP4
ARGP4
ADDRFP4 16
INDIRI4
CNSTI4 1
BORI4
ARGI4
ADDRLP4 24
ARGP4
ADDRFP4 24
INDIRF4
ARGF4
ADDRGP4 ST_DrawString
CALLV
pop
LABELV $119
endproc CG_Draw3DString 164 28
bss
align 4
LABELV $196
skip 16
export CG_FadeColor
code
proc CG_FadeColor 8 0
ADDRFP4 0
INDIRI4
CNSTI4 0
NEI4 $197
CNSTP4 0
RETP4
ADDRGP4 $195
JUMPV
LABELV $197
ADDRLP4 0
ADDRGP4 cg+1835980
INDIRI4
ADDRFP4 0
INDIRI4
SUBI4
ASGNI4
ADDRLP4 0
INDIRI4
ADDRFP4 4
INDIRI4
LTI4 $200
CNSTP4 0
RETP4
ADDRGP4 $195
JUMPV
LABELV $200
ADDRFP4 4
INDIRI4
ADDRLP4 0
INDIRI4
SUBI4
CNSTI4 500
GEI4 $202
ADDRGP4 $196+12
CNSTF4 1065353216
ADDRFP4 4
INDIRI4
ADDRLP4 0
INDIRI4
SUBI4
CVIF4 4
MULF4
CNSTF4 1140457472
DIVF4
ASGNF4
ADDRGP4 $203
JUMPV
LABELV $202
ADDRGP4 $196+12
CNSTF4 1065353216
ASGNF4
LABELV $203
ADDRLP4 4
CNSTF4 1065353216
ASGNF4
ADDRGP4 $196+8
ADDRLP4 4
INDIRF4
ASGNF4
ADDRGP4 $196+4
ADDRLP4 4
INDIRF4
ASGNF4
ADDRGP4 $196
ADDRLP4 4
INDIRF4
ASGNF4
ADDRGP4 $196
RETP4
LABELV $195
endproc CG_FadeColor 8 0
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
