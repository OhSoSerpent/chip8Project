#include "emulator.h"
#include <fstream>
#include <cstddef>
#include <iostream>
#include <vector>
#include <cstdlib>
#include <iomanip>
#include <cmath>
#include <string>
// Third-party library for keyboard-based events
#include "colors.h"


void emulator::loadRom(){
    std::cout << "File name? \n";
    std::getline(std::cin, inputFile);
    // reads file into stream, finds the beginning and sets the pointer there
    std::cout << "reading" << std::endl;
    std::ifstream file(inputFile, std::ios::binary);
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
        unsigned char x = (opcode & 0x0F00u) >> 8u;
        unsigned short nn = (opcode & 0x00FFu);
        unsigned short nnn = (opcode & 0x0FFFu);
        unsigned char y = (opcode & 0x00F0u) >> 4u;
        programCounter += 2;
        //Decode and execute
        switch(opcode & 0xF000){
            case 0x7000:    // Add given value to virtual register
                v[x] += nn;
            break;
            case 0xA000:    // Set index to the address NNN
            {
                indexReg = nnn;
            break;
            }
            case 0xB000:    // Jump to address + v[0]
            {
                indexReg = (nnn + v[0]);
            break;
            }
            case 0xC000:         // Generates a random number, binary ANDs it with nn, then stores it in X
            {
                int random = rand();
                v[x] = (nn & random);
            break;
            }
            case 0xE000:   
            {
                switch(opcode & 0x000F){
                    case 0x000E:    // Skip next instruction if holding given key
                    {
                        set_raw_mode(true);
                        int ch = quick_read();
                        if(ch == currKey[v[x]]){
                            programCounter += 2;
                        } else {
                        }
                    break;
                    }
                    case 0x0001:    //Skip next instruction if not holding given key
                    {
                        set_raw_mode(true);
                        int ch = quick_read();
                        if(ch != currKey[v[x]]){
                            programCounter += 2;
                        }   else{
                        }
                    break;
                    }
                }
            }
            case 0xF000:   
            {
                switch(opcode & 0x00F0){
                    case 0x0010:
                    {
                        indexReg += v[x]; // Add v[x] to the index register, set v[15] if it overflows
                        
                        if(indexReg > 255){
                            v[15] = 1;
                        }
                    break;
                    }
                    case 0x0020:   // Index Register is set to the address of character in v[x]
                    {
                        indexReg = v[x] * 0.5;
                    break;
                    }
                    case 0x0030 :   // Take v[x], convert it to 3 decimals, and store each in memory
                    {
                        int newNum = v[x];
                        int smallerNum = newNum / 10;
                        memory[indexReg + 2] = int(newNum % 10);
                        memory[indexReg + 1] = smallerNum % 10;
                        memory[indexReg] = int(newNum / 100);
                    break;
                    }
                    case 0x0050:   // Load V into memory
                    {
                        for(int i = 0; i < x + 1; ++i){
                            memory[indexReg + i] = v[i];
                        }
                    break;
                    }
                    case 0x0060: // Load memory into V
                    {
                        for(int i = 0; i < x + 1; i++){
                            v[i] = memory[indexReg + i];
                        }
                    break;
                    }
                } 
            break;
            }
            case 0x0000: {   // Switch case for zeroes on the nibble
                switch(nnn){
                    case 0x00E0:    // Clear the screen
                    {
                        for(int i = 0; i <= 2048; i++){
                            screen[i] = 0;
                        }
                        drawFlag = true;
                    break;
                    }
                    case 0x00EE:    // Return from subroutine
                    {
                        --stackPointer;
                        programCounter = stack[stackPointer];
                    break;
                    }
                }
            break;
            }
            case 0x1000:    // Jump to given location
            {
                programCounter = nnn;
            break;
            }
            case 0x2000:    // Call subroutine at location NNN
            {
                stack[stackPointer] = programCounter;
                ++stackPointer;
                programCounter = nnn;
            break;
            }
            case 0x3000:    // Skips next instruction if v[x] equals NN, 0x3XNN
            {
                if(v[x] == nn){
                    programCounter += 2;
                } else {
                }
            break;
            }
            case 0x4000:    //Skip next instruction if v[x] is not equal
                if(v[x] != nn){
                    programCounter += 2;
                }   else {
                }
            break;
            case 0x5000:    // Skips instruction if v[x] equals v[y], 0x5XY0
                if(v[x] == v[y]){
                    programCounter += 2;
                }   else {
                }
            break;
            case 0x6000: // Set virtual register to given value
                v[x] = nn;
            break;
            case 0x8000:    //switch case to find which instruction
                v[15] = 1;
                switch(opcode & 0x000F){
                   
                    case 0x0000:    // Self explanatory
                        v[x] = v[y];
                    break;
                    case 0x0001:    // Set V[X] equal to binary OR
                        
                        v[x] |= v[y];
                    break;
                    case 0x0002:    // Set V[X] equal to binary AND
                        v[x] &= v[y];
                    break;
                    case 0x0003:    // Set V[X] equal to binary XOR
                        v[x] ^= v[y];
                    break;
                    case 0x0004:    // Add V[Y] to V[X], setting V[15] to 1 if larger than 255
                        v[15] = (v[x] + v[y] > 0xFF) ? 1 : 0;
                        v[x] += v[y];
                    break;
                    case 0x0005:    // Subtract v[y] from v[x]
                        if(v[x] < v[y]){
                            v[15] = 1;
                        } else{
                            v[15] = 0;
                        }   
                        v[x] = v[x] - v[y];
                    break;
                    case 0x0006:    // set v[x] = v[y], then shift the bit one to the left/right and store the shifted bit in v[15]
                        v[x] = v[y];
                        v[15] = v[x] & 1;
                        v[x] = v[x] >> 1;
                    break;
                    case 0x000E: // See above
                        v[x] = v[y];
                        v[15] = v[x] & 1;
                        v[x] = v[x] << 1; 
                    break;
                    case 0x0007:    //Same as before, but switch the numbers
                        if(v[x] > v[y]){
                            v[15] = 1;
                        } else{
                            v[15] = 0;
                        }
                        v[x] = v[y] - v[x];
                    break;
                }
            break;
            case 0x9000:    // Skips instruction if v[x] != v[y], 0x9XY0
                if(v[x] == v[(opcode & 0x00F0) >> 2]){
                    programCounter += 2;
                } else {
                }
            break;
            case 0xD000:    // Draw to screen
            {
            int xScreen = v[x];
            int yScreen = v[y];
            int ht = opcode & 0x000F; //N
            int wt = 8;
            v[0x0F] = 0;

            for (int i = 0; i < ht; i++)
            {
                int pixel = memory[indexReg + i];
                for (int j = 0; j < wt; j++)
                {
                    if ((pixel & (0x80 >> j)) != 0)
                    {
                        int index = ((xScreen + j) % 64) + (((yScreen + i) % 32) * 64);
                        if (screen[index] == 1)
                        {
                            v[0x0F] = 1;
                        }
                        screen[index] ^= 1;
                    }
                }
            }
            drawFlag = true;
            }
            break;
        }
    }
    void emulator::stopFlag(){
        drawFlag = false;
    }
    void emulator::initialize(){
        std::cout << "Initializing!" << std::endl;
        // Initialize registers and memory
        unsigned char font[80] = {
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
        programCounter = 512; //The first 512 bytes(?) of memory are consumed by fonts
        opcode = 0;
        indexReg = 0;
        stackPointer = 0;
        for(int i = 0; i < 4096; i++){
            memory[i] = 0;
        }
        // Load fontset into memory
        for(int i = 0; i < 80; i++){
            memory[i] = font[i];
        }
        // Set vregister to 0
        for(int i = 0; i < 16; i++){
            v[i] = 0;
            stack[i] = 0;
            currKey[i] = 0;
        }

    }
    void emulator::drawScreen(){
        for(int y = 0; y < 32; y++){
            for(int x = 0; x < 64; x++){
                if(screen[(y*64) + x] == 0){
                    std::cout << " ";
                } else {
                    std::cout << "■";
                }
                
            }
            std::cout << std::endl;
        }
    }