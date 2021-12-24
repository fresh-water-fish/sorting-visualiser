	B test_BoardSquareInput

tprompt  DEFB "Enter square to reveal: ",0
tmesg    DEFB "You entered the index ",0

	ALIGN
test_BoardSquareInput
	ADR R0, tprompt
	BL boardSquareInput
	
	MOV R1, R0
	ADR R0, tmesg
	SWI 3
	MOV R0,R1
	SWI 4
	MOV R0,#10
	SWI 0
	SWI 2

; --- * --- * --- * --- * --- * ---
; boardSquareInput -- read board position from keyboard
; Input:  R0 ---> prompt string address
; Ouptut: R0 <--- index
boardSquareInput
	MOV R2, R0
	MOV R3, #0
	MOV R13, #0x5000
	STMFD R13!, {R4-R8, LR}
boardSquareCond
	MOV R0, R2
	SWI 3
	MOV R4, #0
	BL boardSquareReadIndex
	CMP R3, #0
	BNE boardSquareCond
; first char read with success, now comma
	MOV R4, R1
	MOV R1, #44
	BL boardSquareReadChar
	CMP R3, #0
	BNE boardSquareCond
; comma also read successfully, now index again
	BL boardSquareReadIndex
	CMP R3, #0
	BNE boardSquareCond
; index read successfully
	MOV R5, R1
; must wait for newline feed
	MOV R1, #10
	SWI 1
	SWI 0
	CMP R0, #10
	BLNE boardSquareCond
; success secured!
	MUL R0, R4, R5
	STMFD R13!, {R4-R8, PC}
; --- * --- * --- * --- * --- * ---
; read in the index value
; Output: R1 <-- index
;         R3 <-- bool success
boardSquareReadIndex
	STMFD R13!, {LR}
	MOV R3, #255
	SWI 1
	SWI 0
	MOV R1, R0
	CMP R0, #10
	BEQ boardSquareExitEarly
	CMP R1, #48 ; > 0
	BLE boardSquareIndexError
	CMP R1, #57 ; < 9
	MOVGE PC, LR
	MOVLE R3, #0
	SUB R1, R1, #49 ; CHAR --> INT
	LDMFD R13!, {PC}
boardSquareIndexError
	BL boardSquareWaitTillNL
	LDMFD R13!, {PC}
; --- * --- * --- * --- * --- * ---
; read in a specific character
; Input: R1 <-- char to allow
; Output: R3 <-- bool success
boardSquareReadChar
	STMFD R13!, {LR}
	MOV R3, #255
	SWI 1
	SWI 0
	CMP R0, #10
	BEQ boardSquareExitEarly
	CMP R0, R1
	MOVEQ R3, #0
	BLNE boardSquareWaitTillNL
boardSquareExitEarly
	LDMFD R13!, {PC}
; --- * --- * --- * --- * --- * ---
; will wait until a new line feed
boardSquareWaitTillNL
	SWI 1
	SWI 0
	CMP R0, #10
	BNE boardSquareWaitTillNL
	MOV PC, LR

