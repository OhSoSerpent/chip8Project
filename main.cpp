#include "emulator.h"
#include <iostream>
#include <stdlib.h>
int main(){  
emulator Emulator;
Emulator.initialize();
Emulator.loadRom();
while(1){
    Emulator.emulateCycle();
    if(Emulator.drawFlag){
        system("clear");
        Emulator.drawScreen(); 
    }
}
return 0; 
}