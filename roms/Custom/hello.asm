; --- Load 'H' (5 Bytes) at 0x300 ---
LD I, 0x300     ; Point I to 0x300
LD V0, 0x90     ; Row 1
LD V1, 0x90     ; Row 2
LD V2, 0xF0     ; Row 3 (Crossbar)
LD V3, 0x90     ; Row 4
LD V4, 0x90     ; Row 5
LD [I], V4      ; Write V0-V4 to memory

; --- Load 'E' (5 Bytes) at 0x305 ---
LD I, 0x305     ; Point I to 0x305
LD V0, 0xF0
LD V1, 0x80
LD V2, 0xF0
LD V3, 0x80
LD V4, 0xF0
LD [I], V4

; --- Load 'L' (5 Bytes) at 0x30A ---
LD I, 0x30A
LD V0, 0x80
LD V1, 0x80
LD V2, 0x80
LD V3, 0x80
LD V4, 0xF0
LD [I], V4

; --- Load 'L' (5 Bytes) at 0x30F ---
LD I, 0x30F
LD V0, 0x80
LD V1, 0x80
LD V2, 0x80
LD V3, 0x80
LD V4, 0xF0
LD [I], V4

; --- Load 'O' (5 Bytes) at 0x314 ---
LD I, 0x314
LD V0, 0xF0
LD V1, 0x90
LD V2, 0x90
LD V3, 0x90
LD V4, 0xF0
LD [I], V4

; INITIALIZATION
CLS             ; Clear screen
LD V0, 0x0      ; V0 = X Position
LD V1, 0xC      ; V1 = Y Position
LD V2, 0x0      ; V2 = Direction (0 = Right, 1 = Left)
LD V3, 0x6      ; V3 = Speed (Timer Value)
LD DT, V3       ; Set Delay Timer
LD VA, 0x1      ; VA = 1 (Used for subtraction later)

; SECTION 3: MAIN LOOP (Start Address = 0x254)
; --- Timer Gate (Wait for frame) ---
LD V4, DT       ; Read Timer
SE V4, 0x0      ; Skip Jump if Timer is 0
JP 0x254        ; Jump back to start of loop (Wait)

LD DT, V3       ; Reset Timer
CLS             ; Clear Screen

; --- Draw "HELLO" ---
LD V5, V0       ; Copy X (V0) to Temp (V5)

; Draw H
LD I, 0x300     ; Point to 'H'
DRW V5, V1, 0x5 ; Draw 5 pixels tall
ADD V5, 0x6     ; Move Temp X right by 6
; Draw E
LD I, 0x305
DRW V5, V1, 0x5
ADD V5, 0x6
; Draw L
LD I, 0x30A
DRW V5, V1, 0x5
ADD V5, 0x6
; Draw L
LD I, 0x30F
DRW V5, V1, 0x5
ADD V5, 0x6
; Draw O
LD I, 0x314
DRW V5, V1, 0x5

; --- Move Logic ---
; Check Direction Right (0)
SE V2, 0x1      ; Skip next if Dir is 1 (Left)
ADD V0, 0x1     ; Move Right (X++)

; Check Direction Left (1)
SE V2, 0x0      ; Skip next if Dir is 0 (Right)
SUB V0, VA      ; Move Left (X = X - VA). VA is 1.

; --- Bounce Logic ---
; Right Bound Check (64 - 40 = 24 -> 0x18)
SE V0, 0x18     ; Skip if X != 24
LD V2, 0x1      ; Hit wall! Set Dir = Left

; Left Bound Check
SE V0, 0x0      ; Skip if X != 0
LD V2, 0x0      ; Hit wall! Set Dir = Right

; Loop Forever
JP 0x254        ; Jump back to Timer Gate