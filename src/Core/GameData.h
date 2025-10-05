#ifndef GAMEDATA_H
#define GAMEDATA_H

#include <string>
#include <vector>
#include "json.hpp"

using json = nlohmann::json;

// Forward declarations
struct PlantData;
struct WorkerData;
struct InventoryData;
struct StatisticsData;

// Plant data structure
struct PlantData {
    int id;
    std::string type;
    int posX, posY;
    std::string state;
    int growthProgress;
    int waterLevel;
    long lastWateredTime;
    
    json toJson() const;
    static PlantData fromJson(const json& j);
};

// Worker data structure
struct WorkerData {
    int id;
    std::string name;
    int wateringSkill;
    int harvestingSkill;
    int cashierSkill;
    int securitySkill;
    int overallLevel;
    int experience;
    double salary;
    std::string currentRole;
    int assignedZone;
    
    json toJson() const;
    static WorkerData fromJson(const json& j);
};

// Inventory data structure
struct InventoryData {
    std::vector<std::pair<std::string, int>> harvestedPlants;  // plantType, quantity
    std::vector<std::pair<std::string, int>> seeds;            // seedType, quantity
    int nutrients;
    int storageCapacity;
    
    json toJson() const;
    static InventoryData fromJson(const json& j);
};

// Statistics data structure
struct StatisticsData {
    int totalPlantsGrown;
    double totalMoneyEarned;
    int totalCustomersServed;
    int totalRobberiesStopped;
    std::vector<std::string> achievementsUnlocked;
    
    json toJson() const;
    static StatisticsData fromJson(const json& j);
};

#endif // GAMEDATA_H