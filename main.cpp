#include "emulator.h"
#include <iostream>
#include <stdlib.h>
int main(){  
    // create emulator object
emulator Emulator;
Emulator.initialize();
Emulator.loadRom();
while(1){
    Emulator.emulateCycle();
    // Update the screen if needed
    if(Emulator.drawFlag){
        system("clear");
        Emulator.drawScreen(); 
    }
}
return 0; 
}