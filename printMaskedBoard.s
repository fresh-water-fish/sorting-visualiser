	B test_printMaskedBoard

; Our board 
; 0, represents an empty space
; 1-8 represents the number of bombs around us
; -1 represents there is a bomb at this location
; No more than 8 bombs
pmbBoard
	DEFW  1,-1, 1, 0, 0, 1,-1,-1, 1, 1, 1, 0, 0, 1, 3,-1, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 1,-1, 1, 1, 1, 1, 0, 0, 1, 1, 1, 1,-1, 1, 0, 0, 1, 1, 2, 2, 2, 1, 0, 0, 1,-1, 2,-1, 1, 0, 0, 0
pmbBoardMask
	DEFW  0,-1,-1,-1,-1, 0,-1, 0,-1,-1,-1,-1,-1,-1, 0,-1,-1,-1,-1,-1, 0,-1,-1,-1, 0, 0,-1,-1,-1,-1,-1,-1,-1,-1,-1, 0, 0,-1,-1,-1,-1, 0,-1, 0,-1,-1,-1,-1,-1,-1, 0,-1,-1,-1,-1,-1, 0,-1,-1,-1, 0, 0,-1,-1
        

test_printMaskedBoard
	ADR R0, pmbBoard 
	ADR R1, pmbBoardMask
	BL printMaskedBoard
	SWI 2

; printMaskedBoard -- print the board with only the squares visible when boardMask contains zero
; Input: R0 <-- Address of board
;        R1 <-- Address of board Mask

printMaskedBoard
; Insert your implementation here
 	MOV R13, #0x5000                                      
 	MOV R2, R0
	MOV R3, R1
 	STMFD R13!, {R0-R1, R4-R8, LR}
 ; Print 1-8 horizontally
 	BL printHeader
 ; Print 1-8 vertically along with each row of board
 	BL printRow
 	LDMFD R13!, {R0-R1, R4-R8, PC}
 ; --- * --- * --- * --- * --- * ---
 ; --- Print the Header Row
 ; Input: none
 printHeader
 	STMFD R13!, {LR}
 	MOV R0, #32
 	SWI 0
 	SWI 0
 	SWI 0
 	MOV R1, #0
 printHeaderCond
 	ADD R1, R1, #1
 	BL printIntSpace
 	CMP R1, #8
 	BNE printHeaderCond
 	MOV R0, #10
 	SWI 0
 	LDMFD R13!, {PC}
 ; --- * --- * --- * --- * --- * ---
 ; --- Print Each Row 
 ; Input: R2 <-- board
 ;        R3 <-- board mask
 printRow
 	STMFD R13!, {R4, LR}
 	MOV R4, #0
 printRowCond
 	MOV R0, #10
 	SWI 0
 	ADD R4, R4, #1
 	MOV R1, R4
 	BL printIntIndex
 	BL printItems
 	CMP R4, #8
 	BNE printRowCond
 	LDMFD R13!, {R4, PC}
 ; --- * --- * --- * --- * --- * ---
 ; --- Print 8 Consecuative Items 
 ; Input: R2 <-- start index of board
 ;        R3 <-- start of mask board
 printItems
 	STMFD R13!, {R4-R6, LR}
 	MOV R4, #1
 printItemsCond
 	LDR R5, [R2], #4
	LDR R6, [R3], #4
; if mask is 0 then do not print value
	CMP R6, #0
	MOVNE R1, #35
	BLNE printIntCharSpace
; otherwise print the value
 	CMP R5, #0
 	MOVEQ R1, #32
 	MOVLT R1, #77
 	MOVGT R1, R5
 	BL printIntCharSpace
 	ADD R4, R4, #1
 	CMP R4, #9
 	BNE printItemsCond
 	MOV R0, #10
 	SWI 0
 	LDMFD R13!, {R4-R6, PC}
 ; --- * --- * --- * --- * --- * ---
 ; Print an Int / Char with spacing
 ; Input: R1 <-- Int / Char
 ;        R4 <-- Count
 printIntCharSpace
 	MOV R0, #32
 	SWI 0
 	SWI 0
 	MOV R0, R1
 	CMP R1, #9
 	SWIGE 0
 	SWILT 4
 	CMP R4, #8
 	MOVEQ PC, LR
 	MOVNE R0, #32
 	SWI 0
 	SWI 0
 	MOV PC, LR
 ; --- * --- * --- * --- * --- * ---
 ; Print an Int with spacing
 ; Input: R1 <-- Int 
 printIntSpace
 	MOV R0, #32
 	SWI 0
 	SWI 0
 	MOV R0, R1
 	SWI 4
 	CMP R1, #8
 	MOVEQ PC, LR
 	MOV R0, #32
 	SWI 0
 	SWI 0
 	MOV PC, LR
 ; --- * --- * --- * --- * --- * ---
 ; Print an Int with only right spacing
 ; Input: R1 <-- Int
 printIntIndex
 	MOV R0, R1
 	SWI 4
 	MOV R0, #32
 	SWI 0
 	SWI 0
 	MOV PC, LR
 
