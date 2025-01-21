#include "emulator.h"
#include <iostream>
#include <stdlib.h>
#include <chrono>
#include <thread>
int main(){  
    // create emulator object
emulator Emulator;
Emulator.initialize();
Emulator.loadRom();

while(1){
    Emulator.emulateCycle();
    std::this_thread::sleep_for(std::chrono::milliseconds(50));
    // Update the screen if needed
    if(Emulator.drawFlag){
        system("clear");
        Emulator.drawScreen(); 
        Emulator.drawFlag = false;
    }
}
return 0; 
}