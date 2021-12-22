	B test_printBoard

; Our board 
; 0, represents an empty space
; 1-8 represents the number of bombs around us
; -1 represents there is a bomb at this location
; No more than 8 bombs
tpb_board   DEFW  1,-1, 1, 0, 0, 1,-1,-1, 1, 1, 1, 0, 0, 1, 3,-1, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 1,-1, 1, 1, 1, 1, 0, 0, 1, 1, 1, 1,-1, 1, 0, 0, 1, 1, 2, 2, 2, 1, 0, 0, 1,-1, 2,-1, 1, 0, 0, 0

	ALIGN
test_printBoard    
  ADR R0, tpb_board 
  BL printBoard
  SWI 2

; printBoard -- print the board 
; Input: R0 <-- Address of board
printBoard
	MOV R13, #0x10000
	MOV R3, R0
	STMFD R13!, {R0, R4-R8, LR}
; Print 1-8 horizontally
	BL printHeader
; Print 1-8 vertically along with each row of board
	BL printLines
	LDMFD R13!, {R0, R4-R8, PC}

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
	MOV R0, #0
	SWI 0
	LDMFD R13!, {PC}

; --- * --- * --- * --- * --- * ---
; --- Print Each Row 
; Input: R3 <-- board
printRow
	STMFD R13!, {R4-R8, LR}
	MOV R4, #0
	MOV R5, R3
	MOV R6, #0
printRowCond
	MOV R6, R4, LSL #3
	ADD R4, R4, #1
	MOV R1, R4
	BL printIntIndex
	BL printItems
	CMP R4, #8
	BNE printRowCond
	LDMFD R13!, {R4-R8, PC}

; --- * --- * --- * --- * --- * ---
; --- Print 8 Consecuative Items 
; Input: R2 <-- start index
;        R3 <-- board
printItems
	STMFD R13!, {R4-R8, LR}
	MOV R4, #1
printItemsCond
	LDR R5, [R2], #4
	CMP R5, #0
	MOVEQ R1, #32
	MOVLT R1, #77
	MOVGT R1, R5
	BL printIntCharSpace
printItemPrinted
	ADD R4, R4, #1
	CMP R4, #9
	BNE printItemsCond
	MOV R0, #10
	SWI 0
	LDMFD R13!, {R4-R8, PC}

; --- * --- * --- * --- * --- * ---
; Print an Int / Char with spacing
; Input: R1 <-- Int / Char
;        R4 <-- Count
printIntCharSpace
	MOV R0, #32
	SWI 0
	SWI 0
	MOV R0, R1
	CMP R1, #0
	SWIGT 4
	SWILE 0
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

