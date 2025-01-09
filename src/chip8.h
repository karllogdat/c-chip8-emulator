#ifndef CHIP8_H
#define CHIP8_H

// c data type size informations
// char         8 bits          1 byte
// short        16 bits         2 bytes

typedef struct chip8
{
        // stores current opcode
        unsigned short opcode;

        // memory
        unsigned char memory[4096];

        // registers
        // last register VF commonly used as carry flag
        unsigned char V[16];

        // index register and program counter
        // has values from 0x000 to OxFFF
        unsigned short I;
        unsigned short pc;

        // memory map
        // 0x000 - 0x1FF Chip8 interpreter (contains font set in emulator)
        // 0x050 - 0x0A0 Used for built in 4x5 pixel font set (0-F)
        // 0x200 - 0xFFF Program ROM and work RAM

        // The graphics system:
        // The chip 8 has one instruction that draws sprite to the screen.
        // Drawing is done in XOR mode and if a pixel is turned off as a result of drawing,
        // the VF register is set. This is used for collision detection.
        // The graphics of the Chip 8 are black and white and the screen has a total of
        // 2048 pixels (64 x 32).
        // This can easily be implemented using an array that hold the pixel state (1 or 0):
        unsigned char gfx[64 * 32];

        // timer registers that count at 60hz
        // when set above zero, they count down to zero
        unsigned char delay_timer;
        unsigned char sound_timer;

        // emulator stack
        unsigned short stack[16];
        // stack pointer
        unsigned short sp;

        // hex keypad used for input
        unsigned char key[16];
} chip8;

void chip8_init(chip8 *machine);
void chip8_emulate_cycle(chip8 *machine);
void chip8_load_program(chip8 *machine, char *program_path);

#endif