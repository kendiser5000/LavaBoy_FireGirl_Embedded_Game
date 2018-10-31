; Print.s
; Student names: change this to your names or look very silly
; Last modification date: change this to the last modification date or look very silly
; Runs on LM4F120 or TM4C123
; EE319K lab 7 device driver for any LCD
;
; As part of Lab 7, students need to implement these LCD_OutDec and LCD_OutFix
; This driver assumes two low-level LCD functions
; ST7735_OutChar   outputs a single 8-bit ASCII character
; ST7735_OutString outputs a null-terminated string 



    IMPORT   ST7735_OutChar
    IMPORT   ST7735_OutString
    EXPORT   LCD_OutDec
    EXPORT   LCD_OutFix
		
	
    AREA    |.text|, CODE, READONLY, ALIGN=2
    THUMB
		
INVALID DCB "*.***\0"
	
;-----------------------LCD_OutDec-----------------------
; Output a 32-bit number in unsigned decimal format
; Input: R0 (call by value) 32-bit unsigned number
; Output: none
; Invariables: This function must not permanently modify registers R4 to R11
;BINDING PHASE
NUM EQU 4		;Bind Num
FACTOR EQU 0	;Bind Factor
TEN EQU 10

LCD_OutDec

	PUSH{R4-R10, LR}
	PRESERVE8
	;R0 HAS NUM, USE LOCAL VARIABLE
	;ALLOCATION PHASE
	PUSH {R0}		;R0 = input, push on stack
	SUB SP, #4		;Allocate space for Factor
	;ACCESS PHASE
	CMP R0, #0
	BEQ Zero
	MOV R1, #0
	LDR R2, =10
Count_Length
	CMP R0, #0
	BEQ Prep_Factor
	ADD R1, #1
	UDIV R0, R2
	B Count_Length

Prep_Factor
	MOV R3, #1
	
Create_Factor
	CMP R1, #1
	BEQ Prep_Print
	MUL R3, R2
	SUB R1, #1
	B Create_Factor
	
Prep_Print
	STR R3, [SP, #FACTOR]	;Declare factor/ store value into factor
	LDR R2, [SP, #FACTOR]	;Load factor into R2
Print
	CMP R2, #0
	BEQ FIN
	LDR R3, [SP, #NUM]		;Load Num
	UDIV R0, R3, R2
	MUL R1, R0, R2
	SUB R3, R1
	LDR R4, =10
	UDIV R2, R4
	STR R2, [SP, #FACTOR]	;Update Factor
	ADD R0, #0x30
	STR R3, [SP, #NUM]		;Update Num
	BL ST7735_OutChar
	LDR R2, [SP, #FACTOR]	;Load Factor
	B Print
	
	
Zero
	ADD R0, #0x30
	BL ST7735_OutChar
	
FIN		
	;DEALLOCATION PHASE
	ADD SP, #8
	POP{R4-R10, PC}
;* * * * * * * * End of LCD_OutDec * * * * * * * *

; -----------------------LCD _OutFix----------------------
; Output characters to LCD display in fixed-point format
; unsigned decimal, resolution 0.001, range 0.000 to 9.999
; Inputs:  R0 is an unsigned 32-bit number
; Outputs: none
; E.g., R0=0,    then output "0.000 "
;       R0=3,    then output "0.003 "
;       R0=89,   then output "0.089 "
;       R0=123,  then output "0.123 "
;       R0=9999, then output "9.999 "
;       R0>9999, then output "*.*** "
; Invariables: This function must not permanently modify registers R4 to R11

;BINDING PHASE
LCD_OutFix
MAX EQU 9999	; MAX NUMBER
POINT EQU 0x2E ; DOT IN ASCII
DELTA EQU 1000 ; DIVIDING FACTOR
;FACTOR EQU 0	;Bind Factor
FIXED EQU 4		;Bind FIXED
	;R0 HAS NUM, USE LOCAL VARIABLE
	PUSH{R4, LR}
	LDR R1, =MAX
	CMP R0, R1
	BHI EXCEED_MAX
	PRESERVE8
	PUSH{R0}			;Save num in R1
	LDR R2, =DELTA		;Print the first number
	UDIV R0, R2
	ADD R0, #0x30
	BL ST7735_OutChar	
	LDR R0, =POINT		;Print the decimal point
	BL ST7735_OutChar	
	LDR R2, =DELTA
	POP{R3}				
	UDIV R0, R3, R2
	MUL R0, R2
	SUB R3, R0
;Create local var Factor
;ALLOCATION PHASE
	PUSH {R3}			;R3 = Fixed
	SUB SP, #4			;Allocate Factor
	LDR R2, =100
	STR R2, [SP, #FACTOR]	;Declare Factor/ store value to factor
Print_Fixed
	CMP R2, #0
	BEQ FIN_FIXED
	LDR R3, [SP, #FIXED]	;Load Fixed
	UDIV R0, R3, R2
	MUL R1, R0, R2
	SUB R3, R1
	LDR R4, =10
	UDIV R2, R4
	STR R2, [SP, #FACTOR]	;Updated Factor
	ADD R0, #0x30
	STR R3, [SP, #FIXED]	;Updated Fixed
	BL ST7735_OutChar
	LDR R2, [SP, #FACTOR]	;Load Factor
	B Print_Fixed
EXCEED_MAX
	LDR R0, =INVALID
	BL ST7735_OutString
	SUB SP, #8
FIN_FIXED
	;DEALLOCATION PHASE
	ADD SP, #8
	POP{R4, PC}
     ALIGN
;* * * * * * * * End of LCD_OutFix * * * * * * * *

     ALIGN                           ; make sure the end of this section is aligned
     END                             ; end of file
