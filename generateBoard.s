	B test_generateBoard


; Our board 
; 0, represents an empty space
; 1-8 represents the number of bombs around us
; 66 represents there is a bomb at this location
; No more than 8 bombs
testGenboard   
	DEFW  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
tgbBoardMask
	DEFW  0,0,0,0,0, 0,0, 0,0,0,0,0,0,0, 0,0,0,0,0,0, 0,0,0,0, 0, 0,0,0,0,0,0,0,0,0,0, 0, 0,0,0,0,0, 0,0, 0,0,0,0,0,0,0, 0,0,0,0,0,0, 0,0,0,0, 0, 0,0,0

	ALIGN
test_generateBoard
	MOV R13, #0x10000
	ADR R0, testGenboard 
	BL generateBoard

	ADR R0, testGenboard 
	ADR R1, tgbBoardMask
	BL printMaskedBoard
	SWI 2

; --- * --- * --- * --- * --- * ---
; generateBoard
; Input R0 -- array to generate board in
generateBoard
	STMFD R13!, {R4-R8, LR}
;	MOV R1,#0
;	MOV R2,#0
;gb_Clear
;	STR R2,[R0, R1 LSL #2] 
;	ADD R1,R1, #1
;	CMP R1,#64
;	BLT gb_Clear
	BL generateBoardPopulateMines
	BL generateBoardNumbers
	
	LDMFD R13!, {R4-R8, PC}

; --- * --- * --- * --- * --- * ---
; populate the board with R1 mines
; Input: R0 --> board
; Output: R0 <-- board
generateBoardPopulateMines
	STMFD R13!, {LR}
	MOV R4, R0
	MOV R5, #0
	MOV R7, #-1
generateBoardPMCond
generateBoardTakenIndex
	BL randu
	MOV R0, R0, ASR #8 ; right shift 8 for increased random
	AND R0, R0, #0x3f  ; mod 64
	MOV R0, R0 LSL #2  ; index * 4
	LDR R6, [R4, R0]
	CMP R6, #0
	BNE generateBoardTakenIndex
	STR R7, [R4, R0]
	ADD R5, R5, #1
	CMP R5, #8
	BNE generateBoardPMCond
	MOV R0, R4
	LDMFD R13!, {PC}
	
; --- * --- * --- * --- * --- * ---
; find mines and call to
; Input: R0 --> board
; Output: R0 <-- board
generateBoardNumbers
	STMFD R13!, {LR}
	MOV R5, #0
generateBoard_X
	MOV R6, #0
	MOV R4, R5, LSL #3
generateBoard_Y
	ADD R4, R4, R6
	MOV R4, R4 LSL #2
	LDR R7, [R0, R4]
	MOV R2, R5
	MOV R3, R6
	CMP R7, #-1
	BLNE generateBoardInc

	ADD R6, R6, #1
	CMP R6, #8
	BNE generateBoard_Y
	ADD R5, R5, #1
	CMP R5, #8 
	BNE generateBoard_X

	LDMFD R13!, {PC}

; --- * --- * --- * --- * --- * ---
; increment in a square around R1
; Input: R0 --> board
;        R2 --> row 
;        R3 --> column
; Output: R0 <-- board
generateBoardInc
	STMFD R13!, {R4-R7, LR}
	SUB R2, R2, #1
	SUB R3, R3, #1
	CMP R2, #-1
	MOVEQ R2, #7
	CMP R3, #-1
	MOVEQ R3, #7
	MOV R5, #1
generateBoardInc_X
	CMP R2, #8
	MOVEQ R2, #0
	MOV R6, #1
generateBoardInc_Y
	CMP R3, #8
	MOVEQ R3, #0
; 4 * ( 8r + c )
	MOV R4, R2 LSL #3
	ADD R4, R4, R3
	MOV R4, R4 LSL #2
	LDR R1, [R0, R4]
	CMP R1, #-1
	ADDNE R1, R1, #1
	STR R1, [R0, R4]
; inc pointers
	ADD R3, R3, #1
; inc counters
	ADD R6, R6, #1
	CMP R6, #4
	BNE generateBoardInc_Y
; inc other pointer
	ADD R2, R2, #1
	ADD R5, R5, #1
	CMP R5, #4
	BNE generateBoardInc_X

	LDMFD R13!, {R4-R7, PC}

; DO NOT CHANGE ANYTHING AFTER THIS POINT...
; randu -- Generates a random number
; Input: None
; Ouptut: R0 -> Random number
randu   LDR R2,mult
        MVN R1,#0x80000000
        LDR R0,seed
        MUL R0,R2,R0
        AND R0,R0,R1
        STR R0,seed
        MOV PC, R14

        ALIGN
seed    DEFW    0xC0FFEE
mult    DEFW    65539

        ALIGN
        include printMaskedBoard.s
        ALIGN
        SWI 2
