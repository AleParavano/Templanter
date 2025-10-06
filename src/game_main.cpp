#include <iostream>
#include "Core/GameManager.h"

int main() {
    std::cout << "\n";
    std::cout << "╔════════════════════════════════════════╗" << std::endl;
    std::cout << "║                                        ║" << std::endl;
    std::cout << "║          🌱 TEMPLANTER 🌱              ║" << std::endl;
    std::cout << "║     Plant Management Simulator         ║" << std::endl;
    std::cout << "║                                        ║" << std::endl;
    std::cout << "╚════════════════════════════════════════╝" << std::endl;
    std::cout << "\nInitializing game..." << std::endl;
    
    try {
        GameManager game;
        game.run();
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }
    
    std::cout << "\nThanks for playing! 👋" << std::endl;
    
    return 0;
}