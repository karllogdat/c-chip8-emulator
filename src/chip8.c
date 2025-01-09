#include "chip8.h"

#include <stdio.h>
#include <string.h>

void chip8_init(chip8 *machine)
{
        // since first 512 bytes is occupied by chip8
        // memory used by programs start at 0x200
        machine->pc = 0x200;
        machine->opcode = 0;
        machine->I = 0;
        machine->sp = 0;

        memset(machine->gfx, 0, sizeof(machine->gfx));
        memset(machine->stack, 0, sizeof(machine->stack));
        memset(machine->V, 0, sizeof(machine->V));
        memset(machine->memory, 0, sizeof(machine->V));

        unsigned char fontset[80] = {
            0xF0, 0x90, 0x90, 0x90, 0xF0, // 0
            0x20, 0x60, 0x20, 0x20, 0x70, // 1
            0xF0, 0x10, 0xF0, 0x80, 0xF0, // 2
            0xF0, 0x10, 0xF0, 0x10, 0xF0, // 3
            0x90, 0x90, 0xF0, 0x10, 0x10, // 4
            0xF0, 0x80, 0xF0, 0x10, 0xF0, // 5
            0xF0, 0x80, 0xF0, 0x90, 0xF0, // 6
            0xF0, 0x10, 0x20, 0x40, 0x40, // 7
            0xF0, 0x90, 0xF0, 0x90, 0xF0, // 8
            0xF0, 0x90, 0xF0, 0x10, 0xF0, // 9
            0xF0, 0x90, 0xF0, 0x90, 0x90, // A
            0xE0, 0x90, 0xE0, 0x90, 0xE0, // B
            0xF0, 0x80, 0x80, 0x80, 0xF0, // C
            0xE0, 0x90, 0x90, 0x90, 0xE0, // D
            0xF0, 0x80, 0xF0, 0x80, 0xF0, // E
            0xF0, 0x80, 0xF0, 0x80, 0x80  // F
        };

        // since font set is loaded at 0x050
        // load font set in memory at that index above
        for (int i = 0; i < 80; i++)
        {
                machine->memory[0x050 + i] = fontset[i];
        }

        // reset timers
        machine->delay_timer = 0;
        machine->sound_timer = 0;

        printf("CHIP8 Initialized Sucessfully\n");
}

void chip8_load_program(chip8 *machine, char *program_path)
{
        // TODO: implement program loader
        // for (int i = 0; i < programBuffer.size; i++) {
        //      chip8->memory[i + 0x200] = programBuffer[i];
        // }

        // open file in binary mode
        // return when fopen returns null
        FILE *rom = fopen("../resources/programs/breakout.ch8", "rb");
        if (rom == NULL)
        {
                perror("ROM not found.\n");
                return;
        }

        // buffer size is max_memory - start_mem_location
        unsigned char buffer[0xFFF - 0x200];
        memset(buffer, 0, sizeof(buffer));

        fread(buffer, sizeof(unsigned char), sizeof(buffer), rom);

        for (int i = 0; i < sizeof(buffer); ++i)
        {
                machine->memory[i + 0x200] = buffer[i];
        }

        // close file
        fclose(rom);
}

void chip8_emulate_cycle(chip8 *machine)
{
        // pls learn bitwise operations
        // fetch opcode
        machine->opcode = machine->memory[machine->I] << 8 | machine->memory[machine->I + 1];

        // decode opcode
        switch (machine->opcode & 0xF000)
        {
        case 0xA000:
                machine->I = machine->opcode & 0x0FFF;
                // increment by 2 since opcodes are 2 bytes long
                machine->pc += 2;
                break;

        default:
                printf("Unknown opcode: 0x%X\n", machine->opcode);
                break;
        }

        // execute opcode
        // update timers
}