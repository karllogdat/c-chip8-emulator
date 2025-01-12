#include "chip8.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>

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
        // 1NNN
        // goto NNN
        // jumps to address NNN
        case 0x1000:
                // get 12-bit address (NNN) from opcode by bit masking
                machine->pc = machine->opcode & 0x0FFF;
                break;

        // ANNN
        // I = NNN
        case 0xA000:
                machine->I = machine->opcode & 0x0FFF;
                // increment by 2 since opcodes are 2 bytes long
                machine->pc += 2;
                break;

        // BNNN
        // PC = V0 + NNN
        case 0xB000:
                // extract 12-bit address from opcode
                unsigned int address = machine->opcode & 0x0FFF;

                // move program counter to specified address
                machine->pc = machine->V[0] + address;

                if (machine->pc > 0xFFF)
                {
                        perror("Overflow Error: PC out of memory bounds");
                        // wrap around the memory
                        // this actually might access the reserved memory...
                        machine->pc &= 0xFFF;
                }
                break;

        // CXNN
        // Vx = rand() & NN
        case 0xC000:
                // extract 8-bit address from opcode (NN)
                unsigned char value = machine->opcode & 0x00FF;
                // extract register index from opcode
                unsigned char reg = machine->opcode >> 8 & 0x0F;

                // generate random num from 0-255 and operate bitwise AND
                // to value (NN)
                unsigned char rand_num = rand() % 256;
                machine->V[reg] = rand_num & value;

                // move program counter
                machine->pc += 2;
                break;

        default:
                printf("Unknown opcode: 0x%X\n", machine->opcode);
                break;
        }

        // execute opcode
        // update timers
}