; 2d Stickman World
; Load sprite
LD V0, 0x18     ; 00011000
LD V1, 0x7E     ; 01111110
LD V2, 0x18     ; 00011000
LD V3, 0x3C     ; 00111100
LD V4, 0x24     ; 00100100
LD I, 0x300     ; set I = 0x300
LD [I], V4      ; store sprite in RAM starting at address I
LD VA, 0x10     ; set x coordinate to 16 
LD VB, 0x10     ; set y coordinate to 16
LD I, 0x300     ; set Index register back to 0x300

; Game loop
CLS             ; clear screen
DRW VA, VB, 0x5 ; draw the sprite
LD VC, 0x1      ; store 1 in register V[C]
LD V5, K        ; wait for key press

SE V5, 0x5      ; if key == W
ADD VB, 0x1     ; move up
SE V5, 0x7      ; if key == A
ADD VA, 0x1     ; move left
SE V5, 0x8      ; if key == S
SUB VB, VC      ; move down
SE V5, 0X9      ; if key == D
SUB VA, VC      ; move right

JP 0x212        ; jump back to start of program (CLS at line 13)