# CHIP-8 Architecture and Emulator Design

This document explains the CHIP-8 virtual machine and how my emulator
implements it.

---

## 1. What is CHIP-8?

CHIP-8 is a simple interpreted programming language developed in the 1970s.
It was designed to make game development easier on early microcomputers.

### Key Components
- 4 KB memory
- 16 general-purpose registers (V0–VF)
- Index register (I)
- Program counter (PC)
- Stack and stack pointer
- 64×32 monochrome display
- 16-key hexadecimal keypad
- Delay and sound timers (60 Hz)

---

## 2. Memory Layout
```
0x000–0x04F: Built-in font sprites (0–F)
0x200: Program start address
```
---

## 3. Instruction Execution Cycle

Each CPU cycle:
1. Fetch opcode from memory
2. Decode opcode
3. Execute instruction
4. Update program counter

Timers run independently at 60 Hz, decoupled from CPU execution speed.

---

## 4. Graphics (DRW Opcode)

- Sprites are 8 pixels wide
- XOR drawing is used
- Collision detection sets register VF
- Screen wrapping is supported

---

## 5. Input Handling

- CHIP-8 uses a 16-key keypad
- Input is mapped to a modern keyboard
- Fx0A (wait for key) is implemented with edge-triggered key detection, preventing key-repeat and release issues.
- Timings are stabilized using SDL_Delay

---

## 6. Timers

- Delay timer decreases at 60 Hz
- Sound timer triggers a beep when non-zero

---

## 7. Challenges Faced

- Correct handling of Fx0A (key wait)
- Synchronizing CPU speed with timers
- Understanding CHIP-8 quirks
- Keyboard edge vs level triggering
- Sprite wrapping vs clipping behavior

---

## 8. Testing

The emulator was tested using:
- Timendus' CHIP-8 test suite
- IBM Logo ROM
- Keypad and timing tests

---

## 9. What I Learned

- Emulator design fundamentals
- Low-level CPU emulation concepts
- Input timing issues
- Debugging real-time systems
- Reading technical specifications

---

## 10. Future Improvements

- Better audio
- Configurable quirks
- Super CHIP-8 support
- Save states