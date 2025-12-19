; SETUP: CREATE THE "+" SPRITE IN MEMORY

LD I, 0x300     ; Point Index Register I to safe memory address 0x300
LD V0, 0x10     ; Row 1: 00010000 (Binary) -> Middle dot
LD V1, 0x10     ; Row 2: 00010000 (Binary) -> Middle dot
LD V2, 0x7C     ; Row 3: 01111100 (Binary) -> Horizontal Line (The cross)
LD V3, 0x10     ; Row 4: 00010000 (Binary) -> Middle dot
LD V4, 0x10     ; Row 5: 00010000 (Binary) -> Middle dot
LD [I], V4      ; Dump registers V0-V4 into memory at 0x300.

CLS             ; Clear screen to start fresh

; STEP 1: GET FIRST NUMBER (0-9)
LD V0, K        ; Wait for Key Press -> Store in V0 (Let's say user presses '9')
LD F, V0        ; Set I to the built-in Font Sprite for the number in V0
LD VA, 0xA       ; Set X coordinate (VA) to 10
LD VB, 0xA       ; Set Y coordinate (VB) to 10
DRW VA, VB, 0x5   ; Draw the number (5 pixels tall)

; STEP 2: GET "A" (+) AND DRAW PLUS SIGN
LD V1, K        ; Wait for Key (Expect User to press 'A')
LD I, 0x300     ; Point I to our custom "+" sprite we made earlier
ADD VA, 0x8       ; Move Cursor Right (X + 8 pixels)
DRW VA, VB, 0x5   ; Draw the custom "+" sprite

; STEP 3: GET SECOND NUMBER (0-9)
LD V1, K        ; Wait for Key -> Store in V1 (Let's say user presses '9')
LD F, V1        ; Set I to Font Sprite for number in V1
ADD VA, 0xA       ; Move Cursor Right (X + 10 pixels)
DRW VA, VB, 0x5   ; Draw the number

; STEP 4: GET "B" (=) AND CALCULATE
LD V2, K        ; Wait for Key (Expect User to press 'B')
CLS             ; Clear the screen (As per your request)
ADD V0, V1      ; Add V1 to V0. (9 + 9 = 18). V0 now holds 18.

LD I, 0x400     ; Point I to a safe memory location (0x400)
LD B, V0        ; (BCD) Take value in V0 (18), split it into digits,
; and store them at I, I+1, I+2.
; Memory[0x400] = 0 (Hundreds)
; Memory[0x401] = 1 (Tens)
; Memory[0x402] = 8 (Units)

LD V2, [I]      ; Read memory back into registers V0, V1, V2
; V0 now holds 0 (Hundreds)
; V1 now holds 1 (Tens)
; V2 now holds 8 (Units)

LD F, V1        ; Load Font for Tens digit
LD VA, 0x14       ; Set X to 20 (Center screen roughly)
LD VB, 0xA       ; Set Y to 10
SE V1, 0x0        ; SKIP next instruction if Tens digit is 0 (Don't draw '0'4, just '4')
DRW VA, VB, 0x5   ; Draw the Tens digit
ADD VA, 0x6       ; Move X slightly right
LD F, V2        ; Load Font for Units digit
DRW VA, VB, 0x5   ; Draw the Units digit

; DONE. PROGRAM LOOPS OR STOPS.
LD V0, K        ; Wait for key (Pause so you can see the result)
JP 0x200        ; Jump back to start (Restart calculator)