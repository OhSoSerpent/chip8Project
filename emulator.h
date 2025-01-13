
class emulator{
    private:
        unsigned short opcode;
        unsigned char memory[4096];
        unsigned char v[16];
        unsigned short indexReg;
        unsigned short programCounter;
        unsigned char screen[64 * 32];
        unsigned char delay_timer;
        unsigned char sound_timer;
        unsigned short stack[16];
        unsigned short stackPointer;
        unsigned char currKey[16];
    public:
        bool drawFlag = false;
    void initialize(){
        // Initialize registers and memory
        programCounter = 0x200;
        opcode = 0;
        indexReg = 0;
        stackPointer = 0;
    }
    void emulateCycle(){
        //Fetch
        opcode = memory[pc] << 8 | memory[pc] + 1;
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
            case 0xD000:
            {
                unsigned short x = v[(opcode & 0x0F00) >> 8];
                unsigned short y = v[(opcode & 0x00F0) >> 4];
                unsigned short height = opcode & 0x000F;
                unsigned short pixel;

            V[0xF] = 0;
            for (int yline = 0; yline < height; yline++)
            {
                pixel = memory[indexReg + yline];
                for(int xline = 0; xline < 8; xline++)
                {
                    if((pixel & (0x80 >> xline)) != 0)
                    {
                        if(screen[(x + xline + ((y + yline) * 64))] == 1)
                        {
                            V[0xF] = 1;
                        }
                        screen[x + xline + ((y + yline) * 64)] ^= 1;
                    }
                }
            }

            drawFlag = true;
            pc += 2;
            }
        }
    }
}