#ifndef EMULATOR_H
#define EMULATOR_H
#include <iostream>
#include <cstddef>
#include <string>
#include <SFML/Graphics.hpp>

class emulator{
    private:
        std::string inputFile;
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
        unsigned char flagVal;
        sf::RenderWindow* window;
        sf::RectangleShape pixelBuffer[64*32];
        const int PIXEL_SIZE = 10;
    public:
        bool drawFlag = false;
        void initialize(sf::RenderWindow* w);
        void loadRom();
        void emulateCycle();
        void drawScreen();
        void stopFlag();
        void updateDisplay();
};
#endif
