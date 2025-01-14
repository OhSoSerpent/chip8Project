#include "emulator.h"
#include <fstream>
#include <cstddef>
#include <iostream>
#include <vector>
void emulator::loadRom(){
    // reads file into stream, finds the beginning and sets the pointer there
    std::cout << "reading" << std::endl;
    std::ifstream file("IBM Logo.ch8", std::ios::binary);
    if(!file.is_open()){
        std::cout << "Not working!" << std::endl;
    }
    file.seekg(0, std::ios::end);
    std::streamsize size = file.tellg();
    file.seekg(0, std::ios::beg);
    // reads stream into array
    std::vector<char> buffer(size);
    file.read(buffer.data(), size);
    file.close();
    for(int i =0; i < size; ++i){
        memory[i+512] = buffer[i];
    }
}
void emulator::emulateCycle(){
        //Fetch
        opcode = memory[programCounter] << 8 | memory[programCounter + 1];
        //Decode and execute
        switch(opcode & 0xF000){
            case 0xA000:    // Set index to the address NNN
                indexReg = opcode & 0x0FFF;
                programCounter += 2;
            break;
            case 0x0000:    // Switch case for zeroes on the nibble
                switch(opcode & 0x0FFF){
                    case 0x00E0:    // Clear the screen
                        std::fill(std::begin(screen), std::end(screen), 0);
                        drawFlag = true;
                        programCounter += 2;
                    break;
                    case 0x00EE:    // Return from subroutine
                        --stackPointer;
                        programCounter = stack[stackPointer];
                    break;
                }
            break;
            case 0x1000:    // Jump to given location
                programCounter = opcode & 0x0FFF;
            break;
            case 0x2000:    // Call subroutine at location NNN
                stack[stackPointer] = programCounter;
                ++stackPointer;
                programCounter = opcode & 0x0FFF;
            break;
            case 0x6000: // Set virtual register to given value
                v[(opcode & 0x0F00) >> 8] = opcode & 0x00FF;
                programCounter += 2;
            break;
            case 0x7000:    // Add given value to virtual register
                v[(opcode & 0x0F00) >> 8] += opcode & 0x00FF;
                programCounter += 2;
            break;
            case 0xD000:    // Draw to screen
            {
                std::cout << "Drawing!" << std::endl;
                unsigned short x = v[(opcode & 0x0F00) >> 8];
                unsigned short y = v[(opcode & 0x00F0) >> 4];
                unsigned short height = opcode & 0x000F;
                unsigned short pixel;

            v[0xF] = 0;
            for (int yline = 0; yline < height; yline++)
            {
                pixel = memory[indexReg + yline];
                for(int xline = 0; xline < 8; xline++)
                {
                    if((pixel & (0x80 >> xline)) != 0)
                    {
                        if(screen[(x + xline + ((y + yline) * 64))] == 1)
                        {
                            v[0xF] = 1;
                        }
                        screen[x + xline + ((y + yline) * 64)] ^= 1;
                    }
                }
            }

            drawFlag = true;
            programCounter += 2;
            }
        }
    }
    void emulator::initialize(){
        std::cout << "Initializing!" << std::endl;
        // Initialize registers and memory
        programCounter = 0x200;
        opcode = 0;
        indexReg = 0;
        stackPointer = 0;
    }
    void emulator::drawScreen(){
        for(int y = 0; y < 32; y++){
            for(int x = 0; x < 64; x++){
                if(screen[(y*64) + x] == 0){
                    std::cout << "□";
                } else {
                    std::cout << "■";
                }
                
            }
            std::cout << std::endl;
        }
    }