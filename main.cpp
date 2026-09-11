#include "emulator.h"
#include <iostream>
#include <stdlib.h>
#include <chrono>
#include <thread>
#include <SFML/Graphics.hpp>

int main(){  
    // create SFML window
    sf::RenderWindow window(sf::VideoMode(640, 320), "CHIP-8 Emulator");
    window.setFramerateLimit(60);
    
    // create emulator object
    emulator Emulator;
    Emulator.initialize(&window);
    Emulator.loadRom();

    while(window.isOpen()){
        sf::Event event;
        while(window.pollEvent(event)){
            if(event.type == sf::Event::Closed){
                window.close();
            }
        }
        
        Emulator.emulateCycle();
        
        // Update the screen if needed
        if(Emulator.drawFlag){
            window.clear();
            Emulator.updateDisplay();
            window.display();
            Emulator.stopFlag();
        }
        
        std::this_thread::sleep_for(std::chrono::milliseconds(2));
    }

    return 0; 
}
