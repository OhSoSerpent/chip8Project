#ifndef EMULATOR_H
#define EMULATOR_H
#include <iostream>
#include <cstddef>
class emulator{
    private:
        unsigned short opcode;
        unsigned char memory[4096];
        unsigned char v[16];
        unsigned short indexReg;
        unsigned short programCounter;
        unsigned char screen[64*32];
        unsigned char delay_timer;
        unsigned char sound_timer;
        unsigned short stack[16];
        unsigned short stackPointer;
        unsigned char currKey[16];
    public:
        bool drawFlag = false;
    void initialize();
    void loadRom();
    void emulateCycle();
    void drawScreen();
};
#endif