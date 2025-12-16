def emit(op):
    rom.append((op >> 8) & 0xFF)
    rom.append(op & 0xFF)

rom = []

# ----------------------------
# Init (0x200)
# ----------------------------

emit(0x00E0)        # CLS
emit(0x6000)        # V0 = x
emit(0x610C)        # V1 = y
emit(0x6200)        # V2 = dir (0 = right, 1 = left)
emit(0x6306)        # V3 = speed
emit(0xF315)        # DT = V3

loop = 0x200 + len(rom)

# ----------------------------
# Timer gate
# ----------------------------
emit(0xF407)        # V4 = DT
emit(0x3400)        # if V4 == 0
emit(0x1200 | loop)

emit(0xF315)        # DT = V3
emit(0x00E0)        # CLS

# ----------------------------
# Draw HELLO
# ----------------------------
emit(0x8500)        # V5 = V0 (temp X)

emit(0xA300)        # H
emit(0xD515)
emit(0x7506)

emit(0xA305)        # E
emit(0xD515)
emit(0x7506)

emit(0xA30A)        # L
emit(0xD515)
emit(0x7506)

emit(0xA30F)        # L
emit(0xD515)
emit(0x7506)

emit(0xA314)        # O
emit(0xD515)

# ----------------------------
# Move logic
# ----------------------------

# if dir == right
emit(0x3201)        # SE V2, 1
emit(0x7001)        # V0 += 1

# if dir == left
emit(0x3200)        # SE V2, 0
emit(0x80A5)        # V0 -= 1  (SUB V0, VA where VA=1)

# Right bound (64 - 40 = 24)
emit(0x3018)        # SE V0, 24
emit(0x6201)        # dir = left

# Left bound
emit(0x3000)        # SE V0, 0
emit(0x6200)        # dir = right

emit(0x1200 | loop)

# ----------------------------
# Sprite data @ 0x300
# ----------------------------
while len(rom) < (0x300 - 0x200):
    rom.append(0x00)

sprites = [
    0x90,0x90,0xF0,0x90,0x90,  # H
    0xF0,0x80,0xF0,0x80,0xF0,  # E
    0x80,0x80,0x80,0x80,0xF0,  # L
    0x80,0x80,0x80,0x80,0xF0,  # L
    0xF0,0x90,0x90,0x90,0xF0   # O
]

rom.extend(sprites)

with open("hello.ch8", "wb") as f:
    f.write(bytes(rom))

print("Generated hello.ch8")