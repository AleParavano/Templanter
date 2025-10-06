#include "GameData.h"
#include <iostream>
#include <sstream>
#include <iomanip>

GameData::GameData() {
    generateTimestamp();
    generateSaveName();
}

void GameData::generateSaveName() {
    std::stringstream ss;
    ss << "Day " << time.day << " - $" << static_cast<int>(economy.currency);
    saveName = ss.str();
}

void GameData::generateTimestamp() {
    std::time_t now = std::time(nullptr);
    std::stringstream ss;
    ss << std::put_time(std::localtime(&now), "%Y-%m-%d %H:%M:%S");
    timestamp = ss.str();
}

void GameData::printSummary() const {
    std::cout << "\n╔════════════════════════════════════════╗" << std::endl;
    std::cout << "║         GAME STATE SNAPSHOT            ║" << std::endl;
    std::cout << "╚════════════════════════════════════════╝" << std::endl;
    std::cout << "Name: " << saveName << std::endl;
    std::cout << "Time: " << timestamp << std::endl;
    std::cout << "\nDay " << time.day << " | " 
              << time.hour << ":" << std::setfill('0') << std::setw(2) << time.minute << std::endl;
    std::cout << "Money: $" << economy.currency << " | Rating: " << economy.rating << "/5" << std::endl;
    std::cout << "Greenhouse: " << greenhouse.width << "x" << greenhouse.height 
              << " (" << greenhouse.plants.size() << " plants)" << std::endl;
    std::cout << "Workers: " << workers.size() << std::endl;
    std::cout << "Stats: " << statistics.totalPlantsGrown << " grown, "
              << statistics.totalCustomersServed << " customers served" << std::endl;
}