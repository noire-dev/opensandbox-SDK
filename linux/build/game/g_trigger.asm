code
proc InitTrigger 12 8
ADDRFP4 0
ADDRFP4 0
INDIRP4
ASGNP4
ADDRFP4 0
INDIRP4
CNSTI4 128
ADDP4
ARGP4
ADDRGP4 vec3_origin
ARGP4
ADDRLP4 0
ADDRGP4 VectorCompare
CALLI4
ASGNI4
ADDRLP4 0
INDIRI4
CNSTI4 0
NEI4 $55
ADDRFP4 0
INDIRP4
CNSTI4 128
ADDP4
ARGP4
ADDRFP4 0
INDIRP4
CNSTI4 696
ADDP4
ARGP4
ADDRGP4 G_SetMovedir
CALLV
pop
LABELV $55
ADDRFP4 0
INDIRP4
ARGP4
ADDRFP4 0
INDIRP4
CNSTI4 572
ADDP4
INDIRP4
ARGP4
ADDRGP4 trap_SetBrushModel
CALLV
pop
ADDRFP4 0
INDIRP4
CNSTI4 492
ADDP4
CNSTI4 1073741824
ASGNI4
ADDRFP4 0
INDIRP4
CNSTI4 456
ADDP4
CNSTI4 1
ASGNI4
LABELV $54
endproc InitTrigger 12 8
proc multi_wait 0 0
ADDRFP4 0
INDIRP4
CNSTI4 708
ADDP4
CNSTI4 0
ASGNI4
LABELV $57
endproc multi_wait 0 0
proc multi_trigger 12 8
ADDRFP4 0
ADDRFP4 0
INDIRP4
ASGNP4
ADDRFP4 4
ADDRFP4 4
INDIRP4
ASGNP4
ADDRFP4 0
INDIRP4
CNSTI4 788
ADDP4
ADDRFP4 4
INDIRP4
ASGNP4
ADDRFP4 0
INDIRP4
CNSTI4 708
ADDP4
INDIRI4
CNSTI4 0
EQI4 $59
ADDRGP4 $58
JUMPV
LABELV $59
ADDRFP4 4
INDIRP4
CNSTI4 548
ADDP4
INDIRP4
CVPU4 4
CNSTU4 0
EQU4 $61
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
EQI4 $63
ADDRFP4 4
INDIRP4
CNSTI4 548
ADDP4
INDIRP4
CNSTI4 1512
ADDP4
INDIRI4
ADDRLP4 0
INDIRI4
EQI4 $63
ADDRGP4 $58
JUMPV
LABELV $63
ADDRLP4 4
CNSTI4 2
ASGNI4
ADDRFP4 0
INDIRP4
CNSTI4 560
ADDP4
INDIRI4
ADDRLP4 4
INDIRI4
BANDI4
CNSTI4 0
EQI4 $65
ADDRFP4 4
INDIRP4
CNSTI4 548
ADDP4
INDIRP4
CNSTI4 1512
ADDP4
INDIRI4
ADDRLP4 4
INDIRI4
EQI4 $65
ADDRGP4 $58
JUMPV
LABELV $65
LABELV $61
ADDRFP4 0
INDIRP4
ARGP4
ADDRFP4 0
INDIRP4
CNSTI4 788
ADDP4
INDIRP4
ARGP4
ADDRGP4 G_UseTargets
CALLV
pop
ADDRFP4 0
INDIRP4
CNSTI4 820
ADDP4
INDIRF4
CNSTF4 0
LEF4 $67
ADDRFP4 0
INDIRP4
CNSTI4 712
ADDP4
ADDRGP4 multi_wait
ASGNP4
ADDRLP4 4
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
ADDRLP4 4
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
ADDRGP4 $68
JUMPV
LABELV $67
ADDRFP4 0
INDIRP4
CNSTI4 724
ADDP4
CNSTP4 0
ASGNP4
ADDRFP4 0
INDIRP4
CNSTI4 708
ADDP4
ADDRGP4 level+24
INDIRI4
CNSTI4 100
ADDI4
ASGNI4
ADDRFP4 0
INDIRP4
CNSTI4 712
ADDP4
ADDRGP4 G_FreeEntity
ASGNP4
LABELV $68
LABELV $58
endproc multi_trigger 12 8
proc Use_Multi 0 8
ADDRFP4 0
INDIRP4
ARGP4
ADDRFP4 8
INDIRP4
ARGP4
ADDRGP4 multi_trigger
CALLV
pop
LABELV $71
endproc Use_Multi 0 8
proc Touch_Multi 0 8
ADDRFP4 4
INDIRP4
CNSTI4 548
ADDP4
INDIRP4
CVPU4 4
CNSTU4 0
NEU4 $73
ADDRGP4 $72
JUMPV
LABELV $73
ADDRFP4 0
INDIRP4
ARGP4
ADDRFP4 4
INDIRP4
ARGP4
ADDRGP4 multi_trigger
CALLV
pop
LABELV $72
endproc Touch_Multi 0 8
export SP_trigger_multiple
proc SP_trigger_multiple 12 12
ADDRFP4 0
ADDRFP4 0
INDIRP4
ASGNP4
ADDRGP4 $76
ARGP4
ADDRGP4 $77
ARGP4
ADDRFP4 0
INDIRP4
CNSTI4 820
ADDP4
ARGP4
ADDRGP4 G_SpawnFloat
CALLI4
pop
ADDRGP4 $78
ARGP4
ADDRGP4 $79
ARGP4
ADDRFP4 0
INDIRP4
CNSTI4 824
ADDP4
ARGP4
ADDRGP4 G_SpawnFloat
CALLI4
pop
ADDRLP4 4
ADDRFP4 0
INDIRP4
CNSTI4 820
ADDP4
INDIRF4
ASGNF4
ADDRFP4 0
INDIRP4
CNSTI4 824
ADDP4
INDIRF4
ADDRLP4 4
INDIRF4
LTF4 $80
ADDRLP4 4
INDIRF4
CNSTF4 0
LTF4 $80
ADDRFP4 0
INDIRP4
CNSTI4 824
ADDP4
ADDRFP4 0
INDIRP4
CNSTI4 820
ADDP4
INDIRF4
CNSTF4 1120403456
SUBF4
ASGNF4
ADDRGP4 $82
ARGP4
ADDRGP4 G_Printf
CALLV
pop
LABELV $80
ADDRFP4 0
INDIRP4
CNSTI4 724
ADDP4
ADDRGP4 Touch_Multi
ASGNP4
ADDRFP4 0
INDIRP4
CNSTI4 728
ADDP4
ADDRGP4 Use_Multi
ASGNP4
ADDRFP4 0
INDIRP4
ARGP4
ADDRGP4 InitTrigger
CALLV
pop
ADDRFP4 0
INDIRP4
ARGP4
ADDRGP4 trap_LinkEntity
CALLV
pop
LABELV $75
endproc SP_trigger_multiple 12 12
proc trigger_always_think 4 8
ADDRFP4 0
ADDRFP4 0
INDIRP4
ASGNP4
ADDRFP4 0
INDIRP4
ARGP4
ADDRFP4 0
INDIRP4
ARGP4
ADDRGP4 G_UseTargets
CALLV
pop
ADDRFP4 0
INDIRP4
ARGP4
ADDRGP4 G_FreeEntity
CALLV
pop
LABELV $83
endproc trigger_always_think 4 8
export SP_trigger_always
proc SP_trigger_always 0 0
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
ADDRGP4 trigger_always_think
ASGNP4
LABELV $84
endproc SP_trigger_always 0 0
proc trigger_push_touch 0 8
ADDRFP4 4
INDIRP4
CNSTI4 548
ADDP4
INDIRP4
CVPU4 4
CNSTU4 0
NEU4 $87
ADDRGP4 $86
JUMPV
LABELV $87
ADDRFP4 4
INDIRP4
CNSTI4 548
ADDP4
INDIRP4
ARGP4
ADDRFP4 0
INDIRP4
ARGP4
ADDRGP4 BG_TouchJumpPad
CALLV
pop
LABELV $86
endproc trigger_push_touch 0 8
proc AimAtTarget 76 4
ADDRFP4 0
ADDRFP4 0
INDIRP4
ASGNP4
ADDRLP4 0
ADDRFP4 0
INDIRP4
CNSTI4 496
ADDP4
INDIRF4
ADDRFP4 0
INDIRP4
CNSTI4 508
ADDP4
INDIRF4
ADDF4
ASGNF4
ADDRLP4 0+4
ADDRFP4 0
INDIRP4
CNSTI4 500
ADDP4
INDIRF4
ADDRFP4 0
INDIRP4
CNSTI4 512
ADDP4
INDIRF4
ADDF4
ASGNF4
ADDRLP4 0+8
ADDRFP4 0
INDIRP4
CNSTI4 504
ADDP4
INDIRF4
ADDRFP4 0
INDIRP4
CNSTI4 516
ADDP4
INDIRF4
ADDF4
ASGNF4
ADDRLP4 44
CNSTF4 1056964608
ASGNF4
ADDRLP4 0
ADDRLP4 44
INDIRF4
ADDRLP4 0
INDIRF4
MULF4
ASGNF4
ADDRLP4 0+4
ADDRLP4 44
INDIRF4
ADDRLP4 0+4
INDIRF4
MULF4
ASGNF4
ADDRLP4 0+8
CNSTF4 1056964608
ADDRLP4 0+8
INDIRF4
MULF4
ASGNF4
ADDRFP4 0
INDIRP4
CNSTI4 676
ADDP4
INDIRP4
ARGP4
ADDRLP4 48
ADDRGP4 G_PickTarget
CALLP4
ASGNP4
ADDRLP4 12
ADDRLP4 48
INDIRP4
ASGNP4
ADDRLP4 12
INDIRP4
CVPU4 4
CNSTU4 0
NEU4 $96
ADDRFP4 0
INDIRP4
ARGP4
ADDRGP4 G_FreeEntity
CALLV
pop
ADDRGP4 $89
JUMPV
LABELV $96
ADDRLP4 28
ADDRLP4 12
INDIRP4
CNSTI4 100
ADDP4
INDIRF4
ADDRLP4 0+8
INDIRF4
SUBF4
ASGNF4
ADDRGP4 $99
ARGP4
ADDRLP4 52
ADDRGP4 cvarFloat
CALLF4
ASGNF4
ADDRLP4 24
ADDRLP4 52
INDIRF4
ASGNF4
ADDRLP4 28
INDIRF4
CNSTF4 1056964608
ADDRLP4 24
INDIRF4
MULF4
DIVF4
ARGF4
ADDRLP4 56
ADDRGP4 sqrt
CALLF4
ASGNF4
ADDRLP4 16
ADDRLP4 56
INDIRF4
ASGNF4
ADDRLP4 16
INDIRF4
CNSTF4 0
NEF4 $100
ADDRFP4 0
INDIRP4
ARGP4
ADDRGP4 G_FreeEntity
CALLV
pop
ADDRGP4 $89
JUMPV
LABELV $100
ADDRFP4 0
INDIRP4
CNSTI4 104
ADDP4
ADDRLP4 12
INDIRP4
CNSTI4 92
ADDP4
INDIRF4
ADDRLP4 0
INDIRF4
SUBF4
ASGNF4
ADDRFP4 0
INDIRP4
CNSTI4 108
ADDP4
ADDRLP4 12
INDIRP4
CNSTI4 96
ADDP4
INDIRF4
ADDRLP4 0+4
INDIRF4
SUBF4
ASGNF4
ADDRFP4 0
INDIRP4
CNSTI4 112
ADDP4
ADDRLP4 12
INDIRP4
CNSTI4 100
ADDP4
INDIRF4
ADDRLP4 0+8
INDIRF4
SUBF4
ASGNF4
ADDRFP4 0
INDIRP4
CNSTI4 112
ADDP4
CNSTF4 0
ASGNF4
ADDRFP4 0
INDIRP4
CNSTI4 104
ADDP4
ARGP4
ADDRLP4 60
ADDRGP4 VectorNormalize
CALLF4
ASGNF4
ADDRLP4 32
ADDRLP4 60
INDIRF4
ASGNF4
ADDRLP4 20
ADDRLP4 32
INDIRF4
ADDRLP4 16
INDIRF4
DIVF4
ASGNF4
ADDRLP4 64
ADDRFP4 0
INDIRP4
CNSTI4 104
ADDP4
ASGNP4
ADDRLP4 64
INDIRP4
ADDRLP4 64
INDIRP4
INDIRF4
ADDRLP4 20
INDIRF4
MULF4
ASGNF4
ADDRLP4 68
ADDRFP4 0
INDIRP4
CNSTI4 108
ADDP4
ASGNP4
ADDRLP4 68
INDIRP4
ADDRLP4 68
INDIRP4
INDIRF4
ADDRLP4 20
INDIRF4
MULF4
ASGNF4
ADDRLP4 72
ADDRFP4 0
INDIRP4
CNSTI4 112
ADDP4
ASGNP4
ADDRLP4 72
INDIRP4
ADDRLP4 72
INDIRP4
INDIRF4
ADDRLP4 20
INDIRF4
MULF4
ASGNF4
ADDRFP4 0
INDIRP4
CNSTI4 112
ADDP4
ADDRLP4 16
INDIRF4
ADDRLP4 24
INDIRF4
MULF4
ASGNF4
LABELV $89
endproc AimAtTarget 76 4
export SP_trigger_push
proc SP_trigger_push 4 4
ADDRFP4 0
ADDRFP4 0
INDIRP4
ASGNP4
ADDRFP4 0
INDIRP4
ARGP4
ADDRGP4 InitTrigger
CALLV
pop
ADDRLP4 0
ADDRFP4 0
INDIRP4
CNSTI4 456
ADDP4
ASGNP4
ADDRLP4 0
INDIRP4
ADDRLP4 0
INDIRP4
INDIRI4
CNSTI4 -2
BANDI4
ASGNI4
ADDRGP4 $105
ARGP4
ADDRGP4 G_SoundIndex
CALLI4
pop
ADDRFP4 0
INDIRP4
CNSTI4 4
ADDP4
CNSTI4 7
ASGNI4
ADDRFP4 0
INDIRP4
CNSTI4 724
ADDP4
ADDRGP4 trigger_push_touch
ASGNP4
ADDRFP4 0
INDIRP4
CNSTI4 712
ADDP4
ADDRGP4 AimAtTarget
ASGNP4
ADDRFP4 0
INDIRP4
CNSTI4 708
ADDP4
ADDRGP4 level+24
INDIRI4
CNSTI4 100
ADDI4
ASGNI4
ADDRFP4 0
INDIRP4
ARGP4
ADDRGP4 trap_LinkEntity
CALLV
pop
LABELV $104
endproc SP_trigger_push 4 4
proc Use_target_push 0 12
ADDRFP4 0
ADDRFP4 0
INDIRP4
ASGNP4
ADDRFP4 8
ADDRFP4 8
INDIRP4
ASGNP4
ADDRFP4 8
INDIRP4
CNSTI4 548
ADDP4
INDIRP4
CVPU4 4
CNSTU4 0
NEU4 $108
ADDRGP4 $107
JUMPV
LABELV $108
ADDRFP4 8
INDIRP4
CNSTI4 548
ADDP4
INDIRP4
CNSTI4 4
ADDP4
INDIRI4
CNSTI4 0
EQI4 $110
ADDRGP4 $107
JUMPV
LABELV $110
ADDRFP4 8
INDIRP4
CNSTI4 548
ADDP4
INDIRP4
CNSTI4 336
ADDP4
INDIRI4
CNSTI4 0
EQI4 $112
ADDRGP4 $107
JUMPV
LABELV $112
ADDRFP4 8
INDIRP4
CNSTI4 548
ADDP4
INDIRP4
CNSTI4 32
ADDP4
ADDRFP4 0
INDIRP4
CNSTI4 104
ADDP4
INDIRF4
ASGNF4
ADDRFP4 8
INDIRP4
CNSTI4 548
ADDP4
INDIRP4
CNSTI4 36
ADDP4
ADDRFP4 0
INDIRP4
CNSTI4 108
ADDP4
INDIRF4
ASGNF4
ADDRFP4 8
INDIRP4
CNSTI4 548
ADDP4
INDIRP4
CNSTI4 40
ADDP4
ADDRFP4 0
INDIRP4
CNSTI4 112
ADDP4
INDIRF4
ASGNF4
ADDRFP4 8
INDIRP4
CNSTI4 744
ADDP4
INDIRI4
ADDRGP4 level+24
INDIRI4
GEI4 $114
ADDRFP4 8
INDIRP4
CNSTI4 744
ADDP4
ADDRGP4 level+24
INDIRI4
CNSTI4 1500
ADDI4
ASGNI4
ADDRFP4 8
INDIRP4
ARGP4
CNSTI4 0
ARGI4
ADDRFP4 0
INDIRP4
CNSTI4 816
ADDP4
INDIRI4
ARGI4
ADDRGP4 G_Sound
CALLV
pop
LABELV $114
LABELV $107
endproc Use_target_push 0 12
export SP_target_push
proc SP_target_push 52 8
ADDRFP4 0
ADDRFP4 0
INDIRP4
ASGNP4
ADDRFP4 0
INDIRP4
CNSTI4 692
ADDP4
INDIRF4
CNSTF4 0
NEF4 $119
ADDRFP4 0
INDIRP4
CNSTI4 692
ADDP4
CNSTF4 1148846080
ASGNF4
LABELV $119
ADDRFP4 0
INDIRP4
CNSTI4 128
ADDP4
ARGP4
ADDRFP4 0
INDIRP4
CNSTI4 104
ADDP4
ARGP4
ADDRGP4 G_SetMovedir
CALLV
pop
ADDRLP4 8
ADDRFP4 0
INDIRP4
CNSTI4 104
ADDP4
ASGNP4
ADDRLP4 8
INDIRP4
ADDRLP4 8
INDIRP4
INDIRF4
ADDRFP4 0
INDIRP4
CNSTI4 692
ADDP4
INDIRF4
MULF4
ASGNF4
ADDRLP4 16
ADDRFP4 0
INDIRP4
CNSTI4 108
ADDP4
ASGNP4
ADDRLP4 16
INDIRP4
ADDRLP4 16
INDIRP4
INDIRF4
ADDRFP4 0
INDIRP4
CNSTI4 692
ADDP4
INDIRF4
MULF4
ASGNF4
ADDRLP4 24
ADDRFP4 0
INDIRP4
CNSTI4 112
ADDP4
ASGNP4
ADDRLP4 24
INDIRP4
ADDRLP4 24
INDIRP4
INDIRF4
ADDRFP4 0
INDIRP4
CNSTI4 692
ADDP4
INDIRF4
MULF4
ASGNF4
ADDRFP4 0
INDIRP4
CNSTI4 560
ADDP4
INDIRI4
CNSTI4 1
BANDI4
CNSTI4 0
EQI4 $121
ADDRGP4 $105
ARGP4
ADDRLP4 28
ADDRGP4 G_SoundIndex
CALLI4
ASGNI4
ADDRFP4 0
INDIRP4
CNSTI4 816
ADDP4
ADDRLP4 28
INDIRI4
ASGNI4
ADDRGP4 $122
JUMPV
LABELV $121
ADDRGP4 $123
ARGP4
ADDRLP4 28
ADDRGP4 G_SoundIndex
CALLI4
ASGNI4
ADDRFP4 0
INDIRP4
CNSTI4 816
ADDP4
ADDRLP4 28
INDIRI4
ASGNI4
LABELV $122
ADDRFP4 0
INDIRP4
CNSTI4 676
ADDP4
INDIRP4
CVPU4 4
CNSTU4 0
EQU4 $124
ADDRFP4 0
INDIRP4
CNSTI4 496
ADDP4
ADDRFP4 0
INDIRP4
CNSTI4 92
ADDP4
INDIRF4
ASGNF4
ADDRFP4 0
INDIRP4
CNSTI4 500
ADDP4
ADDRFP4 0
INDIRP4
CNSTI4 96
ADDP4
INDIRF4
ASGNF4
ADDRFP4 0
INDIRP4
CNSTI4 504
ADDP4
ADDRFP4 0
INDIRP4
CNSTI4 100
ADDP4
INDIRF4
ASGNF4
ADDRFP4 0
INDIRP4
CNSTI4 508
ADDP4
ADDRFP4 0
INDIRP4
CNSTI4 92
ADDP4
INDIRF4
ASGNF4
ADDRFP4 0
INDIRP4
CNSTI4 512
ADDP4
ADDRFP4 0
INDIRP4
CNSTI4 96
ADDP4
INDIRF4
ASGNF4
ADDRFP4 0
INDIRP4
CNSTI4 516
ADDP4
ADDRFP4 0
INDIRP4
CNSTI4 100
ADDP4
INDIRF4
ASGNF4
ADDRFP4 0
INDIRP4
CNSTI4 712
ADDP4
ADDRGP4 AimAtTarget
ASGNP4
ADDRFP4 0
INDIRP4
CNSTI4 708
ADDP4
ADDRGP4 level+24
INDIRI4
CNSTI4 100
ADDI4
ASGNI4
LABELV $124
ADDRFP4 0
INDIRP4
CNSTI4 728
ADDP4
ADDRGP4 Use_target_push
ASGNP4
LABELV $118
endproc SP_target_push 52 8
proc trigger_teleporter_touch 12 16
ADDRFP4 4
ADDRFP4 4
INDIRP4
ASGNP4
ADDRFP4 4
INDIRP4
CNSTI4 548
ADDP4
INDIRP4
CVPU4 4
CNSTU4 0
NEU4 $128
ADDRGP4 $127
JUMPV
LABELV $128
ADDRFP4 4
INDIRP4
CNSTI4 548
ADDP4
INDIRP4
CNSTI4 4
ADDP4
INDIRI4
CNSTI4 3
NEI4 $130
ADDRGP4 $127
JUMPV
LABELV $130
ADDRFP4 0
INDIRP4
CNSTI4 560
ADDP4
INDIRI4
CNSTI4 1
BANDI4
CNSTI4 0
EQI4 $132
ADDRFP4 4
INDIRP4
CNSTI4 548
ADDP4
INDIRP4
CNSTI4 1512
ADDP4
INDIRI4
CNSTI4 3
EQI4 $132
ADDRGP4 $127
JUMPV
LABELV $132
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
NEU4 $134
ADDRGP4 $136
ARGP4
ADDRGP4 G_Printf
CALLV
pop
ADDRGP4 $127
JUMPV
LABELV $134
ADDRFP4 4
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
LABELV $127
endproc trigger_teleporter_touch 12 16
export SP_trigger_teleport
proc SP_trigger_teleport 4 4
ADDRFP4 0
ADDRFP4 0
INDIRP4
ASGNP4
ADDRFP4 0
INDIRP4
ARGP4
ADDRGP4 InitTrigger
CALLV
pop
ADDRFP4 0
INDIRP4
CNSTI4 560
ADDP4
INDIRI4
CNSTI4 1
BANDI4
CNSTI4 0
EQI4 $138
ADDRLP4 0
ADDRFP4 0
INDIRP4
CNSTI4 456
ADDP4
ASGNP4
ADDRLP4 0
INDIRP4
ADDRLP4 0
INDIRP4
INDIRI4
CNSTI4 1
BORI4
ASGNI4
ADDRGP4 $139
JUMPV
LABELV $138
ADDRLP4 0
ADDRFP4 0
INDIRP4
CNSTI4 456
ADDP4
ASGNP4
ADDRLP4 0
INDIRP4
ADDRLP4 0
INDIRP4
INDIRI4
CNSTI4 -2
BANDI4
ASGNI4
LABELV $139
ADDRGP4 $105
ARGP4
ADDRGP4 G_SoundIndex
CALLI4
pop
ADDRFP4 0
INDIRP4
CNSTI4 4
ADDP4
CNSTI4 8
ASGNI4
ADDRFP4 0
INDIRP4
CNSTI4 724
ADDP4
ADDRGP4 trigger_teleporter_touch
ASGNP4
ADDRFP4 0
INDIRP4
ARGP4
ADDRGP4 trap_LinkEntity
CALLV
pop
LABELV $137
endproc SP_trigger_teleport 4 4
proc hurt_use 0 4
ADDRFP4 0
INDIRP4
CNSTI4 448
ADDP4
INDIRI4
CNSTI4 0
EQI4 $141
ADDRFP4 0
INDIRP4
ARGP4
ADDRGP4 trap_UnlinkEntity
CALLV
pop
ADDRGP4 $142
JUMPV
LABELV $141
ADDRFP4 0
INDIRP4
ARGP4
ADDRGP4 trap_LinkEntity
CALLV
pop
LABELV $142
LABELV $140
endproc hurt_use 0 4
proc hurt_touch 12 32
ADDRFP4 0
ADDRFP4 0
INDIRP4
ASGNP4
ADDRFP4 4
INDIRP4
CNSTI4 756
ADDP4
INDIRI4
CNSTI4 0
NEI4 $144
ADDRGP4 $143
JUMPV
LABELV $144
ADDRFP4 0
INDIRP4
CNSTI4 668
ADDP4
INDIRI4
ADDRGP4 level+24
INDIRI4
LEI4 $146
ADDRGP4 $143
JUMPV
LABELV $146
ADDRFP4 0
INDIRP4
CNSTI4 560
ADDP4
INDIRI4
CNSTI4 16
BANDI4
CNSTI4 0
EQI4 $149
ADDRFP4 0
INDIRP4
CNSTI4 668
ADDP4
ADDRGP4 level+24
INDIRI4
CNSTI4 1000
ADDI4
ASGNI4
ADDRGP4 $150
JUMPV
LABELV $149
ADDRFP4 0
INDIRP4
CNSTI4 668
ADDP4
ADDRGP4 level+24
INDIRI4
CNSTI4 100
ADDI4
ASGNI4
LABELV $150
ADDRFP4 0
INDIRP4
CNSTI4 560
ADDP4
INDIRI4
CNSTI4 4
BANDI4
CNSTI4 0
NEI4 $153
ADDRFP4 4
INDIRP4
ARGP4
CNSTI4 0
ARGI4
ADDRFP4 0
INDIRP4
CNSTI4 816
ADDP4
INDIRI4
ARGI4
ADDRGP4 G_Sound
CALLV
pop
LABELV $153
ADDRFP4 0
INDIRP4
CNSTI4 560
ADDP4
INDIRI4
CNSTI4 8
BANDI4
CNSTI4 0
EQI4 $155
ADDRLP4 0
CNSTI4 8
ASGNI4
ADDRGP4 $156
JUMPV
LABELV $155
ADDRLP4 0
CNSTI4 0
ASGNI4
LABELV $156
ADDRFP4 4
INDIRP4
ARGP4
ADDRFP4 0
INDIRP4
ARGP4
ADDRFP4 0
INDIRP4
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
ADDRFP4 0
INDIRP4
CNSTI4 760
ADDP4
INDIRI4
ARGI4
ADDRLP4 0
INDIRI4
ARGI4
CNSTI4 35
ARGI4
ADDRGP4 G_Damage
CALLV
pop
LABELV $143
endproc hurt_touch 12 32
export SP_trigger_hurt
proc SP_trigger_hurt 4 4
ADDRFP4 0
ADDRFP4 0
INDIRP4
ASGNP4
ADDRFP4 0
INDIRP4
ARGP4
ADDRGP4 InitTrigger
CALLV
pop
ADDRGP4 $158
ARGP4
ADDRLP4 0
ADDRGP4 G_SoundIndex
CALLI4
ASGNI4
ADDRFP4 0
INDIRP4
CNSTI4 816
ADDP4
ADDRLP4 0
INDIRI4
ASGNI4
ADDRFP4 0
INDIRP4
CNSTI4 724
ADDP4
ADDRGP4 hurt_touch
ASGNP4
ADDRFP4 0
INDIRP4
CNSTI4 760
ADDP4
INDIRI4
CNSTI4 0
NEI4 $159
ADDRFP4 0
INDIRP4
CNSTI4 760
ADDP4
CNSTI4 5
ASGNI4
LABELV $159
ADDRFP4 0
INDIRP4
CNSTI4 492
ADDP4
CNSTI4 1073741824
ASGNI4
ADDRFP4 0
INDIRP4
CNSTI4 560
ADDP4
INDIRI4
CNSTI4 2
BANDI4
CNSTI4 0
EQI4 $161
ADDRFP4 0
INDIRP4
CNSTI4 728
ADDP4
ADDRGP4 hurt_use
ASGNP4
LABELV $161
ADDRFP4 0
INDIRP4
CNSTI4 560
ADDP4
INDIRI4
CNSTI4 1
BANDI4
CNSTI4 0
NEI4 $163
ADDRFP4 0
INDIRP4
ARGP4
ADDRGP4 trap_LinkEntity
CALLV
pop
LABELV $163
LABELV $157
endproc SP_trigger_hurt 4 4
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
LABELV $158
byte 1 115
byte 1 111
byte 1 117
byte 1 110
byte 1 100
byte 1 47
byte 1 119
byte 1 111
byte 1 114
byte 1 108
byte 1 100
byte 1 47
byte 1 101
byte 1 108
byte 1 101
byte 1 99
byte 1 116
byte 1 114
byte 1 111
byte 1 46
byte 1 119
byte 1 97
byte 1 118
byte 1 0
align 1
LABELV $136
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
LABELV $123
byte 1 115
byte 1 111
byte 1 117
byte 1 110
byte 1 100
byte 1 47
byte 1 109
byte 1 105
byte 1 115
byte 1 99
byte 1 47
byte 1 119
byte 1 105
byte 1 110
byte 1 100
byte 1 102
byte 1 108
byte 1 121
byte 1 46
byte 1 119
byte 1 97
byte 1 118
byte 1 0
align 1
LABELV $105
byte 1 115
byte 1 111
byte 1 117
byte 1 110
byte 1 100
byte 1 47
byte 1 119
byte 1 111
byte 1 114
byte 1 108
byte 1 100
byte 1 47
byte 1 106
byte 1 117
byte 1 109
byte 1 112
byte 1 112
byte 1 97
byte 1 100
byte 1 46
byte 1 119
byte 1 97
byte 1 118
byte 1 0
align 1
LABELV $99
byte 1 103
byte 1 95
byte 1 103
byte 1 114
byte 1 97
byte 1 118
byte 1 105
byte 1 116
byte 1 121
byte 1 0
align 1
LABELV $82
byte 1 116
byte 1 114
byte 1 105
byte 1 103
byte 1 103
byte 1 101
byte 1 114
byte 1 95
byte 1 109
byte 1 117
byte 1 108
byte 1 116
byte 1 105
byte 1 112
byte 1 108
byte 1 101
byte 1 32
byte 1 104
byte 1 97
byte 1 115
byte 1 32
byte 1 114
byte 1 97
byte 1 110
byte 1 100
byte 1 111
byte 1 109
byte 1 32
byte 1 62
byte 1 61
byte 1 32
byte 1 119
byte 1 97
byte 1 105
byte 1 116
byte 1 10
byte 1 0
align 1
LABELV $79
byte 1 48
byte 1 0
align 1
LABELV $78
byte 1 114
byte 1 97
byte 1 110
byte 1 100
byte 1 111
byte 1 109
byte 1 0
align 1
LABELV $77
byte 1 48
byte 1 46
byte 1 53
byte 1 0
align 1
LABELV $76
byte 1 119
byte 1 97
byte 1 105
byte 1 116
byte 1 0
